#include "../command.h"
#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../util.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

int cmd_install(const std::vector<std::string>&) {
    const std::string& os = detect_os();
    int version = config().llvm;
    std::string vstr = std::to_string(version);

    printf(ANSI_YELLOW "Installing dependencies for %s..." ANSI_RESET "\n\n", os.c_str());

    if (os == "macos") {
        if (!has_tool("brew")) {
            fprintf(stderr, ANSI_RED "Homebrew not found." ANSI_RESET
                           " Install it first:\n"
                           "  /bin/bash -c \"$(curl -fsSL "
                           "https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"\n");
            return 1;
        }
        int rc = run_sh("brew install llvm@" + vstr + " clang-format");
        if (rc != 0) return rc;

        CaptureResult prefix = run_capture({"brew", "--prefix", "llvm@" + vstr});
        if (prefix.code == 0 && !prefix.stdout_.empty()) {
            std::string p = prefix.stdout_;
            while (!p.empty() && (p.back() == '\n' || p.back() == '\r')) p.pop_back();
            printf(ANSI_GREEN "\nllvm@%s at %s" ANSI_RESET "\n", vstr.c_str(), p.c_str());
            printf("Add this to your shell rc:\n  export PATH=\"%s/bin:$PATH\"\n", p.c_str());
        }
        return 0;
    }

    if (os == "linux-apt") {
        std::string cmd = "sudo apt-get update && sudo apt-get install -y llvm-" + vstr
                        + " clang-" + vstr + " llvm-" + vstr + "-tools clang-format";
        int rc = run_sh(cmd);
        if (rc != 0) return rc;
        // Link unversioned tools → -N variants so the compiler build sees them.
        for (const char* t : {"clang", "clang++", "llc", "llvm-config"}) {
            std::string v = std::string(t) + "-" + vstr;
            if (has_tool(v)) {
                run_sh("sudo ln -sf \"$(command -v " + v + ")\" /usr/local/bin/" + t);
                printf("  " ANSI_DIM "linked %s -> %s" ANSI_RESET "\n", t, v.c_str());
            }
        }
        return 0;
    }
    if (os == "linux-pacman") {
        return run_sh("sudo pacman -S --noconfirm llvm clang clang-format");
    }
    if (os == "linux-dnf") {
        return run_sh("sudo dnf install -y llvm" + vstr + " clang clang-format");
    }
    fprintf(stderr, ANSI_RED "install: unknown platform %s" ANSI_RESET
                    ". Install llvm-%s, clang, and clang-format manually.\n",
                    os.c_str(), vstr.c_str());
    return 1;
}
