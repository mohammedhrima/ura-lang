// The words the language knows today. Keep these lists in sync with the
// tokenizer in src/main.c (its keywords[] and specials[] tables) and with
// syntaxes/ura.tmLanguage.json.

export const CONTROL = ["if", "elif", "else", "while", "break", "continue", "return"];
export const LOGICAL = ["and", "or"];
export const DECLARATION = ["fn", "proto"];
export const TYPES = ["i8", "char", "i32", "b1"];
export const REF = "ref";
export const BOOLEANS = ["True", "False"];

// Language built-ins that are written like calls.
export interface Builtin {
    name: string;
    params: string[];
    returns: string;
    doc: string;
}

export const BUILTINS: Builtin[] = [
    {
        name: "own",
        params: ["variable"],
        returns: "ref(type of variable)",
        doc: "The address of a variable, to store in a `ref`.",
    },
    {
        name: "dref",
        params: ["ref"],
        returns: "the value the ref points to",
        doc: "Reads through a ref. `dref(p) = value` writes through it.",
    },
];

// Words that can never be a variable or function name.
export const RESERVED = new Set<string>([
    ...CONTROL,
    ...LOGICAL,
    ...DECLARATION,
    ...TYPES,
    REF,
    ...BOOLEANS,
    ...BUILTINS.map((b) => b.name),
]);

export function findBuiltin(name: string): Builtin | undefined {
    return BUILTINS.find((b) => b.name === name);
}

export function builtinSignature(builtin: Builtin): string {
    return `${builtin.name}(${builtin.params.join(", ")})`;
}
