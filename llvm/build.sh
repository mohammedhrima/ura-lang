# clang++ main.cpp \
#   -I/opt/homebrew/opt/llvm/include \
#   -L/opt/homebrew/opt/llvm/lib \
#   -Wno-unused-command-line-argument \
#   -lLLVM

cc $1 `llvm-config --cflags --ldflags --libs core` \
&& ./a.out && rm -rf a.out && llc out.ll -o out.s
