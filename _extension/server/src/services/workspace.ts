// Files as the server sees them: open documents first (they may hold unsaved
// text), then the disk; plus the conversions between URIs and paths.

import * as fs from "fs";
import * as path from "path";
import { fileURLToPath, pathToFileURL } from "url";
import type { TextDocuments } from "vscode-languageserver/node";
import type { TextDocument } from "vscode-languageserver-textdocument";
import { importsOf, type Reader } from "../core/imports";
import { analyze, NO_IMPORTS, type Imports } from "../core/parser";
import { CONFIG, findProject, packageDirs, parseConfig, type Project } from "../core/project";
import type { FileContext } from "../core/references";

// Folders never searched for .ura files.
const SKIPPED = new Set(["node_modules", "out", "build", "bin", "dependencies", ".git", ".vscode-test"]);
const MAX_FILES = 5000;

export const readFromDisk: Reader = (file) => {
    try {
        return fs.readFileSync(file, "utf8");
    } catch {
        return undefined; // missing or unreadable: callers treat it as "no such file"
    }
};

export interface Workspace {
    root(): string | undefined;
    setRoot(uri: string | undefined): void;
    // the ura.conf project holding `file`, undefined outside any project
    projectOf(file: string): Project | undefined;
    // where `use "@/name"` looks from `file`: its project and the project's
    // dependencies (nothing outside a project)
    libs(file: string | undefined): string[];
    read: Reader;
    exists(file: string): boolean;
    // the path of a file: URI; undefined for untitled documents
    fileOf(uri: string): string | undefined;
    uriOf(file: string): string;
    // what a document brings in through `use` (nothing for untitled documents)
    imports(document: TextDocument): Imports;
    // a document as the lookups see it; `file` is its path, or its URI when untitled
    contextOf(document: TextDocument): FileContext;
    // the URI of a FileContext's `file`
    uriOfContext(file: string): string;
    // every .ura file of the workspace and every open document, once each
    allContexts(): FileContext[];
}

// The .ura files under `root` (none without a workspace folder).
function uraFiles(root: string | undefined): string[] {
    const files: string[] = [];
    const visit = (dir: string): void => {
        let entries: fs.Dirent[];
        try {
            entries = fs.readdirSync(dir, { withFileTypes: true });
        } catch {
            return; // unreadable folder: skip it
        }
        for (const entry of entries) {
            if (files.length >= MAX_FILES) {
                return;
            }
            const full = path.join(dir, entry.name);
            if (entry.isDirectory() && !SKIPPED.has(entry.name) && !entry.name.startsWith(".")) {
                visit(full);
            } else if (entry.isFile() && entry.name.endsWith(".ura")) {
                files.push(full);
            }
        }
    };
    if (root !== undefined) {
        visit(root);
    }
    return files;
}

export function createWorkspace(documents: TextDocuments<TextDocument>): Workspace {
    let rootPath: string | undefined;

    const fileOf = (uri: string): string | undefined =>
        uri.startsWith("file:") ? fileURLToPath(uri) : undefined;
    const uriOf = (file: string): string => pathToFileURL(file).toString();
    const read: Reader = (file) => documents.get(uriOf(file))?.getText() ?? readFromDisk(file);
    const exists = (file: string): boolean => fs.existsSync(file);
    const projectOf = (file: string): Project | undefined => {
        const dir = findProject(file, exists);
        const text = dir === undefined ? undefined : read(path.join(dir, CONFIG));
        return dir === undefined || text === undefined ? undefined : parseConfig(dir, text);
    };
    const libs = (file: string | undefined): string[] => {
        const project = file === undefined ? undefined : projectOf(file);
        return project ? packageDirs(project) : [];
    };
    const context = (file: string | undefined, uri: string, source: string): FileContext => {
        const imports = file === undefined ? NO_IMPORTS : importsOf(file, source, read, libs(file));
        return { file: file ?? uri, analysis: analyze(source), imports };
    };

    return {
        root: (): string | undefined => rootPath,
        setRoot(uri: string | undefined): void {
            rootPath = uri === undefined ? undefined : fileOf(uri);
        },
        projectOf,
        libs,
        read,
        exists,
        fileOf,
        uriOf,
        imports(document: TextDocument): Imports {
            const file = fileOf(document.uri);
            return file === undefined ? NO_IMPORTS : importsOf(file, document.getText(), read, libs(file));
        },
        contextOf(document: TextDocument): FileContext {
            return context(fileOf(document.uri), document.uri, document.getText());
        },
        uriOfContext(file: string): string {
            return path.isAbsolute(file) ? uriOf(file) : file;
        },
        allContexts(): FileContext[] {
            const contexts = new Map<string, FileContext>();
            for (const document of documents.all()) {
                const found = context(fileOf(document.uri), document.uri, document.getText());
                contexts.set(found.file, found);
            }
            for (const file of uraFiles(rootPath)) {
                const source = contexts.has(file) ? undefined : readFromDisk(file);
                if (source !== undefined) {
                    contexts.set(file, context(file, file, source));
                }
            }
            return [...contexts.values()];
        },
    };
}
