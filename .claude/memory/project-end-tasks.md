---
name: project-end-tasks
description: "done only when the user says the work is finished: merge refactor -> dev -> main, rewrite ura-lang's git history"
metadata:
  node_type: memory
  type: project
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T02:51:22.406Z
---

At the very end (user's call, not before):
- merge `refactor` into `dev`, then `dev` into `main` (only after refactor has every feature main/dev had);
- rewrite ura-lang's git history (too heavy, holds files that no longer exist);
- push ura-lib's new commits (main was fast-forwarded to new-syntax on 2026-11-18; later core.ura commits are local).

**Why:** the user wants one clean public result, not intermediate pushes.
**How to apply:** never push, merge to main or rewrite history without an explicit go; confirm in full sentences right before. See [[project-roadmap]].
