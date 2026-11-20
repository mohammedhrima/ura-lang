// Starts the ura language server (out/server/src/server.js) in its own process.

import * as path from "path";
import type * as vscode from "vscode";
import { LanguageClient, TransportKind, type LanguageClientOptions, type ServerOptions } from "vscode-languageclient/node";
import type { Logger } from "./logger";

export function createLanguageClient(context: vscode.ExtensionContext, logger: Logger): LanguageClient {
    const module = context.asAbsolutePath(path.join("out", "server", "src", "server.js"));
    const serverOptions: ServerOptions = {
        run: { module, transport: TransportKind.ipc },
        debug: { module, transport: TransportKind.ipc, options: { execArgv: ["--nolazy", "--inspect=6009"] } },
    };
    const clientOptions: LanguageClientOptions = {
        documentSelector: [
            { scheme: "file", language: "ura" },
            { scheme: "untitled", language: "ura" },
        ],
        synchronize: { configurationSection: "ura" },
        outputChannel: logger.channel,
    };
    return new LanguageClient("ura", "Ura Language Server", serverOptions, clientOptions);
}
