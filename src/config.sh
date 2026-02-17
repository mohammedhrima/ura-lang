#!/usr/bin/env bash

# =========================================================
#  Paths & Configuration
# =========================================================
CONFIG_FILE="${BASH_SOURCE[0]:-$0}"
ROOT_DIR="$(cd "$(dirname "$CONFIG_FILE")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
TESTS_DIR="$ROOT_DIR/tests"

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
URA_COMPILER="$ROOT_DIR/build/ura"
SRC_FILES=(
    "$ROOT_DIR/main.c" 
    "$ROOT_DIR/gen.c" 
    "$ROOT_DIR/utils.c"
    "$ROOT_DIR/llvm.c"
    )

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
export CFLAGS SRC_FILES ROOT_DIR BUILD_DIR TESTS_DIR

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

ir() {
    if [[ ! -f "$ROOT_DIR/file.ura" ]]; then
        echo -e "${RED}file.ura not found${RESET}"
        return 1
    fi
    
    mkdir -p "$BUILD_DIR"
    "$URA_COMPILER" "$ROOT_DIR/file.ura" || {
        echo -e "${RED}Compilation to IR failed${RESET}"
        return 1
    }
    
    [[ -f "$ROOT_DIR/file.ll" ]] && mv "$ROOT_DIR/file.ll" "$BUILD_DIR/file.ll"
    echo -e "${GREEN}Generated IR${RESET}"
}

asm() {
    local ll_file="$BUILD_DIR/file.ll"
    local s_file="$BUILD_DIR/file.s"
    
    if [[ ! -f "$ll_file" ]]; then
        echo -e "${RED}$ll_file not found. Run 'ir' first${RESET}"
        return 1
    fi
    
    llc "$ll_file" -o "$s_file" || {
        echo -e "${RED}Assembly generation failed${RESET}"
        return 1
    }
    
    clang "$s_file" -lc++ -o "$BUILD_DIR/exe.out" || {
        echo -e "${RED}Linking failed${RESET}"
        return 1
    }
    
    echo -e "${GREEN}Generated assembly and executable${RESET}"
}

comp() {
    build && ir && asm
}

run() {
    comp && "$BUILD_DIR/exe.out"
}

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
    local ura_src_file="$ROOT_DIR/file.ura"
    local ura_dest="$test_dir/${filename}.ura"
    local ll_dest="$test_dir/${filename}.ll"

    # Check if source file exists
    if [[ ! -f "$ura_src_file" ]]; then
        echo -e "${RED}$ura_src_file not found${RESET}"
        return 1
    fi

    # Create test directory
    mkdir -p "$test_dir"

    # Build and compile to IR
    build || return 1
    ir || return 1

    local ll_src="$BUILD_DIR/file.ll"
    if [[ ! -f "$ll_src" ]]; then
        echo -e "${RED}Generated IR not found${RESET}"
        return 1
    fi

    # Copy files
    cp "$ura_src_file" "$ura_dest" || {
        echo -e "${RED}Failed to copy $ura_src_file${RESET}"
        return 1
    }

    cp "$ll_src" "$ll_dest" || {
        echo -e "${RED}Failed to copy IR file${RESET}"
        return 1
    }

    echo -e "${GREEN}Copied:${RESET}"
    echo "  $ura_dest"
    echo "  $ll_dest"
}

indent() {
    echo -e "${YELLOW}Formatting code...${RESET}"
    astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
       --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
       --max-code-length=150 --break-after-logical \
       --suffix=none "$ROOT_DIR"/*.c "$ROOT_DIR"/*.h
    echo -e "${GREEN}Code formatted${RESET}"
}

tests() {
    echo -e "${YELLOW}Running tests...${RESET}"
    mkdir -p "$BUILD_DIR"
    build || return 1
    
    local failed=0
    local passed=0
    local folder_filter="${1:-.}"  # Default to all folders (.)

    for ura_file in "$TESTS_DIR"/$folder_filter/**/*.ura; do
        [[ -e "$ura_file" ]] || continue

        local base_name=$(basename "$ura_file" .ura)
        local dir_name=$(basename "$(dirname "$ura_file")")
        local ll_file="$(dirname "$ura_file")/${base_name}.ll"
        local tmp_ll="$BUILD_DIR/tmp_${dir_name}_${base_name}.ll"

        # Copy test file to build directory
        cp "$ura_file" "$BUILD_DIR/test.ura"

        # Compile to IR
        if ! "$URA_COMPILER" "$BUILD_DIR/test.ura" > /dev/null 2>&1; then
            echo -e "  ${RED}$dir_name/$base_name (compilation error)${RESET}"
            ((failed++))
            continue
        fi

        # Check if generated .ll exists
        if [[ ! -f "$BUILD_DIR/test.ll" ]]; then
            echo -e "  ${RED}$dir_name/$base_name (no IR generated)${RESET}"
            ((failed++))
            continue
        fi

        # Compare generated .ll with expected .ll
        if [[ ! -f "$ll_file" ]]; then
            echo -e "  ${RED}$dir_name/$base_name (expected .ll not found)${RESET}"
            ((failed++))
            continue
        fi

        if diff -q <(tail -n +3 "$BUILD_DIR/test.ll") <(tail -n +3 "$ll_file") > /dev/null 2>&1; then
            echo -e "  ${GREEN}$dir_name/$base_name${RESET}"
            ((passed++))
        else
            echo -e "  ${RED}$dir_name/$base_name (output mismatch)${RESET}"
            ((failed++))
        fi
    done

    # Cleanup
    rm -f "$BUILD_DIR/test.ura" "$BUILD_DIR/test.ll"

    echo ""
    echo -e "${GREEN}Passed: $passed${RESET}"
    [[ $failed -gt 0 ]] && echo -e "${RED}Failed: $failed${RESET}"

    return $failed
}

update() {
    echo -e "${YELLOW}Reloading config${RESET}"
    source "$CONFIG_FILE"
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