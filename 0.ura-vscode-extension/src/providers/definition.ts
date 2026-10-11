import * as vscode from "vscode";
import { analyze, resolve } from "../analysis/parser";
import { importsOf } from "./documents";

export class DefinitionProvider implements vscode.DefinitionProvider {
    provideDefinition(document: vscode.TextDocument, position: vscode.Position): vscode.Location | undefined {
        const analysis = analyze(document.getText());
        const at = { line: position.line, character: position.character };
        const found = resolve(analysis, at, importsOf(document));
        if (!found) {
            return undefined;
        }
        const target = found.kind === "function" ? found.function : found.variable;
        const uri = found.kind === "function" && found.file ? vscode.Uri.file(found.file) : document.uri;
        return new vscode.Location(uri, new vscode.Position(target.line, target.character));
    }
}
