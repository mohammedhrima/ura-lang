#pragma once

#include <functional>
#include <string>
#include <vector>

// ============================================================================
// ANSI colors
// ============================================================================
#define ANSI_RESET   "\033[0m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_DIM     "\033[2m"
#define ANSI_RED     "\033[0;31m"
#define ANSI_GREEN   "\033[0;32m"
#define ANSI_YELLOW  "\033[1;33m"
#define ANSI_CYAN    "\033[0;36m"

// ============================================================================
// Misc
// ============================================================================

// "macos" / "linux-apt" / "linux-pacman" / "linux-dnf" / "linux" / "unknown"
const std::string& detect_os();

// e.g. "2026-04-21"
std::string today_string();

// ============================================================================
// Filesystem
// ============================================================================

void        walk(const std::string& root, const std::function<void(const std::string&)>& cb);
std::string read_file(const std::string& path);
bool        is_file(const std::string& path);
std::string dirname_of(const std::string& path);
std::string stem(const std::string& path);

// ============================================================================
// Project paths (anvil.toml resolution)
// ============================================================================

const std::string& project_root();
const std::string& anvil_toml_path();
void               set_anvil_toml(const std::string& path);
std::string        resolve(const std::string& rel);

// ============================================================================
// Process exec
// ============================================================================

struct CaptureResult {
    int         code = 0;
    std::string stdout_;
    std::string stderr_;
};

int                      run_stream(const std::vector<std::string>& argv);
CaptureResult            run_capture(const std::vector<std::string>& argv);
int                      run_sh(const std::string& cmd);
std::vector<std::string> split_ws(const std::string& s);
bool                     has_tool(const std::string& tool);

// ============================================================================
// Config
// ============================================================================

struct AnvilConfig {
    // paths (all relative to anvil.toml's directory)
    std::string source;
    std::string build;
    std::string output;
    std::string ura_lib;
    std::string tests;

    // compiler
    int                      llvm = 14;
    std::vector<std::string> compile;

    // flags
    std::vector<std::string> warn;
    std::vector<std::string> san;
    std::vector<std::string> release;

    // tests
    int                      max_parallel = 3;
    std::vector<std::string> ignore_ir;

    // sync
    std::string anvil_repo;
    std::string ura_lib_repo;
    std::string extension_repo;
    std::string projects_dir;
    std::string project_url;
};

const AnvilConfig& config();
void               invalidate_config();

// ============================================================================
// Command dispatch
// ============================================================================

struct Command {
    const char* name;
    int       (*fn)(const std::vector<std::string>& args);
    const char* summary;
};

const Command*              find_command(const std::string& name);
const std::vector<Command>& all_commands();
