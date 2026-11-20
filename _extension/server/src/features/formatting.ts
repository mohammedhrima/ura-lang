// Format Document and Format Selection.

import {
    TextEdit,
    type DocumentFormattingParams,
    type DocumentRangeFormattingParams,
} from "vscode-languageserver/node";
import { formatDocument, formatLines } from "../core/format";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function formatAll(s: Services, params: DocumentFormattingParams): TextEdit[] {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return [];
    }
    const text = document.getText();
    const formatted = formatDocument(text);
    if (formatted === text) {
        return [];
    }
    return [TextEdit.replace({ start: { line: 0, character: 0 }, end: document.positionAt(text.length) }, formatted)];
}

// Only the lines the selection touches, one edit per changed line; blank lines
// between top-level blocks are a whole-document matter and stay as they are.
function formatSelection(s: Services, params: DocumentRangeFormattingParams): TextEdit[] {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return [];
    }
    const { start, end } = params.range;
    const last = end.character === 0 && end.line > start.line ? end.line - 1 : end.line;
    const text = document.getText();
    const lines = text.split(/\r?\n/);
    const formatted = formatLines(text, start.line, last);
    const edits: TextEdit[] = [];
    formatted.forEach((line, i) => {
        const number = start.line + i;
        const original = lines[number] ?? "";
        if (line !== original) {
            edits.push(TextEdit.replace({ start: { line: number, character: 0 }, end: { line: number, character: original.length } }, line));
        }
    });
    return edits;
}

export const feature: Feature = {
    capabilities: { documentFormattingProvider: true, documentRangeFormattingProvider: true },
    register(s: Services): void {
        s.connection.onDocumentFormatting(
            guard(s.logger, "formatting", [], (params: DocumentFormattingParams) => formatAll(s, params)),
        );
        s.connection.onDocumentRangeFormatting(
            guard(s.logger, "range formatting", [], (params: DocumentRangeFormattingParams) => formatSelection(s, params)),
        );
    },
};
