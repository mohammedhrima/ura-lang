
unset URA_PATH
clang src/main.c src/utils.c -fsanitize=null -g3 \
    $(llvm-config-14 --cflags --ldflags --libs core) -o \
    build/ura && ./build/ura ./src/file.ura
