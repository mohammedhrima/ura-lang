#!/bin/bash
set -e
cd "$(dirname "$0")"

LLVM=~/llvm-16/bin/llvm-config
CC=~/llvm-16/bin/clang

CFLAGS=$($LLVM --cflags)
LDFLAGS=$($LLVM --ldflags)
LIBS=$($LLVM --libs core native bitwriter passes)
SYSLIBS=$($LLVM --system-libs)

# The official LLVM 16 release tarball needs -lzstd at link time, which
# requires libzstd-dev (the unversioned "libzstd.so" symlink). ldconfig
# doesn't track that symlink (it only tracks versioned runtime libs), so
# ask the linker directly instead of grepping ldconfig.
ZSTD_LIB=""
if ! echo 'int main(){}' | $CC -lzstd -xc - -o /dev/null 2>/dev/null; then
   RUNTIME=$(ldconfig -p | grep 'libzstd\.so\.1 ' | awk '{print $NF}' | head -1)
   if [ -n "$RUNTIME" ]; then
      mkdir -p .zstd-shim
      ln -sf "$RUNTIME" .zstd-shim/libzstd.so
      ZSTD_LIB="-L$(pwd)/.zstd-shim"
   fi
fi

mkdir -p build
$CC $CFLAGS -fsanitize=address -g3 main.c $LDFLAGS $ZSTD_LIB $LIBS $SYSLIBS -lstdc++ -o build/ura

echo "Built build/ura"

if [ "$#" -gt 0 ]; then
   ./build/ura "$@"
fi
