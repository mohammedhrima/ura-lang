// A light, line-based reader for ura source. It doesn't replace the compiler:
// it only finds what editor features need (functions, variables, calls).
// No `vscode` import here, so this can move into a language server later.
// All positions are 0-based.

import { findBuiltin, RESERVED } from "./keywords";

export interface Position {
    line: number;
    character: number;
}

export interface Param {
    name: string;
    type: string;
    line: number;
    character: number;
}

export interface FunctionInfo {
    name: string;
    keyword: string; // "fn", or "proto" for a function declared elsewhere
    params: Param[];
    returnType: string; // "void" when the header has none
    line: number;
    character: number; // where the name starts
    indent: number;
    bodyStart: number; // first line after the header
    bodyEnd: number; // last non-blank line of the body; < bodyStart when empty
}

export interface VariableInfo {
    name: string;
    type: string;
    line: number;
    character: number;
    indent: number;
    fn: FunctionInfo | undefined; // undefined for a top-level declaration
    isParam: boolean;
}

export interface Analysis {
    lines: string[];
    functions: FunctionInfo[];
    variables: VariableInfo[];
}

export interface CallContext {
    name: string;
    argument: number; // which argument the cursor is in, 0-based
}

const IDENT = /[A-Za-z_]\w*/;
const FN_HEADER = /^(\s*)(fn|proto)\s+([A-Za-z_]\w*)\s*\(/;
// `name type` or `name type = value`, where type is i32, b1 or a ref form
const DECLARATION = /^(\s*)([A-Za-z_]\w*)(\s+)(i32|b1|ref\b[^=]*?)\s*(=.*)?$/;

// Where the comment starts on this line, or -1. A `//` inside a string
// literal doesn't start one, just like in the tokenizer.
function commentStart(line: string): number {
    let inString = false;
    for (let i = 0; i < line.length; i++) {
        if (line[i] === '"' || line[i] === '\'' ) {
            inString = !inString;
        } else if (!inString && line[i] === "/" && line[i + 1] === "/") {
            return i;
        }
    }
    return -1;
}

export function stripComment(line: string): string {
    const at = commentStart(line);
    return at < 0 ? line : line.slice(0, at);
}

export function isInComment(line: string, character: number): boolean {
    const at = commentStart(line);
    return at >= 0 && character > at;
}

function isBlank(line: string): boolean {
    return stripComment(line).trim() === "";
}

function indentOf(line: string): number {
    return line.length - line.trimStart().length;
}

interface Piece {
    text: string;
    offset: number;
}

// Split on commas that aren't inside parentheses, keeping each piece's offset.
function splitTopLevel(text: string, offset: number): Piece[] {
    const pieces: Piece[] = [];
    let depth = 0;
    let start = 0;
    for (let i = 0; i < text.length; i++) {
        const c = text[i];
        if (c === "(") {
            depth++;
        } else if (c === ")") {
            depth--;
        } else if (c === "," && depth === 0) {
            pieces.push({ text: text.slice(start, i), offset: offset + start });
            start = i + 1;
        }
    }
    pieces.push({ text: text.slice(start), offset: offset + start });
    return pieces;
}

function parseParams(text: string, line: number, offset: number): Param[] {
    const params: Param[] = [];
    for (const piece of splitTopLevel(text, offset)) {
        const lead = piece.text.length - piece.text.trimStart().length;
        const trimmed = piece.text.trim();
        const match = /^([A-Za-z_]\w*)\s*(.*)$/.exec(trimmed);
        if (!match) {
            continue;
        }
        params.push({
            name: match[1],
            type: match[2].trim(),
            line,
            character: piece.offset + lead,
        });
    }
    return params;
}

function parseFunction(lines: string[], line: number): FunctionInfo | undefined {
    const text = stripComment(lines[line]);
    const header = FN_HEADER.exec(text);
    if (!header) {
        return undefined;
    }
    const indent = header[1].length;
    const keyword = header[2];
    const name = header[3];
    const character = text.indexOf(name, indent + keyword.length);

    // find the ')' that closes the parameter list
    const open = header[0].length - 1;
    let close = -1;
    let depth = 0;
    for (let i = open; i < text.length; i++) {
        if (text[i] === "(") {
            depth++;
        } else if (text[i] === ")" && --depth === 0) {
            close = i;
            break;
        }
    }
    const paramsText = close < 0 ? text.slice(open + 1) : text.slice(open + 1, close);
    const params = parseParams(paramsText, line, open + 1);

    let returnType = "void";
    if (close >= 0) {
        const rest = text.slice(close + 1).trim().replace(/:$/, "").trim();
        if (rest) {
            returnType = rest;
        }
    }

    // the body is every following line indented deeper than the header
    let bodyEnd = line;
    for (let i = line + 1; i < lines.length; i++) {
        if (isBlank(lines[i])) {
            continue;
        }
        if (indentOf(lines[i]) <= indent) {
            break;
        }
        bodyEnd = i;
    }

    return { name, keyword, params, returnType, line, character, indent, bodyStart: line + 1, bodyEnd };
}

function enclosingFunction(functions: FunctionInfo[], line: number): FunctionInfo | undefined {
    let found: FunctionInfo | undefined;
    for (const fn of functions) {
        if (line >= fn.bodyStart && line <= fn.bodyEnd) {
            found = fn;
        }
    }
    return found;
}

export function analyze(source: string): Analysis {
    const lines = source.split(/\r?\n/);
    const functions: FunctionInfo[] = [];
    for (let line = 0; line < lines.length; line++) {
        const fn = parseFunction(lines, line);
        if (fn) {
            functions.push(fn);
        }
    }

    const variables: VariableInfo[] = [];
    for (const fn of functions) {
        for (const param of fn.params) {
            variables.push({ ...param, indent: fn.indent + 1, fn, isParam: true });
        }
    }
    for (let line = 0; line < lines.length; line++) {
        const match = DECLARATION.exec(stripComment(lines[line]));
        if (!match || RESERVED.has(match[2])) {
            continue;
        }
        variables.push({
            name: match[2],
            type: match[4].trim(),
            line,
            character: match[1].length,
            indent: match[1].length,
            fn: enclosingFunction(functions, line),
            isParam: false,
        });
    }

    return { lines, functions, variables };
}

// A declaration stays visible until a line indented less than it: blocks
// (if / elif / else / while) end their variables, like the compiler's scopes.
function stillInScope(lines: string[], variable: VariableInfo, at: Position): boolean {
    for (let i = variable.line + 1; i <= at.line && i < lines.length; i++) {
        const blank = isBlank(lines[i]);
        const indent = blank && i === at.line ? at.character : indentOf(lines[i]);
        if ((!blank || i === at.line) && indent < variable.indent) {
            return false;
        }
    }
    return true;
}

export function visibleVariables(analysis: Analysis, at: Position): VariableInfo[] {
    const fn = enclosingFunction(analysis.functions, at.line);
    const visible = new Map<string, VariableInfo>();
    for (const variable of analysis.variables) {
        if (variable.fn !== fn) {
            continue;
        }
        if (variable.isParam) {
            if (fn !== undefined && at.line >= fn.bodyStart) {
                visible.set(variable.name, variable);
            }
            continue;
        }
        const before =
            variable.line < at.line ||
            (variable.line === at.line && variable.character < at.character);
        if (before && stillInScope(analysis.lines, variable, at)) {
            visible.set(variable.name, variable); // a later declaration wins
        }
    }
    return [...visible.values()];
}

export interface Word {
    text: string;
    start: number;
    end: number;
}

export function wordAt(line: string, character: number): Word | undefined {
    const pattern = new RegExp(IDENT.source, "g");
    let match: RegExpExecArray | null;
    while ((match = pattern.exec(line)) !== null) {
        const start = match.index;
        const end = start + match[0].length;
        if (character >= start && character <= end) {
            return { text: match[0], start, end };
        }
    }
    return undefined;
}

export type Resolved =
    | { kind: "function"; function: FunctionInfo }
    | { kind: "variable"; variable: VariableInfo };

export function findFunction(analysis: Analysis, name: string): FunctionInfo | undefined {
    return analysis.functions.find((fn) => fn.name === name);
}

// What the identifier under the cursor refers to, if it's declared in this file.
export function resolve(analysis: Analysis, at: Position): Resolved | undefined {
    const line = analysis.lines[at.line] ?? "";
    if (isInComment(line, at.character)) {
        return undefined;
    }
    const word = wordAt(line, at.character);
    if (!word || RESERVED.has(word.text)) {
        return undefined;
    }

    const own = analysis.functions.find((fn) => fn.line === at.line && fn.character === word.start);
    if (own) {
        return { kind: "function", function: own };
    }
    const isCall = /^\s*\(/.test(line.slice(word.end));
    if (!isCall) {
        const variable = visibleVariables(analysis, { line: at.line, character: word.end + 1 })
            .find((v) => v.name === word.text);
        if (variable) {
            return { kind: "variable", variable };
        }
    }
    const fn = findFunction(analysis, word.text);
    return fn ? { kind: "function", function: fn } : undefined;
}

// The innermost unclosed `name(` before the cursor, on the cursor's line.
export function callContext(line: string, character: number): CallContext | undefined {
    if (isInComment(line, character)) {
        return undefined;
    }
    let depth = 0;
    let argument = 0;
    for (let i = Math.min(character, line.length) - 1; i >= 0; i--) {
        const c = line[i];
        if (c === ")") {
            depth++;
        } else if (c === "(") {
            if (depth > 0) {
                depth--;
                continue;
            }
            const before = line.slice(0, i).match(/([A-Za-z_]\w*)\s*$/);
            const name = before ? before[1] : undefined;
            if (name && (!RESERVED.has(name) || findBuiltin(name))) {
                return { name, argument };
            }
            // a grouping parenthesis (or `if (`, `ref(`...): keep looking for
            // the call around it
        } else if (c === "," && depth === 0) {
            argument++;
        }
    }
    return undefined;
}

export function functionSignature(fn: FunctionInfo): string {
    const params = fn.params.map((p) => `${p.name} ${p.type}`.trim()).join(", ");
    const ret = fn.returnType === "void" ? "" : ` ${fn.returnType}`;
    return `${fn.keyword} ${fn.name}(${params})${ret}`;
}
