#pragma once

#include <string>

// Project root = directory containing the anvil.toml we resolved.
// Empty if none was found.
const std::string& project_root();

// Full path to the anvil.toml we're using (empty if none).
const std::string& anvil_toml_path();

// Override the toml path (used by `anvil -f <path>`). Must be called before
// anything reads the config.
void set_anvil_toml(const std::string& path);

// Resolve a project-relative path. Absolute paths pass through.
std::string resolve(const std::string& rel);
