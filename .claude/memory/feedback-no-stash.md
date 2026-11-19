---
name: feedback-no-stash
description: "never run git stash in ura-lang; the user edits files (TODO.md, projects/) while Claude works"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-18T23:03:43.377Z
---

Never use `git stash` in ura-lang to compare before/after. The user edits files (TODO.md, projects/raytracer, src/test/) while Claude works, so a stash pop can fail or mix changes.

**Why:** 2026-11-18 a stash pop failed on TODO.md mid-run, and once a stash was run by mistake; both needed manual recovery.
**How to apply:** for a baseline, use `git worktree add --detach <scratchpad>/base HEAD` (init the ura-lib submodule there or point URA_LIB at projects/ura-lib), or compare against recorded test output. See [[feedback-step-by-step]].
