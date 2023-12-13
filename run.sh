cmd="cc main.c -fsanitize=address -fsanitize=null -g3"
echo $cmd
$cmd
./a.out
rm -rf a.out