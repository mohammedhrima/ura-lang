"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;
const vscode = __importStar(require("vscode"));
const path = __importStar(require("path"));
const fs = __importStar(require("fs"));
// ─────────────────────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────────────────────
function getWorkspaceRoot() {
    return vscode.workspace.workspaceFolders?.[0]?.uri.fsPath;
}
function getUraLibDir() {
    const root = getWorkspaceRoot();
    if (!root)
        return undefined;
    const envLib = process.env["URA_LIB"];
    if (envLib && fs.existsSync(envLib))
        return envLib;
    const candidate = path.join(root, "src", "ura-lib");
    return fs.existsSync(candidate) ? candidate : undefined;
}
function resolveImportPath(importPath, currentFilePath) {
    if (importPath.startsWith("@/")) {
        const libDir = getUraLibDir();
        if (!libDir)
            return undefined;
        return path.join(libDir, importPath.slice(2) + ".ura");
    }
    return path.join(path.dirname(currentFilePath), importPath + ".ura");
}
function getImportedModules(text) {
    const imported = new Set();
    const re = /^\s*use\s+"@\/([^"]+)"/gm;
    let m;
    while ((m = re.exec(text)) !== null)
        imported.add(m[1]);
    return imported;
}
function getInsertLineForImport(lines) {
    let last = -1;
    for (let i = 0; i < lines.length; i++) {
        if (/^\s*use\s+"/.test(lines[i]))
            last = i;
    }
    return last + 1;
}
// ─────────────────────────────────────────────────────────────
//  Signature parsing
//  Matches: (proto )?fn name(param1 type1, ...) returnType
// ─────────────────────────────────────────────────────────────
const SIG_RE = /^\s*(proto\s+)?fn\s+([a-zA-Z_]\w*)\s*\(([^)]*)\)\s*([a-zA-Z_][\w\[\]]*)/;
function parseParams(paramsStr) {
    const result = [];
    const parts = paramsStr.split(",").map((s) => s.trim()).filter(Boolean);
    for (const part of parts) {
        if (part === "...") {
            result.push({ name: "...", type: "", variadic: true });
            continue;
        }
        // May be "name type" or "name type ref" etc.
        const tokens = part.split(/\s+/);
        if (tokens.length >= 2) {
            result.push({ name: tokens[0], type: tokens.slice(1).join(" "), variadic: false });
        }
        else if (tokens.length === 1) {
            result.push({ name: tokens[0], type: "", variadic: false });
        }
    }
    return result;
}
function parseLine(line, uri, lineNum, module) {
    const m = SIG_RE.exec(line);
    if (!m)
        return undefined;
    return {
        name: m[2],
        module,
        uri,
        line: lineNum,
        params: parseParams(m[3]),
        returnType: m[4] || "void",
        isProto: !!m[1],
    };
}
function buildSignatureLabel(fn) {
    const params = fn.params
        .map((p) => (p.variadic ? "..." : `${p.name} ${p.type}`))
        .join(", ");
    const prefix = fn.isProto ? "proto fn" : "fn";
    return `${prefix} ${fn.name}(${params}) ${fn.returnType}`;
}
// ─────────────────────────────────────────────────────────────
//  Indexes
// ─────────────────────────────────────────────────────────────
let libIndex = [];
let workspaceIndex = [];
let headerModules = new Set();
function buildHeaderModules() {
    headerModules = new Set();
    const libDir = getUraLibDir();
    if (!libDir)
        return;
    const headerPath = path.join(libDir, "header.ura");
    if (!fs.existsSync(headerPath))
        return;
    const text = fs.readFileSync(headerPath, "utf8");
    // header.ura uses relative imports like: use "io"
    const re = /^\s*use\s+"([^@"][^"]*)"/gm;
    let m;
    while ((m = re.exec(text)) !== null)
        headerModules.add(m[1]);
}
function buildLibIndex() {
    libIndex = [];
    const libDir = getUraLibDir();
    if (!libDir || !fs.existsSync(libDir))
        return;
    for (const filename of fs.readdirSync(libDir)) {
        if (!filename.endsWith(".ura"))
            continue;
        const module = filename.slice(0, -4);
        const filePath = path.join(libDir, filename);
        const uri = vscode.Uri.file(filePath);
        const lines = fs.readFileSync(filePath, "utf8").split("\n");
        for (let i = 0; i < lines.length; i++) {
            const fn = parseLine(lines[i], uri, i, module);
            if (fn)
                libIndex.push(fn);
        }
    }
    buildHeaderModules();
}
/** Choose which import to insert for a ura-lib module, and whether it's already covered. */
function chooseImport(module, docText) {
    const imported = getImportedModules(docText);
    // @/header already imported → covers everything
    if (imported.has("header")) {
        return { importStr: "@/header", alreadyImported: true };
    }
    // Module is covered by header → prefer @/header over specific module
    if (headerModules.has(module)) {
        if (imported.has(module)) {
            return { importStr: `@/${module}`, alreadyImported: true };
        }
        return { importStr: "@/header", alreadyImported: false };
    }
    // Module not in header → import specifically
    return { importStr: `@/${module}`, alreadyImported: imported.has(module) };
}
async function buildWorkspaceIndex() {
    workspaceIndex = [];
    const uris = await vscode.workspace.findFiles("**/*.ura", "{**/build/**,**/.git/**}");
    for (const uri of uris) {
        const text = (await vscode.workspace.fs.readFile(uri)).toString();
        const lines = text.split("\n");
        for (let i = 0; i < lines.length; i++) {
            const fn = parseLine(lines[i], uri, i, "");
            if (fn)
                workspaceIndex.push(fn);
        }
    }
}
// Look up a function by name — lib first, then workspace
function lookupFn(name) {
    return (libIndex.find((f) => f.name === name) ||
        workspaceIndex.find((f) => f.name === name));
}
// ─────────────────────────────────────────────────────────────
//  Call context (for signature help)
//  Walks backwards from cursor to find enclosing fn call name
//  and current active parameter index.
// ─────────────────────────────────────────────────────────────
function getCallContext(doc, pos) {
    const offset = doc.offsetAt(pos);
    const text = doc.getText();
    let depth = 0;
    let activeParam = 0;
    for (let i = offset - 1; i >= 0; i--) {
        const ch = text[i];
        if (ch === ")") {
            depth++;
        }
        else if (ch === "(") {
            if (depth > 0) {
                depth--;
            }
            else {
                // Found the opening paren — get the function name before it
                const before = text.slice(0, i).trimEnd();
                const m = before.match(/([a-zA-Z_][a-zA-Z0-9_]*)$/);
                return m ? { fnName: m[1], activeParam } : undefined;
            }
        }
        else if (ch === "," && depth === 0) {
            activeParam++;
        }
        else if (ch === "\n" && depth === 0) {
            // Don't walk past a blank line at top level
            break;
        }
    }
    return undefined;
}
// ─────────────────────────────────────────────────────────────
//  1. Definition Provider
// ─────────────────────────────────────────────────────────────
const definitionProvider = {
    async provideDefinition(document, position) {
        const line = document.lineAt(position).text;
        // Import line: use "@/..." or use "./..."
        const importMatch = line.match(/^\s*use\s+"([^"]+)"/);
        if (importMatch) {
            const resolved = resolveImportPath(importMatch[1], document.uri.fsPath);
            if (resolved && fs.existsSync(resolved)) {
                return new vscode.Location(vscode.Uri.file(resolved), new vscode.Position(0, 0));
            }
            return undefined;
        }
        // Function / method call
        const wordRange = document.getWordRangeAtPosition(position, /[a-zA-Z_][a-zA-Z0-9_]*/);
        if (!wordRange)
            return undefined;
        const word = document.getText(wordRange);
        const after = line.slice(wordRange.end.character).trimStart();
        const isMaybeMethod = position.character > 0 && line[wordRange.start.character - 1] === ".";
        if (!after.startsWith("(") && !isMaybeMethod)
            return undefined;
        const fnPattern = new RegExp(`\\bfn\\s+${word}\\s*\\(`);
        const uris = await vscode.workspace.findFiles("**/*.ura", "{**/build/**,**/.git/**}");
        for (const uri of uris) {
            const text = (await vscode.workspace.fs.readFile(uri)).toString();
            const lines = text.split("\n");
            for (let i = 0; i < lines.length; i++) {
                if (fnPattern.test(lines[i])) {
                    const col = lines[i].indexOf(word);
                    return new vscode.Location(uri, new vscode.Position(i, col));
                }
            }
        }
        return undefined;
    },
};
// ─────────────────────────────────────────────────────────────
//  Import helpers
// ─────────────────────────────────────────────────────────────
/** Compute the relative import string from one .ura file to another. */
function relativeImport(fromFile, toFile) {
    const fromDir = path.dirname(fromFile);
    let rel = path.relative(fromDir, toFile).replace(/\.ura$/, "");
    // Normalize Windows backslashes and ensure ./ prefix
    rel = rel.replace(/\\/g, "/");
    if (!rel.startsWith("."))
        rel = "./" + rel;
    return rel;
}
/** True if the given import string already appears in the document. */
function isAlreadyImported(docText, importStr) {
    const escaped = importStr.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
    return new RegExp(`^\\s*use\\s+"${escaped}"`, "m").test(docText);
}
// ─────────────────────────────────────────────────────────────
//  2. Completion Provider
//     - Keywords
//     - Workspace functions + struct types  (relative-path auto-import)
//     - ura-lib functions                   (@/module auto-import)
//
//  additionalTextEdits set directly in provideCompletionItems
//  (more reliable than resolveCompletionItem for local extensions)
// ─────────────────────────────────────────────────────────────
const KEYWORDS = [
    "if", "elif", "else", "while", "for", "return", "break", "continue",
    "and", "or", "is", "not", "self", "main", "struct", "fn", "as",
    "use", "proto", "True", "False", "NULL",
    "int", "float", "double", "char", "void", "long", "short",
    "unsigned", "signed", "bool", "chars", "pointer", "ref", "array",
    "stack", "heap", "output", "sizeof", "typeof",
];
const completionProvider = {
    async provideCompletionItems(document, _position) {
        const items = [];
        const docText = document.getText();
        const insertLine = getInsertLineForImport(docText.split("\n"));
        const insertPos = new vscode.Position(insertLine, 0);
        // ── Keywords ────────────────────────────────────────────
        for (const kw of KEYWORDS) {
            items.push(new vscode.CompletionItem(kw, vscode.CompletionItemKind.Keyword));
        }
        const wsRoot = getWorkspaceRoot() ?? "";
        // ── Workspace functions and struct types ─────────────────
        const fnRe = /^\s*(?:proto\s+)?fn\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(/;
        const structRe = /^\s*struct\s+([A-Z][a-zA-Z0-9_]*)/;
        const uris = await vscode.workspace.findFiles("**/*.ura", "{**/build/**,**/.git/**}");
        for (const uri of uris) {
            const isSameFile = uri.fsPath === document.uri.fsPath;
            const relPath = path.relative(wsRoot, uri.fsPath).replace(/\\/g, "/");
            const text = (await vscode.workspace.fs.readFile(uri)).toString();
            for (const line of text.split("\n")) {
                const fnMatch = fnRe.exec(line);
                if (fnMatch) {
                    const fn = parseLine(line, uri, 0, "");
                    const item = new vscode.CompletionItem(fnMatch[1], vscode.CompletionItemKind.Function);
                    const importStr = isSameFile ? "" : relativeImport(document.uri.fsPath, uri.fsPath);
                    const needsImport = !isSameFile && !isAlreadyImported(docText, importStr);
                    item.detail = needsImport ? `${relPath}  ← auto-import` : relPath;
                    item.documentation = new vscode.MarkdownString((fn ? "```\n" + buildSignatureLabel(fn) + "\n```\n\n" : "") +
                        `📄 \`${relPath}\``);
                    if (needsImport) {
                        item.additionalTextEdits = [
                            vscode.TextEdit.insert(insertPos, `use "${importStr}"\n`),
                        ];
                    }
                    items.push(item);
                }
                const stMatch = structRe.exec(line);
                if (stMatch) {
                    const item = new vscode.CompletionItem(stMatch[1], vscode.CompletionItemKind.Class);
                    const importStr = isSameFile ? "" : relativeImport(document.uri.fsPath, uri.fsPath);
                    const needsImport = !isSameFile && !isAlreadyImported(docText, importStr);
                    item.detail = needsImport ? `${relPath}  ← auto-import` : relPath;
                    item.documentation = new vscode.MarkdownString(`📄 \`${relPath}\``);
                    if (needsImport) {
                        item.additionalTextEdits = [
                            vscode.TextEdit.insert(insertPos, `use "${importStr}"\n`),
                        ];
                    }
                    items.push(item);
                }
            }
        }
        // ── ura-lib functions — smart @/header import ───────────
        for (const fn of libIndex) {
            const { importStr, alreadyImported } = chooseImport(fn.module, docText);
            const relPath = path.relative(wsRoot, fn.uri.fsPath).replace(/\\/g, "/");
            const item = new vscode.CompletionItem(fn.name, vscode.CompletionItemKind.Function);
            item.detail = alreadyImported
                ? `@/${fn.module} · ${relPath}`
                : `@/${fn.module} · ${relPath}  ← use "${importStr}"`;
            item.documentation = new vscode.MarkdownString("```\n" + buildSignatureLabel(fn) + "\n```\n\n" +
                `📄 \`${relPath}\`\n\n` +
                (alreadyImported
                    ? `*from \`@/${fn.module}\`*`
                    : `*will add \`use "${importStr}"\`*`));
            if (!alreadyImported) {
                item.additionalTextEdits = [
                    vscode.TextEdit.insert(insertPos, `use "${importStr}"\n`),
                ];
            }
            items.push(item);
        }
        return items;
    },
};
// ─────────────────────────────────────────────────────────────
//  3. Signature Help Provider
//     Shows: fn strlen(s chars) int
//                      ───────       ← active param highlighted
// ─────────────────────────────────────────────────────────────
const signatureHelpProvider = {
    provideSignatureHelp(document, position) {
        const ctx = getCallContext(document, position);
        if (!ctx)
            return undefined;
        const fn = lookupFn(ctx.fnName);
        if (!fn)
            return undefined;
        const label = buildSignatureLabel(fn);
        // Build ParameterInformation with character offsets inside label
        const parenOpen = label.indexOf("(") + 1;
        const paramInfos = [];
        let offset = parenOpen;
        for (const p of fn.params) {
            const paramStr = p.variadic ? "..." : `${p.name} ${p.type}`;
            const idx = label.indexOf(paramStr, offset);
            if (idx !== -1) {
                paramInfos.push(new vscode.ParameterInformation([idx, idx + paramStr.length]));
                offset = idx + paramStr.length + 2; // skip ", "
            }
            else {
                paramInfos.push(new vscode.ParameterInformation(paramStr));
            }
        }
        const sigInfo = new vscode.SignatureInformation(label);
        sigInfo.parameters = paramInfos;
        if (fn.module) {
            sigInfo.documentation = new vscode.MarkdownString(`*from \`@/${fn.module}\`*`);
        }
        const help = new vscode.SignatureHelp();
        help.signatures = [sigInfo];
        help.activeSignature = 0;
        const hasVariadic = fn.params.some((p) => p.variadic);
        const maxParam = hasVariadic
            ? fn.params.length - 1
            : Math.max(0, fn.params.length - 1);
        help.activeParameter = Math.min(ctx.activeParam, maxParam);
        return help;
    },
};
// ─────────────────────────────────────────────────────────────
//  4. Hover Provider
//     Hover over any function name → shows full signature
// ─────────────────────────────────────────────────────────────
const hoverProvider = {
    provideHover(document, position) {
        const wordRange = document.getWordRangeAtPosition(position, /[a-zA-Z_][a-zA-Z0-9_]*/);
        if (!wordRange)
            return undefined;
        const word = document.getText(wordRange);
        const fn = lookupFn(word);
        if (!fn)
            return undefined;
        const md = new vscode.MarkdownString();
        md.appendCodeblock(buildSignatureLabel(fn), "ura");
        if (fn.module) {
            md.appendMarkdown(`\n*from \`@/${fn.module}\`*`);
        }
        else {
            md.appendMarkdown(`\n*${path.basename(fn.uri.fsPath)}*`);
        }
        return new vscode.Hover(md, wordRange);
    },
};
// ─────────────────────────────────────────────────────────────
//  5. Diagnostics — wrong argument count
//     Full type checking is out of scope; only counts args.
// ─────────────────────────────────────────────────────────────
// Names to never lint (builtins, control flow, etc.)
const SKIP_LINT = new Set([
    "if", "elif", "else", "while", "for", "return",
    "output", "typeof", "sizeof", "stack", "heap", "main",
]);
function countActualArgs(text, openParenOffset) {
    let depth = 1;
    let commas = 0;
    let hasContent = false;
    let i = openParenOffset + 1;
    while (i < text.length && depth > 0) {
        const ch = text[i];
        if (ch === "(" || ch === "[")
            depth++;
        else if (ch === ")" || ch === "]") {
            depth--;
            if (depth === 0)
                break;
        }
        else if (ch === "," && depth === 1) {
            commas++;
        }
        else if (!/\s/.test(ch) && depth === 1) {
            hasContent = true;
        }
        i++;
    }
    if (!hasContent && commas === 0)
        return 0;
    return commas + 1;
}
let diagCollection;
let diagTimer;
function lintDocument(document) {
    if (document.languageId !== "ura")
        return;
    const text = document.getText();
    const diags = [];
    const callRe = /\b([a-zA-Z_][a-zA-Z0-9_]*)\s*\(/g;
    let m;
    while ((m = callRe.exec(text)) !== null) {
        const fnName = m[1];
        if (SKIP_LINT.has(fnName))
            continue;
        // Skip declaration lines
        const lineStart = text.lastIndexOf("\n", m.index) + 1;
        const lineEnd = text.indexOf("\n", m.index);
        const lineText = text.slice(lineStart, lineEnd === -1 ? undefined : lineEnd);
        if (/^\s*(proto\s+)?fn\s/.test(lineText))
            continue;
        if (/^\s*struct\s/.test(lineText))
            continue;
        const fn = lookupFn(fnName);
        if (!fn)
            continue;
        if (fn.params.some((p) => p.variadic))
            continue;
        const openParen = m.index + m[0].length - 1;
        const actual = countActualArgs(text, openParen);
        const expected = fn.params.length;
        if (actual !== expected) {
            const startPos = document.positionAt(m.index);
            const endPos = document.positionAt(m.index + fnName.length);
            const range = new vscode.Range(startPos, endPos);
            const msg = `'${fnName}' expects ${expected} argument${expected !== 1 ? "s" : ""}, got ${actual}`;
            diags.push(new vscode.Diagnostic(range, msg, vscode.DiagnosticSeverity.Warning));
        }
    }
    diagCollection.set(document.uri, diags);
}
function scheduleLint(document) {
    if (diagTimer)
        clearTimeout(diagTimer);
    diagTimer = setTimeout(() => lintDocument(document), 500);
}
// ─────────────────────────────────────────────────────────────
//  6. Formatter (unchanged from previous version)
// ─────────────────────────────────────────────────────────────
function formatUra(text) {
    const lines = text.split("\n");
    const result = [];
    const stripTrailing = (s) => s.replace(/\s+$/, "");
    function spaceOperators(s) {
        if (/^\s*\/\//.test(s))
            return s;
        const strings = [];
        s = s.replace(/"(?:[^"\\]|\\.)*"/g, (m2) => {
            strings.push(m2);
            return `\x00STR${strings.length - 1}\x00`;
        });
        s = s.replace(/'(?:[^'\\]|\\.)*'/g, (m2) => {
            strings.push(m2);
            return `\x00STR${strings.length - 1}\x00`;
        });
        const ops = [
            "==", "!=", "<=", ">=", "<<", ">>",
            "+=", "-=", "*=", "/=", "%=",
            "&&", "||",
        ];
        for (const op of ops) {
            const e = op.replace(/[*+?^${}()|[\]\\]/g, "\\$&");
            s = s.replace(new RegExp(`\\s*${e}\\s*`, "g"), ` ${op} `);
        }
        s = s.replace(/([^\s=!<>+\-*/%&|^])([=])(?!=)/g, "$1 = ");
        s = s.replace(/([=])(?!=)([^\s>])/g, "= $2");
        s = s.replace(/,\s*/g, ", ");
        s = s.replace(/\x00STR(\d+)\x00/g, (_, i) => strings[parseInt(i)]);
        return s;
    }
    for (let i = 0; i < lines.length; i++) {
        let line = stripTrailing(lines[i]);
        if (line.trim() === "") {
            if (result.length > 0 && result[result.length - 1].trim() === "")
                continue;
            result.push("");
            continue;
        }
        if (/^\s*\/\//.test(line)) {
            result.push(stripTrailing(line));
            continue;
        }
        result.push(spaceOperators(line));
    }
    while (result.length > 0 && result[result.length - 1] === "")
        result.pop();
    return result.join("\n") + "\n";
}
const formattingProvider = {
    provideDocumentFormattingEdits(document) {
        const text = document.getText();
        const formatted = formatUra(text);
        if (formatted === text)
            return [];
        const fullRange = new vscode.Range(document.positionAt(0), document.positionAt(text.length));
        return [vscode.TextEdit.replace(fullRange, formatted)];
    },
};
// ─────────────────────────────────────────────────────────────
//  Activation
// ─────────────────────────────────────────────────────────────
function activate(context) {
    const URA = { language: "ura" };
    // Build ura-lib index synchronously on activation
    buildLibIndex();
    // Build workspace index asynchronously (ready within milliseconds)
    buildWorkspaceIndex();
    // Watch ura-lib for changes
    const libDir = getUraLibDir();
    if (libDir) {
        const libWatcher = vscode.workspace.createFileSystemWatcher(new vscode.RelativePattern(libDir, "*.ura"));
        libWatcher.onDidChange(buildLibIndex);
        libWatcher.onDidCreate(buildLibIndex);
        libWatcher.onDidDelete(buildLibIndex);
        context.subscriptions.push(libWatcher);
    }
    // Watch all workspace .ura files
    const wsWatcher = vscode.workspace.createFileSystemWatcher("**/*.ura");
    wsWatcher.onDidChange(buildWorkspaceIndex);
    wsWatcher.onDidCreate(buildWorkspaceIndex);
    wsWatcher.onDidDelete(buildWorkspaceIndex);
    context.subscriptions.push(wsWatcher);
    // Diagnostics collection
    diagCollection = vscode.languages.createDiagnosticCollection("ura");
    context.subscriptions.push(diagCollection);
    // Lint on open / change / editor switch
    if (vscode.window.activeTextEditor) {
        scheduleLint(vscode.window.activeTextEditor.document);
    }
    context.subscriptions.push(vscode.workspace.onDidOpenTextDocument(scheduleLint), vscode.workspace.onDidChangeTextDocument((e) => scheduleLint(e.document)), vscode.window.onDidChangeActiveTextEditor((e) => e && scheduleLint(e.document)));
    // Register all language providers
    context.subscriptions.push(vscode.languages.registerDefinitionProvider(URA, definitionProvider), vscode.languages.registerCompletionItemProvider(URA, completionProvider), vscode.languages.registerSignatureHelpProvider(URA, signatureHelpProvider, "(", ","), vscode.languages.registerHoverProvider(URA, hoverProvider), vscode.languages.registerDocumentFormattingEditProvider(URA, formattingProvider));
}
function deactivate() {
    if (diagTimer)
        clearTimeout(diagTimer);
}
//# sourceMappingURL=extension.js.map