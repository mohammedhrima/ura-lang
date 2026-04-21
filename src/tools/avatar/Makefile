CXX      ?= clang++
CC       ?= clang

CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Isrc -Ivendor
CFLAGS   := -O2 -Wall -Ivendor

CPP_SRCS := $(wildcard src/*.cpp) $(wildcard src/commands/*.cpp)
C_SRCS   := vendor/toml.c

BUILD    := build
CPP_OBJS := $(patsubst %.cpp,$(BUILD)/%.o,$(CPP_SRCS))
C_OBJS   := $(patsubst %.c,$(BUILD)/%.o,$(C_SRCS))
OBJS     := $(CPP_OBJS) $(C_OBJS)

BIN      := avatar
PREFIX   ?= /usr/local

.PHONY: all clean install uninstall

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) $(BIN)

install: $(BIN)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin/$(BIN)
	@echo "installed $(DESTDIR)$(PREFIX)/bin/$(BIN)"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
