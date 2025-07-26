# clang++ main.cpp \
#   -I/opt/homebrew/opt/llvm/include \
#   -L/opt/homebrew/opt/llvm/lib \
#   -Wno-unused-command-line-argument \
#   -lLLVM

rm -rf out.ir out.s a.out exe.out
cc $1 `llvm-config --cflags --ldflags --libs core`
./a.out
rm -rf a.out
llc out.ir -o out.s
cc out.s -o exe.out
