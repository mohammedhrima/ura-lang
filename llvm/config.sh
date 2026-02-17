#!/usr/bin/env bash

# =========================================================
#  Paths
# =========================================================
CONFIG_FILE="${BASH_SOURCE[0]:-$0}"
ROOT_DIR="$(cd "$(dirname "$CONFIG_FILE")" && pwd)"
TESTING_MODE=0 

BUILD_DIR="$ROOT_DIR/build"
EXAMPLES_DIR="$ROOT_DIR/examples"
TESTS_DIR="$ROOT_DIR/tests"
OUT_LL="$BUILD_DIR/out.ll"

RED="\033[0;31m"
GREEN="\033[0;32m"
RESET="\033[0m"

# =========================================================
#  LLVM toolchain flags (ARRAYS!)
# =========================================================
LLVM_CFLAGS=( $(llvm-config --cflags) )
LLVM_LDFLAGS=( $(llvm-config --ldflags --libs core) )

SAN_FLAGS=(
  -Wall
  -fsanitize=address
  -g3
)

SRC_FILES=("$ROOT_DIR/main.c")

# =========================================================
#  Functions
# =========================================================

run() {
    if [[ $# -eq 0 ]]; then
        echo "Usage: run <example> [args...]"
        return -99
    fi

    local example="$1"
    shift  # remaining arguments will be passed to the driver

    local example_path="$EXAMPLES_DIR/$example"

    if [[ ! -e "$example_path" ]]; then
        echo "Example not found: $example_path"
        return -99
    fi

    mkdir -p "$BUILD_DIR"
    rm -f "$BUILD_DIR"/{comp,out.ll,out.s,exe.out}

    # Add TESTING macro if enabled
    CLANG_DEFS=()
    if [[ $TESTING_MODE -eq 1 ]]; then
        CLANG_DEFS+=("-DTESTING=true")
    fi

    # Compile driver program
    clang \
        "${SRC_FILES[@]}" \
        "${LLVM_CFLAGS[@]}" \
        "${SAN_FLAGS[@]}" \
        "${CLANG_DEFS[@]}" \
        -o "$BUILD_DIR/comp" \
        "${LLVM_LDFLAGS[@]}" || { echo "❌ Compilation failed"; return -99; }

    # Run from project root, first argument is the example path, others are flags
    pushd "$ROOT_DIR" > /dev/null || return -99
    "$BUILD_DIR/comp" "examples/$example" "$@" || { popd > /dev/null; echo "❌ Program failed"; return -99; }
    popd > /dev/null

    # Generate assembly
    llc "$OUT_LL" -o "$BUILD_DIR/out.s" || { echo "❌ llc failed"; return -99; }

    # Compile executable
    clang \
        "$BUILD_DIR/out.s" \
        "${SAN_FLAGS[@]}" \
        -lc++ \
        -o "$BUILD_DIR/exe.out" || { echo "❌ Linking failed"; return -99; }

    "$BUILD_DIR/exe.out"
    local code=$?

    # Only print exit code if not testing
    if [[ "$TESTING_MODE" -eq 0 ]]; then
        echo "exit code $code"
    fi

    # Clean intermediate files
    rm -rf "$BUILD_DIR"/{exe.out,out.s,comp} */*.dSYM

    # Normal program exit code
    return $code
}


copy() {
    mkdir -p "$TESTS_DIR"
    run "$1" || return -99
    cp "$OUT_LL" "$TESTS_DIR/$1.ll"
    echo "✓ Copied → $TESTS_DIR/$1.ll"
}

tests() {
    TESTING_MODE=1
    local failed=0

    for ex in "$EXAMPLES_DIR"/*; do
        local name="$(basename "$ex")"
        echo "▶ Testing $name"

        run "$name"
        local rc=$?

        if [[ $rc -eq -99 ]]; then
            # Driver/compilation/linking failure
            echo -e "  ${RED}RUN FAILED${RESET}"
            failed=1
            continue
        fi

        if diff -u "$TESTS_DIR/$name.ll" "$OUT_LL" > /dev/null; then
            echo -e "  ${GREEN}SUCCESS${RESET}"
        else
            echo -e "  ${RED}FAILED${RESET}"
            failed=1
        fi
    done

    TESTING_MODE=0
    return $failed
}

indent() {
    echo -e "${YELLOW}Formatting code...${NC}"
    astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
       --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
       --max-code-length=100 --break-after-logical \
       --suffix=none $ROOT_DIR/*.c $ROOT_DIR/*.h
}

update() {
    echo "↻ Reloading config.sh"
    source "$CONFIG_FILE"
}

# =========================================================
#  Prompt
# =========================================================
if [[ -n "$ZSH_VERSION" ]]; then
    PROMPT="(env) %n@%1~ % "
elif [[ -n "$BASH_VERSION" ]]; then
    PS1="(env) \u@\W \$ "
fi
