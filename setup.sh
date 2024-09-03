export Sgoinfre="/nfs/sgoinfre/goinfre/Perso/$USER"

#/nfs/sgoinfre/goinfre/Perso/mhrima/code/bin
export PATH="$Sgoinfre/code/bin:$Sgoinfre/node/bin:$HOME/.local/bin:$PATH"
alias cc="gcc"

_asm() {
    gcc $1 -S -fno-verbose-asm -fno-asynchronous-unwind-tables -masm=intel
}

_copy() {
    gcc main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=false -o wcc &&
    ./wcc file.w 
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
    rm -rf ./wcc
}

_test() {
    CURRDIR=$PWD
    gcc main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=false -D OPTIMIZE=true -o wcc

    GREEN='\033[0;32m'
    RED='\033[0;31m'
    NC='\033[0m'

    cd tests
    for folder in */; do
        echo "$folder:"
        # Count the number of .w files in the folder
        count=$(ls "$folder" | grep -c '\.w$')

        if [ $count -gt 0 ]; then
            for file in "$folder"*.w; do
                if [ -f "$file" ]; then
                    # Execute the command to generate file.s
                    $CURRDIR/wcc "$file"
                    # Extract the file name without extension
                    filename=$(basename -- "$file")
                    filename_no_extension="${filename%.*}"
                    echo "$GREEN compare ${filename_no_extension}.s && ${folder}cmp/${filename_no_extension}.s$NC"
                    if cmp -s "${folder}${filename_no_extension}.s" "${folder}cmp/${filename_no_extension}.s"; then
                        echo -e "${GREEN}SUCCESS${NC}: $folder$filename"
                        rm -f "${folder}${filename_no_extension}.s"
                    else
                        echo -e "${RED}FAILED${NC} : ${folder}${filename_no_extension}.w"
                    fi
                fi
            done
        fi
    done
    cd $CURRDIR
    rm -f ./wcc
}
alias asm="_asm"
alias copy="_copy"
alias test="_test"
