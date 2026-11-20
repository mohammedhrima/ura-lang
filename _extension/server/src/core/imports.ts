// Follows `use` lines from one file to the files they bring in.
// Reading files is injected (`Reader`), so this stays free of I/O.

import * as path from "path";
import { analyze, type Analysis, type Imports, type UseInfo } from "./parser";

// Returns a file's text, or undefined when it can't be read.
export type Reader = (file: string) => string | undefined;

export interface SourceFile {
    file: string;
    source: string;
    analysis: Analysis;
    via?: UseInfo; // the `use` that reached this file (undefined for the first one)
}

// A relative `use` path is next to the file that writes it, and ".ura" is added.
export function resolveUse(from: string, target: string): string {
    return path.resolve(path.dirname(from), `${target}.ura`);
}

// Where `use "target"` written in `from` may point, most likely first. A
// package (`@/math`) is `math.ura` in one of `libs` (the project and its
// dependencies, then the lib folder), or an installed `math/current/math.ura`,
// like the compiler's find_package. Without `libs`, packages are not followed.
export function useCandidates(from: string, target: string, libs: string[]): string[] {
    if (!target.startsWith("@/")) {
        return [resolveUse(from, target)];
    }
    const [name = "", version] = target.slice(2).split("@");
    return libs.flatMap((lib) => {
        const installed = path.join(lib, name, version ?? "current", `${name}.ura`);
        return version === undefined ? [path.join(lib, `${name}.ura`), installed] : [installed];
    });
}

// Every program sees the core package without writing `use "@/core"`.
const CORE: UseInfo = { target: "@/core", line: -1, indent: 0, start: 0, end: 0 };

// `file` and every file reachable from it through `use`, each once, so
// diamonds and cycles are fine. `follow` decides which `use` lines count.
// A file that can't be read is skipped: the compiler reports it.
export function walkUses(
    file: string,
    source: string,
    read: Reader,
    follow: (use: UseInfo, from: SourceFile) => boolean,
    libs: string[] = [],
): SourceFile[] {
    const start: SourceFile = { file, source, analysis: analyze(source) };
    const found = new Map<string, SourceFile>([[file, start]]);
    const queue = [start];
    for (let from = queue.shift(); from !== undefined; from = queue.shift()) {
        const uses = from === start && libs.length > 0 ? [CORE, ...from.analysis.uses] : from.analysis.uses;
        for (const use of uses.filter((u) => follow(u, from))) {
            const targets = useCandidates(from.file, use.target, libs);
            const target = targets.find((t) => found.has(t) || read(t) !== undefined);
            const text = target === undefined || found.has(target) ? undefined : read(target);
            if (target === undefined || text === undefined) {
                continue;
            }
            const next = { file: target, source: text, analysis: analyze(text), via: use };
            found.set(target, next);
            queue.push(next);
        }
    }
    return [...found.values()];
}

// The other files whose declarations `file` sees: through its own `use`
// lines, then through each used file's top-level `use` lines and the ones in
// a struct body (a `use` inside a function stays private to it), like the
// compiler's lookup.
export function usedFiles(file: string, source: string, read: Reader, libs: string[] = []): SourceFile[] {
    const follow = (use: UseInfo, from: SourceFile): boolean =>
        from.file === file || use.indent === 0 || use.owner !== undefined;
    return walkUses(file, source, read, follow, libs).filter((f) => f.file !== file);
}

// Everything `file` sees from other files, walking its `use` lines once.
// The functions of a file pasted into a struct body are that struct's methods.
export function importsOf(file: string, source: string, read: Reader, libs: string[] = []): Imports {
    const imports: Imports = { functions: [], structs: [], enums: [], methods: [] };
    for (const f of usedFiles(file, source, read, libs)) {
        const owner = f.via?.owner;
        for (const fn of f.analysis.functions) {
            if (owner !== undefined && fn.owner === undefined) {
                imports.methods.push({ file: f.file, fn: { ...fn, owner } });
            } else {
                imports.functions.push({ file: f.file, fn });
            }
        }
        imports.structs.push(...f.analysis.structs.map((struct) => ({ file: f.file, struct })));
        imports.enums.push(...f.analysis.enums.map((enumeration) => ({ file: f.file, enumeration })));
    }
    return imports;
}
