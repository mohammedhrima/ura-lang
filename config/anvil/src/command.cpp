#include "command.h"

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

static const std::vector<Command> g_commands = {
    {"build",             cmd_build,             "Compile the ura compiler (flags: --san, --release)"},
    {"test",              cmd_test,              "Run the test suite (optionally filter by folder)"},
    {"copy",              cmd_copy,              "Save a .ura file and its IR as a test reference"},
    {"update-tests",      cmd_update_tests,      "Regenerate every .ll reference file"},
    {"update-projects",   cmd_update_projects,   "Fix first-line path comment in every project .ura"},
    {"examples",          cmd_examples,          "Generate examples.ura from the test corpus"},
    {"indent",            cmd_indent,            "Format all src/**/*.c and *.h files"},
    {"check",             cmd_check,             "Verify build dependencies (clang, llvm, etc.)"},
    {"install",           cmd_install,           "Install missing build dependencies"},
    {"release",           cmd_release,           "Full release pipeline (use --confirm to push)"},
    {"release-anvil",     cmd_release_anvil,     "Sync config/anvil/ to the anvil repo (use --confirm)"},
    {"release-projects",  cmd_release_projects,  "Sync tests/projects/* repos (use --confirm)"},
    {"release-extension", cmd_release_extension, "Sync the VSCode extension repo (use --confirm)"},
    {"shell",             cmd_shell,             "Launch an interactive Linux shell in Docker"},
    {"docker-build",      cmd_docker_build,      "Rebuild the Linux dev-env Docker image"},
    {"leaks",             cmd_leaks,             "Run an executable with LSan enabled"},
    {"reload",            cmd_reload,            "Re-read anvil.toml"},
    {"help",              cmd_help,              "List available commands"},
};

const std::vector<Command>& all_commands() { return g_commands; }
const Command* find_command(const std::string& name) {
    for (const auto& c : g_commands) if (name == c.name) return &c;
    return nullptr;
}
