// Highlights the symbol under the cursor everywhere in the document; writes
// (declaration, `=`, `+=`, ...) differ from reads.

import { DocumentHighlightKind, type DocumentHighlight, type DocumentHighlightParams } from "vscode-languageserver/node";
import { resolve } from "../core/parser";
import { isWrite, occurrences } from "../core/references";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function highlight(s: Services, params: DocumentHighlightParams): DocumentHighlight[] {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return [];
    }
    const context = s.workspace.contextOf(document);
    const target = resolve(context.analysis, params.position, context.imports);
    if (!target) {
        return [];
    }
    return occurrences(context, target, context.file).map((span) => ({
        range: { start: { line: span.line, character: span.start }, end: { line: span.line, character: span.start + span.length } },
        kind: isWrite(context, span, target, context.file) ? DocumentHighlightKind.Write : DocumentHighlightKind.Read,
    }));
}

export const feature: Feature = {
    capabilities: { documentHighlightProvider: true },
    register(s: Services): void {
        s.connection.onDocumentHighlight(guard(s.logger, "highlight", [], (params: DocumentHighlightParams) => highlight(s, params)));
    },
};
