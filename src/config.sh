#!/bin/bash

# === Color Definitions ===
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

# === Paths & Global Variables ===
src="./"
files=(main.c utils.c asm.c)

# === Compiler Flags ===
san_flags=(-fsanitize=address -fsanitize=null)
debug_flags=(-g3)
warn_flags=(-Wall -Wextra -Werror=incompatible-pointer-types -Werror=int-conversion)
llvm_flags=($(llvm-config --cflags --ldflags --libs core))

flags=("${san_flags[@]}" "${debug_flags[@]}" "${warn_flags[@]}" "${llvm_flags[@]}")


# === Export Variables for Shell Access ===
export flags files llvm_flags src

# === Build Functions ===

build() {
    echo -e "${YELLOW}Building...${NC}"
    echo "Files: $files"
    echo "Flags: $flags"
    clang "${files[@]}" "${flags[@]}" -o "$src/wcc" || {
        echo -e "${RED}Error:${NC} Build failed."
        return 1
    }
}

ir() {
    echo -e "${YELLOW}Compiling file.w...${NC}"
    "$src/wcc" "$src/file.w" || {
        echo -e "${RED}Error:${NC} Compiling .w file failed."
        return 1
    }
}

asm() {
    echo -e "${YELLOW}Generating file.s from file.ll...${NC}"
    llc "$src/file.ll" -o "$src/file.s" || {
        echo -e "${RED}Error:${NC} Assembly generation failed."
        return 1
    }
}

exec() {
    echo -e "${YELLOW}compile file.s${NC}"
    clang "$src/file.s" -o exe.out
    ./exe.out
}

run() {
    build && ir && asm && exec
}

# === Format Source Code ===
indent() {
    echo -e "${YELLOW}Formatting code...${NC}"
    astyle --mode=c --indent=spaces=4 --pad-oper --pad-header \
        --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
        $src*.c $src*/*.h
    find "$src" -type f \( -name "*.c.orig" -o -name "*.h.orig" \) -delete
}


# === Reload Config & Shell ===

update() {
    source "$src/config.sh"
    # source ~/.zshrc
}


