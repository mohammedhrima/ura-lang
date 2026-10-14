// Facts about symbols: where one is declared, what its hover says, and the
// outline (document symbols) of a file. Pure, like the rest of core.

import {
    functionSignature,
    structSignature,
    type Analysis,
    type FunctionInfo,
    type Position,
    type Resolved,
    type StructInfo,
    type VariableInfo,
} from "./parser";

export interface Declaration {
    name: string;
    file?: string; // set when declared in another file
    line: number;
    character: number;
}

export function declarationOf(found: Resolved): Declaration {
    const file = "file" in found && found.file ? { file: found.file } : {};
    switch (found.kind) {
        case "function":
        case "method":
            return { name: found.function.name, line: found.function.line, character: found.function.character, ...file };
        case "struct":
            return { name: found.struct.name, line: found.struct.line, character: found.struct.character, ...file };
        case "attribute":
            return { name: found.attribute.name, line: found.attribute.line, character: found.attribute.character, ...file };
        case "variable":
            return { name: found.variable.name, line: found.variable.line, character: found.variable.character };
    }
}

// Two lookups found the same symbol. `here` is the file both lookups ran in,
// so a declaration without a file is in `here`.
export function sameSymbol(a: Resolved, b: Resolved, hereA: string, hereB: string): boolean {
    if (a.kind !== b.kind) {
        return false;
    }
    const da = declarationOf(a);
    const db = declarationOf(b);
    return (da.file ?? hereA) === (db.file ?? hereB) && da.line === db.line && da.character === db.character;
}

export interface HoverText {
    code: string; // shown as ura code
    note?: string; // shown under it
}

export function hoverOf(found: Resolved): HoverText {
    switch (found.kind) {
        case "function":
            return { code: functionSignature(found.function) };
        case "method":
            return { code: functionSignature(found.function), note: `method of ${found.struct.name}` };
        case "struct":
            return { code: structSignature(found.struct) };
        case "attribute":
            return { code: `${found.attribute.name} ${found.attribute.type}`, note: `attribute of ${found.struct.name}` };
        case "variable":
            return { code: `${found.variable.name} ${found.variable.type}`, note: found.variable.isParam ? "parameter" : "variable" };
    }
}

export type SymbolKind = "struct" | "field" | "method" | "function" | "variable";

export interface Range {
    start: Position;
    end: Position;
}

export interface OutlineSymbol {
    name: string;
    detail: string;
    kind: SymbolKind;
    range: Range; // the whole declaration (header and body)
    selection: Range; // the name
    children: OutlineSymbol[];
}

function lineEnd(analysis: Analysis, line: number): Position {
    return { line, character: (analysis.lines[line] ?? "").length };
}

function nameRange(line: number, character: number, name: string): Range {
    return { start: { line, character }, end: { line, character: character + name.length } };
}

function variableSymbol(analysis: Analysis, variable: VariableInfo): OutlineSymbol {
    const selection = nameRange(variable.line, variable.character, variable.name);
    return {
        name: variable.name,
        detail: variable.type,
        kind: "variable",
        range: { start: selection.start, end: lineEnd(analysis, variable.line) },
        selection,
        children: [],
    };
}

function functionSymbol(analysis: Analysis, fn: FunctionInfo, kind: SymbolKind): OutlineSymbol {
    const locals = analysis.variables.filter((v) => v.fn === fn && v.name !== "self");
    return {
        name: fn.name,
        detail: functionSignature(fn),
        kind,
        range: { start: { line: fn.line, character: fn.indent }, end: lineEnd(analysis, Math.max(fn.line, fn.bodyEnd)) },
        selection: nameRange(fn.line, fn.character, fn.name),
        children: locals.map((v) => variableSymbol(analysis, v)),
    };
}

function structSymbol(analysis: Analysis, struct: StructInfo): OutlineSymbol {
    const fields: OutlineSymbol[] = struct.attributes.map((a) => {
        const selection = nameRange(a.line, a.character, a.name);
        return { name: a.name, detail: a.type, kind: "field", range: { start: selection.start, end: lineEnd(analysis, a.line) }, selection, children: [] };
    });
    const methods = struct.methods.map((m) => functionSymbol(analysis, m, "method"));
    return {
        name: struct.name,
        detail: "struct",
        kind: "struct",
        range: { start: { line: struct.line, character: struct.indent }, end: lineEnd(analysis, Math.max(struct.line, struct.bodyEnd)) },
        selection: nameRange(struct.line, struct.character, struct.name),
        children: [...fields, ...methods].sort((a, b) => a.range.start.line - b.range.start.line),
    };
}

// The outline of a file, in source order: structs (attributes, methods),
// functions and protos (their parameters and locals), top-level variables.
export function outlineOf(analysis: Analysis): OutlineSymbol[] {
    const symbols = [
        ...analysis.structs.map((st) => structSymbol(analysis, st)),
        ...analysis.functions.filter((fn) => fn.owner === undefined).map((fn) => functionSymbol(analysis, fn, "function")),
        ...analysis.variables.filter((v) => v.fn === undefined).map((v) => variableSymbol(analysis, v)),
    ];
    return symbols.sort((a, b) => a.range.start.line - b.range.start.line);
}

// Case-insensitive "all of the query's letters, in order" (`usad` finds `User.add`).
export function matchesQuery(name: string, query: string): boolean {
    const text = name.toLowerCase();
    let at = 0;
    for (const c of query.toLowerCase()) {
        at = text.indexOf(c, at);
        if (at < 0) {
            return false;
        }
        at++;
    }
    return true;
}
