import os, subprocess, shutil
from functools import lru_cache
from pathlib import Path

from config.ui import ok, err

os.environ.setdefault("URA_LIB", "std")

ROOT   = Path(__file__).resolve().parent.parent
SCRIPT = str(ROOT / "config" / "tasks.py")
BUILD  = ROOT / "build"
URA    = BUILD / "ura"
# substrings of .ll lines that vary per run / machine — ignored when diffing goldens
IGNORE = ("target triple", "target datalayout", "source_filename", "ModuleID", "DIFile", "DICompileUnit")
# dev builds of the compiler itself are sanitized, so a NULL deref reports a file
# and line instead of dying as a silent SIGSEGV. `build release` opts out.
# NOT address: -fsanitize=address deadlocks this binary at startup on macOS/arm64
# (it hangs before main, even with no arguments). undefined alone is fine and fast.
SAN = ["-fsanitize=undefined", "-fno-omit-frame-pointer", "-g", "-O0"]

class Timeout:
    """Stands in for a CompletedProcess that never finished."""
    def __init__(self, secs):
        self.returncode = 124                       # what `timeout(1)` reports
        self.stdout     = ""
        self.stderr     = f"timed out after {secs}s\n"

def run(*args, timeout=None):
    argv = [str(a) for a in args]
    try:
        return subprocess.run(argv, capture_output=True, text=True,
                              timeout=timeout)
    except subprocess.TimeoutExpired:
        return Timeout(timeout)

def exit_code(proc):
    # subprocess returns -signal when killed by a signal; show it as 128+signal
    return proc.returncode if proc.returncode >= 0 else 128 - proc.returncode

@lru_cache
def llvm_version():
    r = run("llvm-config-14", "--version")
    return r.stdout.strip() if r.returncode == 0 else "?"

def git_branch():
    r = run("git", "rev-parse", "--abbrev-ref", "HEAD")
    return r.stdout.strip() if r.returncode == 0 else ""

def ura_fresh():
    if not URA.exists():
        return False
    srcs = list((ROOT / "src").rglob("*.c")) + list((ROOT / "src").rglob("*.h"))
    return not srcs or URA.stat().st_mtime >= max(s.stat().st_mtime for s in srcs)

def abbrev(path):
    home = str(Path.home())
    return "~" + path[len(home):] if path == home or path.startswith(home + os.sep) else path

def check():
    have = shutil.which("clang") and shutil.which("llvm-config-14")
    (ok if have else err)("clang + llvm-config-14 present" if have else "missing clang or llvm-config-14")

def build(mode="dev"):
    BUILD.mkdir(exist_ok=True)
    flags = run("llvm-config-14", "--cflags", "--ldflags", "--libs", "core").stdout.split()
    san = [] if mode == "release" else SAN
    r = run("clang", "src/main.c", *flags, *san, "-o", URA)
    if r.returncode == 0:
        ok(f"build ok: {URA.relative_to(ROOT)}{'' if san else ' (release, no sanitizers)'}")
    else:
        err("build failed"); print(r.stderr)

def install():
    if shutil.which("clang"):
        ok("clang present"); return
    if shutil.which("brew"):
        run("brew", "install", "llvm@14", "clang-format"); ok("installed")
    elif shutil.which("apt-get"):
        run("sudo", "apt-get", "install", "-y", "llvm-14", "clang-14", "clang-format"); ok("installed")
    else:
        err("no package manager")

def activate_env():
    """Put build/ura's directory on PATH so `ura` runs like a command (venv-style)."""
    bdir = str(BUILD)
    if bdir not in os.environ.get("PATH", "").split(os.pathsep):
        os.environ["PATH"] = bdir + os.pathsep + os.environ.get("PATH", "")