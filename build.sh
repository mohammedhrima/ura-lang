LLVM_FLAGS=$(llvm-config-12 --cflags)
# LLVM_LDFLAGS=($(llvm-config-12 --ldflags --libs core))

# flags=$(llvm-config-12 --cflags)
# echo $flags

FLAGS="$LLVM_FLAGS -fsanitize=address -g3 "

clear
clang $FLAGS main.c && ./a.out main.ura
