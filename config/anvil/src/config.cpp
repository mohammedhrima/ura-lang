#include "config.h"
#include "color.h"
#include "paths.h"

extern "C" {
#include "toml.h"
}

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <optional>

static AnvilConfig g_cfg;
static bool        g_loaded = false;

static std::string take_string(toml_datum_t d) {
    if (!d.ok) return {};
    std::string s = d.u.s;
    free(d.u.s);
    return s;
}

static std::string str_at(toml_table_t* t, const char* key) {
    if (!t) return {};
    return take_string(toml_string_in(t, key));
}

static std::optional<int> int_at(toml_table_t* t, const char* key) {
    if (!t) return std::nullopt;
    auto d = toml_int_in(t, key);
    if (!d.ok) return std::nullopt;
    return static_cast<int>(d.u.i);
}

static std::vector<std::string> str_array_at(toml_table_t* t, const char* key) {
    std::vector<std::string> out;
    if (!t) return out;
    toml_array_t* arr = toml_array_in(t, key);
    if (!arr) return out;
    int n = toml_array_nelem(arr);
    out.reserve(n);
    for (int i = 0; i < n; i++) {
        auto d = toml_string_at(arr, i);
        if (d.ok) { out.emplace_back(d.u.s); free(d.u.s); }
    }
    return out;
}

static void die(const std::string& msg) {
    fprintf(stderr, ANSI_RED "anvil config: %s" ANSI_RESET "\n", msg.c_str());
    std::exit(2);
}

static void load() {
    const std::string& path = anvil_toml_path();
    if (path.empty()) die("no anvil.toml in current directory (pass -f <path> to override)");

    std::unique_ptr<FILE, int(*)(FILE*)> fp(fopen(path.c_str(), "r"), fclose);
    if (!fp) die("cannot open " + path);

    char errbuf[256] = {};
    toml_table_t* root = toml_parse_file(fp.get(), errbuf, sizeof(errbuf));
    if (!root) die(std::string("parse error: ") + errbuf);

    g_cfg.source       = str_at(root, "source");
    g_cfg.build        = str_at(root, "build");
    g_cfg.output       = str_at(root, "output");
    g_cfg.ura_lib      = str_at(root, "ura_lib");
    g_cfg.tests        = str_at(root, "tests");

    if (auto v = int_at(root, "llvm")) g_cfg.llvm = *v;
    g_cfg.compile      = str_array_at(root, "compile");

    g_cfg.warn         = str_array_at(root, "warn");
    g_cfg.san          = str_array_at(root, "san");
    g_cfg.release      = str_array_at(root, "release");

    if (auto v = int_at(root, "max_parallel")) g_cfg.max_parallel = *v;
    g_cfg.ignore_ir    = str_array_at(root, "ignore_ir");

    g_cfg.anvil_repo     = str_at(root, "anvil_repo");
    g_cfg.ura_lib_repo   = str_at(root, "ura_lib_repo");
    g_cfg.extension_repo = str_at(root, "extension_repo");
    g_cfg.projects_dir   = str_at(root, "projects_dir");
    g_cfg.project_url    = str_at(root, "project_url");

    toml_free(root);
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
