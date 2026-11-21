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
    return dependency_folder(
        project, entry_value(node, 0), entry_value(node, 1), entry_value(node, 2)
    );
}

Node *find_dependency_node(Config *config, char *name) {
    for (int i = 0; i < config->tree->children_count; i++) {
        Node *node = config->tree->children[i];
        if (is_dependency(node) && strcmp2(entry_value(node, 0), name))
            return node;
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
            eprint(
                "%s %s is not installed\n", entry_value(node, 0), entry_value(node, 1)
            );
            help("run: cd %s && ura install\n", config->dir);
            exit(1);
        }
        bool has_config = access(format("%s/ura.conf", place), R_OK) == 0;
        if (has_config && find_config(realpath2(place)) == NULL)
            load_project(place);
    }
}

bool is_version(char *text) {
    int major, minor, patch, end = 0;
    int count = sscanf(text, "%d.%d.%d%n", &major, &minor, &patch, &end);
    return isdigit(text[0]) && count == 3 && text[end] == '\0';
}

int install_dependency(
    char *project, char *name, char *version, char *where, int depth
) {
    char *place = dependency_folder(project, name, version, where);
    bool is_missing = access(place, F_OK) != 0;
    if (is_missing && is_folder(where)) {
        eprint("dependency %s: no folder %s\n", name, place);
        exit(1);
    }
    if (!is_folder(where) && !is_version(version)) {
        eprint("dependency %s: version '%s' is not a number like 0.0.1\n", name, version);
        exit(1);
    }
    bool is_download = is_missing && !is_folder(where);
    char *repo = basename(strdup2(where));
    if (ends_with(repo, ".git"))
        repo = strndup2(repo, strlen(repo) - 4);
    char *cache = format("%s/.ura/packages/%s@%s", home2(), repo, version);
    if (is_download && access(cache, F_OK) != 0) {
        char *clone[] = { "git",      "clone", "-q",  "--depth", "1",
                          "--branch", version, where, cache,     NULL };
        if (run_command(clone, NULL) != 0) {
            eprint("can't get %s %s from %s\n", name, version, where);
            exit(1);
        }
        help("installed %s %s\n", name, version);
    }
    if (is_download) {
        mkdir2(format("%s/dependencies", project));
        symlink2(cache, place);
    }
    bool has_config = access(format("%s/ura.conf", place), R_OK) == 0;
    if (has_config)
        parse_config(place, is_download);
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
        out = format(
            "%s%s%s \"%s\" \"%s\"\n",
            out,
            needs_newline ? "\n" : "",
            entry,
            version,
            where
        );
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
    bool is_same = old && strcmp2(entry_value(old, 1), version) &&
                   strcmp2(entry_value(old, 2), where);
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
        bool has_config =
            !is_missing && access(format("%s/ura.conf", place), R_OK) == 0;
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
            list_project(
                place, format("%s%s", indent, is_last ? "    " : "│   "), depth + 1
            );
    }
}

void run_script(char *name) {
    char *project = find_project();
    char *script = config_value(load_config(project), MACRO_SCRIPT, name);
    if (script == NULL) {
        eprint("no @script \"%s\" in ura.conf\n", name);
        help("add a line: @script \"%s\" \"<command>\"\n", name);
        exit(1);
    }
    if (chdir(project) != 0) {
        eprint("can't enter %s\n", project);
        exit(1);
    }
    int status = system(script);
    exit(WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}


char *find_package(char *name, File *from) {
    char *dir = dirname2(from->path);
    if (!strncmp2(name, "@/", 2)) {
        char *path = format("%s/%s.ura", dir, name);
        return access(path, R_OK) == 0 ? path : NULL;
    }
    char *project = find_folder_with(dir, "ura.conf");
    Config *config = project ? find_config(realpath2(project)) : NULL;
    if (config == NULL)
        return NULL;
    char *slash = strchr(name + 2, '/');
    char *dependency = slash ? strndup2(name + 2, slash - name - 2) : name + 2;
    Node *node = find_dependency_node(config, dependency);
    if (node == NULL)
        return NULL;
    char *place = dependency_dir(config->dir, node);
    Config *own = find_config(realpath2(place));
    char *entry = own ? config_value(own, MACRO_ENTRY, NULL) : NULL;
    char *file = slash ? format("%s.ura", slash + 1) : entry;
    if (file == NULL)
        return NULL;
    char *path = format("%s/%s", place, file);
    return access(path, R_OK) == 0 ? path : NULL;
}

char *template_files[][2] = {
    { "main.ura",
      "use \"@/core\"\n"
      "use \"./src/user\"\n\n"
      "fn main() i32:\n"
      "    name String = user()\n"
      "    if name.is_empty():\n"
      "        name = \"world\"\n\n"
      "    wave unic = '👋'\n"
      "    io::out($\"hello {name} {wave}\\n\")\n\n"
      "    return 0\n" },
    { "src/user.ura",
      "use \"@/posix\"\n\n"
      "fn env(key String) String:\n"
      "    value char[] = getenv(key.chars())\n"
      "    if value == null:\n"
      "        return \"\"\n"
      "    return String::create(value)\n\n"
      "fn user() String:\n"
      "    @if os == \"windows\":\n"
      "        return env(\"USERNAME\")\n"
      "    @elif os == \"linux\":\n"
      "        return env(\"USER\")\n"
      "    @elif os == \"macos\":\n"
      "        return env(\"USER\")\n"
      "    @else:\n"
      "        return \"unknown user\"\n" },
    { NULL, NULL },
};

void init_project(char *name) {
    if (access(format("%s/ura.conf", name), F_OK) == 0) {
        eprint("'%s' already has a ura.conf, replace its starter files? [y/N] ", name);
        char answer[8] = "";
        bool has_answer = fgets(answer, sizeof(answer), stdin) != NULL;
        bool is_yes = has_answer && (answer[0] == 'y' || answer[0] == 'Y');
        if (!is_yes)
            exit(1);
    }
    mkdir2(name);
    mkdir2(format("%s/src", name));
    char *config = format(
        "@name \"%s\"\n"
        "@version \"0.0.1\"\n"
        "@script \"build\" \"ura main.ura -o build/%s\"\n"
        "@script \"run\" \"ura build && ./build/%s\"\n"
        "@dependency \"core\" \"0.0.1\" "
        "\"https://github.com/ura-computing/ura-core\"\n"
        "@dependency \"posix\" \"0.0.1\" "
        "\"https://github.com/ura-computing/ura-posix\"\n",
        name,
        name,
        name
    );
    char *readme = format(
        "# %s\n"
        "\n"
        "A [ura](https://github.com/ura-computing/ura-lang) project.\n"
        "\n"
        "```\n"
        "main.ura        says hello to the user\n"
        "src/user.ura    finds the user name on every OS, used with: use "
        "\"./src/user\"\n"
        "```\n"
        "\n"
        "| command | what it does |\n"
        "|---|---|\n"
        "| `ura install` | clone the dependencies (run it first) |\n"
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
    help("created %s/, run it with: cd %s && ura install && ura run\n", name, name);
}

char *find_project(void) {
    char *project = find_folder_with(getcwd2(), "ura.conf");
    if (project)
        return project;
    eprint("no ura.conf in this folder or above\n");
    help("create a project with: ura init <name>\n");
    exit(1);
}
