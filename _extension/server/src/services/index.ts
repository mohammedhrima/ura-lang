// What every feature gets: built once in server.ts and passed in.

import type { Connection, ServerCapabilities, TextDocuments } from "vscode-languageserver/node";
import type { TextDocument } from "vscode-languageserver-textdocument";
import type { Compiler } from "./compiler";
import type { Config } from "./config";
import type { Logger } from "./logger";
import type { Workspace } from "./workspace";

export interface Services {
    connection: Connection;
    documents: TextDocuments<TextDocument>;
    workspace: Workspace;
    config: Config;
    logger: Logger;
    compiler: Compiler;
}

// One language feature: what it adds to the server's capabilities, and the
// handlers it registers. Features never import each other.
export interface Feature {
    capabilities: ServerCapabilities;
    register(services: Services): void;
}
