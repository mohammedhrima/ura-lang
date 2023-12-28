#!/bin/bash

rm -rf comp
cc main.c -lm -fsanitize=address -fsanitize=null -g3 -o comp

if [ $? -eq 0 ]; then
    ./comp $1    
    if [ $? -eq 0 ]; then
        cc file.s -o asm        
        if [ $? -eq 0 ]; then
            ./asm
            rm -rf asm
        fi
    fi
fi
