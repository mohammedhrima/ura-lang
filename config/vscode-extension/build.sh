#!/bin/bash

# Variables
EXT_NAME="ura-lang"
VERSION="0.0.1"
VSIX_FILE="${EXT_NAME}-${VERSION}.vsix"
EXTENSIONS_DIR="$HOME/.vscode/extensions"
TARGET_DIR="$EXTENSIONS_DIR/mohamedhrima.${EXT_NAME}-${VERSION}"

echo "=== Building Ura VSCode Extension ==="

# Step 1: Check dependencies
echo "Step 1: Checking dependencies..."
if ! command -v vsce &> /dev/null; then
    echo "Installing vsce globally..."
    npm install -g @vscode/vsce
else
    echo "✓ vsce is installed"
fi

# Step 2: Install npm dependencies and compile TypeScript
echo "Step 2: Installing dependencies and compiling TypeScript..."
npm install
npx tsc --noEmit false
if [ $? -ne 0 ]; then
    echo "❌ TypeScript compilation failed!"
    exit 1
fi
echo "✓ TypeScript compiled to out/"

# Step 3: Clean old builds
echo "Step 3: Cleaning old builds..."
rm -f *.vsix

# Step 4: Package the extension
echo "Step 4: Packaging extension..."
vsce package

if [ ! -f "$VSIX_FILE" ]; then
    echo "❌ Error: VSIX file not created!"
    exit 1
fi
echo "✓ VSIX created: $VSIX_FILE"

# Step 5: Install extension (using VSCode CLI for a clean install)
echo "Step 5: Installing extension..."
code --install-extension "$VSIX_FILE" --force
echo "✓ Extension installed"

# Step 7: Verify installation
echo ""
echo "=== Installation Complete ==="
echo ""
echo "Next steps:"
echo "1. Restart VSCode completely (Cmd+Q then reopen)"
echo "2. Open a .ura file — syntax highlighting, go-to-definition, autocomplete and formatting should all work"