#!/usr/bin/env bash

# =========================================================
#  Paths & Configuration
# =========================================================
CONFIG_FILE="/Users/hrimamohammed/Desktop/Personal/ura-lang/config.sh"
ROOT_DIR="/Users/hrimamohammed/Desktop/Personal/ura-lang"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build"
TESTS_DIR="$ROOT_DIR/tests"
LLVM_DIR="$ROOT_DIR/llvm"
export URA_LIB="$SRC_DIR/ura-lib"

# =========================================================
#  Color Definitions
# =========================================================
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# =========================================================
#  Compiler Configuration
# =========================================================
URA_COMPILER="$BUILD_DIR/ura"

TMP_FLAG=1

if [ "$TMP_FLAG" -eq 0 ]; then
    SRC_FILES=(
        "$SRC_DIR/main.c"
        "$SRC_DIR/utils.c"
        "$SRC_DIR/gen.c"
        "$SRC_DIR/llvm.c"
    )
else
    SRC_FILES=(
        "$SRC_DIR/main.c"
        "$SRC_DIR/llvm.c"
    )
fi

# Compiler Flags
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

LLVM_CFLAGS=( $(llvm-config --cflags) )
LLVM_LDFLAGS=( $(llvm-config --ldflags --libs core) )

CFLAGS=("${SAN_FLAGS[@]}" "${WARN_FLAGS[@]}" "${LLVM_CFLAGS[@]}" "${LLVM_LDFLAGS[@]}")

# =========================================================
#  Export Variables for Shell Access
# =========================================================
export CFLAGS SRC_FILES ROOT_DIR SRC_DIR BUILD_DIR TESTS_DIR LLVM_DIR PATH=$PATH:$BUILD_DIR

# =========================================================
#  Build Functions
# =========================================================
build() {
    mkdir -p "$BUILD_DIR"
    clang "${SRC_FILES[@]}" "${CFLAGS[@]}" -o "$URA_COMPILER" || {
        echo -e "${RED}Build failed${RESET}"
        return 1
    }
    echo -e "${GREEN}Built compiler${RESET}"
}

# =========================================================
#  Test Helpers
# =========================================================
copy() {
    if [[ $# -lt 2 ]]; then
        echo -e "${RED}Usage: copy <folder> <filename>${RESET}"
        echo "  folder:   test folder name (e.g., builtins, op, data_types)"
        echo "  filename: file name without .ura extension"
        return 1
    fi

    local folder="$1"
    local filename="$2"
    local test_dir="$TESTS_DIR/$folder"
    local ura_src_file="$SRC_DIR/file.ura"
    local ura_dest="$test_dir/${filename}.ura"
    local ll_dest="$test_dir/${filename}.ll"

    if [[ ! -f "$ura_src_file" ]]; then
        echo -e "${RED}$ura_src_file not found${RESET}"
        return 1
    fi

    mkdir -p "$test_dir"

    build || return 1

    # compile — ura puts .ll in src/build/
    "$URA_COMPILER" "$ura_src_file" || {
        echo -e "${RED}Compilation failed${RESET}"
        return 1
    }

    local ll_src="$SRC_DIR/build/file.ll"
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
    local folder_filter="${1:-.}"
    local base_name dir_name ll_expected tmp_dir tmp_ura ll_got

    for ura_file in "$TESTS_DIR"/$folder_filter/**/*.ura; do
        [[ -e "$ura_file" ]] || continue

        base_name=$(basename "$ura_file" .ura)
        dir_name=$(basename "$(dirname "$ura_file")")
        ll_expected="$(dirname "$ura_file")/${base_name}.ll"
        tmp_dir=$(mktemp -d 2>/dev/null)
        tmp_ura="$tmp_dir/${base_name}.ura"

        cp "$ura_file" "$tmp_ura"

        if ! "$URA_COMPILER" "$tmp_ura" > /dev/null 2>&1; then
            echo -e "  ${RED}FAIL $dir_name/$base_name (compilation error)${RESET}"
            ((failed++)); rm -rf "$tmp_dir"; continue
        fi

        ll_got="$tmp_dir/build/${base_name}.ll"

        if [[ ! -f "$ll_got" ]]; then
            echo -e "  ${RED}FAIL $dir_name/$base_name (no IR generated)${RESET}"
            ((failed++)); rm -rf "$tmp_dir"; continue
        fi

        if [[ ! -f "$ll_expected" ]]; then
            echo -e "  ${RED}FAIL $dir_name/$base_name (expected .ll not found)${RESET}"
            ((failed++)); rm -rf "$tmp_dir"; continue
        fi

        if diff -q <(tail -n +4 "$ll_got") <(tail -n +4 "$ll_expected") > /dev/null 2>&1; then
            echo -e "  ${GREEN}PASS $dir_name/$base_name${RESET}"
            ((passed++))
        else
            echo -e "  ${RED}FAIL $dir_name/$base_name (IR mismatch)${RESET}"
            ((failed++))
        fi

        rm -rf "$tmp_dir"
    done

    echo ""
    echo -e "${GREEN}Passed: $passed${RESET}"
    [[ $failed -gt 0 ]] && echo -e "${RED}Failed: $failed${RESET}"

    return $failed
}

# =========================================================
#  LLVM Sandbox
# =========================================================
llvm_build() {
    mkdir -p "$LLVM_DIR/build"
    clang "$LLVM_DIR/main.c" "$LLVM_DIR/utils.c" "$LLVM_DIR/wrapper.c" \
        "${LLVM_CFLAGS[@]}" "${LLVM_LDFLAGS[@]}" \
        -o "$LLVM_DIR/build/llvm_sandbox" || {
        echo -e "${RED}LLVM sandbox build failed${RESET}"
        return 1
    }
    echo -e "${GREEN}Built LLVM sandbox${RESET}"
}

# =========================================================
#  Formatting
# =========================================================
indent() {
    echo -e "${YELLOW}Formatting with Uncrustify...${RESET}"
    if [ -z "$SRC_DIR" ]; then
        echo "SRC_DIR is not set!"
        return 1
    fi
    uncrustify -c uncrustify.cfg --no-backup "$SRC_DIR"/*.c "$SRC_DIR"/*.h
}

# =========================================================
#  Reload Config
# =========================================================
update() {
    echo -e "${YELLOW}Reloading config...${RESET}"
    source "$CONFIG_FILE"
    echo -e "${GREEN}Config reloaded${RESET}"
}

# =========================================================
#  Help
# =========================================================
help() {
    echo -e "${BOLD}Ura Environment Commands:${RESET}"
    echo ""
    echo -e "  ${GREEN}build${RESET}                        Build the ura compiler"
    echo -e "  ${GREEN}copy <folder> <name>${RESET}         Save src/file.ura + its IR to tests/<folder>/<name>"
    echo -e "  ${GREEN}tests [folder]${RESET}               Run all tests (optionally filter by folder)"
    echo -e "  ${GREEN}indent${RESET}                       Format all .c and .h files in src/"
    echo -e "  ${GREEN}update${RESET}                       Reload config.sh"
    echo -e "  ${GREEN}llvm_build${RESET}                   Build the LLVM sandbox"
    echo ""
    echo -e "${BOLD}Ura Compiler Usage:${RESET}"
    echo ""
    echo -e "  ${GREEN}ura <file.ura> [options]${RESET}"
    echo ""
    echo -e "  ${CYAN}Optimization:${RESET}"
    echo -e "    -O0   No optimization (debug)"
    echo -e "    -O1   Basic optimization (mem2reg, instcombine)"
    echo -e "    -O2   Standard optimization (default for production)"
    echo -e "    -O3   Aggressive optimization"
    echo -e "    -Os   Optimize for size"
    echo -e "    -Oz   Minimize size"
    echo ""
    echo -e "  ${CYAN}Sanitizers:${RESET}"
    echo -e "    -asan  Enable AddressSanitizer (detects memory errors)"
    echo -e "           Link with: clang -fsanitize=address"
    echo ""
    echo -e "  ${CYAN}Output:${RESET}"
    echo -e "    -o <name>  Set executable name (default: exe.out)"
    echo ""
    echo -e "${BOLD}Examples:${RESET}"
    echo ""
    echo -e "  ${YELLOW}\$URA_COMPILER src/file.ura${RESET}"
    echo -e "  ${YELLOW}\$URA_COMPILER src/file.ura -O2 -o myapp${RESET}"
    echo -e "  ${YELLOW}\$URA_COMPILER src/file.ura -asan -o myapp${RESET}"
    echo -e "  ${YELLOW}\$URA_COMPILER a.ura b.ura -O2 -o myapp${RESET}"
    echo ""
    echo -e "  Intermediate files go to ${CYAN}build/${RESET} next to each source file."
}

# =========================================================
#  Shell Prompt
# =========================================================
if [[ -n "$ZSH_VERSION" ]]; then
    PROMPT="(ura-env) %n@%1~ % "
elif [[ -n "$BASH_VERSION" ]]; then
    PS1="(ura-env) \u@\W \$ "
fi

# =========================================================
#  Initialization
# =========================================================
cd "$ROOT_DIR" 2>/dev/null
echo -e "${GREEN}Ura environment loaded!${RESET}"