"""Compile and run ONE case in a throwaway directory.

Everything that spawns a subprocess lives here. Nothing else in config/
shells out to the compiler.
"""
import os, re, tempfile
from pathlib import Path

from config.env import URA, IGNORE, run, exit_code

# A case that loops forever must not hang the whole suite. Set well above any
# real case (slowest measured: 3.6s under 16-way load) so a loaded machine
# never trips them — these catch hangs, they are not a performance budget.
COMPILE_TIMEOUT = 120
RUN_TIMEOUT     = 60

FILES  = re.compile(r"(?m)^//[ \t]*----[ \t]*(\S+)[^\n]*")
STATUS = ("Compiling", "Finished", "Running", "Exited", "Crashed")

def decolor(s):
    return re.sub(r'\x1b\[[0-9;]*m', '', s)

def _diagnostics(text):
    """Compile-time stderr minus the progress lines — what's left is warnings,
    so a successful build can still record them in the err golden."""
    kept = [l for l in decolor(text).splitlines() if not l.startswith(STATUS)]
    return "\n".join(kept).strip()

def split_files(src):
    """A '// ---- name.ura' marker splits one ura block into several files, so a
    case can exercise `use`. The LAST section is the entry point; it is written
    as <stem>.ura like a single-file case, so diagnostics stay stable."""
    parts = FILES.split(src)
    if len(parts) == 1:
        return [], src
    names, bodies = parts[1::2], parts[2::2]
    extra = [(n if n.endswith(".ura") else n + ".ura", b)
             for n, b in zip(names, bodies)]
    return extra[:-1], extra[-1][1]

def _write_sources(d, src, stem):
    """Lay the case out on disk; return the entry file."""
    extra, entry = split_files(src)
    for name, body in extra:
        fp = Path(d) / name
        fp.parent.mkdir(parents=True, exist_ok=True)
        fp.write_text(body.strip() + "\n")
    p = Path(d) / f"{stem}.ura"
    p.write_text(entry.rstrip() + "\n")
    return p

def _normalizer(d, entry, stem):
    """The temp path is baked into @trap_msg and every diagnostic. Rewrite it to
    <stem>.ura so goldens are deterministic AND portable across machines."""
    paths = (str(entry), os.path.realpath(entry))
    roots = (os.path.realpath(d) + os.sep, str(d) + os.sep)
    def norm(s):
        for pp in paths: s = s.replace(pp, f"{stem}.ura")
        for rr in roots: s = s.replace(rr, "")
        return s
    return norm

def _read_ll(entry, stem, norm):
    ll = entry.parent / "build" / f"{stem}.ll"
    if not ll.exists():
        return ""
    keep = [l for l in ll.read_text().splitlines()
            if not any(ig in l for ig in IGNORE)]
    return norm("\n".join(keep))

def actual(src, stem):
    """Compile+run one source under -testing → {ll, out, err, tree}, "" when N/A.

    A case that fails to compile records its diagnostic in err — an "error test"
    is just a case whose err block is non-empty, not a separate kind of test.
    """
    res = {"ll": "", "out": "", "err": "", "tree": ""}
    with tempfile.TemporaryDirectory() as d:
        entry = _write_sources(d, src, stem)
        norm  = _normalizer(d, entry, stem)
        exe   = Path(d) / "exe"

        c = run(URA, entry, "-o", exe, "-testing", "-tree",
                timeout=COMPILE_TIMEOUT)
        if c.returncode != 0:                       # compile failed
            # NOT decolored: "Too many errors, stopping" (diagnostics.c) is
            # printed with raw RED() and ignores -testing, so its escapes are
            # already in the goldens. Decoloring here would rewrite them.
            res["err"] = norm(c.stderr)
            return res

        res["tree"] = norm(c.stdout)                # -tree prints the AST to stdout
        res["ll"]   = _read_ll(entry, stem, norm)

        r = run(exe, timeout=RUN_TIMEOUT)
        res["out"] = r.stdout
        if r.stderr or exit_code(r):
            res["err"] = norm(r.stderr) + f"exit: {exit_code(r)}\n"

        warn = _diagnostics(norm(c.stderr))
        if warn:
            res["err"] = warn + "\n" + res["err"]
        return res
