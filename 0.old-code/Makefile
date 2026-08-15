LLVM      ?= llvm-config-14
CC        ?= clang
LLVMFLAGS := $(shell $(LLVM) --cflags --ldflags --libs core)
SRC       := src/main.c
OUT       := build/ura

.PHONY: build release dev

build:
	@mkdir -p build
	$(CC) $(SRC) $(LLVMFLAGS) -o $(OUT)

release:
	@mkdir -p build
	$(CC) -O2 $(SRC) $(LLVMFLAGS) -o $(OUT)

dev:
	uv run config/tasks.py