// Tests for the analysis layer, run with Node's own test runner (no VS Code).
// The sweep test reads every program recorded in the compiler's src/tests/*.md;
// the other tests carry their own small programs.

import assert from "node:assert/strict";
import * as fs from "node:fs";
import * as path from "node:path";
import { describe, test } from "node:test";
import { lint, parseStderr } from "../analysis/compiler";
import {
    analyze,
    callContext,
    findFunction,
    functionSignature,
    isInComment,
    resolve,
    stripComment,
    visibleVariables,
} from "../analysis/parser";

const REPO = path.resolve(__dirname, "../../..");
const RECORDED = path.join(REPO, "src", "tests");
const COMPILER = path.join(REPO, "src", "build", "ura");

// every ```ura block of every src/tests/*.md, labelled "file.md #n"
function recordedPrograms(): { label: string; source: string }[] {
    const programs: { label: string; source: string }[] = [];
    for (const file of fs.readdirSync(RECORDED).filter((f) => f.endsWith(".md"))) {
        const text = fs.readFileSync(path.join(RECORDED, file), "utf8");
        const blocks = text.match(/^```ura\n[\s\S]*?^```$/gm) ?? [];
        blocks.forEach((block, i) => {
            programs.push({
                label: `${file} #${i + 1}`,
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
    drefRead: [
        "// read a value through a ref",
        "fn main() i32:",
        "    a i32 = 10",
        "    p ref(i32) = own(a)",
        "    b i32 = dref(p)",
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

function example(name: keyof typeof PROGRAMS): string {
    return PROGRAMS[name];
}

// line/character of the n-th occurrence of `needle` in `source`
function positionOf(source: string, needle: string, nth = 0): { line: number; character: number } {
    const lines = source.split("\n");
    let seen = 0;
    for (let line = 0; line < lines.length; line++) {
        let from = 0;
        let at: number;
        while ((at = lines[line].indexOf(needle, from)) >= 0) {
            if (seen++ === nth) {
                return { line, character: at };
            }
            from = at + 1;
        }
    }
    throw new Error(`'${needle}' not found`);
}

describe("functions", () => {
    test("every recorded test program parses and every fn line is found", () => {
        const programs = recordedPrograms();
        assert.ok(programs.length > 0, "no recorded programs found in src/tests");
        for (const { label, source } of programs) {
            const headers = source.split("\n").filter((l) => /^\s*fn\s/.test(l)).length;
            assert.equal(analyze(source).functions.length, headers, label);
        }
    });

    test("ref parameters and no return type", () => {
        const swap = findFunction(analyze(example("swap")), "swap");
        assert.ok(swap);
        assert.deepEqual(
            swap.params.map((p) => [p.name, p.type]),
            [
                ["x", "ref(i32)"],
                ["y", "ref(i32)"],
            ],
        );
        assert.equal(swap.returnType, "void");
        assert.equal(functionSignature(swap), "fn swap(x ref(i32), y ref(i32))");
    });

    test("return types", () => {
        const analysis = analyze(example("boolReturn"));
        assert.equal(findFunction(analysis, "is_even")?.returnType, "b1");
        assert.equal(findFunction(analysis, "main")?.returnType, "i32");
        const same = findFunction(analyze(example("refReturn")), "same");
        assert.equal(same?.returnType, "ref(i32)");
    });

    test("parameter positions point at the names", () => {
        const source = example("twoParams");
        const minus = findFunction(analyze(source), "minus");
        assert.ok(minus);
        const line = source.split("\n")[minus.line];
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
        const [a, b] = analyze(source).functions;
        assert.deepEqual([a.bodyStart, a.bodyEnd], [1, 3]);
        assert.deepEqual([b.bodyStart, b.bodyEnd], [6, 6]);
    });
});

describe("variables", () => {
    test("declarations visible before a use", () => {
        const source = example("drefRead");
        const at = positionOf(source, "return b");
        const names = visibleVariables(analyze(source), at).map((v) => `${v.name} ${v.type}`);
        assert.deepEqual(names.sort(), ["a i32", "b i32", "p ref(i32)"]);
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
        const [a] = visibleVariables(analyze(source), { line: 3, character: 4 });
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
        const source = example("drefRead");
        const analysis = analyze(source);
        assert.equal(resolve(analysis, positionOf(source, "dref(p)")), undefined);
        assert.equal(resolve(analysis, positionOf(source, "read a value")), undefined);
    });
});

describe("call context", () => {
    test("argument index", () => {
        assert.deepEqual(callContext("    return minus(10, 4)", 22), { name: "minus", argument: 1 });
        assert.deepEqual(callContext("    return minus(", 17), { name: "minus", argument: 0 });
    });

    test("innermost call wins", () => {
        const line = "    return inc(inc(";
        assert.deepEqual(callContext(line, line.length), { name: "inc", argument: 0 });
    });

    test("grouping parentheses and keywords are skipped", () => {
        assert.deepEqual(callContext("    f(a, (b", 11), { name: "f", argument: 1 });
        assert.equal(callContext("    if (a", 9), undefined);
        assert.deepEqual(callContext("    dref(", 9), { name: "dref", argument: 0 });
    });

    test("closed calls and comments give nothing", () => {
        assert.equal(callContext("    x = f(1)", 12), undefined);
        assert.equal(callContext("    // f(", 9), undefined);
        assert.equal(isInComment("    a = 1 // note", 16), true);
    });
});

describe("recent language additions", () => {
    test("and / or / proto are keywords, not names", () => {
        const source =
            "proto exit(code i32)\n\nfn main() i32:\n    a i32 = 1\n    if a > 0 and a < 3 or a == 9:\n        return 1\n    return 0\n";
        const analysis = analyze(source);
        for (const keyword of ["and ", "or ", "proto "]) {
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
        const [problem] = parseStderr(stderr);
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
        const source = "fn main() i32:\n    a i32 = 10\n    b i32 = dref(a)\n    return 0\n";
        const problems = await lint(COMPILER, source);
        assert.equal(problems.length, 1);
        assert.equal(problems[0].message, "dref() expects a ref variable");
    });

    test("a missing compiler is reported, not thrown", async () => {
        const problems = await lint("/nonexistent/ura", "fn main() i32:\n    return 0\n");
        assert.equal(problems.length, 1);
        assert.match(problems[0].message, /cannot run the ura compiler/);
    });
});
