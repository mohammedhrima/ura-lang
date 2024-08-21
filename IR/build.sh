rm -rf wcc a.out
flags="-Wall -Werror -Wextra"
CC="gcc"

$CC main.c -fsanitize=address -g3 -o wcc && ./wcc file.w && $CC file.s -o exe && ./exe
