// Facts about symbols: where one is declared, what its hover says, and the
// outline (document symbols) of a file. Pure, like the rest of core.

import {
    enumSignature,
    functionSignature,
    structSignature,
    type Analysis,
    type EnumInfo,
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
    const at = (item: { name: string; line: number; character: number }): Declaration => ({
        name: item.name,
        line: item.line,
        character: item.character,
        ...file,
    });
    switch (found.kind) {
        case "function":
        case "method":
            return at(found.function);
        case "struct":
            return at(found.struct);
        case "attribute":
            return at(found.attribute);
        case "enum":
            return at(found.enumeration);
        case "variant":
            return at(found.variant);
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
        case "enum":
            return { code: enumSignature(found.enumeration) };
        case "variant":
            return { code: `${found.enumeration.name}::${found.variant.name}`, note: `variant of ${found.enumeration.name}` };
        case "variable":
            return { code: `${found.variable.name} ${found.variable.type}`, note: found.variable.isParam ? "parameter" : "variable" };
    }
}

export type SymbolKind = "struct" | "field" | "method" | "function" | "variable" | "enum" | "variant";

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

// A symbol that is one name on one line: a variable, an attribute, a variant.
function lineSymbol(analysis: Analysis, item: { name: string; line: number; character: number }, detail: string, kind: SymbolKind): OutlineSymbol {
    const selection = nameRange(item.line, item.character, item.name);
    return { name: item.name, detail, kind, range: { start: selection.start, end: lineEnd(analysis, item.line) }, selection, children: [] };
}

function variableSymbol(analysis: Analysis, variable: VariableInfo): OutlineSymbol {
    return lineSymbol(analysis, variable, variable.type, "variable");
}

function functionSymbol(analysis: Analysis, fn: FunctionInfo, kind: SymbolKind): OutlineSymbol {
    const locals = analysis.variables.filter((v) => v.fn === fn && v.name !== "self");
    return {
        name: fn.keyword === "operator" ? `operator ${fn.name}` : fn.name,
        detail: functionSignature(fn),
        kind,
        range: { start: { line: fn.line, character: fn.indent }, end: lineEnd(analysis, Math.max(fn.line, fn.bodyEnd)) },
        selection: nameRange(fn.line, fn.character, fn.name),
        children: locals.map((v) => variableSymbol(analysis, v)),
    };
}

function structSymbol(analysis: Analysis, struct: StructInfo): OutlineSymbol {
    const fields = struct.attributes.map((a) => lineSymbol(analysis, a, a.type, "field"));
    const methods = struct.methods.map((m) => functionSymbol(analysis, m, "method"));
    return {
        name: struct.name,
        detail: struct.parents.length === 0 ? "struct" : `struct from ${struct.parents.join(" and ")}`,
        kind: "struct",
        range: { start: { line: struct.line, character: struct.indent }, end: lineEnd(analysis, Math.max(struct.line, struct.bodyEnd)) },
        selection: nameRange(struct.line, struct.character, struct.name),
        children: [...fields, ...methods].sort((a, b) => a.range.start.line - b.range.start.line),
    };
}

function enumSymbol(analysis: Analysis, enumeration: EnumInfo): OutlineSymbol {
    const last = enumeration.variants[enumeration.variants.length - 1]?.line ?? enumeration.line;
    return {
        name: enumeration.name,
        detail: "enum",
        kind: "enum",
        range: { start: { line: enumeration.line, character: enumeration.indent }, end: lineEnd(analysis, last) },
        selection: nameRange(enumeration.line, enumeration.character, enumeration.name),
        children: enumeration.variants.map((v) => lineSymbol(analysis, v, enumeration.name, "variant")),
    };
}

// The outline of a file, in source order: structs (attributes, methods),
// enums (variants), functions and protos (their parameters and locals),
// top-level variables.
export function outlineOf(analysis: Analysis): OutlineSymbol[] {
    const symbols = [
        ...analysis.structs.map((st) => structSymbol(analysis, st)),
        ...analysis.enums.map((en) => enumSymbol(analysis, en)),
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
