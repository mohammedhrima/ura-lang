import * as vscode from "vscode";
import { builtinSignature, findBuiltin } from "../analysis/keywords";
import { analyze, callContext, findFunction, functionSignature } from "../analysis/parser";
import { importsOf } from "./documents";

export class SignatureProvider implements vscode.SignatureHelpProvider {
    provideSignatureHelp(
        document: vscode.TextDocument,
        position: vscode.Position,
    ): vscode.SignatureHelp | undefined {
        const line = document.lineAt(position.line).text;
        const call = callContext(line, position.character);
        if (!call) {
            return undefined;
        }

        let signature: vscode.SignatureInformation;
        const builtin = findBuiltin(call.name);
        if (builtin) {
            signature = new vscode.SignatureInformation(builtinSignature(builtin), builtin.doc);
            signature.parameters = builtin.params.map((p) => new vscode.ParameterInformation(p));
        } else {
            const fn = findFunction(analyze(document.getText()), call.name, importsOf(document));
            if (!fn) {
                return undefined;
            }
            signature = new vscode.SignatureInformation(functionSignature(fn));
            signature.parameters = fn.params.map(
                (p) => new vscode.ParameterInformation(`${p.name} ${p.type}`.trim()),
            );
        }

        const help = new vscode.SignatureHelp();
        help.signatures = [signature];
        help.activeSignature = 0;
        help.activeParameter = call.argument;
        return help;
    }
}
