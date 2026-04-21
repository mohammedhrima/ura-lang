#include "../color.h"
#include "../fs.h"

#include <cstdio>
#include <string>
#include <vector>

static int write_file(const std::string& path, const std::string& content) {
    FILE* f = fopen(path.c_str(), "w");
    if (!f) return -1;
    fwrite(content.data(), 1, content.size(), f);
    fclose(f);
    return 0;
}

int cmd_new(const std::vector<std::string>& args) {
    if (args.empty()) {
        fprintf(stderr, ANSI_RED "usage: avatar new <name>" ANSI_RESET "\n");
        return 1;
    }
    const std::string& name = args[0];
    if (is_dir(name) || is_file(name)) {
        fprintf(stderr, ANSI_RED "avatar new: %s already exists" ANSI_RESET "\n", name.c_str());
        return 1;
    }

    mkdir_p(name + "/src");
    mkdir_p(name + "/tests");

    std::string toml =
        "[project]\n"
        "name    = \"" + name + "\"\n"
        "version = \"0.1.0\"\n\n"
        "[build]\n"
        "src      = \"src/main.ura\"\n"
        "output   = \"target/" + name + "\"\n"
        "optimize = \"O2\"\n\n"
        "[dependencies]\n";
    std::string main_ura =
        "use \"@/io\"\n\n"
        "main():\n"
        "    output(\"hello from " + name + "!\\n\")\n"
        "    return 0\n";
    std::string gitignore = "target/\n";

    write_file(name + "/ura.toml",     toml);
    write_file(name + "/src/main.ura", main_ura);
    write_file(name + "/.gitignore",   gitignore);

    printf(ANSI_GREEN "Created %s" ANSI_RESET "\n", name.c_str());
    printf("  %s/ura.toml\n", name.c_str());
    printf("  %s/src/main.ura\n", name.c_str());
    printf("  %s/.gitignore\n", name.c_str());
    printf("\n  cd %s && avatar run\n\n", name.c_str());
    return 0;
}
