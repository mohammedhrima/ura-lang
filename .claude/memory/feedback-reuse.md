---
name: feedback-reuse
description: "what is needed already exists in ura-lang; reuse it, never reinvent"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-17T02:38:05.693Z
---

Most of what is needed already exists. Reuse it, don't reinvent.

**Why:** user rejected new parallel mechanisms (e.g. a separate ENDLOOP when END could serve if and loop).
**How to apply:** before proposing code, search src/ir.c for a helper that fits (`ucg_branch`, `ucg_then`, `ucg_end`, `ucg_op`, `append`, `format`, ...) and extend it. See [[feedback-code-style]].
