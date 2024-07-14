flags="-Wall -Werror -Wextra"
cc main.c -fsanitize=address -g3 -o wolf-c && ./wolf-c