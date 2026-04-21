#include "config.h"
#include "color.h"
#include "paths.h"

extern "C" {
#include "toml.h"
}

#include <cstdio>
#include <cstdlib>
#include <memory>

static UraConfig g_cfg;
static bool      g_loaded = false;

static std::string take_str(toml_datum_t d) {
    if (!d.ok) return {};
    std::string s = d.u.s;
    free(d.u.s);
    return s;
}

static void die(const std::string& msg) {
    fprintf(stderr, ANSI_RED "avatar: %s" ANSI_RESET "\n", msg.c_str());
    std::exit(2);
}

static void load() {
    const std::string& path = ura_toml_path();
    if (path.empty()) die("not a ura project (no ura.toml found in this directory or any parent)");

    std::unique_ptr<FILE, int(*)(FILE*)> fp(fopen(path.c_str(), "r"), fclose);
    if (!fp) die("cannot open " + path);

    char errbuf[256] = {};
    toml_table_t* root = toml_parse_file(fp.get(), errbuf, sizeof(errbuf));
    if (!root) die(std::string("parse error in ura.toml: ") + errbuf);

    auto* proj  = toml_table_in(root, "project");
    auto* build = toml_table_in(root, "build");
    auto* deps  = toml_table_in(root, "dependencies");

    g_cfg.name     = take_str(toml_string_in(proj,  "name"));
    g_cfg.version  = take_str(toml_string_in(proj,  "version"));
    g_cfg.src      = take_str(toml_string_in(build, "src"));
    g_cfg.output   = take_str(toml_string_in(build, "output"));
    g_cfg.optimize = take_str(toml_string_in(build, "optimize"));

    if (g_cfg.src.empty())    g_cfg.src    = "src/main.ura";
    if (g_cfg.output.empty()) g_cfg.output = "target/" + (g_cfg.name.empty() ? "app" : g_cfg.name);

    if (deps) {
        int nkv = toml_table_nkval(deps);
        int ntb = toml_table_ntab(deps);
        // Inline-table form: name = { git = "..." }
        for (int i = 0; i < ntb; i++) {
            const char* key = toml_key_in(deps, nkv + i);
            if (!key) continue;
            auto* t = toml_table_in(deps, key);
            if (!t) continue;
            Dep d;
            d.name = key;
            d.git  = take_str(toml_string_in(t, "git"));
            if (!d.git.empty()) g_cfg.deps.push_back(d);
        }
        // Simple-string form: name = "git-url"
        for (int i = 0; i < nkv; i++) {
            const char* key = toml_key_in(deps, i);
            if (!key) continue;
            Dep d;
            d.name = key;
            d.git  = take_str(toml_string_in(deps, key));
            if (!d.git.empty()) g_cfg.deps.push_back(d);
        }
    }

    toml_free(root);
    g_loaded = true;
}

const UraConfig& ura_config() {
    if (!g_loaded) load();
    return g_cfg;
}
