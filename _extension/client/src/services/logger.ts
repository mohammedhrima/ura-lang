// The "Ura" output channel: the client's own messages and the server's console.

import { inspect } from "util";
import * as vscode from "vscode";

export interface Logger extends vscode.Disposable {
    channel: vscode.OutputChannel;
    info(message: string): void;
    error(message: string, error?: unknown): void;
}

export function createLogger(): Logger {
    const channel = vscode.window.createOutputChannel("Ura");
    return {
        channel,
        info(message: string): void {
            channel.appendLine(message);
        },
        error(message: string, error?: unknown): void {
            const details = error instanceof Error ? (error.stack ?? error.message) : inspect(error);
            channel.appendLine(error === undefined ? message : `${message}\n${details}`);
        },
        dispose(): void {
            channel.dispose();
        },
    };
}
