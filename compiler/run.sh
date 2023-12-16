rm -rf a.out
cc main.c -lm -fsanitize=address -fsanitize=null -g3
./a.out
