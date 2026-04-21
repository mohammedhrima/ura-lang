// build, run, test, check, clean, fmt — everything that operates on a ura
// project via the `ura` compiler on PATH.

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

namespace {

std::string ura_bin() {
    if (const char* p = std::getenv("URA_BIN")) return p;
    return "ura";
}

// Collect -L paths for any cloned deps under target/deps/<name>/src.
// (Phase 9 wiring: the `ura` compiler accepts -L<path> to extend its module search.)
std::vector<std::string> dep_search_paths() {
    std::vector<std::string> out;
    for (const auto& d : ura_config().deps) {
        std::string dep_src = resolve("target/deps/" + d.name + "/src");
        if (is_dir(dep_src)) out.push_back(dep_src);
    }
    return out;
}

// Invoke the ura compiler, producing a linked binary at cfg.output.
// The compiler links but does not execute the result, so `run` has to do that
// step itself.
int compile_to_binary() {
    const auto& cfg = ura_config();
    std::vector<std::string> argv = {ura_bin(), resolve(cfg.src)};

    if (!cfg.optimize.empty()) argv.push_back("-" + cfg.optimize);
    for (const auto& p : dep_search_paths()) {
        argv.push_back("-L");
        argv.push_back(p);
    }
    argv.push_back("-no-debug");
    argv.push_back("-o");
    argv.push_back(resolve(cfg.output));

    mkdir_p(resolve("target"));
    return run_stream(argv);
}

} // namespace

// ---- build ----------------------------------------------------------------
int cmd_build(const std::vector<std::string>&) {
    int rc = compile_to_binary();
    if (rc != 0) return rc;
    printf(ANSI_GREEN "Built" ANSI_RESET " -> %s\n", resolve(ura_config().output).c_str());
    return 0;
}

// ---- run ------------------------------------------------------------------
int cmd_run(const std::vector<std::string>& args) {
    int rc = compile_to_binary();
    if (rc != 0) return rc;
    std::vector<std::string> argv = {resolve(ura_config().output)};
    argv.insert(argv.end(), args.begin(), args.end());
    return run_stream(argv);
}

// ---- check ----------------------------------------------------------------
int cmd_check(const std::vector<std::string>&) {
    const auto& cfg = ura_config();
    std::vector<std::string> argv = {ura_bin(), resolve(cfg.src)};
    for (const auto& p : dep_search_paths()) { argv.push_back("-L"); argv.push_back(p); }
    argv.push_back("-no-exec");
    argv.push_back("-no-debug");
    return run_stream(argv);
}

// ---- test -----------------------------------------------------------------
int cmd_test(const std::vector<std::string>&) {
    std::string tests = resolve("tests");
    if (!is_dir(tests)) {
        fprintf(stderr, ANSI_RED "test: no tests/ directory" ANSI_RESET "\n");
        return 1;
    }
    std::vector<std::string> files;
    walk(tests, [&](const std::string& p) {
        if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") files.push_back(p);
    });
    std::sort(files.begin(), files.end());
    if (files.empty()) { printf("no tests\n"); return 0; }

    int p = 0, f = 0;
    auto dep_paths = dep_search_paths();
    for (const auto& t : files) {
        std::vector<std::string> argv = {ura_bin(), t};
        for (const auto& sp : dep_paths) { argv.push_back("-L"); argv.push_back(sp); }
        argv.push_back("-no-exec");
        argv.push_back("-no-debug");
        CaptureResult r = run_capture(argv);
        std::string rel = t;
        std::string prefix = tests + "/";
        if (rel.rfind(prefix, 0) == 0) rel = rel.substr(prefix.size());
        if (rel.size() >= 4 && rel.substr(rel.size() - 4) == ".ura") rel = rel.substr(0, rel.size() - 4);
        if (r.code == 0) { printf("  " ANSI_GREEN "PASS %s" ANSI_RESET "\n", rel.c_str()); p++; }
        else             { printf("  " ANSI_RED   "FAIL %s" ANSI_RESET "\n", rel.c_str()); f++; }
    }
    printf("\n" ANSI_GREEN "Passed: %d" ANSI_RESET "\n", p);
    if (f) printf(ANSI_RED "Failed: %d" ANSI_RESET "\n", f);
    return f;
}

// ---- clean ----------------------------------------------------------------
int cmd_clean(const std::vector<std::string>&) {
    std::string tgt = resolve("target");
    if (!is_dir(tgt)) { printf(ANSI_DIM "nothing to clean" ANSI_RESET "\n"); return 0; }
    int rc = run_sh("rm -rf '" + tgt + "'");
    if (rc == 0) printf(ANSI_GREEN "Cleaned target/" ANSI_RESET "\n");
    return rc;
}

// ---- fmt ------------------------------------------------------------------
int cmd_fmt(const std::vector<std::string>&) {
    printf(ANSI_YELLOW "avatar fmt: ura formatter not yet available" ANSI_RESET "\n");
    return 0;
}
