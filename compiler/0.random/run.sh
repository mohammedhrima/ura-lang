rm -rf a.out
cc $1 -S -fno-verbose-asm -fno-asynchronous-unwind-tables -masm=intel -O3
rm -rf a.out.dSYM
