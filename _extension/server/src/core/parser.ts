// A light, line-based reader for ura source. It doesn't replace the compiler:
// it only finds what editor features need (functions, structs, enums, modules,
// variables, calls). Pure: no editor, LSP or file-system imports, so it runs
// in plain Node tests. All positions are 0-based.

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
    name: string; // `add`, or an operator's symbol (`+`, `[]`, `drop`)
    keyword: string; // "fn", "proto" (declared elsewhere) or "operator"
    isPub: boolean; // `pub fn`: called on the type, `Point::create()`
    params: Param[];
    returnType: string; // "void" when the header has none
    line: number;
    character: number; // where the name starts
    indent: number;
    bodyStart: number; // first line after the header
    bodyEnd: number; // last non-blank line of the body; < bodyStart when empty
    owner?: string; // the struct a method belongs to
    module?: string; // the `mod` it is declared in (`math`, `a::b`)
}

export interface VariableInfo {
    name: string;
    type: string; // "" when unknown (a `for` over something unknown)
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
    owner?: string; // a `use` inside a struct body pastes methods into that struct
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
    parents: string[]; // `struct B from A<T> and C`: ["A", "C"]
    line: number;
    character: number; // where the name starts
    indent: number;
    bodyStart: number;
    bodyEnd: number; // last non-blank line of the body
    attributes: AttributeInfo[];
    methods: FunctionInfo[];
    module?: string;
}

export interface VariantInfo {
    name: string;
    line: number;
    character: number;
    owner: string; // the enum it belongs to
}

export interface EnumInfo {
    name: string;
    line: number;
    character: number;
    indent: number;
    variants: VariantInfo[];
    module?: string;
}

export interface ModuleInfo {
    name: string;
    line: number;
    character: number;
    indent: number;
    bodyStart: number;
    bodyEnd: number;
}

export interface Analysis {
    lines: string[];
    functions: FunctionInfo[];
    variables: VariableInfo[];
    uses: UseInfo[];
    structs: StructInfo[];
    enums: EnumInfo[];
    modules: ModuleInfo[];
}

// Where a name is written: one line, `length` characters from `start`.
export interface Span {
    line: number;
    start: number;
    length: number;
}

// Something declared in another file and brought in through `use`.
export interface ImportedFunction {
    file: string;
    fn: FunctionInfo;
}

export interface ImportedStruct {
    file: string; // "" when it is declared in the file being looked at
    struct: StructInfo;
}

export interface ImportedEnum {
    file: string;
    enumeration: EnumInfo;
}

// What a file sees from other files. `methods` are functions pasted into a
// struct by a `use` inside its body; each one's `owner` names the struct.
export interface Imports {
    functions: ImportedFunction[];
    structs: ImportedStruct[];
    enums: ImportedEnum[];
    methods: ImportedFunction[];
}

export const NO_IMPORTS: Imports = { functions: [], structs: [], enums: [], methods: [] };

export interface CallContext {
    name: string;
    argument: number; // which argument the cursor is in, 0-based
    start: number; // where the called name starts
}

const IDENT = /[A-Za-z_]\w*/;
// a type: `i32`, `&Point`, `T[]`, `Array<String>`, `geo::Point`
const TYPE = String.raw`&*[A-Za-z_][\w:]*(?:<[^=]*>)?(?:\[\])*`;
const FN_HEADER = /^(\s*)(pub\s+)?(fn|proto)\s+([A-Za-z_]\w*)\s*\(/;
// `operator +(`, `operator [](`, `operator in(`, `operator drop(`
const OPERATOR_HEADER = /^(\s*)(operator)\s*([^\s(]+)\s*\(/;
// `name type` or `name type = value`
const DECLARATION = new RegExp(String.raw`^(\s*)([A-Za-z_]\w*)(\s+)(${TYPE})\s*(=.*)?$`);
const USE = /^(\s*)use\s+"([^"]*)"/;
// `struct Name:`, `struct Name from A<T> and B:`
const STRUCT = /^(\s*struct\s+)([A-Za-z_]\w*)(?:\s+from\s+([^:]*))?/;
const ENUM = /^(\s*)enum\s+([A-Za-z_]\w*)/;
const MODULE = /^(\s*)mod\s+([A-Za-z_]\w*)/;
const FOR = /^(\s*)for\s+([A-Za-z_]\w*)\s+in\s+(.+?)\s*:\s*$/;
const CATCH = new RegExp(String.raw`^(\s*)catch\s+([A-Za-z_]\w*)\s+(${TYPE})\s*:`);
// a value written as names, members and indexes: `p`, `self.shapes[i].center`
const CHAIN_PART = /\.\s*[A-Za-z_]\w*|\[[^\]]*\]/g;
const CHAIN_END = /([A-Za-z_]\w*(?:\s*(?:\.\s*[A-Za-z_]\w*|\[[^\]]*\]))*)\s*$/;

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

// Split on commas that aren't inside brackets, keeping each piece's offset.
function splitTopLevel(text: string, offset: number): Piece[] {
    const pieces: Piece[] = [];
    let depth = 0;
    let start = 0;
    for (let i = 0; i < text.length; i++) {
        const c = text[i] ?? "";
        if ("([{<".includes(c)) {
            depth++;
        } else if (")]}>".includes(c)) {
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
        const match = /^([A-Za-z_]\w*)\s*(.*)$/.exec(piece.text.trim());
        if (!match) {
            continue; // `...`
        }
        params.push({ name: group(match, 1), type: group(match, 2).trim(), line, character: piece.offset + lead });
    }
    return params;
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

interface Header {
    indent: number;
    keyword: string;
    isPub: boolean;
    name: string;
    character: number;
    open: number; // the '(' of the parameter list
}

function parseHeader(text: string): Header | undefined {
    const fn = FN_HEADER.exec(text);
    if (fn) {
        const indent = group(fn, 1).length;
        const name = group(fn, 4);
        const character = group(fn, 0).lastIndexOf(name);
        return { indent, keyword: group(fn, 3), isPub: group(fn, 2) !== "", name, character, open: group(fn, 0).length - 1 };
    }
    const op = OPERATOR_HEADER.exec(text);
    if (op) {
        const indent = group(op, 1).length;
        const name = group(op, 3);
        const character = text.indexOf(name, indent + "operator".length);
        return { indent, keyword: "operator", isPub: false, name, character, open: group(op, 0).length - 1 };
    }
    return undefined;
}

function parseFunction(lines: string[], line: number): FunctionInfo | undefined {
    const text = stripComment(lines[line] ?? "");
    const header = parseHeader(text);
    if (!header) {
        return undefined;
    }
    const { indent, keyword, isPub, name, character, open } = header;

    // find the ')' that closes the parameter list
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
    const rest = close < 0 ? "" : text.slice(close + 1).trim().replace(/:$/, "").trim();
    const returnType = rest || "void";
    const bodyEnd = bodyEndOf(lines, line, indent);
    return { name, keyword, isPub, params, returnType, line, character, indent, bodyStart: line + 1, bodyEnd };
}

function parseStruct(lines: string[], line: number): StructInfo | undefined {
    const match = STRUCT.exec(stripComment(lines[line] ?? ""));
    if (!match) {
        return undefined;
    }
    const indent = indentOf(lines[line] ?? "");
    const parents = group(match, 3)
        .split(/\s+and\s+|,/)
        .map((parent) => parent.replace(/<.*$/, "").trim())
        .filter((parent) => parent !== "");
    return {
        name: group(match, 2),
        parents,
        line,
        character: group(match, 1).length,
        indent,
        bodyStart: line + 1,
        bodyEnd: bodyEndOf(lines, line, indent),
        attributes: [],
        methods: [],
    };
}

// The variants written in `text` from `from` on: `RED, GREEN = 4, BLUE`.
function variantsOn(text: string, line: number, from: number, owner: string): VariantInfo[] {
    return splitTopLevel(text.slice(from), from).flatMap((piece) => {
        const match = /^\s*([A-Za-z_]\w*)/.exec(piece.text);
        if (!match) {
            return [];
        }
        const name = group(match, 1);
        return [{ name, line, character: piece.offset + piece.text.indexOf(name), owner }];
    });
}

// `enum Color: RED, GREEN`, or one variant (or more) per indented line.
function parseEnum(lines: string[], line: number): EnumInfo | undefined {
    const text = stripComment(lines[line] ?? "");
    const match = ENUM.exec(text);
    if (!match) {
        return undefined;
    }
    const indent = group(match, 1).length;
    const name = group(match, 2);
    const colon = text.indexOf(":");
    const variants = colon < 0 ? [] : variantsOn(text, line, colon + 1, name);
    const bodyEnd = bodyEndOf(lines, line, indent);
    for (let i = line + 1; i <= bodyEnd; i++) {
        variants.push(...variantsOn(stripComment(lines[i] ?? ""), i, 0, name));
    }
    return { name, line, character: group(match, 0).length - name.length, indent, variants };
}

function parseModule(lines: string[], line: number): ModuleInfo | undefined {
    const match = MODULE.exec(stripComment(lines[line] ?? ""));
    if (!match) {
        return undefined;
    }
    const indent = group(match, 1).length;
    const name = group(match, 2);
    const bodyEnd = bodyEndOf(lines, line, indent);
    return { name, line, character: group(match, 0).length - name.length, indent, bodyStart: line + 1, bodyEnd };
}

function inBody(block: { bodyStart: number; bodyEnd: number }, line: number): boolean {
    return line >= block.bodyStart && line <= block.bodyEnd;
}

// The innermost struct whose body holds `line`.
function enclosingStruct(structs: StructInfo[], line: number): StructInfo | undefined {
    let found: StructInfo | undefined;
    for (const st of structs) {
        if (inBody(st, line) && (found === undefined || st.line > found.line)) {
            found = st;
        }
    }
    return found;
}

function enclosingFunction(functions: FunctionInfo[], line: number): FunctionInfo | undefined {
    let found: FunctionInfo | undefined;
    for (const fn of functions) {
        if (inBody(fn, line)) {
            found = fn;
        }
    }
    return found;
}

// `a::b` for a line inside `mod b:` inside `mod a:`; undefined outside any mod.
function moduleOf(modules: ModuleInfo[], line: number): string | undefined {
    const names = modules.filter((mod) => inBody(mod, line)).map((mod) => mod.name);
    return names.length === 0 ? undefined : names.join("::");
}

function withModule<T extends { module?: string }>(item: T, module: string | undefined): void {
    if (module !== undefined) {
        item.module = module;
    }
}

export function analyze(source: string): Analysis {
    const lines = source.split(/\r?\n/);
    const functions: FunctionInfo[] = [];
    const structs: StructInfo[] = [];
    const enums: EnumInfo[] = [];
    const modules: ModuleInfo[] = [];
    for (let line = 0; line < lines.length; line++) {
        const fn = parseFunction(lines, line);
        if (fn) {
            functions.push(fn);
        }
        const st = parseStruct(lines, line);
        if (st) {
            structs.push(st);
        }
        const en = parseEnum(lines, line);
        if (en) {
            enums.push(en);
        }
        const mod = parseModule(lines, line);
        if (mod) {
            modules.push(mod);
        }
    }

    // a function written inside a struct is one of its methods
    for (const fn of functions) {
        const owner = fn.keyword === "proto" ? undefined : enclosingStruct(structs, fn.line);
        if (owner) {
            fn.owner = owner.name;
            owner.methods.push(fn);
        } else {
            withModule(fn, moduleOf(modules, fn.line));
        }
    }
    for (const st of structs) {
        withModule(st, moduleOf(modules, st.line));
    }
    for (const en of enums) {
        withModule(en, moduleOf(modules, en.line));
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
    const loops: { variable: VariableInfo; iterable: string }[] = [];
    for (let line = 0; line < lines.length; line++) {
        const text = stripComment(lines[line] ?? "");
        const fn = enclosingFunction(functions, line);
        const loop = FOR.exec(text) ?? CATCH.exec(text);
        if (loop) {
            // visible in the block below its header
            const name = group(loop, 2);
            const header = group(loop, 1).length;
            const character = text.indexOf(name, header + 4);
            const isFor = text.trimStart().startsWith("for");
            const variable = { name, type: isFor ? "" : group(loop, 3), line, character, indent: header + 1, fn, isParam: false };
            variables.push(variable);
            if (isFor) {
                loops.push({ variable, iterable: group(loop, 3) });
            }
            continue;
        }
        const match = DECLARATION.exec(text);
        if (!match || RESERVED.has(group(match, 2))) {
            continue;
        }
        const name = group(match, 2);
        const type = group(match, 4).trim();
        const character = group(match, 1).length;
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
        const use: UseInfo = { target: group(match, 2), line, indent: group(match, 1).length, start, end };
        const owner = enclosingStruct(structs, line);
        if (owner && enclosingFunction(functions, line) === undefined) {
            use.owner = owner.name;
        }
        uses.push(use);
    }

    const analysis = { lines, functions, variables, uses, structs, enums, modules };
    // a `for` variable is one element of what it walks
    for (const { variable, iterable } of loops) {
        const at = { line: variable.line, character: 0 };
        const walked = /^Range::create\b/.test(iterable) ? "Array<i32>" : chainType(analysis, NO_IMPORTS, iterable, at);
        variable.type = (walked === undefined ? undefined : elementOf(walked)) ?? "";
    }
    return analysis;
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
// (if / while / for / match ...) end their variables, like the compiler's scopes.
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
    | { kind: "attribute"; attribute: AttributeInfo; struct: StructInfo; file?: string }
    | { kind: "enum"; enumeration: EnumInfo; file?: string }
    | { kind: "variant"; variant: VariantInfo; enumeration: EnumInfo; file?: string };

const fileOf = (found: { file: string }): { file?: string } => (found.file ? { file: found.file } : {});

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

export function findEnum(analysis: Analysis, imports: Imports, name: string): ImportedEnum | undefined {
    const own = analysis.enums.find((en) => en.name === name);
    if (own) {
        return { file: "", enumeration: own };
    }
    return imports.enums.find((i) => i.enumeration.name === name);
}

// `found` and every struct it takes members from (`struct B from A`), nearest first.
export function lineage(analysis: Analysis, imports: Imports, found: ImportedStruct): ImportedStruct[] {
    const result: ImportedStruct[] = [];
    const seen = new Set<string>();
    const queue = [found];
    for (let next = queue.shift(); next !== undefined; next = queue.shift()) {
        if (seen.has(next.struct.name)) {
            continue;
        }
        seen.add(next.struct.name);
        result.push(next);
        for (const parent of next.struct.parents) {
            const struct = findStruct(analysis, imports, parent);
            if (struct) {
                queue.push(struct);
            }
        }
    }
    return result;
}

// A member and where it is declared: `struct` declares it (maybe a parent),
// `file` holds it ("" for the file being looked at).
export interface Member<T> {
    item: T;
    struct: StructInfo;
    file: string;
}

export interface Members {
    attributes: Member<AttributeInfo>[];
    methods: Member<FunctionInfo>[];
}

// Keeps the first member of each name: a child's member overrides its parent's.
function nearest<T extends { name: string }>(members: Member<T>[]): Member<T>[] {
    const seen = new Set<string>();
    return members.filter((m) => !seen.has(m.item.name) && seen.add(m.item.name));
}

// The attributes and methods of a struct, with the ones it inherits and the
// ones pasted into it by a `use` in its body. Operators are left out.
export function membersOf(analysis: Analysis, imports: Imports, found: ImportedStruct): Members {
    const attributes: Member<AttributeInfo>[] = [];
    const methods: Member<FunctionInfo>[] = [];
    for (const { struct, file } of lineage(analysis, imports, found)) {
        attributes.push(...struct.attributes.map((item) => ({ item, struct, file })));
        const own = struct.methods.filter((fn) => fn.keyword === "fn");
        methods.push(...own.map((item) => ({ item, struct, file })));
        const pasted = imports.methods.filter((m) => m.fn.owner === struct.name && m.fn.keyword === "fn");
        methods.push(...pasted.map((m) => ({ item: m.fn, struct, file: m.file })));
    }
    return { attributes: nearest(attributes), methods: nearest(methods) };
}

// The struct a type names (`User`, `&User`, `Array<i32>`, `geo::Point`), or
// undefined (`i32`, `User[]`).
function structOfType(analysis: Analysis, imports: Imports, type: string): ImportedStruct | undefined {
    if (type.endsWith("[]")) {
        return undefined;
    }
    const name = type.replace(/^&+/, "").replace(/<.*$/, "").replace(/^.*::/, "");
    return /^[A-Za-z_]\w*$/.test(name) ? findStruct(analysis, imports, name) : undefined;
}

// The type of one element of `type`: `T[]` and `Array<T>` give `T`, `String` gives `char`.
export function elementOf(type: string): string | undefined {
    const plain = type.replace(/^&+/, "");
    if (plain.endsWith("[]")) {
        return plain.slice(0, -2);
    }
    const array = /^Array<(.*)>$/.exec(plain);
    if (array) {
        return group(array, 1).trim();
    }
    return plain === "String" ? "char" : undefined;
}

// The type of a value written as a chain (`p`, `self.shapes[i].center`), seen from `at`.
function chainType(analysis: Analysis, imports: Imports, chain: string, at: Position): string | undefined {
    const text = chain.trim();
    const first = IDENT.exec(text);
    if (!first || first.index !== 0) {
        return undefined;
    }
    const rest = text.slice(group(first, 0).length);
    const parts = rest.match(CHAIN_PART) ?? [];
    if (parts.join("").replace(/\s/g, "") !== rest.replace(/\s/g, "")) {
        return undefined; // a call or an operator: not a plain chain
    }
    let type = visibleVariables(analysis, at).find((v) => v.name === group(first, 0))?.type;
    for (const part of parts) {
        if (!type) {
            return undefined;
        }
        if (part.startsWith("[")) {
            type = elementOf(type);
            continue;
        }
        const struct = structOfType(analysis, imports, type);
        const name = part.slice(1).trim();
        type = struct ? membersOf(analysis, imports, struct).attributes.find((a) => a.item.name === name)?.item.type : undefined;
    }
    return type;
}

// The struct of the value that ends right before `dot` on `line`
// (`u` in `u.a`, `self.shapes[i]` in `self.shapes[i].hit`), or undefined.
function receiverStruct(analysis: Analysis, imports: Imports, line: number, dot: number): ImportedStruct | undefined {
    const text = analysis.lines[line] ?? "";
    const chain = CHAIN_END.exec(text.slice(0, dot));
    if (!chain) {
        return undefined;
    }
    const first = IDENT.exec(group(chain, 1));
    const at = { line, character: chain.index + (first ? group(first, 0).length : 0) + 1 };
    const type = chainType(analysis, imports, group(chain, 1), at);
    return type ? structOfType(analysis, imports, type) : undefined;
}

// Where the `.` before `start` is on `line`, or -1 when the name isn't a member.
function dotBefore(line: string, start: number): number {
    let i = start - 1;
    while (i >= 0 && (line[i] === " " || line[i] === "\t")) {
        i--;
    }
    return line[i] === "." ? i : -1;
}

// The name right before `::` in `Name::start`, or undefined.
function scopeBefore(line: string, start: number): string | undefined {
    const scope = /([A-Za-z_]\w*)\s*::\s*$/.exec(line.slice(0, start));
    return scope ? group(scope, 1) : undefined;
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

// `Scope::name`: a variant of an enum, a method of a struct (`Point::create`),
// or something declared in a module (`math::add`).
function resolveScoped(analysis: Analysis, imports: Imports, scope: string, name: string): Resolved | undefined {
    const enumeration = findEnum(analysis, imports, scope);
    if (enumeration) {
        const variant = enumeration.enumeration.variants.find((v) => v.name === name);
        return variant ? { kind: "variant", variant, enumeration: enumeration.enumeration, ...fileOf(enumeration) } : undefined;
    }
    const struct = findStruct(analysis, imports, scope);
    if (struct) {
        const method = membersOf(analysis, imports, struct).methods.find((m) => m.item.name === name);
        return method ? { kind: "method", function: method.item, struct: method.struct, ...fileOf(method) } : undefined;
    }
    return resolveName(analysis, imports, name, true);
}

// A plain name: a struct or enum (not when called), else a function.
function resolveName(analysis: Analysis, imports: Imports, name: string, isCall: boolean): Resolved | undefined {
    if (!isCall) {
        const struct = findStruct(analysis, imports, name);
        if (struct) {
            return { kind: "struct", struct: struct.struct, ...fileOf(struct) };
        }
        const enumeration = findEnum(analysis, imports, name);
        if (enumeration) {
            return { kind: "enum", enumeration: enumeration.enumeration, ...fileOf(enumeration) };
        }
    }
    const fn = findFunction(analysis, name);
    if (fn) {
        return { kind: "function", function: fn };
    }
    const other = imports.functions.find((i) => i.fn.name === name && i.fn.owner === undefined);
    return other ? { kind: "function", function: other.fn, file: other.file } : undefined;
}

// The declaration written exactly at (`line`, `character`), if any.
function declaredAt(analysis: Analysis, line: number, character: number): Resolved | undefined {
    const here = (item: { line: number; character: number }): boolean => item.line === line && item.character === character;
    const struct = analysis.structs.find(here);
    if (struct) {
        return { kind: "struct", struct };
    }
    const enumeration = analysis.enums.find(here);
    if (enumeration) {
        return { kind: "enum", enumeration };
    }
    for (const en of analysis.enums) {
        const variant = en.variants.find(here);
        if (variant) {
            return { kind: "variant", variant, enumeration: en };
        }
    }
    const fn = analysis.functions.find(here);
    if (fn) {
        const owner = analysis.structs.find((st) => st.name === fn.owner);
        return owner ? { kind: "method", function: fn, struct: owner } : { kind: "function", function: fn };
    }
    for (const st of analysis.structs) {
        const attribute = st.attributes.find(here);
        if (attribute) {
            return { kind: "attribute", attribute, struct: st };
        }
    }
    return undefined;
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
    const declaration = declaredAt(analysis, at.line, word.start);
    if (declaration) {
        return declaration;
    }
    // `f(`, `f<T>(`
    const isCall = /^\s*(?:<[^()]*>)?\s*\(/.test(line.slice(word.end));

    // `x.name`: an attribute, or a method when it's called
    const dot = dotBefore(line, word.start);
    if (dot >= 0) {
        const receiver = receiverStruct(analysis, imports, at.line, dot);
        if (!receiver) {
            return undefined;
        }
        const members = membersOf(analysis, imports, receiver);
        if (isCall) {
            const method = members.methods.find((m) => m.item.name === word.text);
            return method ? { kind: "method", function: method.item, struct: method.struct, ...fileOf(method) } : undefined;
        }
        const attribute = members.attributes.find((a) => a.item.name === word.text);
        return attribute ? { kind: "attribute", attribute: attribute.item, struct: attribute.struct, ...fileOf(attribute) } : undefined;
    }

    const scope = scopeBefore(line, word.start);
    if (scope !== undefined) {
        return resolveScoped(analysis, imports, scope, word.text);
    }

    if (!isCall) {
        const variable = visibleVariables(analysis, { line: at.line, character: word.end + 1 }).find(
            (v) => v.name === word.text,
        );
        if (variable) {
            return { kind: "variable", variable };
        }
    }
    return resolveName(analysis, imports, word.text, isCall);
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
            // `name(` or `name<T>(`
            const before = /([A-Za-z_]\w*)\s*(?:<[^()]*>)?\s*$/.exec(line.slice(0, i));
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

// `fn add(a i32) i32`, `fn User.add(n i32) i32` (`self` is implicit),
// `pub fn Point::create(x i32) Point`, `fn math::add(a i32) i32`, `operator +(o &Vec3) Vec3`.
export function functionSignature(fn: FunctionInfo): string {
    const params = fn.params.map((p) => `${p.name} ${p.type}`.trim()).join(", ");
    const ret = fn.returnType === "void" ? "" : ` ${fn.returnType}`;
    if (fn.keyword === "operator") {
        return `operator ${fn.name}(${params})${ret}`;
    }
    let name = fn.name;
    if (fn.owner !== undefined) {
        name = `${fn.owner}${fn.isPub ? "::" : "."}${fn.name}`;
    } else if (fn.module !== undefined) {
        name = `${fn.module}::${fn.name}`;
    }
    return `${fn.isPub ? "pub " : ""}${fn.keyword} ${name}(${params})${ret}`;
}

// `struct User from Base:` with its attributes, one per line.
export function structSignature(struct: StructInfo): string {
    const parents = struct.parents.length === 0 ? "" : ` from ${struct.parents.join(" and ")}`;
    return [`struct ${struct.name}${parents}:`, ...struct.attributes.map((a) => `    ${a.name} ${a.type}`)].join("\n");
}

// `enum Color: RED, GREEN, BLUE`
export function enumSignature(enumeration: EnumInfo): string {
    return `enum ${enumeration.name}: ${enumeration.variants.map((v) => v.name).join(", ")}`;
}
