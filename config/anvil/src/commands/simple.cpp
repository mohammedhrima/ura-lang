// Collection of short, shell-wrapper commands: reload, leaks, indent, examples.

#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../fs.h"
#include "../paths.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

// ---- reload ---------------------------------------------------------------
int cmd_reload(const std::vector<std::string>&) {
    invalidate_config();
    (void)config();   // reload now so errors surface immediately
    printf(ANSI_GREEN "Config reloaded" ANSI_RESET "\n");
    return 0;
}

// ---- leaks ----------------------------------------------------------------
// Matches config.sh check_leaks(): runs <exe> with LSan enabled and a
// suppression file at config/lsan.supp.
int cmd_leaks(const std::vector<std::string>& args) {
    if (args.empty()) {
        fprintf(stderr, ANSI_RED "usage: leaks <executable> [args...]" ANSI_RESET "\n");
        return 1;
    }
    std::string supp = project_root() + "/config/lsan.supp";
    std::string cmd  = "ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=" + supp;
    for (const auto& a : args) { cmd += " '"; cmd += a; cmd += "'"; }
    return run_sh(cmd);
}

// ---- indent ---------------------------------------------------------------
// Shells out to clang-format + merge_cases.py, matching config.sh indent().
int cmd_indent(const std::vector<std::string>&) {
    if (!has_tool("clang-format")) {
        fprintf(stderr, ANSI_RED "indent: clang-format not found" ANSI_RESET "\n");
        return 1;
    }
    printf(ANSI_YELLOW "Formatting..." ANSI_RESET "\n");
    std::string src = resolve(config().source);
    // Collect .c and .h files
    std::vector<std::string> files;
    walk(src, [&](const std::string& p) {
        if ((p.size() >= 2 && p.substr(p.size() - 2) == ".c") ||
            (p.size() >= 2 && p.substr(p.size() - 2) == ".h")) files.push_back(p);
    });
    if (files.empty()) { printf("no .c/.h files under %s\n", src.c_str()); return 0; }

    std::vector<std::string> argv = {"clang-format", "-i"};
    argv.insert(argv.end(), files.begin(), files.end());
    int rc = run_stream(argv);
    if (rc != 0) return rc;

    std::string mc = project_root() + "/config/merge_cases.py";
    if (!is_file(mc)) return 0;             // optional, silently skip
    argv = {"python3", mc};
    argv.insert(argv.end(), files.begin(), files.end());
    return run_stream(argv);
}

// ---- examples -------------------------------------------------------------
// Concatenate all test .ura files into <root>/examples.ura with headers,
// mirroring config.sh examples().
int cmd_examples(const std::vector<std::string>&) {
    std::string tests = resolve(config().tests);
    std::string out   = project_root() + "/examples.ura";
    FILE* f = fopen(out.c_str(), "w");
    if (!f) { fprintf(stderr, ANSI_RED "examples: cannot open %s" ANSI_RESET "\n", out.c_str()); return 1; }

    fputs("// ============================================================================\n"
          "// URA LANGUAGE EXAMPLES\n"
          "// ============================================================================\n"
          "// Aggregated from the test suite.\n"
          "// ============================================================================\n\n", f);

    std::vector<std::string> files;
    walk(tests, [&](const std::string& p) {
        if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") files.push_back(p);
    });
    std::sort(files.begin(), files.end());

    int count = 0;
    for (const auto& p : files) {
        std::string body = read_file(p);
        // "category" = dirname relative to tests root
        std::string rel = p;
        if (rel.rfind(tests + "/", 0) == 0) rel = rel.substr(tests.size() + 1);
        auto slash = rel.find_last_of('/');
        std::string category = (slash == std::string::npos) ? "" : rel.substr(0, slash);
        std::string name     = (slash == std::string::npos) ? rel : rel.substr(slash + 1);
        if (name.size() >= 4 && name.substr(name.size() - 4) == ".ura")
            name = name.substr(0, name.size() - 4);

        std::string uc = category;
        for (char& c : uc) c = (char)std::toupper((unsigned char)c);

        fprintf(f, "\n// ============================================================================\n"
                   "// %s: %s\n"
                   "// File: %s\n"
                   "// ============================================================================\n\n",
                   uc.c_str(), name.c_str(), p.c_str());
        fputs(body.c_str(), f);
        fputc('\n', f);
        count++;
    }
    fclose(f);
    printf(ANSI_GREEN "Generated examples.ura with %d examples" ANSI_RESET "\n  -> %s\n", count, out.c_str());
    return 0;
}
