NODE      ?= node
NAME      := $(shell $(NODE) -p "require('./package.json').name")
PUBLISHER := $(shell $(NODE) -p "require('./package.json').publisher")
VERSION   := $(shell $(NODE) -p "require('./package.json').version")
VSIX      := $(NAME)-$(VERSION).vsix
SRC       := $(shell find client/src server/src test -name '*.ts') \
             tsconfig.json tsconfig.base.json client/tsconfig.json server/tsconfig.json test/tsconfig.json
ASSETS    := $(wildcard syntaxes/*.json) $(wildcard snippets/*.json) $(wildcard icons/*) \
             language-configuration.json README.md LICENSE
# E2E runs in this VS Code (the Electron binary behind `code`); empty: download one
VSCODE_EXECUTABLE ?= $(shell p=$$(command -v code) && d=$$(dirname "$$(readlink -f "$$p")")/.. && \
                       [ -x "$$d/code" ] && readlink -f "$$d/code")

.PHONY: all build lint test test-e2e package install uninstall clean fclean re

all: build

node_modules: package.json
	npm install
	@touch node_modules

build: out/client/src/extension.js

out/client/src/extension.js: $(SRC) node_modules
	npx tsc -b

lint: node_modules
	npx eslint .

test: build
	npm test

test-e2e: build
	VSCODE_EXECUTABLE="$(VSCODE_EXECUTABLE)" npm run test:e2e

$(VSIX): build package.json $(ASSETS)
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
	rm -rf node_modules .vscode-test

re: fclean all
