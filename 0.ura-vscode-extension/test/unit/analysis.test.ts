// Unit tests for the server's core (and the compiler service), run with Node's
// own test runner: no VS Code, no language server.
// The sweep test reads every program recorded in the compiler's src/tests/*.md;
// the other tests carry their own small programs.

import assert from "node:assert/strict";
import * as fs from "node:fs";
import * as path from "node:path";
import { describe, test } from "node:test";
import * as os from "node:os";
import { parseStderr } from "../../server/src/core/compiler";
import { importedFunctions, importedStructs, resolveUse, walkUses, type Reader } from "../../server/src/core/imports";
import {
    analyze,
    callContext,
    findFunction,
    functionSignature,
    isInComment,
    resolve,
    spansOf,
    stripComment,
    visibleVariables,
} from "../../server/src/core/parser";
import { lint, stage } from "../../server/src/services/compiler";

// out/test/unit → the ura-lang repository
const REPO = path.resolve(__dirname, "../../../..");
// the compiler's recorded programs (src/tests, or src/tests-old once renamed)
const RECORDED = [path.join(REPO, "src", "tests"), path.join(REPO, "src", "tests-old")].filter((d) => fs.existsSync(d));
const COMPILER = path.join(REPO, "bin", "ura");

// every ```ura block of every src/tests/*.md, labelled "file.md #n"
function recordedPrograms(): { label: string; source: string }[] {
    const programs: { label: string; source: string }[] = [];
    for (const file of RECORDED.flatMap((dir) => fs.readdirSync(dir).filter((f) => f.endsWith(".md")).map((f) => path.join(dir, f)))) {
        const text = fs.readFileSync(file, "utf8");
        const blocks = text.match(/^```ura\n[\s\S]*?^```$/gm) ?? [];
        blocks.forEach((block, i) => {
            programs.push({
                label: `${path.relative(REPO, file)} #${i + 1}`,
                source: block.replace(/^```ura\n/, "").replace(/```$/, ""),
            });
        });
    }
    return programs;
}

const PROGRAMS: Record<string, string> = {
    swap: [
        "// swap two variables through ref parameters",
        "fn swap(x &i32, y &i32):",
        "    t i32 = x",
        "    x = y",
        "    y = t",
        "",
        "fn main() i32:",
        "    a i32 = 1",
        "    b i32 = 2",
        "    swap(&a, &b)",
        "    return a * 10 + b",
        "",
    ].join("\n"),
    boolReturn: [
        "fn is_even(n i32) b1:",
        "    return n % 2 == 0",
        "",
        "fn main() i32:",
        "    if is_even(4):",
        "        return 1",
        "    return 0",
        "",
    ].join("\n"),
    refReturn: [
        "fn same(p &i32) &i32:",
        "    return &p",
        "",
        "fn main() i32:",
        "    a i32 = 6",
        "    q &i32 = same(&a)",
        "    return q",
        "",
    ].join("\n"),
    twoParams: [
        "// two parameters, and their order matters",
        "fn minus(a i32, b i32) i32:",
        "    return a - b",
        "",
        "fn main() i32:",
        "    return minus(10, 4)",
        "",
    ].join("\n"),
    oneParam: [
        "fn twice(x i32) i32:",
        "    return x * 2",
        "",
        "fn main() i32:",
        "    return twice(21)",
        "",
    ].join("\n"),
    refRead: [
        "// read a value through a ref",
        "fn main() i32:",
        "    a i32 = 10",
        "    p &i32 = &a",
        "    b i32 = p",
        "    return b",
        "",
    ].join("\n"),
    forwardReference: [
        "fn main() i32:",
        "    return later()",
        "",
        "fn later() i32:",
        "    return 9",
        "",
    ].join("\n"),
};

// items[i], failing the test when it doesn't exist
function nth<T>(items: readonly T[], i: number): T {
    const item = items[i];
    assert.ok(item !== undefined, `no item ${i} in a list of ${items.length}`);
    return item;
}

function example(name: keyof typeof PROGRAMS): string {
    const program = PROGRAMS[name];
    assert.ok(program !== undefined, `no example program ${name}`);
    return program;
}

// line/character of the occurrence-th match of `needle` in `source`
function positionOf(source: string, needle: string, occurrence = 0): { line: number; character: number } {
    const lines = source.split("\n");
    let seen = 0;
    for (let line = 0; line < lines.length; line++) {
        let from = 0;
        let at: number;
        while ((at = nth(lines, line).indexOf(needle, from)) >= 0) {
            if (seen++ === occurrence) {
                return { line, character: at };
            }
            from = at + 1;
        }
    }
    throw new Error(`'${needle}' not found`);
}

describe("functions", () => {
    test("every recorded test program parses and every fn / proto line is found", () => {
        const programs = recordedPrograms();
        assert.ok(programs.length > 0, "no recorded programs found in src/tests or src/tests-old");
        for (const { label, source } of programs) {
            const headers = source.split("\n").filter((l) => /^\s*(fn|proto)\s+[A-Za-z_]/.test(l)).length; // named headers
            assert.equal(analyze(source).functions.length, headers, label);
        }
    });

    test("ref parameters and no return type", () => {
        const swap = findFunction(analyze(example("swap")), "swap");
        assert.ok(swap);
        assert.deepEqual(
            swap.params.map((p) => [p.name, p.type]),
            [
                ["x", "&i32"],
                ["y", "&i32"],
            ],
        );
        assert.equal(swap.returnType, "void");
        assert.equal(functionSignature(swap), "fn swap(x &i32, y &i32)");
    });

    test("return types", () => {
        const analysis = analyze(example("boolReturn"));
        assert.equal(findFunction(analysis, "is_even")?.returnType, "b1");
        assert.equal(findFunction(analysis, "main")?.returnType, "i32");
        const same = findFunction(analyze(example("refReturn")), "same");
        assert.equal(same?.returnType, "&i32");
    });

    test("parameter positions point at the names", () => {
        const source = example("twoParams");
        const minus = findFunction(analyze(source), "minus");
        assert.ok(minus);
        const line = nth(source.split("\n"), minus.line);
        for (const param of minus.params) {
            assert.equal(line.slice(param.character, param.character + param.name.length), param.name);
        }
    });

    test("proto declares a function with no body", () => {
        const source = 'proto exit(code i32)\n\nfn main() i32:\n    exit(1)\n    return 0\n';
        const analysis = analyze(source);
        const exit = findFunction(analysis, "exit");
        assert.ok(exit);
        assert.equal(exit.keyword, "proto");
        assert.equal(functionSignature(exit), "proto exit(code i32)");
        assert.ok(exit.bodyEnd < exit.bodyStart, "a proto has no body");
        assert.equal(findFunction(analysis, "main")?.keyword, "fn");
        // and it can still be jumped to from a call
        assert.equal(resolve(analysis, positionOf(source, "exit(1)"))?.kind, "function");
    });

    test("body range follows indentation", () => {
        const source = "fn a() i32:\n    x i32 = 1\n\n    return x\n\nfn b():\n    y i32 = 2\n";
        const functions = analyze(source).functions;
        const a = nth(functions, 0);
        const b = nth(functions, 1);
        assert.deepEqual([a.bodyStart, a.bodyEnd], [1, 3]);
        assert.deepEqual([b.bodyStart, b.bodyEnd], [6, 6]);
    });
});

describe("variables", () => {
    test("declarations visible before a use", () => {
        const source = example("refRead");
        const at = positionOf(source, "return b");
        const names = visibleVariables(analyze(source), at).map((v) => `${v.name} ${v.type}`);
        assert.deepEqual(names.sort(), ["a i32", "b i32", "p &i32"]);
    });

    test("parameters are visible in the body only", () => {
        const source = example("oneParam");
        const analysis = analyze(source);
        const inBody = positionOf(source, "return x");
        assert.deepEqual(visibleVariables(analysis, inBody).map((v) => v.name), ["x"]);
        const inMain = positionOf(source, "return twice");
        assert.deepEqual(visibleVariables(analysis, inMain), []);
    });

    test("a variable declared in a block ends with the block", () => {
        const source = "fn main() i32:\n    if True:\n        t i32 = 1\n        t = 2\n    return 0\n";
        const analysis = analyze(source);
        const inside = { line: 3, character: 8 };
        const after = { line: 4, character: 4 };
        assert.deepEqual(visibleVariables(analysis, inside).map((v) => v.name), ["t"]);
        assert.deepEqual(visibleVariables(analysis, after), []);
    });

    test("a later declaration of the same name wins", () => {
        const source = "fn main() i32:\n    a i32 = 1\n    a b1 = True\n    return 0\n";
        const a = nth(visibleVariables(analyze(source), { line: 3, character: 4 }), 0);
        assert.equal(a.type, "b1");
    });
});

describe("resolve", () => {
    test("a use of a parameter goes to the parameter", () => {
        const source = example("twoParams");
        const found = resolve(analyze(source), positionOf(source, "a - b"));
        assert.equal(found?.kind, "variable");
        if (found?.kind === "variable") {
            assert.equal(found.variable.isParam, true);
            assert.equal(found.variable.type, "i32");
        }
    });

    test("a call goes to the function", () => {
        const source = example("twoParams");
        const analysis = analyze(source);
        const found = resolve(analysis, positionOf(source, "minus(10"));
        assert.equal(found?.kind, "function");
        if (found?.kind === "function") {
            assert.equal(found.function.line, findFunction(analysis, "minus")?.line);
        }
    });

    test("a call to a function declared later still resolves", () => {
        const source = example("forwardReference");
        const found = resolve(analyze(source), positionOf(source, "later()"));
        assert.equal(found?.kind, "function");
    });

    test("keywords and comments resolve to nothing", () => {
        const source = example("refRead");
        const analysis = analyze(source);
        assert.equal(resolve(analysis, positionOf(source, "return b")), undefined);
        assert.equal(resolve(analysis, positionOf(source, "read a value")), undefined);
    });
});

describe("call context", () => {
    test("argument index", () => {
        assert.deepEqual(callContext("    return minus(10, 4)", 22), { name: "minus", argument: 1, start: 11 });
        assert.deepEqual(callContext("    return minus(", 17), { name: "minus", argument: 0, start: 11 });
    });

    test("innermost call wins", () => {
        const line = "    return inc(inc(";
        assert.deepEqual(callContext(line, line.length), { name: "inc", argument: 0, start: 15 });
    });

    test("grouping parentheses and keywords are skipped", () => {
        assert.deepEqual(callContext("    f(a, (b", 11), { name: "f", argument: 1, start: 4 });
        assert.equal(callContext("    if (a", 9), undefined);
        assert.deepEqual(callContext("    dref(", 9), { name: "dref", argument: 0, start: 4 });
    });

    test("closed calls and comments give nothing", () => {
        assert.equal(callContext("    x = f(1)", 12), undefined);
        assert.equal(callContext("    // f(", 9), undefined);
        assert.equal(isInComment("    a = 1 // note", 16), true);
    });
});

describe("recent language additions", () => {
    test("and / or / proto / use are keywords, not names", () => {
        const source =
            "use \"helper\"\nproto exit(code i32)\n\nfn main() i32:\n    a i32 = 1\n    if a > 0 and a < 3 or a == 9:\n        return 1\n    return 0\n";
        const analysis = analyze(source);
        for (const keyword of ["and ", "or ", "proto ", "use "]) {
            assert.equal(resolve(analysis, positionOf(source, keyword)), undefined, keyword.trim());
        }
        // the variable next to them still resolves
        assert.equal(resolve(analysis, positionOf(source, "a > 0"))?.kind, "variable");
        assert.equal(analysis.variables.map((v) => v.name).includes("and"), false);
    });

    test("compound assignment is not a declaration", () => {
        const source = "fn main() i32:\n    a i32 = 1\n    a += 2\n    a *= 3\n    return a\n";
        const names = analyze(source).variables.map((v) => `${v.name} ${v.type}`);
        assert.deepEqual(names, ["a i32"]);
    });

    test("// inside a string is not a comment", () => {
        const line = 'proto puts(s i8)   // note';
        assert.equal(isInComment(line, 20), true);
        assert.equal(isInComment('    x = "a // b"', 12), false);
        assert.equal(stripComment('    x = "a // b" // real'), '    x = "a // b" ');
    });
});

describe("compiler output", () => {
    test("today's format keeps the message and the compiler location", () => {
        const stderr = "\x1b[1m\x1b[0;31mmain.c:827 analyze\x1b[0m dref() expects a ref variable\n";
        assert.deepEqual(parseStderr(stderr), [
            { message: "dref() expects a ref variable", code: "main.c:827 analyze" },
        ]);
    });

    test("details on later lines belong to the error", () => {
        const stderr = "asm.c:47 asm_finalize module verification failed:\nStore operand must be a pointer.\n";
        const problem = nth(parseStderr(stderr), 0);
        assert.equal(problem.message, "module verification failed:\nStore operand must be a pointer.");
    });

    test("positioned format", () => {
        assert.deepEqual(parseStderr("/tmp/x/input.ura:3:5: variable 'b' not found\n"), [
            { message: "variable 'b' not found", line: 2, character: 4 },
        ]);
    });

    test("a sanitizer report is a crash", () => {
        const stderr = "==123==ERROR: AddressSanitizer: SEGV on unknown address 0x10\nstack...\n";
        assert.deepEqual(parseStderr(stderr), [
            { message: "compiler crashed: AddressSanitizer: SEGV on unknown address 0x10" },
        ]);
    });

    const noCompiler = !fs.existsSync(COMPILER);

    test("a valid program has no problems", { skip: noCompiler }, async () => {
        assert.deepEqual(await lint(COMPILER, example("swap")), []);
    });

    test("an invalid program reports the compiler's message", { skip: noCompiler }, async () => {
        const source = "fn main() i32:\n    return x\n";
        const problems = await lint(COMPILER, source);
        assert.equal(problems.length, 1);
        assert.equal(nth(problems, 0).message, "undeclared variable 'x'");
    });

    test("a missing compiler is reported, not thrown", async () => {
        const problems = await lint("/nonexistent/ura", "fn main() i32:\n    return 0\n");
        assert.equal(problems.length, 1);
        assert.match(nth(problems, 0).message, /cannot run the ura compiler/);
    });
});

describe("use", () => {
    // an in-memory project: absolute path -> source
    function project(files: Record<string, string>): Reader {
        return (file) => files[file];
    }

    test("use lines are found, with the range of their path", () => {
        const source = 'use "helper"\n\nfn main() i32:\n    use "sub/deep" // block use\n    return 0\n';
        const uses = analyze(source).uses;
        assert.deepEqual(
            uses.map((u) => [u.target, u.line, u.indent]),
            [["helper", 0, 0], ["sub/deep", 3, 4]],
        );
        assert.equal(nth(source.split("\n"), 0).slice(nth(uses, 0).start, nth(uses, 0).end), "helper");
        assert.equal(nth(source.split("\n"), 3).slice(nth(uses, 1).start, nth(uses, 1).end), "sub/deep");
    });

    test("a use path is relative to the file that writes it", () => {
        assert.equal(resolveUse("/p/app/main.ura", "../lib/m"), "/p/lib/m.ura");
        assert.equal(resolveUse("/p/app/main.ura", "sub/deep"), "/p/app/sub/deep.ura");
    });

    test("a diamond is read once and a cycle ends", () => {
        const read = project({
            "/p/d1.ura": 'use "base"\nuse "d2"\n',
            "/p/d2.ura": 'use "base"\nuse "main"\n',
            "/p/base.ura": "fn base() i32:\n    return 5\n",
        });
        const files = walkUses("/p/main.ura", 'use "d1"\n', read, () => true);
        assert.deepEqual(files.map((f) => f.file), ["/p/main.ura", "/p/d1.ura", "/p/base.ura", "/p/d2.ura"]);
    });

    test("imported functions are transitive, but not through a use inside a function", () => {
        const read = project({
            "/p/mid.ura": 'use "deep"\n\nfn total() i32:\n    use "private"\n    return deep() + 1\n',
            "/p/deep.ura": "fn deep() i32:\n    return 3\n",
            "/p/private.ura": "fn hidden() i32:\n    return 0\n",
        });
        const main = 'use "mid"\n\nfn main() i32:\n    return total() + deep()\n';
        const imported = importedFunctions("/p/main.ura", main, read);
        assert.deepEqual(imported.map((i) => [i.fn.name, i.file]), [["total", "/p/mid.ura"], ["deep", "/p/deep.ura"]]);

        const found = resolve(analyze(main), positionOf(main, "deep()"), { functions: imported, structs: [] });
        assert.equal(found?.kind, "function");
        assert.equal(found?.kind === "function" && found.file, "/p/deep.ura");
        assert.equal(findFunction(analyze(main), "total", imported)?.line, 2);
    });

    test("a missing file is skipped: the compiler reports it", () => {
        const files = walkUses("/p/main.ura", 'use "nope"\n', project({}), () => true);
        assert.deepEqual(files.map((f) => f.file), ["/p/main.ura"]);
    });

    test("lint copies used files with their layout, even above the document", () => {
        const dir = fs.mkdtempSync(path.join(os.tmpdir(), "ura-stage-test-"));
        try {
            const read = project({ "/p/lib/m.ura": "fn m() i32:\n    return 1\n" });
            const input = stage(dir, "/p/app/main.ura", 'use "../lib/m"\n', read);
            assert.equal(input, path.join(dir, "app", "main.ura"));
            assert.equal(fs.readFileSync(input, "utf8"), 'use "../lib/m"\n');
            assert.equal(fs.readFileSync(path.join(dir, "lib", "m.ura"), "utf8"), "fn m() i32:\n    return 1\n");
        } finally {
            fs.rmSync(dir, { recursive: true, force: true });
        }
    });
});

describe("structs", () => {
    const source = [
        "struct user:", // lower case on purpose: any spelling is a struct name
        "    name i8[]",
        "",
        "struct Point:",
        "    x i32",
        "",
        "fn make(p &Point, all user[]) Point: // user and Point here too",
        '    u user = "user is in a string"',
        "    return p",
        "",
    ].join("\n");

    test("struct declarations are found, whatever their case", () => {
        const structs = analyze(source).structs;
        assert.deepEqual(structs.map((s) => [s.name, s.line, s.character]), [["user", 0, 7], ["Point", 3, 7]]);
    });

    test("struct names are found in code, not in strings or comments", () => {
        const analysis = analyze(source);
        const spans = spansOf(analysis.lines, new Set(["user", "Point"]));
        const found = spans.map((s) => `${s.line}:${nth(analysis.lines, s.line).slice(s.start, s.start + s.length)}@${s.start}`);
        assert.deepEqual(found, ["0:user@7", "3:Point@7", "6:Point@11", "6:user@22", "6:Point@30", "7:user@6"]);
    });

    test("struct names from used files count too", () => {
        const read: Reader = (file) => ({ "/p/types.ura": "struct User:\n    a i32\n" } as Record<string, string>)[file];
        assert.deepEqual(importedStructs("/p/main.ura", 'use "types"\n', read).map((i) => [i.file, i.struct.name]), [["/p/types.ura", "User"]]);
    });
});

describe("comments and quotes", () => {
    test("// inside a string is not a comment, escaped quotes included", () => {
        assert.equal(stripComment('printf("\\" // not a comment") // comment'), 'printf("\\" // not a comment") ');
        assert.equal(stripComment('printf("it\'s // text") // c'), 'printf("it\'s // text") ');
        assert.equal(stripComment("c char = '\\'' // c"), "c char = '\\'' ");
        assert.equal(stripComment('s i8[] = "a\\\\" // c'), 's i8[] = "a\\\\" ');
    });
});
