"""The task verbs. One function per shell command, each kept thin.

The work lives in the layers below:
    suite    read/render/write a group .md
    runner   execute cases in parallel
    sandbox  compile+run one case
"""
from collections import defaultdict
from pathlib import Path

from config import suite
from config.runner import run_all, mismatch, is_runnable
from config.sandbox import actual, decolor
from config.suite import case_id, fence
from config.ui import ok, err, warn, _group_header, _row, _summary

def tests(target=None):
    """run .md tests — no arg = all of tests/ · a dir = its .md files · a .md file = just that one."""
    mds = suite.group_files(target)
    if not mds:
        err(f"no such test path: {target}"); return

    jobs, todo, rows = [], {}, defaultdict(list)
    skipped = 0
    for md in mds:
        cases    = suite.read(md)
        runnable = [c for c in cases if is_runnable(c)]
        skipped += len(cases) - len(runnable)
        if runnable:
            todo[md] = len(runnable)
            jobs    += [(md, c) for c in runnable]

    passed = failed = 0
    owner  = {id(c): md for md, c in jobs}

    def landed(case, got):
        """Print a group the moment its last case finishes, so output streams."""
        nonlocal passed, failed
        md = owner[id(case)]
        rows[md].append((case, mismatch(case, got)))
        todo[md] -= 1
        if todo[md]:
            return
        _group_header(md)
        for c, bad in sorted(rows[md], key=lambda r: r[0]['name']):
            if bad:
                failed += 1; _row("fail", f"{c['name']} ({bad})")
            else:
                passed += 1; _row("pass", c['name'])

    run_all([c for _, c in jobs], on_done=landed)
    _summary(passed, failed, skipped)

def update(target="tests"):
    """Regenerate tree/out/err/ll goldens in place for every case in the target .md file(s)."""
    mds  = suite.group_files(target)
    jobs = [(md, c) for md in mds for c in suite.read(md)]
    done = run_all([c for _, c in jobs])

    fresh = defaultdict(list)
    for (md, _), (case, got) in zip(jobs, done):
        fresh[md].append({**case, **got})
    for md in mds:
        suite.write(md, fresh[md])
        ok(f"updated {md} ({len(fresh[md])} cases)")

def migrate(dirpath):
    """ADD newly-implemented cases from <dir> into tests/<group>.md, PRESERVING already-migrated
    ones (success = compiles+runs; error = its recorded .err still reproduces). Run `prune` after."""
    d   = Path(dirpath)
    rel = Path(*d.parts[1:])                        # strip leading tests-old/
    out = (suite.TESTS / rel).with_suffix(".md")
    kept = {case_id(c): c for c in (suite.read(out) if out.exists() else [])}

    pending = [u for u in sorted(d.glob("*.ura")) if u.stem not in kept]
    sources = [{'name': u.stem, 'ura': u.read_text(), 'file': u} for u in pending]
    added   = 0
    for case, got in run_all(sources):
        u, src = case['file'], case['ura']
        old = u.with_suffix(".err")
        if old.exists():
            if got["err"].strip() != decolor(old.read_text()).strip():
                continue                            # error path not implemented yet
        elif not got["ll"]:
            continue                                # doesn't compile yet
        name = f"{u.stem} — {suite.describe(src)}".rstrip(" —")
        kept[u.stem] = {'name': name, 'ura': src, **got}
        added += 1

    if not kept:
        return 0
    suite.write(out, [kept[k] for k in sorted(kept)])
    if added: ok(f"{out}: +{added} case(s) → {len(kept)} total")
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
    for md in suite.group_files():
        olddir = Path(src) / Path(*md.parts[1:]).with_suffix("")
        for c in suite.read(md):
            stem = case_id(c)
            for f in list(olddir.glob(f"{stem}.*")) + list(olddir.glob(f"build/{stem}.ll")):
                if f.is_file(): f.unlink(); removed += 1
    for d in sorted((p for p in Path(src).rglob("*") if p.is_dir()),
                    key=lambda p: len(p.parts), reverse=True):
        try:
            if not any(d.iterdir()): d.rmdir()
        except OSError: pass
    ok(f"pruned {removed} migrated files from {src}")

def show(spec):
    """show <group>:<NNN>  — print one case (jump straight to it, no scrolling the .md)."""
    group, _, which = spec.partition(":")
    for c in suite.read(suite.TESTS / f"{group}.md"):
        if not which or case_id(c) == which or which in c['name']:
            print(f"## {c['name']}\n")
            for tag in ('ura', 'out', 'err', 'll'):
                if tag in c: print(fence(tag, c[tag]) + "\n")

def index(group):
    """index <group>  — list the case names in a group .md."""
    for c in suite.read(suite.TESTS / f"{group}.md"):
        print(c['name'])

def doctor():
    """doctor  — report duplicate case ids, which silently break `migrate`."""
    bad = 0
    for md in suite.group_files():
        for dup in suite.duplicate_ids(suite.read(md)):
            warn(f"{md}: duplicate case id '{dup}'"); bad += 1
    ok("no duplicate case ids") if not bad else err(f"{bad} duplicate id(s)")
