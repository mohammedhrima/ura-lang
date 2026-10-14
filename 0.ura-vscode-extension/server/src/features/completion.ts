// Completion. After `x.`: the attributes and methods of x's struct. Elsewhere:
// keywords, types, struct names, booleans, built-ins, functions (this file and
// the files it `use`s) and the variables visible at the cursor.

import {
    CompletionItemKind,
    InsertTextFormat,
    MarkupKind,
    type CompletionItem,
    type CompletionParams,
} from "vscode-languageserver/node";
import { BOOLEANS, BUILTINS, builtinSignature, CONTROL, DECLARATION, IMPORT, LOGICAL, TYPES } from "../core/keywords";
import {
    functionSignature,
    isInComment,
    memberContext,
    visibleVariables,
    type FunctionInfo,
    type StructInfo,
} from "../core/parser";
import type { FileContext } from "../core/references";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function keyword(label: string, kind: CompletionItemKind): CompletionItem {
    return { label, kind };
}

function call(fn: FunctionInfo, kind: CompletionItemKind): CompletionItem {
    const args = fn.params.map((p, i) => `\${${i + 1}:${p.name}}`).join(", ");
    return {
        label: fn.name,
        kind,
        insertText: `${fn.name}(${args})`,
        insertTextFormat: InsertTextFormat.Snippet,
        detail: functionSignature(fn),
    };
}

function members(struct: StructInfo): CompletionItem[] {
    return [
        ...struct.attributes.map((a) => ({ label: a.name, kind: CompletionItemKind.Field, detail: a.type })),
        ...struct.methods.map((m) => call(m, CompletionItemKind.Method)),
    ];
}

function everything(context: FileContext, params: CompletionParams): CompletionItem[] {
    const items: CompletionItem[] = [];
    for (const word of [...CONTROL, ...LOGICAL, ...DECLARATION, ...IMPORT]) {
        items.push(keyword(word, CompletionItemKind.Keyword));
    }
    for (const type of TYPES) {
        items.push(keyword(type, CompletionItemKind.TypeParameter));
    }
    const structs = [...context.analysis.structs, ...context.imports.structs.map((i) => i.struct)];
    for (const struct of structs) {
        items.push({ label: struct.name, kind: CompletionItemKind.Struct, detail: "struct" });
    }
    for (const value of BOOLEANS) {
        items.push(keyword(value, CompletionItemKind.Constant));
    }
    for (const builtin of BUILTINS) {
        items.push({
            label: builtin.name,
            kind: CompletionItemKind.Function,
            insertText: `${builtin.name}(\${1:${builtin.params[0] ?? ""}})`,
            insertTextFormat: InsertTextFormat.Snippet,
            detail: builtinSignature(builtin),
            documentation: { kind: MarkupKind.Markdown, value: builtin.doc },
        });
    }
    const functions = [...context.analysis.functions, ...context.imports.functions.map((i) => i.fn)];
    for (const fn of functions.filter((f) => f.owner === undefined)) {
        items.push(call(fn, CompletionItemKind.Function));
    }
    for (const variable of visibleVariables(context.analysis, params.position)) {
        items.push({ label: variable.name, kind: CompletionItemKind.Variable, detail: variable.type });
    }
    return items;
}

function complete(s: Services, params: CompletionParams): CompletionItem[] {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return [];
    }
    const context = s.workspace.contextOf(document);
    const line = context.analysis.lines[params.position.line] ?? "";
    if (isInComment(line, params.position.character)) {
        return [];
    }
    const receiver = memberContext(context.analysis, context.imports, params.position);
    if (receiver) {
        return members(receiver.struct);
    }
    // after a `.` whose left side has no known struct: nothing sensible to offer
    if (/\.\s*[A-Za-z_]?\w*$/.test(line.slice(0, params.position.character))) {
        return [];
    }
    return everything(context, params);
}

export const feature: Feature = {
    capabilities: { completionProvider: { triggerCharacters: ["."] } },
    register(s: Services): void {
        s.connection.onCompletion(guard(s.logger, "completion", [], (params: CompletionParams) => complete(s, params)));
    },
};
