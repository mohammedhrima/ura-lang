// Find All References (Shift+F12): a variable's uses in its function; any other
// symbol's uses in every .ura file of the workspace.

import type { Location, ReferenceParams } from "vscode-languageserver/node";
import { resolve } from "../core/parser";
import { isLocal, occurrences } from "../core/references";
import { declarationOf } from "../core/symbols";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function references(s: Services, params: ReferenceParams): Location[] {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return [];
    }
    const here = s.workspace.contextOf(document);
    const target = resolve(here.analysis, params.position, here.imports);
    if (!target) {
        return [];
    }
    const declaration = declarationOf(target);
    const contexts = isLocal(target) ? [here] : s.workspace.allContexts();
    const locations: Location[] = [];
    for (const context of contexts) {
        const uri = s.workspace.uriOfContext(context.file);
        for (const span of occurrences(context, target, here.file)) {
            const isDeclaration =
                (declaration.file ?? here.file) === context.file &&
                span.line === declaration.line &&
                span.start === declaration.character;
            if (isDeclaration && !params.context.includeDeclaration) {
                continue;
            }
            const start = { line: span.line, character: span.start };
            locations.push({ uri, range: { start, end: { line: span.line, character: span.start + span.length } } });
        }
    }
    return locations;
}

export const feature: Feature = {
    capabilities: { referencesProvider: true },
    register(s: Services): void {
        s.connection.onReferences(guard(s.logger, "references", [], (params: ReferenceParams) => references(s, params)));
    },
};
