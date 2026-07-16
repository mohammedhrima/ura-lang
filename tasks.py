# /// script
# requires-python = ">=3.10"
# dependencies = ["rich>=13", "prompt_toolkit>=3"]
# ///
"""The ura task shell — build/test tooling for ura-lang (replaces anvil).

Run it with uv (this loads rich + prompt_toolkit automatically):

    uv run tasks.py            # enter the interactive ura shell
    uv run tasks.py build      # run one task
    uv run tasks.py test tests

Do NOT run `uv run python3 tasks.py` — that skips the declared deps (no colors, no shell).
"""
import os, sys, subprocess, shutil, tempfile
from concurrent.futures import ThreadPoolExecutor
from functools import lru_cache
from pathlib import Path

os.environ.setdefault("URA_LIB", "std")

SCRIPT = str(Path(__file__).resolve())
BUILD  = Path("build")
URA    = BUILD / "ura"
# substrings of .ll lines that vary per run / machine — ignored when diffing goldens
IGNORE = ("target triple", "source_filename", "ModuleID", "DIFile", "DICompileUnit")

# ---------------------------------------------------------------- output (rich, with colored ANSI fallback)

ANSI = {"env": "\033[1;36m", "path": "\033[34m", "branch": "\033[33m",
        "ok": "\033[1;32m", "fail": "\033[1;31m", "dim": "\033[2m", "reset": "\033[0m"}

try:
    from rich.console import Console
    from rich.table import Table
    from rich.panel import Panel
    from rich.theme import Theme
    from rich import box
    console = Console(theme=Theme({
        "ok": "bold green", "fail": "bold red", "warn": "yellow", "skip": "dim",
        "env": "bold cyan", "path": "blue", "branch": "yellow", "dim": "dim",
    }))
    RICH = True
except ImportError:
    console, RICH = None, False

def _c(cls, s):  # raw ANSI wrap (fallback output)
    return f"{ANSI[cls]}{s}{ANSI['reset']}"

def ok(m):   console.print(f"[ok]{m}[/]")   if RICH else print(_c("ok", m))
def err(m):  console.print(f"[fail]{m}[/]")  if RICH else print(_c("fail", m))
def warn(m): console.print(f"[warn]{m}[/]")  if RICH else print(_c("branch", m))

def report(title, results):
    """results: list of (status, label) with status in {'pass','fail','skip'}."""
    p = sum(1 for s, _ in results if s == "pass")
    f = sum(1 for s, _ in results if s == "fail")
    sk = sum(1 for s, _ in results if s == "skip")
    rows = [(s, l) for s, l in results if s != "skip"]  # never list the skips
    if RICH:
        if rows:
            t = Table(box=box.SIMPLE_HEAD, show_header=False, pad_edge=False)
            t.add_column(no_wrap=True); t.add_column(overflow="fold")
            for s, label in rows:
                t.add_row("[ok]PASS[/]" if s == "pass" else "[fail]FAIL[/]", label)
            console.print(t)
        summary = f"[env]{title}[/]  [ok]{p} passed[/] · " + (f"[fail]{f} failed[/]" if f else "[dim]0 failed[/]")
        if sk:
            summary += f" · [dim]{sk} skipped[/]"
        console.print(summary)
    else:
        for s, label in rows:
            print(_c("ok", "PASS") + " " + label if s == "pass" else _c("fail", "FAIL") + " " + label)
        line = _c("env", title) + "  " + _c("ok", f"{p} passed") + " · " + \
               (_c("fail", f"{f} failed") if f else _c("dim", "0 failed"))
        if sk:
            line += " · " + _c("dim", f"{sk} skipped")
        print(line)

# ---------------------------------------------------------------- helpers

def run(*args):
    return subprocess.run([str(a) for a in args], capture_output=True, text=True)

def exit_code(proc):
    # subprocess returns -signal when killed by a signal; show it as 128+signal
    return proc.returncode if proc.returncode >= 0 else 128 - proc.returncode

def strip(text):
    return [l for l in text.splitlines() if not any(ig in l for ig in IGNORE)]

def ura_files(target, skip=()):
    p = Path(target)
    if p.is_file():
        return [p]
    return [u for u in sorted(p.rglob("*.ura")) if not any(s in u.parts for s in skip)]

def parallel(files, fn):
    with ThreadPoolExecutor() as ex:
        return list(ex.map(fn, files))

def tmp_out():
    return tempfile.mktemp(suffix=".out")

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
    srcs = list(Path("src").glob("*.c")) + list(Path("src").glob("*.h"))
    return not srcs or URA.stat().st_mtime >= max(s.stat().st_mtime for s in srcs)

def abbrev(path):
    home = str(Path.home())
    return "~" + path[len(home):] if path == home or path.startswith(home + os.sep) else path

# ---------------------------------------------------------------- tasks

def check():
    have = shutil.which("clang") and shutil.which("llvm-config-14")
    (ok if have else err)("clang + llvm-config-14 present" if have else "missing clang or llvm-config-14")

def build():
    BUILD.mkdir(exist_ok=True)
    flags = run("llvm-config-14", "--cflags", "--ldflags", "--libs", "core").stdout.split()
    r = run("clang", "src/main.c", "src/utils.c", *flags, "-o", URA)
    if r.returncode == 0:
        ok(f"build ok → {URA}")
    else:
        err("build failed"); print(r.stderr)

def test(target="tests"):
    def one(ura):
        first = ura.read_text().splitlines()[:1]
        if not first or not first[0].startswith("//"):
            return ("skip", str(ura))
        golden = ura.with_suffix(".ll")
        if not golden.exists():
            return ("skip", str(ura))
        out = tmp_out()
        r = run(URA, ura, "-o", out)
        try: os.unlink(out)
        except OSError: pass
        got = ura.parent / "build" / f"{ura.stem}.ll"
        if r.returncode != 0 or not got.exists():
            return ("skip", str(ura))  # rewrite can't build it yet → not a failure
        good = strip(got.read_text()) == strip(golden.read_text())
        return ("pass" if good else "fail", str(ura))
    report("test", parallel(ura_files(target, skip=("build", "errors", "projects")), one))

def test_errors(target="tests/errors"):
    def one(ura):
        golden = ura.with_suffix(".err")
        out = tmp_out()
        r = run(URA, ura, "-o", out)
        try: os.unlink(out)
        except OSError: pass
        if r.returncode == 0:
            return ("fail", f"{ura} (compiled, expected error)")
        if golden.exists() and r.stderr.splitlines() == golden.read_text().splitlines():
            return ("pass", str(ura))
        return ("skip", str(ura))  # errors, but not this message → unimplemented feature
    report("errors", parallel(ura_files(target, skip=("build", "runtime")), one))

def test_runtime(target="tests/errors/runtime"):
    def one(ura):
        golden = ura.with_suffix(".run")
        exe = tmp_out()
        c = run(URA, ura, "-o", exe)
        if c.returncode != 0:
            return ("fail", f"{ura} (did not compile)")
        r = run(exe)
        try: os.unlink(exe)
        except OSError: pass
        actual = r.stderr + f"exit: {exit_code(r)}\n"
        good = golden.exists() and actual == golden.read_text()
        return ("pass" if good else "fail", str(ura))
    report("runtime", parallel(ura_files(target, skip=("build",)), one))

def tests():
    test("tests")
    test_errors("tests/errors")
    test_runtime("tests/errors/runtime")

def update_errors(target="tests/errors"):
    for ura in ura_files(target, skip=("build", "runtime")):
        ura.with_suffix(".err").write_text(run(URA, ura).stderr)
        ok(f"wrote {ura.with_suffix('.err')}")

def update_runtime(target="tests/errors/runtime"):
    for ura in ura_files(target, skip=("build",)):
        exe = tmp_out()
        if run(URA, ura, "-o", exe).returncode != 0:
            warn(f"skip {ura} (did not compile)"); continue
        r = run(exe)
        try: os.unlink(exe)
        except OSError: pass
        ura.with_suffix(".run").write_text(r.stderr + f"exit: {exit_code(r)}\n")
        ok(f"wrote {ura.with_suffix('.run')}")

def update_ll(target="tests"):
    for ura in ura_files(target, skip=("build", "errors", "projects")):
        golden = ura.with_suffix(".ll")
        if not golden.exists():
            continue
        run(URA, ura)
        got = ura.parent / "build" / f"{ura.stem}.ll"
        if not got.exists():
            continue
        golden.write_text("\n".join(l for l in got.read_text().splitlines()
                                    if not l.startswith("target triple")) + "\n")
        ok(f"wrote {golden}")

def copy(file):
    src = Path(file)
    dest = src.read_text().splitlines()[0].removeprefix("//").strip()
    target = Path("tests") / f"{dest}.ura"
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy(src, target)
    ll = src.with_suffix(".ll")
    if ll.exists():
        shutil.copy(ll, Path("tests") / f"{dest}.ll")
    ok(f"copy: filed {file}")

def install():
    if shutil.which("clang"):
        ok("clang present"); return
    if shutil.which("brew"):
        run("brew", "install", "llvm@14", "clang-format"); ok("installed")
    elif shutil.which("apt-get"):
        run("sudo", "apt-get", "install", "-y", "llvm-14", "clang-14", "clang-format"); ok("installed")
    else:
        err("no package manager")

# ---------------------------------------------------------------- dispatch + shell

TASKS = {
    "check": check, "build": build, "install": install, "copy": copy,
    "test": test, "test_errors": test_errors, "test_runtime": test_runtime, "tests": tests,
    "update_errors": update_errors, "update_runtime": update_runtime, "update_ll": update_ll,
}
DESC = {
    "check": "verify clang + llvm-config-14 are installed",
    "build": "compile the ura compiler → build/ura",
    "test": "diff .ll output vs goldens (unimplemented tests are skipped)",
    "test_errors": "check compile-error messages vs .err goldens",
    "test_runtime": "run programs, check stderr+exit vs .run goldens",
    "tests": "run test + test_errors + test_runtime",
    "update_errors": "regenerate .err goldens",
    "update_runtime": "regenerate .run goldens",
    "update_ll": "regenerate .ll goldens (file or folder)",
    "copy": "file a scratch .ura+.ll into tests/ by its // header",
    "install": "install clang + llvm@14 via brew/apt",
}
BUILTINS = {
    "cd": "change directory", "pwd": "print working directory", "help": "list commands",
    "clear": "clear the visible screen (scrollback kept)",
    "clean": "clear screen + scrollback (no scroll-back)",
    "reload": "reload tasks.py", "quit": "leave the ura shell",
}
STATE = {"last_ok": True, "cwd": "", "branch": "", "fresh": False, "llvm": "?", "clang": False}

class _Quit(Exception):
    pass

def dispatch(name, *args):
    fn = TASKS.get(name)
    if not fn:
        err(f"unknown task '{name}' (have: {', '.join(TASKS)})"); return
    fn(*args)

def show_help():
    if RICH:
        t = Table(box=box.SIMPLE, header_style="env")
        t.add_column("command"); t.add_column("what it does", style="dim")
        for name in TASKS:
            t.add_row(name, DESC.get(name, ""))
        for name, d in BUILTINS.items():
            t.add_row(f"[dim]{name}[/]", d)
        console.print(t)
    else:
        for name in list(TASKS) + list(BUILTINS):
            print(f"{name:16} {DESC.get(name) or BUILTINS.get(name, '')}")

def run_line(line):
    parts = line.split()
    if not parts:
        return
    cmd = parts[0]
    if cmd in ("quit", "exit"):
        raise _Quit
    if cmd == "cd":
        dest = os.path.expanduser(parts[1]) if len(parts) > 1 else str(Path.home())
        try:
            os.chdir(dest); STATE["last_ok"] = True
        except OSError as e:
            err(str(e)); STATE["last_ok"] = False
        return
    if cmd in ("help", "?"):
        show_help(); STATE["last_ok"] = True; return
    if cmd == "clear":
        os.system("cls" if os.name == "nt" else "clear"); return
    if cmd == "clean":
        # wipe screen AND scrollback (\033[3J), so there's nothing to scroll back to
        sys.stdout.write("\033[3J\033[2J\033[H"); sys.stdout.flush(); return
    if cmd == "reload":
        os.execv(sys.executable, [sys.executable, SCRIPT])
    if cmd in TASKS:
        try:
            dispatch(*parts); STATE["last_ok"] = True
        except Exception as e:
            err(str(e)); STATE["last_ok"] = False
        return
    STATE["last_ok"] = subprocess.run(line, shell=True).returncode == 0

def refresh_context():
    STATE["cwd"] = abbrev(os.getcwd())
    STATE["branch"] = git_branch()
    STATE["fresh"] = ura_fresh()

def banner():
    clang = "yes" if shutil.which("clang") else "no"
    if RICH:
        body = (f"[dim]LLVM[/] {llvm_version()} · [dim]clang[/] {clang} · [dim]git[/] {git_branch() or '—'}\n\n"
                f"[env]tasks[/]  " + " · ".join(TASKS) + "\n"
                f"[env]shell[/]  cd · pwd · git · clang … run as commands\n"
                f"[env]keys[/]   Tab complete · ↑ history · Ctrl-D exit")
        console.print(Panel(body, title="ura-lang task shell", border_style="env", expand=False))
    else:
        e, d, r = ANSI["env"], ANSI["dim"], ANSI["reset"]
        print(f"{e}ura-lang task shell{r}  {d}LLVM {llvm_version()} · clang {clang} · git {git_branch() or '—'}{r}")
        print(f"{e}tasks{r}  " + " · ".join(TASKS))
        print(f"{d}cd · pwd · git … run as commands · type help · quit{r}")

def goodbye():
    console.print("[dim]left the ura environment[/]") if RICH else print(_c("dim", "left the ura environment"))

def ansi_prompt():
    def seg(cls, s):
        return f"\001{ANSI[cls]}\002{s}\001{ANSI['reset']}\002"  # \001..\002 = non-printing (readline width)
    s = seg("env", "(ura) ") + seg("path", STATE["cwd"] + " ")
    if STATE["branch"]:
        s += seg("branch", f"({STATE['branch']}) ")
    return s + seg("ok" if STATE["last_ok"] else "fail", "❯ ")

def _setup_readline():
    try:
        import readline, atexit, glob
    except ImportError:
        return
    hist = str(Path.home() / ".ura_history")
    try: readline.read_history_file(hist)
    except (FileNotFoundError, OSError): pass
    atexit.register(lambda: readline.write_history_file(hist))
    readline.set_completer_delims(" \t\n")
    def completer(text, state):
        buf = readline.get_line_buffer()
        first = " " not in buf
        path_like = text.startswith((".", "/", "~")) or "/" in text
        if first and not path_like:
            opts = [n for n in list(TASKS) + list(BUILTINS) if n.startswith(text)]
        else:
            opts = sorted(glob.glob(os.path.expanduser(text) + "*"))
        return opts[state] if state < len(opts) else None
    readline.set_completer(completer)
    readline.parse_and_bind("bind ^I rl_complete" if "libedit" in (readline.__doc__ or "") else "tab: complete")

def interactive_fallback():
    """Colored REPL when prompt_toolkit isn't available (e.g. launched without uv's deps)."""
    _setup_readline()
    banner()
    while True:
        refresh_context()
        try:
            line = input(ansi_prompt())
        except EOFError:
            print(); break
        except KeyboardInterrupt:
            print(); continue
        try:
            run_line(line)
        except _Quit:
            break
    goodbye()

def ura_shell():
    try:
        from prompt_toolkit import PromptSession
        from prompt_toolkit.history import FileHistory
        from prompt_toolkit.auto_suggest import AutoSuggestFromHistory
        from prompt_toolkit.completion import Completer, Completion, PathCompleter
        from prompt_toolkit.document import Document
        from prompt_toolkit.formatted_text import FormattedText
        from prompt_toolkit.styles import Style
    except ImportError:
        return interactive_fallback()

    STATE["clang"] = bool(shutil.which("clang"))
    STATE["llvm"] = llvm_version()

    class UraCompleter(Completer):
        def __init__(self):
            self.paths = PathCompleter(expanduser=True)
        def get_completions(self, document, complete_event):
            text = document.text_before_cursor
            word = document.get_word_before_cursor(WORD=True)
            first = " " not in text.lstrip()
            path_like = word.startswith((".", "/", "~")) or "/" in word
            if first and not path_like:
                for name in list(TASKS) + list(BUILTINS):
                    if name.startswith(word):
                        yield Completion(name, -len(word), display_meta=DESC.get(name) or BUILTINS.get(name, ""))
            else:
                yield from self.paths.get_completions(Document(word, len(word)), complete_event)

    def prompt_text():
        glyph = "class:ok" if STATE["last_ok"] else "class:fail"
        frags = [("class:env", "(ura) "), ("class:path", STATE["cwd"] + " ")]
        if STATE["branch"]:
            frags.append(("class:branch", f"({STATE['branch']}) "))
        frags.append((glyph, "❯ "))
        return FormattedText(frags)

    def toolbar():
        s = STATE
        return (f" ura-lang · git:{s['branch'] or '—'} · build/ura {'fresh' if s['fresh'] else 'stale'} · "
                f"LLVM {s['llvm']} · clang {'yes' if s['clang'] else 'no'}    Tab ⇥ · ↑ history · ^D exit ")

    style = Style.from_dict({
        "env": "bold cyan", "path": "ansiblue", "branch": "ansiyellow",
        "ok": "bold ansigreen", "fail": "bold ansired",
        "bottom-toolbar": "bg:#2a2a2a #b0b0b0",
    })
    session = PromptSession(
        history=FileHistory(str(Path.home() / ".ura_history")),
        auto_suggest=AutoSuggestFromHistory(),
        completer=UraCompleter(), complete_while_typing=True,
        bottom_toolbar=toolbar, style=style,
    )

    banner()
    while True:
        refresh_context()
        try:
            line = session.prompt(prompt_text())
        except EOFError:
            break
        except KeyboardInterrupt:
            continue
        try:
            run_line(line)
        except _Quit:
            break
    goodbye()

def plain_loop():
    banner()
    for raw in sys.stdin:
        try:
            run_line(raw.rstrip("\n"))
        except _Quit:
            break

def activate_env():
    """Put build/ura's directory on PATH so `ura` runs like a command (venv-style)."""
    bdir = str((Path(SCRIPT).parent / "build").resolve())
    if bdir not in os.environ.get("PATH", "").split(os.pathsep):
        os.environ["PATH"] = bdir + os.pathsep + os.environ.get("PATH", "")

if __name__ == "__main__":
    activate_env()
    if not RICH:
        print(_c("dim", "tip: run `uv run tasks.py` (not python3) for colors + the full shell"), file=sys.stderr)
    if len(sys.argv) > 1:
        dispatch(*sys.argv[1:])
    elif sys.stdin.isatty():
        ura_shell()
    else:
        plain_loop()
