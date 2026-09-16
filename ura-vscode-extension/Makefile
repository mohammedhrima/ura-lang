NODE      ?= node
NAME      := $(shell $(NODE) -p "require('./package.json').name")
PUBLISHER := $(shell $(NODE) -p "require('./package.json').publisher")
VERSION   := $(shell $(NODE) -p "require('./package.json').version")
VSIX      := $(NAME)-$(VERSION).vsix
SRC       := $(wildcard src/*.ts src/*/*.ts)
ASSETS    := $(wildcard syntaxes/*.json) $(wildcard icons/*) \
             language-configuration.json README.md LICENSE

.PHONY: all build test package install uninstall clean fclean re

all: build

build: out/extension.js

node_modules: package.json
	npm install
	@touch node_modules

out/extension.js: $(SRC) tsconfig.json node_modules
	npx tsc -p .

test: build
	node --test out/test/*.test.js

$(VSIX): out/extension.js package.json $(ASSETS)
	@rm -f *.vsix
	npx vsce package

package: $(VSIX)

install: $(VSIX)
	code --install-extension $(VSIX) --force

uninstall:
	code --uninstall-extension $(PUBLISHER).$(NAME)

clean:
	rm -rf out *.vsix

fclean: clean
	rm -rf node_modules

re: fclean all
