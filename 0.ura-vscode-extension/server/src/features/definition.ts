// Go to definition: variables, functions, methods, structs, attributes (also
// in used files).

import type { DefinitionParams, Location } from "vscode-languageserver/node";
import { resolve } from "../core/parser";
import { declarationOf } from "../core/symbols";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function define(s: Services, params: DefinitionParams): Location | null {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return null;
    }
    const context = s.workspace.contextOf(document);
    const found = resolve(context.analysis, params.position, context.imports);
    if (!found) {
        return null;
    }
    const declaration = declarationOf(found);
    const uri = declaration.file ? s.workspace.uriOf(declaration.file) : document.uri;
    const start = { line: declaration.line, character: declaration.character };
    const end = { line: declaration.line, character: declaration.character + declaration.name.length };
    return { uri, range: { start, end } };
}

export const feature: Feature = {
    capabilities: { definitionProvider: true },
    register(s: Services): void {
        s.connection.onDefinition(guard(s.logger, "definition", null, (params: DefinitionParams) => define(s, params)));
    },
};
