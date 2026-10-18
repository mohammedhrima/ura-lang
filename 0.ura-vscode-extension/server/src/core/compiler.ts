// Turns what the ura compiler prints on stderr into problems.
// Running the compiler lives in services/compiler.ts.

export interface Problem {
    message: string;
    line?: number; // 0-based; undefined when the compiler gave no position
    character?: number; // 0-based
    length?: number; // how many characters the compiler underlined (its ^^^)
    help?: string; // how to fix it, from the compiler's "help:" line
    code?: string; // where in the compiler the error came from, e.g. "main.c:827 analyze"
}

// eslint-disable-next-line no-control-regex -- ANSI color codes start with ESC (\x1b)
export const COLORS = /\x1b\[[0-9;]*m/g;
// "error: main.ura:2:15 unknown character '$'" (also without "error:", with a ':' after the column)
const POSITIONED = /^(?:error:\s*)?(.*\.ura):(\d+):(\d+):?\s*(.*)$/;
// the code snippet under an error: "  |", "2 |     x i32 = 5 $ 3", "  |               ^"
const SNIPPET = /^\s*\d*\s*\|/;
const UNDERLINE = /^\s*\|\s*(\^+)\s*$/;
const HELP = /^help:\s*(.*)$/;
// what eprint prints: "<compiler file>:<line> <function> <message>"
const COMPILER_SIDE = /^(\S+\.[ch]:\d+(?: \w+)?) (.*)$/;
const SANITIZER = /==\d+==ERROR: (AddressSanitizer: .*)/;

export function parseStderr(stderr: string): Problem[] {
    const text = stderr.replace(COLORS, "");
    const crash = SANITIZER.exec(text);
    if (crash) {
        return [{ message: `compiler crashed: ${(crash[1] ?? "").trim()}` }];
    }

    const problems: Problem[] = [];
    for (const raw of text.split("\n")) {
        const line = raw.trimEnd();
        if (!line.trim()) {
            continue;
        }
        const positioned = POSITIONED.exec(line);
        if (positioned) {
            problems.push({
                message: positioned[4] ?? "",
                line: Number(positioned[2]) - 1,
                character: Number(positioned[3]) - 1,
            });
            continue;
        }
        const compilerSide = COMPILER_SIDE.exec(line);
        if (compilerSide) {
            problems.push({ message: (compilerSide[2] ?? "").trim(), code: compilerSide[1] ?? "" });
            continue;
        }
        // the lines after an error belong to it
        const last = problems[problems.length - 1];
        if (!last) {
            continue;
        }
        const help = HELP.exec(line);
        const underline = UNDERLINE.exec(line);
        if (help) {
            last.help = help[1] ?? "";
        } else if (underline) {
            last.length = (underline[1] ?? "").length;
        } else if (!SNIPPET.test(line)) {
            last.message += `\n${line}`; // the editor shows the snippet itself
        }
    }
    return problems;
}
