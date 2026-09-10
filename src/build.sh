CXX=clang

FLAGS="-fsanitize=address -fsanitize=null -g3 "

$CXX $FLAGS main.c -o ura && \
setarch "$(uname -m)" -R ./ura file.ura && \
rm -rf ura