// What every client command gets: built once in extension.ts and passed in.

import type * as vscode from "vscode";
import type { LanguageClient } from "vscode-languageclient/node";
import type { Logger } from "./logger";

export interface ClientServices {
    context: vscode.ExtensionContext;
    client: LanguageClient;
    logger: Logger;
}
