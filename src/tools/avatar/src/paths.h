#pragma once

#include <string>

// Project root = nearest ancestor (incl. CWD) containing `ura.toml`.
// Empty string if not found.
const std::string& project_root();

// Full path to the project's ura.toml (empty if no root found).
const std::string& ura_toml_path();

// Resolve a project-relative path. Absolute paths pass through unchanged.
std::string resolve(const std::string& rel);
