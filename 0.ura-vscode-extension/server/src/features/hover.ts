// Hover: built-ins, functions, methods, structs, attributes, variables.

import { MarkupKind, type Hover, type HoverParams } from "vscode-languageserver/node";
import { builtinSignature, findBuiltin } from "../core/keywords";
import { isInComment, resolve, wordAt } from "../core/parser";
import { hoverOf } from "../core/symbols";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function markdown(code: string, note?: string): string {
    return "```ura\n" + code + "\n```" + (note ? `\n${note}` : "");
}

function hover(s: Services, params: HoverParams): Hover | null {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return null;
    }
    const context = s.workspace.contextOf(document);
    const { line, character } = params.position;
    const text = context.analysis.lines[line] ?? "";
    if (isInComment(text, character)) {
        return null;
    }
    const word = wordAt(text, character);
    if (!word) {
        return null;
    }
    const range = { start: { line, character: word.start }, end: { line, character: word.end } };

    const builtin = findBuiltin(word.text);
    if (builtin) {
        return { contents: { kind: MarkupKind.Markdown, value: markdown(builtinSignature(builtin), builtin.doc) }, range };
    }
    const found = resolve(context.analysis, params.position, context.imports);
    if (!found) {
        return null;
    }
    const { code, note } = hoverOf(found);
    return { contents: { kind: MarkupKind.Markdown, value: markdown(code, note) }, range };
}

export const feature: Feature = {
    capabilities: { hoverProvider: true },
    register(s: Services): void {
        s.connection.onHover(guard(s.logger, "hover", null, (params: HoverParams) => hover(s, params)));
    },
};
