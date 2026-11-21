// The words the language knows. Keep these lists in sync with the tables in
// ura-lang's src/main.c (keywords[], builtins[]) and with syntaxes/ura.tmLanguage.json.

export const CONTROL = [
    "if", "elif", "else", "while", "for", "in", "break", "continue", "return",
    "match", "case", "default", "try", "catch", "throw",
];
export const LOGICAL = ["and", "or", "not", "is", "as"];
export const DECLARATION = ["fn", "proto", "struct", "enum", "mod", "template", "operator", "pub", "from"];
export const IMPORT = ["use", "link"];
export const TYPES = [
    "bool", "i1", "char", "unic", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "void", "num",
];
export const CONSTANTS = ["True", "False", "null"];

// Built-ins written like calls. They are soft keywords: after `.` or `::`
// (`p.drop()`, `fn drop()`) the same word is a plain name.
export interface Builtin {
    name: string;
    params: string[];
    returns: string;
    doc: string;
}

export const BUILTINS: Builtin[] = [
    { name: "sizeof", params: ["x"], returns: "i32", doc: "The size in bytes of a type or a value." },
    { name: "typeof", params: ["x"], returns: "char[]", doc: "The type name of a value, as text." },
    { name: "drop", params: ["x"], returns: "void", doc: "Runs `operator drop` on `x` now instead of at the end of its block." },
    { name: "isZero", params: ["x"], returns: "bool", doc: "True when every byte of `x` is zero." },
    { name: "memory_alloc", params: ["bytes"], returns: "i8[]", doc: "Allocates `bytes` zeroed bytes on the heap." },
    { name: "memory_copy", params: ["dst", "src", "bytes"], returns: "void", doc: "Copies `bytes` bytes from `src` to `dst`." },
    { name: "memory_free", params: ["memory"], returns: "void", doc: "Frees memory from `memory_alloc`." },
];

// Words that can never be a variable or function name. `self` is not one:
// it is the implicit first parameter of a method.
export const RESERVED = new Set<string>([...CONTROL, ...LOGICAL, ...DECLARATION, ...IMPORT, ...TYPES, ...CONSTANTS]);

export function findBuiltin(name: string): Builtin | undefined {
    return BUILTINS.find((b) => b.name === name);
}

export function builtinSignature(builtin: Builtin): string {
    return `${builtin.name}(${builtin.params.join(", ")}) ${builtin.returns}`;
}
