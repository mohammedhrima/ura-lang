// TYPEDEFS
typedef struct Token Token;
typedef struct Node Node;
typedef struct NodePrint NodePrint;
typedef struct uraError uraError;

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

struct uraError {
    Node *file;
    Token *token;
    int s;
    int e;
    int line;
    char *msg;
    char *help;
};

struct NodePrint {
    expand(int, depths);
    expand(Node *, nodes);
};

// Global Ura
struct {
    expand(File *, files);
    File *file;
    int line;

    expand(Token *, tokens);

    Node *tree;

    expand(Node *, scopes);
    Node *scope;
    Node *types;

    expand(uraError, errors);

    int exe_pos;
} ura;

struct {
    char *value;
    Token token;
} keywords[] = {
    { "bool", { .type = INT, .is_type = true, .shift = 1 } },
    { "i1", { .type = INT, .is_type = true, .shift = 1 } },
    { "void", { .type = VOID, .is_type = true } },
    { "f32", { .type = FLOAT, .is_type = true, .shift = 32 } },
    { "f64", { .type = FLOAT, .is_type = true, .shift = 64 } },
    { "char", { .type = INT, .is_type = true, .shift = 8 } },
    { "i8", { .type = INT, .is_type = true, .shift = 8 } },
    { "i16", { .type = INT, .is_type = true, .shift = 16 } },
    { "i32", { .type = INT, .is_type = true, .shift = 32 } },
    { "i64", { .type = INT, .is_type = true, .shift = 64 } },
    { "u8", { .type = INT, .is_type = true, .shift = 8, .is_uint = true } },
    { "u16", { .type = INT, .is_type = true, .shift = 16, .is_uint = true } },
    { "u32", { .type = INT, .is_type = true, .shift = 32, .is_uint = true } },
    { "u64", { .type = INT, .is_type = true, .shift = 64, .is_uint = true } },
    { "num", { .type = INT, .is_type = true } },

    { "True", { .type = INT, .shift = 1, .num = { .value = true } } },
    { "False", { .type = INT, .shift = 1, .num = { .value = false } } },

    { "struct", { .type = STRUCT } },
    { "enum", { .type = ENUM } },
    { "and", { .type = AND } },
    { "or", { .type = OR } },
    { "not", { .type = NOT } },
    { "proto", { .type = PROTO } },
    { "link", { .type = LINK } },
    { "use", { .type = USE } },
    { "@if", { .type = MACRO_IF } },
    { "@elif", { .type = MACRO_ELIF } },
    { "@else", { .type = MACRO_ELSE } },
    { "@error", { .type = MACRO_ERROR } },
    { "operator", { .type = OPERATOR } },
    { "pub", { .type = PUB } },
    { "fn", { .type = FN_DEC } },
    { "return", { .type = RETURN } },
    { "if", { .type = IF } },
    { "elif", { .type = ELIF } },
    { "else", { .type = ELSE } },
    { "while", { .type = LOOP } },
    { "break", { .type = BRK } },
    { "continue", { .type = CNT } },
    { "match", { .type = MATCH } },
    { "case", { .type = CASE } },
    { "default", { .type = DEFAULT } },
    { "template", { .type = TEMPLATE } },
    // { "null", {.type = ZERO } },
    { "as", { .type = AS } },

    { NULL },
};

struct {
    char *value;
    Type type;
} specials[] = {
    { "(", LPARENT },     { ")", RPARENT },     { "::", DOUBLE_DOTS },
    { ":", DOTS },        { "[", LBRACK },      { "]", RBRACK },
    { "{", LCBRACK },     { "}", RCBRACK },

    { "+=", ADD_ASSIGN }, { "-=", SUB_ASSIGN }, { "*=", MUL_ASSIGN },
    { "/=", DIV_ASSIGN }, { "%=", MOD_ASSIGN },
    { "<<=", LSHIFT_ASSIGN }, { ">>=", RSHIFT_ASSIGN }, { "&=", BAND_ASSIGN },
    { "|=", BOR_ASSIGN }, { "^=", BXOR_ASSIGN },
    { "<<", LSHIFT },     { ">>", RSHIFT },

    { "...", VARIADIC },  { ".", DOT },

    { "+", ADD },         { "-", SUB },         { "*", MUL },
    { "/", DIV },         { "%", MOD },

    { ">=", GE },         { "<=", LE },         { ">", GT },
    { "<", LT },          { "==", EQ },         { "!=", NQ },
    { "!", NOT },

    { "=", ASSIGN },      { ",", COMA },        { "&&", AND },
    { "||", OR },         { "&", REF },         { "|", BOR },
    { "^", BXOR },        { NULL, NONE },
};

char *binary_ops[END + 1] = {
    [ADD] = "+",        [SUB] = "-",        [MUL] = "*",
    [DIV] = "/",        [MOD] = "%",

    [ADD_ASSIGN] = "+", [SUB_ASSIGN] = "-", [MUL_ASSIGN] = "*",
    [DIV_ASSIGN] = "/", [MOD_ASSIGN] = "%",

    [BAND] = "&",       [BOR] = "|",        [BXOR] = "^",
    [LSHIFT] = "<<",    [RSHIFT] = ">>",

    [BAND_ASSIGN] = "&", [BOR_ASSIGN] = "|", [BXOR_ASSIGN] = "^",
    [LSHIFT_ASSIGN] = "<<", [RSHIFT_ASSIGN] = ">>",

    [EQ] = "==",        [NQ] = "!=",        [LT] = "<",
    [GT] = ">",         [LE] = "<=",        [GE] = ">=",

    [AND] = "and",      [OR] = "or",
};

int precedence[END + 1] = {
    [ASSIGN] = 1,     [ADD_ASSIGN] = 1,   [SUB_ASSIGN] = 1,
    [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1,   [MOD_ASSIGN] = 1,
    [BAND_ASSIGN] = 1, [BOR_ASSIGN] = 1,  [BXOR_ASSIGN] = 1,
    [LSHIFT_ASSIGN] = 1, [RSHIFT_ASSIGN] = 1,

    [OR] = 11,        [AND] = 12,

    [EQ] = 21,        [NQ] = 21,          [GT] = 22,
    [LT] = 22,        [GE] = 22,          [LE] = 22,

    [BOR] = 23,       [BXOR] = 24,        [BAND] = 25,
    [LSHIFT] = 28,    [RSHIFT] = 28,

    [ADD] = 30,       [SUB] = 30,         [MUL] = 31,
    [DIV] = 31,       [MOD] = 31,

    [AS] = 35,

    [DOT] = 51,       [DOUBLE_DOTS] = 51, [LBRACK] = 51,
};

char *escaped[256] = {
    ['n'] = "\n",   ['t'] = "\t",   ['r'] = "\r",   ['b'] = "\b",
    ['f'] = "\f",   ['v'] = "\v",   ['a'] = "\a",   ['?'] = "\?",
    ['0'] = "\0",   ['"'] = "\"",   ['\''] = "\'",  ['\\'] = "\\",

    ['\n'] = "\\n", ['\t'] = "\\t", ['\r'] = "\\r", ['\0'] = "\\0",
};

char *type_names[END + 1] = {
    [VOID] = "void",
    // [BOOL] = "bool",
    // [I8] = "i8",
    // [I32] = "num",
};

#include "./check.c"
#include "./package.c"

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

    new->num.value = strtol(buff + s, NULL, 10);
    new->shift = 32;
    if (memchr(buff + s, '.', e - s)) {
        new->type = FLOAT;
        new->num.real = strtod(buff + s, NULL);
        new->shift = 64;
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

    char *str = parse_escaped(buff, s + 1, e - 1);
    // TODO: assert_char
    new->num.value = str[0];

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
    int first_error = ura.errors_count;
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
        while (strncmp2(buff + s, "//", 2) && !strchr2("\n", buff[e]))
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
            if (buff[e] == '\0') {
                add_error(s, s + 2, "unclosed comment", "add */ to close it");
                break;
            }
            e += 2;
            continue;
        }

        if (buff[s] != '\0' && strchr2("\"\'", buff[s])) {
            char c = buff[s];
            e++;
            bool valid = true;
            int letters = 0;
            while (buff[e] != '\0' && buff[e] != '\n' && buff[e] != c) {
                char next = buff[e + 1];
                if (buff[e] == '\\' && next != '\0' && next != '\n') {
                    if (!isprint(next) || !escaped[(unsigned char)next]) {
                        add_error(
                            e,
                            e + 2,
                            format("unknown escape '\\%c'", next),
                            "the escapes are \\n \\t \\r \\0 \\\\ \\\" \\'"
                        );
                        valid = false;
                    }
                    e++;
                }
                letters++;
                e++;
            }
            if (buff[e] != c) {
                int start = line_start_in(buff, s);
                add_error(
                    s,
                    e,
                    c == '"' ? "unclosed string" : "unclosed character",
                    format("use %.*s%c", e - start, buff + start, c)
                );
                continue;
            }
            e++; // skip last ' or "
            if (!valid)
                continue;
            if (c == '"')
                parse_string(s, e, space);
            else if (letters > 1)
                add_error(
                    s,
                    e,
                    format("a character holds one letter, not %d", letters),
                    format("use \"%.*s\" for a string", e - s - 2, buff + s + 1)
                );
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
        }
        if (e != s) {
            // TODO:
            // if (!check_number_fits(s, e, line))
            //     continue;
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
            Type type = specials[i].type;
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
        add_error(s, s + 1, format("unknown character '%c'", buff[s]), "remove it");
        e++;
    }
    for (int i = first_token; i < ura.tokens_count; i++) {
        ura.tokens[i]->s += file->start;
        ura.tokens[i]->e += file->start;
    }
    for (int i = first_error; i < ura.errors_count; i++) {
        ura.errors[i].s += file->start;
        ura.errors[i].e += file->start;
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

char *token_text(Token *token) {
    return format("%.*s", token->e - token->s, text_at(token->s));
}

int line_end(int pos) {
    File *file = file_at(pos);
    return file->start + line_end_in(file->buff, pos - file->start);
}

void expect(Type type, char *msg, char *fix) {
    if (peek(0)->type == type) {
        next();
        return;
    }
    Token *prev = ura.tokens[ura.exe_pos - 1];
    Token *at = peek(0)->line == prev->line ? peek(0) : prev;
    int start = line_start(prev->s);
    int end = line_end(prev->e);
    add_error(
        at->s,
        at->e,
        msg,
        format(
            "use %.*s%s%.*s",
            prev->e - start,
            text_at(start),
            fix,
            end - prev->e,
            text_at(prev->e)
        )
    );
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
            bool is_same = other->args->children_count == method->args->children_count;
            for (int k = 0; is_same && k < method->args->children_count; k++) {
                Node *left = method->args->children[k]->data_type;
                Node *right = other->args->children[k]->data_type;
                bool has_types = left && right;
                is_same = has_types ? same_type(left, right) : left == right;
            }
            if (!is_same)
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

    Node *params = template->args;
    if (types->children_count != params->children_count) {
        char *declared = "";
        for (int i = 0; i < params->children_count; i++)
            declared = format(
                "%s%s%s", declared, i ? ", " : "", params->children[i]->token->name
            );
        add_error(
            token->s,
            token->e,
            format(
                "'%s' takes %d type argument%s, got %d",
                token->name,
                params->children_count,
                params->children_count == 1 ? "" : "s",
                types->children_count
            ),
            format("it is declared 'template<%s>'", declared)
        );
        return name;
    }
    if (find_struct(name) || find_function(name))
        return name;

    Token *body = template->token;
    int start = copy_tokens(body->body.start, body->body.end, token->name, name, 0);
    types->declared = template;
    parse_tokens(template->scope, start, types);
    return name;
}

Node *parse_type(void) {
    Token *token = peek(0);
    Token *prev = ura.tokens[ura.exe_pos - 1];
    bool is_ref = token->type == REF && token->line == prev->line;
    Token *after = is_ref ? peek(1) : token;
    bool is_value_after = after->type == ID ?
                          find_variable(after->name) != NULL :
                          !after->is_type;
    if (is_ref && !is_value_after) {
        next(); // SKIP &
        Node *ref = new_node(token);
        ref->token->is_type = true;
        ref->data_type = parse_type();
        return ref;
    }
    if (token->type == ID && token->line == prev->line)
        token->is_type = true;
    if (token->is_type) {
        next();
        Node *node = token->type == ID ? find_template_type(token->name) : NULL;
        Node *template = token->type == ID ? find_template(token->name) : NULL;
        if (node == NULL)
            node = new_node(token);
        if (template && peek(0)->type != LT)
            add_error(
                token->s,
                token->e,
                format("expected '<' after '%s'", token->name),
                NULL
            );
        else if (template)
            token->name = init_from_template(template, parse_types(token));
        else if (peek(0)->type == LT) {
            add_error(
                token->s,
                token->e,
                format("no template named '%s'", token->name),
                format("declare it: template<T>: struct %s:", token->name)
            );
            parse_types(token);
        }
        while (peek(0)->type == LBRACK && peek(1)->type == RBRACK) {
            next(); // SKIP [
            next(); // SKIP ]
            Node *array = new_node(new_token(ARRAY, token));
            array->token->is_type = true;
            array->data_type = node;
            node = array;
        }
        return node;
    }
    return NULL;
}

Node *new_builtin(Type type, char *name, int fd, Node *ret_type) {
    Node *node = new_node(new_token(type, NULL));
    node->token->name = name;
    node->token->num.value = fd;
    node->ret_type = ret_type;
    return node;
}

Node *parse_block(Token *token) {
    Node *node = new_node(token);
    enter_scope(node);
    if (token->type != ELSE && peek(0)->type == DOTS)
        add_error(
            token->s,
            token->e,
            format("expected a condition after '%s'", token_text(token)),
            NULL
        );
    else if (token->type != ELSE)
        node->cond = parse_expr(0);
    else if (peek(0)->type != DOTS && peek(0)->line == token->line) {
        add_error(
            token->s,
            token->e,
            "else has no condition",
            format("use elif%.*s", line_end(token->e) - token->e, text_at(token->e))
        );
        node->cond = parse_expr(0);
    }
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
    case SCOPE: {
        node = new_node(token);
        enter_scope(node);
        while (peek(0)->type != LAST)
            append(node->children, parse_expr(0));
        exit_scope();
        append(node->children, new_builtin(WRITE, "output", 1, NULL));
        append(node->children, new_builtin(WRITE, "errput", 2, NULL));
        append(
            node->children,
            new_builtin(READ, "input", 0, new_type(ARRAY, 0, new_type(INT, 8, NULL)))
        );
        append(
            node->children, new_builtin(SIZEOF, "sizeof", 0, new_type(INT, 32, NULL))
        );
        append(
            node->children,
            new_builtin(
                TYPEOF, "typeof", 0, new_type(ARRAY, 0, new_type(INT, 8, NULL))
            )
        );
        append(node->children, new_builtin(DROP, "drop", 0, NULL));
        return node;
    }
    case ID: {
        Node *template_type = find_template_type(token->name);
        Node *template = find_template(token->name);
        if (template_type || template || find_struct(token->name))
            check_type_first(token);
        if (template_type)
            return template_type;
        if (template && peek(0)->type == LT)
            token->name = init_from_template(template, parse_types(token));
        Node *type = parse_type();
        if (type != NULL) {
            node = new_node(new_token(DECLARE, token));
            node->var = new_node(token);
            node->var->token->type = VAR;
            node->var->data_type = type;
            append(ura.scope->children, node);
            node = node->var;
        } else if (peek(0)->type == LPARENT) {
            token->type = FN_CALL;
            node = new_node(token);
            next(); // SKIP (
            while (!includes(peek(0)->type, RPARENT, LAST, 0)) {
                append(node->children, parse_expr(0));
                if (peek(0)->type != COMA)
                    break;
                next(); // SKIP ,
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
    case LPARENT: {
        node = parse_expr(0);
        expect(RPARENT, "expected ')' to close this '('", ")");
        return node;
    }
    case SUB: {
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
        } else if (peek(0)->type != ID) {
            Token *at = peek(0);
            int start = line_start(at->s);
            add_error(
                at->s,
                at->e,
                "expected a name after 'fn'",
                format(
                    "use %.*sname%.*s",
                    at->s - start,
                    text_at(start),
                    line_end(at->s) - at->s,
                    text_at(at->s)
                )
            );
            node->token->name = "";
        } else {
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
                    append(node->right->children, param);
                } else if (token->type == VARIADIC) {
                    // node->token->is_varg = true;
                    Node *param = new_node(token);
                    append(node->right->children, param);
                    next(); // skip ...
                    break;
                } else
                    break;
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
                add_error(
                    head->s, head->e, "expected 'case' or 'default' inside a match", NULL
                );
                continue;
            }
            Node *branch = new_node(head);
            while (head->type == CASE && !includes(peek(0)->type, DOTS, LAST, 0)) {
                Node *value = new_node(new_token(ID, head));
                value->token->name = declare->var->token->name;
                Node *eq = new_node(new_token(EQ, head));
                eq->token->name = "==";
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
            if (head->type == CASE && branch->cond == NULL)
                add_error(head->s, head->e, "expected a value after 'case'", "like: case 1:");
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
    case CASE:
    case DEFAULT: {
        add_error(
            token->s,
            token->e,
            format("'%s' without a matching 'match'", token_text(token)),
            NULL
        );
        while (peek(0)->line == token->line || inside(token->space))
            next();
        return new_node(token);
    }
    case ELIF:
    case ELSE: {
        char *name = token->type == ELIF ? "elif" : "else";
        char *help = NULL;
        if (token->type == ELIF)
            help =
                format("use if%.*s", line_end(token->e) - token->e, text_at(token->e));
        add_error(
            token->s, token->e, format("%s without an if before it", name), help
        );
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
        char *message = "@error";
        char *help = NULL;
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            message = next()->chars.value;
        if (peek(0)->type == CHARS && peek(0)->line == token->line)
            help = next()->chars.value;
        add_error(token->s, token->e, message, help);
        return node;
    }
    case USE: {
        node = new_node(token);
        if (peek(0)->type != CHARS) {
            expect(CHARS, "expected a package name after 'use'", " \"@/name\"");
            return node;
        }
        Token *name = next();
        char *package = name->chars.value;
        char *path = find_package(package);
        if (path == NULL)
            add_error(
                name->s,
                name->e,
                format("package '%s' not found", package),
                strncmp2(package, "@/", 2)
                    ? format("run: ura install %s", package + 2)
                    : NULL
            );
        else
            parse_package(new_file(path));
        return node;
    }
    case TEMPLATE: {
        node = new_node(token);
        node->scope = ura.scope;
        node->args = new_node(new_token(ARGS, token));
        expect(LT, "expected '<' after 'template'", "<T>");
        while (!includes(peek(0)->type, GT, LAST, 0)) {
            Token *param = next();
            if (param->type != ID)
                add_error(
                    param->s,
                    param->e,
                    "expected names after template",
                    "use template<T, V>"
                );
            append(node->args->children, new_node(param));
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
            if (curr->space != space || curr->line == prev->line)
                continue;
            if (!includes(curr->type, STRUCT, FN_DEC, PROTO, PUB, 0))
                add_error(curr->s, curr->e, "expected struct/fn after template", NULL);
            else if (peek(0)->type == ID && curr->type != PROTO)
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
    case STRUCT: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else
            expect(ID, "expected a struct name after 'struct'", " Name");
        Node *parents = new_node(new_token(TYPES, token));
        bool has_parents = peek(0)->type == ID && strcmp2(peek(0)->name, "from");
        if (has_parents) {
            next(); // SKIP from
            while (true) {
                Token *name = peek(0);
                Node *type = parse_type();
                Node *parent = type ? find_struct(type->token->name) : NULL;
                if (parent) {
                    append(parents->children, parent);
                } else
                    add_error(
                        name->s,
                        name->e,
                        format("no struct named '%s' to take from", token_text(name)),
                        NULL
                    );
                if (peek(0)->type != AND)
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
            if (includes(child->token->type, FN_DEC, TEMPLATE, 0))
                append(node->children, child);
        }
        exit_scope();
        remove_overridden(node);
        for (int i = 0; i < node->children_count; i++) {
            Node *method = node->children[i];
            if (method->token->type != FN_DEC || method->token->is_pub)
                continue;
            Token *name = method->token;
            Node *symbol = method->args->symbol;
            if (symbol && method->args->children_count > 1)
                add_error(
                    name->s,
                    name->e,
                    format(
                        "operator '%s' takes one parameter, the right side; the left "
                        "side is self",
                        name->name
                    ),
                    format("use operator %s(other %s)", name->name, node->token->name)
                );
            bool is_assign = symbol && symbol->token->type == ASSIGN;
            bool has_one_param = method->args->children_count == 1;
            Node *other = has_one_param ? method->args->children[0]->data_type : NULL;
            bool is_by_value = other && other->token->type == ID;
            bool is_same_type =
                is_by_value && strcmp2(other->token->name, node->token->name);
            if (is_assign && is_same_type)
                add_error(
                    name->s,
                    name->e,
                    "operator = takes the other side by reference, a copy would need "
                    "itself",
                    format("use operator =(other &%s)", node->token->name)
                );
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
    case BRK: case CNT: {  // clang-format on
        node = new_node(token);
        for (int i = ura.scopes_count - 1; i >= 0 && node->loop == NULL; i--) {
            Type type = ura.scopes[i]->token->type;
            bool is_target = type == LOOP || (type == MATCH && token->type == BRK);
            if (is_target)
                node->loop = ura.scopes[i];
        }
        if (node->loop == NULL && token->type == BRK)
            add_error(
                token->s,
                token->e,
                "break outside a loop or match",
                "break only works inside a while or a match"
            );
        else if (node->loop == NULL)
            add_error(
                token->s,
                token->e,
                "continue outside a loop",
                "continue only works inside a while"
            );
        return node;
    }
    default: {
        add_error(
            token->s, token->e, format("unexpected '%s'", token_text(token)), NULL
        );
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
        if (peek(0)->type == LAST || peek(0)->line != token->line) {
            add_error(
                token->s,
                token->e,
                format("expected a value after '%s'", token_text(token)),
                NULL
            );
            return node;
        }
        if (token->type == LBRACK) {
            token->type = ACCESS;
            node->index = parse_expr(0);
            expect(RBRACK, "expected ']' after the index", "]");
        } else if (token->type == AS) {
            node->to_type = parse_type();
            if (node->to_type == NULL)
                add_error(token->s, token->e, "expected a type after 'as'", NULL);
        } else
            node->right = parse_expr(op);
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

Node *find_variable(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        print(GREEN("find variable %s in %k\n"), name, curr->token);
        Node *found = find_in_scope(curr, name);
        if (found)
            return found;
    }
    return NULL;
}

Node *find_scope(Type type) {
    for (int i = ura.scopes_count - 1; i >= 0; i--)
        if (ura.scopes[i]->token->type == type)
            return ura.scopes[i];
    return NULL;
}

Node *find_function_like(char *name, int max_diff) {
    Node *found = NULL;
    int found_diff = max_diff + 1;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            bool is_function = includes(
                child->token->type, FN_DEC, PROTO, WRITE, READ, SIZEOF, TYPEOF, DROP, 0
            );
            bool is_operator = child->args && child->args->symbol;
            if (!is_function || is_operator)
                continue;
            char *other = child->token->name;
            int diff = 0;
            int k = 0;
            for (; other[k] && name[k]; k++)
                diff += other[k] != name[k];
            diff += strlen(other + k) + strlen(name + k);
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

Node *find_method(Node *st, char *name, Node **args, int args_count, bool is_pub) {
    int self = is_pub ? 0 : 1;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != FN_DEC || child->token->is_pub != is_pub)
            continue;
        if (!strcmp2(child->token->name, name))
            continue;
        if (child->args->children_count != args_count + self)
            continue;
        bool fits = true;
        for (int j = 0; j < args_count && fits; j++) {
            Node *type = child->args->children[j + self]->data_type;
            Node *found = type_of(args[j]);
            bool is_unknown = found == NULL || type == NULL;
            if (is_unknown || (is_number(args[j]) && is_number(type)))
                continue;
            bool takes_ref = type->token->type == REF && found->token->type != REF;
            fits = same_type(found, takes_ref ? type->data_type : type);
        }
        if (!fits)
            continue;
        return child;
    }
    return NULL;
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
    return node->right->token->type == REF && !node->right->token->is_type;
}

char *type_name(Node *type) {
    switch (type->token->type) {
    case INT: {
        int shift = type->token->shift;
        if (shift == 1)
            return "bool";
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
    case CHARS:     return new_type(ARRAY, 0, new_type(INT, 8, NULL));
    case VAR: case PARAM: return is_ref_variable(node) ? node->data_type->data_type : node->data_type;
    case REF: {
        Node *type = type_of(node->operand);
        return type ? new_type(REF, 0, type) : NULL;
    }
    case FN_CALL: {
        Node *type = node->to_call ? ret_type_of(node->to_call) : NULL;
        return type && type->token->type == REF ? type->data_type : type;
    }
    case NEG:       return type_of(node->operand);
    case COPY:      return type_of(node->to_copy);
    case LT: case GT: case EQ: case LE: case GE: case NQ:
    case AND: case OR: case NOT: return bool_type();
    case ID:        return NULL;
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

bool is_number(Node *node) {
    return node->token->type == INT && node->token->shift != 1;
}

void analyze(Node *node) {
    switch (node->token->type) { // clang-format off
    case INT: case FLOAT: case VOID: 
    case VAR: case PARAM: case CHARS: case VARIADIC:
    case WRITE: case READ: case SIZEOF: case DROP:
    case TYPEOF: case TEMPLATE: case USE:
    case MACRO_IF: case MACRO_ERROR: { // clang-format on
        break;
    }
    case ID: {
        check_name(node);
        break;
    }
    case DECLARE: {
        check_declare(node);
        analyze(node->var->data_type);
        break;
    }
    case SCOPE:
    case FN_DEC:
    case PROTO:
    case STRUCT: {
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

        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    }
    case RETURN: {
        if (node->left) {
            analyze(node->left);
            check_return(node);
        } else {
            // TODO: return void
        }
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
    case IF: case ELIF: case ELSE: case LOOP: case CASE: case DEFAULT: { // clang-format on
        if (node->cond) {
            analyze(node->cond);
            check_cond(node->cond);
        }
        enter_scope(node);
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        if (node->next)
            analyze(node->next);
        break;
    } // clang-format off
    case BRK: case CNT: { // clang-format on
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
    case LINK: {
        check_link(node);
        break;
    }
    case NEG: {
        analyze(node->operand);
        break;
    }
    case NOT: {
        analyze(node->operand);
        check_type(node->operand, bool_type(), NULL);
        break;
    } // clang-format off
    case REF: case ARRAY: { // clang-format on
        if (node->token->is_type)
            analyze(node->data_type);
        else
            analyze(node->operand);
        break;
    }
    case AS: {
        analyze(node->left);
        break;
    }
    case LCBRACK: {
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]->left);
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
    case ASSIGN: // clang-format off
	case ADD: case SUB: case MUL: case DIV: case MOD: // math
	case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: // bitwise
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite
	case DIV_ASSIGN: case MOD_ASSIGN:
	case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
	case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
	case LT: case GT: case EQ: case LE: case GE: case NQ: // comparision
	case AND: case OR: { // clang-format on
        analyze(node->left);
        analyze(node->right);
        check_operation(node);
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
bool ends_with_jump(Node *block) {
    for (int i = 0; i < block->children_count; i++)
        if (includes(block->children[i]->token->type, RETURN, BRK, CNT, 0))
            return true;
    return false;
}

void codegen_drop(Ucg *place, Node *type) {
    Node *drop = find_drop(type);
    if (drop) {
        Ucg *self = ucg_ref(place);
        ucg_fn_call(drop->token->ucg, &self, 1);
    }
    Node *st = type->declared;
    int index = 0;
    for (int i = 0; i < st->children_count; i++) {
        Node *attr = st->children[i];
        if (attr->token->type != DECLARE)
            continue;
        if (needs_drop(attr->var->data_type))
            codegen_drop(ucg_get_attr(place, index), attr->var->data_type);
        index++;
    }
}

void codegen_drops(Node *last, Node *skip) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (int j = scope->children_count - 1; j >= 0; j--) {
            Node *child = scope->children[j];
            bool is_declared = child->token->type == DECLARE && child->var->token->ucg;
            if (!is_declared || child->var == skip)
                continue;
            if (needs_drop(child->var->data_type))
                codegen_drop(child->var->token->ucg, child->var->data_type);
        }
        for (int j = 0; scope->token->type == FN_DEC && j < scope->args->children_count; j++) {
            Node *param = scope->args->children[j];
            if (param == skip || param->token->type != PARAM)
                continue;
            if (needs_drop(param->data_type))
                codegen_drop(param->token->ucg, param->data_type);
        }
        if (scope == last)
            return;
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
    for (int i = 0; i < node->children_count; i++) {
        codegen_statement(node->children[i]);
        if (includes(node->children[i]->token->type, RETURN, BRK, CNT, 0))
            break;
    }
    if (!ends_with_jump(node))
        codegen_drops(node, NULL);
    exit_scope();
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
        if (child->token->type != STRUCT)
            continue;
        for (int j = 0; j < child->children_count; j++) {
            Node *attr = child->children[j];
            if (attr->token->type != DECLARE)
                continue;
            codegen(attr->var->data_type);
            Ucg *type = attr->var->data_type->token->ucg;
            ucg_add_attr(child->token->ucg, ucg_attr(attr->var->token->name, type));
        }
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

void codegen_protos(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(child->token->type, FN_DEC, PROTO, 0))
            codegen_proto(child, child->token->name);
        if (child->token->type != STRUCT)
            continue;
        for (int j = 0; j < child->children_count; j++) {
            Node *method = child->children[j];
            if (method->token->type != FN_DEC)
                continue;
            codegen_proto(
                method, format("%s.%s", child->token->name, method->token->name)
            );
        }
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
                WRITE,
                READ,
                SIZEOF,
                TYPEOF,
                DROP,
                0
            ))
            continue;
        codegen_statement(child);
    }
}

void codegen(Node *node) {
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
        } else
            node->token->ucg = ucg_ref(codegen_in_variable(node->operand));
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
        node->token->ucg = ucg_string_value(node->token->chars.value);
        break;
    }
    case DECLARE: {
        codegen(node->var->data_type); // data type

        char *name = node->var->token->name;
        Ucg *type = node->var->data_type->token->ucg;
        if (ura.scope->token->type == SCOPE)
            ucg = ucg_global(name, type);
        else
            ucg = ucg_var(name, type);
        node->var->token->ucg = ucg;
        break;
    }
    case SCOPE: {
        enter_scope(node);
        codegen_structs(node);
        codegen_protos(node);
        bool has_main = false;
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (child->token->type == DECLARE)
                codegen(child);
            if (child->token->type == LINK)
                ucg_link(child->token->chars.value);
            if (child->token->type == FN_DEC && strcmp2(child->token->name, "main"))
                has_main = true;
        }
        if (!has_main) {
            Ucg *fn_main = ucg_fn("main", ucg_i32_type(), NULL, 0);
            codegen_statements(node);
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
        codegen_structs(node);
        codegen_protos(node);
        ucg_entry(node->token->ucg);
        if (parent->token->type == SCOPE && strcmp2(node->token->name, "main"))
            codegen_statements(parent);
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (includes(child->token->type, PROTO, STRUCT, 0))
                continue;
            codegen_statement(child);
            if (includes(child->token->type, RETURN, BRK, CNT, 0))
                break;
        }
        if (!ends_with_jump(node))
            codegen_drops(node, NULL);
        ucg_end(node->token->ucg);
        exit_scope();
        break;
    }
    case STRUCT: {
        enter_scope(node);
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (child->token->type == FN_DEC)
                codegen(child);
        }
        exit_scope();
        break;
    }
    case FN_CALL: {
        Ucg **args = arena_alloc(node->children_count + 1, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            codegen(node->children[i]);
            args[i] = node->children[i]->token->ucg;
        }
        Token *fn = node->to_call->token;
        if (fn->type == WRITE)
            node->token->ucg =
                ucg_write(ucg_i32_value(fn->num.value), args, node->children_count);
        else if (fn->type == READ)
            node->token->ucg = ucg_read(ucg_i32_value(fn->num.value));
        else if (includes(fn->type, SIZEOF, TYPEOF, 0)) {
            Node *arg = node->children[0];
            Node *type = arg->token->is_type ? arg : type_of(arg);
            codegen(type);
            Ucg *ucg = type->token->ucg;
            node->token->ucg = fn->type == SIZEOF ? ucg_sizeof(ucg) : ucg_typeof(ucg);
        } else if (fn->type == DROP) {
            Node *type = type_of(node->children[0]);
            codegen(type);
            if (needs_drop(type))
                codegen_drop(args[0], type);
            node->token->ucg = ucg_assign(args[0], ucg_null(type->token->ucg));
        } else
            node->token->ucg = codegen_call_result(
                node->to_call, ucg_fn_call(fn->ucg, args, node->children_count)
            );
        break;
    }
    case RETURN: {
        Node *fn = find_scope(FN_DEC);
        if (node->left) {
            codegen(node->left);
            Ucg *value = ucg_value(node->left->token->ucg);
            codegen_drop_temporaries();
            bool is_local = is_local_variable(node->left);
            codegen_drops(fn, is_local ? node->left->declared : NULL);
            ucg = ucg_ret(value);
        } else {
            codegen_drops(fn, NULL);
            ucg = ucg_return();
        }
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
        codegen_drops(node, NULL);
        exit_scope();
        break;
    }
    case BRK: {
        bool is_match = node->loop->token->type == MATCH;
        Node *subject = is_match ? node->loop->children[0]->var : NULL;
        codegen_drops(node->loop, subject);
        ucg_brk(node->loop->token->ucg);
        break;
    } // clang-format off
    case TEMPLATE: case USE: case MACRO_IF: 
    case MACRO_ERROR: case ENUM: { // clang-format on
        break;
    }
    case ID: {
        node->token->ucg = node->declared->token->ucg;
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
        node->token->ucg = ucg_struct_value(node->data_type->token->ucg, values, values_count);
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
        Ucg **args = arena_alloc(call->children_count + 1, sizeof(Ucg *));
        for (int i = 0; i < call->children_count; i++) {
            codegen(call->children[i]);
            args[i] = call->children[i]->token->ucg;
        }
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
    case NOT: {
        codegen(node->operand);
        node->token->ucg = ucg_not(node->operand->token->ucg);
        break;
    }
    case CNT: {
        codegen_drops(node->loop, NULL);
        ucg_cnt(node->loop->token->ucg);
        break;
    }
    case ASSIGN: // clang-format off
	case ADD: case SUB: case MUL: case DIV: case MOD: // math
	case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: // bitwise
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite
	case DIV_ASSIGN: case MOD_ASSIGN:
	case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
	case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
	case LT: case GT: case EQ: case LE: case GE: case NQ: // comparision
	case AND: case OR: { // clang-format on
        if (is_ref_binding(node)) {
            Node *slot = node->left->declared ? node->left->declared : node->left;
            codegen(node->right);
            node->token->ucg = ucg_assign(slot->token->ucg, node->right->token->ucg);
            break;
        }
        codegen(node->left);
        codegen(node->right);
        Node *type = type_of(node->left);
        bool is_declaration =
            node->left->token->type == VAR && node->left->declared == NULL;
        if (node->token->type == ASSIGN && !is_declaration && needs_drop(type)) {
            Ucg *value = ucg_value(node->right->token->ucg);
            codegen_drop(node->left->token->ucg, type);
            node->token->ucg = ucg_assign(node->left->token->ucg, value);
            break;
        }
        ucg =
            ucg_op(node->token->type, node->left->token->ucg, node->right->token->ucg);
        node->token->ucg = ucg;
        break;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, node->token->type);
        exit(1);
        break;
    }
    }
}

/*
TODO:
    + replace all fprintf with builtins
    + try buffring instead of calling fprintf everytime
    + use (reopen already opened file)
    + reuse (don't reopen opened files)
*/

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

    check_errors();

    print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tokens_count; i++)
        print(CYAN("token ") "%k\n", ura.tokens[i]);

#    if AST
    print(GREEN("======================================\n"));
    print(GREEN("[AST]:\n"));
    ura.tree = parse_expr(0);
    check_errors();
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#        if ANALYZE
    print(GREEN("======================================\n"));
    print(GREEN("[ANALYZE]:\n"));
    analyze(ura.tree);
    check_errors();
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
    default:		return fprintf(fp, "%s", type_names[node->token->type]);
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

void init_project(char *name) {
    if (access(format("%s/ura.project", name), F_OK) == 0) {
        eprint("'%s' already has a ura.project\n", name);
        exit(1);
    }
    mkdir2(name);
    mkdir2(format("%s/src", name));
    char *files[][2] = {
        { "ura.project", format("name \"%s\"\nversion \"0.1.0\"\n", name) },
        { "src/main.ura", "fn main() i32:\n    output(\"hello\\n\")\n    return 0\n" },
        { ".gitignore", "build/\n" },
    };
    for (size_t i = 0; i < sizeof(files) / sizeof(*files); i++) {
        ioFile fp = fopen(format("%s/%s", name, files[i][0]), "w");
        if (fp == NULL) {
            eprint("can't write %s/%s\n", name, files[i][0]);
            exit(1);
        }
        fputs(files[i][1], fp);
        fclose(fp);
    }
    help("created %s/, run it with: cd %s && ura run\n", name, name);
}

void build_project(void) {
    if (access("ura.project", R_OK) != 0) {
        eprint("no ura.project in this folder\n");
        help("create a project with: ura init <name>\n");
        exit(1);
    }
    File *project = new_file("ura.project");
    open_file(project);
    char *name = strstr(project->buff, "name \"");
    char *end = name ? strchr(name + 6, '"') : NULL;
    if (end == NULL) {
        eprint("ura.project has no name\n");
        help("add a line: name \"<name>\"\n");
        exit(1);
    }
    name += 6;
    ucg_bin_path(format("build/%s", strndup2(name, end - name)));
    append(ura.files, new_file("src/main.ura"));
}

void print_help(char *ura) {
    help("usage:\n");
    help("  %s <file>.ura [flags]    compile files\n", ura);
    help("  %s init <name>           create a project in a new folder\n", ura);
    help("  %s build                 build the project in this folder\n", ura);
    help("  %s run                   build the project and run it\n", ura);
    help("  %s install <name>[@ver]  install a package from the registry\n", ura);
    help("  %s remove <name>         remove an installed package\n", ura);
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
    bool install = strcmp2(av[1], "install");
    if (install || strcmp2(av[1], "remove")) {
        if (ac < 3) {
            eprint("expected a package name:\n");
            help("%s %s <name>\n", av[0], av[1]);
            exit(1);
        }
        for (int i = 2; i < ac; i++) {
            if (install)
                install_package(av[i]);
            else
                remove_package(av[i]);
        }
        exit(0);
    }
    bool run = strcmp2(av[1], "run");
    if (run || strcmp2(av[1], "build")) {
        build_project();
        return run;
    }
    bool has_exe = false;
    bool no_link = false;
    for (int i = 1; i < ac && ura.errors_count == 0; i++) {
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
            char *str = escaped[input[s + 1]];
            if (str != NULL) {
                c = str[0];
                s += 2;
            }
        }
        if (c == -1)
            c = input[s++];
        assert(c >= 0);
        res[r++] = c;
    }
    return res;
}
