#pragma once

#include <string>
#include <vector>

struct AnvilConfig {
    // paths (all relative to anvil.toml's directory)
    std::string source;
    std::string build;
    std::string output;
    std::string ura_lib;
    std::string tests;

    // compiler
    int                      llvm = 14;
    std::vector<std::string> compile;       // filenames relative to `source`

    // flags
    std::vector<std::string> warn;
    std::vector<std::string> san;
    std::vector<std::string> release;

    // tests
    int                      max_parallel = 3;
    std::vector<std::string> ignore_ir;

    // sync
    std::string anvil_repo;
    std::string ura_lib_repo;
    std::string extension_repo;
    std::string projects_dir;
    std::string project_url;
};

const AnvilConfig& config();
void               invalidate_config();
