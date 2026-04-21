#include "command.h"

int cmd_new(const std::vector<std::string>&);
int cmd_build(const std::vector<std::string>&);
int cmd_run(const std::vector<std::string>&);
int cmd_test(const std::vector<std::string>&);
int cmd_check(const std::vector<std::string>&);
int cmd_clean(const std::vector<std::string>&);
int cmd_add(const std::vector<std::string>&);
int cmd_update(const std::vector<std::string>&);
int cmd_fmt(const std::vector<std::string>&);
int cmd_help(const std::vector<std::string>&);

static const std::vector<Command> g_commands = {
    {"new",    cmd_new,    "Scaffold a new ura project: avatar new <name>"},
    {"build",  cmd_build,  "Compile the project (resolves deps first)"},
    {"run",    cmd_run,    "Build and execute the project"},
    {"test",   cmd_test,   "Compile every .ura file under tests/"},
    {"check",  cmd_check,  "Compile without linking (syntax / type check)"},
    {"add",    cmd_add,    "Add a dependency: avatar add <name> <git-url>"},
    {"update", cmd_update, "Clone / update all dependencies into target/deps/"},
    {"clean",  cmd_clean,  "Remove target/"},
    {"fmt",    cmd_fmt,    "Format .ura source (stub for now)"},
    {"help",   cmd_help,   "List available commands"},
};

const std::vector<Command>& all_commands() { return g_commands; }
const Command* find_command(const std::string& name) {
    for (const auto& c : g_commands) if (name == c.name) return &c;
    return nullptr;
}
