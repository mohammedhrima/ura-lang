# config.sh - sourced workflow for building and testing ura-lang.
#
#   source config.sh
#
# defines these shell commands:
#   build    rebuild <repo>/bin/ura (the compiler), plus proto.h and
#            compile_commands.json
#   tests [target] [number]
#            rebuild, then compile tests and compare the generated LLVM IR
#            with the recorded one; one line per test, with a file:line link
#            target: nothing (all), a folder (data_types), a file
#            (data_types/bool, or just bool); number: one entry of a file
#   record [target] [number]
#            rebuild, then compile tests and (re)write their recorded IR;
#            an entry that doesn't compile is left as it was and reported
#   copy <ura_file> <file> <description>
#            add <ura_file> to tests/<file>.md as a new entry (an entry with
#            the same description is replaced in place)
#   extension
#            package the VS Code extension (../0.ura-vscode-extension) and
#            install it into VS Code
#   update   re-source this file after editing it
#
# Works when sourced from either bash or zsh. Everything else here is an
# internal helper, prefixed _ura_ so it doesn't collide with your shell.
#
# Depends on `ura <file>.ura -o <exe>` writing its LLVM IR to
# <current directory>/build/out.ll. If that contract changes, only
# _ura_compile_case() below needs to change.

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

URA_BIN_DIR="$(cd "$URA_ROOT/.." && pwd)/bin" # the ura compiler itself, in the repo root
URA_BUILD_DIR="$URA_ROOT/build" # what ura generates when it compiles a file
URA_TESTS_DIR="$URA_ROOT/tests"
URA_BIN="$URA_BIN_DIR/ura"

# put bin/ on PATH so `ura` is callable by name; the case guard keeps
# re-sourcing from stacking duplicate entries
case ":$PATH:" in
    *":$URA_BIN_DIR:"*) ;;
    *) PATH="$URA_BIN_DIR:$PATH" ;;
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
# build - compile the ura compiler itself into bin/ura
# ============================================================================
build() {
    (
        cd "$URA_ROOT" || exit 1
        set -e

        CC=clang
        # arrays, not strings: zsh does not word-split unquoted "$VAR"
        FLAGS=(-fsanitize=address -g3 -Werror)

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
        # the rpath lets bin/ura find a shared LLVM installed outside system paths
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

        # proto.h: a prototype for every function in main.c so
        # definition order never matters. Needs Universal Ctags; without it
        # the committed proto.h is used as is
        if ctags --version 2>/dev/null | grep -q 'Universal Ctags'; then
            {
                echo '// generated by build in config.sh - do not edit'
                ctags -x --c-kinds=f --language-force=C \
                    --_xformat='%{typeref} %{name}%{signature};' main.c |
                    sed -E -e 's/^[a-z]+://' -e 's/ (\*+) / \1/g' -e 's/,/, /g' |
                    grep -v ' main('
            } > proto.h.tmp
            mv proto.h.tmp proto.h
        else
            echo "build: Universal Ctags not found, proto.h not regenerated" >&2
        fi

        # compile_commands.json: the exact flags of the compile below, so the
        # editor's C/C++ IntelliSense finds the LLVM headers and colors their names
        {
            printf '[{"directory": "%s", "file": "main.c", "arguments": [' "$URA_ROOT"
            printf '"%s", ' "$CC" "${FLAGS[@]}" "${CFLAGS_LLVM[@]}"
            printf '"-c", "main.c"]}]\n'
        } > compile_commands.json

        mkdir -p "$URA_BIN_DIR"
        "$CC" "${FLAGS[@]}" "${CFLAGS_LLVM[@]}" main.c "${LIBS[@]}" -o "$URA_BIN"
    ) && _ura_green "build: bin/ura ready" || { _ura_red "build: failed"; return 1; }
}

# ============================================================================
# _ura_compile_case <ura_file> <out_name>
#
# Compiles <ura_file> and stores the results in three globals:
#   URA_CASE_IR      the generated LLVM IR (empty when the compile failed)
#   URA_CASE_STDERR  the compiler's stderr, cleaned by _ura_clean_stderr
#   URA_CASE_STATUS  the compiler's exit status (124 when it timed out)
# The executable is only a by-product: it's never run, and it is deleted as
# soon as the compile finishes.
#
# The compiler runs from the source's directory with a bare file name, so
# messages say "test.ura:2:11" instead of a temporary path. It writes its IR
# to build/out.ll under that directory, so each test (in its own temporary
# directory) reads its own IR.
#
# Returns 0 when the compile succeeded, 1 when it failed, 2 when there is no
# compiler to run.
# ============================================================================
_ura_compile_case() {
    local ura_file="$1" out_name="$2"
    local exe="$URA_BUILD_DIR/$out_name"
    local dir base ll
    local -a limit
    dir=$(cd "$(dirname "$ura_file")" && pwd)
    base=$(basename "$ura_file")
    # ura writes its IR under the directory it runs in: <dir>/build/out.ll
    ll="$dir/build/out.ll"

    if [ ! -x "$URA_BIN" ]; then
        _ura_red "compile: bin/ura not found - run 'build' first"
        return 2
    fi
    mkdir -p "$URA_BUILD_DIR"

    # a compiler that loops forever fails its test instead of the whole run;
    # macOS has no timeout, so it runs unguarded there
    if command -v timeout >/dev/null 2>&1; then
        limit=(timeout 10)
    fi

    # a stale .ll from an earlier run must not pass for this run's IR
    rm -f "$ll"
    URA_CASE_STDERR=$(cd "$dir" && "${limit[@]}" "$URA_BIN" "$base" -o "$exe" 2>&1 >/dev/null)
    URA_CASE_STATUS=$?
    URA_CASE_STDERR=$(printf '%s\n' "$URA_CASE_STDERR" | _ura_clean_stderr)
    URA_CASE_IR=$(cat "$ll" 2>/dev/null)
    rm -f "$exe"

    [ "$URA_CASE_STATUS" -eq 0 ] || return 1
}

# ============================================================================
# _ura_compile_as_test <ura_file>
#
# Compiles a copy of <ura_file> named test.ura, the name tests compile every
# entry under, so recorded messages match what tests prints later. Leaves
# the same globals and returns the same codes as _ura_compile_case.
# ============================================================================
_ura_compile_as_test() {
    # not "status": zsh reserves that name
    local ura_file="$1" tmp_dir code
    tmp_dir=$(mktemp -d)
    cp "$ura_file" "$tmp_dir/test.ura"
    _ura_compile_case "$tmp_dir/test.ura" test
    code=$?
    rm -rf "$tmp_dir"
    return $code
}

# ============================================================================
# _ura_clean_stderr
#
# Reads compiler stderr on stdin and prints it without colors, and with
# eprint's "main.c:1165 find_function message" written as
# "find_function: message": the file and line number move on every edit of
# main.c, the function name doesn't.
# ============================================================================
_ura_clean_stderr() {
    sed -E \
        -e $'s/\033\\[[0-9;]*m//g' \
        -e 's/^[^ :]+\.[ch]:[0-9]+ ([A-Za-z_][A-Za-z0-9_]*) /\1: /'
}

# ============================================================================
# _ura_print_failure <stderr>
#
# Prints a failed compile's stderr, indented, short enough to read in a test
# run: for a sanitizer crash only the error line and the first two stack
# frames (where it crashed), otherwise the first 8 lines.
# ============================================================================
_ura_print_failure() {
    local stderr="$1"
    if printf '%s\n' "$stderr" | grep -q 'AddressSanitizer'; then
        printf '%s\n' "$stderr" | grep -E 'ERROR: AddressSanitizer|^ *#[01] ' | sed 's/^ */      /'
    else
        printf '%s\n' "$stderr" | head -8 | sed 's/^/      /'
    fi
}

# ============================================================================
# _ura_test_files [target]
#
# Prints the test files <target> names, one per line:
#   (nothing)          every .md under tests/
#   data_types         every .md in tests/data_types/
#   data_types/bool    tests/data_types/bool.md
#   bool               the file named bool.md, in whichever folder it is
# Returns 1 when <target> names nothing.
# ============================================================================
_ura_test_files() {
    local target="$1" found

    if [ -z "$target" ]; then
        found=$(find "$URA_TESTS_DIR" -name '*.md' 2>/dev/null | sort)
    elif [ -f "$URA_TESTS_DIR/$target.md" ]; then
        found="$URA_TESTS_DIR/$target.md"
    elif [ -d "$URA_TESTS_DIR/$target" ]; then
        found=$(find "$URA_TESTS_DIR/$target" -name '*.md' 2>/dev/null | sort)
    else
        found=$(find "$URA_TESTS_DIR" -name "$target.md" 2>/dev/null | sort)
    fi

    if [ -z "$found" ]; then
        return 1
    fi
    printf '%s\n' "$found"
}

# ============================================================================
# _ura_label <md_file>
#
# Prints the name a test file is shown under: its path below tests/, without
# ".md" (tests/data_types/bool.md -> data_types/bool).
# ============================================================================
_ura_label() {
    printf '%s\n' "$1" | sed -e "s|^$URA_TESTS_DIR/||" -e 's|\.md$||'
}

# ============================================================================
# _ura_pick_number <md_files> <number>
#
# Checks that <number> can be used with <md_files> (it needs exactly one
# file) and prints it padded to 3 digits. Prints nothing for no number.
# ============================================================================
_ura_pick_number() {
    local md_files="$1" number="$2" count
    if [ -z "$number" ]; then
        return 0
    fi
    count=$(printf '%s\n' "$md_files" | wc -l)
    if [ "$count" -ne 1 ]; then
        _ura_red "a test number needs one file, like: data_types/bool $number" >&2
        return 1
    fi
    printf '%03d\n' "$((10#$number))"
}

# ============================================================================
# _ura_entry_link <md_file> <number>
#
# Prints "<path>:<line>" for entry <number>, the line its "## NNN" heading is
# on. The path is relative to tests/ (control_flow/if.md:21): every test is
# there, so the prefix adds nothing. Terminals that turn "file:line" into a
# link can still open it (VS Code finds it in the workspace).
# ============================================================================
_ura_entry_link() {
    local md_file="$1" number="$2" line
    line=$(awk -v want="## $number " 'index($0, want) == 1 { print NR; exit }' "$md_file")
    printf '%s:%s' "${md_file#$URA_TESTS_DIR/}" "$line"
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
# Prints one complete test entry to stdout, using the globals the last
# _ura_compile_case call left behind: the LLVM IR when the compile
# succeeded, else the compiler's stderr and exit status.
# ============================================================================
_ura_write_entry() {
    local number="$1" description="$2" ura_file="$3"

    echo "---"
    echo
    # printf, not echo: zsh's echo turns a "\0" in the description into a NUL
    printf '## %s — %s\n' "$number" "$description"
    echo
    echo '```ura'
    _ura_trim_blank_lines "$ura_file"
    echo '```'
    echo
    if [ "$URA_CASE_STATUS" -ne 0 ]; then
        echo "### stderr"
        echo
        echo '```'
        printf '%s\n' "$URA_CASE_STDERR"
        echo '```'
        echo
        echo "### status"
        echo
        echo '```'
        printf '%s\n' "$URA_CASE_STATUS"
        echo '```'
        echo
        return 0
    fi
    echo "### llvm ir"
    echo
    echo '```llvm'
    printf '%s\n' "$URA_CASE_IR"
    echo '```'
    echo
}

# ============================================================================
# copy <ura_file> <file> <description>
#
# Snapshots <ura_file> into tests/<file>.md (folders are created), alongside
# its generated LLVM IR, or its stderr and exit status when the compile
# fails. An entry with the same description is replaced in place, keeping
# its number; otherwise a new one is appended.
#
#   copy file.ura control_flow/while "basic while"
#   copy bad.ura errors/variables "unknown variable"
# ============================================================================
copy() {
    local ura_file="$1" level="$2" description="$3"
    if [ -z "$ura_file" ] || [ -z "$level" ] || [ -z "$description" ]; then
        _ura_red "usage: copy <ura_file> <file> <description>"
        return 1
    fi
    if [ ! -f "$ura_file" ]; then
        _ura_red "copy: no such file: $ura_file"
        return 1
    fi
    # level becomes a path below tests/: folders are fine, leaving tests/ isn't
    case "$level" in
        /*|.*|*/.*) _ura_red "copy: give a path inside tests/, like data_types/bool"; return 1 ;;
    esac

    local md_file="$URA_TESTS_DIR/$level.md"
    mkdir -p "$(dirname "$md_file")"
    # the header itself is written by _ura_rewrite_index
    touch "$md_file"

    # a failed compile is recorded too, as an error test; only a missing
    # compiler stops here
    _ura_compile_as_test "$ura_file"
    [ $? -ne 2 ] || return 1

    local number
    number=$(_ura_entry_number "$md_file" "$description")

    if [ -z "$number" ]; then
        number=$(_ura_next_number "$md_file")
        _ura_write_entry "$number" "$description" "$ura_file" >> "$md_file"
        _ura_rewrite_index "$md_file"
        _ura_green "copy: added $number — $description to tests/$level.md"
        return 0
    fi

    _ura_replace_entry "$md_file" "$number" "$description" "$ura_file"
    _ura_rewrite_index "$md_file"
    _ura_green "copy: replaced $number — $description in tests/$level.md"
}

# ============================================================================
# _ura_record_file <md_file> [number]
#
# Compiles every entry of <md_file> (or only entry <number>) and rewrites its
# "### llvm ir" block with the IR the compiler generates now. An entry that
# doesn't compile is left as it was. Prints the file, then one line per
# entry: green when recorded, red when it doesn't compile. Adds to the _ura_recorded / _ura_not_compiling counters that
# record() resets.
# ============================================================================
_ura_record_file() {
    local md_file="$1" only="$2"
    local numbers number description link tmp_dir tmp_ura compiled

    numbers=$(grep -oE '^## [0-9]{3}' "$md_file" | grep -oE '[0-9]{3}')

    if [ -n "$only" ]; then
        if ! printf '%s\n' "$numbers" | grep -qx "$only"; then
            _ura_red "record: no test $only in $(_ura_label "$md_file")"
            return 1
        fi
        numbers=$only
    fi
    [ -n "$numbers" ] || return 0

    _ura_label "$md_file"

    # command substitution, not a bare $numbers: zsh does not word-split variables
    for number in $(printf '%s\n' "$numbers"); do
        description=$(_ura_entry_description "$md_file" "$number")
        link=$(_ura_entry_link "$md_file" "$number")

        tmp_dir=$(mktemp -d)
        tmp_ura="$tmp_dir/test.ura"
        _ura_extract_entry "$md_file" "$number" | _ura_extract_fence > "$tmp_ura"

        _ura_compile_case "$tmp_ura" "test_$number"
        compiled=$?
        if [ "$compiled" -eq 2 ]; then
            rm -rf "$tmp_dir"
            return 1
        fi

        if [ "$compiled" -eq 0 ]; then
            # needs the source file, so before the temporary directory goes
            _ura_replace_entry "$md_file" "$number" "$description" "$tmp_ura"
            _ura_green "$link $description"
            _ura_recorded=$((_ura_recorded + 1))
        else
            _ura_red "$link $description (doesn't compile, not recorded)"
            _ura_print_failure "$URA_CASE_STDERR"
            _ura_not_compiling=$((_ura_not_compiling + 1))
        fi
        rm -rf "$tmp_dir"
    done

    _ura_rewrite_index "$md_file"
}

# ============================================================================
# record [target] [number]
#
#   record                     every test file
#   record data_types          every file in tests/data_types/
#   record data_types/bool     one file (or just: record bool)
#   record bool 4              only entry 004 of it
#
# Rebuilds first, then (re)writes the recorded IR. Use it to fill new
# entries, and after a change that alters the IR on purpose (naming,
# codegen); review the result with `git diff`.
# ============================================================================
record() {
    local target="$1" number="$2" md_files md_file
    _ura_recorded=0
    _ura_not_compiling=0

    build || return 1
    echo

    if ! md_files=$(_ura_test_files "$target"); then
        _ura_red "record: no tests named '$target' in tests/"
        return 1
    fi
    number=$(_ura_pick_number "$md_files" "$number") || return 1

    while IFS= read -r md_file; do
        _ura_record_file "$md_file" "$number" || return 1
    done <<EOF
$md_files
EOF

    echo
    echo "$_ura_recorded recorded, $_ura_not_compiling not compiling (left as they were)"
}

# ============================================================================
# _ura_compare <entry_text> <section_heading> <fresh_value> <number> <name> <link>
#
# Diffs <fresh_value> against the recorded <section_heading> block inside
# <entry_text>. Prints nothing on match; on mismatch, the entry's line in
# red plus a few lines of diff. Returns 1 on mismatch, 0 on match.
# ============================================================================
_ura_compare() {
    local entry="$1" heading="$2" fresh="$3" number="$4" name="$5" link="$6"
    local recorded
    recorded=$(printf '%s\n' "$entry" | _ura_extract_fence "$heading")

    [ "$recorded" = "$fresh" ] && return 0

    _ura_red "$link $name ($heading differs)"
    diff <(printf '%s\n' "$recorded") <(printf '%s\n' "$fresh") | head -6 | sed 's/^/      /'
    return 1
}

# ============================================================================
# _ura_test_file <md_file> [number]
#
# Runs every entry in <md_file>, or only entry <number>. Prints the file,
# then one line per entry, green when it passes, red when it fails: a
# file:line link to the entry (ctrl+click opens it in VS Code) and its name.
# A failure shows its details under its line. Adds to the _ura_passed /
# _ura_failed counters that tests() resets.
# ============================================================================
_ura_test_file() {
    local md_file="$1" only="$2"
    local numbers number entry name link tmp_dir tmp_ura compiled

    numbers=$(grep -oE '^## [0-9]{3}' "$md_file" | grep -oE '[0-9]{3}')

    if [ -n "$only" ]; then
        if ! printf '%s\n' "$numbers" | grep -qx "$only"; then
            _ura_red "tests: no test $only in $(_ura_label "$md_file")"
            return 1
        fi
        numbers=$only
    fi
    [ -n "$numbers" ] || return 0

    _ura_label "$md_file"

    # command substitution, not a bare $numbers: zsh does not word-split variables
    for number in $(printf '%s\n' "$numbers"); do
        entry=$(_ura_extract_entry "$md_file" "$number")
        name=$(printf '%s\n' "$entry" | head -1 | sed -E 's/^## [0-9]{3} — //')
        link=$(_ura_entry_link "$md_file" "$number")

        # a private directory, so the build/ that ura creates is removed with it
        tmp_dir=$(mktemp -d)
        tmp_ura="$tmp_dir/test.ura"
        printf '%s\n' "$entry" | _ura_extract_fence > "$tmp_ura"

        _ura_compile_case "$tmp_ura" "test_$number"
        compiled=$?
        rm -rf "$tmp_dir"
        [ "$compiled" -ne 2 ] || return 1

        if printf '%s\n' "$entry" | grep -qx '### status'; then
            # an entry that records a status expects the compile to fail,
            # with exactly that stderr and status
            if _ura_compare "$entry" "stderr" "$URA_CASE_STDERR" "$number" "$name" "$link" &&
                _ura_compare "$entry" "status" "$URA_CASE_STATUS" "$number" "$name" "$link"; then
                _ura_green "$link $name"
                _ura_passed=$((_ura_passed + 1))
            else
                _ura_failed=$((_ura_failed + 1))
            fi
        elif [ "$compiled" -ne 0 ]; then
            _ura_red "$link $name (compile error, status $URA_CASE_STATUS)"
            _ura_print_failure "$URA_CASE_STDERR"
            _ura_failed=$((_ura_failed + 1))
        elif _ura_compare "$entry" "llvm ir" "$URA_CASE_IR" "$number" "$name" "$link"; then
            _ura_green "$link $name"
            _ura_passed=$((_ura_passed + 1))
        else
            _ura_failed=$((_ura_failed + 1))
        fi
    done
}

# ============================================================================
# tests [target] [number]
#
#   tests                      every test file
#   tests data_types           every file in tests/data_types/
#   tests data_types/bool      one file (or just: tests bool)
#   tests bool 4               only entry 004 of it
#
# Runs build first and stops there if the build fails. Prints each file and
# one line per test under it (see _ura_test_file), then the totals.
# ============================================================================
tests() {
    local target="$1" number="$2" md_files md_file
    _ura_passed=0
    _ura_failed=0

    # always test the current sources, never a stale bin/ura
    build || return 1
    echo

    if ! md_files=$(_ura_test_files "$target"); then
        _ura_red "tests: no tests named '$target' in tests/"
        return 1
    fi
    number=$(_ura_pick_number "$md_files" "$number") || return 1

    while IFS= read -r md_file; do
        _ura_test_file "$md_file" "$number" || return 1
    done <<EOF
$md_files
EOF

    echo
    echo "$_ura_passed passed, $_ura_failed failed"
}

# ============================================================================
# extension - package the VS Code extension and install it into VS Code
# ============================================================================
extension() {
    if make -C "$URA_ROOT/../0.ura-vscode-extension" install; then
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

printf '\033[0;32m(ura-lang)\033[0m ready — commands: build, tests, record, copy, extension, update\n'
