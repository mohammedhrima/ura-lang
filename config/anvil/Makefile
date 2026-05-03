CXX      ?= clang++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Isrc

SRCS     := src/anvil.cpp src/commands.cpp

BIN      := anvil
PREFIX   ?= /usr/local

.PHONY: all clean install uninstall

all: $(BIN)

$(BIN): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

clean:
	rm -f $(BIN)

install: $(BIN)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin/$(BIN)
	@echo "installed $(DESTDIR)$(PREFIX)/bin/$(BIN)"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
