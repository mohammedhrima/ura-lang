"""A group file: tests/<group>.md holding many cases.

Read it, render it, write it. No subprocesses, no parallelism — pure text,
so it is cheap to call and easy to test by eye.

A case is a plain dict:  {'name': '001 — does a thing', 'ura': ..., 'tree': ...}
"""
import re
from pathlib import Path

TESTS = Path("tests")

# order the fences are written in, after the ura block. Also the order
# _compare walks them, so a case reports the earliest meaningful mismatch.
FENCES = ("tree", "out", "err", "ll")

CASE  = re.compile(r'(?m)^## ')
BLOCK = re.compile(r'(?ms)^```([a-z]+)\n(.*?)^```$')

def fence(tag, body):
    body = body.rstrip("\n")
    return f"```{tag}\n{body}\n```" if body else f"```{tag}\n```"

def case_id(case):
    return case['name'].split()[0]

def describe(src):
    """'// loops/003.ura - skip walls' -> 'skip walls'."""
    first = (src.splitlines() or [""])[0]
    if not first.startswith("//"): return ""
    t = first.lstrip("/").strip()
    for sep in (" - ", " — "):
        if sep in t: return t.split(sep, 1)[1].strip()
    return ""

def read(path):
    """One group .md -> [case]. Skips the ## index section."""
    cases = []
    for part in CASE.split(Path(path).read_text())[1:]:
        head, _, body = part.partition('\n')
        if head.strip().lower() == 'index':
            continue
        blocks = dict(BLOCK.findall(body))
        if 'ura' in blocks:
            cases.append({'name': head.strip(), **blocks})
    return cases

def title_of(path):
    """tests/projects/ura-libft.md -> 'projects / ura-libft'."""
    rel = Path(*Path(path).parts[1:]).with_suffix("")
    return str(rel).replace('/', ' / ')

def render(path, cases):
    """[case] -> the full .md text. The ONLY place this document is built."""
    index = "\n".join(f"- {c['name']}" for c in cases)
    body  = "\n\n".join(
        f"## {c['name']}\n\n" + "\n\n".join(
            [fence("ura", c['ura'])] + [fence(t, c.get(t, "")) for t in FENCES])
        for c in cases)
    return f"# {title_of(path)}\n\n## index\n\n{index}\n\n{body}\n"

def write(path, cases):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(render(path, cases))

def group_files(target=None):
    """Resolve a target to the .md files it names. None -> all of tests/."""
    if not target:
        return sorted(TESTS.rglob("*.md"))
    for cand in (Path(target), Path(target).with_suffix(".md"),
                 TESTS / target, (TESTS / target).with_suffix(".md")):
        if cand.exists():
            return [cand] if cand.suffix == ".md" else sorted(cand.rglob("*.md"))
    return []

def duplicate_ids(cases):
    """Two cases sharing an id silently break `migrate`, which keys by id."""
    seen, dupes = set(), []
    for c in cases:
        i = case_id(c)
        if i in seen: dupes.append(i)
        seen.add(i)
    return dupes
