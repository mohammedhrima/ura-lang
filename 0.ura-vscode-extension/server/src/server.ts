// The ura language server: builds the services once, then lets every feature
// register itself. Adding a feature = one file in features/ + one line below.

import {
    createConnection,
    ProposedFeatures,
    TextDocuments,
    TextDocumentSyncKind,
    type InitializeParams,
    type InitializeResult,
    type ServerCapabilities,
} from "vscode-languageserver/node";
import { TextDocument } from "vscode-languageserver-textdocument";
import * as completion from "./features/completion";
import * as compilerPath from "./features/compilerPath";
import * as definition from "./features/definition";
import * as diagnostics from "./features/diagnostics";
import * as formatting from "./features/formatting";
import * as highlight from "./features/highlight";
import * as hover from "./features/hover";
import * as links from "./features/links";
import * as references from "./features/references";
import * as rename from "./features/rename";
import * as semanticTokens from "./features/semanticTokens";
import * as signature from "./features/signature";
import * as symbols from "./features/symbols";
import * as workspaceSymbols from "./features/workspaceSymbols";
import type { Feature, Services } from "./services";
import { createCompiler } from "./services/compiler";
import { createConfig } from "./services/config";
import { createLogger } from "./services/logger";
import { createWorkspace } from "./services/workspace";

const FEATURES: Feature[] = [
    completion.feature,
    hover.feature,
    signature.feature,
    definition.feature,
    links.feature,
    semanticTokens.feature,
    diagnostics.feature,
    formatting.feature,
    symbols.feature,
    workspaceSymbols.feature,
    references.feature,
    highlight.feature,
    rename.feature,
    compilerPath.feature,
];

const connection = createConnection(ProposedFeatures.all);
const documents = new TextDocuments(TextDocument);
const logger = createLogger(connection);
const workspace = createWorkspace(documents);
const config = createConfig(connection, logger);
const compiler = createCompiler(config, workspace, logger);
const services: Services = { connection, documents, workspace, config, logger, compiler };

connection.onInitialize((params: InitializeParams): InitializeResult => {
    workspace.setRoot(params.workspaceFolders?.[0]?.uri ?? params.rootUri ?? undefined);
    config.setSupported(params.capabilities.workspace?.configuration === true);
    const capabilities: ServerCapabilities = {
        textDocumentSync: { openClose: true, change: TextDocumentSyncKind.Incremental, save: true },
    };
    for (const feature of FEATURES) {
        Object.assign(capabilities, feature.capabilities);
    }
    return { capabilities };
});

connection.onInitialized(() => {
    config.refresh().catch((error: unknown) => logger.error("reading the settings failed", error));
});

connection.onDidChangeConfiguration(() => {
    config.refresh().catch((error: unknown) => logger.error("reading the settings failed", error));
});

connection.onShutdown(() => {
    compiler.dispose();
});

for (const feature of FEATURES) {
    feature.register(services);
}

documents.listen(connection);
connection.listen();
