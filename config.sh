#!/usr/bin/env bash

# If script is executed (not sourced), re-execute with proper shell
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "Loading ura environment..."
    
    # Detect user's default shell
    if command -v dscl &> /dev/null; then
        # macOS
        user_shell=$(dscl . -read ~/ UserShell 2>/dev/null | sed 's/UserShell: //')
    elif command -v getent &> /dev/null; then
        # Linux
        user_shell=$(getent passwd "$USER" 2>/dev/null | cut -d: -f7)
    else
        # Fallback
        user_shell="$SHELL"
    fi
    
    # Get absolute path to this script
    script_path="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
    
    if [[ "$user_shell" == *"zsh"* ]]; then
        # Create temp zsh config
        temp_rc=$(mktemp)
        cat > "$temp_rc" << RCEOF
# Source default zshrc
if [ -f ~/.zshrc ]; then
    source ~/.zshrc
fi
# Source ura config
source '$script_path'
RCEOF
        exec env ZDOTDIR="$(dirname "$temp_rc")" zsh -c "ln -sf '$temp_rc' \"\$ZDOTDIR/.zshrc\" 2>/dev/null; exec zsh"
    else
        # Create temp bash config
        temp_rc=$(mktemp)
        cat > "$temp_rc" << RCEOF
# Source default bashrc
if [ -f ~/.bashrc ]; then
    source ~/.bashrc
fi
# Source ura config
source '$script_path'
RCEOF
        exec bash --rcfile "$temp_rc"
    fi
fi

# === Color Definitions ===
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m'

# === Paths & Global Variables ===
ura_dir="$HOME/Desktop/Personal/ura-lang"
ura_src="$ura_dir/src"
files=($ura_src/ura_main.c $ura_src/ura_parse.c $ura_src/ura_gen.c $ura_src/ura_utils.c $ura_src/ura_llvm.c)

# === Compiler Flags ===
san_flags=(-fsanitize=address -fsanitize=null)
debug_flags=(-g3)
warn_flags=(-Wall -Wextra -Werror=incompatible-pointer-types -Werror=int-conversion)
llvm_flags=($(llvm-config --cflags --ldflags --libs core))

flags=("${san_flags[@]}" "${debug_flags[@]}" "${warn_flags[@]}" "${llvm_flags[@]}")

# === Export Variables for Shell Access ===
export flags files llvm_flags src

# === Build Functions ===
build() {
    local debug_mode="${1:-true}"
    clang "${files[@]}" "${flags[@]}" -D DEBUG=$debug_mode -o "$ura_src/ura" || {
        echo -e "${RED}Error:${NC} Build failed."
        return 1
    }
}

ir() {
    "$ura_src/ura" "$ura_src/file.ura" || {
        echo -e "${RED}Error:${NC} Compiling .ura file failed."
        return 1
    }
}

asm() {
    llc "$ura_src/file.ll" -o "$ura_src/file.s" || {
        echo -e "${RED}Error:${NC} Assembly generation failed."
        return 1
    }
}

comp_asm() {
    clang "$ura_src/file.s" -o "$ura_src/exe.out"
}

exe() {
    "$ura_src/exe.out"
}

comp() {
    build true && ir && asm && comp_asm
}

run() {
    rm -rf $ura_src/file.ll $ura_src/file.s && build true && ir && asm && comp_asm && exe
}

lines() {
    find $ura_src -type f \( -name "*.c" -o -name "*.h" \) -exec wc -l {} +
}

indent() {
    echo -e "${YELLOW}Formatting code...${NC}"
    astyle --mode=c --indent=spaces=3 --pad-oper --pad-header \
       --keep-one-line-statements --keep-one-line-blocks --convert-tabs \
       --max-code-length=100 --break-after-logical \
       --suffix=none $ura_src/*.c $ura_src/*.h
}

copy() {
    if [ -z "$1" ]; then
        echo -e "${RED}Error:${NC} No directory name provided."
        echo "Usage: copy <dir_name> [file_name]"
        return 1
    fi

    local dir_name="$1"
    local test_dir="$ura_dir/tests/$dir_name"
    mkdir -p "$test_dir"

    local filename
    if [ -z "$2" ]; then
        local count=$(ls "$test_dir"/*.ura 2>/dev/null | wc -l)
        local next=$(printf "%03d" $((count + 1)))
        filename="$next"
    else
        filename="$2"
    fi

    local ura_dest="$test_dir/${filename}.ura"
    local ll_dest="$test_dir/${filename}.ll"

    build false || return 1
    ir || return 1

    cp "$ura_src/file.ura" "$ura_dest" || {
        echo -e "${RED}Error:${NC} Failed to copy file.ura"
        return 1
    }
    cp "$ura_src/file.ll" "$ll_dest" || {
        echo -e "${RED}Error:${NC} Failed to copy file.ll"
        return 1
    }

    echo -e "${GREEN}Saved:${NC} $ura_dest and $ll_dest"
}

test() {
    build false || return 1

    for ura_file in "$ura_dir/tests"/*/*.ura; do
        [ -e "$ura_file" ] || continue

        local base_name=$(basename "$ura_file" .ura)
        local dir_name=$(basename "$(dirname "$ura_file")")
        local ll_file="$(dirname "$ura_file")/${base_name}.ll"

        cp "$ura_file" "$ura_src/tmp.ura"

        if ! "$ura_src/ura" "$ura_src/tmp.ura" > /dev/null 2>&1; then
            echo -e "${RED}FAILED:${NC} $dir_name/$base_name.ura (compilation error)"
            continue
        fi

        if [[ ! -f "$ura_src/tmp.ll" ]]; then
            echo -e "${RED}FAILED:${NC} $dir_name/$base_name (tmp.ll does not exist)"
        elif [[ ! -f "$ll_file" ]]; then
            echo -e "${RED}FAILED:${NC} $dir_name/$base_name ($(basename "$ll_file") does not exist)"
        elif diff -q <(tail -n +3 "$ura_src/tmp.ll") <(tail -n +3 "$ll_file") > /dev/null 2>&1; then
            echo -e "${GREEN}PASSED:${NC} $dir_name/$base_name"
        else
            echo -e "${RED}FAILED:${NC} $dir_name/$base_name"
        fi
    done
    rm -rf "$ura_src/tmp.ura" "$ura_src/tmp.ll"
}

update() {
    echo -e "${GREEN}updating ura config...${NC}"
    source "$ura_src/../config.sh"
}

# === Prompt Handling ===
set_prompt() {
    if [ -n "$ZSH_VERSION" ]; then
        PROMPT="(ura) %n@%1~ % "
    elif [ -n "$BASH_VERSION" ]; then
        PS1="(ura) \u@\W \$ "
    fi
}

set_prompt
export tests=test
cd "$ura_src" 2>/dev/null || cd "$ura_dir"

echo -e "${GREEN}Ura environment loaded!${NC}"
echo -e "Available commands: build, ir, asm, comp_asm, exe, comp, run, lines, indent, copy, test, update"