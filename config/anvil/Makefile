CXX      ?= clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Isrc

SRCS     := src/anvil.cpp src/commands.cpp

UNAME_S  := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    OS := linux
else ifeq ($(UNAME_S),Darwin)
    OS := mac
else ifneq (,$(findstring MINGW,$(UNAME_S)))
    OS := windows
else ifneq (,$(findstring MSYS,$(UNAME_S)))
    OS := windows
else
    OS := unknown
endif

OUT_DIR  := build/$(OS)
BIN      := $(OUT_DIR)/anvil
PREFIX   ?= /usr/local

.PHONY: all clean re install uninstall

all: $(BIN)

$(BIN): $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

clean:
	rm -rf build

re: clean all

install: $(BIN)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin/anvil
	@echo "installed $(DESTDIR)$(PREFIX)/bin/anvil ($(OS))"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/anvil
