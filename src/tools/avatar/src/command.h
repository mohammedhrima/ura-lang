#pragma once

#include <string>
#include <vector>

struct Command {
    const char* name;
    int       (*fn)(const std::vector<std::string>& args);
    const char* summary;
};

const Command*                     find_command(const std::string& name);
const std::vector<Command>&        all_commands();
