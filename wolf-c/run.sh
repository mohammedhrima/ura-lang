clear && 
cc main.c utils.c -fsanitize=address -g3 -o wcc && 
./wcc file.w 
# && cc file.s && ./a.out
