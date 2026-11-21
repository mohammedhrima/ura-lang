// The `ura.*` settings, read once and again whenever they change.

import type { Connection } from "vscode-languageserver/node";
import type { Logger } from "./logger";

export interface UraSettings {
    compilerPath: string;
    lint: { enable: boolean };
}

const DEFAULTS: UraSettings = { compilerPath: "", lint: { enable: true } };

export interface Config {
    get(): UraSettings;
    onChange(listener: () => void): void;
}

export interface ConfigControl extends Config {
    // false when the client can't answer workspace/configuration: defaults are used
    setSupported(supported: boolean): void;
    refresh(): Promise<void>;
}

function merge(value: unknown): UraSettings {
    if (typeof value !== "object" || value === null) {
        return DEFAULTS;
    }
    const raw = value as { compilerPath?: unknown; lint?: { enable?: unknown } };
    return {
        compilerPath: typeof raw.compilerPath === "string" ? raw.compilerPath : DEFAULTS.compilerPath,
        lint: { enable: typeof raw.lint?.enable === "boolean" ? raw.lint.enable : DEFAULTS.lint.enable },
    };
}

export function createConfig(connection: Connection, logger: Logger): ConfigControl {
    let settings = DEFAULTS;
    let supported = false;
    const listeners: (() => void)[] = [];

    return {
        get: (): UraSettings => settings,
        onChange(listener: () => void): void {
            listeners.push(listener);
        },
        setSupported(value: boolean): void {
            supported = value;
        },
        async refresh(): Promise<void> {
            if (supported) {
                try {
                    settings = merge(await connection.workspace.getConfiguration({ section: "ura" }));
                } catch (error) {
                    logger.error("reading the ura settings failed, using the defaults", error);
                    settings = DEFAULTS;
                }
            }
            for (const listener of listeners) {
                listener();
            }
        },
    };
}
