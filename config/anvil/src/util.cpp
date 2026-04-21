#include "util.h"
#include "exec.h"

#include <ctime>
#include <string>
#include <sys/utsname.h>

const std::string& detect_os() {
    static std::string cached;
    if (!cached.empty()) return cached;
    struct utsname u{};
    uname(&u);
    std::string sys = u.sysname;
    if (sys == "Darwin") { cached = "macos"; return cached; }
    if (sys == "Linux") {
        if (has_tool("apt-get"))  { cached = "linux-apt";    return cached; }
        if (has_tool("pacman"))   { cached = "linux-pacman"; return cached; }
        if (has_tool("dnf"))      { cached = "linux-dnf";    return cached; }
        cached = "linux";
        return cached;
    }
    cached = "unknown";
    return cached;
}

std::string today_string() {
    std::time_t t = std::time(nullptr);
    std::tm* tm  = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
    return buf;
}
