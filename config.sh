#!/usr/bin/env bash

# === Color Definitions ===
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

# === Paths & Global Variables ===
pandu_dir="$HOME/Desktop/Personal/pandu-lang"
src="$pandu_dir/src"
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
    local debug_mode="${1:-true}"   # default true unless specified
    clang "${files[@]}" "${flags[@]}" -D DEBUG=$debug_mode -o "$src/pan" || {
        echo -e "${RED}Error:${NC} Build failed."
        return 1
    }
}

ir() {
    "$src/pan" "$src/file.pn" || {
        echo -e "${RED}Error:${NC} Compiling .pn file failed."
        return 1
    }
}

asm() {
    llc "$src/file.ll" -o "$src/file.s" || {
        echo -e "${RED}Error:${NC} Assembly generation failed."
        return 1
    }
}

exe() {
    clang "$src/file.s" -o "$src/exe.out" && "$src/exe.out"
}

run() {
    build true && ir && asm && exe
}

indent() {
    echo -e "${YELLOW}Formatting code...${NC}"
    astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
       --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
       --max-code-length=100 --break-after-logical \
       --suffix=none $src/*.c $src/*.h
}

copy() {
    if [ -z "$1" ]; then
        echo -e "${RED}Error:${NC} No directory name provided."
        echo "Usage: copy <dir_name> [file_name]"
        return 1
    fi

    local dir_name="$1"
    local test_dir="$pandu_dir/tests/$dir_name"
    mkdir -p "$test_dir"

    local filename
    if [ -z "$2" ]; then
        # Auto-generate if no filename given
        local count=$(ls "$test_dir"/*.pn 2>/dev/null | wc -l)
        local next=$(printf "%03d" $((count + 1)))
        filename="$next"
    else
        # Use provided filename
        filename="$2"
    fi

    local pn_dest="$test_dir/${filename}.pn"
    local ll_dest="$test_dir/${filename}.ll"

    # Build with DEBUG=false before copying reference outputs
    build false || return 1
    ir || return 1

    cp "$src/file.pn" "$pn_dest" || {
        echo -e "${RED}Error:${NC} Failed to copy file.pn"
        return 1
    }
    cp "$src/file.ll" "$ll_dest" || {
        echo -e "${RED}Error:${NC} Failed to copy file.ll"
        return 1
    }

    echo -e "${GREEN}Saved:${NC} $pn_dest and $ll_dest"
}

tests() {
    # Build with DEBUG=false for test comparison
    build false || return 1

    for pn_file in "$pandu_dir/tests"/*/*.pn; do
        [ -e "$pn_file" ] || continue

        local base_name=$(basename "$pn_file" .pn)
        local dir_name=$(basename "$(dirname "$pn_file")")
        local ll_file="$(dirname "$pn_file")/${base_name}.ll"

        cp "$pn_file" "$src/tmp.pn"

        if ! "$src/pan" "$src/tmp.pn" > /dev/null 2>&1; then
            echo -e "${RED}FAILED:${NC} $dir_name/$base_name.pn (compilation error)"
            continue
        fi

        if diff -q <(tail -n +3 "$src/tmp.ll") <(tail -n +3 "$ll_file") > /dev/null 2>&1; then
            echo -e "${GREEN}PASSED:${NC} $dir_name/$base_name"
        else
            echo -e "${RED}FAILED:${NC} $dir_name/$base_name"
        fi
    done
    rm -rf  "$src/tmp.pn" "$src/tmp.ll"
}

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
