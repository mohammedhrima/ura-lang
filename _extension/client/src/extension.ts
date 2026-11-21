// Extension entry point: wires the client-side services, starts the language
// server and registers the commands. All language logic lives in the server.

import type * as vscode from "vscode";
import * as compileFile from "./commands/compileFile";
import type { ClientServices } from "./services";
import { createLanguageClient } from "./services/languageClient";
import { createLogger } from "./services/logger";

export async function activate(context: vscode.ExtensionContext): Promise<void> {
    const logger = createLogger();
    context.subscriptions.push(logger);

    const client = createLanguageClient(context, logger);
    context.subscriptions.push(client); // stops the server when the extension deactivates
    const services: ClientServices = { context, client, logger };
    compileFile.register(services);

    try {
        await client.start();
    } catch (error) {
        logger.error("the ura language server failed to start", error);
        throw error;
    }
}

export function deactivate(): void {
    // everything is in context.subscriptions: VS Code disposes it
}
