import * as vscode from "vscode";
import {
    BOOLEANS,
    BUILTINS,
    builtinSignature,
    CONTROL,
    DECLARATION,
    LOGICAL,
    TYPES,
} from "../analysis/keywords";
import { analyze, functionSignature, isInComment, visibleVariables } from "../analysis/parser";

function keyword(label: string, kind: vscode.CompletionItemKind): vscode.CompletionItem {
    return new vscode.CompletionItem(label, kind);
}

export class CompletionProvider implements vscode.CompletionItemProvider {
    provideCompletionItems(
        document: vscode.TextDocument,
        position: vscode.Position,
    ): vscode.CompletionItem[] {
        const line = document.lineAt(position.line).text;
        if (isInComment(line, position.character)) {
            return [];
        }

        const items: vscode.CompletionItem[] = [];
        for (const word of [...CONTROL, ...LOGICAL, ...DECLARATION]) {
            items.push(keyword(word, vscode.CompletionItemKind.Keyword));
        }
        for (const type of TYPES) {
            items.push(keyword(type, vscode.CompletionItemKind.TypeParameter));
        }
        for (const value of BOOLEANS) {
            items.push(keyword(value, vscode.CompletionItemKind.Constant));
        }

        for (const builtin of BUILTINS) {
            const item = keyword(builtin.name, vscode.CompletionItemKind.Function);
            item.insertText = new vscode.SnippetString(`${builtin.name}(\${1:${builtin.params[0]}})`);
            item.detail = builtinSignature(builtin);
            item.documentation = new vscode.MarkdownString(builtin.doc);
            items.push(item);
        }

        const analysis = analyze(document.getText());
        for (const fn of analysis.functions) {
            const item = new vscode.CompletionItem(fn.name, vscode.CompletionItemKind.Function);
            const args = fn.params.map((p, i) => `\${${i + 1}:${p.name}}`).join(", ");
            item.insertText = new vscode.SnippetString(`${fn.name}(${args})`);
            item.detail = functionSignature(fn);
            items.push(item);
        }

        const at = { line: position.line, character: position.character };
        for (const variable of visibleVariables(analysis, at)) {
            const item = new vscode.CompletionItem(variable.name, vscode.CompletionItemKind.Variable);
            item.detail = variable.type;
            items.push(item);
        }
        return items;
    }
}
