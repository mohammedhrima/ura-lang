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
    console, Panel, RICH = None, None, False

def _c(cls, s):  # raw ANSI wrap (fallback output)
    return f"{ANSI[cls]}{s}{ANSI['reset']}"

# style -> (rich tag, ANSI key)
_STYLE = {"ok": ("ok", "ok"), "err": ("fail", "fail"), "warn": ("warn", "branch"),
          "env": ("env", "env"), "dim": ("dim", "dim")}

def pr(s):
    (console.print if RICH else print)(s)

def styled(style, text):
    """One styled fragment, in whichever dialect is active. Compose these
    when a line mixes styles; use emit() when the whole line is one style."""
    tag, key = _STYLE[style]
    return f"[{tag}]{text}[/]" if RICH else _c(key, text)

def emit(style, text):
    pr(styled(style, text))

def ok(m):   emit("ok", m)
def err(m):  emit("err", m)
def warn(m): emit("warn", m)

def _group_header(md):
    rel = md.relative_to("tests").with_suffix("") if "tests" in md.parts else md.with_suffix("").name
    emit("env", str(rel))

def _row(status, label):
    good = status == "pass"
    tag  = styled("ok", "PASS") if good else styled("err", "FAIL")
    pr(f"  {tag}  {label}")

def _summary(p, f, sk):
    passed = styled("ok", f"{p} passed")
    failed = styled("err", f"{f} failed") if f else styled("dim", "0 failed")
    ln     = f"{styled('env', 'tests')}  {passed} · {failed}"
    if sk:
        ln += " · " + styled("dim", f"{sk} skipped")
    pr(ln)
