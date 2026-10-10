import * as vscode from "vscode";
import { CompletionProvider } from "./providers/completion";
import { DefinitionProvider } from "./providers/definition";
import { Diagnostics } from "./providers/diagnostics";
import { HoverProvider } from "./providers/hover";
import { SignatureProvider } from "./providers/signature";

export function activate(context: vscode.ExtensionContext): void {
    const ura: vscode.DocumentSelector = { language: "ura" };
    const diagnostics = new Diagnostics();

    context.subscriptions.push(
        vscode.languages.registerCompletionItemProvider(ura, new CompletionProvider()),
        vscode.languages.registerHoverProvider(ura, new HoverProvider()),
        vscode.languages.registerSignatureHelpProvider(ura, new SignatureProvider(), "(", ","),
        vscode.languages.registerDefinitionProvider(ura, new DefinitionProvider()),
        diagnostics,
    );
    diagnostics.start();
}

export function deactivate(): void {}
