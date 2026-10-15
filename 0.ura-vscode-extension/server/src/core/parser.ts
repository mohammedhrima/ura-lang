// A light, line-based reader for ura source. It doesn't replace the compiler:
// it only finds what editor features need (functions, structs, variables, calls).
// Pure: no editor, LSP or file-system imports, so it runs in plain Node tests.
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
    owner?: string; // the struct a method belongs to
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

export interface UseInfo {
    target: string; // the path as written, without ".ura"
    line: number;
    indent: number;
    start: number; // where the path starts, after the opening quote
    end: number; // the closing quote
}

export interface AttributeInfo {
    name: string;
    type: string;
    line: number;
    character: number;
    owner: string; // the struct it belongs to
}

export interface StructInfo {
    name: string;
    line: number;
    character: number; // where the name starts
    indent: number;
    bodyStart: number;
    bodyEnd: number; // last non-blank line of the body
    attributes: AttributeInfo[];
    methods: FunctionInfo[];
}

export interface Analysis {
    lines: string[];
    functions: FunctionInfo[];
    variables: VariableInfo[];
    uses: UseInfo[];
    structs: StructInfo[];
}

// Where a name is written: one line, `length` characters from `start`.
export interface Span {
    line: number;
    start: number;
    length: number;
}

// A function declared in another file and brought in through `use`.
export interface ImportedFunction {
    file: string;
    fn: FunctionInfo;
}

// A struct declared in another file and brought in through `use`.
export interface ImportedStruct {
    file: string;
    struct: StructInfo;
}

// What a file sees from other files.
export interface Imports {
    functions: ImportedFunction[];
    structs: ImportedStruct[];
}

export const NO_IMPORTS: Imports = { functions: [], structs: [] };

export interface CallContext {
    name: string;
    argument: number; // which argument the cursor is in, 0-based
    start: number; // where the called name starts
}

const IDENT = /[A-Za-z_]\w*/;
const FN_HEADER = /^(\s*)(fn|proto)\s+([A-Za-z_]\w*)\s*\(/;
// `name type` or `name type = value`; type: a name, maybe `&` refs and `[]` arrays
const DECLARATION = /^(\s*)([A-Za-z_]\w*)(\s+)(&*[A-Za-z_]\w*(?:\[\])*)\s*(=.*)?$/;
// `use "path"`, at any indentation
const USE = /^(\s*)use\s+"([^"]*)"/;
// `struct Name:`
const STRUCT = /^(\s*struct\s+)([A-Za-z_]\w*)/;

// Where the comment starts on this line, or -1. A `//` inside a string or
// character literal doesn't start one, just like in the tokenizer; inside a
// literal, `\"` and `\'` don't end it, and the other quote kind is plain text.
function commentStart(line: string): number {
    let quote: string | undefined; // the quote of the literal we're in
    for (let i = 0; i < line.length; i++) {
        const c = line[i];
        if (quote !== undefined) {
            if (c === "\\") {
                i++; // skip the escaped character
            } else if (c === quote) {
                quote = undefined;
            }
        } else if (c === '"' || c === "'") {
            quote = c;
        } else if (c === "/" && line[i + 1] === "/") {
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

// Group `i` of a successful match ("" if the group didn't take part).
function group(match: RegExpExecArray, i: number): string {
    return match[i] ?? "";
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
            name: group(match, 1),
            type: group(match, 2).trim(),
            line,
            character: piece.offset + lead,
        });
    }
    return params;
}

function parseFunction(lines: string[], line: number): FunctionInfo | undefined {
    const text = stripComment(lines[line] ?? "");
    const header = FN_HEADER.exec(text);
    if (!header) {
        return undefined;
    }
    const indent = group(header, 1).length;
    const keyword = group(header, 2);
    const name = group(header, 3);
    const character = text.indexOf(name, indent + keyword.length);

    // find the ')' that closes the parameter list
    const open = group(header, 0).length - 1;
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

    const bodyEnd = bodyEndOf(lines, line, indent);
    return { name, keyword, params, returnType, line, character, indent, bodyStart: line + 1, bodyEnd };
}

// The body of a block is every following line indented deeper than its header.
function bodyEndOf(lines: string[], header: number, indent: number): number {
    let bodyEnd = header;
    for (let i = header + 1; i < lines.length; i++) {
        if (isBlank(lines[i] ?? "")) {
            continue;
        }
        if (indentOf(lines[i] ?? "") <= indent) {
            break;
        }
        bodyEnd = i;
    }
    return bodyEnd;
}

function parseStruct(lines: string[], line: number): StructInfo | undefined {
    const match = STRUCT.exec(stripComment(lines[line] ?? ""));
    if (!match) {
        return undefined;
    }
    const indent = indentOf(lines[line] ?? "");
    return {
        name: group(match, 2),
        line,
        character: group(match, 1).length,
        indent,
        bodyStart: line + 1,
        bodyEnd: bodyEndOf(lines, line, indent),
        attributes: [],
        methods: [],
    };
}

function enclosingStruct(structs: StructInfo[], line: number): StructInfo | undefined {
    return structs.find((st) => line >= st.bodyStart && line <= st.bodyEnd);
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
    const structs: StructInfo[] = [];
    for (let line = 0; line < lines.length; line++) {
        const fn = parseFunction(lines, line);
        if (fn) {
            functions.push(fn);
        }
        const st = parseStruct(lines, line);
        if (st) {
            structs.push(st);
        }
    }

    // a function written inside a struct is one of its methods
    for (const fn of functions) {
        const owner = enclosingStruct(structs, fn.line);
        if (owner && fn.keyword === "fn") {
            fn.owner = owner.name;
            owner.methods.push(fn);
        }
    }

    const variables: VariableInfo[] = [];
    for (const fn of functions) {
        if (fn.owner !== undefined) {
            // `self` is a ref to the struct the method is called on
            const self = { name: "self", type: `&${fn.owner}`, line: fn.line, character: fn.character };
            variables.push({ ...self, indent: fn.indent + 1, fn, isParam: true });
        }
        for (const param of fn.params) {
            variables.push({ ...param, indent: fn.indent + 1, fn, isParam: true });
        }
    }
    for (let line = 0; line < lines.length; line++) {
        const match = DECLARATION.exec(stripComment(lines[line] ?? ""));
        if (!match || RESERVED.has(group(match, 2))) {
            continue;
        }
        const name = group(match, 2);
        const type = group(match, 4).trim();
        const character = group(match, 1).length;
        const fn = enclosingFunction(functions, line);
        const owner = fn === undefined ? enclosingStruct(structs, line) : undefined;
        if (owner) {
            owner.attributes.push({ name, type, line, character, owner: owner.name });
            continue;
        }
        variables.push({ name, type, line, character, indent: character, fn, isParam: false });
    }

    const uses: UseInfo[] = [];
    for (let line = 0; line < lines.length; line++) {
        const match = USE.exec(stripComment(lines[line] ?? ""));
        if (!match) {
            continue;
        }
        const end = group(match, 0).length - 1;
        const start = end - group(match, 2).length;
        uses.push({ target: group(match, 2), line, indent: group(match, 1).length, start, end });
    }

    return { lines, functions, variables, uses, structs };
}

// The line without its comment, and with string and character literals
// blanked out (same length), so every identifier left in it is code.
export function codeOnly(line: string): string {
    return stripComment(line).replace(/"(\\.|[^"\\])*"|'(\\.|[^'\\])*'/g, (m) => " ".repeat(m.length));
}

// Every place one of `names` is written as an identifier in code. A struct's
// name is never reused for a variable or a function, so each match is the struct.
export function spansOf(lines: string[], names: Set<string>): Span[] {
    const spans: Span[] = [];
    lines.forEach((line, i) => {
        const ident = new RegExp(IDENT.source, "g");
        const code = codeOnly(line);
        let match: RegExpExecArray | null;
        while ((match = ident.exec(code)) !== null) {
            if (names.has(group(match, 0))) {
                spans.push({ line: i, start: match.index, length: group(match, 0).length });
            }
        }
    });
    return spans;
}

// A declaration stays visible until a line indented less than it: blocks
// (if / elif / else / while) end their variables, like the compiler's scopes.
function stillInScope(lines: string[], variable: VariableInfo, at: Position): boolean {
    for (let i = variable.line + 1; i <= at.line && i < lines.length; i++) {
        const blank = isBlank(lines[i] ?? "");
        const indent = blank && i === at.line ? at.character : indentOf(lines[i] ?? "");
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
        const end = start + group(match, 0).length;
        if (character >= start && character <= end) {
            return { text: group(match, 0), start, end };
        }
    }
    return undefined;
}

// What a name refers to. `file` is set when it's declared in another file.
export type Resolved =
    | { kind: "function"; function: FunctionInfo; file?: string }
    | { kind: "method"; function: FunctionInfo; struct: StructInfo; file?: string }
    | { kind: "variable"; variable: VariableInfo }
    | { kind: "struct"; struct: StructInfo; file?: string }
    | { kind: "attribute"; attribute: AttributeInfo; struct: StructInfo; file?: string };

// This file's functions come first, then the ones brought in through `use`.
// Methods aren't found by name alone: they are called on a struct (`u.add()`).
export function findFunction(
    analysis: Analysis,
    name: string,
    imported: ImportedFunction[] = [],
): FunctionInfo | undefined {
    const plain = (fn: FunctionInfo): boolean => fn.name === name && fn.owner === undefined;
    return analysis.functions.find(plain) ?? imported.find((i) => plain(i.fn))?.fn;
}

// A struct by name: this file's first, then the ones brought in through `use`.
export function findStruct(analysis: Analysis, imports: Imports, name: string): ImportedStruct | undefined {
    const own = analysis.structs.find((st) => st.name === name);
    if (own) {
        return { file: "", struct: own };
    }
    return imports.structs.find((i) => i.struct.name === name);
}

// The struct a type names (`User`, `&User`), or undefined (`i32`, `User[]`).
function structOfType(analysis: Analysis, imports: Imports, type: string): ImportedStruct | undefined {
    const name = type.replace(/^&+/, "");
    return /^[A-Za-z_]\w*$/.test(name) ? findStruct(analysis, imports, name) : undefined;
}

const fileOf = (found: ImportedStruct): { file?: string } => (found.file ? { file: found.file } : {});

// The struct of the expression that ends right before `dot` on `at.line`
// (`u` in `u.a`, `r.b` in `r.b.x`), or undefined.
function receiverStruct(analysis: Analysis, imports: Imports, line: number, dot: number): ImportedStruct | undefined {
    const text = analysis.lines[line] ?? "";
    const chain = /([A-Za-z_]\w*(?:\s*\.\s*[A-Za-z_]\w*)*)\s*$/.exec(text.slice(0, dot));
    if (!chain) {
        return undefined;
    }
    const parts = group(chain, 1).split(".").map((part) => part.trim());
    const first = parts[0] ?? "";
    const variable = visibleVariables(analysis, { line, character: chain.index + first.length + 1 }).find(
        (v) => v.name === first,
    );
    let current = variable ? structOfType(analysis, imports, variable.type) : undefined;
    for (const part of parts.slice(1)) {
        const attribute = current?.struct.attributes.find((a) => a.name === part);
        current = attribute ? structOfType(analysis, imports, attribute.type) : undefined;
    }
    return current;
}

// Where the `.` before `start` is on `line`, or -1 when the name isn't a member.
function dotBefore(line: string, start: number): number {
    let i = start - 1;
    while (i >= 0 && (line[i] === " " || line[i] === "\t")) {
        i--;
    }
    return line[i] === "." ? i : -1;
}

// The struct whose members can be completed at `at` (right after `x.` or `x.pa`).
export function memberContext(analysis: Analysis, imports: Imports, at: Position): ImportedStruct | undefined {
    const line = analysis.lines[at.line] ?? "";
    if (isInComment(line, at.character)) {
        return undefined;
    }
    const before = /[A-Za-z_]\w*$/.exec(line.slice(0, at.character));
    const start = before ? before.index : at.character;
    const dot = dotBefore(line, start);
    return dot < 0 ? undefined : receiverStruct(analysis, imports, at.line, dot);
}

// What the identifier under the cursor refers to: something declared in this
// file, or brought in through `use` (`imports`).
export function resolve(analysis: Analysis, at: Position, imports: Imports = NO_IMPORTS): Resolved | undefined {
    const line = analysis.lines[at.line] ?? "";
    if (isInComment(line, at.character)) {
        return undefined;
    }
    const word = wordAt(line, at.character);
    if (!word || RESERVED.has(word.text)) {
        return undefined;
    }
    const isCall = /^\s*\(/.test(line.slice(word.end));
    const declaredHere = (item: { line: number; character: number }): boolean =>
        item.line === at.line && item.character === word.start;

    // the declarations themselves
    const ownStruct = analysis.structs.find(declaredHere);
    if (ownStruct) {
        return { kind: "struct", struct: ownStruct };
    }
    const own = analysis.functions.find(declaredHere);
    if (own) {
        const owner = own.owner === undefined ? undefined : analysis.structs.find((st) => st.name === own.owner);
        return owner ? { kind: "method", function: own, struct: owner } : { kind: "function", function: own };
    }
    for (const st of analysis.structs) {
        const attribute = st.attributes.find(declaredHere);
        if (attribute) {
            return { kind: "attribute", attribute, struct: st };
        }
    }

    // `x.name`: an attribute, or a method when it's called
    const dot = dotBefore(line, word.start);
    if (dot >= 0) {
        const receiver = receiverStruct(analysis, imports, at.line, dot);
        if (!receiver) {
            return undefined;
        }
        const { struct } = receiver;
        if (isCall) {
            const method = struct.methods.find((m) => m.name === word.text);
            return method ? { kind: "method", function: method, struct, ...fileOf(receiver) } : undefined;
        }
        const attribute = struct.attributes.find((a) => a.name === word.text);
        return attribute ? { kind: "attribute", attribute, struct, ...fileOf(receiver) } : undefined;
    }

    if (!isCall) {
        const variable = visibleVariables(analysis, { line: at.line, character: word.end + 1 }).find(
            (v) => v.name === word.text,
        );
        if (variable) {
            return { kind: "variable", variable };
        }
        const struct = findStruct(analysis, imports, word.text);
        if (struct) {
            return { kind: "struct", struct: struct.struct, ...fileOf(struct) };
        }
    }
    const fn = findFunction(analysis, word.text);
    if (fn) {
        return { kind: "function", function: fn };
    }
    const other = imports.functions.find((i) => i.fn.name === word.text && i.fn.owner === undefined);
    return other ? { kind: "function", function: other.fn, file: other.file } : undefined;
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
            const before = /([A-Za-z_]\w*)\s*$/.exec(line.slice(0, i));
            const name = before ? group(before, 1) : undefined;
            if (before && name && (!RESERVED.has(name) || findBuiltin(name))) {
                return { name, argument, start: before.index };
            }
            // a grouping parenthesis (or `if (`...): keep looking for the
            // call around it
        } else if (c === "," && depth === 0) {
            argument++;
        }
    }
    return undefined;
}

// `fn add(a i32, b i32) i32`; a method shows its struct: `fn User.add(n i32) i32`
// (`self` is implicit, so it isn't listed).
export function functionSignature(fn: FunctionInfo): string {
    const params = fn.params.map((p) => `${p.name} ${p.type}`.trim()).join(", ");
    const ret = fn.returnType === "void" ? "" : ` ${fn.returnType}`;
    const name = fn.owner === undefined ? fn.name : `${fn.owner}.${fn.name}`;
    return `${fn.keyword} ${name}(${params})${ret}`;
}

// `struct User` with its attributes, one per line.
export function structSignature(struct: StructInfo): string {
    return [`struct ${struct.name}:`, ...struct.attributes.map((a) => `    ${a.name} ${a.type}`)].join("\n");
}
