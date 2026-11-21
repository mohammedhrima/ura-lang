---
name: feedback-tests
description: every ura-lang feature gets a milestone .md test entry; ucg features also get a run_test case in ucg.c
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T02:51:23.480Z
---

Every feature gets a test: an entry in tests/milestoneN.md (`// stdout:` / `// expect:` lines; error tests use `// error:`), and new ucg features a `case N:` in ucg.c's run_test. The user records and runs them ([[feedback-user-runs-tests]]): give exact commands, e.g. `record milestone1 060` and `{ tests milestone1; } 2>&1 | sed 's/\x1b\[[0-9;]*m//g' > build-tests.log`, then read build-tests.log.

**Why:** coverage of everything built; tests have found real bugs.
**How to apply:** after a feature, add the entry with the Edit tool; check the program's output yourself with a quick compile+run before asking the user to record.
