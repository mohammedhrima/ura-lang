#!/bin/bash
nasm -f macho $1 -o exe.o && ld -e main -maarch64linux exe.o -lSystem

# the compilation way that is working
# nasm -f elf hello.asm -o hello.o
# ld -m elf_i386 -o hello hello.o
