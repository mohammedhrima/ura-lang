# =========================================================
#  Ura Language — Windows Bootstrap
#  Requires: Windows 10/11, PowerShell 5+, winget
#  Run as Administrator: powershell -ExecutionPolicy Bypass -File setup.ps1
# =========================================================

$ErrorActionPreference = "Stop"

$MSYS2_DIR  = "C:\msys64"
$MSYS2_EXE  = "$MSYS2_DIR\msys2_shell.cmd"
$BASH_EXE   = "$MSYS2_DIR\usr\bin\bash.exe"

# ANSI colors (Windows Terminal / PowerShell 7+)
$GREEN  = "`e[32m"
$RED    = "`e[31m"
$YELLOW = "`e[33m"
$CYAN   = "`e[36m"
$BOLD   = "`e[1m"
$DIM    = "`e[2m"
$RESET  = "`e[0m"

function Write-Ok   { param($msg) Write-Host "  ${GREEN}v${RESET}  $msg" }
function Write-Fail { param($msg) Write-Host "  ${RED}x${RESET}  $msg" }
function Write-Info { param($msg) Write-Host "  ${DIM}$msg${RESET}" }

Write-Host ""
Write-Host "  ${BOLD}+------------------------------+${RESET}"
Write-Host "  ${BOLD}|     Ura Setup (Windows)      |${RESET}"
Write-Host "  ${BOLD}+------------------------------+${RESET}"
Write-Host ""

# =========================================================
#  Step 1 — Check winget
# =========================================================
Write-Host "  ${BOLD}Step 1:${RESET} Checking winget..."
if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
    Write-Fail "winget not found."
    Write-Host ""
    Write-Info "winget comes with Windows 10 1709+ via the App Installer."
    Write-Info "Install it from the Microsoft Store: search 'App Installer'"
    Write-Info "Or download: https://aka.ms/getwinget"
    exit 1
}
Write-Ok "winget found"
Write-Host ""

# =========================================================
#  Step 2 — Install MSYS2 if missing
# =========================================================
Write-Host "  ${BOLD}Step 2:${RESET} Checking MSYS2..."

if (Test-Path $BASH_EXE) {
    Write-Ok "MSYS2 already installed at $MSYS2_DIR"
} else {
    Write-Host "  ${YELLOW}!${RESET}  MSYS2 not found — installing via winget..."
    Write-Host ""
    winget install --id MSYS2.MSYS2 --silent --accept-package-agreements --accept-source-agreements

    if (-not (Test-Path $BASH_EXE)) {
        Write-Fail "MSYS2 installation failed or installed to unexpected path."
        Write-Host ""
        Write-Info "Install manually from: https://www.msys2.org"
        Write-Info "Then re-run this script."
        exit 1
    }
    Write-Ok "MSYS2 installed"
}
Write-Host ""

# =========================================================
#  Step 3 — Update MSYS2 package database
# =========================================================
Write-Host "  ${BOLD}Step 3:${RESET} Updating MSYS2 packages..."
Write-Host ""

# First update (may close and need re-run — common MSYS2 behavior)
& $BASH_EXE -lc "pacman -Syuu --noconfirm" 2>&1 | ForEach-Object { Write-Host "  $_" }

Write-Host ""
Write-Ok "Package database updated"
Write-Host ""

# =========================================================
#  Step 4 — Run setup.sh inside MSYS2
# =========================================================
Write-Host "  ${BOLD}Step 4:${RESET} Running setup.sh inside MSYS2..."
Write-Host ""

# Convert Windows path to MSYS2 path (e.g. C:\Users\x\repo -> /c/Users/x/repo)
$RepoPath = (Get-Location).Path -replace '\\', '/' -replace '^([A-Z]):', { '/' + $args[0].ToLower() }

& $BASH_EXE -lc "cd '$RepoPath' && bash setup.sh" 2>&1 | ForEach-Object { Write-Host "  $_" }

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Fail "setup.sh reported errors. Check the output above."
    exit 1
}

# =========================================================
#  Done
# =========================================================
Write-Host ""
Write-Host "  ${GREEN}${BOLD}All done!${RESET}"
Write-Host ""
Write-Host "  Open the ${CYAN}MSYS2 MinGW x64${RESET} terminal, navigate to this repo, then:"
Write-Host ""
Write-Host "    ${CYAN}source config.sh${RESET}"
Write-Host "    ${CYAN}build${RESET}"
Write-Host ""
