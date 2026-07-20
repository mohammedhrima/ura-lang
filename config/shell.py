import os, sys, subprocess, shutil
from pathlib import Path

from config.ui import console, RICH, Panel, ANSI, _c, pr, emit, styled, err
from config.env import (ROOT, SCRIPT, check, build, install,
                        abbrev, git_branch, ura_fresh, llvm_version)
from config.commands import (tests, update, show, index, migrate,
                             rebuild, prune, doctor)

TASKS = {
    "check": check, "build": build, "install": install, "tests": tests,
    "update": update, "show": show, "index": index,
    "migrate": migrate, "rebuild": rebuild, "prune": prune,
    "doctor": doctor,
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
    "doctor":  "report duplicate case ids across tests/",
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
    cwd = os.getcwd()
    os.chdir(ROOT)          # tasks act on the project, not wherever you cd'd
    try:
        fn(*args)
    finally:
        os.chdir(cwd)

def show_help():
    shell = " · ".join(BUILTINS) + " · (else → your shell)"
    emit("env", "commands")
    for name in TASKS:
        pr(f"  {styled('ok', f'{name:9}')} {styled('dim', DESC.get(name, ''))}")
    emit("env", "shell")
    pr("  " + styled("dim", shell))

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
    names = list(TASKS)
    rows = [" · ".join(names[i:i + 5]) for i in range(0, len(names), 5)]
    if RICH:
        body = f"[dim]{meta}[/]\n\n" + "\n".join(rows) + f"\n\n[dim]{keys}[/]"
        console.print(Panel(body, title="ura · task shell", border_style="env", expand=False))
    else:
        e, d, r = ANSI["env"], ANSI["dim"], ANSI["reset"]
        print(f"{e}ura · task shell{r}  {d}{meta}{r}")
        for row in rows:
            print(row)
        print(f"{d}{keys}{r}")

def goodbye():
    emit("dim", "left the ura environment")

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