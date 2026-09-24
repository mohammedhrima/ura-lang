// Runs the real ura compiler and turns what it prints into problems.
// No `vscode` import here, so this can move into a language server later.

import { spawn } from "child_process";
import * as fs from "fs";
import * as os from "os";
import * as path from "path";

export interface Problem {
    message: string;
    line?: number; // 0-based; undefined when the compiler gave no position
    character?: number; // 0-based
    code?: string; // where in the compiler the error came from, e.g. "main.c:827 analyze"
}

// `setting` wins; otherwise the compiler built inside the workspace; otherwise PATH.
export function resolveCompiler(setting: string, workspaceRoot: string | undefined): string {
    if (setting) {
        return setting;
    }
    if (workspaceRoot) {
        const local = path.join(workspaceRoot, "src", "build", "ura");
        if (fs.existsSync(local)) {
            return local;
        }
    }
    return "ura";
}

const COLORS = /\x1b\[[0-9;]*m/g;
// the format the compiler should print once tokens know their position
const POSITIONED = /^(.*\.ura):(\d+):(\d+):\s*(.*)$/;
// what eprint prints today: "<compiler file>:<line> <function> <message>"
const COMPILER_SIDE = /^(\S+\.[ch]:\d+(?: \w+)?) (.*)$/;
const SANITIZER = /==\d+==ERROR: (AddressSanitizer: .*)/;

export function parseStderr(stderr: string): Problem[] {
    const text = stderr.replace(COLORS, "");
    const crash = SANITIZER.exec(text);
    if (crash) {
        return [{ message: `compiler crashed: ${crash[1].trim()}` }];
    }

    const problems: Problem[] = [];
    for (const raw of text.split("\n")) {
        const line = raw.trimEnd();
        if (!line.trim()) {
            continue;
        }
        const positioned = POSITIONED.exec(line);
        if (positioned) {
            problems.push({
                message: positioned[4],
                line: Number(positioned[2]) - 1,
                character: Number(positioned[3]) - 1,
            });
            continue;
        }
        const compilerSide = COMPILER_SIDE.exec(line);
        if (compilerSide) {
            problems.push({ message: compilerSide[2].trim(), code: compilerSide[1] });
            continue;
        }
        // details printed on the lines after an error belong to it
        const last = problems[problems.length - 1];
        if (last) {
            last.message += `\n${line}`;
        }
    }
    return problems;
}

// Compiles `source` in a temporary directory, so unsaved edits are checked and
// no build/ folder appears next to the user's file.
export function lint(compiler: string, source: string, timeoutMs = 10_000): Promise<Problem[]> {
    const dir = fs.mkdtempSync(path.join(os.tmpdir(), "ura-lint-"));
    const input = path.join(dir, "input.ura");
    fs.writeFileSync(input, source);

    return new Promise((resolve) => {
        let stderr = "";
        let finished = false;
        let timer: NodeJS.Timeout | undefined;

        const finish = (problems: Problem[]) => {
            if (finished) {
                return;
            }
            finished = true;
            if (timer) {
                clearTimeout(timer);
            }
            fs.rmSync(dir, { recursive: true, force: true });
            resolve(problems);
        };

        // stdout is ignored: the compiler prints its debug trees there
        const child = spawn(compiler, [input, "-o", path.join(dir, "exe")], {
            stdio: ["ignore", "ignore", "pipe"],
        });
        timer = setTimeout(() => {
            child.kill("SIGKILL");
            finish([{ message: `the ura compiler timed out after ${timeoutMs / 1000}s` }]);
        }, timeoutMs);

        child.stderr.on("data", (chunk: Buffer) => {
            stderr += chunk.toString();
        });
        child.on("error", (error) => {
            finish([{ message: `cannot run the ura compiler '${compiler}': ${error.message}` }]);
        });
        child.on("close", (code) => {
            const problems = parseStderr(stderr);
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
