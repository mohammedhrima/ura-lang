flags="-fsanitize=address -fsanitize=null -g3 -Wall" # -Wall -Werror -Wextra
clear && rm -rf ./a.out && gcc main.c $flags && ./a.out 
