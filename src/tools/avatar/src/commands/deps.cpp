// add, update — dependency plumbing.  v1 supports git URLs only.

#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../fs.h"
#include "../paths.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

// ---- add ------------------------------------------------------------------
// Appends a `<name> = { git = "<url>" }` line to the project's ura.toml.
int cmd_add(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        fprintf(stderr, ANSI_RED "usage: avatar add <name> <git-url>" ANSI_RESET "\n");
        return 1;
    }
    const std::string& name = args[0];
    const std::string& url  = args[1];
    (void)ura_config();  // surface missing ura.toml before we try to write
    const std::string& path = ura_toml_path();

    // Simple approach: read file, find [dependencies] section, append. If no
    // section, add one. Keeps formatting intact for the user's existing lines.
    std::string body = read_file(path);
    std::size_t sec = body.find("[dependencies]");
    std::string line = name + " = { git = \"" + url + "\" }\n";

    if (sec == std::string::npos) {
        if (!body.empty() && body.back() != '\n') body += '\n';
        body += "\n[dependencies]\n" + line;
    } else {
        // Insert after the section's line-end.
        std::size_t eol = body.find('\n', sec);
        if (eol == std::string::npos) eol = body.size();
        body.insert(eol + 1, line);
    }

    FILE* f = fopen(path.c_str(), "w");
    if (!f) { fprintf(stderr, ANSI_RED "add: cannot write %s" ANSI_RESET "\n", path.c_str()); return 1; }
    fwrite(body.data(), 1, body.size(), f);
    fclose(f);

    printf(ANSI_GREEN "added" ANSI_RESET " %s = %s\n", name.c_str(), url.c_str());
    printf("  run " ANSI_CYAN "avatar update" ANSI_RESET " to fetch it\n");
    return 0;
}

// ---- update ---------------------------------------------------------------
// Clone each declared dep to target/deps/<name>/, or `git pull` if already there.
// Writes ura.lock with pinned commit hashes.
int cmd_update(const std::vector<std::string>&) {
    const auto& cfg = ura_config();
    if (cfg.deps.empty()) {
        printf(ANSI_DIM "no dependencies declared" ANSI_RESET "\n");
        return 0;
    }
    if (!has_tool("git")) {
        fprintf(stderr, ANSI_RED "update: git not found" ANSI_RESET "\n");
        return 1;
    }
    std::string deps_dir = resolve("target/deps");
    mkdir_p(deps_dir);

    std::vector<std::pair<std::string, std::string>> locked;
    for (const auto& d : cfg.deps) {
        std::string dest = deps_dir + "/" + d.name;
        if (is_dir(dest + "/.git")) {
            printf(ANSI_YELLOW "updating" ANSI_RESET " %s\n", d.name.c_str());
            int rc = run_sh("git -C '" + dest + "' pull --ff-only --quiet");
            if (rc != 0) { fprintf(stderr, ANSI_RED "update failed for %s" ANSI_RESET "\n", d.name.c_str()); return rc; }
        } else {
            printf(ANSI_YELLOW "cloning" ANSI_RESET " %s\n", d.name.c_str());
            int rc = run_sh("git clone --quiet '" + d.git + "' '" + dest + "'");
            if (rc != 0) { fprintf(stderr, ANSI_RED "clone failed for %s" ANSI_RESET "\n", d.name.c_str()); return rc; }
        }
        CaptureResult r = run_capture({"git", "-C", dest, "rev-parse", "HEAD"});
        std::string sha = r.stdout_;
        while (!sha.empty() && (sha.back() == '\n' || sha.back() == '\r')) sha.pop_back();
        locked.push_back({d.name, sha});
    }

    std::string lock = "# avatar lock file — do not edit by hand\n\n";
    for (const auto& [n, s] : locked) lock += n + " = \"" + s + "\"\n";
    std::string lockpath = resolve("ura.lock");
    FILE* f = fopen(lockpath.c_str(), "w");
    if (f) { fwrite(lock.data(), 1, lock.size(), f); fclose(f); }
    printf(ANSI_GREEN "locked" ANSI_RESET " %zu dep(s) -> %s\n", locked.size(), lockpath.c_str());
    return 0;
}
