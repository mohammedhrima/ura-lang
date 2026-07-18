import re
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

from config.goldens import _actual, parse_md, _fence, _desc, _decolor
from config.ui import ok, err, _group_header, _row, _summary

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
