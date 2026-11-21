---
name: feedback-ask-before-c
description: "ask the user before editing any C code (src/*.c, src/*.h) in ura-lang"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T06:45:59.402Z
---

Don't edit C code (src/main.c, check.c, ucg.c, package.c, headers) until the user says yes. Test files (.md) and ura-lib .ura files can be edited while working through a batch.

**Why:** the user said so (2026-11-19) after I added too much compiler code on my own (the include machinery that had to be reverted).
**How to apply:** when a test needs a compiler change, finish the test edits, then propose the C change in a few lines (what, where, why) and wait for a yes. See [[feedback-step-by-step]], [[feedback-reuse]].
