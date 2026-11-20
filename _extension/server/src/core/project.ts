// A project is a folder with a ura.conf file:
//
//     @name "raytracer"
//     @main "src/main.ura"
//     @dependency "ura-core" "0.1.0" "https://github.com/mohammedhrima/ura-core"
//     @dependency "geo" "0.2.0" "../geometry"
//
// Same rules as the compiler's src/project.c. Pure: file access is injected.

import * as path from "path";

export const CONFIG = "ura.conf";

export interface Dependency {
    name: string;
    version: string;
    where: string; // a git repo url, or a folder (starts with . or /)
}

export interface Project {
    dir: string;
    name?: string;
    version?: string;
    main: string; // relative to dir, "main.ura" when not given
    scripts: Map<string, string>;
    dependencies: Dependency[];
}

// Every `@macro "a" "b" "c"` line; lines inside `@if` blocks count too.
export function parseConfig(dir: string, text: string): Project {
    const project: Project = { dir, main: "main.ura", scripts: new Map(), dependencies: [] };
    for (const line of text.split(/\r?\n/)) {
        const macro = /^\s*@(\w+)/.exec(line)?.[1];
        const strings = [...line.matchAll(/"([^"]*)"/g)].map((m) => m[1] ?? "");
        const [first, second, third] = strings;
        if (macro === undefined || first === undefined) {
            continue;
        }
        if (macro === "name") {
            project.name = first;
        } else if (macro === "version") {
            project.version = first;
        } else if (macro === "main") {
            project.main = first;
        } else if (macro === "script" && second !== undefined) {
            project.scripts.set(first, second);
        } else if (macro === "dependency" && second !== undefined && third !== undefined) {
            project.dependencies.push({ name: first, version: second, where: third });
        }
    }
    return project;
}

// The folder holding ura.conf at or above `file`'s folder, or undefined.
export function findProject(file: string, exists: (file: string) => boolean): string | undefined {
    for (let dir = path.dirname(file); ; dir = path.dirname(dir)) {
        if (exists(path.join(dir, CONFIG))) {
            return dir;
        }
        if (path.dirname(dir) === dir) {
            return undefined;
        }
    }
}

// Where a dependency is on disk: a folder is relative to the project; a repo
// is cloned into the project, in dependencies/<name>@<version>.
export function dependencyDir(project: string, dependency: Dependency): string {
    const { name, version, where } = dependency;
    if (where.startsWith(".") || where.startsWith("/")) {
        return path.resolve(project, where);
    }
    return path.join(project, "dependencies", `${name}@${version}`);
}

// The folders where `use "@/name"` looks for name.ura: the project's own
// folder, then each dependency.
export function packageDirs(project: Project): string[] {
    return [project.dir, ...project.dependencies.map((d) => dependencyDir(project.dir, d))];
}

// The ura.conf for a copy of the project in another folder: the real text,
// line for line (so errors keep their positions), with each valid dependency
// pointing at its real folder, so the copy compiles against the same ones.
export function stagedConfig(project: Project, text: string): string {
    const line = /^(\s*@dependency\s+"([^"]*)"\s+"([^"]*)"\s+)"[^"]*"/gm;
    return text.replace(line, (whole: string, head: string, name: string, version: string) => {
        const dependency = project.dependencies.find((d) => d.name === name && d.version === version);
        return dependency ? `${head}"${dependencyDir(project.dir, dependency)}"` : whole;
    });
}
