// TYPEDEFS
typedef struct Token Token;
typedef struct Node Node;
typedef struct NodePrint NodePrint;
typedef struct uraError uraError;

#define print(...)  _print(stdout, __VA_ARGS__)
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

#include "./ir.c"

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
    };
    union {
        Node *right;
        Node *args;
        Node *next;
        Node *declared;
        Node *to_type;
        Node *index;
    };

    expand(Node *, children);
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
    // int errors_count;
    // File errors;
    // char *errors_buff;
    // int errors_len;
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
    { "and", { .type = AND } },
    { "or", { .type = OR } },
    { "not", { .type = NOT } },
    { "proto", { .type = PROTO } },
    { "link", { .type = LINK } },
    { "use", { .type = USE } },
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

    { "...", VARIADIC },  { ".", DOT },

    { "+", ADD },         { "-", SUB },         { "*", MUL },
    { "/", DIV },         { "%", MOD },

    { ">=", GE },         { "<=", LE },         { ">", GT },
    { "<", LT },          { "==", EQ },         { "!=", NQ },
    { "!", NOT },

    { "=", ASSIGN },      { ",", COMA },        { "&&", AND },
    { "||", OR },         { "&", REF },         { NULL, NONE },
};

char *binary_ops[END + 1] = {
    [ADD] = "+",        [SUB] = "-",        [MUL] = "*",
    [DIV] = "/",        [MOD] = "%",

    [ADD_ASSIGN] = "+", [SUB_ASSIGN] = "-", [MUL_ASSIGN] = "*",
    [DIV_ASSIGN] = "/", [MOD_ASSIGN] = "%",

    [EQ] = "==",        [NQ] = "!=",        [LT] = "<",
    [GT] = ">",         [LE] = "<=",        [GE] = ">=",

    [AND] = "and",      [OR] = "or",
};

int precedence[END + 1] = {
    [ASSIGN] = 1,     [ADD_ASSIGN] = 1,   [SUB_ASSIGN] = 1,
    [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1,   [MOD_ASSIGN] = 1,

    [OR] = 11,        [AND] = 12,

    [EQ] = 21,        [NQ] = 21,          [GT] = 22,
    [LT] = 22,        [GE] = 22,          [LE] = 22,

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

void tokenize(int start, int space2) {
    int s = start;
    int e = start;
    int space = space2;
    char *buff = ura.file->buff;
    while (buff[e] != '\0') {
        s = e;
        if (isspace(buff[e])) {
            while (buff[e] == '\n') {
                ura.line++;
                e++;
            }
            if (buff[s] == '\n') {
                space = space2;
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
                int start = line_start(s);
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
        while (isalpha(buff[s]) && (isalnum(buff[e]) || buff[e] == '_'))
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
    char *buff = ura.file->buff;
    while (pos > 0 && buff[pos - 1] != '\n')
        pos--;
    while (buff[pos] == ' ')
        pos++;
    return pos;
}

char *token_text(Token *token) {
    return format("%.*s", token->e - token->s, ura.file->buff + token->s);
}

int line_end(int pos) {
    char *buff = ura.file->buff;
    while (buff[pos] && buff[pos] != '\n')
        pos++;
    return pos;
}

void expect(Type type, char *msg, char *fix) {
    if (peek(0)->type == type) {
        next();
        return;
    }
    char *buff = ura.file->buff;
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
            buff + start,
            fix,
            end - prev->e,
            buff + prev->e
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

int type_end(int pos) {
    if (peek(pos)->type == LT) {
        int depth = 0;
        do {
            if (peek(pos)->type == LAST)
                return pos;
            if (peek(pos)->type == LT)
                depth++;
            if (peek(pos)->type == GT)
                depth--;
            pos++;
        } while (depth > 0);
    }
    if (peek(pos)->type == LBRACK && peek(pos + 1)->type == RBRACK)
        pos += 2;
    return pos;
}

bool fix_type_first(Token *token) {
    int end = type_end(0);
    Token *name = peek(end);
    if (name->type != ID || name->line != token->line)
        return false;
    int start = ura.exe_pos - 1;
    Token *last = ura.tokens[ura.exe_pos + end - 1];
    char *buff = ura.file->buff;
    add_error(
        token->s,
        last->e,
        "the type goes after the name",
        format(
            "use %s %.*s%.*s",
            name->name,
            last->e - token->s,
            buff + token->s,
            line_end(name->e) - name->e,
            buff + name->e
        )
    );
    for (int i = ura.exe_pos + end; i > start; i--)
        ura.tokens[i] = ura.tokens[i - 1];
    ura.tokens[start] = name;
    ura.exe_pos = start;
    return true;
}

void parse_tokens(Node *scope, int start) {
    int exe_pos = ura.exe_pos;
    ura.exe_pos = start;
    while (peek(0)->type != LAST)
        append(scope->children, parse_expr(0));
    ura.exe_pos = exe_pos;
}

char *find_module(char *name) {
    if (!strncmp2(name, "@/", 2)) {
        char *path = format("%s/%s.ura", dirname2(ura.files[0]->path), name);
        return access(path, R_OK) == 0 ? path : NULL;
    }
    char *package = strdup2(name + 2);
    char *version = strchr(package, '@');
    if (version)
        *version++ = '\0';
    char *dirs[] = { getenv("URA_LIB"), format("%s/.ura/lib", getenv("HOME")) };
    for (int i = 0; i < 2; i++) {
        if (dirs[i] == NULL)
            continue;
        char *paths[] = {
            format(
                "%s/%s/%s/%s.ura", dirs[i], package, version ? version : "current", package
            ),
            version ? NULL : format("%s/%s.ura", dirs[i], package),
        };
        for (int j = 0; j < 2; j++) {
            if (paths[j] == NULL || access(paths[j], R_OK) != 0)
                continue;
            return paths[j];
        }
    }
    return NULL;
}

void parse_module(File *module) {
    open_file(module);
    for (int i = 0; i < ura.files_count; i++) {
        if (strcmp2(ura.files[i]->path, module->path))
            return;
    }
    File *file = ura.file;
    module->start = file->len + 2;
    char *buff = arena_alloc(module->start + module->len + 1, sizeof(char));
    memcpy(buff, file->buff, file->len);
    buff[file->len + 1] = '\n';
    memcpy(buff + module->start, module->buff, module->len);
    file->buff = buff;
    file->len = module->start + module->len;
    append(ura.files, module);

    int line = ura.line;
    int start = ura.tokens_count;
    ura.line = 1;
    tokenize(module->start, 0);
    new_token(LAST, NULL);
    ura.line = line;
    parse_tokens(ura.scope, start);
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
    expect(GT, format("expected '>' after the types of '%s'", token->name), ">");
    return types;
}

char *instantiate(Node *template, Node *types) {
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

    int start = ura.tokens_count;
    Token *body = template->token;
    for (int i = body->body.start; i < body->body.end; i++) {
        Token *copy = new_token(ura.tokens[i]->type, NULL);
        *copy = *ura.tokens[i];
        if (copy->type == ID && strcmp2(copy->name, token->name))
            copy->name = name;
    }
    new_token(LAST, NULL);
    Node *outer = ura.types;
    types->declared = template;
    ura.types = types;
    parse_tokens(template->scope, start);
    ura.types = outer;
    return name;
}

Node *parse_type(void) {
    Token *token = peek(0);
    Token *prev = ura.tokens[ura.exe_pos - 1];
    if (token->type == REF && token->line == prev->line) {
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
            token->name = instantiate(template, parse_types(token));
        else if (peek(0)->type == LT) {
            add_error(
                token->s,
                token->e,
                format("no template named '%s'", token->name),
                format("declare it: template<T>: struct %s:", token->name)
            );
            parse_types(token);
        }
        if (peek(0)->type == LBRACK && peek(1)->type == RBRACK) {
            next(); // SKIP [
            next(); // SKIP ]
            Node *array = new_node(new_token(ARRAY, token));
            array->token->is_type = true;
            array->data_type = node;
            return array;
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
        char *buff = ura.file->buff;
        add_error(
            token->s,
            token->e,
            "else has no condition",
            format("use elif%.*s", line_end(token->e) - token->e, buff + token->e)
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
        if (token->is_type && fix_type_first(token))
            return parse_prime();
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
            new_builtin(TYPEOF, "typeof", 0, new_type(ARRAY, 0, new_type(INT, 8, NULL)))
        );
        return node;
    }
    case ID: {
        bool is_type_name = find_struct(token->name) || find_template(token->name) ||
                            find_template_type(token->name);
        if (is_type_name && fix_type_first(token))
            return parse_prime();
        Node *template_type = find_template_type(token->name);
        if (template_type)
            return template_type;
        Node *template = find_template(token->name);
        if (template && peek(0)->type == LT)
            token->name = instantiate(template, parse_types(token));
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
        if (token->type == OPERATOR) {
            token->type = FN_DEC;
            token = next();
            node->token->name = token_text(token);
            if (token->type == LBRACK) {
                expect(RBRACK, "expected ']' after 'operator ['", "]");
                node->token->name = "[]";
            }
        } else if (peek(0)->type != ID) {
            char *buff = ura.file->buff;
            Token *at = peek(0);
            int start = line_start(at->s);
            add_error(
                at->s,
                at->e,
                "expected a name after 'fn'",
                format(
                    "use %.*sname%.*s",
                    at->s - start,
                    buff + start,
                    line_end(at->s) - at->s,
                    buff + at->s
                )
            );
            node->token->name = "";
        } else {
            token = next();
            node->token->name = token->name;
        }
        node->right = new_node(new_token(ARGS, token));
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
        while (includes(peek(0)->type, ELIF, ELSE, 0) && peek(0)->space == token->space
        ) {
            branch->next = parse_block(next());
            branch = branch->next;
        }
        return node;
    }
    case ELIF:
    case ELSE: {
        char *buff = ura.file->buff;
        char *name = token->type == ELIF ? "elif" : "else";
        char *help = NULL;
        if (token->type == ELIF)
            help =
                format("use if%.*s", line_end(token->e) - token->e, buff + token->e);
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
    case USE: {
        node = new_node(token);
        if (peek(0)->type != CHARS) {
            expect(CHARS, "expected a module name after 'use'", " \"@/name\"");
            return node;
        }
        Token *name = next();
        char *module = name->chars.value;
        char *path = find_module(module);
        if (path == NULL)
            add_error(
                name->s,
                name->e,
                format("module '%s' not found", module),
                strncmp2(module, "@/", 2) ? format("run: ura --install %s", module + 2)
                                          : NULL
            );
        else
            parse_module(new_file(path));
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
    case STRUCT: {
        node = new_node(token);
        if (peek(0)->type == ID)
            node->token->name = next()->name;
        else
            expect(ID, "expected a struct name after 'struct'", " Name");
        expect(DOTS, "expected ':' after the struct name", ":");
        enter_scope(node);
        while (inside(node->token->space)) {
            Node *child = parse_expr(0);
            if (includes(child->token->type, FN_DEC, TEMPLATE, 0))
                append(node->children, child);
        }
        exit_scope();
        for (int i = 0; i < node->children_count; i++) {
            Node *method = node->children[i];
            if (method->token->type != FN_DEC || method->token->is_pub)
                continue;
            Token *name = method->token;
            bool is_operator = !isalpha(name->name[0]) && name->name[0] != '_';
            if (is_operator && method->args->children_count > 1)
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
    }
    case BRK:
    case CNT: {
        node = new_node(token);
        node->loop = find_scope(LOOP);
        if (node->loop == NULL) {
            char *name = token->type == BRK ? "break" : "continue";
            add_error(
                token->s,
                token->e,
                format("%s outside a loop", name),
                format("%s only works inside a while", name)
            );
        }
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

int name_diff(char *a, char *b) {
    int d = 0;
    int i = 0;
    int j = 0;
    while (a[i] && b[j])
        d += a[i++] != b[j++];
    return d + strlen(a + i) + strlen(b + j);
}

Node *find_function_like(char *name, int max_diff) {
    Node *found = NULL;
    int found_diff = max_diff + 1;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        for (int j = 0; j < curr->children_count; j++) {
            Node *child = curr->children[j];
            if (!includes(child->token->type, FN_DEC, PROTO, WRITE, READ, SIZEOF, TYPEOF, 0))
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

bool fits_type(Node *value, Node *type) {
    Node *found = type_of(value);
    if (found == NULL || type == NULL)
        return true;
    if (is_number(value) && is_number(type))
        return true;
    if (type->token->type == REF && found->token->type != REF)
        return same_type(found, type->data_type);
    return same_type(found, type);
}

Node *find_drop(Node *type) {
    if (type == NULL || type->token->type != ID || type->declared == NULL)
        return NULL;
    return find_method(type->declared, "drop", NULL, 0, false);
}

bool lives_in_variable(Node *node) {
    if (node->token->type == DOT)
        return node->right->token->type != FN_CALL;
    return includes(node->token->type, VAR, PARAM, ACCESS, 0);
}

void check_copy(Node *value, Node *type) {
    if (!lives_in_variable(value) || find_drop(type) == NULL)
        return;
    Token *token = value->token;
    add_error(
        token->s,
        token->e,
        format("'%s' has operator drop, passing a copy would free it twice", type_name(type)),
        format("take it as &%s", type_name(type))
    );
}

void check_args(Node *fn, Node **args, int count, int self) {
    for (int i = 0; i < count && i + self < fn->args->children_count; i++) {
        Node *type = fn->args->children[i + self]->data_type;
        if (type == NULL || args[i]->token->type == REF)
            continue;
        if (type->token->type != REF) {
            check_copy(args[i], type);
            continue;
        }
        Node *ref = new_node(new_token(REF, args[i]->token));
        ref->operand = args[i];
        args[i] = ref;
    }
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
        for (int j = 0; j < args_count && fits; j++)
            fits = fits_type(args[j], child->args->children[j + self]->data_type);
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
    }
    case FLOAT:
        return format("f%d", type->token->shift);
    case ARRAY:
        return format("%s[]", type_name(type->data_type));
    case REF:
        return format("&%s", type_name(type->data_type));
    case VOID:
        return "void";
    case ID:
        return type->token->name;
    default:
        return (char *)to_string(type->token->type);
    }
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
    case LT: case GT: case EQ: case LE: case GE: case NQ:
    case AND: case OR: return bool_type();
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

void check_struct_value(Node *value, Node *expected) {
    Token *token = value->token;
    if (expected->token->type != ID) {
        add_error(
            token->s,
            token->e,
            format("expected '%s', found a struct value", type_name(expected)),
            NULL
        );
        return;
    }
    if (expected->declared == NULL)
        return;
    value->data_type = expected;
    Node *st = expected->declared;
    for (int i = 0; i < value->children_count; i++) {
        Node *attr = value->children[i];
        Token *name = attr->token;
        attr->declared = find_attr(st, name->name, &name->attr.index);
        if (attr->declared == NULL) {
            add_error(
                name->s,
                name->e,
                format("'%s' has no attribute '%s'", st->token->name, name->name),
                NULL
            );
            continue;
        }
        check_type(attr->left, attr->declared->data_type, NULL);
    }
}

void check_type(Node *value, Node *expected, char *help) {
    if (expected && value->token->type == LCBRACK) {
        check_struct_value(value, expected);
        return;
    }
    Node *found = type_of(value);
    bool unknown = expected && expected->token->type == ID && !expected->declared;
    if (found == NULL || expected == NULL || unknown)
        return;
    Token *token = value->token;
    if (is_number(value) && is_number(expected)) {
        int bits = expected->token->shift - !expected->token->is_uint;
        long long max = bits == 64 ? ~0ULL >> 1 : (1ULL << bits) - 1;
        long long min = expected->token->is_uint ? 0 : -max - 1;
        long long number = token->num.value;
        if (number < min || number > max)
            add_error(
                token->s,
                token->e,
                format("%lld doesn't fit in %s", number, type_name(expected)),
                format("%s goes from %lld to %lld", type_name(expected), min, max)
            );
        token->shift = expected->token->shift;
        token->is_uint = expected->token->is_uint;
        return;
    }
    bool is_real = value->token->type == FLOAT && !value->token->is_type;
    if (is_real && expected->token->type == FLOAT) {
        token->shift = expected->token->shift;
        return;
    }
    if (!same_type(found, expected))
        add_error(
            token->s,
            token->e,
            format("expected '%s', found '%s'", type_name(expected), type_name(found)),
            help
        );
}

void check_same_type(Node *left, Node *right) {
    if (is_number(left))
        check_type(left, type_of(right), NULL);
    else
        check_type(right, type_of(left), NULL);
}

void analyze_operator(Node *node, Node *st, char *symbol) {
    Token *op = node->token;
    Node *method = find_method(st, symbol, &node->right, 1, false);
    if (method == NULL) {
        Node *right = type_of(node->right);
        add_error(
            op->s,
            op->e,
            format(
                "'%s' has no operator '%s' that takes '%s'",
                st->token->name,
                symbol,
                right ? type_name(right) : "?"
            ),
            NULL
        );
        return;
    }
    check_args(method, &node->right, 1, 1);
    Node *call = new_node(new_token(FN_CALL, op));
    call->token->name = symbol;
    call->to_call = method;
    append(call->children, node->right);
    op->type = DOT;
    node->right = call;
}

void analyze(Node *node) {
    switch (node->token->type) { // clang-format off
    case INT: case FLOAT: case VOID: 
    case VAR: case PARAM: case CHARS: case VARIADIC:
    case WRITE: case READ: case SIZEOF: 
    case TYPEOF: case TEMPLATE: case USE: { // clang-format on
        break;
    }
    case ID: {
        if (node->token->is_type) {
            Token *token = node->token;
            node->declared = find_struct(token->name);
            if (node->declared == NULL)
                add_error(
                    token->s,
                    token->e,
                    format("unknown type '%s'", token->name),
                    format("declare it: struct %s:", token->name)
                );
            break;
        }
        Node *var = find_variable(node->token->name);
        if (var) {
            node->token->type = var->token->type;
            node->data_type = var->data_type;
            node->declared = var;
        } else {
            Token *token = node->token;
            add_error(
                token->s,
                token->e,
                format("undeclared variable '%s'", token->name),
                format("declare it first, like: %s i32 = 0", token->name)
            );
        }
        break;
    }
    case DECLARE: {
        Token *token = node->var->token;
        if (find_in_scope(ura.scope, token->name) != node->var)
            add_error(
                token->s,
                token->e,
                format("'%s' is already declared", token->name),
                NULL
            );
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
        Node *fn = find_scope(FN_DEC);
        if (node->left) {
            analyze(node->left);
            if (fn)
                check_type(node->left, ret_type_of(fn), NULL);
        } else {
            // TODO: return void
        }
        break;
    } // clang-format off
    case IF: case ELIF: case ELSE: case LOOP: { // clang-format on
        if (node->cond) {
            analyze(node->cond);
            char *help = NULL;
            if (includes(node->cond->token->type, VAR, PARAM, 0))
                help = format("write '%s != 0'", node->cond->token->name);
            check_type(node->cond, bool_type(), help);
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
        Token *name = node->left->token;
        Node *st = find_struct(name->name);
        if (st == NULL) {
            add_error(name->s, name->e, format("unknown type '%s'", name->name), NULL);
            break;
        }
        Node *call = node->right;
        for (int i = 0; i < call->children_count; i++)
            analyze(call->children[i]);
        call->to_call = find_method(
            st, call->token->name, call->children, call->children_count, true
        );
        if (call->to_call == NULL)
            add_error(
                call->token->s,
                call->token->e,
                format("'%s' has no pub fn '%s'", st->token->name, call->token->name),
                NULL
            );
        else
            check_args(call->to_call, call->children, call->children_count, 0);
        break;
    }
    case LINK: {
        Token *token = node->token;
        if (token->name == NULL)
            break;
        token->chars.value = ucg_find_lib(token->name);
        if (token->chars.value == NULL)
            add_error(
                token->s,
                token->e,
                format("library '%s' not found", token->name),
                "install it, or set URA_LIB_PATH to the folder that holds it"
            );
        break;
    }
    case NEG: {
        analyze(node->operand);
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
        Node *type = type_of(node->left);
        if (type && type->token->type == ID && type->declared) {
            analyze_operator(node, type->declared, "[]");
            break;
        }
        check_type(node->index, new_type(INT, 32, NULL), NULL);
        if (type == NULL || type->token->type == ARRAY)
            break;
        Token *base = node->left->token;
        add_error(
            base->s,
            base->e,
            format("'%s' is '%s', not an array", token_text(base), type_name(type)),
            NULL
        );
        break;
    }
    case DOT: {
        analyze(node->left);
        Node *type = type_of(node->left);
        if (type == NULL)
            break;
        Token *base = node->left->token;
        if (type->token->type != ID) {
            add_error(
                base->s,
                base->e,
                format(
                    "'%s' is '%s', not a struct", token_text(base), type_name(type)
                ),
                NULL
            );
            break;
        }
        if (type->declared == NULL)
            break;
        Node *st = type->declared;
        if (node->right->token->type == FN_CALL) {
            Node *call = node->right;
            for (int i = 0; i < call->children_count; i++)
                analyze(call->children[i]);
            call->to_call = find_method(
                st, call->token->name, call->children, call->children_count, false
            );
            if (call->to_call == NULL)
                add_error(
                    call->token->s,
                    call->token->e,
                    format(
                        "'%s' has no method '%s'", st->token->name, call->token->name
                    ),
                    NULL
                );
            else
                check_args(call->to_call, call->children, call->children_count, 1);
            break;
        }
        Token *attr = node->right->token;
        node->right->declared = find_attr(st, attr->name, &attr->attr.index);
        if (node->right->declared == NULL)
            add_error(
                attr->s,
                attr->e,
                format("'%s' has no attribute '%s'", st->token->name, attr->name),
                NULL
            );
        break;
    }
    case FN_CALL: {
        Token *token = node->token;
        char *name = token->name;
        node->to_call = find_function(name);
        if (node->to_call == NULL) {
            Node *like = find_function_like(name, 2);
            if (find_variable(name))
                add_error(
                    token->s,
                    token->e,
                    format("'%s' is a variable, not a function", name),
                    NULL
                );
            else
                add_error(
                    token->s,
                    token->e,
                    format("no function named '%s'", name),
                    like ? format("did you mean '%s'?", like->token->name) : NULL
                );
            break;
        }
        bool takes_type = includes(node->to_call->token->type, SIZEOF, TYPEOF, 0);
        if (takes_type && node->children_count != 1)
            add_error(
                token->s,
                token->e,
                format("'%s' takes one argument, got %d", name, node->children_count),
                format("use %s(x)", name)
            );
        Node *params = node->to_call->args;
        for (int i = 0; i < node->children_count; i++) {
            analyze(node->children[i]);
            if (params == NULL || i >= params->children_count)
                continue;
            Node *param = params->children[i];
            if (param->token->type != VARIADIC) {
                check_type(node->children[i], param->data_type, NULL);
                check_copy(node->children[i], param->data_type);
            }
        }
        break;
    }
    case ASSIGN: // clang-format off
	case ADD: case SUB: case MUL: case DIV: case MOD: // math
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite 
	case DIV_ASSIGN: case MOD_ASSIGN: 
	case LT: case GT: case EQ: case LE: case GE: case NQ: // comparision
	case AND: case OR: { // clang-format on
        analyze(node->left);
        analyze(node->right);
        Node *left_type = type_of(node->left);
        bool is_struct =
            left_type && left_type->token->type == ID && left_type->declared;
        if (is_struct) {
            Node *st = left_type->declared;
            bool is_declaration =
                node->left->token->type == VAR && node->left->declared == NULL;
            bool has_assign = find_method(st, "=", &node->right, 1, false) != NULL;
            bool is_copy = node->token->type == ASSIGN && lives_in_variable(node->right);
            bool has_drop = find_drop(left_type) != NULL;
            if (is_copy && has_drop && !has_assign) {
                Token *token = node->token;
                add_error(
                    token->s,
                    token->e,
                    format(
                        "'%s' has operator drop, a copy would free it twice",
                        st->token->name
                    ),
                    format("define operator =(other &%s) to copy it", st->token->name)
                );
                break;
            }
            bool copy_by_operator = has_assign && (!is_declaration || (has_drop && is_copy));
            if (node->token->type != ASSIGN || copy_by_operator) {
                analyze_operator(node, st, token_text(node->token));
                break;
            }
        }
        if (is_ref_binding(node))
            check_type(node->right, node->left->data_type, NULL);
        else if (!includes(node->token->type, AND, OR, 0))
            check_same_type(node->left, node->right);
        else if (!type_of(node->left) || same_type(type_of(node->left), bool_type()))
            check_type(node->right, bool_type(), NULL);
        else
            check_type(node->left, bool_type(), NULL);
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
    if (block->children_count == 0)
        return false;
    Node *last = block->children[block->children_count - 1];
    return includes(last->token->type, RETURN, BRK, CNT, 0);
}

void codegen_drops(Node *scope, Node *skip) {
    for (int i = scope->children_count - 1; i >= 0; i--) {
        Node *child = scope->children[i];
        if (child->token->type != DECLARE || child->var == skip)
            continue;
        Node *drop = find_drop(child->var->data_type);
        if (drop == NULL)
            continue;
        Ucg *self = ucg_ref(child->var->token->ucg);
        ucg_fn_call(drop->token->ucg, &self, 1);
    }
    if (scope->token->type != FN_DEC)
        return;
    for (int i = 0; i < scope->args->children_count; i++) {
        Node *param = scope->args->children[i];
        if (param == skip || param->token->type != PARAM)
            continue;
        Node *drop = find_drop(param->data_type);
        if (drop == NULL)
            continue;
        Ucg *self = ucg_ref(param->token->ucg);
        ucg_fn_call(drop->token->ucg, &self, 1);
    }
}

void codegen_drops_until(Node *last, Node *skip) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        codegen_drops(ura.scopes[i], skip);
        if (ura.scopes[i] == last)
            return;
    }
}

void codegen_branch(Node *node, Ucg *stmt) {
    node->token->ucg = stmt;
    Ucg *cond = NULL;
    if (node->cond) {
        codegen(node->cond);
        cond = node->cond->token->ucg;
    }
    ucg_then(stmt, cond);
    enter_scope(node);
    for (int i = 0; i < node->children_count; i++)
        codegen(node->children[i]);
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
    Ucg *tmp = ucg_var("tmp", type_of(node)->declared->token->ucg);
    ucg_assign(tmp, ucg);
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
                0
            ))
            continue;
        codegen(child);
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
        } else {
            codegen(node->operand);
            node->token->ucg = ucg_ref(node->operand->token->ucg);
        }
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
            codegen(child);
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
            bool is_variable = includes(node->left->token->type, VAR, PARAM, 0);
            codegen_drops_until(fn, is_variable ? node->left->declared : NULL);
            ucg = ucg_ret(value);
        } else {
            codegen_drops_until(fn, NULL);
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
    case BRK: {
        codegen_drops_until(node->loop, NULL);
        ucg_brk(node->loop->token->ucg);
        break;
    } // clang-format off
    case TEMPLATE: case USE: { // clang-format on
        break;
    }
    case ID: {
        node->token->ucg = node->declared->token->ucg;
        break;
    }
    case LCBRACK: {
        codegen(node->data_type);
        Node *st = node->data_type->declared;
        Ucg **values = arena_alloc(st->children_count + 1, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            Node *attr = node->children[i];
            codegen(attr->left);
            values[attr->token->attr.index] = attr->left->token->ucg;
        }
        node->token->ucg = ucg_struct_value(node->data_type->token->ucg, values);
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
    case CNT: {
        codegen_drops_until(node->loop, NULL);
        ucg_cnt(node->loop->token->ucg);
        break;
    }
    case ASSIGN: // clang-format off
	case ADD: case SUB: case MUL: case DIV: case MOD: // math
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite 
	case DIV_ASSIGN: case MOD_ASSIGN: 
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
    parse_args(ac, av);
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
        open_file(file);
        ura.file = file;
        arena_reserve(file->len * 192);
        tokenize(0, 0);
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
    ucg_path("build/out.ll");
    ucg_gen();
    ucg_run_bin();
    ucg_finish();


    // gen_bin();
    // return exec_bin();
#            endif // ASM
#        endif     // ANALYZE
#    endif         // AST
#endif             // TOKENIZE
    printf("did allocate %zu\n", arena_size());
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

void add_error(int s, int e, char *msg, char *help) {
    uraError error = { .s = s, .e = e, .msg = msg, .help = help };
    append(ura.errors, error);
}

void check_errors(void) {
    if (ura.errors_count == 0)
        return;
    char *buff = ura.file->buff;
    char *cwd = getcwd2();
    size_t cwd_len = strlen(cwd);
    for (int i = 0; i < ura.errors_count; i++) {
        uraError *error = &ura.errors[i];
        File *file = ura.files[0];
        for (int j = 0; j < ura.files_count; j++) {
            File *curr = ura.files[j];
            if (curr->start <= error->s && curr->start > file->start)
                file = curr;
        }
        char *path = file->path;
        if (strncmp2(path, cwd, cwd_len) && path[cwd_len] == '/')
            path += cwd_len + 1;
        int line = 1;
        int start = file->start;
        for (int j = file->start; j < error->s; j++) {
            if (buff[j] == '\n') {
                line++;
                start = j + 1;
            }
        }
        int end = error->s;
        while (buff[end] && buff[end] != '\n')
            end++;
        int col = error->s - start;
        int width = snprintf(NULL, 0, "%d", line);

        fprintf(
            stderr, "%s: %s:%d:%d %s\n", RED("error"), path, line, col + 1, error->msg
        );
        fprintf(stderr, "%*s |\n", width, "");
        fprintf(stderr, "%d | %.*s\n", line, end - start, buff + start);
        fprintf(stderr, "%*s | %*s", width, "", col, "");
        for (int j = error->s; j < error->e || j == error->s; j++)
            fprintf(stderr, "^");
        fprintf(stderr, "\n");
        if (error->help)
            fprintf(stderr, "help: %s\n", error->help);
    }
    fprintf(
        stderr,
        "%s: aborting due to %d error%s\n",
        RED("error"),
        (int)ura.errors_count,
        ura.errors_count > 1 ? "s" : ""
    );
    exit(1);
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

void parse_args(int ac, char **av) {
    if (ac < 2) {
        eprint("expected an argument:\n");
        help("%s <file>.ura\n", av[0]);
        exit(1);
    }
    for (int i = 1; i < ac && ura.errors_count == 0; i++) {
        char *arg = av[i];
        if (strcmp2(arg, "-o")) {
            if (i + 1 >= ac) {
                eprint("expected argument:\n");
                help("-o exe.out\n");
                exit(1);
            }
            ucg_bin_path(av[++i]);
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
