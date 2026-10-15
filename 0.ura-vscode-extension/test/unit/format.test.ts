// The formatter: each rule, idempotence over every recorded program, and the
// compiler's output unchanged by formatting.

import assert from "node:assert/strict";
import { spawnSync } from "node:child_process";
import * as fs from "node:fs";
import * as os from "node:os";
import * as path from "node:path";
import { describe, test } from "node:test";
import { formatDocument, formatLine, levelOf } from "../../server/src/core/format";

// out/test/unit → the ura-lang repository
const REPO = path.resolve(__dirname, "../../../..");
const COMPILER = path.join(REPO, "bin", "ura");
const RECORDED = ["src/tests", "src/tests-old", "1.old-code/tests"].map((dir) => path.join(REPO, dir));

// every ```ura block of every recorded *.md, labelled "file.md #n"
function recordedPrograms(): { label: string; source: string }[] {
    const programs: { label: string; source: string }[] = [];
    for (const dir of RECORDED.filter((d) => fs.existsSync(d))) {
        for (const file of fs.readdirSync(dir).filter((f) => f.endsWith(".md"))) {
            const text = fs.readFileSync(path.join(dir, file), "utf8");
            const blocks = text.match(/^```ura\n[\s\S]*?^```$/gm) ?? [];
            blocks.forEach((block, i) => {
                programs.push({
                    label: `${path.basename(dir)}/${file} #${i + 1}`,
                    source: block.replace(/^```ura\n/, "").replace(/```$/, ""),
                });
            });
        }
    }
    return programs;
}

describe("formatter: inside a line", () => {
    const cases: [string, string][] = [
        ["a=b+c*2", "a = b + c * 2"],
        ["x i32=-10", "x i32 = -10"],
        ["return -5+12", "return -5 + 12"],
        ["return x-1", "return x - 1"],
        ["f(-1 , -x)", "f(-1, -x)"],
        ["p &i32=&a", "p &i32 = &a"],
        ["swap( &a , &b )", "swap(&a, &b)"],
        ["twice ( 3 )", "twice(3)"],
        ["a [ 0 ] = 9", "a[0] = 9"],
        ['s i8 [ ] = "abc"', 's i8[] = "abc"'],
        ["proto printf(fmt i8[],...) i32", "proto printf(fmt i8[], ...) i32"],
        ["return(2+3)*4", "return (2 + 3) * 4"],
        ["fn main()i32 :", "fn main() i32:"],
        ["if a>3:n+=1", "if a > 3: n += 1"],
        ["if a>1 and a<10:", "if a > 1 and a < 10:"],
        ["return-x", "return -x"],
        ["u . a = r.b .x", "u.a = r.b.x"],
        ["a i32[]=[1,2 ,3]", "a i32[] = [1, 2, 3]"],
        ["c char='\\''", "c char = '\\''"],
        ['printf("a  ,b",1)//  keep  this', 'printf("a  ,b", 1) //  keep  this'],
        ["x = 1 /* note */ + 2", "x = 1 /* note */ + 2"],
        ["a ; b", "a ; b"], // unknown character: spacing kept
    ];
    for (const [input, expected] of cases) {
        test(JSON.stringify(input), () => {
            assert.equal(formatLine(input), expected);
        });
    }
});

describe("formatter: indentation keeps the compiler's levels", () => {
    test("a level is 4 characters, rounded up; a tab counts as 1", () => {
        assert.deepEqual(["x", "  x", "    x", "     x", "\tx"].map(levelOf), [0, 1, 1, 2, 1]);
    });

    test("levels are written as 4 spaces each", () => {
        assert.equal(formatLine("  x i32"), "    x i32");
        assert.equal(formatLine("\treturn 0"), "    return 0");
        assert.equal(formatLine("     y = 1"), "        y = 1");
    });
});

describe("formatter: blank lines", () => {
    function format(lines: string[]): string[] {
        return formatDocument(lines.join("\n")).split("\n").slice(0, -1);
    }

    test("one blank line between top-level functions, none at the start or end", () => {
        assert.deepEqual(format(["", "fn a():", "    return", "fn b():", "    return", "", ""]), [
            "fn a():",
            "    return",
            "",
            "fn b():",
            "    return",
        ]);
    });

    test("runs of blank lines collapse to one", () => {
        assert.deepEqual(format(["fn a():", "    x i32 = 1", "", "", "", "    return x"]), [
            "fn a():",
            "    x i32 = 1",
            "",
            "    return x",
        ]);
    });

    test("consecutive use and proto lines stay together, other groups are separated", () => {
        assert.deepEqual(format(['use "a"', 'use "b"', "proto exit(code i32)", "fn main() i32:", "    return 0"]), [
            'use "a"',
            'use "b"',
            "",
            "proto exit(code i32)",
            "",
            "fn main() i32:",
            "    return 0",
        ]);
    });

    test("a comment right above a function stays with it", () => {
        assert.deepEqual(format(["fn a():", "    return", "// b does nothing", "fn b():", "    return"]), [
            "fn a():",
            "    return",
            "",
            "// b does nothing",
            "fn b():",
            "    return",
        ]);
    });

    test("a struct after a function gets its blank line", () => {
        assert.deepEqual(format(["fn a():", "    return", "struct User:", "    a i32"]), [
            "fn a():",
            "    return",
            "",
            "struct User:",
            "    a i32",
        ]);
    });

    test("multi-line block comments are kept exactly", () => {
        const source = "/*  keep\n  this   as is\n*/\nfn main() i32:\n    return 0\n";
        assert.equal(formatDocument(source), source);
    });

    test("CRLF line endings are kept", () => {
        assert.equal(formatDocument("fn main() i32:\r\n  return 0\r\n"), "fn main() i32:\r\n    return 0\r\n");
    });

    test("an empty document stays empty", () => {
        assert.equal(formatDocument(""), "");
    });
});

describe("formatter: on every recorded program", () => {
    const programs = recordedPrograms();

    test("there are recorded programs", () => {
        assert.ok(programs.length > 0);
    });

    test("formatting twice changes nothing more", () => {
        for (const { label, source } of programs) {
            const once = formatDocument(source);
            assert.equal(formatDocument(once), once, label);
        }
    });

    // the IR the compiler writes for `source`, or undefined when it doesn't compile
    function compile(source: string): string | undefined {
        const dir = fs.mkdtempSync(path.join(os.tmpdir(), "ura-format-"));
        try {
            fs.writeFileSync(path.join(dir, "input.ura"), source);
            const run = spawnSync(COMPILER, ["input.ura", "-o", "exe"], { cwd: dir, timeout: 10_000, stdio: "ignore" });
            const ir = path.join(dir, "build", "out.ll");
            return run.status === 0 && fs.existsSync(ir) ? fs.readFileSync(ir, "utf8") : undefined;
        } finally {
            fs.rmSync(dir, { recursive: true, force: true });
        }
    }

    test("the compiler produces the same IR before and after formatting", { skip: !fs.existsSync(COMPILER) }, () => {
        let compared = 0;
        for (const { label, source } of programs) {
            const before = compile(source);
            if (before === undefined) {
                continue; // the current compiler doesn't support this program yet
            }
            assert.equal(compile(formatDocument(source)), before, label);
            compared++;
        }
        assert.ok(compared > 0, "no recorded program compiled, nothing was compared");
    });
});
