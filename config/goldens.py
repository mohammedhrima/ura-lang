import os, tempfile, re
from pathlib import Path

from config.env import URA, IGNORE, run, exit_code

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

def _actual(src, stem):
    """Compile+run one source under -testing → always {ll, out, err} (empty when N/A).
    A case that fails to compile records its diagnostic in err — an "error test" is just
    a case whose err block is non-empty, not a separate kind of test.
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
            res["err"] = norm(c.stderr)
            return res
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