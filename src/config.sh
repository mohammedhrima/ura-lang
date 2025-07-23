#!/bin/bash

# Define colors
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

src="$PWD"

san_flags="-fsanitize=address,null"
debug_flags="-g3"
warn_flags="-Wall -Wextra -Werror=incompatible-pointer-types -Werror=int-conversion"

flags="$san_flags $debug_flags $warn_flags"

files="main.c utils.c"

copy() {
    currflags="$flags"
    flags="$flags -D DEBUG=false"

    gcc $files $flags -o "$src/wcc" && "$src/wcc" "$src/file.w"
    if [ $? -eq 0 ]; then
        test_dir="../tests/$1"
        asm_dir="../tests/$1/asm"
        mkdir -p "$test_dir" "$asm_dir"

        file_count=$(ls "$test_dir"/*.w 2>/dev/null | wc -l)
        next_file_number=$((file_count + 1))
        new_filename="$(printf '%03d' "$next_file_number")"

        new_w_file="$test_dir/${new_filename}.w"
        new_s_file="$asm_dir/${new_filename}.s"

        echo "create $new_w_file"
        echo "create $new_s_file"
        cp "$src/file.w" "$new_w_file"
        cp "$src/file.s" "$new_s_file"
    else
        echo -e "${RED}Error:${NC} Compilation or execution of $src/wcc failed. Aborting copy operation."
    fi

    flags="$currflags"
}

test() {
    rm -rf "$src/wcc"
    currflags="$flags"
    flags="$flags -D DEBUG=false -D OPTIMIZE=true -D TESTING=true"
    gcc $files $flags -o "$src/wcc" || { echo -e "${RED}Build failed.${NC}"; return; }

    CURRDIR=$PWD
    cd ../tests || exit

    for folder in */; do
        for file in "$folder"*.w; do
            [ -e "$file" ] || continue
            base=$(basename -- "$file")
            name="${base%.*}"
            "$CURRDIR/wcc" "$PWD/${folder}$name.w"
            if cmp -s "$PWD/${folder}$name.s" "$PWD/${folder}asm/$name.s"; then
                echo -e "${GREEN}SUCCESS${NC}: $folder$name.w"
                rm -f "${folder}${name}.s"
            else
                echo -e "${RED}FAILED${NC}: ${folder}${name}.w"
            fi
        done
    done

    cd "$CURRDIR" || exit
    flags="$currflags"
}

wcc_build() {
    gcc $files $flags -o "$src/wcc" && "$src/wcc" "$src/file.w"
    rm -rf "$src/wcc"
}

run() {
    gcc "$src/file.s" $san_flags && ./a.out
}

update() {
    source config.sh
    source ~/.zshrc
}

indent() {
    astyle --mode=c --indent=spaces=4 --pad-oper --pad-header \
        --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
        "$src"/*.c "$src"/include/*.h \
        && rm -rf "$src"/*.c.orig "$src"/include/*.h.orig
}

alias build="wcc_build"
