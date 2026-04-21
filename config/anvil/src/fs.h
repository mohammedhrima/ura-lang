#pragma once

#include <functional>
#include <string>

// Recursively walk `root`, calling cb(full_path) for every regular file.
// Directories named "build" (at any depth) are pruned.
void walk(const std::string& root, const std::function<void(const std::string&)>& cb);

// Read a file fully. Returns empty string on error.
std::string read_file(const std::string& path);

// Returns true iff the path exists and is a regular file.
bool is_file(const std::string& path);

// Returns everything up to and including the last '/'. "foo/bar.txt" -> "foo/".
std::string dirname_of(const std::string& path);

// Returns the filename with no extension. "a/b/c.ura" -> "c".
std::string stem(const std::string& path);
