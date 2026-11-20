// Problems from the real compiler: on open, on save (every open file, since
// any of them may `use` the saved one) and when the settings change.

import { DiagnosticSeverity, type Diagnostic, type Range } from "vscode-languageserver/node";
import { TextDocument } from "vscode-languageserver-textdocument";
import type { Problem } from "../core/compiler";
import { analyze, wordAt } from "../core/parser";
import type { Feature, Services } from "../services";

function rangeOf(document: TextDocument, problem: Problem): Range {
    const lines = analyze(document.getText()).lines;
    if (problem.line === undefined || problem.line >= lines.length) {
        // no position from the compiler yet: mark the first line
        return { start: { line: 0, character: 0 }, end: { line: 0, character: (lines[0] ?? "").length } };
    }
    const text = lines[problem.line] ?? "";
    const start = Math.min(problem.character ?? 0, text.length);
    if (problem.length !== undefined) {
        // exactly what the compiler underlined
        const end = Math.min(start + problem.length, text.length);
        return { start: { line: problem.line, character: start }, end: { line: problem.line, character: end } };
    }
    const word = wordAt(text, start);
    const [from, to] = word ? [word.start, word.end] : [start, text.length];
    return { start: { line: problem.line, character: from }, end: { line: problem.line, character: to } };
}

function toDiagnostic(document: TextDocument, problem: Problem): Diagnostic {
    return {
        range: rangeOf(document, problem),
        message: problem.help ? `${problem.message}\n\nhelp: ${problem.help}` : problem.message,
        severity: DiagnosticSeverity.Error,
        source: "ura",
        ...(problem.code ? { code: problem.code } : {}),
    };
}

// The text of `uri` for placing problems: the open document, else the file on disk.
function textOf(s: Services, uri: string, file: string): TextDocument | undefined {
    const open = s.documents.get(uri);
    if (open) {
        return open;
    }
    const text = s.workspace.read(file);
    return text === undefined ? undefined : TextDocument.create(uri, "ura", 0, text);
}

// Files that got problems from the last check started by each document, so a
// fixed file is cleared even when it's not the one being edited.
const reported = new Map<string, Set<string>>();

async function check(s: Services, document: TextDocument): Promise<void> {
    if (!s.config.get().lint.enable) {
        await s.connection.sendDiagnostics({ uri: document.uri, diagnostics: [] });
        return;
    }
    const version = document.version;
    const file = s.workspace.fileOf(document.uri);
    const problems = await s.compiler.check(document.getText(), file);
    const current = s.documents.get(document.uri);
    if (!current || current.version !== version) {
        return; // the result is for text that no longer exists
    }
    // each problem goes to the file it names; one without a file to the checked document
    const byUri = new Map<string, Problem[]>([[document.uri, []]]);
    for (const problem of problems) {
        const uri = problem.file && problem.file !== file ? s.workspace.uriOf(problem.file) : document.uri;
        byUri.set(uri, [...(byUri.get(uri) ?? []), problem]);
    }
    for (const uri of reported.get(document.uri) ?? []) {
        byUri.set(uri, byUri.get(uri) ?? []);
    }
    reported.set(document.uri, new Set([...byUri].filter(([, found]) => found.length > 0).map(([uri]) => uri)));
    for (const [uri, found] of byUri) {
        const text = uri === document.uri ? current : textOf(s, uri, s.workspace.fileOf(uri) ?? "");
        const diagnostics = text ? found.map((p) => toDiagnostic(text, p)) : [];
        await s.connection.sendDiagnostics({ uri, diagnostics });
    }
}

function checkLogged(s: Services, document: TextDocument): void {
    check(s, document).catch((error: unknown) => s.logger.error(`checking ${document.uri} failed`, error));
}

function checkAll(s: Services): void {
    for (const document of s.documents.all()) {
        checkLogged(s, document);
    }
}

export const feature: Feature = {
    capabilities: {},
    register(s: Services): void {
        s.documents.onDidOpen((event) => checkLogged(s, event.document));
        s.documents.onDidSave(() => checkAll(s));
        s.documents.onDidClose((event) => {
            s.connection.sendDiagnostics({ uri: event.document.uri, diagnostics: [] }).catch((error: unknown) => {
                s.logger.error("clearing diagnostics failed", error);
            });
        });
        s.config.onChange(() => checkAll(s));
    },
};
