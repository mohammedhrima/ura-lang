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
        if (S_ISDIR(st.st_mode)) walk(full, cb);
        else if (S_ISREG(st.st_mode)) cb(full);
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

bool is_dir(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

int mkdir_p(const std::string& path) {
    std::string acc;
    for (std::size_t i = 0; i < path.size(); i++) {
        acc += path[i];
        if (path[i] == '/' && !acc.empty()) mkdir(acc.c_str(), 0755);
    }
    return mkdir(path.c_str(), 0755);
}
