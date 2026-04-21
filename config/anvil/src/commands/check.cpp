#include "../command.h"
#include "../color.h"
#include "../config.h"
#include "../exec.h"

#include <cstdio>
#include <string>
#include <vector>

namespace {

struct Check {
    const char* name;
    const char* probe;      // `<name> --version` style; first line is printed
    bool        required;
};

void print_tool(const Check& c, int& missing) {
    CaptureResult r = run_capture({"/bin/sh", "-c", std::string(c.probe) + " 2>&1 | head -1"});
    if (r.code == 0 && !r.stdout_.empty()) {
        // Strip trailing newline
        std::string ver = r.stdout_;
        while (!ver.empty() && (ver.back() == '\n' || ver.back() == '\r')) ver.pop_back();
        printf("  " ANSI_GREEN "✓" ANSI_RESET "  %-18s " ANSI_DIM "%s" ANSI_RESET "\n",
               c.name, ver.c_str());
    } else if (c.required) {
        printf("  " ANSI_RED "✗" ANSI_RESET "  %-18s " ANSI_RED "not found" ANSI_RESET "\n", c.name);
        missing++;
    } else {
        printf("  " ANSI_YELLOW "–" ANSI_RESET "  %-18s " ANSI_DIM "not found (optional)" ANSI_RESET "\n", c.name);
    }
}

} // namespace

int cmd_check(const std::vector<std::string>&) {
    const AnvilConfig& cfg = config();
    std::string llvm_cfg = "llvm-config-" + std::to_string(cfg.llvm);
    std::string llvm_ver = llvm_cfg + " --version";

    printf("\n  " ANSI_BOLD "Dependency Check" ANSI_RESET "\n");
    printf("  " ANSI_DIM "────────────────────────────────────" ANSI_RESET "\n");

    int missing = 0;
    Check checks[] = {
        {"clang",          "clang --version",                   true},
        {llvm_cfg.c_str(), llvm_ver.c_str(),                    true},
        {"llc",            "llc --version | grep 'version'",    true},
        {"clang-format",   "clang-format --version",            false},
    };
    for (auto& c : checks) print_tool(c, missing);
    printf("\n");

    if (missing == 0) {
        printf("  " ANSI_GREEN ANSI_BOLD "All good!" ANSI_RESET "\n\n");
    } else {
        printf("  " ANSI_RED ANSI_BOLD "%d missing." ANSI_RESET "  Run: " ANSI_CYAN "install" ANSI_RESET "\n\n", missing);
    }
    return missing == 0 ? 0 : 1;
}
