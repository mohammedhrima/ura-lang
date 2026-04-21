#include "../command.h"
#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../fs.h"
#include "../paths.h"

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

// ---- IR diff --------------------------------------------------------------
// Read `path`, drop first 3 lines, drop any line containing any of the
// `ignores` substrings, return the remainder as a single string.
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

// ---- child worker ---------------------------------------------------------
void write_result(const std::string& path, const char* status,
                  const std::string& rel, const std::string& reason) {
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return;
    fprintf(f, "%s\t%s\t%s\n", status, rel.c_str(), reason.c_str());
    fclose(f);
}

// Run ura compiler silently (stdout/stderr redirected to /dev/null), return code.
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

// Whole per-test flow that runs in the forked child. Never returns (calls _exit).
[[noreturn]] void run_one_test(const std::string& ura_file,
                               const std::string& tests_root,
                               const std::string& ura_bin,
                               const std::vector<std::string>& ignores,
                               const std::string& result_path) {
    // Relative name used in result lines
    std::string rel = ura_file;
    std::string root_with_slash = tests_root + "/";
    if (rel.rfind(root_with_slash, 0) == 0) rel = rel.substr(root_with_slash.size());
    if (rel.size() >= 4 && rel.substr(rel.size() - 4) == ".ura")
        rel = rel.substr(0, rel.size() - 4);

    // 1. First-line sentinel
    std::string head = read_file(ura_file);
    bool has_comment = head.size() >= 2 && head[0] == '/' && head[1] == '/';
    if (!has_comment) {
        write_result(result_path, "SKIP", rel, "");
        _exit(0);
    }

    // 2. Check reference .ll exists
    std::string dir  = dirname_of(ura_file);
    std::string base = stem(ura_file);
    std::string ll_expected = dir + base + ".ll";
    std::string ll_got      = dir + "build/" + base + ".ll";
    if (!is_file(ll_expected)) {
        write_result(result_path, "FAIL", rel, "no .ll reference");
        _exit(0);
    }

    // 3. Compile
    int rc = run_compiler_silent(ura_bin, ura_file);
    if (rc != 0) {
        write_result(result_path, "FAIL", rel, "compilation error");
        _exit(0);
    }
    if (!is_file(ll_got)) {
        write_result(result_path, "FAIL", rel, "no IR generated");
        _exit(0);
    }

    // 4. Diff
    std::string a = normalize_ir(ll_got,      ignores);
    std::string b = normalize_ir(ll_expected, ignores);
    if (a == b) write_result(result_path, "PASS", rel, "");
    else        write_result(result_path, "FAIL", rel, "IR mismatch");
    _exit(0);
}

// ---- parent: gather + throttle --------------------------------------------
TestResult parse_result(const std::string& path) {
    TestResult r{Status::FAIL, "", "missing result"};
    std::string s = read_file(path);
    if (s.empty()) return r;
    // Format: STATUS\tREL\tREASON\n
    auto t1 = s.find('\t');
    if (t1 == std::string::npos) return r;
    auto t2 = s.find('\t', t1 + 1);
    std::string status = s.substr(0, t1);
    std::string rel    = (t2 == std::string::npos) ? s.substr(t1 + 1) : s.substr(t1 + 1, t2 - t1 - 1);
    std::string reason = (t2 == std::string::npos) ? "" : s.substr(t2 + 1);
    // trim trailing \n
    while (!reason.empty() && (reason.back() == '\n' || reason.back() == '\r')) reason.pop_back();
    if (status == "PASS") r.status = Status::PASS;
    else if (status == "SKIP") r.status = Status::SKIP;
    else r.status = Status::FAIL;
    r.rel    = rel;
    r.reason = reason;
    return r;
}

} // namespace

int cmd_test(const std::vector<std::string>& args) {
    const AnvilConfig& cfg = config();
    std::string ura_bin    = resolve(cfg.output);
    std::string tests_root = resolve(cfg.tests);
    std::string ura_lib    = resolve(cfg.ura_lib);

    if (!is_file(ura_bin)) {
        fprintf(stderr, ANSI_RED "test: compiler not built (%s). Run `build` first." ANSI_RESET "\n",
                ura_bin.c_str());
        return 1;
    }
    setenv("URA_LIB", ura_lib.c_str(), 1);

    // Collect .ura files. If `args` is non-empty, each arg selects a subdir under tests/.
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

    // Tmpdir for result files
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
            // unreachable
        }
        running.insert(pid);
    }
    while (!running.empty()) reap_one();

    // Aggregate results in original order
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

    // Cleanup tmpdir
    for (std::size_t i = 0; i < files.size(); i++) unlink((tdir + "/" + std::to_string(i)).c_str());
    rmdir(tdir.c_str());

    printf("\n" ANSI_GREEN "Passed: %d" ANSI_RESET "\n", p);
    if (f) printf(ANSI_RED    "Failed: %d" ANSI_RESET "\n", f);
    if (s) printf(ANSI_YELLOW "Skipped: %d" ANSI_RESET "\n", s);

    return f;
}
