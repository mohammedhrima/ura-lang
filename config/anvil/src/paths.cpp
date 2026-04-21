#include "paths.h"

#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

static std::string g_root;
static std::string g_toml;
static bool        g_resolved = false;

static bool file_exists(const std::string& p) {
    struct stat st;
    return stat(p.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

static std::string realpath_or(const std::string& p) {
    char buf[4096];
    return realpath(p.c_str(), buf) ? std::string(buf) : p;
}

// Discover: is there an anvil.toml in the current working directory?
// Unlike cargo/git, we do NOT walk up — match the `make` convention.
static void resolve_cwd() {
    char buf[4096];
    if (!getcwd(buf, sizeof(buf))) return;
    std::string candidate = std::string(buf) + "/anvil.toml";
    if (file_exists(candidate)) {
        g_toml = candidate;
        g_root = buf;
    }
}

void set_anvil_toml(const std::string& path) {
    if (path.empty() || !file_exists(path)) return;
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
