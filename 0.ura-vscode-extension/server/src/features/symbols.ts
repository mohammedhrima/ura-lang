// The outline and breadcrumbs: structs (attributes, methods), functions and
// protos (parameters, locals), top-level variables.

import { SymbolKind, type DocumentSymbol, type DocumentSymbolParams } from "vscode-languageserver/node";
import { analyze } from "../core/parser";
import { outlineOf, type OutlineSymbol, type SymbolKind as Kind } from "../core/symbols";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

const KINDS: Record<Kind, SymbolKind> = {
    struct: SymbolKind.Struct,
    field: SymbolKind.Field,
    method: SymbolKind.Method,
    function: SymbolKind.Function,
    variable: SymbolKind.Variable,
};

function toLsp(symbol: OutlineSymbol): DocumentSymbol {
    return {
        name: symbol.name,
        detail: symbol.detail,
        kind: KINDS[symbol.kind],
        range: symbol.range,
        selectionRange: symbol.selection,
        children: symbol.children.map(toLsp),
    };
}

function symbols(s: Services, params: DocumentSymbolParams): DocumentSymbol[] {
    const document = s.documents.get(params.textDocument.uri);
    return document ? outlineOf(analyze(document.getText())).map(toLsp) : [];
}

export const feature: Feature = {
    capabilities: { documentSymbolProvider: true },
    register(s: Services): void {
        s.connection.onDocumentSymbol(guard(s.logger, "document symbols", [], (params: DocumentSymbolParams) => symbols(s, params)));
    },
};
