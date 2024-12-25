flags="-fsanitize=address -fsanitize=null -g3 -Wall" # -Wall -Werror -Wextra
clear && rm -rf ./a.out ./wcc && gcc main.c $flags -o wcc && ./wcc
