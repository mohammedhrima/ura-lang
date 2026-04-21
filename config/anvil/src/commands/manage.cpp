// copy, update-tests, update-projects — commands that manage test artifacts.

#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../fs.h"
#include "../paths.h"

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

// ---- shared helpers -------------------------------------------------------
namespace {

int mkdir_p(const std::string& path) {
    std::string acc;
    for (std::size_t i = 0; i < path.size(); i++) {
        acc += path[i];
        if (path[i] == '/' && !acc.empty()) mkdir(acc.c_str(), 0755);
    }
    return mkdir(path.c_str(), 0755);
}

// Copy a file byte-for-byte. Returns 0 on success.
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

// Run the ura compiler on `ura_file` with -no-exec -no-debug, streaming output
// to /dev/null. Returns the exit code.
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

// Extract the path string from a first-line comment:
//   "// folder/name" → "folder/name"
// Matches [a-zA-Z0-9_/-]+. Returns empty string if no match.
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
    // Strip trailing ".ura" if present
    if (p.size() >= 4 && p.substr(p.size() - 4) == ".ura") p.resize(p.size() - 4);
    return p;
}

} // namespace

// ---- copy -----------------------------------------------------------------
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

    // Generated .ll lives next to the source under build/<basename>.ll
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

// ---- update-tests ---------------------------------------------------------
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

// ---- update-projects ------------------------------------------------------
// Rewrite the first-line path comment in every projects/*.ura to match its
// location. Mirrors config.sh update_projects().
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
        // expected first line: "// <rel-from-tests>-without-.ura>"
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
            // Replace existing comment
            new_body = expected + "\n" + rest;
        } else {
            // Prepend comment
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
