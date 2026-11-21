---
name: feedback-user-runs-tests
description: "don't run tests, record or ucg suites in ura-lang; give the user the exact commands to run"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T10:58:21.303Z
---

Don't run `tests`, `record`, `ucg` or other long test runs in ura-lang. After a change, give the user the exact commands (e.g. `tests milestone12`, `record milestone12 12`) and let them run and report back. A single quick build to check compile errors is fine.

**Why:** the user said (2026-11-19) I take too much time; test runs block the session for minutes.
**How to apply:** finish the edit, then list which suites/tests to run and what result to expect. Supersedes the "run tests after every step" part of [[feedback-tests]].

No probe runs either (2026-11-19, later): my check_md.sh / probe loops cost too many tokens; only build, then hand the user the commands and read ONE log they produce. Batch them (2026-11-19): the user got annoyed by a command after every message. Work through a whole suite (or a big chunk) on my own, checking with quick compiler probes and the scratchpad check_ir.sh, and give ONE block of record + test commands at the end of that batch, not per fix.
