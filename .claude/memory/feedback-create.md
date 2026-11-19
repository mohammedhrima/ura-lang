---
name: feedback-create
description: "every ura constructor is `pub fn create(...)`, called Type::create — never make/from/new"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-18T23:48:09.646Z
---

The one standard constructor name in ura, ura-lib and every future library is `create`: `String::create("abc")`, `Error::create("boom")`, `Range::create(0, 10)`. Never `make`, `from` or `new`.

**Why:** the user wants one syntax everywhere (said 2026-11-18 about `Error::make` / `String::from`).
**How to apply:** name new constructors `create`; the literal-to-struct conversion rule looks for `pub fn create(text i8[])`. Rename the old ones in ura-lib, tests and projects/raytracer. See [[project-roadmap]].
