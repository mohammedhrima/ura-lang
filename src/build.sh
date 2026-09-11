#!/bin/sh
# Build old.c into build/ura against whichever LLVM is installed, then run
# build/ura with this script's arguments, if any.
# The newest llvm-config found is used; to choose one yourself:
#     LLVM_CONFIG=/path/to/llvm-config ./build.sh
set -e
cd "$(dirname "$0")"

CC=clang
FLAGS="-fsanitize=address -g3"

# every llvm-config in sight: on PATH, plain or versioned (Ubuntu's
# llvm-config-12), and in the usual prefixes (Debian/Ubuntu, Homebrew,
# release tarballs unpacked in $HOME)
candidates() {
   for name in llvm-config $(seq -f 'llvm-config-%g' 3 40); do
      command -v "$name" || true
   done
   ls /usr/lib/llvm-*/bin/llvm-config /usr/local/opt/llvm*/bin/llvm-config \
      /opt/homebrew/opt/llvm*/bin/llvm-config "$HOME"/llvm*/bin/llvm-config 2>/dev/null || true
}

links() {
   echo 'int main(void) { return 0; }' | $CC -x c - "$@" -o /dev/null 2>/dev/null
}

if [ -z "$LLVM_CONFIG" ]; then
   LLVM_CONFIG=$(for c in $(candidates); do echo "$("$c" --version) $c"; done |
                 sort -t . -k1,1n -k2,2n -k3,3n | tail -n 1 | cut -d ' ' -f 2)
fi
if ! command -v "$LLVM_CONFIG" >/dev/null 2>&1; then
   echo "build.sh: no llvm-config found, run as: LLVM_CONFIG=/path/to/llvm-config ./build.sh" >&2
   exit 1
fi
VERSION=$("$LLVM_CONFIG" --version)
echo "using LLVM $VERSION ($LLVM_CONFIG)"

# ipo holds the legacy pass manager old.c falls back to before LLVM 13; the
# rpath lets build/ura find a shared LLVM installed outside the system paths
LIBS="$("$LLVM_CONFIG" --ldflags) -Wl,-rpath,$("$LLVM_CONFIG" --libdir)"
LIBS="$LIBS $("$LLVM_CONFIG" --libs core native bitwriter passes ipo)"

# a static LLVM is C++, and clang only adds the C++ runtime when linking C++
if [ "$("$LLVM_CONFIG" --shared-mode)" = static ]; then
   case "$(uname)" in
      Darwin) LIBS="$LIBS -lc++" ;;
      *)      LIBS="$LIBS -lstdc++" ;;
   esac
fi

# --system-libs can name libraries (-lzstd, -lxml2...) that are only installed
# as versioned runtimes (libzstd.so.1) when their -dev package is missing, so -l
# can't find them; link through a temporary unversioned symlink instead
SHIM=
for lib in $("$LLVM_CONFIG" --system-libs); do
   LIBS="$LIBS $lib"
   case $lib in -l*) ;; *) continue ;; esac
   links "$lib" && continue
   runtime=$(ldconfig -p 2>/dev/null |
             awk -v n="lib${lib#-l}.so." 'index($1, n) == 1 { print $NF; exit }')
   [ -n "$runtime" ] || continue
   if [ -z "$SHIM" ]; then
      SHIM=$(mktemp -d)
      trap 'rm -rf "$SHIM"' EXIT
      LIBS="-L$SHIM $LIBS"
   fi
   ln -sf "$runtime" "$SHIM/lib${lib#-l}.so"
done

mkdir -p build
$CC $FLAGS $("$LLVM_CONFIG" --cflags) main.c $LIBS -o build/ura || {
   echo "build.sh: build failed against LLVM $VERSION; to try another one:" >&2
   echo "    LLVM_CONFIG=/path/to/llvm-config ./build.sh" >&2
   exit 1
}
echo "Built build/ura"

if [ "$#" -gt 0 ]; then
   # ASan's shadow memory can clash with high-entropy ASLR, so run without
   # ASLR where setarch exists (Linux)
   if command -v setarch >/dev/null 2>&1; then
      setarch "$(uname -m)" -R ./build/ura "$@"
   else
      ./build/ura "$@"
   fi
fi
