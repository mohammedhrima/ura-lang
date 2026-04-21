#pragma once

#include <string>
#include <vector>

// Result of capturing a process's output.
struct CaptureResult {
    int         code = 0;
    std::string stdout_;
    std::string stderr_;
};

// Run `argv[0] argv[1:]` inheriting stdin/stdout/stderr from the current
// process. Returns the child's exit code (128+signo if terminated by signal).
int run_stream(const std::vector<std::string>& argv);

// Same, but capture stdout and stderr into the result.
CaptureResult run_capture(const std::vector<std::string>& argv);

// Split `s` on whitespace. Does NOT honor quotes (llvm-config doesn't need it).
std::vector<std::string> split_ws(const std::string& s);

// Run a raw shell command via /bin/sh -c, streaming output. Returns exit code.
int run_sh(const std::string& cmd);

// True iff `tool` is found on PATH (`command -v`).
bool has_tool(const std::string& tool);
