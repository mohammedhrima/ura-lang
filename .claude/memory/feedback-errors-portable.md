---
name: feedback-errors-portable
description: "every add_error call belongs in check.c; code must build with LLVM 12/15/21 and on Linux, macOS, Windows with clang"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T03:13:00.428Z
---

1. Errors live in check.c: new error reporting goes into a check_* function there; existing add_error calls left in main.c (parser) should move into check.c over time.
2. The compiler must build with every supported LLVM (12, 15, 21) and with clang on Linux, macOS and Windows: no GNU-only or POSIX-only calls in new code without a portable path, no LLVM API missing from 12 or removed in 21.

**Why:** the user said so (2026-11-19), keeping analyze/parse short and the compiler portable.
**How to apply:** when adding an error, write or extend a check_* in check.c; when using an LLVM-C or libc function, make sure it exists in all three LLVM versions and on all three systems. See [[feedback-code-style]].
