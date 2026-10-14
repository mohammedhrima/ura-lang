// Struct names wherever they are used (`u User`, `p &User`, `User[]`), for
// structs declared in this file or in a file it `use`s. The grammar can't do
// it: whether a name is a struct depends on the declarations.

import { SemanticTokensBuilder, type SemanticTokens, type SemanticTokensParams } from "vscode-languageserver/node";
import { analyze, spansOf } from "../core/parser";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

const TOKEN_TYPES = ["struct"];
const STRUCT = 0; // index in TOKEN_TYPES

function tokens(s: Services, params: SemanticTokensParams): SemanticTokens {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return { data: [] };
    }
    const analysis = analyze(document.getText());
    const imported = s.workspace.imports(document).structs.map((i) => i.struct.name);
    const names = new Set([...analysis.structs.map((st) => st.name), ...imported]);
    const builder = new SemanticTokensBuilder();
    for (const span of spansOf(analysis.lines, names)) {
        builder.push(span.line, span.start, span.length, STRUCT, 0);
    }
    return builder.build();
}

export const feature: Feature = {
    capabilities: { semanticTokensProvider: { legend: { tokenTypes: TOKEN_TYPES, tokenModifiers: [] }, full: true } },
    register(s: Services): void {
        s.connection.languages.semanticTokens.on(
            guard(s.logger, "semantic tokens", { data: [] }, (params: SemanticTokensParams) => tokens(s, params)),
        );
    },
};
