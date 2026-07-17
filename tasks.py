# /// script
# requires-python = ">=3.10"
# dependencies = ["rich>=13", "prompt_toolkit>=3"]
# ///
"""The ura task shell — build/test tooling for ura-lang (replaces anvil).

Run it with uv (this loads rich + prompt_toolkit automatically):

    uv run tasks.py            # enter the interactive ura shell
    uv run tasks.py build      # run one task
    uv run tasks.py tests      # run all .md tests (or: tests match · tests errors/lexer.md)

Do NOT run `uv run python3 tasks.py` — that skips the declared deps (no colors, no shell).
"""
import os, sys, subprocess, shutil, tempfile, re
from concurrent.futures import ThreadPoolExecutor, as_completed
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
    from rich.panel import Panel
    from rich.theme import Theme
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

# ---------------------------------------------------------------- helpers

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
        ok(f"build ok: {URA}")
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

# ---------------------------------------------------------------- consolidated .md tests

def _desc(src):
    first = (src.splitlines() or [""])[0]
    if not first.startswith("//"): return ""
    t = first.lstrip("/").strip()
    for sep in (" - ", " — "):
        if sep in t: return t.split(sep, 1)[1].strip()
    return ""

def _fence(tag, body):
    body = body.rstrip("\n")
    return f"```{tag}\n{body}\n```" if body else f"```{tag}\n```"

def _actual(src, stem, is_error):
    """Compile+run one source under -testing → always {ll, out, err} (empty when N/A).
    The unstable temp path (baked into @trap_msg + diagnostics) is normalized to <stem>.ura
    so goldens are deterministic AND portable across machines."""
    res = {"ll": "", "out": "", "err": "", "tree": ""}
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / f"{stem}.ura"
        p.write_text(src.rstrip() + "\n")
        paths = (str(p), os.path.realpath(p))
        def norm(s):
            for pp in paths: s = s.replace(pp, f"{stem}.ura")
            return s
        exe = Path(d) / "exe"
        c = run(URA, p, "-o", exe, "-testing", "-tree")
        if c.returncode != 0:                               # compile failed
            if is_error: res["err"] = norm(c.stderr)
            return res                                       # success-group fail → all empty → skip
        res["tree"] = norm(c.stdout)                         # -tree prints the AST to stdout during compile
        ll = p.parent / "build" / f"{stem}.ll"
        if ll.exists():
            res["ll"] = norm("\n".join(l for l in ll.read_text().splitlines()
                                       if not any(ig in l for ig in IGNORE)))
        r = run(exe)
        res["out"] = r.stdout
        if r.stderr or exit_code(r): res["err"] = norm(r.stderr) + f"exit: {exit_code(r)}\n"
        return res

def parse_md(path):
    """One group .md → [{name, ura, out?, err?, ll?}] (skips the ## index section)."""
    cases = []
    for part in re.split(r'(?m)^## ', Path(path).read_text())[1:]:
        head, _, body = part.partition('\n')
        if head.strip().lower() == 'index': continue
        blocks = dict(re.findall(r'(?ms)^```([a-z]+)\n(.*?)^```$', body))
        if 'ura' in blocks:
            cases.append({'name': head.strip(), **blocks})
    return cases

def _decolor(s):
    return re.sub(r'\x1b\[[0-9;]*m', '', s)

def migrate(dirpath):
    """ADD newly-implemented cases from <dir> into tests/<group>.md, PRESERVING already-migrated
    ones (success = compiles+runs; error = its recorded .err still reproduces). Run `prune` after."""
    d = Path(dirpath)
    is_error = "errors" in d.parts
    rel = Path(*d.parts[1:])                             # strip leading tests-old/
    out = (Path("tests") / rel).with_suffix(".md")
    cases = {c['name'].split()[0]: c for c in (parse_md(out) if out.exists() else [])}
    added = 0
    for ura in sorted(d.glob("*.ura")):
        if ura.stem in cases: continue
        src = ura.read_text()
        got = _actual(src, ura.stem, is_error)
        if is_error:
            old = ura.with_suffix(".err")
            if not old.exists() or got["err"].strip() != _decolor(old.read_text()).strip():
                continue                                 # error path not implemented yet
        elif not got["ll"]:
            continue                                     # doesn't compile yet
        cases[ura.stem] = {'name': f"{ura.stem} — {_desc(src)}".rstrip(" —"), 'ura': src, **got}
        added += 1
    if not cases:
        return 0
    index, sections = [], []
    for c in (cases[k] for k in sorted(cases)):
        index.append(f"- {c['name']}")
        blocks = [_fence("ura", c['ura'])] + [_fence(t, c.get(t, "")) for t in ("out", "err", "ll")]
        sections.append(f"## {c['name']}\n\n" + "\n\n".join(blocks))
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(f"# {str(rel).replace('/', ' / ')}\n\n## index\n\n" + "\n".join(index)
                   + "\n\n" + "\n\n".join(sections) + "\n")
    if added: ok(f"{out}: +{added} case(s) → {len(cases)} total")
    return added

def rebuild(src="tests-old"):
    """Regenerate a fresh tests/ from <src>: one .md per leaf dir, implemented cases only."""
    leaves = sorted({u.parent for u in Path(src).rglob("*.ura") if "build" not in u.parts})
    n = sum(migrate(d) for d in leaves)
    ok(f"rebuilt tests/ — {n} implemented cases across {len(leaves)} source groups")

def prune(src="tests-old"):
    """Delete from <src> every case that now lives in tests/ (migrated → implemented). Repeat as
    features land, until tests-old is empty and can be removed."""
    removed = 0
    for md in sorted(Path("tests").rglob("*.md")):
        olddir = Path(src) / Path(*md.parts[1:]).with_suffix("")
        for c in parse_md(md):
            stem = c['name'].split()[0]
            for f in list(olddir.glob(f"{stem}.*")) + list(olddir.glob(f"build/{stem}.ll")):
                if f.is_file(): f.unlink(); removed += 1
    for d in sorted((p for p in Path(src).rglob("*") if p.is_dir()),
                    key=lambda p: len(p.parts), reverse=True):
        try:
            if not any(d.iterdir()): d.rmdir()
        except OSError: pass
    ok(f"pruned {removed} migrated files from {src}")

def _group_header(md):
    rel = md.relative_to("tests").with_suffix("") if "tests" in md.parts else md.with_suffix("").name
    console.print(f"[env]{rel}[/]") if RICH else print(_c("env", str(rel)))

def _row(status, label):
    tag = ("[ok]PASS[/]" if status == "pass" else "[fail]FAIL[/]") if RICH else \
          (_c("ok", "PASS") if status == "pass" else _c("fail", "FAIL"))
    (console.print if RICH else print)(f"  {tag}  {label}")

def _summary(p, f, sk):
    passed = f"[ok]{p} passed[/]" if RICH else _c("ok", f"{p} passed")
    failed = (f"[fail]{f} failed[/]" if f else "[dim]0 failed[/]") if RICH else \
             (_c("fail", f"{f} failed") if f else _c("dim", "0 failed"))
    head = "[env]tests[/]" if RICH else _c("env", "tests")
    line = f"{head}  {passed} · {failed}"
    if sk:
        line += f" · [dim]{sk} skipped[/]" if RICH else " · " + _c("dim", f"{sk} skipped")
    (console.print if RICH else print)(line)

def _resolve(target):
    if not target:
        return Path("tests")
    for cand in (Path(target), Path(target).with_suffix(".md"),
                 Path("tests") / target, (Path("tests") / target).with_suffix(".md")):
        if cand.exists():
            return cand
    return None

def _run_case(case, is_error):
    got = _actual(case['ura'], case['name'].split()[0], is_error)
    bad = next((t for t in ("ll", "out", "err", "tree") if got[t].strip() != case.get(t, "").strip()), None)
    return case, bad

def tests(target=None):
    """run .md tests — no arg = all of tests/ · a dir = its .md files · a .md file = just that one."""
    root = _resolve(target)
    if root is None:
        err(f"no such test path: {target}"); return
    mds = [root] if root.suffix == ".md" else sorted(root.rglob("*.md"))
    p = f = sk = 0
    for md in mds:
        is_error = "errors" in md.parts
        cases = parse_md(md)
        runnable = [c for c in cases if any(c.get(t, "").strip() for t in ("ll", "out", "err"))]
        sk += len(cases) - len(runnable)
        if not runnable:
            continue
        _group_header(md)
        with ThreadPoolExecutor() as ex:
            for fut in as_completed([ex.submit(_run_case, c, is_error) for c in runnable]):
                case, bad = fut.result()
                if bad:
                    f += 1; _row("fail", f"{case['name']} ({bad})")
                else:
                    p += 1; _row("pass", case['name'])
    _summary(p, f, sk)

def update(target="tests"):
    """Regenerate out/err/ll goldens in place for every case in the target .md file(s)."""
    mds = [Path(target)] if str(target).endswith(".md") else sorted(Path(target).rglob("*.md"))
    for md in mds:
        is_error = "errors" in md.parts
        index, sections = [], []
        for c in parse_md(md):
            got = _actual(c['ura'], c['name'].split()[0], is_error)
            index.append(f"- {c['name']}")
            blocks = [_fence("ura", c['ura']), _fence("tree", got["tree"])] + [_fence(t, got[t]) for t in ("out", "err", "ll")]
            sections.append(f"## {c['name']}\n\n" + "\n\n".join(blocks))
        rel = Path(*md.parts[1:]).with_suffix("")
        md.write_text(f"# {str(rel).replace('/', ' / ')}\n\n## index\n\n" + "\n".join(index)
                      + "\n\n" + "\n\n".join(sections) + "\n")
        ok(f"updated {md} ({len(index)} cases)")

def show(spec):
    """show <group>:<NNN>  — print one case (jump straight to it, no scrolling the .md)."""
    group, _, which = spec.partition(":")
    for c in parse_md(Path("tests") / f"{group}.md"):
        if not which or c['name'].split()[0] == which or which in c['name']:
            print(f"## {c['name']}\n")
            for tag in ('ura', 'out', 'err', 'll'):
                if tag in c: print(_fence(tag, c[tag]) + "\n")

def index(group):
    """index <group>  — list the case names in a group .md."""
    for c in parse_md(Path("tests") / f"{group}.md"): print(c['name'])

# ---------------------------------------------------------------- dispatch + shell

TASKS = {
    "check": check, "build": build, "install": install, "tests": tests,
    "update": update, "show": show, "index": index,
    "migrate": migrate, "rebuild": rebuild, "prune": prune,
}
DESC = {
    "check":   "verify clang + llvm-config-14 are installed",
    "build":   "compile the ura compiler: build/ura",
    "install": "install clang + llvm@14 via brew/apt",
    "tests":   "run .md tests — no arg = all · a dir or .md file = subset",
    "update":  "regenerate a group's out/err/ll goldens",
    "show":    "print one case — show <group>:<NNN>",
    "index":   "list the case names in a group",
    "migrate": "add newly-passing tests-old cases into tests/",
    "rebuild": "migrate every tests-old group at once",
    "prune":   "drop already-migrated cases from tests-old",
}
GROUPS = {
    "build":   ["check", "build", "install"],
    "test":    ["tests"],
    "author":  ["update", "show", "index"],
    "backlog": ["migrate", "rebuild", "prune"],
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
    shell = " · ".join(BUILTINS) + " · (else → your shell)"
    if RICH:
        for group, names in GROUPS.items():
            console.print(f"[env]{group}[/]")
            for name in names:
                console.print(f"  [ok]{name:9}[/] [dim]{DESC.get(name, '')}[/]")
        console.print(f"[env]shell[/]\n  [dim]{shell}[/]")
    else:
        for group, names in GROUPS.items():
            print(_c("env", group))
            for name in names:
                print(f"  {_c('ok', f'{name:9}')} {_c('dim', DESC.get(name, ''))}")
        print(_c("env", "shell") + "\n  " + _c("dim", shell))

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
    clang = "✓" if shutil.which("clang") else "✗"
    meta = f"LLVM {llvm_version()} · clang {clang} · git {git_branch() or '—'}"
    keys = "⇥ complete · ↑ history · ^D exit · type help"
    if RICH:
        rows = "\n".join(f"[env]{g:8}[/] {' · '.join(names)}" for g, names in GROUPS.items())
        body = f"[dim]{meta}[/]\n\n{rows}\n\n[dim]{keys}[/]"
        console.print(Panel(body, title="ura · task shell", border_style="env", expand=False))
    else:
        e, d, r = ANSI["env"], ANSI["dim"], ANSI["reset"]
        print(f"{e}ura · task shell{r}  {d}{meta}{r}")
        for g, names in GROUPS.items():
            print(f"{e}{g:8}{r} " + " · ".join(names))
        print(f"{d}{keys}{r}")

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
        print(_c("dim", "tip: launch with `uv run tasks.py` for colors + the interactive shell"), file=sys.stderr)
    if len(sys.argv) > 1:
        dispatch(*sys.argv[1:])
    elif sys.stdin.isatty():
        ura_shell()
    else:
        plain_loop()
