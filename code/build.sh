flags="-fsanitize=address -fsanitize=null -g3" # -Wall -Werror
clear && rm -rf ./a.out && gcc main.c $flags && ./a.out 
