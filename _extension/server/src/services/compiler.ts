// Runs the real ura compiler on a document and returns its problems.

import { spawn, type ChildProcess } from "child_process";
import * as fs from "fs";
import * as os from "os";
import * as path from "path";
import { COLORS, parseStderr, type Problem } from "../core/compiler";
import { walkUses, type Reader } from "../core/imports";
import { CONFIG, stagedConfig, type Project } from "../core/project";
import type { Config } from "./config";
import type { Logger } from "./logger";
import { readFromDisk, type Workspace } from "./workspace";

// `setting` wins; otherwise the compiler built inside the workspace (bin/ura); otherwise PATH.
export function resolveCompiler(setting: string, workspaceRoot: string | undefined): string {
    if (setting) {
        return setting;
    }
    if (workspaceRoot) {
        const local = path.join(workspaceRoot, "bin", "ura");
        if (fs.existsSync(local)) {
            return local;
        }
    }
    return "ura";
}

// The deepest directory that holds every one of `files`.
function commonDir(files: string[]): string {
    let dir = path.dirname(files[0] ?? ".");
    const inside = (file: string): boolean => {
        const relative = path.relative(dir, file);
        return relative !== ".." && !relative.startsWith(`..${path.sep}`) && !path.isAbsolute(relative);
    };
    while (!files.every(inside)) {
        const parent = path.dirname(dir);
        if (parent === dir) {
            break;
        }
        dir = parent;
    }
    return dir;
}

export interface Staged {
    input: string; // the copy of the file to compile
    root: string; // the real folder that `dir` mirrors
}

// Copies `file` (with its unsaved `source`) and every file it reaches through
// a relative `use` into `dir`, keeping their layout, so each `use` path
// resolves the same way it does next to the real files. A project also gets
// its ura.conf, pointing at the real dependency folders.
export function stage(dir: string, file: string, source: string, read: Reader = readFromDisk, project?: Project): Staged {
    const files = walkUses(file, source, read, () => true);
    const config = project ? [path.join(project.dir, CONFIG)] : [];
    const root = commonDir([...files.map((f) => f.file), ...config]);
    for (const f of files) {
        const copy = path.join(dir, path.relative(root, f.file));
        fs.mkdirSync(path.dirname(copy), { recursive: true });
        fs.writeFileSync(copy, f.source);
    }
    if (project) {
        const text = read(path.join(project.dir, CONFIG)) ?? "";
        fs.writeFileSync(path.join(dir, path.relative(root, project.dir), CONFIG), stagedConfig(project, text));
    }
    return { input: path.join(dir, path.relative(root, file)), root };
}

export interface LintOptions {
    file?: string; // where `source` lives: its `use` files are staged too
    project?: Project; // the project `file` belongs to
    read?: Reader;
    timeoutMs?: number;
    // called with the compiler process, so the caller can kill it on shutdown
    onSpawn?: (child: ChildProcess) => void;
}

// Compiles `source` in a temporary directory, so unsaved edits are checked
// and no build/ folder appears next to the user's files. Each problem's
// `file` is the real file it is about.
export function lint(compiler: string, source: string, options: LintOptions = {}): Promise<Problem[]> {
    const { file, project, read = readFromDisk, timeoutMs = 10_000, onSpawn } = options;
    const dir = fs.mkdtempSync(path.join(os.tmpdir(), "ura-lint-"));
    let input = path.join(dir, "input.ura");
    let root = dir;
    if (file) {
        ({ input, root } = stage(dir, file, source, read, project));
    } else {
        fs.writeFileSync(input, source);
    }
    // inside a project the compiler runs from the copy of its folder, so it finds its ura.conf
    const cwd = project ? path.join(dir, path.relative(root, project.dir)) : dir;
    const realFile = (printed: string): string => path.join(root, path.relative(dir, path.resolve(cwd, printed)));

    return new Promise((resolve) => {
        let stderr = "";
        let finished = false;

        const finish = (problems: Problem[]): void => {
            if (finished) {
                return;
            }
            finished = true;
            clearTimeout(timer);
            fs.rmSync(dir, { recursive: true, force: true });
            resolve(problems);
        };

        // stdout is ignored: the compiler prints its debug trees there
        const child = spawn(compiler, [input, "-o", path.join(dir, "exe")], {
            cwd,
            stdio: ["ignore", "ignore", "pipe"],
        });
        onSpawn?.(child);
        const timer = setTimeout(() => {
            child.kill("SIGKILL");
            finish([{ message: `the ura compiler timed out after ${timeoutMs / 1000}s` }]);
        }, timeoutMs);

        child.stderr?.on("data", (chunk: Buffer) => {
            stderr += chunk.toString();
        });
        child.on("error", (error) => {
            finish([{ message: `cannot run the ura compiler '${compiler}': ${error.message}` }]);
        });
        child.on("close", (code) => {
            const problems = parseStderr(stderr).map((p) => (p.file && file ? { ...p, file: realFile(p.file) } : p));
            if (code !== 0 && problems.length === 0) {
                const details = stderr.replace(COLORS, "").trim();
                problems.push({
                    message: `the ura compiler exited with code ${code}` + (details ? `: ${details}` : ""),
                });
            }
            finish(problems);
        });
    });
}

// The project's main file when it reaches `file` through `use`: one compile
// of it checks the whole project. Undefined outside a project, or for a file
// main doesn't use.
function mainReaching(project: Project | undefined, file: string, read: Reader): { file: string; source: string } | undefined {
    if (!project) {
        return undefined;
    }
    const main = path.join(project.dir, project.main);
    const source = read(main);
    if (source === undefined) {
        return undefined;
    }
    // ura.conf itself is checked through main: the compiler reads it first
    const isConfig = file === path.join(project.dir, CONFIG);
    const reaches = isConfig || walkUses(main, source, read, () => true).some((f) => f.file === file);
    return reaches ? { file: main, source } : undefined;
}

export interface Compiler {
    // the compiler to run: ura.compilerPath, else <workspace>/bin/ura, else `ura`
    path(): string;
    // the compiler's problems for `source`, which lives at `file` (undefined: untitled)
    check(source: string, file: string | undefined): Promise<Problem[]>;
    // kills every compiler still running
    dispose(): void;
}

export function createCompiler(config: Config, workspace: Workspace, logger: Logger): Compiler {
    const running = new Set<ChildProcess>();
    const compilerPath = (): string => resolveCompiler(config.get().compilerPath, workspace.root());
    return {
        path: compilerPath,
        async check(source: string, file: string | undefined): Promise<Problem[]> {
            const compiler = compilerPath();
            const project = file === undefined ? undefined : workspace.projectOf(file);
            const main = file === undefined ? undefined : mainReaching(project, file, workspace.read);
            const target = main ?? { file, source };
            const problems = await lint(compiler, target.source, {
                ...(target.file === undefined ? {} : { file: target.file }),
                ...(project === undefined ? {} : { project }),
                read: workspace.read,
                onSpawn: (child) => {
                    running.add(child);
                    child.on("close", () => running.delete(child));
                },
            });
            const missing = problems.find((p) => p.message.startsWith("cannot run the ura compiler"));
            if (missing) {
                logger.notifyOnce(`${missing.message}. Build it with \`build\`, or set "ura.compilerPath".`);
            }
            return problems;
        },
        dispose(): void {
            for (const child of running) {
                child.kill("SIGKILL");
            }
            running.clear();
        },
    };
}
