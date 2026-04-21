#include "fs.h"

#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

void walk(const std::string& root, const std::function<void(const std::string&)>& cb) {
    DIR* d = opendir(root.c_str());
    if (!d) return;
    while (dirent* e = readdir(d)) {
        if (std::strcmp(e->d_name, ".") == 0 || std::strcmp(e->d_name, "..") == 0) continue;
        std::string full = root + "/" + e->d_name;
        struct stat st;
        if (lstat(full.c_str(), &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) {
            if (std::strcmp(e->d_name, "build") == 0) continue;     // prune
            walk(full, cb);
        } else if (S_ISREG(st.st_mode)) {
            cb(full);
        }
    }
    closedir(d);
}

std::string read_file(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return {};
    std::string out;
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) out.append(buf, buf + n);
    fclose(f);
    return out;
}

bool is_file(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

std::string dirname_of(const std::string& path) {
    auto slash = path.find_last_of('/');
    if (slash == std::string::npos) return "./";
    return path.substr(0, slash + 1);
}

std::string stem(const std::string& path) {
    auto slash = path.find_last_of('/');
    std::string base = slash == std::string::npos ? path : path.substr(slash + 1);
    auto dot = base.find_last_of('.');
    return dot == std::string::npos ? base : base.substr(0, dot);
}
