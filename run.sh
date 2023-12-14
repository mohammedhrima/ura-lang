rm -rf a.out
rm -rf exe

cc main.c -fsanitize=address -fsanitize=null -g3
./a.out
