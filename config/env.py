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
IGNORE = ("target triple", "source_filename", "ModuleID", "DIFile", "DICompileUnit")

def run(*args):
    return subprocess.run([str(a) for a in args], capture_output=True, text=True)

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

def build():
    BUILD.mkdir(exist_ok=True)
    flags = run("llvm-config-14", "--cflags", "--ldflags", "--libs", "core").stdout.split()
    r = run("clang", "src/main.c", *flags, "-o", URA)
    if r.returncode == 0:
        ok(f"build ok: {URA.relative_to(ROOT)}")
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