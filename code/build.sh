
flags="-fsanitize=address -fsanitize=null -g3 -Wall -Werror=incompatible-pointer-types" # -Wall -Werror -Wextra

clear && rm -rf ./a.out ./wcc && gcc main.c $flags -o wcc && ./wcc
