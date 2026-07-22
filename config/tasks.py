# /// script
# requires-python = ">=3.10"
# dependencies = ["rich>=13", "prompt_toolkit>=3"]
# ///
"""The ura task shell — build/test tooling for ura-lang (replaces anvil).

Run it with uv (this loads rich + prompt_toolkit automatically):

    make dev                      # enter the interactive ura shell
    uv run config/tasks.py build  # run one task
    uv run config/tasks.py tests  # run all .md tests (or: tests match · tests errors/lexer.md)

Compiling the compiler itself needs none of this: `make build` / `make release`.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))  # repo root, so config.* resolves

from config.env import activate_env
from config.ui import RICH, _c
from config.shell import dispatch, ura_shell, plain_loop

if __name__ == "__main__":
    activate_env()
    if not RICH:
        print(_c("dim", "tip: launch with `make dev` for colors + the interactive shell"), file=sys.stderr)
    if len(sys.argv) > 1:
        dispatch(*sys.argv[1:])
    elif sys.stdin.isatty():
        ura_shell()
    else:
        plain_loop()