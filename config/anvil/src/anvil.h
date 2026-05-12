#pragma once

#include <functional>
#include <string>
#include <vector>

#define ANSI_RESET   "\033[0m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_DIM     "\033[2m"
#define ANSI_RED     "\033[0;31m"
#define ANSI_GREEN   "\033[0;32m"
#define ANSI_YELLOW  "\033[1;33m"
#define ANSI_CYAN    "\033[0;36m"

const std::string& detect_os();

std::string today_string();

void        walk(const std::string& root, const std::function<void(const std::string&)>& cb);
std::string read_file(const std::string& path);
bool        is_file(const std::string& path);
std::string dirname_of(const std::string& path);
std::string stem(const std::string& path);

const std::string& project_root();
const std::string& anvil_toml_path();
void               set_anvil_toml(const std::string& path);
std::string        resolve(const std::string& rel);

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

struct AnvilConfig {
    
    std::string source;
    std::string build;
    std::string output;
    std::string ura_lib;
    std::string tests;
    std::string errors;     // tests under this subtree are expected to fail compilation

    
    int                      llvm = 14;
    std::vector<std::string> compile;


    std::vector<std::string> flags;
    // Host-OS-specific extras appended to `flags` at load time, so things like
    // `-fsanitize=address` can be Linux-only (macOS 14+ has a known ASan-init
    // deadlock that hangs every binary before main runs).
    std::vector<std::string> flags_extra;
    std::vector<std::string> release;

    
    int                      max_parallel = 3;
    std::vector<std::string> ignore_ir;
    std::vector<std::string> skip;

    
    std::string anvil_repo;
    std::string ura_lib_repo;
    std::string extension_repo;
    std::string projects_dir;
    std::string project_url;
};

const AnvilConfig& config();
void               invalidate_config();

struct Command {
    const char* name;
    int       (*fn)(const std::vector<std::string>& args);
    const char* summary;
};

const Command*              find_command(const std::string& name);
const std::vector<Command>& all_commands();

int cmd_help(const std::vector<std::string>& args);
int cmd_build(const std::vector<std::string>& args);
int cmd_test(const std::vector<std::string>& args);
int cmd_reload(const std::vector<std::string>& args);
int cmd_leaks(const std::vector<std::string>& args);
int cmd_indent(const std::vector<std::string>& args);
int cmd_examples(const std::vector<std::string>& args);
int cmd_check(const std::vector<std::string>& args);
int cmd_install(const std::vector<std::string>& args);
int cmd_copy(const std::vector<std::string>& args);
int cmd_update_tests(const std::vector<std::string>& args);
int cmd_update_projects(const std::vector<std::string>& args);
int cmd_release(const std::vector<std::string>& args);
int cmd_release_projects(const std::vector<std::string>& args);
int cmd_release_extension(const std::vector<std::string>& args);
int cmd_release_anvil(const std::vector<std::string>& args);
int cmd_shell(const std::vector<std::string>& args);
int cmd_docker_build(const std::vector<std::string>& args);
