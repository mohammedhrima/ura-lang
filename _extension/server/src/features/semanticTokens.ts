// Struct and enum names wherever they are used (`u User`, `p &User`,
// `Array<User>`, `Color::RED`), for the ones declared in this file or in a
// file it `use`s. The grammar can't do it: whether a name is a type depends
// on the declarations.

import { SemanticTokensBuilder, type SemanticTokens, type SemanticTokensParams } from "vscode-languageserver/node";
import { spansOf } from "../core/parser";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

const TOKEN_TYPES = ["struct", "enum"];
const STRUCT = 0; // index in TOKEN_TYPES
const ENUM = 1;

function tokens(s: Services, params: SemanticTokensParams): SemanticTokens {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return { data: [] };
    }
    const { analysis, imports } = s.workspace.contextOf(document);
    const structs = new Set([...analysis.structs.map((st) => st.name), ...imports.structs.map((i) => i.struct.name)]);
    const enums = new Set([...analysis.enums.map((en) => en.name), ...imports.enums.map((i) => i.enumeration.name)]);
    const spans = [
        ...spansOf(analysis.lines, structs).map((span) => ({ span, type: STRUCT })),
        ...spansOf(analysis.lines, enums).map((span) => ({ span, type: ENUM })),
    ].sort((a, b) => a.span.line - b.span.line || a.span.start - b.span.start);
    const builder = new SemanticTokensBuilder();
    for (const { span, type } of spans) {
        builder.push(span.line, span.start, span.length, type, 0);
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
