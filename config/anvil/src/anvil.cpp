#include "anvil.h"

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <map>
#include <optional>
#include <spawn.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

extern char** environ;

const std::string& detect_os() {
    static std::string cached;
    if (!cached.empty()) return cached;
    struct utsname u{};
    uname(&u);
    std::string sys = u.sysname;
    if (sys == "Darwin") { cached = "macos"; return cached; }
    if (sys == "Linux") {
        if (has_tool("apt-get"))  { cached = "linux-apt";    return cached; }
        if (has_tool("pacman"))   { cached = "linux-pacman"; return cached; }
        if (has_tool("dnf"))      { cached = "linux-dnf";    return cached; }
        cached = "linux";
        return cached;
    }
    cached = "unknown";
    return cached;
}

std::string today_string() {
    std::time_t t = std::time(nullptr);
    std::tm* tm  = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return buf;
}

void walk(const std::string& root, const std::function<void(const std::string&)>& cb) {
    DIR* d = opendir(root.c_str());
    if (!d) return;
    while (dirent* e = readdir(d)) {
        if (std::strcmp(e->d_name, ".") == 0 || std::strcmp(e->d_name, "..") == 0) continue;
        std::string full = root + "/" + e->d_name;
        struct stat st;
        if (lstat(full.c_str(), &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) {
            if (std::strcmp(e->d_name, "build") == 0) continue;     
            walk(full, cb);
        } else if (S_ISREG(st.st_mode)) {
            cb(full);
        }
    }
    closedir(d);
}

std::string read_file(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return {};
    std::string out;
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) out.append(buf, buf + n);
    fclose(f);
    return out;
}

bool is_file(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

std::string dirname_of(const std::string& path) {
    auto slash = path.find_last_of('/');
    if (slash == std::string::npos) return "./";
    return path.substr(0, slash + 1);
}

std::string stem(const std::string& path) {
    auto slash = path.find_last_of('/');
    std::string base = slash == std::string::npos ? path : path.substr(slash + 1);
    auto dot = base.find_last_of('.');
    return dot == std::string::npos ? base : base.substr(0, dot);
}

static std::string g_root;
static std::string g_toml;
static bool        g_resolved = false;

static std::string realpath_or(const std::string& p) {
    char buf[4096];
    return realpath(p.c_str(), buf) ? std::string(buf) : p;
}

static void resolve_cwd() {
    char buf[4096];
    if (!getcwd(buf, sizeof(buf))) return;
    std::string candidate = std::string(buf) + "/anvil.toml";
    if (is_file(candidate)) {
        g_toml = candidate;
        g_root = buf;
    }
}

void set_anvil_toml(const std::string& path) {
    if (path.empty() || !is_file(path)) return;
    g_toml = realpath_or(path);
    auto slash = g_toml.find_last_of('/');
    g_root = (slash == std::string::npos) ? "." : g_toml.substr(0, slash);
    g_resolved = true;
}

const std::string& project_root() {
    if (!g_resolved) { resolve_cwd(); g_resolved = true; }
    return g_root;
}

const std::string& anvil_toml_path() {
    if (!g_resolved) { resolve_cwd(); g_resolved = true; }
    return g_toml;
}

std::string resolve(const std::string& rel) {
    if (!rel.empty() && rel[0] == '/') return rel;
    const auto& r = project_root();
    return r.empty() ? rel : (r + "/" + rel);
}

static std::vector<char*> to_argv(const std::vector<std::string>& argv) {
    std::vector<char*> out;
    out.reserve(argv.size() + 1);
    for (const auto& s : argv) out.push_back(const_cast<char*>(s.c_str()));
    out.push_back(nullptr);
    return out;
}

static int finalize(int status) {
    if (WIFEXITED(status))   return WEXITSTATUS(status);
    if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
    return -1;
}

int run_stream(const std::vector<std::string>& argv) {
    if (argv.empty()) return 1;
    auto cargv = to_argv(argv);
    pid_t pid;
    int rc = posix_spawnp(&pid, cargv[0], nullptr, nullptr, cargv.data(), environ);
    if (rc != 0) {
        fprintf(stderr, "spawn '%s' failed: %s\n", cargv[0], std::strerror(rc));
        return 127;
    }
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return -1;
    return finalize(status);
}

static std::string slurp(int fd) {
    std::string out;
    char buf[4096];
    for (;;) {
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n > 0) out.append(buf, buf + n);
        else if (n == 0) break;
        else if (errno == EINTR) continue;
        else break;
    }
    return out;
}

CaptureResult run_capture(const std::vector<std::string>& argv) {
    CaptureResult r;
    if (argv.empty()) { r.code = 1; return r; }

    int out_pipe[2]{};
    int err_pipe[2]{};
    if (pipe(out_pipe) != 0 || pipe(err_pipe) != 0) { r.code = 127; return r; }

    posix_spawn_file_actions_t acts;
    posix_spawn_file_actions_init(&acts);
    posix_spawn_file_actions_adddup2(&acts, out_pipe[1], STDOUT_FILENO);
    posix_spawn_file_actions_adddup2(&acts, err_pipe[1], STDERR_FILENO);
    posix_spawn_file_actions_addclose(&acts, out_pipe[0]);
    posix_spawn_file_actions_addclose(&acts, err_pipe[0]);
    posix_spawn_file_actions_addclose(&acts, out_pipe[1]);
    posix_spawn_file_actions_addclose(&acts, err_pipe[1]);

    auto cargv = to_argv(argv);
    pid_t pid;
    int rc = posix_spawnp(&pid, cargv[0], &acts, nullptr, cargv.data(), environ);
    posix_spawn_file_actions_destroy(&acts);

    close(out_pipe[1]);
    close(err_pipe[1]);

    if (rc != 0) {
        close(out_pipe[0]);
        close(err_pipe[0]);
        r.code = 127;
        return r;
    }

    r.stdout_ = slurp(out_pipe[0]);
    r.stderr_ = slurp(err_pipe[0]);
    close(out_pipe[0]);
    close(err_pipe[0]);

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) { r.code = -1; return r; }
    r.code = finalize(status);
    return r;
}

std::vector<std::string> split_ws(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!cur.empty()) { out.push_back(cur); cur.clear(); }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

int run_sh(const std::string& cmd) {
    return run_stream({"/bin/sh", "-c", cmd});
}

bool has_tool(const std::string& tool) {
    CaptureResult r = run_capture({"/bin/sh", "-c", "command -v '" + tool + "' >/dev/null 2>&1"});
    return r.code == 0;
}

static AnvilConfig g_cfg;
static bool        g_loaded = false;

[[noreturn]] static void die(const std::string& msg) {
    fprintf(stderr, ANSI_RED "anvil config: %s" ANSI_RESET "\n", msg.c_str());
    std::exit(2);
}

namespace {

struct Toml {
    std::map<std::string, std::string>              strs;
    std::map<std::string, int>                      ints;
    std::map<std::string, std::vector<std::string>> arrs;

    void parse(const std::string& path);

    std::string str(const char* k) const {
        auto it = strs.find(k);
        return it == strs.end() ? std::string{} : it->second;
    }
    std::optional<int> num(const char* k) const {
        auto it = ints.find(k);
        if (it == ints.end()) return std::nullopt;
        return it->second;
    }
    std::vector<std::string> arr(const char* k) const {
        auto it = arrs.find(k);
        return it == arrs.end() ? std::vector<std::string>{} : it->second;
    }
};

static void skip_ws(const std::string& s, size_t& i) {
    while (i < s.size()) {
        if (std::isspace(static_cast<unsigned char>(s[i]))) { i++; continue; }
        if (s[i] == '#') { while (i < s.size() && s[i] != '\n') i++; continue; }
        break;
    }
}

static std::string read_quoted(const std::string& s, size_t& i, const std::string& path) {
    if (i >= s.size() || s[i] != '"') die("expected '\"' in " + path);
    i++;
    std::string out;
    while (i < s.size() && s[i] != '"') {
        if (s[i] == '\\' && i + 1 < s.size()) {
            switch (s[++i]) {
                case '\\': out += '\\'; break;
                case '"':  out += '"';  break;
                case 'n':  out += '\n'; break;
                case 't':  out += '\t'; break;
                case 'r':  out += '\r'; break;
                default:   out += s[i]; break;
            }
            i++;
        } else {
            out += s[i++];
        }
    }
    if (i >= s.size()) die("unterminated string in " + path);
    i++; 
    return out;
}

void Toml::parse(const std::string& path) {
    std::ifstream f(path);
    if (!f) die("cannot open " + path);
    std::stringstream ss;
    ss << f.rdbuf();
    std::string s = ss.str();

    size_t i = 0;
    while (i < s.size()) {
        skip_ws(s, i);
        if (i >= s.size()) break;

        
        size_t k0 = i;
        while (i < s.size() &&
               (std::isalnum(static_cast<unsigned char>(s[i])) || s[i] == '_' ||
                s[i] == '-' || s[i] == '.'))
            i++;
        if (i == k0) die("expected key in " + path);
        std::string key = s.substr(k0, i - k0);

        skip_ws(s, i);
        if (i >= s.size() || s[i] != '=') die("expected '=' after " + key);
        i++;
        skip_ws(s, i);
        if (i >= s.size()) die("missing value for " + key);

        if (s[i] == '"') {
            strs[key] = read_quoted(s, i, path);
        } else if (s[i] == '[') {
            i++;
            std::vector<std::string> v;
            while (i < s.size()) {
                skip_ws(s, i);
                if (i < s.size() && s[i] == ']') { i++; break; }
                if (s[i] != '"') die("expected string in array " + key);
                v.push_back(read_quoted(s, i, path));
                skip_ws(s, i);
                if (i < s.size() && s[i] == ',') i++;
            }
            arrs[key] = std::move(v);
        } else if (std::isdigit(static_cast<unsigned char>(s[i])) || s[i] == '-') {
            size_t n0 = i;
            if (s[i] == '-') i++;
            while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) i++;
            ints[key] = std::stoi(s.substr(n0, i - n0));
        } else {
            die("unsupported value for " + key + " in " + path);
        }
    }
}

} 

static void load() {
    const std::string& path = anvil_toml_path();
    if (path.empty()) die("no anvil.toml in current directory (pass -f <path> to override)");

    Toml t;
    t.parse(path);

    g_cfg.source  = t.str("source");
    g_cfg.build   = t.str("build");
    g_cfg.output  = t.str("output");
    g_cfg.ura_lib = t.str("ura_lib");
    g_cfg.tests   = t.str("tests");

    if (auto v = t.num("llvm")) g_cfg.llvm = *v;
    g_cfg.compile = t.arr("compile");
    g_cfg.flags   = t.arr("flags");
    g_cfg.release = t.arr("release");

    if (auto v = t.num("max_parallel")) g_cfg.max_parallel = *v;
    g_cfg.ignore_ir = t.arr("ignore_ir");

    g_cfg.anvil_repo     = t.str("anvil_repo");
    g_cfg.ura_lib_repo   = t.str("ura_lib_repo");
    g_cfg.extension_repo = t.str("extension_repo");
    g_cfg.projects_dir   = t.str("projects_dir");
    g_cfg.project_url    = t.str("project_url");

    if (!g_cfg.ura_lib.empty())
        setenv("URA_LIB", resolve(g_cfg.ura_lib).c_str(), 1);

    g_loaded = true;
}

const AnvilConfig& config() {
    if (!g_loaded) load();
    return g_cfg;
}

void invalidate_config() {
    g_loaded = false;
    g_cfg    = {};
}

static const std::vector<Command> g_commands = {
    {"build",             cmd_build,             "Compile the ura compiler (flags: --release)"},
    {"test",              cmd_test,              "Run the test suite (optionally filter by folder)"},
    {"copy",              cmd_copy,              "Save a .ura file and its IR as a test reference"},
    {"update-tests",      cmd_update_tests,      "Regenerate every .ll reference file"},
    {"update-projects",   cmd_update_projects,   "Fix first-line path comment in every project .ura"},
    {"examples",          cmd_examples,          "Generate examples.ura from the test corpus"},
    {"indent",            cmd_indent,            "Format all src/**/*.c and *.h files"},
    {"check",             cmd_check,             "Verify build dependencies (clang, llvm, etc.)"},
    {"install",           cmd_install,           "Install missing build dependencies"},
    {"release",           cmd_release,           "Full release pipeline (use --confirm to push)"},
    {"release-anvil",     cmd_release_anvil,     "Sync config/anvil/ to the anvil repo (use --confirm)"},
    {"release-projects",  cmd_release_projects,  "Sync tests/projects/* repos (use --confirm)"},
    {"release-extension", cmd_release_extension, "Sync the VSCode extension repo (use --confirm)"},
    {"shell",             cmd_shell,             "Launch an interactive Linux shell in Docker"},
    {"docker-build",      cmd_docker_build,      "Rebuild the Linux dev-env Docker image"},
    {"leaks",             cmd_leaks,             "Run an executable with LSan enabled"},
    {"reload",            cmd_reload,            "Re-read anvil.toml"},
    {"help",              cmd_help,              "List available commands"},
};

const std::vector<Command>& all_commands() { return g_commands; }
const Command* find_command(const std::string& name) {
    for (const auto& c : g_commands) if (name == c.name) return &c;
    return nullptr;
}

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
