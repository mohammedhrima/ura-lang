// `ura/compilerPath`: tells the client which compiler to run for Run / Build,
// so the choice (setting, workspace bin/ura, PATH) lives in one place.

import { guard } from "../services/logger";
import type { Feature, Services } from "../services";

export const COMPILER_PATH_REQUEST = "ura/compilerPath";

export interface CompilerPathResult {
    path: string;
}

export const feature: Feature = {
    capabilities: {},
    register(s: Services): void {
        s.connection.onRequest(
            COMPILER_PATH_REQUEST,
            guard(s.logger, COMPILER_PATH_REQUEST, { path: "ura" }, (): CompilerPathResult => ({ path: s.compiler.path() })),
        );
    },
};
