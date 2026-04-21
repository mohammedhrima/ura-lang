#pragma once

#include <string>
#include <vector>

struct CaptureResult {
    int         code = 0;
    std::string stdout_;
    std::string stderr_;
};

int           run_stream(const std::vector<std::string>& argv);
CaptureResult run_capture(const std::vector<std::string>& argv);
int           run_sh(const std::string& cmd);
bool          has_tool(const std::string& tool);
