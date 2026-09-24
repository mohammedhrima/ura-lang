import * as vscode from "vscode";
import { lint, Problem, resolveCompiler } from "../analysis/compiler";

function isUra(document: vscode.TextDocument): boolean {
    return document.languageId === "ura" && document.uri.scheme === "file";
}

function toDiagnostic(document: vscode.TextDocument, problem: Problem): vscode.Diagnostic {
    let range: vscode.Range;
    if (problem.line !== undefined && problem.line < document.lineCount) {
        const text = document.lineAt(problem.line).text;
        const start = Math.min(problem.character ?? 0, text.length);
        const word = document.getWordRangeAtPosition(new vscode.Position(problem.line, start));
        range = word ?? new vscode.Range(problem.line, start, problem.line, text.length);
    } else {
        // no position from the compiler yet: mark the first line
        range = document.lineCount > 0 ? document.lineAt(0).range : new vscode.Range(0, 0, 0, 0);
    }
    const diagnostic = new vscode.Diagnostic(range, problem.message, vscode.DiagnosticSeverity.Error);
    diagnostic.source = "ura";
    if (problem.code) {
        diagnostic.code = problem.code;
    }
    return diagnostic;
}

// Runs the compiler on open and on save and shows its errors as problems.
export class Diagnostics implements vscode.Disposable {
    private readonly collection = vscode.languages.createDiagnosticCollection("ura");
    private readonly subscriptions: vscode.Disposable[] = [];

    start(): void {
        this.subscriptions.push(
            vscode.workspace.onDidOpenTextDocument((d) => this.lint(d)),
            vscode.workspace.onDidSaveTextDocument((d) => this.lint(d)),
            vscode.workspace.onDidCloseTextDocument((d) => this.collection.delete(d.uri)),
            vscode.workspace.onDidChangeConfiguration((e) => {
                if (e.affectsConfiguration("ura")) {
                    this.lintAll();
                }
            }),
        );
        this.lintAll();
    }

    private lintAll(): void {
        for (const document of vscode.workspace.textDocuments) {
            this.lint(document);
        }
    }

    private async lint(document: vscode.TextDocument): Promise<void> {
        if (!isUra(document)) {
            return;
        }
        const config = vscode.workspace.getConfiguration("ura", document.uri);
        if (!config.get<boolean>("lint.enable", true)) {
            this.collection.delete(document.uri);
            return;
        }
        const folder =
            vscode.workspace.getWorkspaceFolder(document.uri) ?? vscode.workspace.workspaceFolders?.[0];
        const compiler = resolveCompiler(config.get<string>("compilerPath", ""), folder?.uri.fsPath);

        const version = document.version;
        const problems = await lint(compiler, document.getText());
        if (document.isClosed || document.version !== version) {
            return; // the result is for text that no longer exists
        }
        this.collection.set(
            document.uri,
            problems.map((p) => toDiagnostic(document, p)),
        );
    }

    dispose(): void {
        for (const subscription of this.subscriptions) {
            subscription.dispose();
        }
        this.collection.dispose();
    }
}
