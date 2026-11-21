# config.sh - sourced workflow for building and testing ura-lang.
#
#   source config.sh
#
# defines these shell commands:
#   build [-D DEBUG=true]
#            rebuild <repo>/bin/ura (the compiler), plus proto.h and
#            compile_commands.json; ura prints nothing but errors unless
#            built with -D DEBUG=true (tokens, AST, IR, then runs the program)
#   ucg [-D DEBUG=true] [number]
#            build ucg_test.c (ucg.c and its tests) into <repo>/bin/ucg and run its test <number>
#            (every test when none); -D DEBUG=true prints the ucg nodes
#   tests [target] [number]
#            rebuild, then compile tests and compare the generated LLVM IR
#            with the recorded one, then run them and compare "// stdout:" and
#            "// expect:"; one line per test, with a file:line link
#            target: nothing (all) or a milestone folder (milestone01);
#            number: one entry of it
#   record [target] [number]
#            rebuild, then compile tests and (re)write their recorded IR;
#            an entry that doesn't compile is left as it was and reported
#   copy <ura_file> <file> <description>
#            add <ura_file> to the last file of tests/<file>/ as a new entry
#            (an entry with the same description is replaced in place)
#   extension
#            package the VS Code extension (../_extension) and
#            install it into VS Code
#   set-llvm [version]
#            list the LLVM versions on this machine, or pick the one build
#            uses (15 or 15.0.6); the pick is kept in llvm-version.txt
#   run <target> <number>
#            compile one entry and run its program in the terminal
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
URA_TESTS_DIR="$(cd "$URA_ROOT/.." && pwd)/tests" # one folder per milestone, in the repo root
URA_BIN="$URA_BIN_DIR/ura"
URA_JOBS="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)" # tests run this many at once
URA_LLVM_FILE="$URA_ROOT/llvm-version.txt" # written by set-llvm, read by build
# the libraries named in tests/ura.conf, as git clones in projects/ (build
# clones a missing one), so tests run against the code being edited. Each
# test compiles in its own temporary folder, next to a ura.conf that points
# at these folders.
URA_PROJECTS_DIR="$(cd "$URA_ROOT/.." && pwd)/projects"
URA_DEPENDENCIES_DIR="$URA_TESTS_DIR/dependencies"
URA_TEST_CONFIG="$(sed -n 's|^@dependency "\([^"]*\)" "\([^"]*\)" "[^"]*".*|@dependency "\1" "\2" "'"$URA_PROJECTS_DIR"'/\1"|p' "$URA_TESTS_DIR/ura.conf")"

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
# _ura_llvm_list
#
# Prints "<version> <llvm-config>" for every LLVM in sight, oldest first, one
# line per version: llvm-config on PATH, plain or versioned (Ubuntu's
# llvm-config-12), and in the usual install prefixes.
# ============================================================================
_ura_llvm_list() {
    # zsh complains about a glob that matches nothing, and does it before the
    # loop runs; null_glob drops it instead (bash keeps it, and -x fails it)
    if [ -n "$ZSH_VERSION" ]; then setopt local_options null_glob; fi
    local c
    {
        for c in llvm-config $(seq -f 'llvm-config-%g' 3 40); do
            command -v "$c" || true
        done
        for c in /usr/lib/llvm-*/bin/llvm-config /usr/local/opt/llvm*/bin/llvm-config \
                 /opt/homebrew/opt/llvm*/bin/llvm-config "$HOME"/.local/opt/llvm*/bin/llvm-config; do
            [ -x "$c" ] && echo "$c"
        done
    } | while read -r c; do echo "$("$c" --version) $c"; done |
        sort -t . -k1,1n -k2,2n -k3,3n | awk '!seen[$1]++'
}

# ============================================================================
# _ura_llvm_find [version]
#
# Prints the llvm-config of the newest LLVM matching [version]: exact
# (15.0.6) or a prefix ending at a dot (15, 15.0). No version: the newest.
# Prints nothing when none matches.
# ============================================================================
_ura_llvm_find() {
    _ura_llvm_list |
        awk -v w="$1" 'w == "" || $1 == w || index($1, w ".") == 1 { p = $2 } END { print p }'
}

# ============================================================================
# _ura_build <file> <name> [flags...]
#
# Compiles <file> with LLVM into bin/<name>, plus proto.h and
# compile_commands.json; [flags] go to the compiler.
# ============================================================================
_ura_build() {
    local src="$1" name="$2"
    shift 2
    (
        cd "$URA_ROOT" || exit 1
        set -e

        CC=clang

        # Set to 1 to enable AddressSanitizer, 0 to disable it.
        ENABLE_SAN=0

        SAN_FLAGS=()
        if (( ENABLE_SAN )); then
            SAN_FLAGS=(-fsanitize=address -g3)
        fi

        FLAGS=("${SAN_FLAGS[@]}" -Werror "$@")

        _ura_links() {
            echo 'int main(void) { return 0; }' | $CC -x c - "$@" -o /dev/null 2>/dev/null
        }

        # LLVM_CONFIG from the shell wins, then set-llvm's pick, then the newest.
        if [ -z "$LLVM_CONFIG" ]; then
            want=$(cat "$URA_LLVM_FILE" 2>/dev/null || true)
            LLVM_CONFIG=$(_ura_llvm_find "$want")
            if [ -n "$want" ] && [ -z "$LLVM_CONFIG" ]; then
                echo "build: LLVM $want (from set-llvm) not found, run set-llvm to list them" >&2
                exit 1
            fi
        fi

        if ! command -v "$LLVM_CONFIG" >/dev/null 2>&1; then
            echo "build: no llvm-config found, run as: LLVM_CONFIG=/path/to/llvm-config" >&2
            exit 1
        fi

        echo "using LLVM $("$LLVM_CONFIG" --version) ($LLVM_CONFIG)"

        LIBS=($("$LLVM_CONFIG" --ldflags) "-Wl,-rpath,$("$LLVM_CONFIG" --libdir)")
        LIBS+=($("$LLVM_CONFIG" --libs core native bitwriter passes ipo))

        # Static LLVM requires the C++ runtime.
        if [ "$("$LLVM_CONFIG" --shared-mode)" = static ]; then
            case "$(uname)" in
                Darwin) LIBS+=(-lc++) ;;
                *)      LIBS+=(-lstdc++) ;;
            esac
        fi

        # LLVM system libraries.
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

        # Math library (log2, sin, cos, etc.).
        LIBS+=(-lm)

        CFLAGS_LLVM=($("$LLVM_CONFIG" --cflags))

        # Generate prototypes using Universal Ctags.
        if ctags --version 2>/dev/null | grep -q 'Universal Ctags'; then
            _ura_protos() {
                ctags -x --c-kinds=f --language-force=C \
                    --_xformat='%{typeref} %{name}%{signature};' "$1" |
                    sed -E -e 's/^[a-z]+://' -e 's/ (\*+) / \1/g' -e 's/,/, /g' |
                    grep -v ' main('
            }

            {
                echo '// generated by build in config.sh - do not edit'
                _ura_protos ucg.c
                echo '#if !UCG'
                _ura_protos main.c
                _ura_protos check.c
                _ura_protos project.c
                echo '#endif'
            } > proto.h.tmp

            mv proto.h.tmp proto.h
        else
            echo "build: Universal Ctags not found, proto.h not regenerated" >&2
        fi


        # Generate compile_commands.json.
        {
            printf '[{"directory": "%s", "file": "main.c", "arguments": [' "$URA_ROOT"
            printf '"%s", ' "$CC" "${FLAGS[@]}" "${CFLAGS_LLVM[@]}"
            printf '"-c", "main.c"]}]\n'
        } > compile_commands.json

        mkdir -p "$URA_BIN_DIR"

        "$CC" "${FLAGS[@]}" "${CFLAGS_LLVM[@]}" \
            "$src" "${LIBS[@]}" \
            -o "$URA_BIN_DIR/$name"

    ) && _ura_green "build: bin/$name ready" || {
        _ura_red "build: failed"
        return 1
    }
}

# ============================================================================
# build - compile the ura compiler itself into bin/ura
# ============================================================================
build() {
    _ura_build main.c ura "$@" || return 1
    # `ura` from any folder: ~/.local/bin is on PATH
    mkdir -p "$HOME/.local/bin" && ln -sf "$URA_BIN" "$HOME/.local/bin/ura"
    # a library missing from projects/: clone it; one with dependencies of its
    # own (ura-posix uses ura-math): install them
    printf '%s\n' "$URA_TEST_CONFIG" | sed 's/.*"\(.*\)"$/\1/' | while read -r dir; do
        [ -d "$dir" ] || git clone -q "git@github.com:ura-computing/$(basename "$dir").git" "$dir"
        if grep -q '^@dependency' "$dir/ura.conf" && [ ! -d "$dir/dependencies" ]; then
            (cd "$dir" && "$URA_BIN" install)
        fi
    done
}

# ============================================================================
# ucg [number] - compile ucg_test.c (ucg.c plus its tests) into bin/ucg and run
# test <number> (every test when none): prints its nodes, writes
# build/out.ll, then links and runs the program
# ============================================================================
ucg() {
    local flags=()
    while [ "$1" = "-D" ]; do
        flags+=(-D "$2")
        shift 2
    done
    _ura_build ucg_test.c ucg -DUCG=1 "${flags[@]}" || return 1
    (cd "$URA_ROOT" && "$URA_BIN_DIR/ucg" "$@")
}

# ============================================================================
# _ura_compile_case <ura_file> <out_name> [run]
#
# Compiles <ura_file> and stores the results in these globals:
#   URA_CASE_IR      the generated LLVM IR (empty when the compile failed)
#   URA_CASE_STDERR  the compiler's stderr, cleaned by _ura_clean_stderr
#   URA_CASE_STATUS  the compiler's exit status (124 when it timed out)
#   URA_CASE_OUT     the program's stdout, URA_CASE_CODE its exit code: only
#                    with [run], when the source has "// stdout:" or "// expect:" lines;
#                    its stdin is the "// stdin:" lines
# The executable is deleted as soon as it has run.
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
    local ura_file="$1" out_name="$2" run="$3"
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
    URA_CASE_STDERR=$(cd "$dir" && "${limit[@]}" "$URA_BIN" "$base" -o "$exe" -ll 2>&1 >/dev/null)
    URA_CASE_STATUS=$?
    URA_CASE_STDERR=$(printf '%s\n' "$URA_CASE_STDERR" | _ura_clean_stderr)
    URA_CASE_IR=$(cat "$ll" 2>/dev/null)
    URA_CASE_OUT=
    URA_CASE_CODE=
    if [ "$run" = run ] && [ "$URA_CASE_STATUS" -eq 0 ] && grep -qE '^// (stdout|expect):' "$ura_file"; then
        URA_CASE_OUT=$(grep '^// stdin:' "$ura_file" | sed -E 's|^// stdin: ?||' |
            (cd "$dir" && "${limit[@]}" "$exe" 2>/dev/null))
        URA_CASE_CODE=$?
    fi
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
    printf '%s\n' "$URA_TEST_CONFIG" > "$tmp_dir/ura.conf"
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
#   milestone1         every .md in tests/milestone1/
#   milestone1 42      only the file of tests/milestone1/ that holds entry 042
# Returns 1 when <target> names nothing.
# ============================================================================
_ura_test_files() {
    local target="$1" number="$2" found

    if [ -z "$target" ]; then
        found=$(find "$URA_TESTS_DIR" -path "$URA_DEPENDENCIES_DIR" -prune -o -name '*.md' -print 2>/dev/null | sort)
    elif [ -f "$URA_TESTS_DIR/$target.md" ]; then
        found="$URA_TESTS_DIR/$target.md"
    elif [ -d "$URA_TESTS_DIR/$target" ]; then
        found=$(find "$URA_TESTS_DIR/$target" -name '*.md' 2>/dev/null | sort)
    else
        found=$(find "$URA_TESTS_DIR" -path "$URA_DEPENDENCIES_DIR" -prune -o -name "$target.md" -print 2>/dev/null | sort)
    fi
    if [ -n "$found" ] && [ -n "$number" ]; then
        found=$(printf '%s\n' "$found" | xargs grep -l "^## $(printf '%03d' "$((10#$number))") ")
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
# ".md" (tests/milestone01/001-variables.md -> milestone01/001-variables).
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
        _ura_red "a test number needs one folder, like: milestone01 $number" >&2
        return 1
    fi
    printf '%03d\n' "$((10#$number))"
}

# ============================================================================
# _ura_entry_link <md_file> <number>
#
# Prints "<path>:<line>" for entry <number>, the line its "## NNN" heading is
# on. The path is relative to tests/ (milestone01/001-variables.md:21): every test is
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
    local md_file="$1" tmp label
    tmp=$(mktemp)
    label=$(_ura_label "$md_file")

    {
        # the header is rebuilt from the file name every time, so a file that
        # is empty or lost its header still comes out well formed
        # (milestone01/220-operators.md -> milestone01)
        printf '# %s\n\n## index\n\n' "${label%%/*}"
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
#   copy file.ura milestone01 "basic while"
#   copy bad.ura milestone01 "unknown variable"
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
        /*|.*|*/.*) _ura_red "copy: give a path inside tests/, like milestone1"; return 1 ;;
    esac

    local md_file="$URA_TESTS_DIR/$level.md"
    # a folder: the file that has this description, else its last file
    if [ -d "$URA_TESTS_DIR/$level" ]; then
        local file
        for file in $(find "$URA_TESTS_DIR/$level" -name '*.md' | sort); do
            md_file=$file
            [ -n "$(_ura_entry_number "$file" "$description")" ] && break
        done
    fi
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
        _ura_green "copy: added $number — $description to $(_ura_label "$md_file").md"
        return 0
    fi

    _ura_replace_entry "$md_file" "$number" "$description" "$ura_file"
    _ura_rewrite_index "$md_file"
    _ura_green "copy: replaced $number — $description in $(_ura_label "$md_file").md"
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
    local numbers number description link tmp_dir tmp_ura compiled is_error

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
        printf '%s\n' "$URA_TEST_CONFIG" > "$tmp_dir/ura.conf"

        _ura_compile_case "$tmp_ura" "test_$number"
        compiled=$?
        if [ "$compiled" -eq 2 ]; then
            rm -rf "$tmp_dir"
            return 1
        fi

        # an error test records its stderr when the compile fails, any other
        # test records its IR when it compiles
        is_error=false
        if _ura_extract_entry "$md_file" "$number" | grep -qx '### status' ||
           grep -q '^// error:' "$tmp_ura"; then
            is_error=true
        fi

        if { [ "$compiled" -eq 0 ] && ! $is_error; } || { [ "$compiled" -ne 0 ] && $is_error; }; then
            # needs the source file, so before the temporary directory goes
            _ura_replace_entry "$md_file" "$number" "$description" "$tmp_ura"
            _ura_green "$link $description"
            _ura_recorded=$((_ura_recorded + 1))
        elif $is_error; then
            _ura_red "$link $description (compiles, expected an error: not recorded)"
            _ura_not_compiling=$((_ura_not_compiling + 1))
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
#   record milestone01         one milestone
#   record milestone01 4       only entry 004 of it
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

    if ! md_files=$(_ura_test_files "$target" "$number"); then
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
# The "target triple" line is skipped on both sides: it names the machine
# (x86_64-pc-linux-gnu, x86_64-unknown-linux-gnu, ...), not ura's output.
# ============================================================================
_ura_compare() {
    local entry="$1" heading="$2" fresh="$3" number="$4" name="$5" link="$6"
    local recorded
    recorded=$(printf '%s\n' "$entry" | _ura_extract_fence "$heading" | grep -v '^target triple = ')
    fresh=$(printf '%s\n' "$fresh" | grep -v '^target triple = ')

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
# rebuild bin/ura only when a source or the LLVM pick is newer than it
_ura_build_if_changed() {
    local newer
    if [ -x "$URA_BIN" ]; then
        newer=$(find "$URA_ROOT" -maxdepth 2 \( -name '*.c' -o -name '*.h' -o -name 'llvm-version.txt' \) \
            ! -name proto.h -newer "$URA_BIN" -print -quit)
        [ -z "$newer" ] && return 0
    fi
    build
}

# one pass over a test file: <dir>/NNN.md holds entry NNN, <dir>/NNN.line its line
_ura_split_entries() {
    awk -v dir="$2" '
        /^## [0-9][0-9][0-9] / {
            if (out) close(out)
            number = substr($0, 4, 3)
            out = dir "/" number ".md"
            print NR > (dir "/" number ".line")
            close(dir "/" number ".line")
        }
        out { print > out }
    ' "$1"
}

# ============================================================================
# _ura_check_run <entry> <name> <link>
#
# Compares the program's run (URA_CASE_OUT, URA_CASE_CODE) with the entry's
# "// stdout:" lines and its "// expect:" exit code. An entry with neither
# passes. Prints a failure like _ura_compare does. Returns 1 on mismatch.
# ============================================================================
_ura_check_run() {
    local entry="$1" name="$2" link="$3" text expected out code
    text=$(printf '%s\n' "$entry" | _ura_extract_fence)
    if printf '%s\n' "$text" | grep -q '^// stdout:'; then
        # trailing spaces don't count: a "// stdout:" comment can't keep them
        expected=$(printf '%s\n' "$text" | grep '^// stdout:' | sed -E -e 's|^// stdout: ?||' -e 's/[[:space:]]+$//')
        out=$(printf '%s\n' "$URA_CASE_OUT" | sed -E 's/[[:space:]]+$//')
        if [ "$expected" != "$out" ]; then
            _ura_red "$link $name (stdout differs)"
            diff <(printf '%s\n' "$expected") <(printf '%s\n' "$out") | head -6 | sed 's/^/      /'
            return 1
        fi
    fi
    code=$(printf '%s\n' "$text" | grep -m1 '^// expect:' | grep -oE '[0-9]+' | head -1)
    if [ -n "$code" ] && [ "$code" != "$URA_CASE_CODE" ]; then
        _ura_red "$link $name (exit $URA_CASE_CODE, expected $code)"
        return 1
    fi
    return 0
}

# runs one entry and prints its result line; 0 passed, 1 failed, 2 stop everything
_ura_test_case() {
    local md_file="$1" number="$2" work="$3"
    local entry name link tmp_ura compiled

    entry=$(cat "$work/$number.md")
    name=$(printf '%s\n' "$entry" | head -1 | sed -E 's/^## [0-9]{3} — //')
    link="${md_file#$URA_TESTS_DIR/}:$(cat "$work/$number.line")"

    # a private directory, so the build/ that ura creates is removed with it
    mkdir -p "$work/$number"
    tmp_ura="$work/$number/test.ura"
    printf '%s\n' "$entry" | _ura_extract_fence > "$tmp_ura"
    printf '%s\n' "$URA_TEST_CONFIG" > "$work/$number/ura.conf"

    _ura_compile_case "$tmp_ura" "test_$number" run
    compiled=$?
    rm -rf "$work/$number"
    [ "$compiled" -ne 2 ] || return 2

    if printf '%s\n' "$entry" | grep -qx '### status'; then
        # an entry that records a status expects the compile to fail,
        # with exactly that stderr and status
        if _ura_compare "$entry" "stderr" "$URA_CASE_STDERR" "$number" "$name" "$link" &&
            _ura_compare "$entry" "status" "$URA_CASE_STATUS" "$number" "$name" "$link"; then
            _ura_green "$link $name"
            return 0
        fi
        return 1
    elif [ "$compiled" -ne 0 ]; then
        _ura_red "$link $name (compile error, status $URA_CASE_STATUS)"
        _ura_print_failure "$URA_CASE_STDERR"
        return 1
    elif _ura_compare "$entry" "llvm ir" "$URA_CASE_IR" "$number" "$name" "$link" &&
        _ura_check_run "$entry" "$name" "$link"; then
        _ura_green "$link $name"
        return 0
    fi
    return 1
}

_ura_test_file() {
    local md_file="$1" only="$2"
    local numbers number work code passed failed stop

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
    work=$(mktemp -d)
    _ura_split_entries "$md_file" "$work"

    # everything runs in a subshell, so job control prints nothing: workers start
    # in the background, and each result prints as soon as it and the ones before
    # it are done; command substitution, not a bare $numbers: zsh does not split
    (
        (
            running=0
            for number in $(printf '%s\n' "$numbers"); do
                (
                    _ura_test_case "$md_file" "$number" "$work" > "$work/$number.out" 2>&1
                    echo $? > "$work/$number.code"
                ) &
                running=$((running + 1))
                if [ "$running" -ge "$URA_JOBS" ]; then
                    wait
                    running=0
                fi
            done
            wait
        ) &
        passed=0
        failed=0
        stop=0
        for number in $(printf '%s\n' "$numbers"); do
            while [ ! -s "$work/$number.code" ]; do
                sleep 0.05
            done
            cat "$work/$number.out"
            code=$(cat "$work/$number.code")
            if [ "$code" -eq 0 ]; then
                passed=$((passed + 1))
            elif [ "$code" -eq 2 ]; then
                stop=1
                break
            else
                failed=$((failed + 1))
            fi
        done
        wait
        echo "$passed $failed $stop" > "$work/tally"
    )

    read -r passed failed stop < "$work/tally"
    _ura_passed=$((_ura_passed + passed))
    _ura_failed=$((_ura_failed + failed))
    if [ "$stop" -eq 1 ]; then
        rm -rf "$work"
        return 1
    fi
    rm -rf "$work"
}

# ============================================================================
# tests [target] [number]
#
#   tests                      every test file
#   tests milestone01          one milestone
#   tests milestone01 4        only entry 004 of it
#
# Runs build first and stops there if the build fails. Prints each file and
# one line per test under it (see _ura_test_file), then the totals.
# ============================================================================
tests() {
    local target="$1" number="$2" md_files md_file
    _ura_passed=0
    _ura_failed=0

    # always test the current sources, never a stale bin/ura
    _ura_build_if_changed || return 1
    echo

    if ! md_files=$(_ura_test_files "$target" "$number"); then
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
# run <target> <number>
#
#   run milestone13 41
#
# Rebuilds, then compiles one entry and runs its program with the terminal
# as stdin and stdout: an SDL window opens, input() reads what you type.
# The entry links what its `link` lines name, like any ura program.
# ============================================================================
run() {
    local target="$1" number="$2" md_files tmp_dir code
    if [ -z "$target" ] || [ -z "$number" ]; then
        _ura_red "run: give a file and a number, like: run milestone13 41"
        return 1
    fi
    build || return 1
    if ! md_files=$(_ura_test_files "$target" "$number"); then
        _ura_red "run: no tests named '$target' in tests/"
        return 1
    fi
    number=$(_ura_pick_number "$md_files" "$number") || return 1

    tmp_dir=$(mktemp -d)
    _ura_extract_entry "$md_files" "$number" | _ura_extract_fence > "$tmp_dir/test.ura"
    printf '%s\n' "$URA_TEST_CONFIG" > "$tmp_dir/ura.conf"
    if [ ! -s "$tmp_dir/test.ura" ]; then
        _ura_red "run: no test $number in $(_ura_label "$md_files")"
        rm -rf "$tmp_dir"
        return 1
    fi
    if ! (cd "$tmp_dir" && "$URA_BIN" test.ura -o program >/dev/null </dev/null); then
        _ura_red "run: test $number doesn't compile"
        rm -rf "$tmp_dir"
        return 1
    fi
    (cd "$tmp_dir" && ./program)
    code=$?
    rm -rf "$tmp_dir"
    echo "exit code: $code"
}

# ============================================================================
# extension - package the VS Code extension and install it into VS Code
# ============================================================================
extension() {
    if make -C "$URA_ROOT/../_extension" install; then
        _ura_green "extension: installed, reload VS Code to use it"
    else
        _ura_red "extension: failed"
        return 1
    fi
}

# ============================================================================
# set-llvm [version] - with no version, list the LLVM versions on this
# machine (* marks the one build uses); with one, make build use it. The
# pick is kept in llvm-version.txt
# ============================================================================
set-llvm() {
    local want="$1" active config version mark
    if [ -z "$want" ]; then
        active=$(_ura_llvm_find "$(cat "$URA_LLVM_FILE" 2>/dev/null)")
        _ura_llvm_list | while read -r version config; do
            if [ "$config" = "$active" ]; then mark='*'; else mark=' '; fi
            printf '%s %-8s %s\n' "$mark" "$version" "$config"
        done
        [ -f "$URA_LLVM_FILE" ] || echo "none set: build uses the newest"
        return
    fi
    config=$(_ura_llvm_find "$want")
    if [ -z "$config" ]; then
        _ura_red "set-llvm: no LLVM $want, run set-llvm to list them"
        return 1
    fi
    "$config" --version > "$URA_LLVM_FILE"
    _ura_green "set-llvm: build uses LLVM $(cat "$URA_LLVM_FILE") ($config)"
}

# ============================================================================
# update - re-source this file, picking up edits made to it
# ============================================================================
update() {
    source "$URA_ROOT/config.sh"
}

printf '\033[0;32m(ura-lang)\033[0m ready — commands: build, ucg, tests, record, copy, run, extension, set-llvm, update\n'
