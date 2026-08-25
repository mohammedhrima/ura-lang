CXX=/usr/lib/llvm-21/bin/clang++

FLAGS="-fsanitize=address -g3 -std=c++2b -lstdc++exp "

$CXX $FLAGS main.cpp -o ura && \
setarch "$(uname -m)" -R ./ura file.ura && \
rm -rf ura