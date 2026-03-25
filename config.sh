#!/usr/bin/env bash

# =========================================================
#  Root Detection (no hardcoded paths)
# =========================================================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build"
TESTS_DIR="$ROOT_DIR/tests"
LLVM_DIR="$ROOT_DIR/llvm"
CONFIG_FILE="$SCRIPT_DIR/config.sh"
export URA_LIB="$SRC_DIR/ura-lib"
export ASAN_FILE="$ROOT_DIR/config/lsan.supp"

# =========================================================
#  Native Library Link Flags (used by `link "@/<key>"`)
# =========================================================
_URA_SDK=$(xcrun --show-sdk-path 2>/dev/null)
export URA_LINK_raylib="-I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -isysroot $_URA_SDK -F$_URA_SDK/System/Library/Frameworks -L$_URA_SDK/usr/lib"

# =========================================================
#  OS Detection
# =========================================================
case "$(uname -s 2>/dev/null)" in
    Darwin*)            OS_TYPE="macos"   ;;
    Linux*)             OS_TYPE="linux"   ;;
    MINGW*|MSYS*|CYGWIN*) OS_TYPE="windows" ;;
    *)                  OS_TYPE="unknown" ;;
esac

# =========================================================
#  Color Definitions
# =========================================================
BOLD="\033[1m"
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
CYAN="\033[0;36m"
DIM="\033[2m"
RESET="\033[0m"

# =========================================================
#  Compiler Configuration  (pinned to LLVM 14)
# =========================================================
URA_COMPILER="$BUILD_DIR/ura"

# SRC_FILES=(
#     "$SRC_DIR/main.c"
#     "$SRC_DIR/lexer.c"
#     "$SRC_DIR/parser.c"
#     "$SRC_DIR/ir.c"
#     "$SRC_DIR/codegen.c"
#     "$SRC_DIR/utils.c"
# )

SRC_FILES=(
    "$SRC_DIR/main.c"
    "$SRC_DIR/utils.c"
)

SAN_FLAGS=(
    -fsanitize=address
    -fsanitize=null
    -g3
)

WARN_FLAGS=(
    -Wall
    -Wextra
    -Werror=incompatible-pointer-types
    -Werror=int-conversion
)

LLVM_CFLAGS=( $(llvm-config-14 --cflags 2>/dev/null) )
LLVM_LDFLAGS=( $(llvm-config-14 --ldflags --libs core 2>/dev/null) )

CFLAGS=("${SAN_FLAGS[@]}" "${WARN_FLAGS[@]}" "${LLVM_CFLAGS[@]}" "${LLVM_LDFLAGS[@]}")

export CFLAGS SRC_FILES ROOT_DIR SRC_DIR BUILD_DIR TESTS_DIR LLVM_DIR
export PATH="$PATH:$BUILD_DIR"

# =========================================================
#  Dependency Check
# =========================================================

# Returns install instructions for a given OS
_install_hint() {
    case "$OS_TYPE" in
        macos)   echo "brew install llvm@14" ;;
        linux)
            if command -v apt-get &>/dev/null; then
                echo "sudo apt install llvm-14 clang-14"
            elif command -v pacman &>/dev/null; then
                echo "sudo pacman -S llvm clang"
            elif command -v dnf &>/dev/null; then
                echo "sudo dnf install llvm14 clang"
            else
                echo "install llvm-14 and clang via your package manager"
            fi
            ;;
        windows) echo "pacman -S mingw-w64-x86_64-llvm mingw-w64-x86_64-clang" ;;
        *)       echo "install llvm-14 and clang for your platform" ;;
    esac
}

check() {
    local missing=0

    echo ""
    echo -e "  ${BOLD}Dependency Check${RESET}"
    echo -e "  ${DIM}────────────────────────────────────${RESET}"

    # clang
    if command -v clang &>/dev/null; then
        local cv
        cv=$(clang --version 2>&1 | head -1)
        echo -e "  ${GREEN}✓${RESET}  clang              ${DIM}$cv${RESET}"
    else
        echo -e "  ${RED}✗${RESET}  clang              ${RED}not found${RESET}"
        ((missing++))
    fi

    # llvm-config-14
    if command -v llvm-config-14 &>/dev/null; then
        local lv
        lv=$(llvm-config-14 --version 2>&1)
        echo -e "  ${GREEN}✓${RESET}  llvm-config-14     ${DIM}LLVM $lv${RESET}"
    else
        echo -e "  ${RED}✗${RESET}  llvm-config-14     ${RED}not found${RESET}"
        ((missing++))
    fi

    # llc
    if command -v llc &>/dev/null; then
        local lcv
        lcv=$(llc --version 2>&1 | grep -o 'version [0-9.]*' | head -1)
        echo -e "  ${GREEN}✓${RESET}  llc                ${DIM}$lcv${RESET}"
    else
        echo -e "  ${RED}✗${RESET}  llc                ${RED}not found${RESET}"
        ((missing++))
    fi

    # clang-format (optional)
    if command -v clang-format &>/dev/null; then
        local cfv
        cfv=$(clang-format --version 2>&1 | head -1)
        echo -e "  ${GREEN}✓${RESET}  clang-format       ${DIM}$cfv${RESET}"
    else
        echo -e "  ${YELLOW}–${RESET}  clang-format       ${DIM}not found (optional, needed for indent)${RESET}"
    fi

    echo ""

    if [[ $missing -eq 0 ]]; then
        echo -e "  ${GREEN}${BOLD}All good!${RESET}  Commands: ${CYAN}build  copy  tests  check  install  help${RESET}"
    else
        echo -e "  ${RED}${BOLD}$missing missing.${RESET}  Run: ${CYAN}install${RESET}"
        echo -e "  ${DIM}Or manually: $(_install_hint)${RESET}"
    fi

    echo ""
    export _URA_DEPS_CHECKED=1
    return $missing
}

# =========================================================
#  Install Missing Dependencies
# =========================================================
install() {
    echo -e "${YELLOW}Installing dependencies for ${OS_TYPE}...${RESET}"
    echo ""

    case "$OS_TYPE" in
        macos)
            if ! command -v brew &>/dev/null; then
                echo -e "${RED}Homebrew not found.${RESET} Install it first:"
                echo -e "  ${CYAN}/bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"${RESET}"
                return 1
            fi
            brew install llvm@14 clang-format

            # llvm@14 is keg-only — add its bin to PATH now and persist it
            local llvm_prefix
            llvm_prefix="$(brew --prefix llvm@14 2>/dev/null)"
            if [[ -d "$llvm_prefix/bin" ]]; then
                export PATH="$llvm_prefix/bin:$PATH"
                echo ""
                echo -e "${GREEN}Added llvm@14 to PATH for this session.${RESET}"

                # Persist to shell profile
                local shell_profile=""
                if [[ -n "$ZSH_VERSION" ]]; then
                    shell_profile="$HOME/.zshrc"
                elif [[ -n "$BASH_VERSION" ]]; then
                    shell_profile="$HOME/.bash_profile"
                fi
                if [[ -n "$shell_profile" ]]; then
                    local export_line="export PATH=\"$llvm_prefix/bin:\$PATH\""
                    if ! grep -qF "$export_line" "$shell_profile" 2>/dev/null; then
                        echo "" >> "$shell_profile"
                        echo "# llvm@14 (added by ura install)" >> "$shell_profile"
                        echo "$export_line" >> "$shell_profile"
                        echo -e "${GREEN}Persisted to $shell_profile${RESET}"
                    fi
                fi
            fi
            ;;
        linux)
            if command -v apt-get &>/dev/null; then
                sudo apt-get update && sudo apt-get install -y llvm-14 clang-14 llvm-14-tools clang-format
                # Point all unversioned commands to the -14 versions
                for tool in clang clang++ llc llvm-config; do
                    local versioned="${tool}-14"
                    if command -v "$versioned" &>/dev/null; then
                        sudo ln -sf "$(command -v "$versioned")" "/usr/local/bin/$tool"
                        echo -e "  ${DIM}linked $tool -> $versioned${RESET}"
                    fi
                done
            elif command -v pacman &>/dev/null; then
                sudo pacman -S --noconfirm llvm clang clang-format
            elif command -v dnf &>/dev/null; then
                sudo dnf install -y llvm14 clang clang-format
            else
                echo -e "${RED}Unknown package manager.${RESET} Install llvm-14, clang, and clang-format manually."
                return 1
            fi
            ;;
        windows)
            if command -v pacman &>/dev/null; then
                pacman -S --noconfirm mingw-w64-x86_64-llvm mingw-w64-x86_64-clang mingw-w64-x86_64-clang-tools-extra
                # MSYS2 installs llvm-config (unversioned) — create llvm-config-14 symlink
                for tool in llvm-config llc clang; do
                    local versioned="${tool}-14"
                    if command -v "$tool" &>/dev/null && ! command -v "$versioned" &>/dev/null; then
                        local tool_dir
                        tool_dir="$(dirname "$(command -v "$tool")")"
                        ln -sf "$(command -v "$tool")" "$tool_dir/$versioned"
                        echo -e "  ${DIM}linked $versioned -> $tool${RESET}"
                    fi
                done
            else
                echo -e "${RED}MSYS2 pacman not found.${RESET}"
                echo -e "Install MSYS2 from ${CYAN}https://www.msys2.org${RESET} then re-run."
                return 1
            fi
            ;;
        *)
            echo -e "${RED}Unknown OS.${RESET} Install llvm-14, clang, and clang-format manually."
            return 1
            ;;
    esac

    echo ""
    echo -e "${GREEN}Done!${RESET} Re-source to verify: ${CYAN}source config.sh${RESET}"
}

# =========================================================
#  Build
# =========================================================
build() {
    mkdir -p "$BUILD_DIR"
    local _tmp
    _tmp=$(mktemp)
    if clang "${SRC_FILES[@]}" "${CFLAGS[@]}" -o "$URA_COMPILER" 2>"$_tmp"; then
        grep -v "^ld: warning:" "$_tmp" >&2 || true
        rm -f "$_tmp"
        echo -e "${GREEN}Built compiler${RESET}"
    else
        cat "$_tmp" >&2
        rm -f "$_tmp"
        echo -e "${RED}Build failed${RESET}"
        return 1
    fi
}

# =========================================================
#  Release helpers
# =========================================================

# _sync_repo <local_src_dir> <git_remote> <label>
_sync_repo() {
    local src="$1" remote="$2" label="$3"
    echo -e "${YELLOW}Syncing $label...${RESET}"
    local _tmp
    _tmp=$(mktemp -d)
    if ! git clone --quiet "$remote" "$_tmp"; then
        echo -e "${RED}Failed to clone $label${RESET}"
        rm -rf "$_tmp"
        return 1
    fi
    find "$_tmp" -mindepth 1 -not -path "$_tmp/.git*" -delete
    # Exclude .git from source — src may itself be a git repo
    rsync -a --exclude='.git' "$src/" "$_tmp/"
    local _changed
    _changed=$(git -C "$_tmp" status --porcelain)
    if [[ -z "$_changed" ]]; then
        echo -e "${DIM}$label unchanged, nothing to push${RESET}"
    else
        git -C "$_tmp" add -A
        git -C "$_tmp" commit -m "sync: $label $(date +%Y-%m-%d)"
        git -C "$_tmp" push --quiet
        echo -e "${GREEN}$label pushed${RESET}"
    fi
    rm -rf "$_tmp"
}

release_projects() {
    local projects_dir="$ROOT_DIR/tests/projects"
    if [[ ! -d "$projects_dir" ]]; then
        echo -e "${RED}No tests/projects directory found${RESET}"
        return 1
    fi
    for project_dir in "$projects_dir"/*/; do
        local name
        name=$(basename "$project_dir")
        _sync_repo "$project_dir" "git@github.com:mohammedhrima/$name.git" "$name"
    done
}

release_extension() {
    _sync_repo "$ROOT_DIR/config/vscode-extension" "git@github.com:mohammedhrima/ura-vscode-extension.git" "vscode-extension"
}

update_projects() {
    local projects_dir="$ROOT_DIR/tests/projects"
    local updated=0
    while IFS= read -r ura_file; do
        local rel="${ura_file#$ROOT_DIR/tests/}"
        rel="${rel%.ura}"
        local expected="// $rel"
        local first_line
        first_line=$(head -n 1 "$ura_file")
        if [[ "$first_line" == "$expected" ]]; then
            continue
        fi
        if [[ "$first_line" == //* ]]; then
            # Replace existing comment
            local rest
            rest=$(tail -n +2 "$ura_file")
            printf '%s\n%s\n' "$expected" "$rest" > "$ura_file"
        else
            # Prepend comment
            local content
            content=$(cat "$ura_file")
            printf '%s\n%s\n' "$expected" "$content" > "$ura_file"
        fi
        echo -e "  ${GREEN}updated${RESET} $rel"
        ((updated++))
    done < <(find "$projects_dir" -name "*.ura" | sort)
    echo -e "${GREEN}Done: $updated file(s) updated${RESET}"
}

# =========================================================
#  Release
# =========================================================
release() {
    # ── 1. Build binary ──────────────────────────────────────────
    mkdir -p "$BUILD_DIR"
    local _tmp
    _tmp=$(mktemp)
    echo -e "${YELLOW}Building release binary (no sanitizers, -O2)...${RESET}"
    if ! clang "${SRC_FILES[@]}" "${WARN_FLAGS[@]}" "${LLVM_CFLAGS[@]}" "${LLVM_LDFLAGS[@]}" \
        -O2 -DDEBUG=0 -o "$URA_COMPILER" 2>"$_tmp"; then
        cat "$_tmp" >&2
        rm -f "$_tmp"
        echo -e "${RED}Release build failed${RESET}"
        return 1
    fi
    grep -v "^ld: warning:" "$_tmp" >&2 || true
    rm -f "$_tmp"
    echo -e "${GREEN}Binary ready: $URA_COMPILER${RESET}"

    # ── 2. Sync ura-lib + projects + vscode extension ────────────
    _sync_repo "$SRC_DIR/ura-lib" "git@github.com:mohammedhrima/ura-lib.git" "ura-lib"
    release_projects
    release_extension

    # ── 3. Commit binary in ura-lang ─────────────────────────────
    echo ""
    echo -e "  ${BOLD}Next steps (binary):${RESET}"
    echo "    git add build/ura"
    echo "    git commit -m 'chore: release $(date +%Y-%m-%d)'"
    echo "    git push"
    echo ""
    echo -e "  ${BOLD}Install anywhere:${RESET}"
    echo "    curl -L https://raw.githubusercontent.com/mohammedhrima/ura-lang/main/build/ura -o ura && chmod +x ura"
    echo "    git clone https://github.com/mohammedhrima/ura-lib ura-lib"
}

# =========================================================
#  Test Helpers
# =========================================================
copy() {
    if [[ $# -lt 1 ]]; then
        echo -e "${RED}Usage: copy <file.ura>${RESET}"
        echo -e "${YELLOW}The file must have a first line comment: // <folder>/<filename>${RESET}"
        echo -e "${YELLOW}Example: // builtins/printf${RESET}"
        return 1
    fi

    local ura_src_file="$1"

    if [[ ! "$ura_src_file" = /* ]]; then
        ura_src_file="$SRC_DIR/$ura_src_file"
    fi

    if [[ ! -f "$ura_src_file" ]]; then
        echo -e "${RED}$ura_src_file not found${RESET}"
        return 1
    fi

    local first_line
    first_line=$(head -n 1 "$ura_src_file")

    if [[ "$first_line" =~ ^//[[:space:]]*([a-zA-Z0-9_/-]+) ]]; then
        local full_path="${match[1]}"
        full_path="${full_path%.ura}"
        local folder="${full_path%/*}"
        local filename="${full_path##*/}"
        local test_dir="$TESTS_DIR/$folder"
        local ura_dest="$test_dir/${filename}.ura"
        local ll_dest="$test_dir/${filename}.ll"
        echo -e "${YELLOW}Detected path: $folder/$filename${RESET}"
    else
        echo -e "${RED}Invalid or missing path comment in first line${RESET}"
        echo -e "${YELLOW}Expected format: // <folder>/<filename>${RESET}"
        return 1
    fi

    mkdir -p "$test_dir"

    build || return 1

    "$URA_COMPILER" "$ura_src_file" -no-exec || {
        echo -e "${RED}Compilation failed${RESET}"
        return 1
    }

    local src_dir
    src_dir=$(dirname "$ura_src_file")
    local src_basename
    src_basename=$(basename "$ura_src_file" .ura)
    local ll_src="$src_dir/build/${src_basename}.ll"

    if [[ ! -f "$ll_src" ]]; then
        echo -e "${RED}Generated IR not found at $ll_src${RESET}"
        return 1
    fi

    cp "$ura_src_file" "$ura_dest" || {
        echo -e "${RED}Failed to copy $ura_src_file -> $ura_dest${RESET}"
        return 1
    }

    cp "$ll_src" "$ll_dest" || {
        echo -e "${RED}Failed to copy IR -> $ll_dest${RESET}"
        return 1
    }

    echo -e "${GREEN}Saved test:${RESET}"
    echo "  $ura_dest"
    echo "  $ll_dest"
}

tests() {
    echo -e "${YELLOW}Running tests...${RESET}"

    build || return 1

    local targets=("$@")
    local ura_files=()

    if [[ ${#targets[@]} -eq 0 ]]; then
        while IFS= read -r f; do
            ura_files+=("$f")
        done < <(find "$TESTS_DIR" -name "*.ura" -not -path "*/build/*" | sort)
    else
        for target in "${targets[@]}"; do
            local target_dir="$target"
            [[ ! "$target_dir" = /* ]] && target_dir="$TESTS_DIR/$target"
            while IFS= read -r f; do
                ura_files+=("$f")
            done < <(find "$target_dir" -name "*.ura" -not -path "*/build/*" | sort)
        done
    fi

    # Suppress job-control notifications (config.sh is sourced in interactive shell)
    { set +m; } 2>/dev/null

    local tmpdir
    tmpdir=$(mktemp -d)
    local max_jobs=3

    # Launch all tests in parallel (throttled to max_jobs)
    local i=0
    for ura_file in "${ura_files[@]}"; do
        while (( $(jobs -rp 2>/dev/null | wc -l) >= max_jobs )); do
            wait -n 2>/dev/null || sleep 0.05
        done

        local result_file="$tmpdir/$i"
        (
            local rel="${ura_file#$TESTS_DIR/}"
            rel="${rel%.ura}"

            if [[ "$(head -n 1 "$ura_file")" != //* ]]; then
                printf 'SKIP\t%s\t\n' "$rel" > "$result_file"
                exit 0
            fi

            local base_name
            base_name=$(basename "$ura_file" .ura)
            local test_dir
            test_dir=$(dirname "$ura_file")
            local ll_expected="$test_dir/${base_name}.ll"
            local ll_got="$test_dir/build/${base_name}.ll"

            if [[ ! -f "$ll_expected" ]]; then
                printf 'FAIL\t%s\tno .ll reference\n' "$rel" > "$result_file"
                exit 0
            fi

            if ! "$URA_COMPILER" "$ura_file" -testing -no-exec > /dev/null 2>&1; then
                printf 'FAIL\t%s\tcompilation error\n' "$rel" > "$result_file"
                exit 0
            fi

            if [[ ! -f "$ll_got" ]]; then
                printf 'FAIL\t%s\tno IR generated\n' "$rel" > "$result_file"
                exit 0
            fi

            if diff -q \
                <(tail -n +4 "$ll_got"      | grep -v "DIFile\|DICompileUnit\|source_filename\|ModuleID") \
                <(tail -n +4 "$ll_expected" | grep -v "DIFile\|DICompileUnit\|source_filename\|ModuleID") \
                > /dev/null 2>&1; then
                printf 'PASS\t%s\t\n' "$rel" > "$result_file"
            else
                printf 'FAIL\t%s\tIR mismatch\n' "$rel" > "$result_file"
            fi
        ) &

        (( i++ ))
    done

    wait
    { set -m; } 2>/dev/null

    # Display via Python3 — immune to zsh xtrace no matter where it points
    python3 - "$tmpdir" "$i" << 'PYEOF'
import sys, os

G = '\033[0;32m'  # green
R = '\033[0;31m'  # red
Y = '\033[1;33m'  # yellow
E = '\033[0m'     # reset

tmpdir, n = sys.argv[1], int(sys.argv[2])
passed = failed = skipped = 0

for j in range(n):
    path = os.path.join(tmpdir, str(j))
    if not os.path.exists(path):
        continue
    with open(path) as f:
        parts = f.readline().rstrip('\n').split('\t', 2)
    status = parts[0] if parts else ''
    rel    = parts[1] if len(parts) > 1 else ''
    reason = parts[2].strip() if len(parts) > 2 else ''

    if status == 'PASS':
        print(f"{G}  PASS {rel}{E}")
        passed += 1
    elif status == 'FAIL':
        print(f"{R}  FAIL {rel} ({reason}){E}")
        failed += 1
    elif status == 'SKIP':
        print(f"{Y}  SKIP {rel}{E}")
        skipped += 1

print()
print(f"{G}Passed: {passed}{E}")
if failed:
    print(f"{R}Failed: {failed}{E}")
if skipped:
    print(f"{Y}Skipped: {skipped}{E}")

with open(os.path.join(tmpdir, '_ret'), 'w') as f:
    f.write(str(failed))
PYEOF

    local _ret
    _ret=$(cat "$tmpdir/_ret" 2>/dev/null || echo 0)
    rm -rf "$tmpdir"
    return $_ret
}

update_tests() {
    echo -e "${YELLOW}Updating all test .ll files...${RESET}"
    build || return 1

    local failed=0
    local passed=0

    for ura_file in $(find "$TESTS_DIR" -name "*.ura" -not -path "*/build/*"); do
        local dir
        dir=$(dirname "$ura_file")
        local base
        base=$(basename "$ura_file" .ura)
        local ll_expected="$dir/${base}.ll"
        local ll_generated="$dir/build/${base}.ll"

        if ! "$URA_COMPILER" "$ura_file" -no-exec > /dev/null 2>&1; then
            echo -e "  ${RED}FAIL $base (compilation error)${RESET}"
            ((failed++))
            continue
        fi

        if [[ ! -f "$ll_generated" ]]; then
            echo -e "  ${RED}FAIL $base (no IR generated)${RESET}"
            ((failed++))
            continue
        fi

        cp "$ll_generated" "$ll_expected"
        echo -e "  ${GREEN}UPDATED $base.ll${RESET}"
        ((passed++))
    done

    echo ""
    echo -e "${GREEN}Updated: $passed${RESET}"
    [[ $failed -gt 0 ]] && echo -e "${RED}Failed: $failed${RESET}"
}

# =========================================================
#  Examples Generation
# =========================================================
examples() {
    echo -e "${YELLOW}Generating examples.ura...${RESET}"

    local output_file="$ROOT_DIR/examples.ura"

    cat > "$output_file" << 'EOF'
// ============================================================================
// URA LANGUAGE EXAMPLES
// ============================================================================
// This file contains all test examples from the Ura language test suite.
// Each section demonstrates different features and capabilities of the language.
// ============================================================================

EOF

    local count=0
    for file in $(find "$TESTS_DIR" -name "*.ura" -type f | sort); do
        local category
        category=$(dirname "$file" | sed "s|$TESTS_DIR/||")
        local filename
        filename=$(basename "$file" .ura)
        local relative_path
        relative_path=$(echo "$file" | sed "s|$ROOT_DIR/||")

        cat >> "$output_file" << EOF

// ============================================================================
// $(echo $category | tr '[:lower:]' '[:upper:]'): $filename
// File: $relative_path
// ============================================================================

EOF
        cat "$file" >> "$output_file"
        echo "" >> "$output_file"
        ((count++))
    done

    echo -e "${GREEN}Generated examples.ura with $count examples${RESET}"
    echo -e "  Output: $output_file"
}

# =========================================================
#  Formatting
# =========================================================
indent() {
    echo -e "${YELLOW}Formatting...${RESET}"
    if ! command -v clang-format &>/dev/null; then
        echo -e "${RED}clang-format not found.${RESET} Install it and re-run."
        return 1
    fi
    local files=("$SRC_DIR"/**/*.c "$SRC_DIR"/**/*.h)
    clang-format -i "${files[@]}"
    python3 "$ROOT_DIR/config/merge_cases.py" "${files[@]}"
}

# =========================================================
#  Reload Config
# =========================================================
update() {
    echo -e "${YELLOW}Reloading config...${RESET}"
    source "$CONFIG_FILE"
}

# =========================================================
#  Leak Checking
# =========================================================
check_leaks() {
    if [[ $# -lt 1 ]]; then
        echo -e "${RED}Usage: check_leaks <executable>${RESET}"
        return 1
    fi
    ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions="$SRC_DIR/lsan.supp" "$1"
}

# =========================================================
#  Help
# =========================================================
help() {
    echo ""
    echo -e "  ${BOLD}Ura Environment Commands${RESET}"
    echo -e "  ${DIM}────────────────────────────────────${RESET}"
    echo ""
    echo -e "  ${CYAN}build${RESET}                   Build the ura compiler (with sanitizers)"
    echo -e "  ${CYAN}release${RESET}                 Build binary + sync ura-lib + sync all projects"
    echo -e "  ${CYAN}release_projects${RESET}        Sync tests/projects/* to their GitHub repos"
    echo -e "  ${CYAN}release_extension${RESET}       Sync config/vscode-extension to its GitHub repo"
    echo -e "  ${CYAN}update_projects${RESET}         Fix first-line path comment in all tests/projects/*.ura files"
    echo -e "  ${CYAN}copy <file.ura>${RESET}         Save file + IR as a test (reads path from first line comment)"
    echo -e "  ${CYAN}tests [folder]${RESET}          Run all tests (optionally filter by folder)"
    echo -e "  ${CYAN}update_tests${RESET}            Regenerate all expected .ll files"
    echo -e "  ${CYAN}examples${RESET}                Generate examples.ura from all test files"
    echo -e "  ${CYAN}indent${RESET}                  Format all .c and .h files in src/"
    echo -e "  ${CYAN}check${RESET}                   Re-run dependency check"
    echo -e "  ${CYAN}install${RESET}                 Install missing dependencies for this platform"
    echo -e "  ${CYAN}update${RESET}                  Reload config.sh"
    echo ""
    echo -e "  ${BOLD}Compiler flags${RESET}"
    echo ""
    echo -e "  ${DIM}-O0 -O1 -O2 -O3 -Os -Oz${RESET}   Optimization level"
    echo -e "  ${DIM}-san${RESET}                        Enable AddressSanitizer"
    echo -e "  ${DIM}-o <name>${RESET}                   Output executable name (default: exe.out)"
    echo -e "  ${DIM}-no-exec${RESET}                    Compile only, don't run"
    echo ""
    echo -e "  ${BOLD}Example${RESET}"
    echo ""
    echo -e "  ${DIM}\$URA_COMPILER src/file.ura -O2 -o myapp${RESET}"
    echo ""
}

# =========================================================
#  Shell Prompt
# =========================================================
if [[ -n "$ZSH_VERSION" ]]; then
    PROMPT="(ura-env) %n@%1~ %# "
elif [[ -n "$BASH_VERSION" ]]; then
    PS1="(ura-env) \u@\W \$ "
fi

# =========================================================
#  Initialization — run check once per session
# =========================================================
echo ""
echo -e "  ${BOLD}╔══════════════════════════════╗${RESET}"
echo -e "  ${BOLD}║     Ura Development Env      ║${RESET}"
echo -e "  ${BOLD}╚══════════════════════════════╝${RESET}"

if [[ -z "$_URA_DEPS_CHECKED" ]]; then
    check
else
    echo ""
    echo -e "  ${GREEN}Environment ready.${RESET}  Commands: ${CYAN}build  copy  tests  check  install  help${RESET}"
    echo ""
fi
