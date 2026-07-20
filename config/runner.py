"""Run many cases in parallel.

Every command that needs the compiler goes through here, so concurrency is
tuned in exactly one place.

WORKERS: measured, not guessed. On macOS the kernel serialises the first
exec of every freshly-written binary, which caps the suite at ~7.6 cases/s
no matter how many threads we use:

    workers   1    4    8   16   32
    cases/s  2.6  7.8  7.9  7.8  7.8

So the pool exists to overlap the *compile* half (which does scale, ~6x);
past ~8 threads nothing improves. 16 leaves headroom without spawning
dozens of idle threads.
"""
from concurrent.futures import ThreadPoolExecutor

from config.sandbox import actual
from config.suite import FENCES, case_id

WORKERS = 16

def _one(case):
    return case, actual(case['ura'], case_id(case))

def run_all(cases, on_done=None):
    """[case] -> [(case, got)] in input order.

    on_done(case, got) fires as each case lands, so callers can stream
    progress instead of waiting for the whole batch.
    """
    if not cases:
        return []
    results = {}
    with ThreadPoolExecutor(max_workers=WORKERS) as ex:
        for case, got in ex.map(_one, cases):
            results[id(case)] = got
            if on_done:
                on_done(case, got)
    return [(c, results[id(c)]) for c in cases]

def mismatch(case, got):
    """First fence whose actual output differs from the golden, else None."""
    for tag in FENCES:
        if got[tag].strip() != case.get(tag, "").strip():
            return tag
    return None

def is_runnable(case):
    """A case with no recorded output has nothing to compare against yet."""
    return any(case.get(t, "").strip() for t in FENCES)
