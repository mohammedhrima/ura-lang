// release, release-projects, release-extension — repo syncing.
// Default: dry-run (prints the plan). Pass --confirm to actually push.

#include "../color.h"
#include "../config.h"
#include "../exec.h"
#include "../fs.h"
#include "../paths.h"
#include "../util.h"

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

// Re-implementation of config.sh:_sync_repo. Returns 0 on success.
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

    // mktemp + trap rm -rf behavior, all in one shell line so we get cleanup.
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

} // namespace

// ---- release-anvil --------------------------------------------------------
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

// ---- release-extension ----------------------------------------------------
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

// ---- release-projects -----------------------------------------------------
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

// Build avatar (release mode) and return its binary path, or empty on failure.
static std::string build_avatar() {
    printf(ANSI_YELLOW "Building avatar..." ANSI_RESET "\n");
    std::string dir = project_root() + "/src/tools/avatar";
    std::string cmd = "make -C '" + dir + "' clean && make -C '" + dir + "'";
    if (run_sh(cmd) != 0) return {};
    return dir + "/avatar";
}

// Create build/ura-release-<date>.tar.gz with ura + avatar + ura-lib/.
static int package_release(const std::string& ura_bin,
                           const std::string& avatar_bin,
                           const std::string& ura_lib_dir)
{
    std::string tarball = project_root() + "/build/ura-release-" + today_string() + ".tar.gz";
    // Stage files in a scratch dir so the tarball has clean layout.
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

// ---- release --------------------------------------------------------------
// Full release pipeline:
//   1. build release ura compiler
//   2. build release avatar
//   3. package tarball
//   4. sync ura-lib + projects + extension (only with --confirm)
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
