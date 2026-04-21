// shell, docker-build — Docker integration for running the Linux x86_64
// toolchain (ASan works here, unlike on macOS 26).

#include "../color.h"
#include "../exec.h"
#include "../paths.h"

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

} // namespace

// ---- docker-build ---------------------------------------------------------
int cmd_docker_build(const std::vector<std::string>&) {
    std::string cmd = "docker build --platform linux/amd64 -t ";
    cmd += IMAGE; cmd += " '"; cmd += project_root(); cmd += "'";
    return run_sh(cmd);
}

// ---- shell ----------------------------------------------------------------
// Launch an interactive Linux shell inside the ura-dev container with the
// repo mounted at /workspace.
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
