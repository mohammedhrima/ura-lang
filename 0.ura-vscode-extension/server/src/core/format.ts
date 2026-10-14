// The ura formatter. Pure: text in, text out.
//
// It never changes what a program means:
// - indentation keeps the compiler's levels (main.c tokenize: a level is 4
//   whitespace characters at the start of a line, rounded up; a tab counts as 1),
// - inside a line only the spaces between tokens change; strings, characters
//   and comments are copied as they are,
// - a line with a character the formatter doesn't know keeps its spacing.

const TAB = 4;

type Kind = "word" | "number" | "string" | "operator" | "open" | "close" | "comma" | "colon" | "dot" | "comment" | "other";

interface Token {
    kind: Kind;
    text: string;
}

// Longest first, so `+=` wins over `+` and `...` over `.`.
const OPERATORS = ["...", "::", "==", "!=", "<=", ">=", "+=", "-=", "*=", "/=", "%=", "&&", "||", "+", "-", "*", "/", "%", "<", ">", "=", "&", "!"];
// Operators written right against what follows them.
const PREFIX = new Set(["&", "!"]);
// After these a `-` is a sign, not a subtraction.
const SIGN_AFTER_WORDS = new Set(["return", "and", "or", "if", "elif", "while"]);

// The leading whitespace of `line` as a number of compiler levels.
export function levelOf(line: string): number {
    const width = line.length - line.trimStart().length;
    return Math.ceil(width / TAB);
}

function scanQuoted(text: string, start: number): number {
    const quote = text[start];
    let i = start + 1;
    while (i < text.length && text[i] !== quote) {
        i += text[i] === "\\" ? 2 : 1;
    }
    return Math.min(i + 1, text.length);
}

// The tokens of one line's code (no leading whitespace), or undefined when the
// line holds something the formatter doesn't know.
function tokenize(code: string): Token[] | undefined {
    const tokens: Token[] = [];
    let i = 0;
    while (i < code.length) {
        const c = code[i] ?? "";
        if (c === " " || c === "\t") {
            i++;
            continue;
        }
        const rest = code.slice(i);
        if (rest.startsWith("//")) {
            tokens.push({ kind: "comment", text: rest.trimEnd() });
            break;
        }
        if (rest.startsWith("/*")) {
            const end = code.indexOf("*/", i + 2);
            if (end < 0) {
                return undefined; // a block comment continues on the next lines
            }
            tokens.push({ kind: "comment", text: code.slice(i, end + 2) });
            i = end + 2;
            continue;
        }
        if (c === '"' || c === "'") {
            const end = scanQuoted(code, i);
            tokens.push({ kind: "string", text: code.slice(i, end) });
            i = end;
            continue;
        }
        const number = /^\d+(\.\d+)?/.exec(rest);
        if (number) {
            tokens.push({ kind: "number", text: number[0] });
            i += number[0].length;
            continue;
        }
        const word = /^[A-Za-z_@$][\w]*/.exec(rest);
        if (word) {
            tokens.push({ kind: "word", text: word[0] });
            i += word[0].length;
            continue;
        }
        const operator = OPERATORS.find((op) => rest.startsWith(op));
        if (operator) {
            tokens.push({ kind: operator === "..." ? "word" : "operator", text: operator });
            i += operator.length;
            continue;
        }
        const single: Partial<Record<string, Kind>> = {
            "(": "open",
            "[": "open",
            ")": "close",
            "]": "close",
            ",": "comma",
            ":": "colon",
            ".": "dot",
        };
        const kind = single[c];
        if (!kind) {
            return undefined;
        }
        tokens.push({ kind, text: c });
        i++;
    }
    return tokens;
}

// Whether the operator at `index` is a prefix (sign, `&a`, `&i32`, `!x`).
function isPrefix(tokens: Token[], index: number): boolean {
    const token = tokens[index];
    if (!token || token.kind !== "operator") {
        return false;
    }
    if (PREFIX.has(token.text)) {
        return true;
    }
    if (token.text !== "-") {
        return false;
    }
    const prev = tokens[index - 1];
    return (
        prev === undefined ||
        prev.kind === "operator" ||
        prev.kind === "open" ||
        prev.kind === "comma" ||
        prev.kind === "colon" ||
        (prev.kind === "word" && SIGN_AFTER_WORDS.has(prev.text))
    );
}

// The space written between tokens[index - 1] and tokens[index]. Binary
// operators and words get the default: one space.
function gap(tokens: Token[], index: number): string {
    const prev = tokens[index - 1];
    const next = tokens[index];
    if (!prev || !next) {
        return "";
    }
    if (next.kind === "comment") {
        return " ";
    }
    if (next.kind === "comma" || next.kind === "colon" || next.kind === "close") {
        return "";
    }
    if (prev.kind === "comma" || prev.kind === "colon") {
        return " ";
    }
    if (prev.kind === "open") {
        return "";
    }
    if (prev.kind === "dot" || next.kind === "dot" || prev.text === "::" || next.text === "::") {
        return "";
    }
    if (isPrefix(tokens, index - 1)) {
        return "";
    }
    if (next.kind === "open") {
        // calls, indexes and types (`f(`, `a[`, `i8[]`) are tight; `return (`, `= [` are not
        const tight = (prev.kind === "word" && !SIGN_AFTER_WORDS.has(prev.text)) || prev.kind === "close";
        return tight ? "" : " ";
    }
    return " ";
}

// One line's code with the spaces between tokens rewritten; undefined when the
// line must be kept as written.
function formatCode(code: string): string | undefined {
    const tokens = tokenize(code);
    if (!tokens) {
        return undefined;
    }
    let out = "";
    tokens.forEach((token, index) => {
        out += gap(tokens, index) + token.text;
    });
    return out;
}

// One line: indentation by level, then the code.
export function formatLine(line: string): string {
    const code = line.trim();
    if (code === "") {
        return "";
    }
    const indent = " ".repeat(levelOf(line) * TAB);
    return indent + (formatCode(code) ?? code);
}

type TopKind = "fn" | "struct" | "proto" | "use" | "other";

function topKind(line: string): TopKind {
    const word = /^(fn|struct|proto|use)\b/.exec(line);
    return (word?.[1] as TopKind | undefined) ?? "other";
}

function isComment(line: string): boolean {
    return line.trimStart().startsWith("//");
}

// The whole document: every line formatted, then the blank lines:
// one between top-level fn / struct / proto / use groups (a comment right above
// one goes with it), at most one anywhere else, none at the start or the end.
interface FormattedLines {
    lines: string[];
    comment: boolean[]; // line i is a comment (// or part of a /* */ block)
}

// Every line formatted on its own; lines inside a multi-line block comment
// are kept exactly.
function formatEachLine(input: string[]): FormattedLines {
    const lines: string[] = [];
    const comment: boolean[] = [];
    let inBlock = false;
    for (const raw of input) {
        if (inBlock) {
            lines.push(raw.trimEnd());
            comment.push(true);
            inBlock = !raw.includes("*/");
            continue;
        }
        const opens = raw.lastIndexOf("/*");
        if (opens >= 0 && raw.indexOf("*/", opens) < 0 && !isComment(raw)) {
            lines.push(raw.trimEnd());
            comment.push(raw.trimStart().startsWith("/*"));
            inBlock = true;
            continue;
        }
        const line = formatLine(raw);
        lines.push(line);
        comment.push(isComment(line));
    }
    return { lines, comment };
}

// Lines `first`..`last` (0-based, inclusive) formatted, without touching blank
// lines: what range formatting writes back over those lines.
export function formatLines(source: string, first: number, last: number): string[] {
    return formatEachLine(source.split(/\r?\n/)).lines.slice(first, last + 1);
}

export function formatDocument(source: string): string {
    const eol = source.includes("\r\n") ? "\r\n" : "\n";
    const { lines, comment } = formatEachLine(source.split(/\r?\n/));
    const isCommentAt = (i: number): boolean => comment[i] === true;

    const out: string[] = [];
    let previousTop: TopKind | undefined; // kind of the last top-level line written
    let pendingBlank = false;
    let firstCode = true;
    for (let i = 0; i < lines.length; i++) {
        const line = lines[i] ?? "";
        if (line === "") {
            pendingBlank = !firstCode;
            continue;
        }
        const topLevel = levelOf(line) === 0;
        // a top-level comment block takes the kind of the line it sits on
        let kind: TopKind = "other";
        if (topLevel) {
            let j = i;
            while (j < lines.length && isCommentAt(j)) {
                j++;
            }
            kind = topKind(lines[j] ?? "");
        }
        // a comment right above a top-level line already took the blank line
        const startsGroup = topLevel && kind !== "other" && !isCommentAt(i - 1) && !firstCode;
        if (startsGroup) {
            const sameGroup = (kind === "use" || kind === "proto") && previousTop === kind;
            if (!sameGroup || pendingBlank) {
                out.push("");
            }
        } else if (pendingBlank && !firstCode) {
            out.push("");
        }
        out.push(line);
        pendingBlank = false;
        firstCode = false;
        if (topLevel && !isCommentAt(i)) {
            previousTop = topKind(line);
        }
    }
    return out.length === 0 ? "" : out.join(eol) + eol;
}
