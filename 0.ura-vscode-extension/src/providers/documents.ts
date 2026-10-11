import * as vscode from "vscode";
import { importedFunctions, readFromDisk, Reader } from "../analysis/imports";
import { ImportedFunction } from "../analysis/parser";

// An open editor may hold unsaved text: read it from there first, then from disk.
export const readSource: Reader = (file) => {
    const open = vscode.workspace.textDocuments.find((d) => d.uri.scheme === "file" && d.uri.fsPath === file);
    return open ? open.getText() : readFromDisk(file);
};

// The functions `document` brings in through `use`. None for an unsaved
// document: its `use` paths have no directory to be relative to.
export function importsOf(document: vscode.TextDocument): ImportedFunction[] {
    if (document.uri.scheme !== "file") {
        return [];
    }
    return importedFunctions(document.uri.fsPath, document.getText(), readSource);
}
