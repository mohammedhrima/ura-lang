import * as vscode from "vscode";
import { analyze, resolve } from "../analysis/parser";

export class DefinitionProvider implements vscode.DefinitionProvider {
    provideDefinition(document: vscode.TextDocument, position: vscode.Position): vscode.Location | undefined {
        const analysis = analyze(document.getText());
        const found = resolve(analysis, { line: position.line, character: position.character });
        if (!found) {
            return undefined;
        }
        const target = found.kind === "function" ? found.function : found.variable;
        return new vscode.Location(document.uri, new vscode.Position(target.line, target.character));
    }
}
