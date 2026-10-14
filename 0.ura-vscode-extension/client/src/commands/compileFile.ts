// Ura: Build File and Ura: Run File. Saves the file, compiles it with the
// compiler the server picks (build/<name> next to the file) and, for Run,
// runs it in the "Ura" terminal and prints its exit code.

import * as path from "path";
import * as vscode from "vscode";
import type { ClientServices } from "../services";

// must match server/src/features/compilerPath.ts
const COMPILER_PATH_REQUEST = "ura/compilerPath";

// single-quoted for a POSIX shell
function quote(text: string): string {
    return `'${text.replace(/'/g, "'\\''")}'`;
}

function terminal(services: ClientServices): vscode.Terminal {
    const existing = vscode.window.terminals.find((t) => t.name === "Ura");
    if (existing) {
        return existing;
    }
    const created = vscode.window.createTerminal({ name: "Ura" });
    services.context.subscriptions.push(created);
    return created;
}

async function compile(services: ClientServices, run: boolean): Promise<void> {
    const document = vscode.window.activeTextEditor?.document;
    if (!document || document.languageId !== "ura") {
        void vscode.window.showWarningMessage("Open a .ura file to build or run it.");
        return;
    }
    if (document.isUntitled) {
        void vscode.window.showWarningMessage("Save the file first: the compiler needs it on disk.");
        return;
    }
    if (document.isDirty && !(await document.save())) {
        void vscode.window.showWarningMessage(`Couldn't save ${path.basename(document.fileName)}.`);
        return;
    }
    const { path: compiler } = await services.client.sendRequest<{ path: string }>(COMPILER_PATH_REQUEST);
    const file = document.uri.fsPath;
    const executable = path.join("build", path.basename(file, ".ura"));
    const steps = [`cd ${quote(path.dirname(file))}`, "mkdir -p build", `${quote(compiler)} ${quote(file)} -o ${quote(executable)}`];
    if (run) {
        steps.push(quote(`./${executable}`));
    }
    const shell = terminal(services);
    shell.show(true);
    shell.sendText(`${steps.join(" && ")}; echo "[ura] exit code: $?"`);
    services.logger.info(`${run ? "run" : "build"}: ${file} with ${compiler}`);
}

export function register(services: ClientServices): void {
    const failed = (what: string) => (error: unknown): void => {
        services.logger.error(`${what} failed`, error);
        void vscode.window.showErrorMessage(`Ura: ${what} failed, see the "Ura" output.`);
    };
    services.context.subscriptions.push(
        vscode.commands.registerCommand("ura.build", () => compile(services, false).catch(failed("build"))),
        vscode.commands.registerCommand("ura.run", () => compile(services, true).catch(failed("run"))),
    );
}
