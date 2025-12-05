# clang++ main.cpp \
#   -I/opt/homebrew/opt/llvm/include \
#   -L/opt/homebrew/opt/llvm/lib \
#   -Wno-unused-command-line-argument \
#   -lLLVM

rm -rf out.ll out.s && 
clang $1 `llvm-config --cflags --ldflags --libs core` -Wall -Werror -fsanitize=address -g3 && \
    ./a.out && rm -rf a.out && llc out.ll -o out.s && \
    clang out.s -o exe.out && cat out.ll
