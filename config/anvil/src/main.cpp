#include "color.h"
#include "command.h"
#include "paths.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static int print_usage(int rc) {
    printf("\n  " ANSI_BOLD "anvil" ANSI_RESET "  ura-lang dev tool\n\n");
    printf("  " ANSI_DIM "usage: anvil [-f <anvil.toml>] <command> [args]" ANSI_RESET "\n\n");
    for (const auto& c : all_commands()) {
        printf("  " ANSI_CYAN "%-18s" ANSI_RESET "  %s\n", c.name, c.summary);
    }
    printf("\n");
    return rc;
}

int cmd_help(const std::vector<std::string>&) { return print_usage(0); }

int main(int argc, char** argv) {
    // Strip `-f <path>` wherever it appears; everything else is the command.
    std::vector<std::string> cleaned;
    cleaned.reserve(argc);
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, ANSI_RED "-f requires a path" ANSI_RESET "\n");
                return 1;
            }
            set_anvil_toml(argv[++i]);
        } else {
            cleaned.emplace_back(argv[i]);
        }
    }
    if (cleaned.empty()) return print_usage(1);

    const Command* c = find_command(cleaned[0]);
    if (!c) {
        fprintf(stderr, ANSI_RED "unknown command: %s" ANSI_RESET "\n", cleaned[0].c_str());
        return print_usage(1);
    }
    std::vector<std::string> args(cleaned.begin() + 1, cleaned.end());
    return c->fn(args);
}
