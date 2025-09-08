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
    astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
       --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
       --max-code-length=75 --break-after-logical \
       --suffix=none $src*.c $src*.h
}


# === Reload Config & Shell ===

update() {
    source "$src/config.sh"
}

[[ $PS1 != "(wolfc)"* ]] && PS1="(wolfc) $PS1"


# Make sure we're in a git repo
if ! git rev-parse --is-inside-work-tree &>/dev/null; then
    echo -e "${YELLOW}Warning:${NC} Not a git repository, skipping sync check."
    return 0
fi

# Fetch remote changes
git fetch origin &>/dev/null

# Compare local vs remote
LOCAL=$(git rev-parse @)
REMOTE=$(git rev-parse @{u})
BASE=$(git merge-base @ @{u})

if [ "$LOCAL" = "$REMOTE" ]; then
    return 0
elif [ "$LOCAL" = "$BASE" ]; then
    echo -e "${RED}Error:${NC} Your branch is behind the remote. Please pull before building."
    return 1
elif [ "$REMOTE" = "$BASE" ]; then
    echo -e "${YELLOW}Warning:${NC} Your branch is ahead of the remote."
    return 0
else
    echo -e "${RED}Error:${NC} Your branch has diverged from remote. Resolve conflicts first."
    return 1
fi
