

#include "anvil.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <set>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

static void mkdirs(const std::string& p) {
    
    mkdir(p.c_str(), 0755);
}

static std::string llvm_config_bin(int version) {
    if (version <= 0) return "llvm-config";
    return "llvm-config-" + std::to_string(version);
}

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
    bool want_release = false;
    for (const auto& a : args) {
        if (a == "--release" || a == "-release") want_release = true;
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


    std::vector<std::string> argv = {"clang"};
    for (const auto& s : cfg.compile) argv.push_back(resolve(cfg.source + "/" + s));
    if (want_release) argv.insert(argv.end(), cfg.release.begin(), cfg.release.end());
    else              argv.insert(argv.end(), cfg.flags.begin(),   cfg.flags.end());
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

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <set>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace {

enum class Status { PASS, FAIL, SKIP };

struct TestResult {
    Status       status;
    std::string  rel;
    std::string  reason;
};

std::string normalize_ir(const std::string& path, const std::vector<std::string>& ignores) {
    std::string raw = read_file(path);
    std::string out;
    std::size_t i = 0;
    int line_no = 0;
    while (i < raw.size()) {
        std::size_t nl = raw.find('\n', i);
        std::size_t end = (nl == std::string::npos) ? raw.size() : nl + 1;
        line_no++;
        if (line_no > 3) {
            std::string line(raw.data() + i, end - i);
            bool skip = false;
            for (const auto& pat : ignores) {
                if (line.find(pat) != std::string::npos) { skip = true; break; }
            }
            if (!skip) out += line;
        }
        i = end;
    }
    return out;
}

void write_result(const std::string& path, const char* status,
                  const std::string& rel, const std::string& reason) {
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return;
    fprintf(f, "%s\t%s\t%s\n", status, rel.c_str(), reason.c_str());
    fclose(f);
}

int run_compiler_silent(const std::string& ura_bin, const std::string& file) {
    pid_t pid = fork();
    if (pid < 0) return 127;
    if (pid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0) { dup2(devnull, 1); dup2(devnull, 2); close(devnull); }
        const char* argv[] = {
            ura_bin.c_str(), file.c_str(), "-no-exec", "-no-debug", nullptr
        };
        execvp(argv[0], const_cast<char* const*>(argv));
        _exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return -1;
    if (WIFEXITED(status))   return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return -1;
}

[[noreturn]] void run_one_test(const std::string& ura_file,
                               const std::string& tests_root,
                               const std::string& ura_bin,
                               const std::vector<std::string>& ignores,
                               const std::string& result_path) {
    
    std::string rel = ura_file;
    std::string root_with_slash = tests_root + "/";
    if (rel.rfind(root_with_slash, 0) == 0) rel = rel.substr(root_with_slash.size());
    if (rel.size() >= 4 && rel.substr(rel.size() - 4) == ".ura")
        rel = rel.substr(0, rel.size() - 4);

    bool is_negative = ura_file.find("/parse_errors/") != std::string::npos;
    if (is_negative) {
        int rc = run_compiler_silent(ura_bin, ura_file);
        if (rc == 0)        write_result(result_path, "FAIL", rel, "expected parse error, none fired");
        else if (rc >= 128) write_result(result_path, "FAIL", rel, "crashed (signal)");
        else                write_result(result_path, "PASS", rel, "");
        _exit(0);
    }

    std::string head = read_file(ura_file);
    bool has_comment = head.size() >= 2 && head[0] == '/' && head[1] == '/';
    if (!has_comment) {
        write_result(result_path, "SKIP", rel, "");
        _exit(0);
    }

    
    std::string dir  = dirname_of(ura_file);
    std::string base = stem(ura_file);
    std::string ll_expected = dir + base + ".ll";
    std::string ll_got      = dir + "build/" + base + ".ll";
    if (!is_file(ll_expected)) {
        write_result(result_path, "FAIL", rel, "no .ll reference");
        _exit(0);
    }

    
    int rc = run_compiler_silent(ura_bin, ura_file);
    if (rc != 0) {
        write_result(result_path, "FAIL", rel, "compilation error");
        _exit(0);
    }
    if (!is_file(ll_got)) {
        write_result(result_path, "FAIL", rel, "no IR generated");
        _exit(0);
    }

    
    std::string a = normalize_ir(ll_got,      ignores);
    std::string b = normalize_ir(ll_expected, ignores);
    if (a == b) write_result(result_path, "PASS", rel, "");
    else        write_result(result_path, "FAIL", rel, "IR mismatch");
    _exit(0);
}

TestResult parse_result(const std::string& path) {
    TestResult r{Status::FAIL, "", "missing result"};
    std::string s = read_file(path);
    if (s.empty()) return r;
    
    auto t1 = s.find('\t');
    if (t1 == std::string::npos) return r;
    auto t2 = s.find('\t', t1 + 1);
    std::string status = s.substr(0, t1);
    std::string rel    = (t2 == std::string::npos) ? s.substr(t1 + 1) : s.substr(t1 + 1, t2 - t1 - 1);
    std::string reason = (t2 == std::string::npos) ? "" : s.substr(t2 + 1);
    
    while (!reason.empty() && (reason.back() == '\n' || reason.back() == '\r')) reason.pop_back();
    if (status == "PASS") r.status = Status::PASS;
    else if (status == "SKIP") r.status = Status::SKIP;
    else r.status = Status::FAIL;
    r.rel    = rel;
    r.reason = reason;
    return r;
}

} 

int cmd_test(const std::vector<std::string>& args) {
    const AnvilConfig& cfg = config();
    std::string ura_bin    = resolve(cfg.output);
    std::string tests_root = resolve(cfg.tests);

    if (!is_file(ura_bin)) {
        fprintf(stderr, ANSI_RED "test: compiler not built (%s). Run `build` first." ANSI_RESET "\n",
                ura_bin.c_str());
        return 1;
    }

    
    std::vector<std::string> files;
    auto collect = [&](const std::string& p) {
        if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") files.push_back(p);
    };
    if (args.empty()) {
        walk(tests_root, collect);
    } else {
        for (const auto& a : args) {
            std::string path = (a[0] == '/') ? a : (tests_root + "/" + a);
            walk(path, collect);
        }
    }
    std::sort(files.begin(), files.end());

    if (files.empty()) {
        printf(ANSI_YELLOW "No tests found under %s" ANSI_RESET "\n", tests_root.c_str());
        return 0;
    }

    printf(ANSI_YELLOW "Running %zu tests..." ANSI_RESET "\n", files.size());

    
    char tdir_tpl[] = "/tmp/anvil_test.XXXXXX";
    if (!mkdtemp(tdir_tpl)) {
        fprintf(stderr, ANSI_RED "test: mkdtemp failed" ANSI_RESET "\n");
        return 1;
    }
    std::string tdir = tdir_tpl;

    int max_parallel = std::max(1, cfg.max_parallel);
    std::set<pid_t> running;

    auto reap_one = [&]() {
        int status = 0;
        pid_t pid = waitpid(-1, &status, 0);
        if (pid > 0) running.erase(pid);
    };

    for (std::size_t i = 0; i < files.size(); i++) {
        while ((int)running.size() >= max_parallel) reap_one();

        pid_t pid = fork();
        if (pid < 0) { fprintf(stderr, "fork failed\n"); break; }
        if (pid == 0) {
            std::string out = tdir + "/" + std::to_string(i);
            run_one_test(files[i], tests_root, ura_bin, cfg.ignore_ir, out);
            
        }
        running.insert(pid);
    }
    while (!running.empty()) reap_one();

    
    int p = 0, f = 0, s = 0;
    for (std::size_t i = 0; i < files.size(); i++) {
        TestResult r = parse_result(tdir + "/" + std::to_string(i));
        switch (r.status) {
            case Status::PASS:
                printf(ANSI_GREEN "  PASS %s" ANSI_RESET "\n", r.rel.c_str());
                p++; break;
            case Status::FAIL:
                printf(ANSI_RED   "  FAIL %s (%s)" ANSI_RESET "\n", r.rel.c_str(), r.reason.c_str());
                f++; break;
            case Status::SKIP:
                printf(ANSI_YELLOW"  SKIP %s" ANSI_RESET "\n", r.rel.c_str());
                s++; break;
        }
    }

    
    for (std::size_t i = 0; i < files.size(); i++) unlink((tdir + "/" + std::to_string(i)).c_str());
    rmdir(tdir.c_str());

    printf("\n" ANSI_GREEN "Passed: %d" ANSI_RESET "\n", p);
    if (f) printf(ANSI_RED    "Failed: %d" ANSI_RESET "\n", f);
    if (s) printf(ANSI_YELLOW "Skipped: %d" ANSI_RESET "\n", s);

    return f;
}

#include <cstdio>
#include <string>
#include <vector>

namespace {

struct Check {
    const char* name;
    const char* probe;      
    bool        required;
};

void print_tool(const Check& c, int& missing) {
    CaptureResult r = run_capture({"/bin/sh", "-c", std::string(c.probe) + " 2>&1 | head -1"});
    if (r.code == 0 && !r.stdout_.empty()) {
        
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

} 

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

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace {

int mkdir_p(const std::string& path) {
    std::string acc;
    for (std::size_t i = 0; i < path.size(); i++) {
        acc += path[i];
        if (path[i] == '/' && !acc.empty()) mkdir(acc.c_str(), 0755);
    }
    return mkdir(path.c_str(), 0755);
}

int copy_file(const std::string& src, const std::string& dst) {
    FILE* in  = fopen(src.c_str(), "rb");
    if (!in) return -1;
    FILE* out = fopen(dst.c_str(), "wb");
    if (!out) { fclose(in); return -1; }
    char buf[8192];
    std::size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) { fclose(in); fclose(out); return -1; }
    }
    fclose(in); fclose(out);
    return 0;
}

int compile_silent(const std::string& ura_bin, const std::string& ura_file) {
    pid_t pid = fork();
    if (pid < 0) return 127;
    if (pid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0) { dup2(devnull, 1); dup2(devnull, 2); close(devnull); }
        const char* argv[] = {
            ura_bin.c_str(), ura_file.c_str(), "-no-exec", "-no-debug", nullptr
        };
        execvp(argv[0], const_cast<char* const*>(argv));
        _exit(127);
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return -1;
    if (WIFEXITED(status))   return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return -1;
}

std::string parse_path_comment(const std::string& content) {
    std::size_t i = 0;
    if (content.size() < 2 || content[0] != '/' || content[1] != '/') return "";
    i = 2;
    while (i < content.size() && (content[i] == ' ' || content[i] == '\t')) i++;
    std::size_t start = i;
    while (i < content.size()) {
        char c = content[i];
        bool ok = std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '/' || c == '-' || c == '.';
        if (!ok) break;
        i++;
    }
    std::string p = content.substr(start, i - start);
    
    if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") p.resize(p.size() - 4);
    return p;
}

} 

int cmd_copy(const std::vector<std::string>& args) {
    if (args.empty()) {
        fprintf(stderr, ANSI_RED "usage: copy <file.ura>" ANSI_RESET
                        " (first line must be `// <folder>/<name>`)\n");
        return 1;
    }
    const AnvilConfig& cfg = config();
    std::string ura_src = args[0];
    if (!ura_src.empty() && ura_src[0] != '/') ura_src = resolve(cfg.source) + "/" + ura_src;
    if (!is_file(ura_src)) {
        fprintf(stderr, ANSI_RED "copy: %s not found" ANSI_RESET "\n", ura_src.c_str());
        return 1;
    }

    std::string body = read_file(ura_src);
    std::string subpath = parse_path_comment(body);
    if (subpath.empty()) {
        fprintf(stderr, ANSI_RED "copy: first line must be `// <folder>/<name>`" ANSI_RESET "\n");
        return 1;
    }
    auto slash = subpath.find_last_of('/');
    if (slash == std::string::npos) {
        fprintf(stderr, ANSI_RED "copy: path must have a folder, e.g. `// builtins/printf`" ANSI_RESET "\n");
        return 1;
    }
    std::string folder   = subpath.substr(0, slash);
    std::string name     = subpath.substr(slash + 1);
    std::string test_dir = resolve(cfg.tests) + "/" + folder;
    std::string ura_dst  = test_dir + "/" + name + ".ura";
    std::string ll_dst   = test_dir + "/" + name + ".ll";

    mkdir_p(test_dir);

    std::string ura_bin = resolve(cfg.output);
    if (!is_file(ura_bin)) {
        fprintf(stderr, ANSI_RED "copy: compiler not built. Run `build` first." ANSI_RESET "\n");
        return 1;
    }
    int rc = compile_silent(ura_bin, ura_src);
    if (rc != 0) {
        fprintf(stderr, ANSI_RED "copy: compilation failed (code %d)" ANSI_RESET "\n", rc);
        return 1;
    }

    
    std::string src_dir  = dirname_of(ura_src);
    std::string src_stem = stem(ura_src);
    std::string ll_src   = src_dir + "build/" + src_stem + ".ll";
    if (!is_file(ll_src)) {
        fprintf(stderr, ANSI_RED "copy: generated IR not found at %s" ANSI_RESET "\n", ll_src.c_str());
        return 1;
    }
    if (copy_file(ura_src, ura_dst) != 0) {
        fprintf(stderr, ANSI_RED "copy: cannot write %s" ANSI_RESET "\n", ura_dst.c_str());
        return 1;
    }
    if (copy_file(ll_src, ll_dst) != 0) {
        fprintf(stderr, ANSI_RED "copy: cannot write %s" ANSI_RESET "\n", ll_dst.c_str());
        return 1;
    }
    printf(ANSI_GREEN "Saved test:" ANSI_RESET "\n  %s\n  %s\n", ura_dst.c_str(), ll_dst.c_str());
    return 0;
}

int cmd_update_tests(const std::vector<std::string>&) {
    const AnvilConfig& cfg = config();
    std::string ura_bin = resolve(cfg.output);
    if (!is_file(ura_bin)) {
        fprintf(stderr, ANSI_RED "update-tests: compiler not built. Run `build` first." ANSI_RESET "\n");
        return 1;
    }

    std::string tests = resolve(cfg.tests);
    std::vector<std::string> files;
    walk(tests, [&](const std::string& p) {
        if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") files.push_back(p);
    });
    std::sort(files.begin(), files.end());

    int passed = 0, failed = 0;
    printf(ANSI_YELLOW "Regenerating %zu .ll reference files..." ANSI_RESET "\n", files.size());
    for (const auto& ura_file : files) {
        std::string d = dirname_of(ura_file);
        std::string b = stem(ura_file);
        std::string ll_expected  = d + b + ".ll";
        std::string ll_generated = d + "build/" + b + ".ll";

        int rc = compile_silent(ura_bin, ura_file);
        if (rc != 0) {
            printf("  " ANSI_RED "FAIL %s (compilation error)" ANSI_RESET "\n", b.c_str());
            failed++; continue;
        }
        if (!is_file(ll_generated)) {
            printf("  " ANSI_RED "FAIL %s (no IR generated)" ANSI_RESET "\n", b.c_str());
            failed++; continue;
        }
        if (copy_file(ll_generated, ll_expected) != 0) {
            printf("  " ANSI_RED "FAIL %s (cannot write)" ANSI_RESET "\n", b.c_str());
            failed++; continue;
        }
        printf("  " ANSI_GREEN "UPDATED %s.ll" ANSI_RESET "\n", b.c_str());
        passed++;
    }
    printf("\n" ANSI_GREEN "Updated: %d" ANSI_RESET "\n", passed);
    if (failed) printf(ANSI_RED "Failed: %d" ANSI_RESET "\n", failed);
    return failed;
}

int cmd_update_projects(const std::vector<std::string>&) {
    const AnvilConfig& cfg = config();
    std::string projects_dir = resolve(cfg.projects_dir);
    std::string tests_dir    = resolve(cfg.tests);

    std::vector<std::string> files;
    walk(projects_dir, [&](const std::string& p) {
        if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") files.push_back(p);
    });
    std::sort(files.begin(), files.end());

    int updated = 0;
    for (const auto& path : files) {
        
        std::string rel = path;
        if (rel.rfind(tests_dir + "/", 0) == 0) rel = rel.substr(tests_dir.size() + 1);
        if (rel.size() >= 4 && rel.substr(rel.size() - 4) == ".ura")
            rel = rel.substr(0, rel.size() - 4);
        std::string expected = "// " + rel;

        std::string body = read_file(path);
        std::size_t nl = body.find('\n');
        std::string first = (nl == std::string::npos) ? body : body.substr(0, nl);
        if (first == expected) continue;

        std::string rest = (nl == std::string::npos) ? "" : body.substr(nl + 1);
        std::string new_body;
        if (first.size() >= 2 && first[0] == '/' && first[1] == '/') {
            
            new_body = expected + "\n" + rest;
        } else {
            
            new_body = expected + "\n" + body;
        }
        FILE* f = fopen(path.c_str(), "w");
        if (!f) continue;
        fwrite(new_body.data(), 1, new_body.size(), f);
        fclose(f);
        printf("  " ANSI_GREEN "updated" ANSI_RESET " %s\n", rel.c_str());
        updated++;
    }
    printf(ANSI_GREEN "Done: %d file(s) updated" ANSI_RESET "\n", updated);
    return 0;
}

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <vector>

namespace {

bool flag_set(const std::vector<std::string>& args, const char* name) {
    for (const auto& a : args) if (a == name) return true;
    return false;
}

int sync_repo(const std::string& src_dir,
              const std::string& remote,
              const std::string& label,
              bool               confirm)
{
    printf(ANSI_YELLOW "Syncing %s..." ANSI_RESET "  " ANSI_DIM "(%s)" ANSI_RESET "\n",
           label.c_str(), remote.c_str());
    if (!confirm) {
        printf("  " ANSI_DIM "dry-run: would clone %s, mirror from %s, commit, push."
               " Pass --confirm to actually push." ANSI_RESET "\n", remote.c_str(), src_dir.c_str());
        return 0;
    }

    
    std::string cmd =
        "set -e; "
        "tmp=$(mktemp -d); "
        "trap 'rm -rf \"$tmp\"' EXIT; "
        "git clone --quiet '" + remote + "' \"$tmp\"; "
        "find \"$tmp\" -mindepth 1 -not -path \"$tmp/.git*\" -delete; "
        "rsync -a --exclude='.git' '" + src_dir + "/' \"$tmp/\"; "
        "if [ -z \"$(git -C \"$tmp\" status --porcelain)\" ]; then "
        "  printf '  \\033[2m%s unchanged\\033[0m\\n' '" + label + "'; "
        "  exit 0; "
        "fi; "
        "git -C \"$tmp\" add -A; "
        "git -C \"$tmp\" commit -m 'sync: " + label + " " + today_string() + "'; "
        "git -C \"$tmp\" push -u --quiet origin HEAD; "
        "printf '  \\033[0;32m%s pushed\\033[0m\\n' '" + label + "'; ";
    return run_sh(cmd);
}

} 

int cmd_release_anvil(const std::vector<std::string>& args) {
    bool confirm = flag_set(args, "--confirm");
    std::string src    = project_root() + "/config/anvil";
    std::string remote = config().anvil_repo;
    if (remote.empty()) {
        fprintf(stderr, ANSI_RED "release-anvil: [sync].anvil_repo missing from anvil.toml" ANSI_RESET "\n");
        return 1;
    }
    return sync_repo(src, remote, "anvil", confirm);
}

int cmd_release_extension(const std::vector<std::string>& args) {
    bool confirm = flag_set(args, "--confirm");
    std::string src    = project_root() + "/config/vscode-extension";
    std::string remote = config().extension_repo;
    if (remote.empty()) {
        fprintf(stderr, ANSI_RED "release-extension: [sync].extension_repo missing from anvil.toml" ANSI_RESET "\n");
        return 1;
    }
    return sync_repo(src, remote, "vscode-extension", confirm);
}

int cmd_release_projects(const std::vector<std::string>& args) {
    bool confirm = flag_set(args, "--confirm");
    const AnvilConfig& cfg = config();
    std::string projects = resolve(cfg.projects_dir);
    DIR* d = opendir(projects.c_str());
    if (!d) {
        fprintf(stderr, ANSI_RED "release-projects: %s not found" ANSI_RESET "\n", projects.c_str());
        return 1;
    }
    int rc = 0;
    while (dirent* e = readdir(d)) {
        if (e->d_name[0] == '.') continue;
        std::string sub = projects + "/" + e->d_name;
        struct stat st;
        if (stat(sub.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) continue;
        std::string url = cfg.project_url;
        auto pos = url.find("{name}");
        if (pos != std::string::npos) url.replace(pos, 6, e->d_name);
        int r = sync_repo(sub, url, e->d_name, confirm);
        if (r != 0) rc = r;
    }
    closedir(d);
    return rc;
}

static std::string build_avatar() {
    printf(ANSI_YELLOW "Building avatar..." ANSI_RESET "\n");
    std::string dir = project_root() + "/src/tools/avatar";
    std::string cmd = "make -C '" + dir + "' clean && make -C '" + dir + "'";
    if (run_sh(cmd) != 0) return {};
    return dir + "/avatar";
}

static int package_release(const std::string& ura_bin,
                           const std::string& avatar_bin,
                           const std::string& ura_lib_dir)
{
    std::string tarball = project_root() + "/build/ura-release-" + today_string() + ".tar.gz";
    
    std::string cmd =
        "set -e; "
        "stage=$(mktemp -d); "
        "trap 'rm -rf \"$stage\"' EXIT; "
        "mkdir -p \"$stage/bin\" \"$stage/share/ura\"; "
        "cp '" + ura_bin    + "' \"$stage/bin/ura\"; "
        "cp '" + avatar_bin + "' \"$stage/bin/avatar\"; "
        "cp -R '" + ura_lib_dir + "' \"$stage/share/ura/lib\"; "
        "cat > \"$stage/install.sh\" <<'EOF'\n"
        "#!/bin/sh\n"
        "set -e\n"
        "PREFIX=\"${PREFIX:-$HOME/.local}\"\n"
        "mkdir -p \"$PREFIX/bin\" \"$PREFIX/share/ura\"\n"
        "cp bin/ura bin/avatar \"$PREFIX/bin/\"\n"
        "cp -R share/ura/lib \"$PREFIX/share/ura/\"\n"
        "echo \"installed to $PREFIX\"\n"
        "echo \"export URA_LIB=$PREFIX/share/ura/lib   # add to your shell rc\"\n"
        "EOF\n"
        "chmod +x \"$stage/install.sh\"; "
        "tar -czf '" + tarball + "' -C \"$stage\" .; "
        "printf 'packaged %s\\n' '" + tarball + "'; ";
    return run_sh(cmd);
}

int cmd_release(const std::vector<std::string>& args) {
    bool confirm = flag_set(args, "--confirm");
    const AnvilConfig& cfg = config();

    printf(ANSI_YELLOW "Building release binary..." ANSI_RESET "\n");
    extern int cmd_build(const std::vector<std::string>&);
    int rc = cmd_build({"--release"});
    if (rc != 0) return rc;

    std::string avatar_bin = build_avatar();
    if (avatar_bin.empty()) return 1;

    std::string ura_lib = resolve(cfg.ura_lib);
    rc = package_release(resolve(cfg.output), avatar_bin, ura_lib);
    if (rc != 0) return rc;

    if (!cfg.anvil_repo.empty())
        sync_repo(project_root() + "/config/anvil", cfg.anvil_repo, "anvil", confirm);
    if (!cfg.ura_lib_repo.empty())
        sync_repo(ura_lib, cfg.ura_lib_repo, "ura-lib", confirm);
    cmd_release_projects(args);
    cmd_release_extension(args);

    if (!confirm) {
        printf("\n  " ANSI_DIM "dry-run: re-run with " ANSI_CYAN "--confirm" ANSI_DIM
               " to actually push." ANSI_RESET "\n");
        return 0;
    }
    printf("\n  " ANSI_BOLD "Next steps (binary):" ANSI_RESET "\n"
           "    git add %s src/tools/avatar/avatar\n"
           "    git commit -m 'chore: release %s'\n"
           "    git push\n",
           cfg.output.c_str(), today_string().c_str());
    return 0;
}

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

int cmd_reload(const std::vector<std::string>&) {
    invalidate_config();
    (void)config();
    const char *ura_lib = getenv("URA_LIB");
    printf(ANSI_GREEN "Config reloaded" ANSI_RESET "\n");
    printf("URA_LIB=%s\n", ura_lib ? ura_lib : "(unset)");
    return 0;
}

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

int cmd_indent(const std::vector<std::string>&) {
    if (!has_tool("clang-format")) {
        fprintf(stderr, ANSI_RED "indent: clang-format not found" ANSI_RESET "\n");
        return 1;
    }
    printf(ANSI_YELLOW "Formatting..." ANSI_RESET "\n");
    std::string src = resolve(config().source);
    
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
    if (!is_file(mc)) return 0;             
    argv = {"python3", mc};
    argv.insert(argv.end(), files.begin(), files.end());
    return run_stream(argv);
}

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

#include <cstdio>
#include <string>
#include <vector>

namespace {

const char* IMAGE = "ura-dev";

bool image_exists() {
    CaptureResult r = run_capture({
        "docker", "image", "inspect", IMAGE, "--format", "{{.Id}}"
    });
    return r.code == 0;
}

int build_image_if_needed() {
    if (image_exists()) return 0;
    printf(ANSI_YELLOW "Building %s image (first time)..." ANSI_RESET "\n", IMAGE);
    std::string cmd = "docker build --platform linux/amd64 -t ";
    cmd += IMAGE; cmd += " '"; cmd += project_root(); cmd += "'";
    return run_sh(cmd);
}

} 

int cmd_docker_build(const std::vector<std::string>&) {
    std::string cmd = "docker build --platform linux/amd64 -t ";
    cmd += IMAGE; cmd += " '"; cmd += project_root(); cmd += "'";
    return run_sh(cmd);
}

int cmd_shell(const std::vector<std::string>&) {
    if (!has_tool("docker")) {
        fprintf(stderr, ANSI_RED "shell: docker not found in PATH" ANSI_RESET "\n");
        return 1;
    }
    int rc = build_image_if_needed();
    if (rc != 0) return rc;

    std::string cmd = "docker run --platform linux/amd64 --rm -it -v '"
                    + project_root() + ":/workspace' " + IMAGE;
    return run_sh(cmd);
}
