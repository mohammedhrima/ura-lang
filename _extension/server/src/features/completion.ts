// Completion. After `x.`: the attributes and methods of x's struct (its own,
// inherited, pasted by `use`). After `Name::`: an enum's variants or a
// struct's `pub fn`s. Elsewhere: keywords, types, structs, enums, constants,
// built-ins, functions (this file and the files it `use`s) and the variables
// visible at the cursor.

import {
    CompletionItemKind,
    InsertTextFormat,
    MarkupKind,
    type CompletionItem,
    type CompletionParams,
} from "vscode-languageserver/node";
import { BUILTINS, builtinSignature, CONSTANTS, CONTROL, DECLARATION, IMPORT, LOGICAL, TYPES } from "../core/keywords";
import {
    findEnum,
    findStruct,
    functionSignature,
    isInComment,
    memberContext,
    membersOf,
    visibleVariables,
    type FunctionInfo,
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

// After `Name::`: the variants of enum Name, or the `pub fn`s of struct Name.
function scoped(context: FileContext, scope: string): CompletionItem[] {
    const { analysis, imports } = context;
    const enumeration = findEnum(analysis, imports, scope);
    if (enumeration) {
        return enumeration.enumeration.variants.map((v) => ({ label: v.name, kind: CompletionItemKind.EnumMember, detail: scope }));
    }
    const struct = findStruct(analysis, imports, scope);
    if (!struct) {
        return [];
    }
    const statics = membersOf(analysis, imports, struct).methods.filter((m) => m.item.isPub);
    return statics.map((m) => call(m.item, CompletionItemKind.Method));
}

function everything(context: FileContext, params: CompletionParams): CompletionItem[] {
    const items: CompletionItem[] = [];
    for (const word of [...CONTROL, ...LOGICAL, ...DECLARATION, ...IMPORT]) {
        items.push(keyword(word, CompletionItemKind.Keyword));
    }
    for (const type of TYPES) {
        items.push(keyword(type, CompletionItemKind.TypeParameter));
    }
    for (const value of CONSTANTS) {
        items.push(keyword(value, CompletionItemKind.Constant));
    }
    const structs = [...context.analysis.structs, ...context.imports.structs.map((i) => i.struct)];
    for (const struct of structs) {
        items.push({ label: struct.name, kind: CompletionItemKind.Struct, detail: "struct" });
    }
    const enums = [...context.analysis.enums, ...context.imports.enums.map((i) => i.enumeration)];
    for (const enumeration of enums) {
        items.push({ label: enumeration.name, kind: CompletionItemKind.Enum, detail: "enum" });
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
    const before = line.slice(0, params.position.character);
    const scope = /([A-Za-z_]\w*)\s*::\s*\w*$/.exec(before);
    if (scope) {
        return scoped(context, scope[1] ?? "");
    }
    const receiver = memberContext(context.analysis, context.imports, params.position);
    if (receiver) {
        const { attributes, methods } = membersOf(context.analysis, context.imports, receiver);
        return [
            ...attributes.map((a) => ({ label: a.item.name, kind: CompletionItemKind.Field, detail: a.item.type })),
            ...methods.map((m) => call(m.item, CompletionItemKind.Method)),
        ];
    }
    // after a `.` whose left side has no known struct: nothing sensible to offer
    if (/\.\s*[A-Za-z_]?\w*$/.test(before)) {
        return [];
    }
    return everything(context, params);
}

export const feature: Feature = {
    capabilities: { completionProvider: { triggerCharacters: [".", ":"] } },
    register(s: Services): void {
        s.connection.onCompletion(guard(s.logger, "completion", [], (params: CompletionParams) => complete(s, params)));
    },
};
