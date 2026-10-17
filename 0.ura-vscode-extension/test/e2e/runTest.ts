// Starts VS Code with the extension under development and runs the E2E suite
// inside it. VSCODE_EXECUTABLE picks an installed VS Code; empty: download one.

import * as path from "path";
import { runTests } from "@vscode/test-electron";

// When this runs from a VS Code terminal, it inherits variables that make the
// test VS Code start as plain Node (ELECTRON_RUN_AS_NODE) or talk to the running
// VS Code (VSCODE_IPC_HOOK, ...). The test instance must start clean.
function cleanEnvironment(): void {
    for (const name of Object.keys(process.env)) {
        if (name.startsWith("ELECTRON_") || name.startsWith("VSCODE_")) {
            delete process.env[name];
        }
    }
}

async function main(): Promise<void> {
    const extensionDevelopmentPath = path.resolve(__dirname, "../../.."); // out/test/e2e → extension root
    const extensionTestsPath = path.resolve(__dirname, "suite");
    const fixtures = path.resolve(extensionDevelopmentPath, "test", "e2e", "fixtures");
    const executable = process.env.VSCODE_EXECUTABLE;
    cleanEnvironment();
    await runTests({
        extensionDevelopmentPath,
        extensionTestsPath,
        launchArgs: [fixtures, "--disable-extensions"],
        ...(executable ? { vscodeExecutablePath: executable } : {}),
    });
}

main().catch((error: unknown) => {
    console.error("E2E tests failed:", error);
    process.exit(1);
});
