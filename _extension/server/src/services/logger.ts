// Logging for the server: everything goes to the "Ura" output channel
// (the client shows the server's console there).

import { inspect } from "util";
import { ResponseError, type Connection } from "vscode-languageserver/node";

export interface Logger {
    info(message: string): void;
    error(message: string, error?: unknown): void;
    // A warning the user can act on (e.g. "compiler not found"), shown once per message.
    notifyOnce(message: string): void;
}

function describe(error: unknown): string {
    if (error instanceof Error) {
        return error.stack ?? error.message;
    }
    return inspect(error);
}

export function createLogger(connection: Connection): Logger {
    const notified = new Set<string>();
    return {
        info(message: string): void {
            connection.console.info(message);
        },
        error(message: string, error?: unknown): void {
            connection.console.error(error === undefined ? message : `${message}\n${describe(error)}`);
        },
        notifyOnce(message: string): void {
            connection.console.warn(message);
            if (notified.has(message)) {
                return;
            }
            notified.add(message);
            void connection.window.showWarningMessage(message);
        },
    };
}

// Wraps a request handler: an exception is logged and answered with `fallback`,
// so one bad document never takes the server down. A ResponseError is an answer
// meant for the user (e.g. "this name is taken"): it goes through as is.
export function guard<P, R>(
    logger: Logger,
    name: string,
    fallback: R,
    handler: (params: P) => R | Promise<R>,
): (params: P) => Promise<R> {
    return async (params: P): Promise<R> => {
        try {
            return await handler(params);
        } catch (error) {
            if (error instanceof ResponseError) {
                throw error;
            }
            logger.error(`${name} failed`, error);
            return fallback;
        }
    };
}
