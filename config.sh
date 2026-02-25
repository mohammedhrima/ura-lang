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
export CFLAGS SRC_FILES ROOT_DIR SRC_DIR BUILD_DIR TESTS_DIR LLVM_DIR

# =========================================================
#  Build Functions
# =========================================================
build() {
    mkdir -p "$BUILD_DIR"
    rm -rf "$BUILD_DIR"/ura* "$BUILD_DIR"/*.ll "$BUILD_DIR"/*.s "$BUILD_DIR"/*.out

    clang "${SRC_FILES[@]}" "${CFLAGS[@]}" -o "$URA_COMPILER" || {
        echo -e "${RED}Build failed${RESET}"
        return 1
    }
    echo -e "${GREEN}Built compiler${RESET}"
}

ir() {
    if [[ ! -f "$SRC_DIR/file.ura" ]]; then
        echo -e "${RED}file.ura not found in $SRC_DIR${RESET}"
        return 1
    fi

    "$URA_COMPILER" "$SRC_DIR/file.ura" || {
        echo -e "${RED}Compilation to IR failed${RESET}"
        return 1
    }

    # Move all .ll files generated alongside .ura in src/ to build/
    for ll in "$SRC_DIR"/*.ll; do
        [[ -e "$ll" ]] || continue
        mv "$ll" "$BUILD_DIR/$(basename "$ll")"
        echo -e "${GREEN}Moved $(basename "$ll") -> $BUILD_DIR/${RESET}"
    done
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

    echo -e "${GREEN}Generated assembly -> $s_file${RESET}"
    echo -e "${GREEN}Linked executable  -> $BUILD_DIR/exe.out${RESET}"
}

comp() {
    build && ir && asm
}

run() {
    comp && "$BUILD_DIR/exe.out"
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

    # Build with OPTIMIZE=0 so saved .ll matches what tests() generates
    clang "${SRC_FILES[@]}" "${CFLAGS[@]}" -DOPTIMIZE=0 -o "$URA_COMPILER" || {
        echo -e "${RED}Build failed${RESET}"
        return 1
    }

    ir || return 1

    local ll_src="$BUILD_DIR/file.ll"
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

    # Rebuild normally so the dev compiler stays with default flags
    build
}

tests() {
    echo -e "${YELLOW}Running tests...${RESET}"
    mkdir -p "$BUILD_DIR"

    # Build with OPTIMIZE=0 for deterministic IR comparison
    clang "${SRC_FILES[@]}" "${CFLAGS[@]}" -DOPTIMIZE=0 -o "$URA_COMPILER" || {
        echo -e "${RED}Build failed${RESET}"
        return 1
    }

    local failed=0
    local passed=0
    local folder_filter="${1:-.}"

    for ura_file in "$TESTS_DIR"/$folder_filter/**/*.ura; do
        [[ -e "$ura_file" ]] || continue

        local base_name=$(basename "$ura_file" .ura)
        local dir_name=$(basename "$(dirname "$ura_file")")
        local ll_file="$(dirname "$ura_file")/${base_name}.ll"

        cp "$ura_file" "$BUILD_DIR/test.ura"

        if ! "$URA_COMPILER" "$BUILD_DIR/test.ura" > /dev/null 2>&1; then
            echo -e "  ${RED}FAIL $dir_name/$base_name (compilation error)${RESET}"
            ((failed++))
            continue
        fi

        # Move any .ll files the compiler dropped in build/ (named after test.ura)
        for ll in "$BUILD_DIR"/*.ll; do
            [[ -e "$ll" ]] || continue
            [[ "$(basename "$ll")" == "test.ll" ]] && continue
            mv "$ll" "$BUILD_DIR/test.ll"
        done

        if [[ ! -f "$BUILD_DIR/test.ll" ]]; then
            echo -e "  ${RED}FAIL $dir_name/$base_name (no IR generated)${RESET}"
            ((failed++))
            continue
        fi

        if [[ ! -f "$ll_file" ]]; then
            echo -e "  ${RED}FAIL $dir_name/$base_name (expected .ll not found)${RESET}"
            ((failed++))
            continue
        fi

        if diff -q <(tail -n +4 "$BUILD_DIR/test.ll") <(tail -n +4 "$ll_file") > /dev/null 2>&1; then
            echo -e "  ${GREEN}PASS $dir_name/$base_name${RESET}"
            ((passed++))
        else
            echo -e "  ${RED}FAIL $dir_name/$base_name (IR mismatch)${RESET}"
            # diff <(tail -n +4 "$BUILD_DIR/test.ll") <(tail -n +4 "$ll_file") | head -20
            ((failed++))
        fi
    done

    rm -f "$BUILD_DIR/test.ura" "$BUILD_DIR/test.ll"

    echo ""
    echo -e "${GREEN}Passed: $passed${RESET}"
    [[ $failed -gt 0 ]] && echo -e "${RED}Failed: $failed${RESET}"

    # Rebuild with default flags so dev compiler is restored
    build > /dev/null 2>&1

    return $failed
}

# =========================================================
#  LLVM Sandbox (llvm/ subfolder)
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
    echo -e "${YELLOW}Formatting source files...${RESET}"

    # Make sure SRC_DIR is set
    if [ -z "$SRC_DIR" ]; then
        echo "SRC_DIR is not set!"
        return 1
    fi

    # Format C source and header files
    astyle \
        --mode=c \
        --style=allman \
        --indent=spaces=3 \
        --pad-oper \
        --pad-header \
        --keep-one-line-statements \
        --keep-one-line-blocks \
        --convert-tabs \
        --max-code-length=150 \
        --break-after-logical \
        --suffix=none \
        "$SRC_DIR"/*.c "$SRC_DIR"/*.h

    echo -e "${GREEN}Code formatted successfully!${RESET}"
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
