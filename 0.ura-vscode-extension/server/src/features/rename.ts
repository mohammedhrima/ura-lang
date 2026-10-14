// Rename (F2): every occurrence, in every file that refers to the symbol.
// Refuses keywords, invalid names, `self` and names already taken.

import {
    ErrorCodes,
    ResponseError,
    type PrepareRenameParams,
    type Range,
    type RenameParams,
    type TextEdit,
    type WorkspaceEdit,
} from "vscode-languageserver/node";
import { findBuiltin } from "../core/keywords";
import { analyze, resolve, wordAt } from "../core/parser";
import { isLocal, occurrences, renameProblem, type FileContext } from "../core/references";
import { declarationOf } from "../core/symbols";
import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

function prepare(s: Services, params: PrepareRenameParams): { range: Range; placeholder: string } | null {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return null;
    }
    const context = s.workspace.contextOf(document);
    const line = context.analysis.lines[params.position.line] ?? "";
    const word = wordAt(line, params.position.character);
    const target = resolve(context.analysis, params.position, context.imports);
    if (!word || !target || findBuiltin(word.text)) { // built-ins belong to the language
        throw new ResponseError(ErrorCodes.InvalidRequest, "nothing to rename here");
    }
    if (target.kind === "variable" && target.variable.name === "self") {
        throw new ResponseError(ErrorCodes.InvalidRequest, "'self' can't be renamed");
    }
    const at = params.position.line;
    return { range: { start: { line: at, character: word.start }, end: { line: at, character: word.end } }, placeholder: word.text };
}

// The context of the file that declares `target` (to check name clashes there).
function declaringContext(s: Services, here: FileContext, file: string | undefined): FileContext {
    if (file === undefined || file === here.file) {
        return here;
    }
    const source = s.workspace.read(file) ?? "";
    return { file, analysis: analyze(source), imports: here.imports };
}

function rename(s: Services, params: RenameParams): WorkspaceEdit | null {
    const document = s.documents.get(params.textDocument.uri);
    if (!document) {
        return null;
    }
    const here = s.workspace.contextOf(document);
    const target = resolve(here.analysis, params.position, here.imports);
    if (!target) {
        return null;
    }
    const declaring = declaringContext(s, here, declarationOf(target).file);
    const problem = renameProblem(declaring, target, params.newName);
    if (problem) {
        throw new ResponseError(ErrorCodes.InvalidRequest, problem);
    }
    const changes: Record<string, TextEdit[]> = {};
    for (const context of isLocal(target) ? [here] : s.workspace.allContexts()) {
        const edits = occurrences(context, target, here.file).map((span) => ({
            range: { start: { line: span.line, character: span.start }, end: { line: span.line, character: span.start + span.length } },
            newText: params.newName,
        }));
        if (edits.length > 0) {
            changes[s.workspace.uriOfContext(context.file)] = edits;
        }
    }
    return { changes };
}

export const feature: Feature = {
    capabilities: { renameProvider: { prepareProvider: true } },
    register(s: Services): void {
        s.connection.onPrepareRename(guard(s.logger, "prepare rename", null, (params: PrepareRenameParams) => prepare(s, params)));
        s.connection.onRenameRequest(guard(s.logger, "rename", null, (params: RenameParams) => rename(s, params)));
    },
};
