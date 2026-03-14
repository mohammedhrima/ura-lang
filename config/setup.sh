#!/usr/bin/env bash
set -euo pipefail

# =========================================================
#  Ura Language — Fresh Machine Setup
#  Supports: macOS, Ubuntu/Debian, Arch, Fedora, MSYS2
# =========================================================

BOLD="\033[1m"
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
CYAN="\033[0;36m"
DIM="\033[2m"
RESET="\033[0m"

# =========================================================
#  OS Detection
# =========================================================
case "$(uname -s 2>/dev/null)" in
    Darwin*)              OS_TYPE="macos"   ;;
    Linux*)               OS_TYPE="linux"   ;;
    MINGW*|MSYS*|CYGWIN*) OS_TYPE="windows" ;;
    *)                    OS_TYPE="unknown" ;;
esac

# =========================================================
#  Helpers
# =========================================================
ok()   { echo -e "  ${GREEN}✓${RESET}  $*"; }
fail() { echo -e "  ${RED}✗${RESET}  $*"; }
info() { echo -e "  ${DIM}$*${RESET}"; }
warn() { echo -e "  ${YELLOW}!${RESET}  $*"; }

have() { command -v "$1" &>/dev/null; }

# =========================================================
#  Header
# =========================================================
echo ""
echo -e "  ${BOLD}╔══════════════════════════════╗${RESET}"
echo -e "  ${BOLD}║        Ura Setup             ║${RESET}"
echo -e "  ${BOLD}╚══════════════════════════════╝${RESET}"
echo ""

# =========================================================
#  Show detected platform + package manager
# =========================================================
PKG_MGR=""
PKG_MGR_VERSION=""

case "$OS_TYPE" in
    macos)
        PLATFORM_LABEL="macOS"
        if have brew; then
            PKG_MGR="brew"
            PKG_MGR_VERSION="$(brew --version 2>/dev/null | head -1)"
        fi
        ;;
    linux)
        if have apt-get; then
            PLATFORM_LABEL="Linux (apt)"
            PKG_MGR="apt"
        elif have pacman; then
            PLATFORM_LABEL="Linux (pacman)"
            PKG_MGR="pacman"
        elif have dnf; then
            PLATFORM_LABEL="Linux (dnf)"
            PKG_MGR="dnf"
        else
            PLATFORM_LABEL="Linux (unknown)"
        fi
        ;;
    windows)
        PLATFORM_LABEL="Windows (MSYS2)"
        PKG_MGR="msys2-pacman"
        ;;
    *)
        PLATFORM_LABEL="Unknown"
        ;;
esac

if [[ -n "$PKG_MGR_VERSION" ]]; then
    echo -e "  Detected: ${CYAN}${PLATFORM_LABEL}${RESET}  ${DIM}·  ${PKG_MGR_VERSION}${RESET}"
else
    echo -e "  Detected: ${CYAN}${PLATFORM_LABEL}${RESET}"
fi
echo ""

# =========================================================
#  macOS: ensure Homebrew is installed
# =========================================================
if [[ "$OS_TYPE" == "macos" ]] && ! have brew; then
    echo -e "  ${YELLOW}Homebrew not found — installing...${RESET}"
    echo ""
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    # Add brew to PATH for Apple Silicon
    if [[ -f "/opt/homebrew/bin/brew" ]]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi

    if have brew; then
        ok "Homebrew installed"
    else
        fail "Homebrew installation failed. Please install manually: https://brew.sh"
        exit 1
    fi
    echo ""
fi

# =========================================================
#  Install Dependencies
# =========================================================
echo -e "  ${BOLD}Installing dependencies...${RESET}"
echo -e "  ${DIM}────────────────────────────────────${RESET}"
echo ""

_install_all() {
    case "$OS_TYPE" in
        macos)
            brew install llvm@14 uncrustify
            # Ensure llvm@14 binaries are on PATH (keg-only)
            local llvm_prefix
            llvm_prefix="$(brew --prefix llvm@14 2>/dev/null)"
            if [[ -d "$llvm_prefix/bin" ]]; then
                export PATH="$llvm_prefix/bin:$PATH"
                echo ""
                warn "llvm@14 is keg-only. Add this to your shell profile to make it permanent:"
                echo ""
                info "  echo 'export PATH=\"${llvm_prefix}/bin:\$PATH\"' >> ~/.zshrc"
                echo ""
            fi
            ;;
        linux)
            if [[ "$PKG_MGR" == "apt" ]]; then
                sudo apt-get update -qq
                sudo apt-get install -y llvm-14 clang-14 llvm-14-tools uncrustify
                # Point all unversioned commands to the -14 versions
                for tool in clang clang++ llc llvm-config; do
                    local t14="${tool}-14"
                    if have "$t14"; then
                        sudo ln -sf "$(command -v "$t14")" "/usr/local/bin/$tool"
                        ok "linked $tool -> $t14"
                    fi
                done
            elif [[ "$PKG_MGR" == "pacman" ]]; then
                sudo pacman -S --noconfirm llvm clang uncrustify
            elif [[ "$PKG_MGR" == "dnf" ]]; then
                sudo dnf install -y llvm14 clang uncrustify
            else
                fail "No supported package manager found (apt, pacman, dnf)."
                echo ""
                info "Install manually: llvm-14  clang-14  uncrustify"
                exit 1
            fi
            ;;
        windows)
            pacman -S --noconfirm \
                mingw-w64-x86_64-llvm \
                mingw-w64-x86_64-clang \
                mingw-w64-x86_64-uncrustify
            # MSYS2 installs llvm-config (unversioned) — create llvm-config-14 symlink
            for tool in llvm-config llc clang; do
                local versioned="${tool}-14"
                if have "$tool" && ! have "$versioned"; then
                    local tool_dir
                    tool_dir="$(dirname "$(command -v "$tool")")"
                    ln -sf "$(command -v "$tool")" "$tool_dir/$versioned"
                    ok "linked $versioned -> $tool"
                fi
            done
            ;;
        *)
            fail "Unknown platform. Cannot install dependencies automatically."
            echo ""
            info "Install manually: llvm-14  clang-14  llc  uncrustify"
            exit 1
            ;;
    esac
}

_install_all

# =========================================================
#  Verify All Tools
# =========================================================
echo ""
echo -e "  ${BOLD}Verifying...${RESET}"
echo -e "  ${DIM}────────────────────────────────────${RESET}"
echo ""

ERRORS=0

_check_tool() {
    local cmd="$1"
    local label="$2"
    if have "$cmd"; then
        local ver
        ver=$("$cmd" --version 2>&1 | head -1)
        ok "$(printf '%-20s' "$label")${DIM}$ver${RESET}"
    else
        fail "$(printf '%-20s' "$label")${RED}not found${RESET}"
        ((ERRORS++))
    fi
}

_check_tool clang           "clang"
_check_tool llvm-config-14  "llvm-config-14"
_check_tool llc             "llc"
_check_tool uncrustify      "uncrustify"

echo ""

# =========================================================
#  Result
# =========================================================
if [[ $ERRORS -eq 0 ]]; then
    echo -e "  ${GREEN}${BOLD}Setup complete!${RESET}"
    echo ""
    echo -e "  Next step:  ${CYAN}source config.sh${RESET}"
    echo -e "  Then:       ${CYAN}build${RESET}"
    echo ""
else
    echo -e "  ${RED}${BOLD}$ERRORS tool(s) still missing.${RESET}"
    echo ""
    echo -e "  Try running ${CYAN}source config.sh${RESET} then ${CYAN}install${RESET} to retry."
    echo ""
    exit 1
fi
