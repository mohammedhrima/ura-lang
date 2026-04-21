#pragma once

#include <string>

// "macos" / "linux-apt" / "linux-pacman" / "linux-dnf" / "linux" / "unknown"
const std::string& detect_os();

// e.g. "2026-04-21"
std::string today_string();
