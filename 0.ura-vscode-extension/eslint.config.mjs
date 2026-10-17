// Lint rules for the whole extension. The `no-restricted-imports` blocks keep
// the layers apart: core (pure) ← server features ← server; client separate.

import js from "@eslint/js";
import tseslint from "typescript-eslint";

const banned = (names, message) => names.map((name) => ({ name, message }));

export default tseslint.config(
    { ignores: ["out/**", "node_modules/**", ".vscode-test/**", "eslint.config.mjs"] },
    js.configs.recommended,
    ...tseslint.configs.recommendedTypeChecked,
    {
        languageOptions: {
            parserOptions: { projectService: true, tsconfigRootDir: import.meta.dirname },
        },
        rules: {
            "@typescript-eslint/no-explicit-any": "error",
            "@typescript-eslint/explicit-function-return-type": [
                "error",
                { allowExpressions: true, allowTypedFunctionExpressions: true },
            ],
            "@typescript-eslint/no-floating-promises": [
                "error",
                // node:test tracks the promises its describe/test return
                {
                    allowForKnownSafeCalls: [
                        { from: "package", package: "node:test", name: ["describe", "test", "it", "suite"] },
                    ],
                },
            ],
            "@typescript-eslint/no-misused-promises": "error",
            "@typescript-eslint/consistent-type-imports": "error",
            "no-empty": ["error", { allowEmptyCatch: false }],
        },
    },
    {
        files: ["server/**/*.ts"],
        rules: {
            "no-restricted-imports": [
                "error",
                {
                    paths: banned(
                        ["vscode", "vscode-languageclient", "vscode-languageclient/node"],
                        "the server runs outside VS Code: use the LSP connection",
                    ),
                },
            ],
        },
    },
    {
        files: ["server/src/core/**/*.ts"],
        rules: {
            "no-restricted-imports": [
                "error",
                {
                    paths: banned(
                        [
                            "vscode",
                            "vscode-languageclient",
                            "vscode-languageclient/node",
                            "vscode-languageserver",
                            "vscode-languageserver/node",
                            "fs",
                            "child_process",
                        ],
                        "core is pure: take data and callbacks (e.g. Reader) instead",
                    ),
                },
            ],
        },
    },
    {
        files: ["client/**/*.ts"],
        rules: {
            "no-restricted-imports": [
                "error",
                {
                    paths: banned(
                        ["vscode-languageserver", "vscode-languageserver/node"],
                        "the client only talks to the server through vscode-languageclient",
                    ),
                    patterns: [{ group: ["**/server/**"], message: "the client never imports server code" }],
                },
            ],
        },
    },
);
