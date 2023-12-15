rm -rf a.out
cc main.c -fsanitize=address -fsanitize=null -g3
./a.out
