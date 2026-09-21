// The words the language knows today. Keep these lists in sync with the
// tokenizer in src/main.c (its keywords[] and specials[] tables) and with
// syntaxes/ura.tmLanguage.json.

export const CONTROL = ["if", "elif", "else", "while", "break", "continue", "return"];
export const LOGICAL = ["and", "or"];
export const DECLARATION = ["fn", "proto"];
export const TYPES = ["i8", "char", "i32", "b1"];
export const BOOLEANS = ["True", "False"];

// Language built-ins that are written like calls. None today: references are
// written with `&`, which is an operator, not a built-in.
export interface Builtin {
    name: string;
    params: string[];
    returns: string;
    doc: string;
}

export const BUILTINS: Builtin[] = [];

// Words that can never be a variable or function name.
export const RESERVED = new Set<string>([
    ...CONTROL,
    ...LOGICAL,
    ...DECLARATION,
    ...TYPES,
    ...BOOLEANS,
    ...BUILTINS.map((b) => b.name),
]);

export function findBuiltin(name: string): Builtin | undefined {
    return BUILTINS.find((b) => b.name === name);
}

export function builtinSignature(builtin: Builtin): string {
    return `${builtin.name}(${builtin.params.join(", ")})`;
}
