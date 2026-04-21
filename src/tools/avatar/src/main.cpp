#include "color.h"
#include "command.h"

#include <cstdio>
#include <string>
#include <vector>

static int print_usage(int rc) {
    printf("\n  " ANSI_BOLD "avatar" ANSI_RESET "  ura-lang package manager\n\n");
    printf("  " ANSI_DIM "usage: avatar <command> [args]" ANSI_RESET "\n\n");
    for (const auto& c : all_commands()) {
        printf("  " ANSI_CYAN "%-8s" ANSI_RESET "  %s\n", c.name, c.summary);
    }
    printf("\n");
    return rc;
}

int cmd_help(const std::vector<std::string>&) { return print_usage(0); }

int main(int argc, char** argv) {
    if (argc < 2) return print_usage(1);
    std::string name = argv[1];
    const Command* c = find_command(name);
    if (!c) {
        fprintf(stderr, ANSI_RED "unknown command: %s" ANSI_RESET "\n", name.c_str());
        return print_usage(1);
    }
    std::vector<std::string> args;
    args.reserve(argc - 2);
    for (int i = 2; i < argc; i++) args.emplace_back(argv[i]);
    return c->fn(args);
}
