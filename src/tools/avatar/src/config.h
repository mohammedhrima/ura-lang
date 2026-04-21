#pragma once

#include <string>
#include <vector>

struct Dep {
    std::string name;
    std::string git;   // URL; for v1 this is the only source
};

struct UraConfig {
    // [project]
    std::string name;
    std::string version;

    // [build]
    std::string src;
    std::string output;
    std::string optimize;       // O0 / O1 / O2 / O3 / Os / Oz  (optional)

    // [dependencies]
    std::vector<Dep> deps;
};

const UraConfig& ura_config();
