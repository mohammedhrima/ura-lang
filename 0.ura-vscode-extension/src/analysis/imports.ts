// Follows `use` lines from one file to the files they bring in.
// No `vscode` import here, so this can move into a language server later.

import * as fs from "fs";
import * as path from "path";
import { analyze, Analysis, ImportedFunction, UseInfo } from "./parser";

// Returns a file's text, or undefined when it can't be read.
export type Reader = (file: string) => string | undefined;

export const readFromDisk: Reader = (file) => {
    try {
        return fs.readFileSync(file, "utf8");
    } catch {
        return undefined;
    }
};

export interface SourceFile {
    file: string;
    source: string;
    analysis: Analysis;
}

// A `use` path is relative to the file that writes it, and ".ura" is added.
export function resolveUse(from: string, target: string): string {
    return path.resolve(path.dirname(from), `${target}.ura`);
}

// `file` and every file reachable from it through `use`, each once, so
// diamonds and cycles are fine. `follow` decides which `use` lines count.
// A file that can't be read is skipped: the compiler reports it.
export function walkUses(
    file: string,
    source: string,
    read: Reader,
    follow: (use: UseInfo, from: SourceFile) => boolean,
): SourceFile[] {
    const start: SourceFile = { file, source, analysis: analyze(source) };
    const found = new Map<string, SourceFile>([[file, start]]);
    const queue = [start];
    while (queue.length > 0) {
        const from = queue.shift()!;
        for (const use of from.analysis.uses) {
            if (!follow(use, from)) {
                continue;
            }
            const target = resolveUse(from.file, use.target);
            if (found.has(target)) {
                continue;
            }
            const text = read(target);
            if (text === undefined) {
                continue;
            }
            const next: SourceFile = { file: target, source: text, analysis: analyze(text) };
            found.set(target, next);
            queue.push(next);
        }
    }
    return [...found.values()];
}

// The functions `file` can call from other files: through its own `use`
// lines, then through each used file's top-level `use` lines (a `use` inside
// a function stays private to that function), like the compiler's lookup.
export function importedFunctions(file: string, source: string, read: Reader): ImportedFunction[] {
    const files = walkUses(file, source, read, (use, from) => from.file === file || use.indent === 0);
    return files
        .filter((f) => f.file !== file)
        .flatMap((f) => f.analysis.functions.map((fn) => ({ file: f.file, fn })));
}
