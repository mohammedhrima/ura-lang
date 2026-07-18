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

def emit(style, text):
    tag, key = _STYLE[style]
    pr(f"[{tag}]{text}[/]" if RICH else _c(key, text))

def ok(m):   emit("ok", m)
def err(m):  emit("err", m)
def warn(m): emit("warn", m)

def _group_header(md):
    rel = md.relative_to("tests").with_suffix("") if "tests" in md.parts else md.with_suffix("").name
    emit("env", str(rel))

def _row(status, label):
    tag = ("[ok]PASS[/]" if status == "pass" else "[fail]FAIL[/]") if RICH else \
          (_c("ok", "PASS") if status == "pass" else _c("fail", "FAIL"))
    pr(f"  {tag}  {label}")

def _summary(p, f, sk):
    passed = f"[ok]{p} passed[/]" if RICH else _c("ok", f"{p} passed")
    failed = (f"[fail]{f} failed[/]" if f else "[dim]0 failed[/]") if RICH else \
             (_c("fail", f"{f} failed") if f else _c("dim", "0 failed"))
    head = "[env]tests[/]" if RICH else _c("env", "tests")
    ln = f"{head}  {passed} · {failed}"
    if sk:
        ln += f" · [dim]{sk} skipped[/]" if RICH else " · " + _c("dim", f"{sk} skipped")
    pr(ln)
