---
name: feedback-ucg-in-codegen
description: "ura may call ucg_* functions only from the codegen step, never from tokenize/parse/analyze"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-18T23:29:23.496Z
---

ura code calls ucg_* functions only during codegen. Parse and analyze must not touch ucg (e.g. no ucg_throws() inside the parser).

**Why:** the user keeps ucg separate from the ura front end, and plans to ship ucg as its own library. See [[project-roadmap]].
**How to apply:** if codegen needs a fact found while parsing (like "the program throws"), keep it on the ura side (ask before adding a field) and pass it to ucg when codegen starts.
