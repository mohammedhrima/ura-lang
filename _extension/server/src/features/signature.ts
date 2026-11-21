// Signature help while typing a call: built-ins, functions and methods
// (`u.add(` shows `fn User.add(n i32) i32`; `self` is implicit).

import type { SignatureHelp, SignatureHelpParams, SignatureInformation } from "vscode-languageserver/node";
import { builtinSignature, findBuiltin } from "../core/keywords";
import { callContext, functionSignature, resolve } from "../core/parser";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function help(s: Services, params: SignatureHelpParams): SignatureHelp | null {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return null;
    }
    const context = s.workspace.contextOf(document);
    const line = context.analysis.lines[params.position.line] ?? "";
    const call = callContext(line, params.position.character);
    if (!call) {
        return null;
    }

    let signature: SignatureInformation;
    const builtin = findBuiltin(call.name);
    if (builtin) {
        signature = {
            label: builtinSignature(builtin),
            documentation: builtin.doc,
            parameters: builtin.params.map((p) => ({ label: p })),
        };
    } else {
        const found = resolve(context.analysis, { line: params.position.line, character: call.start }, context.imports);
        if (!found || (found.kind !== "function" && found.kind !== "method")) {
            return null;
        }
        const fn = found.function;
        signature = {
            label: functionSignature(fn),
            parameters: fn.params.map((p) => ({ label: `${p.name} ${p.type}`.trim() })),
        };
    }
    return { signatures: [signature], activeSignature: 0, activeParameter: call.argument };
}

export const feature: Feature = {
    capabilities: { signatureHelpProvider: { triggerCharacters: ["(", ","] } },
    register(s: Services): void {
        s.connection.onSignatureHelp(guard(s.logger, "signature help", null, (params: SignatureHelpParams) => help(s, params)));
    },
};
