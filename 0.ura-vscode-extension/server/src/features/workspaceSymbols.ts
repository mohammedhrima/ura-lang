// Go to Symbol in Workspace (Ctrl+T): structs, functions, protos and methods
// of every .ura file in the workspace.

import { SymbolKind, type SymbolInformation, type WorkspaceSymbolParams } from "vscode-languageserver/node";
import { matchesQuery } from "../core/symbols";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function find(s: Services, params: WorkspaceSymbolParams): SymbolInformation[] {
    const result: SymbolInformation[] = [];
    for (const context of s.workspace.allContexts()) {
        const uri = s.workspace.uriOfContext(context.file);
        const add = (name: string, kind: SymbolKind, line: number, character: number, container?: string): void => {
            const label = container ? `${container}.${name}` : name;
            if (!matchesQuery(label, params.query)) {
                return;
            }
            const at = { line, character };
            result.push({
                name,
                kind,
                location: { uri, range: { start: at, end: { line, character: character + name.length } } },
                ...(container ? { containerName: container } : {}),
            });
        };
        for (const struct of context.analysis.structs) {
            add(struct.name, SymbolKind.Struct, struct.line, struct.character);
            for (const method of struct.methods) {
                add(method.name, SymbolKind.Method, method.line, method.character, struct.name);
            }
        }
        for (const fn of context.analysis.functions.filter((f) => f.owner === undefined)) {
            add(fn.name, SymbolKind.Function, fn.line, fn.character);
        }
    }
    return result;
}

export const feature: Feature = {
    capabilities: { workspaceSymbolProvider: true },
    register(s: Services): void {
        s.connection.onWorkspaceSymbol(guard(s.logger, "workspace symbols", [], (params: WorkspaceSymbolParams) => find(s, params)));
    },
};
