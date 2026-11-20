// Mocha entry point that VS Code loads in the extension host.

import * as fs from "fs";
import * as path from "path";
import Mocha from "mocha";

export function run(): Promise<void> {
    const mocha = new Mocha({ ui: "bdd", color: true, timeout: 30_000 });
    for (const file of fs.readdirSync(__dirname).filter((f) => f.endsWith(".test.js"))) {
        mocha.addFile(path.join(__dirname, file));
    }
    return new Promise((resolve, reject) => {
        mocha.run((failures) => {
            if (failures > 0) {
                reject(new Error(`${failures} E2E test(s) failed`));
            } else {
                resolve();
            }
        });
    });
}
