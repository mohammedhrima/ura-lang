---
name: feedback-no-python-edits
description: "edit ura-lang files with the Edit tool only, never with python/sed scripts"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T01:59:41.275Z
---

Make code and test changes with the Edit tool, one visible edit at a time. Never use python or sed scripts to rewrite source files, tests or ura-lib.

**Why:** the user said (2026-11-19) script edits cause bugs (one truncated package.c earlier; mass sed/regex edits are hard to review).
**How to apply:** even for repeated changes, do several Edit calls; reading files with grep/awk is fine. See [[feedback-step-by-step]].
