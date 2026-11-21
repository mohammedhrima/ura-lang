---
name: feedback-commits
description: "ura-lang commit messages are short lowercase descriptions, never any Anthropic attribution"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-17T02:38:11.046Z
---

Commit messages: short, descriptive, lowercase, like the user's own (`implement while loop`, `implment fn_call`, `fix compilations bugs and fn params access`). Never an Anthropic email, Co-Authored-By or "Generated with" line.

**Why:** user's explicit rule; also in ~/.claude/CLAUDE.md.
**How to apply:** check `git log --format=%s` style before committing; message ends with the description, nothing after.
