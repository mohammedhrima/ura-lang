#!/bin/bash
chmod -R 777 /src /import /random /tests /root/.bashrc

alias cc="gcc"
export PATH="/src:$PATH"

asm() {
	gcc $1 -S -fno-verbose-asm -fno-asynchronous-unwind-tables -masm=intel
}

# Define colors
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

san="-fsanitize=address -fsanitize=null -g3" 
flags="$san -Wall -Wextra -Werror=incompatible-pointer-types -Werror=int-conversion"
files="/src/main.c /src/utils.c"

copy() {
	currflags=$flags
	flags="$flags -D DEBUG=false"

	gcc $files $flags -o /src/wcc && /src/wcc /src/file.w
	if [ $? -eq 0 ]; then
		file_count=$(ls /tests/$1/*.w 2>/dev/null | wc -l)
		next_file_number=$((file_count + 1))
		new_filename="$(printf '%03d' "$next_file_number")"
		mkdir -p /tests/$1 /tests/$1/asm
		new_w_file="/tests/$1/${new_filename}.w"
		new_s_file="/tests/$1/asm/${new_filename}.s"
		echo "create $new_w_file"
		echo "create $new_s_file"
		cp "/src/file.w" "$new_w_file"
		cp "/src/file.s" "$new_s_file"
	else
		echo "Error: Compilation or execution of /src/wcc failed. Aborting copy operation."
	fi

	flags=$currflags
}

test() {
	currflags=$flags
	flags="$flags -D DEBUG=false -D OPTIMIZE=true"
	gcc $files $flags -o /src/wcc
	CURRDIR=$PWD
	cd /tests || exit

	for folder in */; do
		for file in "$folder"*.w; do
			if [ -e "$file" ]; then
				base=$(basename -- "$file")
				name="${base%.*}"
				wcc $PWD/${folder}$name.w
				if cmp -s "$PWD/${folder}$name.s" "$PWD/${folder}asm/$name.s"; then
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
	clear && rm -rf /src/wcc 
	gcc $files $flags -o /src/wcc && /src/wcc /src/file.w
}

run(){
	cc /src/file.s && ./a.out
}

update(){
	source ~/.bashrc
}

indent(){
	astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
	--keep-one-line-statements --keep-one-line-blocks --convert-tabs $files && rm -rf *.c.orig
	chmod -R 777 /src /src/include /import /random /tests /root/.bashrc
}

alias build="wcc_build"
indent
PS1='\[\e[1;32m\]\u@\h\[\e[m\]:\[\e[1;34m\]\w\[\e[m\]\$ '
