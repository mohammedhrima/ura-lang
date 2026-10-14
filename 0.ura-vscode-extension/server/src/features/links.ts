// `use "path"`: the path opens its file on ctrl+click.

import type { DocumentLink, DocumentLinkParams } from "vscode-languageserver/node";
import { resolveUse } from "../core/imports";
import { analyze } from "../core/parser";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function links(s: Services, params: DocumentLinkParams): DocumentLink[] {
    const document = s.documents.get(params.textDocument.uri);
    const file = document && s.workspace.fileOf(document.uri);
    if (!document || file === undefined) {
        return [];
    }
    const result: DocumentLink[] = [];
    for (const use of analyze(document.getText()).uses) {
        const target = resolveUse(file, use.target);
        if (!s.workspace.exists(target)) {
            continue; // the compiler reports the missing file
        }
        result.push({
            range: { start: { line: use.line, character: use.start }, end: { line: use.line, character: use.end } },
            target: s.workspace.uriOf(target),
        });
    }
    return result;
}

export const feature: Feature = {
    capabilities: { documentLinkProvider: {} },
    register(s: Services): void {
        s.connection.onDocumentLinks(guard(s.logger, "document links", [], (params: DocumentLinkParams) => links(s, params)));
    },
};
