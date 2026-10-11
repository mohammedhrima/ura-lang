import * as fs from "fs";
import * as vscode from "vscode";
import { resolveUse } from "../analysis/imports";
import { analyze } from "../analysis/parser";

// Makes the path in `use "path"` open its file on ctrl+click.
export class LinkProvider implements vscode.DocumentLinkProvider {
    provideDocumentLinks(document: vscode.TextDocument): vscode.DocumentLink[] {
        if (document.uri.scheme !== "file") {
            return [];
        }
        const links: vscode.DocumentLink[] = [];
        for (const use of analyze(document.getText()).uses) {
            const target = resolveUse(document.uri.fsPath, use.target);
            if (!fs.existsSync(target)) {
                continue; // the compiler reports the missing file
            }
            const range = new vscode.Range(use.line, use.start, use.line, use.end);
            links.push(new vscode.DocumentLink(range, vscode.Uri.file(target)));
        }
        return links;
    }
}
