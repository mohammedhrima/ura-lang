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

wcc_build() {
    echo -e "${YELLOW}Building...${NC}"
    echo "Files: $files"
    echo "Flags: $flags"
    clang "${files[@]}" "${flags[@]}" -o "$src/wcc" || {
        echo -e "${RED}Error:${NC} Build failed."
        return 1
    }
}

wcc_ir() {
    echo -e "${YELLOW}Compiling file.w...${NC}"
    "$src/wcc" "$src/code/file.w" || {
        echo -e "${RED}Error:${NC} Compilation failed."
        return 1
    }
}

wcc_asm() {
    echo -e "${YELLOW}Generating file.s from file.ir...${NC}"
    llc "$src/code/file.ir" -o "$src/code/file.s" || {
        echo -e "${RED}Error:${NC} Assembly generation failed."
        return 1
    }
}

wcc_run() {
    echo -e "${YELLOW}compile file.s${NC}"
    clang "$src/code/file.s" -o exe.out
    ./exe.out
}

build() {
    unalias build 2>/dev/null || true
    wcc_build && wcc_ir && wcc_asm
}

run() {
    wcc_run
}

# === Test Suite ===

test() {
    echo -e "${YELLOW}Running tests...${NC}"
    rm -f "$src/wcc"
    local currflags="$flags"
    flags="$flags -D DEBUG=false -D OPTIMIZE=true -D TESTING=true"
    gcc $files $flags -o "$src/wcc" || {
        echo -e "${RED}Build failed.${NC}"
        flags="$currflags"
        return 1
    }

    local CURRDIR=$PWD
    cd ../tests || exit

    for folder in */; do
        for file in "$folder"*.w; do
            [ -e "$file" ] || continue
            local base=$(basename -- "$file")
            local name="${base%.*}"
            "$CURRDIR/wcc" "$PWD/${folder}$name.w"
            if cmp -s "$PWD/${folder}$name.s" "$PWD/${folder}asm/$name.s"; then
                echo -e "${GREEN}SUCCESS${NC}: $folder$name.w"
                rm -f "${folder}${name}.s"
            else
                echo -e "${RED}FAILED${NC}: $folder$name.w"
            fi
        done
    done

    cd "$CURRDIR"
    flags="$currflags"
}

# === Copy Utility for Test Management ===

copy() {
    local test_name="$1"
    [ -z "$test_name" ] && echo -e "${RED}Usage:${NC} copy <test_name>" && return 1

    local currflags="$flags"
    flags="$flags -D DEBUG=false"

    gcc $files $flags -o "$src/wcc" && "$src/wcc" "$src/file.w"
    if [ $? -eq 0 ]; then
        local test_dir="../tests/$test_name"
        local asm_dir="$test_dir/asm"
        mkdir -p "$test_dir" "$asm_dir"

        local file_count
        file_count=$(ls "$test_dir"/*.w 2>/dev/null | wc -l)
        local next_file_number=$((file_count + 1))
        local new_filename
        new_filename="$(printf '%03d' "$next_file_number")"

        local new_w_file="$test_dir/${new_filename}.w"
        local new_s_file="$asm_dir/${new_filename}.s"

        echo "Created: $new_w_file"
        echo "Created: $new_s_file"
        cp "$src/file.w" "$new_w_file"
        cp "$src/file.s" "$new_s_file"
    else
        echo -e "${RED}Error:${NC} Compilation or execution failed. Aborting copy operation."
    fi

    flags="$currflags"
}

# === Format Source Code ===

indent() {
    echo -e "${YELLOW}Formatting code...${NC}"
    astyle --mode=c --indent=spaces=4 --pad-oper --pad-header \
        --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
        $src*.c $src*/*.h &&
        rm -f $src*.c.orig $src*/*.h.orig
}

# === Reload Config & Shell ===

update() {
    # Colors
    GREEN="%F{green}"
    BLUE="%F{blue}"
    RESET="%f"

    # Set prompt
    export PS1="${GREEN}%n@%m ${BLUE}%~ %{$RESET%}%# "

    echo -e "${YELLOW}Reloading config...${NC}"
    source "$src/config.sh"
    source ~/.zshrc
}


