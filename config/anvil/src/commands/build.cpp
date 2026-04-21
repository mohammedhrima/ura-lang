#include "../command.h"
#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../paths.h"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

static void mkdirs(const std::string& p) {
    // One level of mkdir is enough — build/ sits at repo root.
    mkdir(p.c_str(), 0755);
}

// Returns e.g. "llvm-config-14" if llvm_version > 0, else "llvm-config".
static std::string llvm_config_bin(int version) {
    if (version <= 0) return "llvm-config";
    return "llvm-config-" + std::to_string(version);
}

// Run `llvm-config --flags...` and return the token list.
// Dies on failure so we don't silently build without LLVM flags.
static std::vector<std::string> llvm_flags(const std::string& bin,
                                           const std::vector<std::string>& args) {
    std::vector<std::string> argv = {bin};
    argv.insert(argv.end(), args.begin(), args.end());
    CaptureResult r = run_capture(argv);
    if (r.code != 0) {
        fprintf(stderr, ANSI_RED "%s failed (code %d):" ANSI_RESET "\n%s",
                bin.c_str(), r.code, r.stderr_.c_str());
        return {};
    }
    return split_ws(r.stdout_);
}

// Strip ld's "ld: warning:" lines from linker stderr — config.sh did this.
static std::string filter_ld_warnings(const std::string& s) {
    std::string out;
    std::size_t i = 0;
    while (i < s.size()) {
        std::size_t nl = s.find('\n', i);
        std::size_t end = (nl == std::string::npos) ? s.size() : nl + 1;
        std::string line = s.substr(i, end - i);
        if (line.rfind("ld: warning:", 0) != 0) out += line;
        i = end;
    }
    return out;
}

int cmd_build(const std::vector<std::string>& args) {
    bool want_san     = false;
    bool want_release = false;
    for (const auto& a : args) {
        if      (a == "--san"     || a == "-san")     want_san     = true;
        else if (a == "--release" || a == "-release") want_release = true;
        else {
            fprintf(stderr, ANSI_RED "build: unknown flag '%s'" ANSI_RESET "\n", a.c_str());
            return 1;
        }
    }

    const AnvilConfig& cfg = config();
    std::string lcbin  = llvm_config_bin(cfg.llvm);
    auto llvm_cflags   = llvm_flags(lcbin, {"--cflags"});
    auto llvm_ldflags  = llvm_flags(lcbin, {"--ldflags", "--libs", "core"});
    if (llvm_cflags.empty() || llvm_ldflags.empty()) {
        fprintf(stderr, ANSI_RED "build: could not query %s" ANSI_RESET "\n", lcbin.c_str());
        return 1;
    }

    // Assemble: clang sources warn san/release llvm-cflags llvm-ldflags -o output
    std::vector<std::string> argv = {"clang"};
    for (const auto& s : cfg.compile) argv.push_back(resolve(cfg.source + "/" + s));
    argv.insert(argv.end(), cfg.warn.begin(), cfg.warn.end());
    if      (want_release) argv.insert(argv.end(), cfg.release.begin(), cfg.release.end());
    else if (want_san)     argv.insert(argv.end(), cfg.san.begin(),     cfg.san.end());
    argv.insert(argv.end(), llvm_cflags.begin(),  llvm_cflags.end());
    argv.insert(argv.end(), llvm_ldflags.begin(), llvm_ldflags.end());

    std::string out = resolve(cfg.output);
    mkdirs(resolve(cfg.build));
    argv.push_back("-o");
    argv.push_back(out);

    CaptureResult r = run_capture(argv);
    std::string err = filter_ld_warnings(r.stderr_);
    if (!err.empty()) fputs(err.c_str(), stderr);

    if (r.code != 0) {
        fprintf(stderr, ANSI_RED "Build failed" ANSI_RESET " (clang exit %d)\n", r.code);
        return r.code;
    }
    printf(ANSI_GREEN "Built compiler" ANSI_RESET "  -> %s\n", out.c_str());
    return 0;
}
