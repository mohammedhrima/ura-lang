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

SRC_FILES=(
    "$SRC_DIR/main.c"
    "$SRC_DIR/llvm.c"
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

    # uncrustify (optional)
    if command -v uncrustify &>/dev/null; then
        local uv
        uv=$(uncrustify --version 2>&1 | head -1)
        echo -e "  ${GREEN}✓${RESET}  uncrustify         ${DIM}$uv${RESET}"
    else
        echo -e "  ${YELLOW}–${RESET}  uncrustify         ${DIM}not found (optional, needed for indent)${RESET}"
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
            brew install llvm@14 uncrustify

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
                sudo apt-get update && sudo apt-get install -y llvm-14 clang-14 llvm-14-tools uncrustify
                # Point all unversioned commands to the -14 versions
                for tool in clang clang++ llc llvm-config; do
                    local versioned="${tool}-14"
                    if command -v "$versioned" &>/dev/null; then
                        sudo ln -sf "$(command -v "$versioned")" "/usr/local/bin/$tool"
                        echo -e "  ${DIM}linked $tool -> $versioned${RESET}"
                    fi
                done
            elif command -v pacman &>/dev/null; then
                sudo pacman -S --noconfirm llvm clang uncrustify
            elif command -v dnf &>/dev/null; then
                sudo dnf install -y llvm14 clang uncrustify
            else
                echo -e "${RED}Unknown package manager.${RESET} Install llvm-14, clang, and uncrustify manually."
                return 1
            fi
            ;;
        windows)
            if command -v pacman &>/dev/null; then
                pacman -S --noconfirm mingw-w64-x86_64-llvm mingw-w64-x86_64-clang mingw-w64-x86_64-uncrustify
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
            echo -e "${RED}Unknown OS.${RESET} Install llvm-14, clang, and uncrustify manually."
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
#  Release
# =========================================================
release() {
    # ── 1. Build binary ──────────────────────────────────────────
    mkdir -p "$BUILD_DIR"
    local _tmp
    _tmp=$(mktemp)
    echo -e "${YELLOW}Building release binary (no sanitizers, -O2)...${RESET}"
    if ! clang "${SRC_FILES[@]}" "${WARN_FLAGS[@]}" "${LLVM_CFLAGS[@]}" "${LLVM_LDFLAGS[@]}" \
        -O2 -o "$URA_COMPILER" 2>"$_tmp"; then
        cat "$_tmp" >&2
        rm -f "$_tmp"
        echo -e "${RED}Release build failed${RESET}"
        return 1
    fi
    grep -v "^ld: warning:" "$_tmp" >&2 || true
    rm -f "$_tmp"
    echo -e "${GREEN}Binary ready: $URA_COMPILER${RESET}"

    # ── 2. Sync ura-lib to its own repo ──────────────────────────
    echo -e "${YELLOW}Syncing ura-lib to github.com/mohammedhrima/ura-lib...${RESET}"
    local _lib_tmp
    _lib_tmp=$(mktemp -d)
    if ! git clone --quiet git@github.com:mohammedhrima/ura-lib.git "$_lib_tmp"; then
        echo -e "${RED}Failed to clone ura-lib repo${RESET}"
        rm -rf "$_lib_tmp"
        return 1
    fi

    # Replace contents (keep .git)
    find "$_lib_tmp" -mindepth 1 -not -path "$_lib_tmp/.git*" -delete
    cp -r "$SRC_DIR/ura-lib/." "$_lib_tmp/"

    local _changed
    _changed=$(git -C "$_lib_tmp" status --porcelain)
    if [[ -z "$_changed" ]]; then
        echo -e "${DIM}ura-lib unchanged, nothing to push${RESET}"
    else
        local _tag
        _tag=$(date +%Y-%m-%d)
        git -C "$_lib_tmp" add -A
        git -C "$_lib_tmp" commit -m "sync: ura-lib $_tag"
        git -C "$_lib_tmp" push --quiet
        echo -e "${GREEN}ura-lib pushed (commit: sync: ura-lib $_tag)${RESET}"
    fi
    rm -rf "$_lib_tmp"

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
    echo "first_line: '$first_line'"

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

    local failed=0
    local passed=0
    local targets=("$@")
    local ura_files=()

    if [[ ${#targets[@]} -eq 0 ]]; then
        while IFS= read -r f; do
            ura_files+=("$f")
        done < <(find "$TESTS_DIR" -name "*.ura" | sort)
    else
        for target in "${targets[@]}"; do
            local target_dir="$target"
            [[ ! "$target_dir" = /* ]] && target_dir="$TESTS_DIR/$target"
            while IFS= read -r f; do
                ura_files+=("$f")
            done < <(find "$target_dir" -name "*.ura" | sort)
        done
    fi

    for ura_file in "${ura_files[@]}"; do
        [[ -e "$ura_file" ]] || continue

        local base_name=$(basename "$ura_file" .ura)
        local test_dir=$(dirname "$ura_file")
        local rel_path="${ura_file#$TESTS_DIR/}"
        rel_path="${rel_path%.ura}"

        local ll_expected="$test_dir/${base_name}.ll"
        local ll_got="$test_dir/build/${base_name}.ll"

        if [[ ! -f "$ll_expected" ]]; then
            echo -e "  ${RED}FAIL $rel_path (expected .ll not found)${RESET}"
            ((failed++))
            continue
        fi

        if ! "$URA_COMPILER" "$ura_file" -testing -no-exec > /dev/null 2>&1; then
            echo -e "  ${RED}FAIL $rel_path (compilation error)${RESET}"
            ((failed++))
            continue
        fi

        if [[ ! -f "$ll_got" ]]; then
            echo -e "  ${RED}FAIL $rel_path (no IR generated)${RESET}"
            ((failed++))
            continue
        fi

        if diff -q \
            <(tail -n +4 "$ll_got"      | grep -v "DIFile\|DICompileUnit\|source_filename\|ModuleID") \
            <(tail -n +4 "$ll_expected" | grep -v "DIFile\|DICompileUnit\|source_filename\|ModuleID") \
            > /dev/null 2>&1; then
            echo -e "  ${GREEN}PASS $rel_path${RESET}"
            ((passed++))
        else
            echo -e "  ${RED}FAIL $rel_path (IR mismatch)${RESET}"
            ((failed++))
        fi
    done

    echo ""
    echo -e "${GREEN}Passed: $passed${RESET}"
    [[ $failed -gt 0 ]] && echo -e "${RED}Failed: $failed${RESET}"

    return $failed
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
    echo -e "${YELLOW}Formatting with Uncrustify...${RESET}"
    if ! command -v uncrustify &>/dev/null; then
        echo -e "${RED}uncrustify not found.${RESET} Install it and re-run."
        return 1
    fi
    uncrustify -c "$ROOT_DIR/config/uncrustify.cfg" --no-backup "$SRC_DIR"/*.c "$SRC_DIR"/*.h
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
  echo -e "  ${CYAN}release${RESET}                 Build a release binary to build/ura (no sanitizers, git-tracked)"
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
