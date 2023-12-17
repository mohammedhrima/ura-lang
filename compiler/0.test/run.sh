rm -rf a.out
gcc $1 -S -fno-verbose-asm -fno-asynchronous-unwind-tables -masm=intel
rm -rf a.out.dSYM


# movsbl
# movzbl
# movq > 64bit
# movw > 16bit
# movl > 32bit
# movb > 08bit