#!/bin/bash

alias cc="gcc"
export PATH="/code:$PATH"

asm() {
    gcc $1 -S -fno-verbose-asm -fno-asynchronous-unwind-tables -masm=intel
}

flags="-fsanitize=address -fsanitize=null -g3" 
flags="$flags -Wall -Werror=incompatible-pointer-types"
# flags="$flags -Werror -Wextra"

copy() {
	currflags=$flags
	flags="$flags -D DEBUG=false"

    gcc /code/main.c $flags -o /code/wcc && /code/wcc /code/file.w
    if [ $? -eq 0 ]; then
        file_count=$(ls ./tests/$1/*.w 2>/dev/null | wc -l)
        next_file_number=$((file_count + 1))
        new_filename="$(printf '%03d' "$next_file_number")"
        mkdir -p ./tests/$1 ./tests/$1/cmp
        new_w_file="./tests/$1/${new_filename}.w"
        new_s_file="./tests/$1/cmp/${new_filename}.s"
        echo "create $new_w_file"
        echo "create $new_s_file"
        cp "file.w" "$new_w_file"
        cp "file.s" "$new_s_file"
    else
        echo "Error: Compilation or execution of /code/wcc failed. Aborting copy operation."
    fi

	flags=$currflags
}

test() {
	currflags=$flags
	flags="$flags -D DEBUG=false"
	gcc /code/main.c $flags -o /code/wcc
	CURRDIR=$PWD
	cd /code/tests || exit

	# Define colors
	GREEN='\033[0;32m'
	YELLOW='\033[0;33m'
	NC='\033[0m'

	for folder in */; do
		# echo -e "${GREEN}$folder:${NC}"

		for file in "$folder"*.w; do
			if [ -e "$file" ]; then
				base=$(basename -- "$file")
				name="${base%.*}"
				wcc $PWD/${folder}$name.w
				# echo -n "   "
				# echo -e "      $GREEN compare $PWD/${folder}$name.s && $PWD/${folder}cmp/$name.s$NC"
				if cmp -s "$PWD/${folder}$name.s" "$PWD/${folder}cmp/$name.s"; then
					echo -e "${GREEN}SUCCESS${NC}: $folder$name.w"
					rm -f "${folder}${name}.s"
				else
					echo -e "${RED}FAILED${NC}: ${folder}${name}.w"
				fi
			fi
		done
	done

	cd "$CURRDIR" || exit
	flags=$currflags
}

wcc_build() {
	clear && rm -rf /code/wcc 
	gcc main.c $flags -o /code/wcc && /code/wcc /code/file.w
}

run(){
	cc /code/file.s && ./a.out
}

alias build="wcc_build"
PS1='\[\e[1;32m\]\u@\h\[\e[m\]:\[\e[1;34m\]\w\[\e[m\]\$ '

