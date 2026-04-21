#include "exec.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

extern char** environ;

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
    if (rc != 0) return 127;
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
    int out_pipe[2]{}, err_pipe[2]{};
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
    close(out_pipe[1]); close(err_pipe[1]);
    if (rc != 0) { close(out_pipe[0]); close(err_pipe[0]); r.code = 127; return r; }

    r.stdout_ = slurp(out_pipe[0]);
    r.stderr_ = slurp(err_pipe[0]);
    close(out_pipe[0]); close(err_pipe[0]);

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) { r.code = -1; return r; }
    r.code = finalize(status);
    return r;
}

int run_sh(const std::string& cmd) {
    return run_stream({"/bin/sh", "-c", cmd});
}

bool has_tool(const std::string& tool) {
    CaptureResult r = run_capture({"/bin/sh", "-c", "command -v '" + tool + "' >/dev/null 2>&1"});
    return r.code == 0;
}
