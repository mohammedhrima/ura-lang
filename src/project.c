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

char *template_files[][2] = {
    { "ura.conf", "@name \"template\"\n@version \"0.1.0\"\n@main \"main.ura\"\n" },
    { "main.ura", "use \"./src/colors\"\nuse \"./src/donut\"\nuse \"./src/shapes\"\nuse \"./src/errors\"\n\nfn main() i32:\n    donut()\n    shapes()\n    errors()\n    paint(Color::CYAN)\n    output(\"\\n  edit main.ura or src/, then: ura run\\n\\n\")\n    reset()\n    return 0\n" },
    { "src/colors.ura", "enum Color:\n\tRED, GREEN, YELLOW, BLUE,\n\tMAGENTA, CYAN, WHITE, GREY\n\nfn code(c Color) i32:\n    match c:\n        case Color::RED:     return 31\n        case Color::GREEN:   return 32\n        case Color::YELLOW:  return 33\n        case Color::BLUE:    return 34\n        case Color::MAGENTA: return 35\n        case Color::CYAN:    return 36\n        case Color::GREY:    return 90\n        default:             return 97\n    return 0\n\nfn paint(c Color): output(27 as char, \"[1;\", code(c), \"m\")\nfn reset(): output(27 as char, \"[0m\")\n\nfn title(text char[]):\n    paint(Color::CYAN)\n    output(\"\\n  \", text, \"\\n\")\n    reset()\n    paint(Color::GREY)\n    output(\"  ----------------------------------------\\n\")\n    reset()\n" },
    { "src/donut.ura", "link \"m\"\nproto sin(x f64) f64\nproto cos(x f64) f64\nproto usleep(usec u32) i32\n\nW i32 = 80\nH i32 = 20\n\nfn glow(light i32) Color:\n    if light < 3: return Color::BLUE\n    if light < 6: return Color::CYAN\n    if light < 9: return Color::WHITE\n    return Color::YELLOW\n\nfn donut():\n    title(\"a spinning donut: sin and cos straight from C\")\n    screen char[] = memory_alloc(W * H) as char[]\n    depth f64[] = memory_alloc(W * H * 8) as f64[]\n    light i32[] = memory_alloc(W * H * 4) as i32[]\n    shades char[] = \".,-~:;=!*#$@\"\n    a f64 = 0.0\n    b f64 = 0.0\n    frame i32 = 0\n    while frame < 140:\n        i i32 = 0\n        while i < W * H:\n            screen[i] = ' '\n            depth[i] = 0.0\n            i += 1\n        theta f64 = 0.0\n        while theta < 6.28:\n            phi f64 = 0.0\n            while phi < 6.28:\n                sp f64 = sin(phi)\n                cp f64 = cos(phi)\n                st f64 = sin(theta)\n                ct f64 = cos(theta)\n                circle f64 = ct + 2.0\n                d f64 = 1.0 / (sp * circle * sin(a) + st * cos(a) + 5.0)\n                t f64 = sp * circle * cos(a) - st * sin(a)\n                x i32 = (40.0 + 30.0 * d * (cp * circle * cos(b) - t * sin(b))) as i32\n                y i32 = (10.0 + 15.0 * d * (cp * circle * sin(b) + t * cos(b))) as i32\n                n i32 = (8.0 * ((st * sin(a) - sp * ct * cos(a)) * cos(b) - sp * ct * sin(a) - st * cos(a) - cp * ct * sin(b))) as i32\n                inside bool = y >= 0 and y < H and x >= 0 and x < W\n                if inside and d > depth[x + W * y]:\n                    depth[x + W * y] = d\n                    if n < 0:\n                        n = 0\n                    screen[x + W * y] = shades[n]\n                    light[x + W * y] = n\n                phi += 0.02\n            theta += 0.07\n        row i32 = 0\n        while row < H:\n            output(\"  \")\n            col i32 = 0\n            while col < W:\n                at i32 = col + W * row\n                if screen[at] != ' ': paint(glow(light[at]))\n                output(screen[at])\n                col += 1\n            reset()\n            output(\"\\n\")\n            row += 1\n        if frame < 139: output(27 as char, \"[\", H, \"A\")\n        a += 0.04\n        b += 0.02\n        usleep(30000)\n        frame += 1\n    memory_free(screen as i8[])\n    memory_free(depth as i8[])\n    memory_free(light as i8[])\n" },
    { "src/errors.ura", "struct Overdraft:\n    wanted i32\n    balance i32\n\nfn withdraw(balance i32, amount i32) i32:\n    if amount > balance:\n        e Overdraft = {\n            wanted: amount,\n            balance: balance,\n        }\n        throw e\n    return balance - amount\n\nfn errors():\n    title(\"try, catch and throw\")\n    balance i32 = 100\n    amount i32 = 30\n    while amount < 130:\n        try:\n            balance = withdraw(balance, amount)\n            paint(Color::GREEN)\n            output(\"  took \", amount, \", \", balance, \" left\\n\")\n        catch e Overdraft:\n            paint(Color::RED)\n            output(\"  can't take \", e.wanted, \": only \", e.balance, \" left\\n\")\n        reset()\n        amount += 40\n" },
    { "src/shapes.ura", "struct Vec2:\n    x f64\n    y f64\n\n    operator +(o &Vec2) Vec2:\n        return {\n            x: self.x + o.x,\n            y: self.y + o.y,\n        }\n\n    operator *(k f64) Vec2:\n        return {\n            x: self.x * k,\n            y: self.y * k,\n        }\n\nstruct Shape:\n    name char[]\n    origin Vec2\n\n    fn show(area f64):\n        paint(Color::YELLOW)\n        output(\"  \", self.name)\n        reset()\n        output(\" at (\",\n               self.origin.x as i32,\n               \", \",\n               self.origin.y as i32,\n               \") area \",\n               area as i32,\n               \"\\n\")\n\nstruct Circle from Shape:\n    radius f64\n\n    fn area() f64: return 3.14159265 * self.radius * self.radius\n\nstruct Rect from Shape:\n    size Vec2\n\n    fn area() f64: return self.size.x * self.size.y\n\nfn shapes():\n    title(\"structs, operators and struct from\")\n    step Vec2 = { x: 2.0, y: 1.0 }\n\n    c Circle = {\n        name: \"circle\",\n        origin: step * 2.0,\n        radius: 2.0,\n    }\n\n    c.show(c.area())\n\n    r Rect = {\n        name: \"rect  \",\n        origin: c.origin + step,\n        size: {\n            x: 4.0,\n            y: 2.5,\n        },\n    }\n\n    r.show(r.area())\n" },
    { NULL, NULL },
};

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
