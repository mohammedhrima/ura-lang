#!/usr/bin/env bash

# === Color Definitions ===
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

# === Paths & Global Variables ===
src="$HOME/Desktop/Personal/pandu-lang/src"
files=($src/main.c $src/utils.c $src/asm.c)

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
    clang "${files[@]}" "${flags[@]}" -o "$src/pan" || {
        echo -e "${RED}Error:${NC} Build failed."
        return 1
    }
}

ir() {
    echo -e "${YELLOW}Compiling file.pn...${NC}"
    "$src/pan" "$src/file.pn" || {
        echo -e "${RED}Error:${NC} Compiling .pn file failed."
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

exe() {
    echo -e "${YELLOW}Compiling file.s...${NC}"
    clang "$src/file.s" -o "$src/exe.out" && "$src/exe.out"
}

run() {
    build && ir && asm && exe
}

# === Format Source Code ===
indent() {
    echo -e "${YELLOW}Formatting code...${NC}"
    astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
       --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
       --max-code-length=75 --break-after-logical \
       --suffix=none $src/*.c $src/*.h
}

# === Reload Config ===
update() {
    source "$src/../config.sh"
}

# === Prompt Handling (works for bash & zsh) ===
set_prompt() {
    if [ -n "$ZSH_VERSION" ]; then
        # zsh: username + last folder + %
        PROMPT="(pandu) %n@%1~ % "
    elif [ -n "$BASH_VERSION" ]; then
        # bash: username + last folder + $
        PS1="(pandu) \u@\W \$ "
    fi
}
set_prompt


# === Git Sync Check ===
if git rev-parse --is-inside-work-tree &>/dev/null; then
    git fetch origin &>/dev/null

    LOCAL=$(git rev-parse @)
    REMOTE=$(git rev-parse @{u} 2>/dev/null || echo "")
    BASE=$(git merge-base @ @{u} 2>/dev/null || echo "")

    if [ "$LOCAL" = "$REMOTE" ]; then
        :
    elif [ "$LOCAL" = "$BASE" ]; then
        echo -e "${RED}Error:${NC} Your branch is behind the remote. Please pull before building."
        return 1
    elif [ "$REMOTE" = "$BASE" ]; then
        echo -e "${YELLOW}Warning:${NC} Your branch is ahead of the remote."
    else
        echo -e "${RED}Error:${NC} Your branch has diverged from remote. Resolve conflicts first."
        return 1
    fi
else
    echo -e "${YELLOW}Warning:${NC} Not a git repository, skipping sync check."
fi
