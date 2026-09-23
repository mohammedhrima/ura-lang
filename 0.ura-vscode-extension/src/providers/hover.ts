import * as vscode from "vscode";
import { builtinSignature, findBuiltin } from "../analysis/keywords";
import { analyze, functionSignature, isInComment, resolve, wordAt } from "../analysis/parser";

function code(text: string): vscode.MarkdownString {
    return new vscode.MarkdownString().appendCodeblock(text, "ura");
}

export class HoverProvider implements vscode.HoverProvider {
    provideHover(document: vscode.TextDocument, position: vscode.Position): vscode.Hover | undefined {
        const line = document.lineAt(position.line).text;
        if (isInComment(line, position.character)) {
            return undefined;
        }
        const word = wordAt(line, position.character);
        if (!word) {
            return undefined;
        }
        const range = new vscode.Range(position.line, word.start, position.line, word.end);

        const builtin = findBuiltin(word.text);
        if (builtin) {
            const text = code(builtinSignature(builtin)).appendMarkdown(`\n${builtin.doc}`);
            return new vscode.Hover(text, range);
        }

        const analysis = analyze(document.getText());
        const found = resolve(analysis, { line: position.line, character: position.character });
        if (!found) {
            return undefined;
        }
        if (found.kind === "function") {
            return new vscode.Hover(code(functionSignature(found.function)), range);
        }
        const variable = found.variable;
        const label = variable.isParam ? "parameter" : "variable";
        return new vscode.Hover(code(`${variable.name} ${variable.type}`).appendMarkdown(`\n${label}`), range);
    }
}
