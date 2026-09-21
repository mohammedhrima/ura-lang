# config.sh - sourced workflow for building and testing ura-lang.
#
#   source config.sh
#
# defines these shell commands:
#   build    rebuild build/ura
#   copy <ura_file> <md_prefix> <description>
#            snapshot <ura_file> into tests/<md_prefix>.md; an entry with the
#            same description is replaced in place, otherwise one is appended
#   replace <ura_file> <number> <md_prefix>
#            re-record test <number>, keeping its description
#   tests [md_prefix] [number]
#            rebuild first (stop if that fails), then run all tests, one
#            file's tests, or a single test; compares the generated LLVM IR
#            against what was recorded
#   extension
#            package the VS Code extension (../ura-vscode-extension) and
#            install it into VS Code
#   update   re-source this file after editing it
#
# Works when sourced from either bash or zsh. Everything else here is an
# internal helper, prefixed _ura_ so it doesn't collide with your shell.
#
# Depends on `ura <dir>/<name>.ura -o <path>` writing its LLVM IR to
# <dir>/build/<name>.ll. If that contract changes, only _ura_compile_case()
# below needs to change.

# ---------------------------------------------------------------------------
# where this file lives: bash and zsh report it differently when sourced
# ---------------------------------------------------------------------------
if [ -n "$ZSH_VERSION" ]; then
    _ura_self=$0
else
    _ura_self=${BASH_SOURCE[0]}
fi
URA_ROOT="$(cd "$(dirname "$_ura_self")" && pwd)"
unset _ura_self

URA_BUILD_DIR="$URA_ROOT/build"
URA_TESTS_DIR="$URA_ROOT/tests"
URA_BIN="$URA_BUILD_DIR/ura"

# put build/ on PATH so `ura` is callable by name; the case guard keeps
# re-sourcing from stacking duplicate entries
case ":$PATH:" in
    *":$URA_BUILD_DIR:"*) ;;
    *) PATH="$URA_BUILD_DIR:$PATH" ;;
esac
export PATH

# ---------------------------------------------------------------------------
# prompt: prepend "(ura-lang) " in green, once. The two shells spell prompt
# escapes differently, so each gets its own form.
# ---------------------------------------------------------------------------
if [ -z "$URA_LANG_PS1_SET" ]; then
    URA_LANG_PS1_SET=1
    if [ -n "$ZSH_VERSION" ]; then
        PS1="%F{green}(ura-lang)%f $PS1"
    else
        PS1="\[\033[0;32m\](ura-lang) \[\033[0m\]$PS1"
    fi
fi

# ---------------------------------------------------------------------------
# small color helpers - green for success, red for failure, nothing else
# ---------------------------------------------------------------------------
_ura_green() { printf '\033[0;32m%s\033[0m\n' "$1"; }
_ura_red()   { printf '\033[0;31m%s\033[0m\n' "$1"; }

# ============================================================================
# build - compile the ura compiler itself into build/ura
# ============================================================================
build() {
    (
        cd "$URA_ROOT" || exit 1
        set -e

        CC=clang
        # arrays, not strings: zsh does not word-split unquoted "$VAR"
        FLAGS=(-fsanitize=address -g3)

        # every llvm-config in sight: on PATH, plain or versioned (Ubuntu's
        # llvm-config-12), and in the usual install prefixes
        _ura_candidates() {
            # zsh complains about a glob that matches nothing, and does it
            # before ls runs, so 2>/dev/null alone would not silence it
            if [ -n "$ZSH_VERSION" ]; then setopt local_options null_glob; fi
            for name in llvm-config $(seq -f 'llvm-config-%g' 3 40); do
                command -v "$name" || true
            done
            ls /usr/lib/llvm-*/bin/llvm-config /usr/local/opt/llvm*/bin/llvm-config \
                /opt/homebrew/opt/llvm*/bin/llvm-config "$HOME"/llvm*/bin/llvm-config 2>/dev/null || true
        }

        _ura_links() {
            echo 'int main(void) { return 0; }' | $CC -x c - "$@" -o /dev/null 2>/dev/null
        }

        if [ -z "$LLVM_CONFIG" ]; then
            LLVM_CONFIG=$(for c in $(_ura_candidates); do echo "$("$c" --version) $c"; done |
                          sort -t . -k1,1n -k2,2n -k3,3n | tail -n 1 | cut -d ' ' -f 2)
        fi
        if ! command -v "$LLVM_CONFIG" >/dev/null 2>&1; then
            echo "build: no llvm-config found, run as: LLVM_CONFIG=/path/to/llvm-config" >&2
            exit 1
        fi
        echo "using LLVM $("$LLVM_CONFIG" --version) ($LLVM_CONFIG)"

        # ipo holds the legacy pass manager main.c falls back to before LLVM 13;
        # the rpath lets build/ura find a shared LLVM installed outside system paths
        LIBS=($("$LLVM_CONFIG" --ldflags) "-Wl,-rpath,$("$LLVM_CONFIG" --libdir)")
        LIBS+=($("$LLVM_CONFIG" --libs core native bitwriter passes ipo))

        # a static LLVM is C++, and clang only adds the C++ runtime when linking C++
        if [ "$("$LLVM_CONFIG" --shared-mode)" = static ]; then
            case "$(uname)" in
                Darwin) LIBS+=(-lc++) ;;
                *)      LIBS+=(-lstdc++) ;;
            esac
        fi

        # --system-libs can name libraries only installed as versioned runtimes
        # (libzstd.so.1) when their -dev package is missing; link through a
        # temporary unversioned symlink in that case
        SHIM=
        for lib in $("$LLVM_CONFIG" --system-libs); do
            LIBS+=("$lib")
            case $lib in -l*) ;; *) continue ;; esac
            _ura_links "$lib" && continue
            runtime=$(ldconfig -p 2>/dev/null |
                      awk -v n="lib${lib#-l}.so." 'index($1, n) == 1 { print $NF; exit }')
            [ -n "$runtime" ] || continue
            if [ -z "$SHIM" ]; then
                SHIM=$(mktemp -d)
                trap 'rm -rf "$SHIM"' EXIT
                LIBS=("-L$SHIM" "${LIBS[@]}")
            fi
            ln -sf "$runtime" "$SHIM/lib${lib#-l}.so"
        done

        CFLAGS_LLVM=($("$LLVM_CONFIG" --cflags))

        mkdir -p build
        "$CC" "${FLAGS[@]}" "${CFLAGS_LLVM[@]}" main.c asm.c errors.c "${LIBS[@]}" -o build/ura
    ) && _ura_green "build: build/ura ready" || { _ura_red "build: failed"; return 1; }
}

# ============================================================================
# _ura_compile_case <ura_file> <out_name>
#
# Compiles <ura_file> and stores the generated LLVM IR in the global
# URA_CASE_IR. The executable is only a by-product: it's never run, and it
# is deleted as soon as the compile finishes.
#
# Returns 1 if the compiler itself failed.
# ============================================================================
_ura_compile_case() {
    local ura_file="$1" out_name="$2"
    local exe="$URA_BUILD_DIR/$out_name"
    # ura derives the IR path from the source, not from -o: <dir>/build/<base>.ll
    local ll="$(dirname "$ura_file")/build/$(basename "$ura_file" .ura).ll"

    if [ ! -x "$URA_BIN" ]; then
        _ura_red "compile: build/ura not found - run 'build' first"
        return 1
    fi

    if ! "$URA_BIN" "$ura_file" -o "$exe" >/dev/null; then
        rm -f "$exe"
        _ura_red "compile: failed for $ura_file"
        return 1
    fi

    URA_CASE_IR=$(cat "$ll" 2>/dev/null)
    rm -f "$exe"
}

# ============================================================================
# _ura_extract_entry <md_file> <number>
#
# Prints the lines of test entry <number> (e.g. "003"): from its
# "## NNN - ..." heading up to, but not including, the next "## NNN" heading
# or end of file.
# ============================================================================
_ura_extract_entry() {
    local md_file="$1" number="$2"
    awk -v want="## $number" '
        /^## [0-9][0-9][0-9] / {
            if (in_entry) exit
            if (index($0, want) == 1) in_entry = 1
        }
        in_entry { print }
    ' "$md_file"
}

# ============================================================================
# _ura_extract_fence [after_heading]
#
# Reads markdown on stdin. With no argument, prints the contents of the
# first fenced code block found. With <after_heading>, first skips ahead to
# the line "### <after_heading>", then prints the contents of the next fenced
# code block after that. Either way, the ``` fence lines themselves are not
# included in the output.
# ============================================================================
_ura_extract_fence() {
    local after_heading="$1"
    awk -v after="$after_heading" '
        BEGIN { seeking = (after != "") }
        seeking && $0 == "### " after { seeking = 0; found = 1; next }
        seeking { next }
        after != "" && !found { next }
        !in_fence && /^```/ { in_fence = 1; next }
        in_fence && /^```/ { exit }
        in_fence { print }
    '
}

# ============================================================================
# _ura_next_number <md_file>
#
# Prints the next zero-padded test number for <md_file>: one past the
# highest existing "## NNN" heading, or "001" if the file has none yet.
# ============================================================================
_ura_next_number() {
    local md_file="$1" last
    last=$(grep -oE '^## [0-9]{3}' "$md_file" 2>/dev/null | grep -oE '[0-9]{3}' | sort -n | tail -1)
    [ -n "$last" ] || last=000
    printf '%03d' $((10#$last + 1))
}

# ============================================================================
# _ura_rewrite_index <md_file>
#
# Regenerates the "## index" bullet list from every "## NNN - description"
# heading currently in <md_file>. Everything before "## index" and
# everything from the first entry onward is left untouched; only the
# bullet list in between is replaced.
# ============================================================================
_ura_rewrite_index() {
    local md_file="$1" tmp
    tmp=$(mktemp)

    {
        # the header is rebuilt from the file name every time, so a file that
        # is empty or lost its header still comes out well formed
        printf '# %s\n\n## index\n\n' "$(basename "$md_file" .md)"
        grep -E '^## [0-9]{3} ' "$md_file" | sed -E 's/^## /- /'
        echo
        # every entry, from the first rule or numbered heading to the end
        awk '
            found { print; next }
            /^---$/ || /^## [0-9][0-9][0-9] / { found = 1; print }
        ' "$md_file"
    } > "$tmp"

    mv "$tmp" "$md_file"
}

# ============================================================================
# _ura_entry_number <md_file> <description>
#
# Prints the NNN of the entry whose description matches exactly, or nothing
# if there is no such entry.
# ============================================================================
_ura_entry_number() {
    local md_file="$1" description="$2"
    local line rest num desc

    while IFS= read -r line; do
        rest=${line#"## "}     # "001 — basic while"
        num=${rest%% *}        # "001"
        desc=${rest#* — }      # "basic while"
        if [ "$desc" = "$description" ]; then
            printf '%s' "$num"
            return 0
        fi
    done < <(grep -E '^## [0-9]{3} ' "$md_file")
}

# ============================================================================
# _ura_entry_description <md_file> <number>
#
# Prints the description of entry <number>, or nothing if there is no
# such entry.
# ============================================================================
_ura_entry_description() {
    local md_file="$1" number="$2" line
    line=$(grep -m1 "^## $number — " "$md_file" 2>/dev/null) || return 0
    printf '%s' "${line#"## $number — "}"
}

# ============================================================================
# _ura_replace_entry <md_file> <number> <description> <ura_file>
#
# Splices a freshly generated entry over the lines the existing <number>
# entry occupies, so its number and its position in the file are kept.
# ============================================================================
_ura_replace_entry() {
    local md_file="$1" number="$2" description="$3" ura_file="$4"
    local head_line start end tmp

    head_line=$(grep -n -m1 "^## $number — " "$md_file" | cut -d: -f1)

    # take the --- rule above the heading with it: an entry is written as
    # rule, blank line, heading, so step back over the blank line first
    start=$head_line
    if [ "$start" -gt 1 ] && [ -z "$(sed -n "$((start - 1))p" "$md_file")" ]; then
        start=$((start - 1))
    fi
    if [ "$start" -gt 1 ] && [ "$(sed -n "$((start - 1))p" "$md_file")" = "---" ]; then
        start=$((start - 1))
    fi

    # the entry runs until the next rule or numbered heading, else to the end
    end=$(awk -v s="$head_line" 'NR > s && (/^---$/ || /^## [0-9][0-9][0-9] /) { print NR - 1; exit }' "$md_file")
    [ -n "$end" ] || end=$(wc -l < "$md_file")

    tmp=$(mktemp)
    sed -n "1,$((start - 1))p" "$md_file" > "$tmp"
    _ura_write_entry "$number" "$description" "$ura_file" >> "$tmp"
    sed -n "$((end + 1)),\$p" "$md_file" >> "$tmp"
    mv "$tmp" "$md_file"
}

# ============================================================================
# _ura_trim_blank_lines <file>
#
# Prints <file> with blank lines at the top and bottom removed, always
# ending in a newline so a following ``` fence starts on its own line.
# Blank lines in the middle are kept.
# ============================================================================
_ura_trim_blank_lines() {
    # blank lines are held back rather than printed; they only reach the
    # output once a non-blank line follows, which drops a leading run and a
    # trailing run alike
    awk '
        /^[[:space:]]*$/ { if (started) pending = pending "\n"; next }
        {
            if (pending != "") { printf "%s", pending; pending = "" }
            started = 1
            print
        }
    ' "$1"
}

# ============================================================================
# _ura_write_entry <number> <description> <ura_file>
#
# Prints one complete test entry to stdout, using the URA_CASE_IR global the
# last _ura_compile_case call left behind.
# ============================================================================
_ura_write_entry() {
    local number="$1" description="$2" ura_file="$3"

    echo "---"
    echo
    echo "## $number — $description"
    echo
    echo '```ura'
    _ura_trim_blank_lines "$ura_file"
    echo '```'
    echo
    echo "### llvm ir"
    echo
    echo '```llvm'
    printf '%s\n' "$URA_CASE_IR"
    echo '```'
    echo
}

# ============================================================================
# copy <ura_file> <md_prefix> <description>
#
# Snapshots <ura_file> into tests/<md_prefix>.md, alongside its generated
# LLVM IR. An entry with the same description is replaced in place, keeping
# its number; otherwise a new one is appended.
#
#   copy file.ura while "basic while"
# ============================================================================
copy() {
    local ura_file="$1" md_prefix="$2" description="$3"
    if [ -z "$ura_file" ] || [ -z "$md_prefix" ] || [ -z "$description" ]; then
        _ura_red "usage: copy <ura_file> <md_prefix> <description>"
        return 1
    fi
    if [ ! -f "$ura_file" ]; then
        _ura_red "copy: no such file: $ura_file"
        return 1
    fi
    # md_prefix becomes a file path, so keep it a plain name
    case "$md_prefix" in
        */*|.*) _ura_red "copy: md_prefix must not contain '/' or start with '.'"; return 1 ;;
    esac

    mkdir -p "$URA_TESTS_DIR"
    local md_file="$URA_TESTS_DIR/$md_prefix.md"
    # the header itself is written by _ura_rewrite_index
    touch "$md_file"

    _ura_compile_case "$ura_file" "$(basename "$ura_file" .ura)" || return 1

    local number
    number=$(_ura_entry_number "$md_file" "$description")

    if [ -z "$number" ]; then
        number=$(_ura_next_number "$md_file")
        _ura_write_entry "$number" "$description" "$ura_file" >> "$md_file"
        _ura_rewrite_index "$md_file"
        _ura_green "copy: added $number — $description to tests/$md_prefix.md"
        return 0
    fi

    _ura_replace_entry "$md_file" "$number" "$description" "$ura_file"
    _ura_rewrite_index "$md_file"
    _ura_green "copy: replaced $number — $description in tests/$md_prefix.md"
}

# ============================================================================
# replace <ura_file> <number> <md_prefix>
#
# Recompiles <ura_file> and overwrites test <number> in tests/<md_prefix>.md,
# keeping that entry's existing description. The number may be written
# without padding.
#
#   replace file.ura 1 while
# ============================================================================
replace() {
    local ura_file="$1" number="$2" md_prefix="$3"
    if [ -z "$ura_file" ] || [ -z "$number" ] || [ -z "$md_prefix" ]; then
        _ura_red "usage: replace <ura_file> <number> <md_prefix>"
        return 1
    fi
    if [ ! -f "$ura_file" ]; then
        _ura_red "replace: no such file: $ura_file"
        return 1
    fi
    case "$md_prefix" in
        */*|.*) _ura_red "replace: md_prefix must not contain '/' or start with '.'"; return 1 ;;
    esac

    local md_file="$URA_TESTS_DIR/$md_prefix.md"
    if [ ! -f "$md_file" ]; then
        _ura_red "replace: no such test file: $md_file"
        return 1
    fi

    number=$(printf '%03d' "$((10#$number))")

    local description
    description=$(_ura_entry_description "$md_file" "$number")
    if [ -z "$description" ]; then
        _ura_red "replace: no test $number in tests/$md_prefix.md"
        return 1
    fi

    _ura_compile_case "$ura_file" "$(basename "$ura_file" .ura)" || return 1

    _ura_replace_entry "$md_file" "$number" "$description" "$ura_file"
    _ura_rewrite_index "$md_file"
    _ura_green "replace: replaced $number — $description in tests/$md_prefix.md"
}

# ============================================================================
# _ura_compare <entry_text> <section_heading> <fresh_value> <number> <name>
#
# Diffs <fresh_value> against the recorded <section_heading> block inside
# <entry_text>. Prints a short failure line plus a few lines of diff on
# mismatch. Returns 1 on mismatch, 0 on match.
# ============================================================================
_ura_compare() {
    local entry="$1" heading="$2" fresh="$3" number="$4" name="$5"
    local recorded
    recorded=$(printf '%s\n' "$entry" | _ura_extract_fence "$heading")

    [ "$recorded" = "$fresh" ] && return 0

    _ura_red "  $number — $name: FAIL ($heading)"
    diff <(printf '%s\n' "$recorded") <(printf '%s\n' "$fresh") | head -6 | sed 's/^/      /'
    return 1
}

# ============================================================================
# _ura_test_file <md_file> [number]
#
# Runs every entry in <md_file>, or only entry <number> when given, under a
# "<name>:" header. Adds to the _ura_passed / _ura_failed counters that
# tests() resets.
# ============================================================================
_ura_test_file() {
    local md_file="$1" only="$2"
    local label numbers number entry name tmp_dir tmp_ura

    label=$(basename "$md_file" .md)
    numbers=$(grep -oE '^## [0-9]{3}' "$md_file" | grep -oE '[0-9]{3}')

    if [ -n "$only" ]; then
        if ! printf '%s\n' "$numbers" | grep -qx "$only"; then
            _ura_red "tests: no test $only in tests/$label.md"
            return 1
        fi
        numbers=$only
    fi
    [ -n "$numbers" ] || return 0

    echo "$label:"

    # command substitution, not a bare $numbers: zsh does not word-split variables
    for number in $(printf '%s\n' "$numbers"); do
        entry=$(_ura_extract_entry "$md_file" "$number")
        name=$(printf '%s\n' "$entry" | head -1 | sed -E 's/^## [0-9]{3} — //')

        # a private directory, so the build/ that ura creates next to the
        # source is removed along with it
        tmp_dir=$(mktemp -d)
        tmp_ura="$tmp_dir/test.ura"
        printf '%s\n' "$entry" | _ura_extract_fence > "$tmp_ura"

        if ! _ura_compile_case "$tmp_ura" "test_$number"; then
            _ura_red "  $number — $name: FAIL (compile error)"
            _ura_failed=$((_ura_failed + 1))
            rm -rf "$tmp_dir"
            continue
        fi
        rm -rf "$tmp_dir"

        if _ura_compare "$entry" "llvm ir" "$URA_CASE_IR" "$number" "$name"; then
            _ura_green "  $number — $name: PASS"
            _ura_passed=$((_ura_passed + 1))
        else
            _ura_failed=$((_ura_failed + 1))
        fi
    done
}

# ============================================================================
# tests [md_prefix] [number]
#
#   tests            every test in tests/*.md
#   tests while      every test in tests/while.md
#   tests while 1    only test 001 in tests/while.md
#
# Runs build first and stops there if the build fails.
# ============================================================================
tests() {
    local md_prefix="$1" number="$2" md_file
    _ura_passed=0
    _ura_failed=0

    # always test the current sources, never a stale build/ura
    build || return 1
    echo

    if [ -n "$md_prefix" ]; then
        md_file="$URA_TESTS_DIR/$md_prefix.md"
        if [ ! -f "$md_file" ]; then
            _ura_red "tests: no such test file: tests/$md_prefix.md"
            return 1
        fi
        if [ -n "$number" ]; then
            number=$(printf '%03d' "$((10#$number))")
        fi
        _ura_test_file "$md_file" "$number" || return 1
    else
        # find, not a glob: zsh errors out on a glob that matches nothing
        while IFS= read -r md_file; do
            _ura_test_file "$md_file"
        done < <(find "$URA_TESTS_DIR" -maxdepth 1 -name '*.md' 2>/dev/null | sort)
    fi

    echo
    echo "$_ura_passed passed, $_ura_failed failed"
}

# ============================================================================
# extension - package the VS Code extension and install it into VS Code
# ============================================================================
extension() {
    if make -C "$URA_ROOT/../ura-vscode-extension" install; then
        _ura_green "extension: installed, reload VS Code to use it"
    else
        _ura_red "extension: failed"
        return 1
    fi
}

# ============================================================================
# update - re-source this file, picking up edits made to it
# ============================================================================
update() {
    source "$URA_ROOT/config.sh"
}

printf '\033[0;32m(ura-lang)\033[0m ready — commands: build, copy, replace, tests, extension, update\n'
