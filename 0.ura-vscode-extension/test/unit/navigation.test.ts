// Structs, methods, member access, outline, references and rename checks.

import assert from "node:assert/strict";
import { describe, test } from "node:test";
import { importsOf, type Reader } from "../../server/src/core/imports";
import {
    analyze,
    functionSignature,
    memberContext,
    NO_IMPORTS,
    resolve,
    structSignature,
    type Resolved,
} from "../../server/src/core/parser";
import { isWrite, occurrences, renameProblem, type FileContext } from "../../server/src/core/references";
import { matchesQuery, outlineOf } from "../../server/src/core/symbols";

const SOURCE = [
    "struct Point:", //                                    0
    "    x i32", //                                        1
    "    y i32", //                                        2
    "", //                                                 3
    "struct Rect:", //                                     4
    "    a Point", //                                      5
    "    b Point", //                                      6
    "", //                                                 7
    "    fn width() i32:", //                              8
    "        return self.b.x - self.a.x", //               9
    "", //                                                10
    "fn area(r Rect) i32:", //                             11
    "    w i32 = r.width()", //                            12
    "    return w * r.b.y", //                             13
    "", //                                                 14
    "fn main() i32:", //                                   15
    "    r Rect", //                                       16
    "    r.b.x = 5", //                                    17
    "    x i32 = r.b.x", //                                18
    "    p &Rect = &r", //                                 19
    "    return area(r) + p.a.y", //                       20
    "",
].join("\n");

const analysis = analyze(SOURCE);
const context: FileContext = { file: "/p/main.ura", analysis, imports: NO_IMPORTS };

// what the name at (line, the n-th `needle` on it) resolves to
function at(line: number, needle: string, nth = 0): Resolved | undefined {
    const text = analysis.lines[line] ?? "";
    let character = -1;
    for (let i = 0; i <= nth; i++) {
        character = text.indexOf(needle, character + 1);
    }
    assert.ok(character >= 0, `'${needle}' not on line ${line}`);
    return resolve(analysis, { line, character }, NO_IMPORTS);
}

function describeFound(found: Resolved | undefined): string {
    if (!found) {
        return "nothing";
    }
    switch (found.kind) {
        case "attribute":
            return `attribute ${found.struct.name}.${found.attribute.name}`;
        case "method":
            return `method ${found.struct.name}.${found.function.name}`;
        case "struct":
            return `struct ${found.struct.name}`;
        case "function":
            return `function ${found.function.name}`;
        case "variable":
            return `variable ${found.variable.name} ${found.variable.type}`;
    }
}

describe("structs and methods", () => {
    test("attributes and methods belong to their struct", () => {
        const [point, rect] = analysis.structs;
        assert.deepEqual(point?.attributes.map((a) => `${a.name} ${a.type}`), ["x i32", "y i32"]);
        assert.deepEqual(rect?.attributes.map((a) => `${a.name} ${a.type}`), ["a Point", "b Point"]);
        assert.deepEqual(rect?.methods.map((m) => m.name), ["width"]);
        assert.equal(rect?.methods[0]?.owner, "Rect");
    });

    test("attributes are not variables; struct-typed declarations are", () => {
        const names = analysis.variables.map((v) => `${v.name} ${v.type}`);
        assert.ok(!names.includes("a Point"));
        assert.ok(names.includes("r Rect"));
        assert.ok(names.includes("p &Rect"));
    });

    test("a method has an implicit self, a ref to its struct", () => {
        assert.equal(describeFound(at(9, "self")), "variable self &Rect");
    });

    test("signatures", () => {
        const width = analysis.structs[1]?.methods[0];
        assert.ok(width);
        assert.equal(functionSignature(width), "fn Rect.width() i32");
        assert.equal(structSignature(analysis.structs[0] ?? assert.fail()), "struct Point:\n    x i32\n    y i32");
    });
});

describe("resolve: members and chains", () => {
    const cases: [number, string, number, string][] = [
        [17, "x", 0, "attribute Point.x"], // r.b.x
        [9, "x", 0, "attribute Point.x"], // self.b.x
        [9, "a", 0, "attribute Rect.a"], // self.a
        [12, "width", 0, "method Rect.width"], // r.width()
        [16, "Rect", 0, "struct Rect"], // a type
        [18, "x", 0, "variable x i32"], // the local x, not the attribute
        [18, "x", 1, "attribute Point.x"], // r.b.x on the same line
        [20, "y", 0, "attribute Point.y"], // p.a.y through a ref
        [20, "area", 0, "function area"],
        [1, "x", 0, "attribute Point.x"], // the declaration itself
        [4, "Rect", 0, "struct Rect"],
    ];
    for (const [line, needle, nth, expected] of cases) {
        test(`${(analysis.lines[line] ?? "").trim()} → ${needle} #${nth}`, () => {
            assert.equal(describeFound(at(line, needle, nth)), expected);
        });
    }

    test("an unknown member resolves to nothing", () => {
        const other = analyze("struct A:\n    v i32\n\nfn main() i32:\n    a A\n    return a.nope\n");
        assert.equal(resolve(other, { line: 5, character: 13 }, NO_IMPORTS), undefined);
    });

    test("member completion knows the struct after a dot", () => {
        assert.equal(memberContext(analysis, NO_IMPORTS, { line: 13, character: 19 })?.struct.name, "Point"); // r.b.
        assert.equal(memberContext(analysis, NO_IMPORTS, { line: 13, character: 17 })?.struct.name, "Rect"); // r.
        assert.equal(memberContext(analysis, NO_IMPORTS, { line: 13, character: 12 }), undefined); // w *
    });

    test("structs and methods from used files", () => {
        const read: Reader = (file) =>
            ({ "/p/geo.ura": "struct Vec:\n    x i32\n\n    fn len() i32:\n        return self.x\n" } as Record<string, string>)[file];
        const main = 'use "geo"\n\nfn main() i32:\n    v Vec\n    return v.len() + v.x\n';
        const imports = importsOf("/p/main.ura", main, read);
        const here = analyze(main);
        const len = resolve(here, { line: 4, character: 13 }, imports);
        assert.equal(len?.kind, "method");
        assert.equal(len?.kind === "method" ? len.file : undefined, "/p/geo.ura");
        assert.equal(resolve(here, { line: 3, character: 6 }, imports)?.kind, "struct");
    });
});

describe("outline", () => {
    test("structs, functions, their members and locals, in source order", () => {
        const outline = outlineOf(analysis);
        assert.deepEqual(outline.map((s) => `${s.kind} ${s.name}`), ["struct Point", "struct Rect", "function area", "function main"]);
        const rect = outline[1];
        assert.deepEqual(rect?.children.map((c) => `${c.kind} ${c.name}`), ["field a", "field b", "method width"]);
        assert.deepEqual(outline[3]?.children.map((c) => c.name), ["r", "x", "p"]);
        assert.deepEqual(rect?.range.end.line, 9);
    });

    test("workspace symbol queries match letters in order", () => {
        assert.ok(matchesQuery("Rect.width", "rw"));
        assert.ok(matchesQuery("area", ""));
        assert.ok(!matchesQuery("area", "xz"));
    });
});

describe("references and rename", () => {
    function spansOf(target: Resolved | undefined): string[] {
        assert.ok(target);
        return occurrences(context, target, context.file).map((s) => `${s.line}:${s.start}`);
    }

    test("an attribute: its declaration and every member access, not same-named variables", () => {
        assert.deepEqual(spansOf(at(1, "x")), ["1:4", "9:22", "9:33", "17:8", "18:16"]);
    });

    test("a variable: only in its own function", () => {
        assert.deepEqual(spansOf(at(16, "r")), ["16:4", "17:4", "18:12", "19:15", "20:16"]);
    });

    test("a method: its declaration and calls", () => {
        assert.deepEqual(spansOf(at(8, "width")), ["8:7", "12:14"]);
    });

    test("writes are the declaration and assignment targets", () => {
        const target = at(1, "x");
        assert.ok(target);
        const kinds = occurrences(context, target, context.file).map((s) => isWrite(context, s, target, context.file));
        assert.deepEqual(kinds, [true, false, false, true, false]);
    });

    test("references across files", () => {
        const read: Reader = (file) => ({ "/p/lib.ura": "fn help() i32:\n    return 7\n" } as Record<string, string>)[file];
        const main = 'use "lib"\n\nfn main() i32:\n    return help() + help()\n';
        const lib: FileContext = { file: "/p/lib.ura", analysis: analyze(read("/p/lib.ura") ?? ""), imports: NO_IMPORTS };
        const user: FileContext = { file: "/p/main.ura", analysis: analyze(main), imports: importsOf("/p/main.ura", main, read) };
        const target = resolve(lib.analysis, { line: 0, character: 3 }, NO_IMPORTS);
        assert.ok(target);
        assert.deepEqual(occurrences(lib, target, lib.file).map((s) => `${s.line}:${s.start}`), ["0:3"]);
        assert.deepEqual(occurrences(user, target, lib.file).map((s) => `${s.line}:${s.start}`), ["3:11", "3:20"]);
    });

    test("rename refuses invalid names, keywords, self and taken names", () => {
        const check = (found: Resolved | undefined, name: string): string | undefined => {
            assert.ok(found);
            return renameProblem(context, found, name);
        };
        assert.equal(check(at(1, "x"), "9x"), "'9x' is not a valid name");
        assert.equal(check(at(1, "x"), "if"), "'if' is a keyword");
        assert.equal(check(at(9, "self"), "me"), "'self' can't be renamed");
        assert.equal(check(at(1, "x"), "y"), "a attribute of Point named 'y' already exists");
        assert.equal(check(at(8, "width"), "a"), "a attribute of Rect named 'a' already exists");
        assert.equal(check(at(16, "r"), "p"), "a variable named 'p' already exists");
        assert.equal(check(at(16, "r"), "w"), undefined); // w lives in area, not main
        assert.equal(check(at(11, "area"), "main"), "a function named 'main' already exists");
        assert.equal(check(at(0, "Point"), "Rect"), "a struct named 'Rect' already exists");
        assert.equal(check(at(0, "Point"), "Vec2"), undefined);
    });
});
