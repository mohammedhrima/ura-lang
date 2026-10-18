// Problems from the real compiler: on open, on save (every open file, since
// any of them may `use` the saved one) and when the settings change.

import { DiagnosticSeverity, type Diagnostic, type Range } from "vscode-languageserver/node";
import type { TextDocument } from "vscode-languageserver-textdocument";
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

async function check(s: Services, document: TextDocument): Promise<void> {
    if (!s.config.get().lint.enable) {
        await s.connection.sendDiagnostics({ uri: document.uri, diagnostics: [] });
        return;
    }
    const version = document.version;
    const problems = await s.compiler.check(document.getText(), s.workspace.fileOf(document.uri));
    const current = s.documents.get(document.uri);
    if (!current || current.version !== version) {
        return; // the result is for text that no longer exists
    }
    await s.connection.sendDiagnostics({ uri: document.uri, diagnostics: problems.map((p) => toDiagnostic(current, p)) });
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
