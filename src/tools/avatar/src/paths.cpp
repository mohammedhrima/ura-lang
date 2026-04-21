#include "paths.h"

#include <string>
#include <sys/stat.h>
#include <unistd.h>

static std::string g_root;
static std::string g_toml;

static bool file_exists(const std::string& p) {
    struct stat st;
    return stat(p.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

static std::string discover_root() {
    char buf[4096];
    if (!getcwd(buf, sizeof(buf))) return "";
    std::string cur = buf;
    while (true) {
        if (file_exists(cur + "/ura.toml")) return cur;
        if (cur.empty() || cur == "/") break;
        auto slash = cur.find_last_of('/');
        if (slash == std::string::npos) break;
        cur = slash == 0 ? "/" : cur.substr(0, slash);
    }
    return "";
}

const std::string& project_root() {
    if (g_root.empty()) g_root = discover_root();
    return g_root;
}

const std::string& ura_toml_path() {
    if (g_toml.empty()) {
        const auto& r = project_root();
        if (!r.empty()) g_toml = r + "/ura.toml";
    }
    return g_toml;
}

std::string resolve(const std::string& rel) {
    if (!rel.empty() && rel[0] == '/') return rel;
    const auto& r = project_root();
    return r.empty() ? rel : (r + "/" + rel);
}
