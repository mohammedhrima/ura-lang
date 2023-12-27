#!/bin/bash
rm -rf a.out 
cc main.c -lm -fsanitize=address -fsanitize=null -g3 -D DEBUG=0
cp -r a.out ./1.tests

# Color codes for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No color

# Change to the 1.tests directory
cd 1.tests || exit

# Iterate over all files with .hr extension
for file in *.hr; do
    # Check if the file is a regular file
    if [ -f "$file" ]; then
        # Execute the command to generate file.s
        ./a.out "$file"
        
        # Extract the file name without extension
        filename=$(basename -- "$file")
        filename_no_extension="${filename%.*}"
        
        # Compare the generated file.s with cmp/file_name.s
        if cmp -s "file.s" "cmp/${filename_no_extension}.s"; then
            # Print SUCCESS in green
            echo -e "${GREEN}SUCCESS${NC} for $file"
        else
            # Print FAILED in red
            echo -e "${RED}FAILED${NC} for $file"
        fi
    fi
done
rm -rf a.out file.s
cd ..

