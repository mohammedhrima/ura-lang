#pragma once

#include <functional>
#include <string>

void        walk(const std::string& root, const std::function<void(const std::string&)>& cb);
std::string read_file(const std::string& path);
bool        is_file(const std::string& path);
bool        is_dir(const std::string& path);
int         mkdir_p(const std::string& path);
