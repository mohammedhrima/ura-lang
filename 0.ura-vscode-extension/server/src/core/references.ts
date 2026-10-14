// References, highlights and rename checks. An occurrence is any identifier
// with the symbol's name that resolves to the same declaration.

import { RESERVED } from "./keywords";
import { resolve, spansOf, type Analysis, type Imports, type Resolved, type Span } from "./parser";
import { declarationOf, sameSymbol } from "./symbols";

// One file as the lookups see it.
export interface FileContext {
    file: string; // its path (or URI for an untitled document)
    analysis: Analysis;
    imports: Imports;
}

// Variables and parameters live in one function: no need to look in other files.
export function isLocal(target: Resolved): boolean {
    return target.kind === "variable";
}

// Every place in `context` that refers to `target`, which was found in the
// file `targetFile`; the declaration counts when it's in this file.
export function occurrences(context: FileContext, target: Resolved, targetFile: string): Span[] {
    const name = declarationOf(target).name;
    return spansOf(context.analysis.lines, new Set([name])).filter((span) => {
        const found = resolve(context.analysis, { line: span.line, character: span.start }, context.imports);
        return found !== undefined && sameSymbol(found, target, context.file, targetFile);
    });
}

const ASSIGNMENT = /^\s*(=(?!=)|\+=|-=|\*=|\/=|%=)/;

// Whether `span` writes the symbol: its declaration, or the target of `=`, `+=`, ...
export function isWrite(context: FileContext, span: Span, target: Resolved, targetFile: string): boolean {
    const declaration = declarationOf(target);
    if ((declaration.file ?? targetFile) === context.file && declaration.line === span.line && declaration.character === span.start) {
        return true;
    }
    const line = context.analysis.lines[span.line] ?? "";
    return ASSIGNMENT.test(line.slice(span.start + span.length));
}

// Why `target` (declared in `context`'s file) can't be renamed to `name`, or
// undefined when it can.
export function renameProblem(context: Pick<FileContext, "analysis" | "imports">, target: Resolved, name: string): string | undefined {
    if (!/^[A-Za-z_]\w*$/.test(name)) {
        return `'${name}' is not a valid name`;
    }
    if (RESERVED.has(name)) {
        return `'${name}' is a keyword`;
    }
    const { analysis, imports } = context;
    const taken = (what: string): string => `a ${what} named '${name}' already exists`;
    const functionNamed = (): boolean =>
        analysis.functions.some((fn) => fn.owner === undefined && fn.name === name) ||
        imports.functions.some((i) => i.fn.owner === undefined && i.fn.name === name);
    const structNamed = (): boolean =>
        analysis.structs.some((st) => st.name === name) || imports.structs.some((i) => i.struct.name === name);

    switch (target.kind) {
        case "variable": {
            if (target.variable.name === "self") {
                return "'self' can't be renamed";
            }
            const fn = target.variable.fn;
            const clash = analysis.variables.some((v) => v.fn === fn && v.name === name && v !== target.variable);
            return clash ? taken("variable") : undefined;
        }
        case "function":
        case "struct":
            if (functionNamed()) {
                return taken("function");
            }
            return structNamed() ? taken("struct") : undefined;
        case "attribute":
        case "method": {
            const { struct } = target;
            if (struct.attributes.some((a) => a.name === name)) {
                return taken(`attribute of ${struct.name}`);
            }
            return struct.methods.some((m) => m.name === name) ? taken(`method of ${struct.name}`) : undefined;
        }
    }
}
