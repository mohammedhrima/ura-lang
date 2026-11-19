#include <fcntl.h>
#include <sys/wait.h>

int run_command(char **argv, char *out_path) {
    pid_t pid = fork();
    if (pid == 0) {
        int quiet = open("/dev/null", O_WRONLY);
        int out = out_path ? open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644) : quiet;
        dup2(out, 1);
        dup2(quiet, 2);
        execvp(argv[0], argv);
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

bool is_package_name(char *name) {
    return name[0] && !strchr(name, '/') && !strstr(name, "..");
}

void install_package(char *spec) {
    char *name = strdup2(spec);
    char *version = strchr(name, '@');
    if (version)
        *version++ = '\0';
    if (!is_package_name(name)) {
        eprint("'%s' is not a package name\n", spec);
        exit(1);
    }
    char *home = getenv("HOME");
    char *url = getenv("URA_REGISTRY");
    if (url == NULL)
        url = "https://github.com/mohammedhrima/ura-lib";
    char *registry = format("%s/.ura/registry", home);
    mkdir2(format("%s/.ura", home));
    mkdir2(format("%s/.ura/lib", home));
    if (access(registry, F_OK) != 0) {
        char *clone[] = { "git", "clone", "-q", url, registry, NULL };
        if (run_command(clone, NULL) != 0) {
            eprint("can't get the packages from %s\n", url);
            exit(1);
        }
    } else {
        char *fetch[] = { "git", "-C", registry, "fetch", "-q", "--tags", NULL };
        run_command(fetch, NULL);
    }

    char *folder = version ? version : "latest";
    char *ref = version ? format("%s-%s", name, version) : "origin/HEAD";
    char *dir = format("%s/.ura/lib/%s", home, name);
    mkdir2(dir);
    mkdir2(format("%s/%s", dir, folder));
    char *path = format("%s/%s/%s.ura", dir, folder, name);
    char *show[] = { "git", "-C", registry, "show", format("%s:%s.ura", ref, name), NULL };
    if (run_command(show, path) != 0) {
        remove(path);
        rmdir(format("%s/%s", dir, folder));
        rmdir(dir);
        eprint("no package '%s'%s\n", name, version ? format(" at version %s", version) : "");
        exit(1);
    }
    char *current = format("%s/current", dir);
    unlink(current);
    symlink(folder, current);
    help("installed %s %s\n", name, folder);

    File *file = new_file(path);
    open_file(file);
    for (char *use = strstr(file->buff, "use \"@/"); use; use = strstr(use + 1, "use \"@/")) {
        char *start = use + strlen("use \"@/");
        char *end = strchr(start, '"');
        if (end == NULL)
            break;
        char *dep = strndup2(start, end - start);
        char *dep_version = strchr(dep, '@');
        char *dep_name = strndup2(dep, dep_version ? dep_version - dep : strlen(dep));
        char *dep_folder = dep_version ? dep_version + 1 : "current";
        char *dep_path = format("%s/.ura/lib/%s/%s/%s.ura", home, dep_name, dep_folder, dep_name);
        if (access(dep_path, R_OK) != 0)
            install_package(dep);
    }
}

void remove_package(char *name) {
    char *dir = format("%s/.ura/lib/%s", getenv("HOME"), name);
    if (!is_package_name(name) || access(dir, F_OK) != 0) {
        eprint("'%s' is not installed\n", name);
        exit(1);
    }
    char *rm[] = { "rm", "-rf", dir, NULL };
    run_command(rm, NULL);
    help("removed %s\n", name);
}


char *find_package(char *name, File *from) {
    if (!strncmp2(name, "@/", 2)) {
        char *path = format("%s/%s.ura", dirname2(from->path), name);
        return access(path, R_OK) == 0 ? path : NULL;
    }
    char *package = strdup2(name + 2);
    char *version = strchr(package, '@');
    if (version)
        *version++ = '\0';
    char *dirs[] = { getenv("URA_LIB"), format("%s/.ura/lib", getenv("HOME")) };
    for (int i = 0; i < 2; i++) {
        if (dirs[i] == NULL)
            continue;
        char *paths[] = {
            format(
                "%s/%s/%s/%s.ura",
                dirs[i],
                package,
                version ? version : "current",
                package
            ),
            version ? NULL : format("%s/%s.ura", dirs[i], package),
        };
        for (int j = 0; j < 2; j++) {
            if (paths[j] == NULL || access(paths[j], R_OK) != 0)
                continue;
            return paths[j];
        }
    }
    return NULL;
}
