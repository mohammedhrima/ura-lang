// End-to-end: the real extension, the real language server, a real VS Code.
// Every check goes through VS Code's own commands, the way the editor asks.

import * as assert from "assert";
import * as fs from "fs";
import * as path from "path";
import * as vscode from "vscode";

const FIXTURES = path.resolve(__dirname, "../../../../test/e2e/fixtures");
const COMPILER = path.resolve(__dirname, "../../../../../bin/ura"); // out/test/e2e/suite → repo/bin/ura

function fixture(name: string): vscode.Uri {
    return vscode.Uri.file(path.join(FIXTURES, name));
}

// Retries `get` until `ok` accepts its result: the server answers once it's up.
async function eventually<T>(get: () => T | Thenable<T>, ok: (value: T) => boolean, what: string): Promise<T> {
    const deadline = Date.now() + 20_000;
    let last: T | undefined;
    while (Date.now() < deadline) {
        last = await get();
        if (ok(last)) {
            return last;
        }
        await new Promise((resolve) => setTimeout(resolve, 250));
    }
    assert.fail(`${what}: last result ${JSON.stringify(last)}`);
}

function hoverText(hovers: vscode.Hover[]): string {
    return hovers
        .flatMap((h) => h.contents)
        .map((c) => (typeof c === "string" ? c : c.value))
        .join("\n");
}

describe("ura extension", () => {
    let main: vscode.TextDocument;

    before(async () => {
        main = await vscode.workspace.openTextDocument(fixture("main.ura"));
        await vscode.window.showTextDocument(main);
    });

    it("activates on .ura files", async () => {
        const extension = vscode.extensions.getExtension("mohamedhrima.ura-lang");
        assert.ok(extension, "extension not found");
        await eventually(() => extension.isActive, (active) => active, "activation");
    });

    it("completes functions from this file and used files, and visible variables", async () => {
        const list = await eventually(
            () => vscode.commands.executeCommand<vscode.CompletionList>("vscode.executeCompletionItemProvider", main.uri, new vscode.Position(8, 4)),
            (l) => l.items.some((i) => i.label === "help"),
            "completion",
        );
        const labels = list.items.map((i) => (typeof i.label === "string" ? i.label : i.label.label));
        for (const expected of ["twice", "help", "a", "return"]) {
            assert.ok(labels.includes(expected), `missing ${expected}`);
        }
    });

    it("hovers a function call with its signature", async () => {
        const hovers = await eventually(
            () => vscode.commands.executeCommand<vscode.Hover[]>("vscode.executeHoverProvider", main.uri, new vscode.Position(7, 13)),
            (h) => h.length > 0,
            "hover",
        );
        assert.match(hoverText(hovers), /fn twice\(n i32\) i32/);
    });

    it("shows signature help inside a call", async () => {
        const help = await eventually(
            () => vscode.commands.executeCommand<vscode.SignatureHelp>("vscode.executeSignatureHelpProvider", main.uri, new vscode.Position(8, 16)),
            (h) => h !== undefined && h.signatures.length > 0,
            "signature help",
        );
        assert.equal(help.signatures[0]?.label, "fn help(n i32) i32");
    });

    it("goes to a definition in a used file", async () => {
        const locations = await eventually(
            () => vscode.commands.executeCommand<vscode.Location[]>("vscode.executeDefinitionProvider", main.uri, new vscode.Position(8, 12)),
            (l) => l.length > 0,
            "definition",
        );
        assert.equal(path.basename(locations[0]?.uri.fsPath ?? ""), "helper.ura");
        assert.equal(locations[0]?.range.start.line, 4);
    });

    it("links the path of a use", async () => {
        const links = await eventually(
            () => vscode.commands.executeCommand<vscode.DocumentLink[]>("vscode.executeLinkProvider", main.uri),
            (l) => l.length > 0,
            "document links",
        );
        assert.equal(path.basename(links[0]?.target?.fsPath ?? ""), "helper.ura");
    });

    it("colors struct names from used files", async () => {
        const tokens = await eventually(
            () => vscode.commands.executeCommand<vscode.SemanticTokens | undefined>("vscode.provideDocumentSemanticTokens", main.uri),
            (t) => t !== undefined && t.data.length > 0,
            "semantic tokens",
        );
        // one token: line 6, character 6, length 5 ("Point"), type 0 (struct)
        assert.deepEqual(Array.from(tokens?.data ?? []), [6, 6, 5, 0, 0]);
    });

    it("shows the compiler's errors as problems", async function () {
        if (!fs.existsSync(COMPILER)) {
            this.skip(); // no compiler built: run `build` in src/ first
        }
        await vscode.workspace.getConfiguration("ura").update("compilerPath", COMPILER, vscode.ConfigurationTarget.Global);
        const broken = await vscode.workspace.openTextDocument(fixture("broken.ura"));
        await vscode.window.showTextDocument(broken);
        const diagnostics = await eventually(
            () => vscode.languages.getDiagnostics(broken.uri),
            (d) => d.length > 0,
            "diagnostics",
        );
        assert.equal(diagnostics[0]?.message, "undeclared variable 'x'");
        assert.equal(diagnostics[0]?.source, "ura");
    });
});

describe("ura extension: formatting and navigation", () => {
    let shapes: vscode.TextDocument;
    const at = (line: number, character: number): vscode.Position => new vscode.Position(line, character);

    before(async () => {
        shapes = await vscode.workspace.openTextDocument(fixture("shapes.ura"));
        await vscode.window.showTextDocument(shapes);
    });

    it("formats a document", async () => {
        const messy = await vscode.workspace.openTextDocument(fixture("messy.ura"));
        const edits = await eventually(
            () => vscode.commands.executeCommand<vscode.TextEdit[]>("vscode.executeFormatDocumentProvider", messy.uri, { tabSize: 4, insertSpaces: true }),
            (e) => e !== undefined && e.length > 0,
            "formatting",
        );
        // VS Code turns the server's edit into minimal edits: apply them, compare the text
        let text = messy.getText();
        const sorted = [...edits].sort((a, b) => messy.offsetAt(b.range.start) - messy.offsetAt(a.range.start));
        for (const edit of sorted) {
            text = text.slice(0, messy.offsetAt(edit.range.start)) + edit.newText + text.slice(messy.offsetAt(edit.range.end));
        }
        assert.equal(text, "fn main() i32:\n    x i32 = 1 + 2\n    return x\n");
    });

    it("outlines structs with their attributes and methods", async () => {
        const symbols = await eventually(
            () => vscode.commands.executeCommand<vscode.DocumentSymbol[]>("vscode.executeDocumentSymbolProvider", shapes.uri),
            (s) => s !== undefined && s.length > 0,
            "document symbols",
        );
        assert.deepEqual(symbols.map((s) => s.name), ["Box", "main"]);
        assert.deepEqual(symbols[0]?.children.map((c) => c.name), ["w", "h", "area"]);
    });

    it("completes the members of a struct after a dot", async () => {
        const list = await eventually(
            () => vscode.commands.executeCommand<vscode.CompletionList>("vscode.executeCompletionItemProvider", shapes.uri, at(11, 13), "."),
            (l) => l.items.length > 0,
            "member completion",
        );
        const labels = list.items.map((i) => (typeof i.label === "string" ? i.label : i.label.label)).sort();
        assert.deepEqual(labels, ["area", "h", "w"]);
    });

    it("hovers a struct with its attributes", async () => {
        const hovers = await vscode.commands.executeCommand<vscode.Hover[]>("vscode.executeHoverProvider", shapes.uri, at(8, 6));
        assert.match(hoverText(hovers), /struct Box:\n\s+w i32\n\s+h i32/);
    });

    it("goes to an attribute's declaration", async () => {
        const locations = await vscode.commands.executeCommand<vscode.Location[]>("vscode.executeDefinitionProvider", shapes.uri, at(9, 6));
        assert.equal(locations[0]?.range.start.line, 1);
    });

    it("finds every reference of an attribute", async () => {
        const locations = await vscode.commands.executeCommand<vscode.Location[]>("vscode.executeReferenceProvider", shapes.uri, at(1, 4));
        assert.deepEqual(locations.map((l) => l.range.start.line).sort((a, b) => a - b), [1, 5, 9]);
    });

    it("highlights a variable's uses", async () => {
        const highlights = await vscode.commands.executeCommand<vscode.DocumentHighlight[]>("vscode.executeDocumentHighlights", shapes.uri, at(8, 4));
        assert.deepEqual(highlights.map((h) => h.range.start.line), [8, 9, 10, 11]);
        assert.equal(highlights[0]?.kind, vscode.DocumentHighlightKind.Write);
    });

    it("renames an attribute everywhere", async () => {
        const edit = await vscode.commands.executeCommand<vscode.WorkspaceEdit>("vscode.executeDocumentRenameProvider", shapes.uri, at(1, 4), "width");
        const edits = edit.get(shapes.uri);
        assert.deepEqual(edits.map((e) => e.range.start.line).sort((a, b) => a - b), [1, 5, 9]);
        assert.ok(edits.every((e) => e.newText === "width"));
    });

    it("refuses a rename to a name already taken", async () => {
        await assert.rejects(
            Promise.resolve(vscode.commands.executeCommand("vscode.executeDocumentRenameProvider", shapes.uri, at(1, 4), "h")),
            /already exists/,
        );
    });

    it("shows a method's signature while calling it", async () => {
        const help = await vscode.commands.executeCommand<vscode.SignatureHelp>("vscode.executeSignatureHelpProvider", shapes.uri, at(11, 18));
        assert.equal(help.signatures[0]?.label, "fn Box.area() i32");
    });

    it("finds symbols in the whole workspace", async () => {
        const symbols = await vscode.commands.executeCommand<vscode.SymbolInformation[]>("vscode.executeWorkspaceSymbolProvider", "area");
        const area = symbols.find((s) => s.name === "area");
        assert.equal(area?.containerName, "Box");
        assert.equal(path.basename(area?.location.uri.fsPath ?? ""), "shapes.ura");
    });

    it("registers Run and Build", async () => {
        const commands = await vscode.commands.getCommands(true);
        assert.ok(commands.includes("ura.run") && commands.includes("ura.build"));
    });
});
