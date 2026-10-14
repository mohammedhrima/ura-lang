// Turns what the ura compiler prints on stderr into problems.
// Running the compiler lives in services/compiler.ts.

export interface Problem {
    message: string;
    line?: number; // 0-based; undefined when the compiler gave no position
    character?: number; // 0-based
    code?: string; // where in the compiler the error came from, e.g. "main.c:827 analyze"
}

// eslint-disable-next-line no-control-regex -- ANSI color codes start with ESC (\x1b)
export const COLORS = /\x1b\[[0-9;]*m/g;
// the format the compiler should print once tokens know their position
const POSITIONED = /^(.*\.ura):(\d+):(\d+):\s*(.*)$/;
// what eprint prints today: "<compiler file>:<line> <function> <message>"
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
        // details printed on the lines after an error belong to it
        const last = problems[problems.length - 1];
        if (last) {
            last.message += `\n${line}`;
        }
    }
    return problems;
}
