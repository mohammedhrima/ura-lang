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
export ASAN_FILE="$ROOT_DIR/lsan.supp"

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

LLVM_CFLAGS=( $(llvm-config-14 --cflags) )
LLVM_LDFLAGS=( $(llvm-config-14 --ldflags --libs core) )

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
    if [[ $# -lt 1 ]]; then
        echo -e "${RED}Usage: copy <file.ura>${RESET}"
        echo -e "${YELLOW}The file must have a first line comment: // <folder>/<filename>${RESET}"
        echo -e "${YELLOW}Example: // builtins/printf${RESET}"
        return 1
    fi

    local ura_src_file="$1"

    # Handle relative paths
    if [[ ! "$ura_src_file" = /* ]]; then
        ura_src_file="$SRC_DIR/$ura_src_file"
    fi

    if [[ ! -f "$ura_src_file" ]]; then
        echo -e "${RED}$ura_src_file not found${RESET}"
        return 1
    fi

    # Read the first line and extract the path
    local first_line=$(head -n 1 "$ura_src_file")
    echo "first_line: '$first_line'"

    # Match: // any/depth/of/path optionally ending in .ura
    if [[ "$first_line" =~ ^//[[:space:]]*([a-zA-Z0-9_/-]+) ]]; then
        local full_path="${match[1]}"
        # Remove trailing .ura if present
        full_path="${full_path%.ura}"

        local folder="${full_path%/*}"    # everything before last /
        local filename="${full_path##*/}" # everything after last /

        local test_dir="$TESTS_DIR/$folder"
        local ura_dest="$test_dir/${filename}.ura"
        local ll_dest="$test_dir/${filename}.ll"

        echo -e "${YELLOW}Detected path: $folder/$filename${RESET}"
    else
        echo -e "${RED}Invalid or missing path comment in first line${RESET}"
        echo -e "${YELLOW}Expected format: // <folder>/<filename>${RESET}"
        echo -e "${YELLOW}Example: // builtins/printf${RESET}"
        return 1
    fi

    mkdir -p "$test_dir"

    build || return 1

    "$URA_COMPILER" "$ura_src_file" -no-exec || {
        echo -e "${RED}Compilation failed${RESET}"
        return 1
    }

    local src_dir=$(dirname "$ura_src_file")
    local src_basename=$(basename "$ura_src_file" .ura)
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
        # find all .ura files recursively
        while IFS= read -r f; do
            ura_files+=("$f")
        done < <(find "$TESTS_DIR" -name "*.ura" | sort)
    else
        for target in "${targets[@]}"; do
            local dir="$target"
            [[ ! "$dir" = /* ]] && dir="$TESTS_DIR/$target"
            while IFS= read -r f; do
                ura_files+=("$f")
            done < <(find "$dir" -name "*.ura" | sort)
        done
    fi

    for ura_file in "${ura_files[@]}"; do
        [[ -e "$ura_file" ]] || continue

        local base_name=$(basename "$ura_file" .ura)
        local dir=$(dirname "$ura_file")
        local rel_path="${ura_file#$TESTS_DIR/}"   # e.g. vars/ref/001
        rel_path="${rel_path%.ura}"

        local ll_expected="$dir/${base_name}.ll"
        local ll_got="$dir/build/${base_name}.ll"

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
        local dir=$(dirname "$ura_file")
        local base=$(basename "$ura_file" .ura)
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
#  Examples Generation
# =========================================================
examples() {
    echo -e "${YELLOW}Generating examples.ura...${RESET}"
    
    local output_file="$ROOT_DIR/examples.ura"
    
    # Create header
    cat > "$output_file" << 'EOF'
// ============================================================================
// URA LANGUAGE EXAMPLES
// ============================================================================
// This file contains all test examples from the Ura language test suite.
// Each section demonstrates different features and capabilities of the language.
// ============================================================================

EOF

    # Find and process all .ura files
    local count=0
    for file in $(find "$TESTS_DIR" -name "*.ura" -type f | sort); do
        local category=$(dirname "$file" | sed "s|$TESTS_DIR/||")
        local filename=$(basename "$file" .ura)
        local relative_path=$(echo "$file" | sed "s|$ROOT_DIR/||")
        
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
    if [ -z "$SRC_DIR" ]; then
        echo "SRC_DIR is not set!"
        return 1
    fi
    uncrustify -c $ROOT_DIR/uncrustify.cfg --no-backup "$SRC_DIR"/*.c "$SRC_DIR"/*.h
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
    echo -e "${BOLD}Ura Environment Commands:${RESET}"
    echo ""
    echo -e "  ${GREEN}build${RESET}                        Build the ura compiler"
    echo -e "  ${GREEN}copy${RESET}                         Save src/file.ura + its IR (reads path from first line comment)"
    echo -e "  ${GREEN}tests [folder]${RESET}               Run all tests (optionally filter by folder)"
    echo -e "  ${GREEN}examples${RESET}                     Generate examples.ura from all test files"
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
    echo -e "    -san  Enable AddressSanitizer (detects memory errors)"
    echo -e "           Link with: clang -fsanitize=address"
    echo ""
    echo -e "  ${CYAN}Output:${RESET}"
    echo -e "    -o <name>  Set executable name (default: exe.out)"
    echo ""
    echo -e "${BOLD}Examples:${RESET}"
    echo ""
    echo -e "  ${YELLOW}\$URA_COMPILER src/file.ura${RESET}"
    echo -e "  ${YELLOW}\$URA_COMPILER src/file.ura -O2 -o myapp${RESET}"
    echo -e "  ${YELLOW}\$URA_COMPILER src/file.ura -san -o myapp${RESET}"
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
# cd "$ROOT_DIR" 2>/dev/null
echo -e "${GREEN}Ura environment loaded!${RESET}"