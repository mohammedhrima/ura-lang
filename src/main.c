// TYPEDEFS
typedef struct Token Token;
typedef struct Node Node;
typedef struct NodePrint NodePrint;
typedef struct Keyword Keyword;
typedef struct Config Config;

#define print(...)  (DEBUG ? _print(stdout, __VA_ARGS__) : 0)
#define help(...)   _print(stdout, __VA_ARGS__)
#define eprint(...) _print(stderr, __VA_ARGS__)

#ifndef TOKENIZE
#    define TOKENIZE 1
#endif

#if TOKENIZE
#    define AST 1
#endif

#if AST
#    define ANALYZE 1
#endif

#if ANALYZE
#    define ASM 1
#endif

#include "./ucg.c"

#define URA_VERSION "0.1.0"

int _fprint(ioFile fp, const char *fmt, va_list ap);
#undef vfprint
#define vfprint _fprint

struct Token {
    Type type;
    char *name;
    int space;

    int line;
    int s;
    int e;

    bool is_type;
    bool is_varg;
    bool is_pub;
    bool is_uint;
    bool is_used;
    // uraIR ir;

    Ucg *ucg;
    int shift;
    union {
        struct {
            int index;
        } attr;
        struct {
            long long value;
            double real;
        } num;
        struct {
            char *value;
        } chars;
        struct {
            int start;
            int end;
            Node *types;
            Node *parents;
        } body;
    };
};

struct Node {
    Token *token;
    union {
        Node *left;
        Node *var;
        Node *data_type;
        Node *ret_type;
        Node *cond;
        Node *to_call;
        Node *loop;
        Node *operand;
        Node *scope;
        Node *symbol;
    };
    union {
        Node *right;
        Node *args;
        Node *next;
        Node *declared;
        Node *to_type;
        Node *index;
        Node *to_copy;
    };

    expand(Node *, children);
    expand(Ucg *, temporaries);
    expand(Node *, temporary_types);
};

struct NodePrint {
    expand(int, depths);
    expand(Node *, nodes);
};

struct Keyword {
    char *value;
    Token token;
};

struct Config {
    char *dir;
    Node *tree;
};

// Global Ura
struct {
    expand(File *, files);
    expand(File *, uses);
    expand(Config *, configs);
    File *file;
    int line;

    expand(Token *, tokens);

    Node *tree;

    expand(Node *, scopes);
    Node *scope;
    Node *types;
    expand(char *, modules);

    int exe_pos;
    bool throws;
} ura;

Keyword keywords[] = { // clang-format off
    { "bool",     { .type = INT,   .is_type = true, .shift = 1 } },
    { "i1",       { .type = INT,   .is_type = true, .shift = 1 } },
    { "void",     { .type = VOID,  .is_type = true } },
    { "f32",      { .type = FLOAT, .is_type = true, .shift = 32 } },
    { "f64",      { .type = FLOAT, .is_type = true, .shift = 64 } },
    { "char",     { .type = INT,   .is_type = true, .shift = 8 } },
    { "unic",     { .type = INT,   .is_type = true, .shift = 21 } },
    { "i8",       { .type = INT,   .is_type = true, .shift = 8 } },
    { "i16",      { .type = INT,   .is_type = true, .shift = 16 } },
    { "i32",      { .type = INT,   .is_type = true, .shift = 32 } },
    { "i64",      { .type = INT,   .is_type = true, .shift = 64 } },
    { "u8",       { .type = INT,   .is_type = true, .shift = 8,  .is_uint = true } },
    { "u16",      { .type = INT,   .is_type = true, .shift = 16, .is_uint = true } },
    { "u32",      { .type = INT,   .is_type = true, .shift = 32, .is_uint = true } },
    { "u64",      { .type = INT,   .is_type = true, .shift = 64, .is_uint = true } },
    { "num",      { .type = INT,   .is_type = true } },

    { "True",     { .type = INT,   .shift = 1, .num = { .value = true } } },
    { "False",    { .type = INT,   .shift = 1, .num = { .value = false } } },

    { "struct",   { .type = STRUCT } },
    { "enum",     { .type = ENUM } },
    { "and",      { .type = AND_LIT } },
    { "or",       { .type = OR_LIT } },
    { "not",      { .type = NOT } },
    { "is",       { .type = EQ } },
    { "proto",    { .type = PROTO } },
    { "link",     { .type = LINK } },
    { "use",      { .type = USE } },
    { "@if",      { .type = MACRO_IF } },
    { "@elif",    { .type = MACRO_ELIF } },
    { "@else",    { .type = MACRO_ELSE } },
    { "@error",   { .type = MACRO_ERROR } },
    { "@name",    { .type = MACRO_NAME } },
    { "@version", { .type = MACRO_VERSION } },
    { "@main",    { .type = MACRO_MAIN } },
    { "@script",  { .type = MACRO_SCRIPT } },
    { "@dependency", { .type = MACRO_DEPENDENCY } },
    { "operator", { .type = OPERATOR } },
    { "pub",      { .type = PUB } },
    { "fn",       { .type = FN_DEC } },
    { "return",   { .type = RETURN } },
    { "if",       { .type = IF } },
    { "elif",     { .type = ELIF } },
    { "else",     { .type = ELSE } },
    { "while",    { .type = LOOP } },
    { "for",      { .type = FOR } },
    { "in",       { .type = IN } },
    { "mod",      { .type = MODULE } },
    { "break",    { .type = BRK } },
    { "continue", { .type = CNT } },
    { "match",    { .type = MATCH } },
    { "case",     { .type = CASE } },
    { "default",  { .type = DEFAULT } },
    { "try",      { .type = TRY } },
    { "catch",    { .type = CATCH } },
    { "throw",    { .type = THROW } },
    { "template", { .type = TEMPLATE } },
    { "null",     { .type = ZERO } },
    { "as",       { .type = AS } },
    { "from",     { .type = FROM } },
    { "self",     { .type = SELF, .name = "self" } },
    { NULL },
};

Keyword builtins[] = {
    { "output",        { .type = WRITE, .num = { .value = 1 } } },
    { "errput",        { .type = WRITE, .num = { .value = 2 } } },
    { "input",         { .type = READ } },
    { "sizeof",        { .type = SIZEOF } },
    { "typeof",        { .type = TYPEOF } },
    { "drop",          { .type = DROP } },
    { "isZero",        { .type = IS_ZERO } },
    { "memory_alloc",  { .type = ALLOC } },
    { "memory_copy",   { .type = MEMCPY } },
    { "memory_free",   { .type = FREE } },
    { "ura::argc",     { .type = ARGC } },
    { "ura::argv",     { .type = ARGV } },
    { "ura::line",     { .type = LINE_NUMBER } },
    { "ura::filename", { .type = FILE_NAME } },
    { "ura::function", { .type = FN_NAME } },
    { NULL },
};

Keyword specials[] = {
    { "(",   { .type = LPARENT } },       { ")",   { .type = RPARENT } },
    { "::",  { .type = DOUBLE_DOTS } },   { ":",   { .type = DOTS } },
    { "[",   { .type = LBRACK } },        { "]",   { .type = RBRACK } },
    { "{",   { .type = LCBRACK } },       { "}",   { .type = RCBRACK } },

    { "+=",  { .type = ADD_ASSIGN } },    { "-=",  { .type = SUB_ASSIGN } },
    { "*=",  { .type = MUL_ASSIGN } },    { "/=",  { .type = DIV_ASSIGN } },
    { "%=",  { .type = MOD_ASSIGN } },
    { "<<=", { .type = LSHIFT_ASSIGN } }, { ">>=", { .type = RSHIFT_ASSIGN } },
    { "&=",  { .type = BAND_ASSIGN } },   { "|=",  { .type = BOR_ASSIGN } },
    { "^=",  { .type = BXOR_ASSIGN } },
    { "<<",  { .type = LSHIFT } },        { ">>",  { .type = RSHIFT } },

    { "...", { .type = VARIADIC } },      { ".",   { .type = DOT } },

    { "+",   { .type = ADD } },           { "-",   { .type = SUB } },
    { "*",   { .type = MUL } },           { "/",   { .type = DIV } },
    { "%",   { .type = MOD } },

    { ">=",  { .type = GE } },            { "<=",  { .type = LE } },
    { ">",   { .type = GT } },            { "<",   { .type = LT } },
    { "==",  { .type = EQ } },            { "!=",  { .type = NQ } },
    { "!",   { .type = NOT } },           { "~",   { .type = BNOT } },

    { "=",   { .type = ASSIGN } },        { ",",   { .type = COMA } },

    { "&&",  { .type = AND } },           { "||",  { .type = OR } },
    { "&",   { .type = REF } },           { "|",   { .type = BOR } },
    { "^",   { .type = BXOR } },

    { NULL },
}; // clang-format on

char *binary_ops[LAST + 1] = {
    // clang-format off
    [ADD] = "+",          [SUB] = "-",          [MUL] = "*",          [DIV] = "/",
    [MOD] = "%",

    [ASSIGN] = "=",       [ADD_ASSIGN] = "+=",  [SUB_ASSIGN] = "-=",  [MUL_ASSIGN] = "*=",
    [DIV_ASSIGN] = "/=",  [MOD_ASSIGN] = "%=",

    [BAND] = "&",         [BOR] = "|",          [BXOR] = "^",
    [LSHIFT] = "<<",      [RSHIFT] = ">>",

    [BAND_ASSIGN] = "&=", [BOR_ASSIGN] = "|=", [BXOR_ASSIGN] = "^=",
    [LSHIFT_ASSIGN] = "<<=", [RSHIFT_ASSIGN] = ">>=",

    [EQ] = "==",          [NQ] = "!=",
    [LT] = "<",           [GT] = ">",           [LE] = "<=",           [GE] = ">=",

    [AND] = "&&",         [OR] = "||",
    [AND_LIT] = "and",    [OR_LIT] = "or",

    [NEG] = "-",          [BNOT] = "~",
}; // clang-format on

int precedence[LAST + 1] = {
    // clang-format off
    [ASSIGN] = 1,      [ADD_ASSIGN] = 1,    [SUB_ASSIGN] = 1,    [MUL_ASSIGN] = 1,
    [DIV_ASSIGN] = 1,  [MOD_ASSIGN] = 1,    [BAND_ASSIGN] = 1,   [BOR_ASSIGN] = 1,
    [BXOR_ASSIGN] = 1, [LSHIFT_ASSIGN] = 1, [RSHIFT_ASSIGN] = 1,

    [OR] = 11,         [AND] = 12,

    [EQ] = 21,         [NQ] = 21,           [GT] = 22,           [LT] = 22,
    [GE] = 22,         [LE] = 22,

    [BOR] = 23,        [BXOR] = 24,         [BAND] = 25,         [LSHIFT] = 28,
    [RSHIFT] = 28,

    [ADD] = 30,        [SUB] = 30,          [MUL] = 31,          [DIV] = 31,
    [MOD] = 31,

    [AS] = 35,

    [DOT] = 51,        [DOUBLE_DOTS] = 51,  [LBRACK] = 51,
}; // clang-format on

char *escaped[256] = { // clang-format off
    ['n'] = "\n",   ['t'] = "\t",   ['r'] = "\r",   ['b'] = "\b",
    ['f'] = "\f",   ['v'] = "\v",   ['a'] = "\a",   ['?'] = "\?",
    ['0'] = "\0",   ['"'] = "\"",   ['\''] = "\'",  ['\\'] = "\\",

    ['\n'] = "\\n", ['\t'] = "\\t", ['\r'] = "\\r", ['\0'] = "\\0",
}; // clang-format on

#include "./check.c"
#include "./project.c"

// TOKENIZE
Token *new_token(Type type, Token *from) {
    Token *new = arena_alloc(1, sizeof(Token));
    new->type = type;
    if (from != NULL) {
        new->space = from->space;
        new->line = from->line;
        new->s = from->s;
        new->e = from->e;
    }
    append(ura.tokens, new);
    return new;
}

File *file_at(int pos) {
    File *found = ura.files[0];
    for (int i = 0; i < ura.files_count; i++) {
        File *file = ura.files[i];
        if (!file->opened || file->start > pos || file->start < found->start)
            continue;
        found = file;
    }
    return found;
}

char *text_at(int pos) {
    File *file = file_at(pos);
    return file->buff + pos - file->start;
}

int end_of_sources(void) {
    int end = 0;
    for (int i = 0; i < ura.files_count; i++) {
        File *file = ura.files[i];
        if (file->opened && file->start + file->len + 1 > end)
            end = file->start + file->len + 1;
    }
    return end;
}

int line_start_in(char *buff, int pos) {
    while (pos > 0 && buff[pos - 1] != '\n')
        pos--;
    while (buff[pos] == ' ')
        pos++;
    return pos;
}

int line_end_in(char *buff, int pos) {
    while (buff[pos] && buff[pos] != '\n')
        pos++;
    return pos;
}

Token *find_builtin(char *name) {
    for (int i = 0; builtins[i].value; i++) {
        if (strcmp2(builtins[i].value, name))
            return &builtins[i].token;
    }
    return NULL;
}

Token *source_value(Token *token) {
    Node *fn = NULL;
    for (int i = 0; i < ura.scopes_count; i++) {
        if (ura.scopes[i]->token->type != FN_DEC)
            continue;
        fn = ura.scopes[i];
    }
    switch (token->type) { // clang-format off
    case LINE_NUMBER: token->num.value = line_at(token->s); break;
    case FILE_NAME:   token->chars.value = relative_path(file_at(token->s)->path); break;
    case FN_NAME:     token->chars.value = fn ? fn->token->name : ""; break;
    default:          break; // clang-format on
    }
    bool is_line = token->type == LINE_NUMBER;
    token->type = is_line ? INT : CHARS;
    token->shift = is_line ? 32 : 0;
    return token;
}

Token *parse_id(int s, int e, int space) {
    Token *new = new_token(ID, NULL);
    char *buff = ura.file->buff;

    int i = 0;
    for (; keywords[i].value; i++) {
        char *value = keywords[i].value;
        size_t len = strlen(value);
        if (e - s == len && strncmp2(buff + s, value, e - s)) {
            *new = keywords[i].token;
            break;
        }
    }
    if (keywords[i].value == NULL)
        new->name = strndup2(buff + s, e - s);

    new->line = ura.line;
    new->s = s;
    new->e = e;
    new->space = space / TAB + ((space % TAB) != 0);
    print("new token %t\n", new->type);
    return new;
}

Token *parse_num(int s, int e, int space) {
    Token *new = new_token(INT, NULL);
    char *buff = ura.file->buff;

    new->num.value = strtoll(buff + s, NULL, 10);
    new->shift = new->num.value > 2147483647 ? 64 : 32;
    if (memchr(buff + s, '.', e - s)) {
        new->type = FLOAT;
        new->num.real = strtod(buff + s, NULL);
        new->shift = buff[e - 1] == 'f' ? 32 : 64;
    }
    new->line = ura.line;
    new->s = s;
    new->e = e;
    new->space = space / TAB + ((space % TAB) != 0);
    print("new token %t\n", new->type);
    return new;
}

Token *parse_string(int s, int e, int space) {
    Token *new = new_token(CHARS, NULL);
    char *buff = ura.file->buff;

    new->chars.value = parse_escaped(buff, s + 1, e - 1);
    new->line = ura.line;
    new->s = s;
    new->e = e;
    new->space = space / TAB + ((space % TAB) != 0);
    print("new token %t\n", new->type);
    return new;
}

Token *parse_char(int s, int e, int space) {
    Token *new = new_token(INT, NULL);
    new->shift = 8;
    char *buff = ura.file->buff;

    // UTF-8: a character is 1 to 4 bytes, read like a number in base 64
    // the first byte says how many bytes follow, minus its marker it is the first digit
    //   0xC0 = 192 = 110xxxxx -> 2 bytes (é)
    //   0xE0 = 224 = 1110xxxx -> 3 bytes (€)
    //   0xF0 = 240 = 11110xxx -> 4 bytes (🦀)
    //   below 0x80 = 128      -> 1 byte, plain ascii char ('a')
    // every next byte is 0x80 (128) + a digit from 0 to 63
    // example 'é' = C3 A9: (0xC3 - 0xC0) * 64 + (0xA9 - 0x80) = 3 * 64 + 41 = 233
    unsigned char *str = (unsigned char *)parse_escaped(buff, s + 1, e - 1);
    int count = 1;
    int value = str[0];
    if (str[0] >= 0xF0) {
        count = 4;
        value = str[0] - 0xF0;
    } else if (str[0] >= 0xE0) {
        count = 3;
        value = str[0] - 0xE0;
    } else if (str[0] >= 0xC0) {
        count = 2;
        value = str[0] - 0xC0;
    }
    for (int i = 1; i < count; i++)
        value = value * 64 + (str[i] - 0x80);
    new->num.value = value;
    // more than 1 byte does not fit a char, it is a unic
    if (count > 1)
        new->shift = 21;

    new->line = ura.line;
    new->s = s;
    new->e = e;
    new->space = space / TAB + ((space % TAB) != 0);
    print("new token %t\n", new->type);
    return new;
}

void tokenize(File *file) {
    ura.file = file;
    int first_token = ura.tokens_count;
    int s = 0;
    int e = 0;
    int space = 0;
    char *buff = file->buff;
    while (buff[e] != '\0') {
        s = e;
        if (isspace(buff[e])) {
            while (buff[e] == '\n') {
                ura.line++;
                e++;
            }
            if (buff[s] == '\n') {
                space = 0;
                s = e - 1;
            }
            while (buff[e] != '\n' && isspace(buff[e])) {
                // calculated spaces after hitting \n in s position
                if (buff[s] == '\n')
                    space++;
                e++;
            }
            //  if (buff[s] == '\n' && buff[e] && buff[e] != '\n') // a line with code
            //     check_indent(s + 1, e, ura.scope->token->line, space);
            continue;
        }
        while (strncmp2(buff + s, "//", 2) && buff[e] && buff[e] != '\n')
            e++;
        if (e != s)
            continue;
        if (strncmp2(buff + s, "/*", 2)) {
            e = s + 2; // skip /*
            while (buff[e] != '\0' && !strncmp2(buff + e, "*/", 2)) {
                if (buff[e] == '\n')
                    ura.line++;
                e++;
            }
            check_lexeme(buff, s, e);
            if (buff[e] == '\0')
                break;
            e += 2;
            continue;
        }

        if (buff[s] != '\0' && strchr2("\"\'", buff[s])) {
            char c = buff[s];
            e++;
            while (buff[e] != '\0' && buff[e] != '\n' && buff[e] != c) {
                char next = buff[e + 1];
                bool is_escape =
                    buff[e] == '\\' && next != '\0' && !strchr2("\n", next);
                e += is_escape ? 2 : 1;
            }
            if (buff[e] == c)
                e++; // skip last ' or "
            if (!check_lexeme(buff, s, e))
                continue;
            if (c == '"')
                parse_string(s, e, space);
            else
                parse_char(s, e, space);
            continue;
        }
        while (isdigit(buff[e]))
            e++;
        if (e != s && buff[e] == '.' && isdigit(buff[e + 1])) {
            e++; // skip .
            while (isdigit(buff[e]))
                e++;
            bool ends_name = !isalnum(buff[e + 1]) && buff[e + 1] != '_';
            if (buff[e] == 'f' && ends_name)
                e++;
        }
        if (e != s) {
            if (check_lexeme(buff, s, e))
                parse_num(s, e, space);
            continue;
        }
        if (buff[s] == '@' && isalpha(buff[s + 1]))
            e++; // SKIP @
        bool starts_name = isalpha(buff[s]) || buff[s] == '_' || buff[s] == '@';
        while (starts_name && (isalnum(buff[e]) || buff[e] == '_'))
            e++;
        if (e != s) {
            parse_id(s, e, space);
            continue;
        }
        for (int i = 0; specials[i].value; i++) {
            char *value = specials[i].value;
            Type type = specials[i].token.type;
            int len = (int)strlen(value);
            if (strncmp2(value, buff + s, len)) {
                Token *token = new_token(type, NULL);
                token->line = ura.line;
                token->s = s;
                token->e = s + len;
                token->space = space / TAB + ((space % TAB) != 0);
                if (type == DOTS)
                    space += TAB;
                e += len;
                break;
            }
        }
        if (e != s) {
            continue;
        }
        check_lexeme(buff, s, s + 1);
        e++;
    }
    for (int i = first_token; i < ura.tokens_count; i++) {
        ura.tokens[i]->s += file->start;
        ura.tokens[i]->e += file->start;
    }
    expand_uses(first_token);
}

void use_file(Token *use, Token *name) {
    char *path = find_package(name->chars.value, ura.file);
    if (!check_package(name, path))
        return;
    File *file = new_file(path);
    open_file(file);
    if (!check_reuse(name, file))
        return;
    file->start = end_of_sources();
    append(ura.files, file);
    append(ura.uses, file);
    File *outer = ura.file;
    int line = ura.line;
    int start = ura.tokens_count;
    ura.line = 1;
    tokenize(file);
    ura.file = outer;
    ura.line = line;
    ura.uses_count--;
    for (int i = start; i < ura.tokens_count; i++)
        ura.tokens[i]->space += use->space;
}

void expand_uses(int first_token) {
    int count = ura.tokens_count - first_token;
    Token **tokens = arena_alloc(count + 1, sizeof(Token *));
    memcpy(tokens, ura.tokens + first_token, count * sizeof(Token *));
    ura.tokens_count = first_token;
    for (int i = 0; i < count; i++) {
        Token *token = tokens[i];
        Token *name = i + 1 < count ? tokens[i + 1] : NULL;
        bool is_inner = token->space > 0 || ura.uses_count > 0;
        bool is_named = name && name->type == CHARS;
        if (token->type != USE || !is_inner || !is_named) {
            append(ura.tokens, token);
            continue;
        }
        use_file(token, name);
        i++; // SKIP the file name
    }
}

// AST
Node *new_node(Token *token) {
    Node *new = arena_alloc(1, sizeof(Node));
    new->token = token;
    print("new node %k\n", token);
    return new;
}

Token *peek(int pos) {
    assert(ura.exe_pos + pos <= ura.tokens_count);
    return ura.tokens[ura.exe_pos + pos];
}

Token *next() {
    Token *token = peek(0);
    if (token->type != LAST)
        ura.exe_pos++;
    return token;
}

int line_start(int pos) {
    File *file = file_at(pos);
    return file->start + line_start_in(file->buff, pos - file->start);
}

int line_at(int pos) {
    File *file = file_at(pos);
    int line = 1;
    for (int i = 0; i < pos - file->start; i++)
        line += file->buff[i] == '\n';
    return line;
}

char *token_text(Token *token) {
    return format("%.*s", token->e - token->s, text_at(token->s));
}

int line_end(int pos) {
    File *file = file_at(pos);
    return file->start + line_end_in(file->buff, pos - file->start);
}

bool inside(int space) {
    return ura.tokens[ura.exe_pos]->space > space;
}

void enter_scope(Node *node) {
    print(CYAN("enter scope %s\n"), node->token->name);
    append(ura.scopes, node);
    ura.scope = node;
}

void exit_scope() {
    print(CYAN("exit scope %s\n"), ura.scope->token->name);
    ura.scopes_count--;
    ura.scopes[ura.scopes_count] = NULL;
    if (ura.scopes_count > 0)
        ura.scope = ura.scopes[ura.scopes_count - 1];
    else
        ura.scope = NULL;
}

Node *find_template(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (child->token->type != TEMPLATE)
                continue;
            for (int k = 0; k < child->children_count; k++) {
                if (!strcmp2(child->children[k]->token->name, name))
                    continue;
                return child;
            }
        }
    }
    return NULL;
}

Node *find_template_type(char *name) {
    if (ura.types == NULL)
        return NULL;
    Node *params = ura.types->declared->args;
    for (int i = 0; i < ura.types->children_count; i++) {
        if (!strcmp2(params->children[i]->token->name, name))
            continue;
        return ura.types->children[i];
    }
    return NULL;
}

void parse_tokens(Node *scope, int start, Node *types) {
    int exe_pos = ura.exe_pos;
    Node *outer = ura.types;
    ura.exe_pos = start;
    ura.types = types;
    while (peek(0)->type != LAST) {
        Node *child = parse_expr(0);
        if (child->token->type != VAR)
            append(scope->children, child);
    }
    ura.exe_pos = exe_pos;
    ura.types = outer;
}

void parse_package(File *package) {
    open_file(package);
    for (int i = 0; i < ura.files_count; i++) {
        if (strcmp2(ura.files[i]->path, package->path))
            return;
    }
    package->start = end_of_sources();
    append(ura.files, package);

    File *file = ura.file;
    int line = ura.line;
    int start = ura.tokens_count;
    ura.line = 1;
    tokenize(package);
    new_token(LAST, NULL);
    ura.file = file;
    ura.line = line;
    parse_tokens(ura.scope, start, ura.types);
}

int copy_tokens(int from_pos, int to_pos, char *from, char *to, int space) {
    int start = ura.tokens_count;
    for (int i = from_pos; i < to_pos; i++) {
        Token *copy = new_token(ura.tokens[i]->type, NULL);
        *copy = *ura.tokens[i];
        copy->space += space;
        if (from && copy->type == ID && strcmp2(copy->name, from))
            copy->name = to;
    }
    new_token(LAST, NULL);
    return start;
}

bool is_declaration(int i) {
    Token *curr = ura.tokens[i];
    Token *after = ura.tokens[i + 1];
    bool is_same_line = after->line == curr->line;
    return curr->type == ID && is_same_line && (after->is_type || after->type == ID);
}

void prefix_names(Token *mod) {
    expand(char *, names);
    zero_init(names);
    int start = ura.exe_pos;
    int end = start;
    while (ura.tokens[end]->space > mod->space)
        end++;
    int space = ura.tokens[start]->space;
    for (int i = start; i < end; i++) {
        Token *curr = ura.tokens[i];
        Token *after = ura.tokens[i + 1];
        bool is_head = curr->space == space && curr->line != ura.tokens[i - 1]->line;
        bool is_head_type = includes(curr->type, STRUCT, FN_DEC, ENUM, MODULE, 0);
        bool is_named = is_head_type && after->type == ID;
        if (is_head && is_named)
            append(names, after->name);
        if (is_head && is_declaration(i))
            append(names, curr->name);
    }
    int struct_space = -1;
    for (int i = start; i < end; i++) {
        Token *curr = ura.tokens[i];
        bool starts_line = curr->line != ura.tokens[i - 1]->line;
        if (starts_line && curr->space <= struct_space)
            struct_space = -1;
        if (starts_line && curr->type == STRUCT)
            struct_space = curr->space;
        bool is_member = includes(ura.tokens[i - 1]->type, DOT, DOUBLE_DOTS, 0);
        bool is_in_struct = struct_space != -1 && curr->space == struct_space + 1;
        bool is_attr = starts_line && is_in_struct && is_declaration(i);
        if (curr->type != ID || is_member || is_attr)
            continue;
        for (size_t j = 0; j < names_count; j++) {
            if (!strcmp2(curr->name, names[j]))
                continue;
            curr->name = format("%s::%s", mod->name, curr->name);
            break;
        }
    }
}

void parse_module_name(Token *token) {
    while (peek(0)->type == DOUBLE_DOTS && peek(1)->type == ID) {
        if (!is_module(token->name))
            return;
        next(); // SKIP ::
        Token *member = next();
        token->name = format("%s::%s", token->name, member->name);
        token->e = member->e;
    }
}


void inherit_from_parent(Node *st, Node *parent) {
    Token *body = parent->token;
    Node *parents = body->body.parents;
    for (int i = 0; parents && i < parents->children_count; i++)
        inherit_from_parent(st, parents->children[i]);
    int space = st->token->space - parent->token->space;
    int start = copy_tokens(
        body->body.start, body->body.end, parent->token->name, st->token->name, space
    );
    parse_tokens(st, start, body->body.types);
}

void remove_overridden(Node *st) {
    for (int i = 0; i < st->children_count; i++) {
        Node *method = st->children[i];
        if (method->token->type != FN_DEC)
            continue;
        for (int j = i + 1; j < st->children_count; j++) {
            Node *other = st->children[j];
            if (other->token->type != FN_DEC)
                continue;
            if (!strcmp2(other->token->name, method->token->name))
                continue;
            if (other->token->is_pub != method->token->is_pub)
                continue;
            if (!same_params(other, method))
                continue;
            for (int k = i; k < st->children_count - 1; k++)
                st->children[k] = st->children[k + 1];
            st->children_count--;
            i--;
            break;
        }
    }
}

Node *parse_types(Token *token) {
    Node *types = new_node(new_token(TYPES, token));
    types->token->name = token->name;
    next(); // SKIP <
    while (!includes(peek(0)->type, GT, LAST, 0)) {
        Node *type = parse_type();
        if (type == NULL)
            break;
        append(types->children, type);
        if (peek(0)->type != COMA)
            break;
        next(); // SKIP ,
    }
    Token *close = peek(0);
    if (close->type == RSHIFT) {
        close->type = GT;
        close->s++;
        return types;
    }
    expect(GT, format("expected '>' after the types of '%s'", token->name), ">");
    return types;
}

char *init_from_template(Node *template, Node *types) {
    Token *token = types->token;
    char *name = format("%s<", token->name);
    for (int i = 0; i < types->children_count; i++)
        name = format("%s%s%s", name, i ? "," : "", type_name(types->children[i]));
    name = format("%s>", name);

    if (find_struct(name) || find_function(name))
        return name;

    Token *body = template->token;
    int start = copy_tokens(body->body.start, body->body.end, NULL, NULL, 0);
    int end = ura.tokens_count - 1;
    int top = ura.tokens[start]->space;
    char *suffix = name + strlen(token->name);
    for (int i = start; i + 1 < end; i++) {
        Token *keyword = ura.tokens[i];
        Token *declared = ura.tokens[i + 1];
        bool is_declaration = includes(keyword->type, FN_DEC, STRUCT, 0) &&
                              keyword->space == top && declared->type == ID;
        if (!is_declaration)
            continue;
        char *old = declared->name;
        char *renamed = format("%s%s", old, suffix);
        for (int j = start; j < end; j++)
            if (ura.tokens[j]->type == ID && strcmp2(ura.tokens[j]->name, old))
                ura.tokens[j]->name = renamed;
    }
    types->declared = template;
    parse_tokens(template->scope, start, types);
    return name;
}

Node *parse_type(void) {
    Token *token = peek(0);
    Token *prev = ura.tokens[ura.exe_pos - 1];
    Token *after = peek(1);
    bool is_ref = includes(token->type, REF, AND, 0) && token->line == prev->line;
    bool is_id = after->type == ID;
    bool is_variable = is_id && find_variable(after->name, after->s);
    bool is_function = is_id && find_function(after->name);
    bool is_name = is_id && !is_variable && !is_function;
    bool is_unary = includes(after->type, ADD, SUB, LBRACK, 0);
    bool is_operator = precedence[after->type] && !is_unary;
    if (is_ref && (after->is_type || is_name || is_operator)) {
        next(); // SKIP & or &&
        Node *ref = new_node(token);
        ref->token->is_type = true;
        ref->data_type = parse_type();
        if (token->type == AND)
            ref->data_type = new_type(REF, 0, ref->data_type);
        token->type = REF;
        return ref;
    }
    if (token->type == ID && token->line == prev->line)
        token->is_type = true;
    if (token->is_type) {
        next();
        parse_module_name(token);
        Node *node = token->type == ID ? find_template_type(token->name) : NULL;
        Node *template = token->type == ID ? find_template(token->name) : NULL;
        if (node == NULL)
            node = new_node(token);
        Node *types = peek(0)->type == LT ? parse_types(token) : NULL;
        if (check_template_use(token, template, types))
            token->name = init_from_template(template, types);
        while (peek(0)->type == LBRACK && peek(0)->line == token->line) {
            next(); // SKIP [
            Node *array = new_node(new_token(ARRAY, token));
            array->token->is_type = true;
            array->data_type = node;
            bool has_size = peek(0)->type != RBRACK && peek(0)->line == token->line;
            if (has_size)
                array->index = parse_expr(0);
            expect(RBRACK, "expected ']' after '['", "]");
            node = array;
        }
        return node;
    }
    return NULL;
}

Node *parse_block(Token *token) {
    Node *node = new_node(token);
    enter_scope(node);
    bool is_same_line = peek(0)->line == token->line;
    bool has_cond = peek(0)->type != DOTS && (token->type != ELSE || is_same_line);
    if (has_cond)
        node->cond = parse_expr(0);
    char *after[LAST + 1] = {
        [IF] = "the if condition",
        [ELIF] = "the elif condition",
        [LOOP] = "the while condition",
        [ELSE] = "else",
    };
    expect(DOTS, format("expected ':' after %s", after[token->type]), ":");
    while (inside(node->token->space)) {
        append(node->children, parse_expr(0));
    }
    exit_scope();
    return node;
}

Node *parse_prime() {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) { // clang-format off
    case INT: case FLOAT: case CHARS: { // clang-format on
        if (token->is_type)
            check_type_first(token);
        return new_node(token);
    }
    case ZERO: {
        return new_node(token);
    }
    case SCOPE: {
        node = new_node(token);
        enter_scope(node);
        append(ura.modules, "ura");
        char *core = find_package("@/core", NULL);
        if (core)
            parse_package(new_file(core));
        while (peek(0)->type != LAST)
            append(node->children, parse_expr(0));
        exit_scope();
        return node;
    }
    case ID: {
        Token *prev = ura.tokens[ura.exe_pos - 2];
        bool is_member = includes(prev->type, DOT, DOUBLE_DOTS, 0);
        parse_module_name(token);
        Token *builtin = is_member ? NULL : find_builtin(token->name);
        if (builtin) {
            token->type = builtin->type;
            token->num = builtin->num;
        }
        bool is_value = builtin && includes(builtin->type, ARGC, ARGV, 0);
        if (is_value)
            return new_node(token);
        bool is_source = includes(token->type, LINE_NUMBER, FILE_NAME, FN_NAME, 0);
        if (is_source)
            return new_node(source_value(token));
        if (builtin && peek(0)->type != LPARENT) {
            expect(LPARENT, format("expected '(' after '%s'", token->name), "(x)");
            node = new_node(token);
            append(node->children, parse_prime());
            return node;
        }
        Node *template_type = find_template_type(token->name);
        Node *template = find_template(token->name);
        bool is_template_use = template && peek(0)->type == LT;
        if (template_type || is_template_use || find_struct(token->name))
            check_type_first(token);
        if (template_type)
            return template_type;
        Node *types = is_template_use ? parse_types(token) : NULL;
        if (types && check_template_use(token, template, types))
            token->name = init_from_template(template, types);
        Node *type = builtin ? NULL : parse_type();
        if (type != NULL) {
            node = new_node(new_token(DECLARE, token));
            node->var = new_node(token);
            node->var->token->type = VAR;
            node->var->data_type = type;
            append(ura.scope->children, node);
            node = node->var;
        } else if (peek(0)->type == LPARENT) {
            if (builtin == NULL)
                token->type = FN_CALL;
            node = new_node(token);
            next(); // SKIP (
            while (!includes(peek(0)->type, RPARENT, LAST, 0)) {
                append(node->children, parse_expr(0));
                if (check_comma(token, peek(0), "arguments"))
                    continue;
                if (peek(0)->type != COMA)
                    break;
                check_comma(next(), peek(0), "arguments");
            }
            expect(
                RPARENT,
                format("expected ')' to close the call to '%s'", token->name),
                ")"
            );
        } else
            node = new_node(token);
        return node;
    }
    case SELF: {
        return new_node(token);
    }
    case LPARENT: {
        node = parse_expr(0);
        expect(RPARENT, "expected ')' to close this '('", ")");
        return node;
    } // clang-format off
    case SUB: case BNOT: { // clang-format on
        if (token->type == SUB)
            token->type = NEG;
        node = new_node(token);
        node->operand = parse_expr(precedence[DOT] - 1);
        return node;
    }
    case REF: {
        node = new_node(token);
        node->operand = parse_expr(precedence[DOT] - 1);
        return node;
    }
    case AND: {
        token->type = REF;
        node = new_node(token);
        node->operand = new_node(new_token(REF, token));
        node->operand->operand = parse_expr(precedence[DOT] - 1);
        return node;
    }
    case ADD: {
        return parse_prime();
    } // clang-format off
    case FN_DEC: case PROTO: case OPERATOR: { // clang-format on
        node = new_node(token);
        bool is_operator = token->type == OPERATOR;
        if (is_operator) {
            token->type = FN_DEC;
            token = next();
            node->token->name = token_text(token);
            if (token->type == LBRACK) {
                expect(RBRACK, "expected ']' after 'operator ['", "]");
                node->token->name = "[]";
            }
        } else if (peek(0)->type != ID)
            node->token->name = "";
        else {
            token = next();
            node->token->name = token->name;
        }
        node->right = new_node(new_token(ARGS, token));
        if (is_operator)
            node->args->symbol = new_node(token);
        if (peek(0)->type == LPARENT) {
            next(); // SKIP (
            while (peek(0)->type != RPARENT) {
                Token *token = peek(0);
                if (token->type == ID) {
                    token->type = PARAM;
                    Node *param = new_node(token);
                    next(); // skip ID
                    param->data_type = parse_type();
                    check_param_type(token, param);
                    append(node->right->children, param);
                } else if (token->type == VARIADIC) {
                    append(node->right->children, new_node(token));
                    next(); // skip ...
                } else
                    break;
                if (check_comma(token, peek(0), "parameters"))
                    continue;
                if (peek(0)->type != COMA)
                    break;
                next(); // SKIP ,
            }
            expect(RPARENT, "expected ')' after the parameters", ")");
        } else
            expect(LPARENT, "expected '(' after the function name", "()");
        node->ret_type = parse_type();
        if (node->token->type == PROTO)
            return node;
        expect(DOTS, "expected ':' after the function header", ":");
        enter_scope(node);
        while (inside(node->token->space)) {
            append(node->children, parse_expr(0));
        }
        exit_scope();
        return node;
    }
    case RETURN: {
        node = new_node(token);
        if (peek(0)->line == token->line)
            node->left = parse_expr(0);
        return node;
    }
    case IF:
    case LOOP: {
        node = parse_block(token);
        if (token->type != IF)
            return node;
        Node *branch = node;
        while (includes(peek(0)->type, ELIF, ELSE, 0)) {
            if (peek(0)->space != token->space)
                break;
            branch->next = parse_block(next());
            branch = branch->next;
        }
        return node;
    }
    case FOR: {
        node = new_node(token);
        enter_scope(node);
        Token *name = peek(0);
        expect(ID, "expected a name after 'for'", " x");
        Node *declare = new_node(new_token(DECLARE, name));
        declare->var = new_node(name);
        declare->var->token->type = VAR;
        node->var = declare->var;
        append(node->children, declare);
        expect(IN, "expected 'in' after the loop name", " in");
        node->right = parse_expr(0);
        expect(DOTS, "expected ':' after the for line", ":");
        while (inside(token->space))
            append(node->children, parse_expr(0));
        exit_scope();
        return node;
    }
    case MATCH: {
        node = new_node(token);
        enter_scope(node);
        Node *declare = new_node(new_token(DECLARE, token));
        declare->var = new_node(new_token(VAR, token));
        declare->var->token->name = format("match@%d", token->s);
        append(node->children, declare);
        node->left = new_node(new_token(ASSIGN, token));
        node->left->left = declare->var;
        node->left->right = parse_expr(0);
        expect(DOTS, "expected ':' after the match value", ":");
        while (inside(token->space)) {
            Token *head = next();
            if (!includes(head->type, CASE, DEFAULT, 0)) {
                check_unexpected(head);
                continue;
            }
            Node *branch = new_node(head);
            while (head->type == CASE && !includes(peek(0)->type, DOTS, LAST, 0)) {
                Node *value = new_node(new_token(ID, head));
                value->token->name = declare->var->token->name;
                Node *eq = new_node(new_token(EQ, head));
                eq->left = value;
                eq->right = parse_expr(0);
                Node *cond = eq;
                if (branch->cond) {
                    cond = new_node(new_token(OR, head));
                    cond->left = branch->cond;
                    cond->right = eq;
                }
                branch->cond = cond;
                if (peek(0)->type != COMA)
                    break;
                next(); // SKIP ,
            }
            expect(DOTS, format("expected ':' after %s", token_text(head)), ":");
            enter_scope(branch);
            while (inside(head->space))
                append(branch->children, parse_expr(0));
            exit_scope();
            append(node->children, branch);
        }
        exit_scope();
        return node;
    }
    case TRY: {
        ura.throws = true;
        node = new_node(token);
        expect(DOTS, "expected ':' after 'try'", ":");
        enter_scope(node);
        while (inside(token->space))
            append(node->children, parse_expr(0));
        exit_scope();
        Node *branch = node;
        while (peek(0)->type == CATCH && peek(0)->space == token->space) {
            Token *head = next();
            branch->next = new_node(head);
            branch = branch->next;
            enter_scope(branch);
            if (peek(0)->type == ID && peek(0)->line == head->line) {
                Token *name = next();
                Node *declare = new_node(new_token(DECLARE, name));
                declare->var = new_node(name);
                declare->var->token->type = VAR;
                declare->var->data_type = parse_type();
                branch->var = declare->var;
                if (declare->var->data_type)
                    append(branch->children, declare);
            }
            expect(DOTS, "expected ':' after the catch type", ":");
            while (inside(head->space))
                append(branch->children, parse_expr(0));
            exit_scope();
        }
        return node;
    }
    case THROW: {
        ura.throws = true;
        node = new_node(token);
        node->left = parse_expr(0);
        return node;
    } // clang-format off
    case CATCH: case CASE: case DEFAULT: { // clang-format on
        check_unexpected(token);
        while (peek(0)->line == token->line || inside(token->space))
            next();
        return new_node(token);
    } // clang-format off
    case ELIF: case ELSE: { // clang-format on
        check_unexpected(token);
        return parse_block(token);
    }
    case PUB: {
        node = parse_prime();
        node->token->is_pub = true;
        return node;
    }
    case LINK: {
        node = new_node(token);
        if (peek(0)->type == CHARS)
            node->token->name = next()->chars.value;
        else
            expect(CHARS, "expected a library name after 'link'", " \"name\"");
        return node;
    }
    case LCBRACK: {
        node = new_node(token);
        while (peek(0)->type == ID) {
            Node *attr = new_node(next());
            attr->token->type = ATTR;
            expect(DOTS, "expected ':' after the attribute name", ":");
            attr->left = parse_expr(0);
            append(node->children, attr);
            if (peek(0)->type != COMA)
                break;
            next(); // SKIP ,
        }
        expect(RCBRACK, "expected '}' to close the struct value", "}");
        return node;
    }
    case LBRACK: {
        node = new_node(token);
        while (!includes(peek(0)->type, RBRACK, LAST, 0)) {
            append(node->children, parse_expr(0));
            if (check_comma(token, peek(0), "elements"))
                continue;
            if (peek(0)->type != COMA)
                break;
            next(); // SKIP ,
        }
        expect(RBRACK, "expected ']' to close the array", "]");
        return node;
    }
    case NOT: {
        node = new_node(token);
        node->operand = parse_expr(precedence[AND]);
        return node;
    }
    case MACRO_IF: {
        node = new_node(token);
        bool is_done = false;
        Token *branch = token;
        while (true) {
            bool is_chosen = !is_done;
            if (branch->type != MACRO_ELSE)
                is_chosen = macro_cond(parse_expr(0)) && !is_done;
            expect(DOTS, format("expected ':' after %s", token_text(branch)), ":");
            while (inside(branch->space)) {
                if (is_chosen) {
                    append(ura.scope->children, parse_expr(0));
                } else
                    next();
            }
            is_done = is_done || is_chosen;
            bool is_branch = includes(peek(0)->type, MACRO_ELIF, MACRO_ELSE, 0);
            bool is_same_level = peek(0)->space == token->space;
            if (!is_branch || !is_same_level)
                break;
            branch = next();
        }
        return node;
    }
    case MACRO_ERROR: {
        node = new_node(token);
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            node->left = new_node(next());
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            node->right = new_node(next());
        check_macro(node);
        return node;
    } // clang-format off
    case MACRO_NAME: case MACRO_VERSION: case MACRO_MAIN:
    case MACRO_SCRIPT: case MACRO_DEPENDENCY: { // clang-format on
        node = new_node(token);
        while (peek(0)->type == CHARS && peek(0)->line == token->line)
            append(node->children, new_node(next()));
        check_config(node);
        return node;
    }
    case USE: {
        node = new_node(token);
        if (peek(0)->type != CHARS) {
            expect(CHARS, "expected a package name after 'use'", " \"@/name\"");
            return node;
        }
        Token *name = next();
        char *path = find_package(name->chars.value, file_at(name->s));
        if (check_package(name, path))
            parse_package(new_file(path));
        return node;
    }
    case TEMPLATE: {
        node = new_node(token);
        node->scope = ura.scope;
        node->args = new_node(new_token(ARGS, token));
        expect(LT, "expected '<' after 'template'", "<T>");
        while (!includes(peek(0)->type, GT, LAST, 0)) {
            append(node->args->children, new_node(next()));
            if (peek(0)->type != COMA)
                break;
            next(); // SKIP ,
        }
        expect(GT, "expected '>' after the template names", ">");
        expect(DOTS, "expected ':' after 'template'", ":");
        int space = peek(0)->space;
        token->body.start = ura.exe_pos;
        while (inside(token->space)) {
            Token *prev = ura.tokens[ura.exe_pos - 1];
            Token *curr = next();
            bool is_head = curr->space == space && curr->line != prev->line;
            bool is_named = includes(curr->type, STRUCT, FN_DEC, PUB, 0);
            if (is_head && is_named && peek(0)->type == ID)
                append(node->children, new_node(peek(0)));
        }
        token->body.end = ura.exe_pos;
        return node;
    }
    case ENUM: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else
            expect(ID, "expected an enum name after 'enum'", " Name");
        expect(DOTS, "expected ':' after the enum name", ":");
        long long value = 0;
        while (true) {
            Token *name = peek(0);
            bool is_near = name->line == token->line || inside(token->space);
            if (name->type != ID || !is_near)
                break;
            Node *variant = new_node(next());
            if (peek(0)->type == ASSIGN) {
                next(); // SKIP =
                variant->left = parse_prime();
                if (variant->left->token->type == INT)
                    value = variant->left->token->num.value;
            }
            name->num.value = value++;
            append(node->children, variant);
            if (peek(0)->type == COMA)
                next(); // SKIP ,
        }
        return node;
    }
    case MODULE: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else
            expect(ID, "expected a mod name after 'mod'", " name");
        expect(DOTS, "expected ':' after the mod name", ":");
        if (!check_module(node))
            return node;
        append(ura.modules, node->token->name);
        prefix_names(node->token);
        while (inside(token->space)) {
            Node *child = parse_expr(0);
            if (child->token->type != VAR)
                append(ura.scope->children, child);
        }
        return node;
    }
    case STRUCT: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else {
            expect(ID, "expected a struct name after 'struct'", " Name");
            node->token->name = "";
        }
        Node *parents = new_node(new_token(TYPES, token));
        if (peek(0)->type == FROM) {
            next(); // SKIP from
            while (true) {
                Token *name = peek(0);
                Node *type = parse_type();
                Node *parent = type ? find_struct(type->token->name) : NULL;
                if (check_parent(name, parent))
                    append(parents->children, parent);
                if (peek(0)->type != AND_LIT)
                    break;
                next(); // SKIP and
            }
        }
        expect(DOTS, "expected ':' after the struct name", ":");
        enter_scope(node);
        for (int i = 0; i < parents->children_count; i++)
            inherit_from_parent(node, parents->children[i]);
        int end = ura.exe_pos;
        while (ura.tokens[end]->type != LAST && ura.tokens[end]->space > token->space)
            end++;
        token->body.start = copy_tokens(ura.exe_pos, end, NULL, NULL, 0);
        token->body.end = token->body.start + end - ura.exe_pos;
        token->body.types = ura.types;
        token->body.parents = parents;
        while (inside(node->token->space)) {
            Node *child = parse_expr(0);
            if (!includes(child->token->type, FN_DEC, TEMPLATE, STRUCT, 0)) {
                check_attribute(child);
                continue;
            }
            append(node->children, child);
        }
        exit_scope();
        remove_overridden(node);
        for (int i = 0; i < node->children_count; i++) {
            Node *method = node->children[i];
            if (method->token->type != FN_DEC || method->token->is_pub)
                continue;
            Node *type = new_type(ID, 0, NULL);
            type->token->name = node->token->name;
            Node *self = new_node(new_token(PARAM, method->token));
            self->token->name = "self";
            self->data_type = new_type(REF, 0, type);
            Node *args = method->args;
            append(args->children, self);
            for (int j = args->children_count - 1; j > 0; j--)
                args->children[j] = args->children[j - 1];
            args->children[0] = self;
        }
        return node;
    } // clang-format off
    case BRK: case CNT: { // clang-format on
        node = new_node(token);
        for (int i = ura.scopes_count - 1; i >= 0 && node->loop == NULL; i--) {
            Type type = ura.scopes[i]->token->type;
            bool is_loop = includes(type, LOOP, FOR, 0);
            bool is_target = is_loop || (type == MATCH && token->type == BRK);
            if (is_target)
                node->loop = ura.scopes[i];
        }
        return node;
    }
    default: {
        check_unexpected(token);
        return new_node(token);
    }
    }
    return NULL;
}

Node *parse_expr(int min_op) {
    Node *left = parse_prime();
    while (true) {
        Token *prev = ura.tokens[ura.exe_pos - 1];
        bool is_band = peek(0)->type == REF && peek(0)->line == prev->line;
        if (is_band)
            peek(0)->type = BAND;
        if (peek(0)->type == AND_LIT)
            peek(0)->type = AND;
        if (peek(0)->type == OR_LIT)
            peek(0)->type = OR;
        int e = peek(0)->type;
        if (e == LAST)
            break;
        assert(e >= 0 && e <= LAST);
        // printf("accessing %d\n", e);
        int op = precedence[e];
        if (op <= min_op)
            break;
        Node *node = new_node(next());
        node->left = left;
        Token *token = node->token;
        bool is_missing = peek(0)->type == LAST || peek(0)->line != token->line;
        if (!is_missing && token->type == LBRACK) {
            token->type = ACCESS;
            node->index = parse_expr(0);
            expect(RBRACK, "expected ']' after the index", "]");
        } else if (!is_missing && token->type == AS)
            node->to_type = parse_type();
        else if (!is_missing)
            node->right = parse_expr(op);
        check_operand(node);
        if (is_missing)
            return node;
        left = node;
    }
    return left;
}

// ANALYZE
Node *find_in_children(Node *parent, char *name) {
    for (int j = 0; j < parent->children_count; j++) {
        Node *child = parent->children[j];
        if (!includes(child->token->type, DECLARE, PARAM, 0))
            continue;
        Node *id = child->token->type == DECLARE ? child->var : child;
        if (strcmp2(id->token->name, name))
            return id;
    }
    return NULL;
}

Node *find_in_scope(Node *scope, char *name) {
    Node *found = NULL;
    if (scope->token->type == FN_DEC)
        found = find_in_children(scope->args, name);
    if (found == NULL)
        found = find_in_children(scope, name);
    return found;
}

Node *find_variable(char *name, int pos) {
    bool is_past_fn = false;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        bool is_global = scope->token->type == SCOPE;
        if (is_past_fn && !is_global)
            continue;
        is_past_fn = is_past_fn || scope->token->type == FN_DEC;
        Node *param =
            scope->token->type == FN_DEC ? find_in_children(scope->args, name) : NULL;
        if (param)
            return param;
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            if (child->token->type != DECLARE ||
                !strcmp2(child->var->token->name, name))
                continue;
            if (is_global || child->var->token->s <= pos)
                return child->var;
        }
    }
    return NULL;
}

Node *find_scope(Type type) {
    for (int i = ura.scopes_count - 1; i >= 0; i--)
        if (ura.scopes[i]->token->type == type)
            return ura.scopes[i];
    return NULL;
}

int name_diff(char *a, char *b) {
    int diff = 0;
    int k = 0;
    for (; a[k] && b[k]; k++)
        diff += a[k] != b[k];
    return diff + strlen(a + k) + strlen(b + k);
}

Node *find_variable_like(char *name, int pos, int max_diff) {
    Node *found = NULL;
    int found_diff = max_diff + 1;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            bool is_before =
                child->token->type == DECLARE && child->var->token->s <= pos;
            if (!is_before)
                continue;
            int diff = name_diff(child->var->token->name, name);
            if (diff < found_diff) {
                found = child->var;
                found_diff = diff;
            }
        }
    }
    return found;
}

Node *find_function_like(char *name, int max_diff) {
    Node *found = NULL;
    int found_diff = max_diff + 1;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        if (curr->token->type == STRUCT)
            continue;
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            bool is_function = includes(child->token->type, FN_DEC, PROTO, 0);
            bool is_operator = child->args && child->args->symbol;
            if (!is_function || is_operator)
                continue;
            int diff = name_diff(child->token->name, name);
            if (diff >= found_diff)
                continue;
            found = child;
            found_diff = diff;
        }
    }
    return found;
}

Node *find_function(char *name) {
    return find_function_like(name, 0);
}

Node *find_method_named(Node *st, char *name) {
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        bool is_method = child->token->type == FN_DEC && !child->token->is_pub;
        if (is_method && strcmp2(child->token->name, name))
            return child;
    }
    return NULL;
}

Node *find_attr(Node *st, char *name, int *index) {
    *index = 0;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != DECLARE)
            continue;
        if (strcmp2(child->var->token->name, name))
            return child->var;
        (*index)++;
    }
    return NULL;
}

bool fits_args(Node *fn, Node **args, int args_count, int self) {
    if (fn->args->children_count != args_count + self)
        return false;
    for (int j = 0; j < args_count; j++) {
        Node *type = fn->args->children[j + self]->data_type;
        Node *found = type_of(args[j]);
        bool is_unknown = found == NULL || type == NULL;
        bool is_text = args[j]->token->type == CHARS && is_chars(type);
        bool is_list = args[j]->token->type == LBRACK && type && element_type_of(type);
        if (is_unknown || is_text || is_list ||
            (is_number(args[j]) && is_number(type)))
            continue;
        bool takes_ref = type->token->type == REF && found->token->type != REF;
        if (!same_type(found, takes_ref ? type->data_type : type))
            return false;
    }
    return true;
}

Node *find_method(Node *st, char *name, Node **args, int args_count, bool is_pub) {
    int self = is_pub ? 0 : 1;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != FN_DEC || child->token->is_pub != is_pub)
            continue;
        if (strcmp2(child->token->name, name) &&
            fits_args(child, args, args_count, self))
            return child;
    }
    return NULL;
}

Node *find_overload(char *name, Node **args, int args_count) {
    Node *proto = NULL;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = 0; j < scope->children_count; j++) {
            Node *child = scope->children[j];
            bool is_function =
                includes(child->token->type, FN_DEC, PROTO, 0) && child->args;
            if (!is_function || !strcmp2(child->token->name, name))
                continue;
            if (!fits_args(child, args, args_count, 0))
                continue;
            if (child->token->type == FN_DEC)
                return child;
            proto = proto ? proto : child;
        }
    }
    return proto;
}

Node *find_struct(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (child->token->type != STRUCT)
                continue;
            if (!strcmp2(child->token->name, name))
                continue;
            return child;
        }
    }
    return NULL;
}

Node *find_enum(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (child->token->type == ENUM && strcmp2(child->token->name, name))
                return child;
        }
    }
    return NULL;
}

bool is_unused_global(Token *var) {
    bool is_package = file_at(var->s) != ura.files[0];
    return is_package && !var->is_used;
}

bool is_module(char *name) {
    for (size_t i = 0; i < ura.modules_count; i++) {
        if (strcmp2(ura.modules[i], name))
            return true;
    }
    return false;
}

Node *new_type(Type type, int shift, Node *data_type) {
    Node *node = new_node(new_token(type, NULL));
    node->token->is_type = true;
    node->token->shift = shift;
    node->data_type = data_type;
    return node;
}

Node *bool_type(void) {
    return new_type(INT, 1, NULL);
}

Node *chars_type(void) {
    return new_type(ARRAY, 0, new_type(INT, 8, NULL));
}

bool is_chars(Node *type) {
    Node *element = type && type->token->type == ARRAY ? type->data_type : NULL;
    return element && element->token->type == INT && element->token->shift == 8;
}

Node *element_type_of(Node *type) {
    if (type->token->type == ARRAY)
        return type->data_type;
    bool is_array =
        type->token->type == ID && strncmp2(type->token->name, "Array<", 6);
    if (is_array && type->declared == NULL)
        type->declared = find_struct(type->token->name);
    if (!is_array || type->declared == NULL)
        return NULL;
    int index = 0;
    Node *data = find_attr(type->declared, "data", &index);
    return data ? data->data_type->data_type : NULL;
}

Node *string_type(void) {
    Node *st = find_struct("String");
    if (st == NULL)
        return chars_type();
    Node *type = new_node(new_token(ID, NULL));
    type->token->name = st->token->name;
    type->token->is_type = true;
    type->declared = st;
    return type;
}

Node *ret_type_of(Node *fn) {
    return fn->ret_type ? fn->ret_type : new_type(VOID, 0, NULL);
}

bool is_ref_variable(Node *node) {
    if (!includes(node->token->type, VAR, PARAM, 0) || node->data_type == NULL)
        return false;
    return node->data_type->token->type == REF;
}

bool is_ref_binding(Node *node) {
    if (node->token->type != ASSIGN || !is_ref_variable(node->left))
        return false;
    Node *right = node->right;
    bool is_member = includes(right->token->type, DOT, DOUBLE_DOTS, 0);
    Node *call = is_member ? right->right : right;
    bool is_address = right->token->type == REF && !right->token->is_type;
    bool is_ref_call = call->token->type == FN_CALL && lives_in_variable(right);
    bool is_declaration = node->left->declared == NULL;
    bool is_null = right->token->type == ZERO;
    return is_address || is_null || (is_ref_call && is_declaration);
}

char *type_name(Node *type) {
    switch (type->token->type) {
    case INT: {
        int shift = type->token->shift;
        if (shift == 1)
            return "bool";
        if (shift == 21)
            return "unic";
        return format("%c%d", type->token->is_uint ? 'u' : 'i', shift);
    } // clang-format off
    case FLOAT: return format("f%d", type->token->shift);
    case ARRAY: return format("%s[]", type_name(type->data_type));
    case REF: return format("&%s", type_name(type->data_type));
    case VOID: return "void";
    case ID: return type->token->name;
    default: return (char *)to_string(type->token->type);
    } // clang-format on
}

Node *type_of(Node *node) {
    switch (node->token->type) { // clang-format off
    case INT: case FLOAT: return node;
    case AS:        return node->to_type;
    case LCBRACK:   return node->data_type;
    case LBRACK:    return node->data_type;
    case ZERO:      return node->data_type;
    case DOUBLE_DOTS: return type_of(node->right);
    case DOT: {
        if (node->right->token->type == FN_CALL)
            return type_of(node->right);
        return node->right->declared ? node->right->declared->data_type : NULL;
    }
    case ACCESS: {
        Node *type = type_of(node->left);
        return type && type->token->type == ARRAY ? type->data_type : NULL;
    }
    case CHARS:     return node->data_type ? node->data_type : string_type();
    case VAR: case PARAM: return is_ref_variable(node) ? node->data_type->data_type : node->data_type;
    case REF: {
        Node *type = type_of(node->operand);
        if (type && type->token->type == REF)
            return type;
        return type ? new_type(REF, 0, type) : NULL;
    }
    case FN_CALL: {
        Node *type = node->to_call ? ret_type_of(node->to_call) : NULL;
        return type && type->token->type == REF ? type->data_type : type;
    }
    case NEG: {
        Node *type = type_of(node->operand);
        return is_numeric(type) ? type : NULL;
    }
    case BNOT: {
        Node *type = type_of(node->operand);
        bool is_integer = is_numeric(type) && type->token->type == INT;
        return is_integer ? type : NULL;
    }
    case COPY:      return type_of(node->to_copy);
    case SIZEOF: case ARGC: return new_type(INT, 32, NULL);
    case TYPEOF: case READ: case ALLOC: return chars_type();
    case IS_ZERO: return bool_type();
    case ARGV: return new_type(ARRAY, 0, chars_type());
    case WRITE: case DROP: case MEMCPY: case FREE: return new_type(VOID, 0, NULL);
    case LT: case GT: case EQ: case LE: case GE: case NQ:
    case AND: case OR: case NOT: return bool_type();
    case ID:        return NULL;
    case SELF:      return NULL;
    default:        return type_of(node->left); // clang-format on
    }
}

bool same_type(Node *a, Node *b) {
    if (a->token->type != b->token->type)
        return false;
    if (includes(a->token->type, REF, ARRAY, 0))
        return same_type(a->data_type, b->data_type);
    if (a->token->type == ID)
        return strcmp2(a->token->name, b->token->name);
    if (a->token->is_uint != b->token->is_uint)
        return false;
    return a->token->shift == b->token->shift;
}

bool same_params(Node *a, Node *b) {
    if (a->args->children_count != b->args->children_count)
        return false;
    for (int i = 0; i < a->args->children_count; i++) {
        Node *left = a->args->children[i];
        Node *right = b->args->children[i];
        if (left->token->type != right->token->type)
            return false;
        bool has_types = left->data_type && right->data_type;
        if (has_types && !same_type(left->data_type, right->data_type))
            return false;
    }
    return true;
}

bool is_number(Node *node) {
    return node->token->type == INT && node->token->shift != 1;
}

bool is_operation(Type type) {
    return precedence[type] && precedence[type] < precedence[AS];
}

void analyze_block(Node *node) {
    enter_scope(node);
    for (int i = 0; i < node->children_count; i++)
        analyze(node->children[i]);
    exit_scope();
}

void analyze(Node *node) {
    if (is_operation(node->token->type)) {
        analyze(node->left);
        analyze(node->right);
        check_operation(node);
        return;
    }
    switch (node->token->type) { // clang-format off
    case INT: case FLOAT: case VOID: 
    case VAR: case PARAM: case CHARS: case VARIADIC:
    case USE: case ZERO:
    case MACRO_IF: case MACRO_ERROR: case MODULE: { // clang-format on
        break;
    } // clang-format off
    case WRITE: case READ: case SIZEOF: case TYPEOF: case DROP: case IS_ZERO:
    case ALLOC: case MEMCPY: case FREE: case ARGC: case ARGV: { // clang-format on
        check_builtin(node);
        break;
    } // clang-format off
    case ID: case SELF: { // clang-format on
        check_name(node);
        break;
    }
    case DECLARE: {
        check_declare(node);
        analyze(node->var->data_type);
        break;
    } // clang-format off
    case SCOPE: case FN_DEC: case PROTO: case STRUCT: { // clang-format on
        enter_scope(node);
        if (node->ret_type)
            analyze(node->ret_type);
        if (node->args) {
            for (int i = 0; i < node->args->children_count; i++) {
                Node *arg = node->args->children[i];
                analyze(arg);
                if (arg->data_type)
                    analyze(arg->data_type);
            }
        }
        if (includes(node->token->type, FN_DEC, PROTO, 0))
            check_function(node, ura.scopes[ura.scopes_count - 2]);
        else if (node->token->type == STRUCT)
            check_struct(node, ura.scopes[ura.scopes_count - 2]);

        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    }
    case RETURN: {
        if (node->left)
            analyze(node->left);
        check_return(node);
        break;
    } // clang-format off
    case MATCH: {
        enter_scope(node);
        analyze(node->left->right);
        check_match(node);
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    } // clang-format off
    case THROW: {
        analyze(node->left);
        check_throw(node);
        break;
    } // clang-format off
    case TRY: case CATCH: { // clang-format on
        check_try(node);
        analyze_block(node);
        if (node->next)
            analyze(node->next);
        break;
    } // clang-format off
    case IF: case ELIF: case ELSE: case LOOP: case CASE: case DEFAULT: { // clang-format on
        check_cond(node);
        analyze_block(node);
        if (node->next)
            analyze(node->next);
        break;
    } // clang-format off
    case BRK: case CNT: { // clang-format on
        check_jump(node);
        break;
    }
    case DOUBLE_DOTS: {
        check_static_call(node);
        break;
    }
    case ENUM: {
        check_enum(node);
        break;
    }
    case TEMPLATE: {
        check_template(node);
        break;
    }
    case LINK: {
        check_link(node);
        break;
    } // clang-format off
    case NEG: case BNOT: { // clang-format on
        analyze(node->operand);
        check_unary(node);
        break;
    }
    case NOT: {
        analyze(node->operand);
        check_type(node->operand, bool_type(), NULL);
        break;
    } // clang-format off
    case REF: case ARRAY: { // clang-format on
        if (node->token->is_type && node->data_type)
            analyze(node->data_type);
        else if (!node->token->is_type)
            analyze(node->operand);
        if (node->token->type == REF)
            check_ref(node);
        else
            check_array(node);
        break;
    }
    case AS: {
        analyze(node->left);
        if (node->to_type)
            analyze(node->to_type);
        check_cast(node);
        break;
    }
    case LCBRACK: {
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]->left);
        break;
    }
    case LBRACK: {
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        check_array_literal(node, NULL);
        break;
    }
    case FOR: {
        analyze(node->right);
        check_for(node);
        analyze_block(node);
        break;
    }
    case ACCESS: {
        analyze(node->left);
        analyze(node->index);
        check_access(node);
        break;
    }
    case DOT: {
        analyze(node->left);
        check_member(node);
        break;
    }
    case FN_CALL: {
        check_call(node);
        break;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, node->token->type);
        exit(1);
        break;
    }
    }
}

// CODE GEN
void codegen_drop(Ucg *place, Node *type) {
    Node *drop = find_drop(type);
    if (drop) {
        Ucg *self = ucg_ref(place);
        ucg_fn_call(drop->token->ucg, &self, 1);
    }
    Node *st = type->declared;
    for (int i = st->children_count - 1; i >= 0; i--) {
        Node *attr = st->children[i];
        if (attr->token->type != DECLARE)
            continue;
        if (!needs_drop(attr->var->data_type))
            continue;
        int index = 0;
        find_attr(st, attr->var->token->name, &index);
        codegen_drop(ucg_get_attr(place, index), attr->var->data_type);
    }
}

void codegen_clean(Ucg *place, Node *type) {
    if (!needs_drop(type))
        return;
    ucg_clean(place);
    codegen_drop(place, type);
    ucg_clean_end();
}

void codegen_global_drops(Node *scope) {
    for (int i = 0; i < scope->children_count; i++) {
        Node *child = scope->children[i];
        bool is_declare = child->token->type == DECLARE;
        if (is_declare && !is_unused_global(child->var->token))
            codegen_clean(child->var->token->ucg, child->var->data_type);
    }
}

void codegen_drop_temporaries(void) {
    Node *scope = ura.scope;
    for (int i = scope->temporaries_count - 1; i >= 0; i--)
        codegen_drop(scope->temporaries[i], scope->temporary_types[i]);
    scope->temporaries_count = 0;
    scope->temporary_types_count = 0;
}

void codegen_statement(Node *node) {
    bool is_method = node->token->type == DOT && node->right->token->type == FN_CALL;
    bool is_call = includes(node->token->type, FN_CALL, DOUBLE_DOTS, 0) || is_method;
    if (is_call && needs_drop(type_of(node)))
        codegen_in_variable(node);
    else
        codegen(node);
    codegen_drop_temporaries();
}

void codegen_branch(Node *node, Ucg *stmt) {
    node->token->ucg = stmt;
    Ucg *cond = NULL;
    if (node->cond) {
        codegen(node->cond);
        cond = node->cond->token->ucg;
        codegen_drop_temporaries();
    }
    ucg_then(stmt, cond);
    enter_scope(node);
    codegen_block(node, 0);
    exit_scope();
}

void codegen_block(Node *node, int start) {
    codegen_structs(node);
    codegen_protos(node);
    for (int i = start; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == PROTO)
            continue;
        if (child->token->type == STRUCT) {
            codegen(child);
            continue;
        }
        codegen_statement(child);
        if (includes(child->token->type, RETURN, BRK, CNT, THROW, 0))
            break;
    }
}

void codegen_try(Node *node) {
    Ucg *try_stmt = ucg_try();
    enter_scope(node);
    codegen_block(node, 0);
    exit_scope();
    Ucg *prev = try_stmt;
    Ucg *first = NULL;
    for (Node *branch = node->next; branch; branch = branch->next) {
        Node *type = branch->var->data_type;
        codegen(type);
        Ucg *catch_stmt = ucg_catch(prev, type->token->ucg);
        first = first ? first : catch_stmt;
        enter_scope(branch);
        codegen(branch->children[0]);
        ucg_assign(branch->var->token->ucg, ucg_error(type->token->ucg));
        codegen_block(branch, 1);
        exit_scope();
        prev = catch_stmt;
    }
    ucg_end_catch(first);
}

void codegen_uncaught(Ucg *try_stmt) {
    Node *hook = find_function("uncaught");
    bool has_one_param = hook && hook->args && hook->args->children_count == 1;
    Node *param = has_one_param ? hook->args->children[0]->data_type : NULL;
    Node *type = param && param->token->type == REF ? param->data_type : param;
    if (type) {
        Ucg *catch_stmt = ucg_catch(try_stmt, type->token->ucg);
        Ucg *code = ucg_call(hook->token->ucg, 1, ucg_ref(ucg_error(type->token->ucg)));
        ucg_ret(code);
        ucg_end(catch_stmt);
    } else
        ucg_end_try(try_stmt);
    Ucg *uncaught = ucg_if();
    ucg_then(uncaught, ucg_thrown());
    ucg_ret(ucg_i32_value(1));
    ucg_end(uncaught);
}

void codegen_proto(Node *node, char *name) {
    Ucg *ret_type = NULL;
    if (node->ret_type) {
        codegen(node->ret_type);
        ret_type = node->ret_type->token->ucg;
    } else
        ret_type = ucg_void_type();

    Node **children = node->args->children;
    int count = node->args->children_count;
    Ucg **args = NULL;
    if (count != 0) {
        args = arena_alloc(count, sizeof(Ucg *));
        for (int i = 0; i < count; i++) {
            Node *param = children[i];
            if (param->token->type == VARIADIC) {
                args[i] = ucg_varg_type();
                continue;
            }
            codegen(param->data_type);
            args[i] = ucg_param(param->token->name, param->data_type->token->ucg);
            param->token->ucg = args[i];
        }
    }
    node->token->ucg = ucg_proto(name, ret_type, args, count);
}

void codegen_structs(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == STRUCT)
            child->token->ucg = ucg_struct(child->token->name);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type == STRUCT)
            codegen_structs(child);
    }
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (child->token->type != STRUCT)
            continue;
        Ucg **attrs = arena_alloc(child->children_count + 1, sizeof(Ucg *));
        int attrs_count = 0;
        for (int j = 0; j < child->children_count; j++) {
            Node *attr = child->children[j];
            if (attr->token->type != DECLARE)
                continue;
            codegen(attr->var->data_type);
            Ucg *type = attr->var->data_type->token->ucg;
            attrs[attrs_count++] = ucg_attr(attr->var->token->name, type);
        }
        ucg_attrs(child->token->ucg, attrs, attrs_count);
    }
}

Ucg *codegen_call_result(Node *fn, Ucg *call) {
    Node *type = ret_type_of(fn);
    if (type->token->type != REF)
        return call;
    Ucg *ref = ucg_var("ref", type->token->ucg);
    ucg_assign(ref, call);
    return ucg_point_to(ref);
}

bool is_struct_value(Node *type) {
    bool is_struct = type && type->token->type == ID && type->declared;
    return is_struct && !same_type(type, string_type());
}

void codegen_write_struct(Ucg *fd, Node *type, Ucg *base) {
    Node *st = type->declared;
    ucg_write(fd, (Ucg *[]){ ucg_string_value(format("%s{", st->token->name)) }, 1);
    bool is_first = true;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != DECLARE)
            continue;
        char *name = child->var->token->name;
        Node *attr_type = child->var->data_type;
        int index = 0;
        find_attr(st, name, &index);
        Ucg *label = ucg_string_value(format("%s%s: ", is_first ? "" : ", ", name));
        Ucg *value = ucg_get_attr(base, index);
        is_first = false;
        if (attr_type->token->type == REF)
            value = ucg_string_value("ref");
        if (is_struct_value(attr_type)) {
            ucg_write(fd, (Ucg *[]){ label }, 1);
            codegen_write_struct(fd, attr_type, value);
        } else
            ucg_write(fd, (Ucg *[]){ label, value }, 2);
    }
    ucg_write(fd, (Ucg *[]){ ucg_string_value("}") }, 1);
}

Ucg **codegen_args(Node *call) {
    Ucg **args = arena_alloc(call->children_count + 1, sizeof(Ucg *));
    for (int i = 0; i < call->children_count; i++) {
        codegen(call->children[i]);
        args[i] = call->children[i]->token->ucg;
    }
    return args;
}

Node *codegen_type_of(Node *arg) {
    Node *type = arg->token->is_type ? arg : type_of(arg);
    codegen(type);
    return type;
}

void codegen_builtin(Node *node) {
    Ucg **args = codegen_args(node);
    Node *arg = node->children_count ? node->children[0] : NULL;
    Ucg *ucg = NULL;
    switch (node->token->type) { // clang-format off
    case READ:   ucg = ucg_read(ucg_i32_value(node->token->num.value)); break;
    case SIZEOF: ucg = ucg_sizeof(codegen_type_of(arg)->token->ucg); break;
    case TYPEOF: ucg = ucg_typeof(codegen_type_of(arg)->token->ucg); break;
    case ALLOC:  ucg = ucg_alloc(ucg_i8_type(), args[0]); break;
    case MEMCPY: ucg = ucg_copy(args[0], args[1], args[2]); break;
    case FREE:   ucg = ucg_free(args[0]); break;
    case ARGC:   ucg = ucg_argc(); break;
    case ARGV:   ucg = ucg_argv(); break; // clang-format on
    case IS_ZERO: {
        bool is_ref = is_ref_variable(arg);
        Node *type = is_ref ? arg->data_type : type_of(arg);
        Ucg *value = is_ref ? arg->declared->token->ucg : args[0];
        codegen(type);
        ucg = ucg_op(EQ, value, ucg_null(type->token->ucg));
        break;
    }
    case DROP: {
        Node *type = codegen_type_of(arg);
        if (needs_drop(type))
            codegen_drop(args[0], type);
        ucg = ucg_assign(args[0], ucg_null(type->token->ucg));
        break;
    }
    default: break;
    }
    node->token->ucg = ucg;
}

void codegen_write(Node *node) {
    Ucg *fd = ucg_i32_value(node->token->num.value);
    Ucg **args = arena_alloc(node->children_count + 1, sizeof(Ucg *));
    int count = 0;
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        Node *type = type_of(child);
        bool is_string = type && type->declared && same_type(type, string_type());
        if (is_string) {
            Node *chars = find_method_named(type->declared, "chars");
            Ucg *self = ucg_ref(codegen_in_variable(child));
            args[count++] = ucg_call(chars->token->ucg, 1, self);
            continue;
        }
        if (!is_struct_value(type)) {
            codegen(child);
            args[count++] = child->token->ucg;
            continue;
        }
        if (count)
            ucg_write(fd, args, count);
        count = 0;
        codegen(type);
        codegen_write_struct(fd, type, codegen_in_variable(child));
    }
    node->token->ucg = ucg_write(fd, args, count);
}

Ucg *codegen_in_variable(Node *node) {
    codegen(node);
    Ucg *ucg = node->token->ucg;
    if (is_locator(ucg))
        return ucg;
    Node *type = type_of(node);
    codegen(type);
    Ucg *tmp = ucg_var("tmp", type->token->ucg);
    ucg_assign(tmp, ucg);
    if (needs_drop(type)) {
        append(ura.scope->temporaries, tmp);
        append(ura.scope->temporary_types, type);
    }
    return tmp;
}

void codegen_method_protos(Node *st) {
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type == STRUCT)
            codegen_method_protos(child);
        if (child->token->type != FN_DEC)
            continue;
        codegen_proto(child, format("%s.%s", st->token->name, child->token->name));
    }
}

void codegen_protos(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(child->token->type, FN_DEC, PROTO, 0))
            codegen_proto(child, child->token->name);
        if (child->token->type == STRUCT)
            codegen_method_protos(child);
    }
}

void codegen_statements(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(
                child->token->type,
                FN_DEC,
                PROTO,
                DECLARE,
                STRUCT,
                LINK,
                0
            ))
            continue;
        bool is_assign = child->token->type == ASSIGN;
        bool is_global = is_assign && child->left->token->type == VAR;
        if (is_global && is_unused_global(child->left->token))
            continue;
        codegen_statement(child);
    }
}

void codegen(Node *node) {
    if (is_operation(node->token->type)) {
        codegen_operation(node);
        return;
    }
    Ucg *ucg = NULL;
    switch (node->token->type) {
    case VAR:
    case PARAM: {
        if (node->declared)
            node->token->ucg = node->declared->token->ucg;
        if (node->declared && is_ref_variable(node))
            node->token->ucg = ucg_point_to(node->token->ucg);
        break;
    }

    case INT: {
        if (node->token->is_type && node->token->is_uint)
            ucg = ucg_uint_type(node->token->shift);
        else if (node->token->is_type)
            ucg = ucg_int_type(node->token->shift);
        else
            ucg = ucg_int_value(node->token->shift, node->token->num.value);

        node->token->ucg = ucg;
        break;
    }
    case VOID: {
        node->token->ucg = ucg_void_type();
        break;
    }
    case FLOAT: {
        if (node->token->is_type)
            ucg = ucg_float_type(node->token->shift);
        else
            ucg = ucg_float_value(node->token->shift, node->token->num.real);
        node->token->ucg = ucg;
        break;
    }
    case REF:
    case ARRAY: {
        if (node->token->is_type) {
            codegen(node->data_type);
            node->token->ucg = ucg_ref_type(node->data_type->token->ucg);
        } else {
            Ucg *place = codegen_in_variable(node->operand);
            Node *type = type_of(node->operand);
            bool is_ref = type && type->token->type == REF;
            node->token->ucg = is_ref ? place : ucg_ref(place);
        }
        break;
    }
    case COPY: {
        Node *type = type_of(node);
        codegen(type);
        Ucg *copy = ucg_var("copy", type->token->ucg);
        Ucg *args[] = {
            ucg_ref(copy),
            ucg_ref(codegen_in_variable(node->to_copy)),
        };
        ucg_fn_call(node->to_call->token->ucg, args, 2);
        node->token->ucg = copy;
        break;
    }
    case CHARS: {
        char *text = node->token->chars.value;
        Node *type = type_of(node);
        node->token->ucg = ucg_string_value(text);
        if (type->token->type == ARRAY)
            break;
        codegen(type);
        node->token->ucg = ucg_struct_value(
            type->token->ucg,
            (Ucg *[]){
                node->token->ucg,
                ucg_i32_value(strlen(text)),
                ucg_i32_value(0),
            },
            3
        );
        break;
    }
    case DECLARE: {
        codegen(node->var->data_type); // data type

        char *name = node->var->token->name;
        Ucg *type = node->var->data_type->token->ucg;
        if (ura.scope->token->type == SCOPE)
            ucg = ucg_global(name, type);
        else {
            ucg = ucg_var(name, type);
            codegen_clean(ucg, node->var->data_type);
        }
        node->var->token->ucg = ucg;
        break;
    }
    case SCOPE: {
        if (ura.throws)
            ucg_throws();
        enter_scope(node);
        codegen_structs(node);
        codegen_protos(node);
        bool has_main = false;
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            bool is_declare = child->token->type == DECLARE;
            if (is_declare && !is_unused_global(child->var->token))
                codegen(child);
            if (child->token->type == LINK)
                ucg_link(child->token->chars.value);
            if (child->token->type == FN_DEC && strcmp2(child->token->name, "main"))
                has_main = true;
        }
        if (!has_main) {
            Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
            codegen_statements(node);
            codegen_global_drops(node);
            ucg_end(fn_main);
        }
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (includes(child->token->type, FN_DEC, STRUCT, 0))
                codegen(child);
        }
        exit_scope();
        break;
    }
    case FN_DEC: {
        Node *parent = ura.scope;
        enter_scope(node);
        ucg_entry(node->token->ucg);
        for (int i = 0; i < node->args->children_count; i++) {
            Node *param = node->args->children[i];
            if (param->token->type == PARAM)
                codegen_clean(param->token->ucg, param->data_type);
        }
        bool is_main =
            parent->token->type == SCOPE && strcmp2(node->token->name, "main");
        Ucg *try_stmt = is_main && ura.throws ? ucg_try() : NULL;
        if (is_main) {
            codegen_statements(parent);
            codegen_global_drops(parent);
        }
        codegen_block(node, 0);
        if (try_stmt)
            codegen_uncaught(try_stmt);
        ucg_end(node->token->ucg);
        exit_scope();
        break;
    }
    case STRUCT: {
        enter_scope(node);
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (includes(child->token->type, FN_DEC, STRUCT, 0))
                codegen(child);
        }
        exit_scope();
        break;
    }
    case FN_CALL: {
        Ucg **args = codegen_args(node);
        Ucg *call = ucg_fn_call(node->to_call->token->ucg, args, node->children_count);
        node->token->ucg = codegen_call_result(node->to_call, call);
        break;
    }
    case WRITE: {
        codegen_write(node);
        break;
    } // clang-format off
    case READ: case SIZEOF: case TYPEOF: case DROP: case IS_ZERO:
    case ALLOC: case MEMCPY: case FREE: case ARGC: case ARGV: { // clang-format on
        codegen_builtin(node);
        break;
    }
    case RETURN: {
        if (node->left) {
            codegen(node->left);
            bool is_local = is_local_variable(node->left);
            Ucg *value = node->left->token->ucg;
            if (!is_local)
                value = ucg_value(value);
            codegen_drop_temporaries();
            ucg = ucg_ret(value);
        } else
            ucg = ucg_return();
        node->token->ucg = ucg;
        break;
    }
    case IF: {
        Ucg *if_stmt = ucg_if();
        codegen_branch(node, if_stmt);
        for (Node *branch = node->next; branch; branch = branch->next) {
            Ucg *stmt =
                branch->token->type == ELIF ? ucg_elif(if_stmt) : ucg_else(if_stmt);
            codegen_branch(branch, stmt);
        }
        ucg_end(if_stmt);
        break;
    }
    case LOOP: {
        Ucg *loop = ucg_loop();
        codegen_branch(node, loop);
        ucg_end(loop);
        break;
    }
    case FOR: {
        Node *type = type_of(node->right);
        Node *in = find_method_named(type->declared, "in");
        Ucg *iter = codegen_in_variable(node->right);
        Ucg *counter = ucg_var("i", ucg_i32_type());
        ucg_assign(counter, ucg_i32_value(0));
        enter_scope(node);
        codegen(node->children[0]);
        Ucg *loop = ucg_loop();
        node->token->ucg = loop;
        Ucg *args[] = {
            ucg_ref(iter),
            ucg_ref(counter),
            ucg_ref(node->var->token->ucg),
        };
        ucg_then(loop, ucg_fn_call(in->token->ucg, args, 3));
        codegen_block(node, 1);
        exit_scope();
        ucg_end(loop);
        break;
    }
    case MATCH: {
        enter_scope(node);
        codegen(node->children[0]);
        codegen_statement(node->left);
        Ucg *if_stmt = NULL;
        for (int i = 1; i < node->children_count; i++) {
            Node *branch = node->children[i];
            Ucg *stmt = NULL;
            if (if_stmt == NULL)
                stmt = if_stmt = node->token->ucg = ucg_if();
            else
                stmt = branch->cond ? ucg_elif(if_stmt) : ucg_else(if_stmt);
            codegen_branch(branch, stmt);
        }
        if (if_stmt)
            ucg_end(if_stmt);
        exit_scope();
        break;
    }
    case TRY: {
        codegen_try(node);
        break;
    }
    case THROW: {
        codegen(node->left);
        Ucg *value = ucg_value(node->left->token->ucg);
        codegen_drop_temporaries();
        node->token->ucg = ucg_throw(value);
        break;
    }
    case BRK: {
        ucg_brk(node->loop->token->ucg);
        break;
    } // clang-format off
    case TEMPLATE: case USE: case MACRO_IF: 
    case MACRO_ERROR: case ENUM: case MODULE: { // clang-format on
        break;
    }
    case ID: {
        node->token->ucg = node->declared->token->ucg;
        break;
    }
    case ZERO: {
        codegen(node->data_type);
        node->token->ucg = ucg_null(node->data_type->token->ucg);
        break;
    }
    case LBRACK: {
        Node *type = node->data_type;
        Node *elem = element_type_of(type);
        codegen(elem);
        Ucg **items = arena_alloc(node->children_count + 1, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            codegen(node->children[i]);
            items[i] = node->children[i]->token->ucg;
        }
        Ucg *data = ucg_array_from(elem->token->ucg, items, node->children_count);
        if (type->token->type == ARRAY) {
            node->token->ucg = data;
            break;
        }
        codegen(type);
        Ucg *fields[] = {
            data,
            ucg_i32_value(node->children_count),
            ucg_i32_value(0),
        };
        Ucg *array = ucg_var("array", type->token->ucg);
        ucg_assign(array, ucg_struct_value(type->token->ucg, fields, 3));
        Node *own = find_method_named(type->declared, "own");
        ucg_call(own->token->ucg, 1, ucg_ref(array));
        node->token->ucg = array;
        break;
    }
    case LCBRACK: {
        codegen(node->data_type);
        Node *st = node->data_type->declared;
        int values_count = st->children_count + 1;
        Ucg **values = arena_alloc(values_count, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            Node *attr = node->children[i];
            codegen(attr->left);
            values[attr->token->attr.index] = attr->left->token->ucg;
        }
        node->token->ucg =
            ucg_struct_value(node->data_type->token->ucg, values, values_count);
        break;
    }
    case ACCESS: {
        codegen(node->left);
        codegen(node->index);
        node->token->ucg = ucg_access(node->left->token->ucg, node->index->token->ucg);
        break;
    }
    case DOT: {
        Ucg *base = codegen_in_variable(node->left);
        Node *left_type = type_of(node->left);
        if (left_type && left_type->token->type == REF)
            base = ucg_point_to(base);
        if (node->right->token->type == FN_CALL) {
            Node *call = node->right;
            Ucg **args = arena_alloc(call->children_count + 2, sizeof(Ucg *));
            args[0] = ucg_ref(base);
            for (int i = 0; i < call->children_count; i++) {
                codegen(call->children[i]);
                args[i + 1] = call->children[i]->token->ucg;
            }
            Ucg *method = call->to_call->token->ucg;
            node->token->ucg = codegen_call_result(
                call->to_call, ucg_fn_call(method, args, call->children_count + 1)
            );
            break;
        }
        int index = node->right->token->attr.index;
        node->token->ucg = ucg_get_attr(base, index);
        break;
    }
    case DOUBLE_DOTS: {
        Node *call = node->right;
        Ucg **args = codegen_args(call);
        node->token->ucg = codegen_call_result(
            call->to_call,
            ucg_fn_call(call->to_call->token->ucg, args, call->children_count)
        );
        break;
    }
    case AS: {
        codegen(node->left);
        codegen(node->to_type);
        node->token->ucg = ucg_cast(node->left->token->ucg, node->to_type->token->ucg);
        break;
    }
    case NEG: {
        codegen(node->operand);
        node->token->ucg = ucg_neg(node->operand->token->ucg);
        break;
    }
    case BNOT: {
        codegen(node->operand);
        Node *type = type_of(node->operand);
        Ucg *ones = ucg_int_value(type->token->shift, -1);
        node->token->ucg = ucg_op(BXOR, node->operand->token->ucg, ones);
        break;
    }
    case NOT: {
        codegen(node->operand);
        node->token->ucg = ucg_not(node->operand->token->ucg);
        break;
    }
    case CNT: {
        ucg_cnt(node->loop->token->ucg);
        break;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, node->token->type);
        exit(1);
        break;
    }
    }
}

void codegen_operation(Node *node) {
    if (is_ref_binding(node)) {
        Node *slot = node->left->declared ? node->left->declared : node->left;
        codegen(node->right);
        Ucg *address = node->right->token->ucg;
        if (!includes(node->right->token->type, REF, ZERO, 0))
            address = ucg_ref(address);
        node->token->ucg = ucg_assign(slot->token->ucg, address);
        return;
    }
    codegen(node->left);
    Node *type = type_of(node->left);
    bool is_var = node->left->token->type == VAR;
    bool is_declaration = is_var && node->left->declared == NULL;
    bool is_literal = node->right->token->type == CHARS;
    if (is_declaration && is_literal && is_chars(type))
        node->right->token->ucg = ucg_string_array(node->right->token->chars.value);
    else
        codegen(node->right);
    if (node->token->type == ASSIGN && !is_declaration && needs_drop(type)) {
        Ucg *value = ucg_value(node->right->token->ucg);
        codegen_drop(node->left->token->ucg, type);
        node->token->ucg = ucg_assign(node->left->token->ucg, value);
        return;
    }
    Ucg *left = node->left->token->ucg;
    Ucg *right = node->right->token->ucg;
    bool is_null_compare = node->right->token->type == ZERO;
    if (is_null_compare && is_ref_variable(node->left))
        left = node->left->declared->token->ucg;
    node->token->ucg = ucg_op(node->token->type, left, right);
}


int main(int ac, char **av) {
    atexit(arena_free);
    arena_reserve(1000);
    ucg_bin_path("build/exe.out");
    bool run = parse_args(ac, av);
    int code = 0;
    ura.tokens_count = 0;
    ura.exe_pos = 0;

#if TOKENIZE
    print(GREEN("======================================\n"));
    print(GREEN("[TOKENIZE]:\n"));
    new_token(SCOPE, NULL)->name = "ura-scope";
    for (int i = 0; i < ura.files_count; i++) {
        File *file = ura.files[i];
        if (file->opened)
            continue;
        file->start = end_of_sources();
        open_file(file);
        arena_reserve(file->len * 192);
        tokenize(file);
    }
    new_token(LAST, NULL)->space = -TAB;

    ucg_check_errors();

    print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tokens_count; i++)
        print(CYAN("token ") "%k\n", ura.tokens[i]);

#    if AST
    print(GREEN("======================================\n"));
    print(GREEN("[AST]:\n"));
    ura.tree = parse_expr(0);
    ucg_check_errors();
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#        if ANALYZE
    print(GREEN("======================================\n"));
    print(GREEN("[ANALYZE]:\n"));
    analyze(ura.tree);
    ucg_check_errors();
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#            if ASM
    print(GREEN("======================================\n"));
    print(GREEN("[CODE GEN]:\n"));
    ucg_start("ura");
    codegen(ura.tree);
    print(GREEN("======================================\n"));
    print(GREEN("[URA IR]:\n"));
    print_ucgs();
    ucg_compile();
    ucg_gen();
    if (DEBUG || run)
        code = ucg_run_bin();
    ucg_finish();
#            endif // ASM
#        endif     // ANALYZE
#    endif         // AST
#endif             // TOKENIZE
    print("did allocate %zu\n", arena_size());
    return code;
}

// UTILS / FORMATING / PUTING
int _print_type(ioFile fp, Node *node) {
    // clang-format off
    if (node == NULL) return fprintf(fp, "void");
    switch(node->token->type) {
    case REF:		return fprintf(fp, "&") + _print_type(fp, node->left);
    case STRUCT:	return fprintf(fp, "%s", node->token->name);
    case ARRAY:		return _print_type(fp, node->left) + fprintf(fp, "[]");
    default:		return fprintf(fp, "%s", to_string(node->token->type));
    }
    // clang-format on
};

int _print_char(ioFile fp, char c) {
    unsigned char u = (unsigned char)c;
    if (iscntrl(u) && escaped[u])
        return fprintf(fp, "%s", escaped[u]);
    return fprintf(fp, "%c", c);
}

// clang-format off
int _print_token(ioFile fp, Token *token) {
    if (token == NULL) return fprintf(fp, "(null token)");
    int r = fprintf(fp, "%s", to_string(token->type));
    if (token->name != NULL) r += fprintf(fp, " name (%s)", token->name);
    if (token->name == NULL && !token->is_type) { 
        switch(token->type) {
        // case I8:    r += fprintf(fp, " value (%c)", token->i8.value); break;
        case INT:   r += fprintf(fp, " value (%lld)", token->num.value); break;
        case CHARS: {
            #if 1
            r += fprintf(fp, " value (");
            char *value = token->chars.value;
            for(int i = 0; value && value[i]; i++)
                r += _print_char(fp, value[i]);
            r += fprintf(fp, ")"); 
            #else
            r += fprintf(fp, " value (%s)", token->chars.value);
            #endif
            break;
        }
        // case BOOL: {
        //     char *b1 = token->b1.value ? "True" : "False";
        //     r += fprintf(fp, " value (%s)", b1);
        //     break;
        // }
        default: break;
        }
    } 
    r += fprintf(fp, " space (%d)", token->space);
    return 0;
}

void _print_node_helper(NodePrint *elems, Node *node, int depth) {
    if (node == NULL)
        return;

    append(elems->nodes, node);
    append(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, FN_CALL, TEMPLATE, 0)) {
        Node *left = node->left;
        bool is_struct = left && left->token->type == STRUCT;
        if (is_struct) {
            append(elems->nodes, left);
            append(elems->depths, depth + 3);
            // print(">>>> %n\n", left);
            // exit(1);
            // if (left->left) {
            //     append(elems->nodes, left->left);
            //     append(elems->depths, depth + 3);
            // }
        } else
            _print_node_helper(elems, left, depth + 3);
    }
    if (!includes(node->token->type, ID, VAR, PARAM, 0))
        _print_node_helper(elems, node->right, depth + 3);
    for (size_t i = 0; i < node->children_count; i++)
        _print_node_helper(elems, node->children[i], depth + 3);
}

bool is_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++) {
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    }
    return 0;
}

int _print_node(ioFile fp, Node *node) {
    NodePrint elems = {};
    int r = 0;
    _print_node_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            r += fprintf(fp, "%s", is_open(&elems, i, level) ? "│ " : " ");
        if (depth)
            r += fprintf(fp, "%s", is_open(&elems, i, depth) ? "├─" : "└─");
        r += _print_token(fp, elems.nodes[i]->token) + fprintf(fp, "\n");
    }
    return 0;
}

int _print(ioFile fp, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = 0;
	if(fp == stderr)
		r += fprintf(stderr, RED("error: "));
	r += _fprint(fp, fmt, args);
    va_end(args);
    return r;
}

int _fprint(ioFile fp, const char *fmt, va_list ap) {
    int r = 0;
    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            continue;
        }
        if (fmt[++i] == '\0')
            break;
        Token *token = NULL;
        switch (fmt[i]) { // clang-format off
        case 's': r += fprintf(fp, "%s", va_arg(ap, char *)); break;
        case 'd': r += fprintf(fp, "%d", va_arg(ap, int)); break;
        case 'c': r += fprintf(fp, "%c", va_arg(ap, int)); break;
        case 'f': r += fprintf(fp, "%f", va_arg(ap, double)); break;
        case 'i': r += fprintf(fp, "%*s", va_arg(ap, int), ""); break;
        case '%': r += fprintf(fp, "%%"); break;
        case 't': r += fprintf(fp, "%s", to_string(va_arg(ap, Type))); break;
        case 'k': r += _print_token(fp, va_arg(ap, Token *)); break;
        case 'n': r += _print_node(fp, va_arg(ap, Node *)); break;
        case 'N': r += _print_type(fp, va_arg(ap, Node *)); break;
        case 'z': {
            r += fprintf(fp, "%zu", va_arg(ap, size_t));
            i++;
            break;
        }
        case '.': {
            int n = va_arg(ap, int);
            char *str = va_arg(ap, char *);
            r += fprintf(fp, "%.*s", n, str);
            i += 2; // skip *s
            break;
        } 
        default: {
			eprint("%s:%d handle this case <%%%c>\n", FILE, LINE, fmt[i]);
			break; // clang-format on
        }
        }
    }
    return r;
}

__attribute__((sentinel)) bool includes(Type to_find, ...) {
    va_list ap;
    va_start(ap, to_find);
    Type curr = va_arg(ap, Type);
    while (curr) {
        if (curr == to_find)
            return true;
        curr = va_arg(ap, Type);
    }
    return false;
}

void print_help(char *ura) {
    help("usage:\n");
    help("  %s <file>.ura [flags]    compile files\n", ura);
    help("  %s init <name>           create a project in a new folder\n", ura);
    help("  %s build                 build the project in this folder\n", ura);
    help("  %s run                   build the project and run it\n", ura);
    help("  %s install               install the dependencies of ura.conf\n", ura);
    help("  %s install <name> <version> <where>\n", ura);
    help("                           add or update one; where: a git repo url or ./folder\n");
    help("  %s remove <name>         remove a dependency and its folder\n", ura);
    help("  %s list                  show the dependencies and where they are\n", ura);
    help("  %s <script>              run a @script of ura.conf\n", ura);
    help("  %s -version              print the ura and LLVM versions\n", ura);
    help("  %s -help                 show this help\n", ura);
    help("flags:\n");
    help("  -o <exe>                 link an executable\n");
    help("  -ll [path]               write the LLVM IR (build/out.ll)\n");
    help("  -S [path]                write the assembly (build/out.s)\n");
    help("  -c [path]                write the object file (build/out.o)\n");
}

char *parse_flag_path(int ac, char **av, int *i, char *fallback) {
    char *next = *i + 1 < ac ? av[*i + 1] : NULL;
    size_t n = next ? strlen(next) : 0;
    bool is_path = next && next[0] != '-' && !(n > 4 && strcmp2(next + n - 4, ".ura"));
    if (!is_path)
        return fallback;
    (*i)++;
    return next;
}

bool parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        print_help(av[0]);
        exit(1);
    }
    if (strcmp2(av[1], "-help") || strcmp2(av[1], "-h")) {
        print_help(av[0]);
        exit(0);
    }
    if (strcmp2(av[1], "-version")) {
        help("ura %s (LLVM %s)\n", URA_VERSION, LLVM_VERSION_STRING);
        exit(0);
    }
    if (strcmp2(av[1], "init")) {
        if (ac < 3) {
            eprint("expected a project name:\n");
            help("%s init <name>\n", av[0]);
            exit(1);
        }
        init_project(av[2]);
        exit(0);
    }
    char *project = find_folder_with(getcwd2(), "ura.conf");
    bool install = strcmp2(av[1], "install");
    if (install && ac == 2) {
        install_project(find_project(), 0);
        exit(0);
    }
    if (install) {
        if (ac != 5) {
            eprint("expected a name, a version and where to get it:\n");
            help("%s install <name> <version> <https://github.com/user/repo | ./folder>\n", av[0]);
            exit(1);
        }
        add_package(find_project(), av[2], av[3], av[4]);
        exit(0);
    }
    if (strcmp2(av[1], "remove")) {
        if (ac < 3) {
            eprint("expected a dependency name:\n");
            help("%s remove <name>\n", av[0]);
            exit(1);
        }
        for (int i = 2; i < ac; i++)
            remove_dependency(find_project(), av[i]);
        exit(0);
    }
    if (strcmp2(av[1], "list")) {
        list_project(find_project(), "", 0);
        exit(0);
    }
    run_script(av[1]);
    bool run = strcmp2(av[1], "run");
    if (run || strcmp2(av[1], "build")) {
        build_project(find_project());
        return run;
    }
    if (project)
        load_project(project);
    bool has_exe = false;
    bool no_link = false;
    for (int i = 1; i < ac && !ucg_has_errors(); i++) {
        char *arg = av[i];
        if (strcmp2(arg, "-o")) {
            if (i + 1 >= ac) {
                eprint("expected argument:\n");
                help("-o exe.out\n");
                exit(1);
            }
            ucg_bin_path(av[++i]);
            has_exe = true;
        } else if (strcmp2(arg, "-ll")) {
            ucg_path(parse_flag_path(ac, av, &i, "build/out.ll"));
        } else if (strcmp2(arg, "-S")) {
            ucg_asm_path(parse_flag_path(ac, av, &i, "build/out.s"));
            no_link = true;
        } else if (strcmp2(arg, "-c")) {
            ucg_obj_path(parse_flag_path(ac, av, &i, "build/out.o"));
            no_link = true;
        } else {
            size_t n = strlen(arg);
            bool is_ura = n > 4 && strcmp2(arg + n - 4, ".ura");
            if (!is_ura) {
                eprint("Invalid file '%s'\n", arg);
                help("use %s.ura\n", arg);
                exit(1);
            }
            File *file = new_file(arg);
            append(ura.files, file);
        }
    }
    if (no_link && !has_exe)
        ucg_bin_path(NULL);
    return false;
}

char *parse_escaped(char *input, int s, int e) {
    char *res = arena_alloc(e - s + 1, sizeof(char));
    int r = 0;
    while (s < e) {
        int c = -1;
        if (s < e - 1 && input[s] == '\\') {
            char *str = escaped[(unsigned char)input[s + 1]];
            if (str != NULL) {
                c = str[0];
                s += 2;
            }
        }
        if (c == -1)
            c = (unsigned char)input[s++];
        res[r++] = c;
    }
    return res;
}
