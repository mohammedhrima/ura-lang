#include "./template.h"

bool is_package_name(char *name) {
    return name[0] && !strchr(name, '/') && !strstr(name, "..");
}

Config *find_config(char *dir) {
    for (int i = 0; i < ura.configs_count; i++) {
        if (strcmp2(ura.configs[i]->dir, dir))
            return ura.configs[i];
    }
    return NULL;
}

Config *parse_config(char *dir, bool is_fresh_clone) {
    dir = realpath2(dir);
    Config *config = find_config(dir);
    if (config)
        return config;
    File *file = new_file(format("%s/ura.conf", dir));
    file->start = end_of_sources();
    open_file(file);
    append(ura.files, file);
    int first = ura.tokens_count;
    int line = ura.line;
    tokenize(file);
    new_token(LAST, NULL)->space = -TAB;
    config = arena_alloc(1, sizeof(Config));
    config->dir = dir;
    config->tree = new_node(new_token(SCOPE, NULL));
    ura.exe_pos = first;
    enter_scope(config->tree);
    while (peek(0)->type != LAST)
        append(config->tree->children, parse_expr(0));
    exit_scope();
    if (is_fresh_clone && ucg_has_errors()) {
        char *rm[] = { "rm", "-rf", dir, NULL };
        run_command(rm, NULL);
    }
    ucg_check_errors();
    ura.tokens_count = first;
    ura.exe_pos = 0;
    ura.line = line;
    ura.files_count--;
    append(ura.configs, config);
    return config;
}

Config *load_config(char *dir) {
    return parse_config(dir, false);
}

bool is_dependency(Node *node) {
    return node->token->type == MACRO_DEPENDENCY && node->children_count == 3;
}

char *entry_value(Node *node, int index) {
    return node->children[index]->token->chars.value;
}

char *config_value(Config *config, Type macro, char *name) {
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (node->token->type != macro || node->children_count == 0)
            continue;
        if (name == NULL)
            return entry_value(node, 0);
        if (node->children_count == 2 && strcmp2(entry_value(node, 0), name))
            return entry_value(node, 1);
    }
    return NULL;
}

bool is_folder(char *where) {
    return where[0] == '.' || where[0] == '/';
}

char *dependency_folder(char *project, char *name, char *version, char *where) {
    if (is_folder(where))
        return where[0] == '/' ? where : format("%s/%s", project, where);
    return format("%s/dependencies/%s@%s", project, name, version);
}

char *dependency_dir(char *project, Node *node) {
    return dependency_folder(project, entry_value(node, 0), entry_value(node, 1), entry_value(node, 2));
}

Node *find_dependency_node(Config *config, char *name) {
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (is_dependency(node) && strcmp2(entry_value(node, 0), name))
            return node;
    }
    return NULL;
}

char *dependency_main(char *dir, char *name) {
    Config *config = find_config(realpath2(dir));
    char *main = config ? config_value(config, MACRO_MAIN, NULL) : NULL;
    return format("%s/%s", dir, main ? main : format("%s.ura", name));
}

char *find_dependency(Config *config, char *package) {
    char *own = format("%s/%s.ura", config->dir, package);
    if (access(own, R_OK) == 0)
        return own;
    Node *named = find_dependency_node(config, package);
    if (named) {
        char *path = dependency_main(dependency_dir(config->dir, named), package);
        return access(path, R_OK) == 0 ? path : NULL;
    }
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (!is_dependency(node))
            continue;
        char *path = format("%s/%s.ura", dependency_dir(config->dir, node), package);
        if (access(path, R_OK) == 0)
            return path;
    }
    return NULL;
}

void load_project(char *dir) {
    Config *config = load_config(dir);
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (!is_dependency(node))
            continue;
        char *place = dependency_dir(config->dir, node);
        if (access(place, F_OK) != 0) {
            eprint("%s %s is not installed\n", entry_value(node, 0), entry_value(node, 1));
            help("run: cd %s && ura install\n", config->dir);
            exit(1);
        }
        bool has_config = access(format("%s/ura.conf", place), R_OK) == 0;
        if (has_config && find_config(realpath2(place)) == NULL)
            load_project(place);
    }
}

int install_dependency(char *project, char *name, char *version, char *where, int depth) {
    char *place = dependency_folder(project, name, version, where);
    bool is_missing = access(place, F_OK) != 0;
    if (is_missing && is_folder(where)) {
        eprint("dependency %s: no folder %s\n", name, place);
        exit(1);
    }
    if (is_missing) {
        char *tagged[] = { "git", "clone", "-q", "--depth", "1", "--branch", version, where, place, NULL };
        char *latest[] = { "git", "clone", "-q", "--depth", "1", where, place, NULL };
        if (run_command(strcmp2(version, "latest") ? latest : tagged, NULL) != 0) {
            eprint("can't get %s %s from %s\n", name, version, where);
            exit(1);
        }
    }
    bool has_config = access(format("%s/ura.conf", place), R_OK) == 0;
    if (has_config)
        parse_config(place, is_missing && !is_folder(where));
    if (is_missing)
        help("installed %s %s\n", name, version);
    int installed = is_missing;
    if (has_config && depth < 16)
        installed += install_project(place, depth + 1);
    return installed;
}

int install_project(char *dir, int depth) {
    Config *config = load_config(dir);
    int count = 0;
    int installed = 0;
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (!is_dependency(node))
            continue;
        installed += install_dependency(
            config->dir,
            entry_value(node, 0),
            entry_value(node, 1),
            entry_value(node, 2),
            depth
        );
        count++;
    }
    if (depth == 0 && count == 0)
        help("no dependencies in ura.conf\n");
    else if (depth == 0 && installed == 0)
        help("every dependency is installed\n");
    return installed;
}

void set_dependency(char *project, char *name, char *version, char *where) {
    File *file = new_file(format("%s/ura.conf", project));
    open_file(file);
    char *entry = format("@dependency \"%s\"", name);
    char *out = "";
    char *line = file->buff;
    while (*line) {
        char *newline = strchr(line, '\n');
        size_t len = newline ? (size_t)(newline - line + 1) : strlen(line);
        char *row = strndup2(line, len);
        line += len;
        char *code = row + strspn(row, " \t");
        if (!strncmp2(code, entry, strlen(entry)))
            out = format("%s%s", out, row);
    }
    bool needs_newline = out[0] && !ends_with(out, "\n");
    if (version)
        out = format("%s%s%s \"%s\" \"%s\"\n", out, needs_newline ? "\n" : "", entry, version, where);
    write_file(file->path, out);
}

void delete_dependency_folder(char *project, Node *node) {
    if (is_folder(entry_value(node, 2)))
        return;
    char *rm[] = { "rm", "-rf", dependency_dir(project, node), NULL };
    run_command(rm, NULL);
}

void add_package(char *project, char *name, char *version, char *where) {
    if (!is_package_name(name)) {
        eprint("'%s' is not a package name\n", name);
        exit(1);
    }
    Config *config = load_config(project);
    Node *old = find_dependency_node(config, name);
    char *place = dependency_folder(config->dir, name, version, where);
    bool is_same = old && strcmp2(entry_value(old, 1), version) && strcmp2(entry_value(old, 2), where);
    if (is_same && access(place, F_OK) == 0) {
        help("%s %s is already installed\n", name, version);
        return;
    }
    install_dependency(config->dir, name, version, where, 0);
    if (is_same)
        return;
    if (old && !strcmp2(dependency_dir(config->dir, old), place))
        delete_dependency_folder(config->dir, old);
    set_dependency(config->dir, name, version, where);
    if (old)
        help("updated %s %s -> %s in ura.conf\n", name, entry_value(old, 1), version);
    else
        help("added %s %s to ura.conf\n", name, version);
}

void remove_dependency(char *project, char *name) {
    Config *config = load_config(project);
    Node *old = find_dependency_node(config, name);
    if (old == NULL) {
        eprint("'%s' is not a dependency in ura.conf\n", name);
        exit(1);
    }
    delete_dependency_folder(config->dir, old);
    set_dependency(config->dir, name, NULL, NULL);
    help("removed %s\n", name);
}

void list_project(char *dir, char *indent, int depth) {
    Config *config = load_config(dir);
    if (depth == 0) {
        char *name = config_value(config, MACRO_NAME, NULL);
        char *version = config_value(config, MACRO_VERSION, NULL);
        help("%s %s\n", name ? name : "(no @name)", version ? version : "");
    }
    Node *last = NULL;
    for (int i = 0; i < config->tree->children_count; i++) {
        if (is_dependency(config->tree->children[i]))
            last = config->tree->children[i];
    }
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (!is_dependency(node))
            continue;
        char *name = entry_value(node, 0);
        char *version = entry_value(node, 1);
        char *where = entry_value(node, 2);
        char *place = dependency_dir(config->dir, node);
        bool is_missing = access(place, F_OK) != 0;
        bool has_config = !is_missing && access(format("%s/ura.conf", place), R_OK) == 0;
        char *kind = is_folder(where) ? "folder" : where;
        bool is_last = node == last;
        help(
            "%s%s%s %s  %s  %s%s\n",
            indent,
            is_last ? "└── " : "├── ",
            name,
            version ? version : "",
            kind,
            place,
            is_missing ? "  missing" : ""
        );
        if (has_config && depth < 16)
            list_project(place, format("%s%s", indent, is_last ? "    " : "│   "), depth + 1);
    }
}

bool run_script(char *name) {
    char *project = find_folder_with(getcwd2(), "ura.conf");
    char *script = project ? config_value(load_config(project), MACRO_SCRIPT, name) : NULL;
    if (script == NULL)
        return false;
    if (chdir(project) != 0) {
        eprint("can't enter %s\n", project);
        exit(1);
    }
    int status = system(script);
    exit(WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}


char *find_package(char *name, File *from) {
    if (!strncmp2(name, "@/", 2)) {
        char *path = format("%s/%s.ura", dirname2(from->path), name);
        return access(path, R_OK) == 0 ? path : NULL;
    }
    char *project = find_folder_with(from ? dirname2(from->path) : getcwd2(), "ura.conf");
    Config *config = project ? find_config(project) : NULL;
    return config ? find_dependency(config, name + 2) : NULL;
}

void init_project(char *name) {
    if (access(format("%s/ura.conf", name), F_OK) == 0) {
        eprint("'%s' already has a ura.conf\n", name);
        exit(1);
    }
    mkdir2(name);
    mkdir2(format("%s/src", name));
    char *config = format(
        "@name \"%s\"\n"
        "@version \"0.1.0\"\n"
        "@main \"main.ura\"\n",
        name
    );
    char *readme = format(
        "# %s\n"
        "\n"
        "A [ura](https://github.com/mohammedhrima/ura-lang) project.\n"
        "\n"
        "```\n"
        "main.ura          runs every demo\n"
        "src/donut.ura     a spinning donut, with sin and cos from C\n"
        "src/shapes.ura    structs, operators and struct from\n"
        "src/errors.ura    try, catch and throw\n"
        "src/colors.ura    enum + match for terminal colours\n"
        "```\n"
        "\n"
        "| command | what it does |\n"
        "|---|---|\n"
        "| `ura run` | build `main.ura` and run it |\n"
        "| `ura build` | build it into `build/%s` |\n"
        "| `ura install <name> <version> <repo url>` | add a dependency |\n"
        "| `ura list` | show the dependencies |\n",
        name,
        name
    );
    char *files[][2] = {
        { "ura.conf", config },
        { "README.md", readme },
        { ".gitignore", "build/\ndependencies/\n" },
    };
    for (size_t i = 0; i < sizeof(files) / sizeof(*files); i++)
        write_file(format("%s/%s", name, files[i][0]), files[i][1]);
    for (int i = 0; template_files[i][0]; i++)
        write_file(format("%s/%s", name, template_files[i][0]), template_files[i][1]);
    help("created %s/, run it with: cd %s && ura run\n", name, name);
}

char *find_project(void) {
    char *project = find_folder_with(getcwd2(), "ura.conf");
    if (project)
        return project;
    bool is_old = access("ura.project", R_OK) == 0;
    eprint("no ura.conf in this folder or above\n");
    if (is_old)
        help("ura.project is now ura.conf, with lines like: @name \"<name>\"\n");
    else
        help("create a project with: ura init <name>\n");
    exit(1);
}

void build_project(char *project) {
    load_project(project);
    Config *config = find_config(realpath2(project));
    char *name = config_value(config, MACRO_NAME, NULL);
    if (name == NULL) {
        eprint("ura.conf has no name\n");
        help("add a line: @name \"<name>\"\n");
        exit(1);
    }
    if (chdir(project) != 0) {
        eprint("can't enter %s\n", project);
        exit(1);
    }
    char *main = config_value(config, MACRO_MAIN, NULL);
    ucg_bin_path(format("build/%s", name));
    append(ura.files, new_file(main ? main : "main.ura"));
}
