#!/bin/bash

BASHRC_PATH="$HOME/.bashrc"
# Check if the aliases are already present
if ! grep -q "run" "$BASHRC_PATH" ; then
    cat <<EOL >> "$BASHRC_PATH"

export PS1="\\W$ "
_cwc() {
    FILENAME_W=\$1
    FILENAME_S=\$(echo \$FILENAME_W | sed 's/\.w$/.s/')
    
    cc /wolf-c/main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=true -o /wolf-c/comp
    if [ \$? -eq 0 ]; then
        /wolf-c/comp \$FILENAME_W
        if [ \$? -eq 0 ]; then
            cc \$FILENAME_S -o asm
            ./asm
        fi
    fi

    rm -rf asm
    rm -rf /wolf-c/comp
}

_copy() {
   case "\$1" in
        "array" | "bool" | "char" | "chars" | "func" | "if" | "int" | "while" )
            cc /wolf-c/main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=false -o /wolf-c/comp
            /wolf-c/comp file.w
            file_count=\$(ls /wolf-c/tests/\$1/*.w 2>/dev/null | wc -l)
            next_file_number=\$((file_count + 1))
            new_filename="\$(printf '%03d' "\$next_file_number")"
            new_w_file="/wolf-c/tests/\$1/\${new_filename}.w"
            new_s_file="/wolf-c/tests/\$1/cmp/\${new_filename}.s"
            echo "create \$new_w_file"
            echo "create \$new_s_file"
            cp "file.w" "\$new_w_file"
            cp "file.s" "\$new_s_file"
            rm -rf /wolf-c/comp
            ;;
        * )
            echo "Invalid argument '\$1'"
            ;;
    esac
}

_test() {
    cc /wolf-c/main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=false -o /wolf-c/comp

    GREEN='\033[0;32m'
    RED='\033[0;31m'
    NC='\033[0m'

    cd /wolf-c/tests || exit
    for folder in */; do
        echo "\$folder:"
        cd "/wolf-c/tests/\$folder" || exit
        for file in *.w; do
            if [ -f "\$file" ]; then
                # Execute the command to generate file.s
                /wolf-c/comp "\$file"
                # Extract the file name without extension
                filename=\$(basename -- "\$file")
                filename_no_extension="\${filename%.*}"
                if cmp -s "\${filename_no_extension}.s" "cmp/\${filename_no_extension}.s"; then
                    echo -e "\${GREEN}SUCCESS\${NC}: \$folder\$file"
                    rm -rf "\${filename_no_extension}.s"
                else
                    echo -e "\${RED}FAILED\${NC} : \$folder\$file"
                fi
            fi
        done
        cd ..
    done
    cd /wolf-c
    rm -rf /wolf-c/comp
}

_refresh() {
    cc /wolf-c/main.c -fsanitize=address -fsanitize=null -g3 -D DEBUG=false -o /wolf-c/comp
    cd /wolf-c/tests || exit
    for folder in */; do
        echo "\$folder:"
        cd "\$folder" || exit
        for file in *.w; do
            if [ -f "\$file" ]; then
                # Extract file name without extension
                filename=\$(basename "\$file" .w)

                # Execute the command
                /wolf-c/comp "\$file"
                # Move the generated file.s to tests/cmp/ with the original filename.s
                mv "\$filename.s" "cmp/\$filename.s"
                echo "create \$filename.s"
            fi
        done
        cd ..
    done
    cd /wolf-cker exec -it 2 bash
    rm -rf comp
}

_progress() {
    file_count=\$(ls /wolf-c/progress/*.c 2>/dev/null | wc -l)
    next_file_number=\$((file_count + 1))
    new_filename="\$(printf '%03d' "\$next_file_number")"
    new_c_file="/wolf-c/progress/\${new_filename}.c"
    cp "main.c" "\$new_c_file"
}

_asm() {
    cc \$1 -S -fno-verbose-asm -fno-asynchronous-unwind-tables -masm=intel
}

alias cwc="_cwc \$1"
alias copy="_copy"
alias test="_test"
alias refresh="_refresh"
alias prog="_progress"
alias asm="_asm"
EOL
fi

tail -f > /dev/null