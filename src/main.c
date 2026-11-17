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
    // uraIR ir;

    Ucg *ucg;
    int shift;
    union {
        struct {
            int index;
        } attr;
        struct {
            long long value;
        } num;
        struct {
            char *value;
        } chars;
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
    };
    union {
        Node *right;
        Node *args;
        Node *next;
        Node *declared;
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

    expand(uraError, errors);

    int exe_pos;
} ura;

struct {
    char *value;
    Token token;
} keywords[] = {
    { "bool", { .type = INT, .is_type = true, .shift = 1 } },
    { "i1", { .type = INT, .is_type = true, .shift = 1 } },
    { "char", { .type = INT, .is_type = true, .shift = 8 } },
    { "i8", { .type = INT, .is_type = true, .shift = 8 } },
    { "i32", { .type = INT, .is_type = true, .shift = 32 } },
    { "num", { .type = INT, .is_type = true } },

    { "True", { .type = INT, .shift = 1, .num = { .value = true } } },
    { "False", { .type = INT, .shift = 1, .num = { .value = false } } },

    { "struct", { .type = STRUCT } },
    { "and", { .type = AND } },
    { "or", { .type = OR } },
    { "not", { .type = NOT } },
    { "proto", { .type = PROTO } },
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
    { "(", LPARENT },     { ")", RPARENT },     { ":", DOTS },
    { "[", LBRACK },      { "]", RBRACK },

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
    [ASSIGN] = 1,     [ADD_ASSIGN] = 1, [SUB_ASSIGN] = 1,
    [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1, [MOD_ASSIGN] = 1,

    [OR] = 11,        [AND] = 12,

    [EQ] = 21,        [NQ] = 21,        [GT] = 22,
    [LT] = 22,        [GE] = 22,        [LE] = 22,

    [ADD] = 30,       [SUB] = 30,       [MUL] = 31,
    [DIV] = 31,       [MOD] = 31,

    [AS] = 35,

    [DOT] = 51,
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

void tokenize(File *file, int space2) {
    open_file(file);
    ura.file = file;
    arena_reserve(file->len * 192);

    int s = 0;
    int e = 0;
    int space = space2;
    char *buff = file->buff;
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
                        add_error(e, e + 2, format("unknown escape '\\%c'", next),
                                  "the escapes are \\n \\t \\r \\0 \\\\ \\\" \\'");
                        valid = false;
                    }
                    e++;
                }
                letters++;
                e++;
            }
            if (buff[e] != c) {
                int start = line_start(s);
                add_error(s, e, c == '"' ? "unclosed string" : "unclosed character",
                          format("use %.*s%c", e - start, buff + start, c));
                continue;
            }
            e++; // skip last ' or "
            if (!valid)
                continue;
            if (c == '"')
                parse_string(s, e, space);
            else if (letters > 1)
                add_error(s, e, format("a character holds one letter, not %d", letters),
                          format("use \"%.*s\" for a string", e - s - 2, buff + s + 1));
            else
                parse_char(s, e, space);
            continue;
        }
        while (isdigit(buff[e]))
            e++;
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
    return NULL;
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
    add_error(at->s, at->e, msg,
              format("use %.*s%s%.*s", prev->e - start, buff + start, fix, end - prev->e, buff + prev->e));
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

Node *parse_type(int pos) {
    Token *token = peek(pos);
    if (token->is_type) {
        next();
        Node *node = new_node(token);
        if (peek(0)->type == LBRACK && peek(1)->type == RBRACK) {
            next(); // SKIP [
            next(); // SKIP ]
            Node *ref = new_node(new_token(REF, token));
            ref->token->is_type = true;
            ref->data_type = node;
            return ref;
        }
        return node;
    }
    return NULL;
}

Node *parse_block(Token *token) {
    Node *node = new_node(token);
    enter_scope(node);
    if (token->type != ELSE)
        node->cond = parse_expr(0);
    else if (peek(0)->type != DOTS && peek(0)->line == token->line) {
        char *buff = ura.file->buff;
        add_error(token->s, token->e, "else has no condition",
                  format("use elif%.*s", line_end(token->e) - token->e, buff + token->e));
        node->cond = parse_expr(0);
    }
    char *after[LAST + 1] = {
        [IF] = "the if condition", [ELIF] = "the elif condition",
        [LOOP] = "the while condition", [ELSE] = "else",
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
    switch (token->type) {
    case INT: case CHARS: {
        return new_node(token);
    }
    case SCOPE: {
        node = new_node(token);
        enter_scope(node);
        while (peek(0)->type != LAST)
            append(node->children, parse_expr(0));
        exit_scope();
        return node;
    }
    case ID: {
        // node = new_node(token);
        Node *type = parse_type(0);
        if (type != NULL) {
            node = new_node(new_token(DECLARE, token));
            node->var = new_node(token);
            node->var->token->type = VAR;
            node->var->data_type = type;
            append(ura.scope->children, node);
            node = node->var;
            // node->right = type;
        } else if (peek(0)->type == LPARENT) {
            token->type = FN_CALL;
            node = new_node(token);
            next(); // SKIP (
            while (!includes(peek(0)->type, RPARENT, LAST)) {
                append(node->children, parse_expr(0));
                if (peek(0)->type == COMA)
                    next();
            }
            assert(peek(0)->type == RPARENT);
            next(); // SKIP )
        } else
            node = new_node(token);
        return node;
    }
    case LPARENT: {
        node = parse_expr(0);
        assert(peek(0)->type == RPARENT);
        next(); // SKIP )
        return node;
    }
    case SUB: {
        token->type = NEG;
        node = new_node(token);
        node->operand = parse_prime();
        return node;
    }
    case ADD: {
        return parse_prime();
    }
    case FN_DEC: case PROTO: {
        node = new_node(token);
        if (peek(0)->type != ID) {
            char *buff = ura.file->buff;
            Token *at = peek(0);
            int start = line_start(at->s);
            add_error(at->s, at->e, "expected a name after 'fn'",
                      format("use %.*sname%.*s", at->s - start, buff + start,
                             line_end(at->s) - at->s, buff + at->s));
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
                    Node *type = parse_type(0);
                    param->left = type;
                    append(node->right->children, param);
                } else if (token->type == VARIADIC) {
                    // node->token->is_varg = true;
                    Node *param = new_node(token);
                    append(node->right->children, param);
                    next(); // skip ...
                    break;
                } else
                    break;
                if (peek(0)->type == COMA) // TODO: to be improved
                    next();
            }
            expect(RPARENT, "expected ')' after the parameters", ")");
        } else
            expect(LPARENT, "expected '(' after the function name", "()");
        node->ret_type = parse_type(0);
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
    case IF: case LOOP: {
        node = parse_block(token);
        if (token->type != IF)
            return node;
        Node *branch = node;
        while (includes(peek(0)->type, ELIF, ELSE, 0) && peek(0)->space == token->space) {
            branch->next = parse_block(next());
            branch = branch->next;
        }
        return node;
    }
    case ELIF: case ELSE: {
        char *buff = ura.file->buff;
        char *name = token->type == ELIF ? "elif" : "else";
        char *help = NULL;
        if (token->type == ELIF)
            help = format("use if%.*s", line_end(token->e) - token->e, buff + token->e);
        add_error(token->s, token->e, format("%s without an if before it", name), help);
        return parse_block(token);
    }
    case BRK: case CNT: {
        node = new_node(token);
        node->loop = find_scope(LOOP);
        if (node->loop == NULL) {
            char *name = token->type == BRK ? "break" : "continue";
            add_error(token->s, token->e, format("%s outside a loop", name),
                      format("%s only works inside a while", name));
        }
        return node;
    }
    default: {
        eprint("%s:%d handle this case %t\n", FILE, LINE, token->type);
        exit(1);
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
        // print(">> %n\n", child);
        Node *id = child->token->type == DECLARE ? child->var : child;
        if (strcmp2(id->token->name, name))
            return id;
    }
    return NULL;
}
Node *find_variable(char *name) {
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *curr = ura.scopes[i];
        print(GREEN("find variable %s in %k\n"), name, curr->token);
        Node *to_find = NULL;
        if (curr->token->type == FN_DEC)
            to_find = find_in_children(curr->right, name);
        if (to_find == NULL)
            to_find = find_in_children(curr, name);
        if (to_find)
            return to_find;
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
            if (!includes(child->token->type, FN_DEC, PROTO, 0))
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

char *type_name(Node *type) {
    switch (type->token->type) {
    case INT: {
        int shift = type->token->shift;
        return shift == 1 ? "bool" : format("i%d", shift);
    }
    case REF:
        return format("%s[]", type_name(type->data_type));
    case VOID:
        return "void";
    default:
        return (char *)to_string(type->token->type);
    }
}

Node *type_of(Node *node) {
    switch (node->token->type) { // clang-format off
    case INT:       return node;
    case CHARS:     return new_type(REF, 0, new_type(INT, 8, NULL));
    case VAR: case PARAM: return node->data_type;
    case FN_CALL:   return node->to_call ? ret_type_of(node->to_call) : NULL;
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
    if (a->token->type == REF)
        return same_type(a->data_type, b->data_type);
    return a->token->shift == b->token->shift;
}

bool is_number(Node *node) {
    return node->token->type == INT && node->token->shift != 1;
}

void check_type(Node *value, Node *expected, char *help) {
    Node *found = type_of(value);
    if (found == NULL || expected == NULL)
        return;
    Token *token = value->token;
    if (is_number(value) && is_number(expected)) {
        long long max = (1LL << (expected->token->shift - 1)) - 1;
        long long number = token->num.value;
        if (number < -max - 1 || number > max)
            add_error(token->s, token->e, format("%lld doesn't fit in %s", number, type_name(expected)),
                      format("%s goes from %lld to %lld", type_name(expected), -max - 1, max));
        return;
    }
    if (!same_type(found, expected))
        add_error(token->s, token->e,
                  format("expected '%s', found '%s'", type_name(expected), type_name(found)), help);
}

void check_same_type(Node *left, Node *right) {
    if (is_number(left))
        check_type(left, type_of(right), NULL);
    else
        check_type(right, type_of(left), NULL);
}

void analyze(Node *node) {
    switch (node->token->type) { // clang-format off
    case INT: case VAR: case PARAM: case CHARS: case VARIADIC: case REF: { // clang-format on
        break;
    }
    case ID: {
        Node *var = find_variable(node->token->name);
        if (var) {
            node->token->type = var->token->type;
            node->data_type = var->data_type;
            node->declared = var;
        } else {
            Token *token = node->token;
            add_error(token->s, token->e, format("undeclared variable '%s'", token->name),
                      format("declare it first, like: %s i32 = 0", token->name));
        }
        break;
    }
    case DECLARE: { // right is the data type
                    // TODO: protect from redeclaring
        analyze(node->var->ret_type);
        break;
    }
    case SCOPE: case FN_DEC: case PROTO: {
        enter_scope(node);
        if (node->ret_type)
            analyze(node->ret_type);
        if (node->args) {
            for (int i = 0; i < node->args->children_count; i++) {
                Node *arg = node->args->children[i];
                analyze(arg);
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
    }
    case IF: case ELIF: case ELSE: case LOOP: {
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
    }
    case BRK: case CNT: {
        break;
    }
    case NEG: {
        analyze(node->operand);
        break;
    }
    case FN_CALL: {
        Token *token = node->token;
        char *name = token->name;
        node->to_call = find_function(name);
        if (node->to_call == NULL) {
            Node *like = find_function_like(name, 2);
            if (find_variable(name))
                add_error(token->s, token->e, format("'%s' is a variable, not a function", name), NULL);
            else
                add_error(token->s, token->e, format("no function named '%s'", name),
                          like ? format("did you mean '%s'?", like->token->name) : NULL);
            break;
        }
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
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
        if (!includes(node->token->type, AND, OR, 0))
            check_same_type(node->left, node->right);
        else if (!type_of(node->left) || same_type(type_of(node->left), bool_type()))
            check_type(node->right, bool_type(), NULL);
        else
            check_type(node->left, bool_type(), NULL);
        break;
    }
    default: { // clang-format off
		eprint("%s:%d handle this case %t\n", 
			FILE, LINE, node->token->type);
		exit(1);
		break;
	} // clang-format on
    }
}

// CODE GEN
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
    exit_scope();
}

void codegen_proto(Node *node) {
    Ucg *ret_type = NULL;
    if (node->left) {
        // TODO: add return type
        codegen(node->left);
        ret_type = node->left->token->ucg;
    } else
        ret_type = ucg_void_type();

    Node **children = node->args->children;
    int count = node->args->children_count;
    Ucg **args = NULL;
    if (count != 0) {
        args = arena_alloc(count, sizeof(Ucg*));
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
    node->token->ucg = ucg_proto(node->token->name, ret_type, args, count);
}

void codegen_protos(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(child->token->type, FN_DEC, PROTO, 0))
            codegen_proto(child);
    }
}

void codegen_statements(Node *node) {
    for (int i = 0; i < node->children_count; i++) {
        Node *child = node->children[i];
        if (includes(child->token->type, FN_DEC, PROTO, DECLARE, 0))
            continue;
        codegen(child);
    }
}

void codegen(Node *node) {
    Ucg *ucg = NULL;
    switch (node->token->type) {
    case VAR: case PARAM: {
        if (node->declared)
            node->token->ucg = node->declared->token->ucg;
        break;
    }

    case INT: {
        if (node->token->is_type) {
            ucg = ucg_int_type(node->token->shift);
        } else {
            ucg = ucg_int_value(node->token->shift, node->token->num.value);
        }
        node->token->ucg = ucg;
        break;
    }
    case REF: {
        codegen(node->data_type);
        node->token->ucg = ucg_ref_type(node->data_type->token->ucg);
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
        codegen_protos(node);
        bool has_main = false;
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (child->token->type == DECLARE)
                codegen(child);
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
            if (child->token->type == FN_DEC)
                codegen(child);
        }
        exit_scope();
        break;
    }
    case FN_DEC: {
        Node *parent = ura.scope;
        enter_scope(node);
        codegen_protos(node);
        ucg_entry(node->token->ucg);
        if (parent->token->type == SCOPE && strcmp2(node->token->name, "main"))
            codegen_statements(parent);
        for (int i = 0; i < node->children_count; i++) {
            Node *child = node->children[i];
            if (child->token->type == PROTO)
                continue;
            codegen(child);
        }
        ucg_end(node->token->ucg);
        exit_scope();
        break;
    }
    case FN_CALL: {
        Ucg **args = arena_alloc(node->children_count + 1, sizeof(Ucg *));
        for (int i = 0; i < node->children_count; i++) {
            codegen(node->children[i]);
            args[i] = node->children[i]->token->ucg;
        }
        Ucg *fn = node->to_call->token->ucg;
        node->token->ucg = ucg_fn_call(fn, args, node->children_count);
        break;
    }
    case RETURN: {
        if (node->left) {
            codegen(node->left);
            ucg = ucg_ret(node->left->token->ucg);
        } else
            ucg = ucg_return();
        node->token->ucg = ucg;
        break;
    }
    case IF: {
        Ucg *if_stmt = ucg_if();
        codegen_branch(node, if_stmt);
        for (Node *branch = node->next; branch; branch = branch->next) {
            Ucg *stmt = branch->token->type == ELIF ? ucg_elif(if_stmt) : ucg_else(if_stmt);
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
        ucg_brk(node->loop->token->ucg);
        break;
    }
    case NEG: {
        codegen(node->operand);
        node->token->ucg = ucg_neg(node->operand->token->ucg);
        break;
    }
    case CNT: {
        ucg_cnt(node->loop->token->ucg);
        break;
    }
    case ASSIGN: // clang-format off
	case ADD: case SUB: case MUL: case DIV: case MOD: // math
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: // composite 
	case DIV_ASSIGN: case MOD_ASSIGN: 
	case LT: case GT: case EQ: case LE: case GE: case NQ: // comparision
	case AND: case OR: { // clang-format on
        print("=========================\n");
        print("%n", node);
        print("=========================\n");
        codegen(node->left);
        codegen(node->right);
        // exit(1);
        ucg = ucg_op(node->token->type, // clang-format off
			node->left->token->ucg, 
			node->right->token->ucg
		); // clang-format on
        node->token->ucg = ucg;
        break;
    }
    default: { // clang-format off
		eprint("%s:%d handle this case %t\n", 
			FILE, LINE, node->token->type
		);
		exit(1);
		break;
	} // clang-format on
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
        tokenize(file, 0);
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
    ucg_finish();


    // gen_bin();
    // return exec_bin();
#            endif // ASM
#        endif     // ANALYZE
#    endif         // AST
#endif             // TOKENIZE
    printf("did allocate %zu\n", arena_size());
}

// UTILS / FORMATING / PRINTING
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

    if (!includes(node->token->type, BRK, CNT, FN_CALL, 0)) {
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
    uraError error = {.s = s, .e = e, .msg = msg, .help = help};
    append(ura.errors, error);
}

void check_errors(void) {
    if (ura.errors_count == 0)
        return;
    char *buff = ura.file->buff;
    char *path = ura.file->path;
    char *cwd = getcwd2();
    size_t cwd_len = strlen(cwd);
    if (strncmp2(path, cwd, cwd_len) && path[cwd_len] == '/')
        path += cwd_len + 1;
    for (int i = 0; i < ura.errors_count; i++) {
        uraError *error = &ura.errors[i];
        int line = 1;
        int start = 0;
        for (int j = 0; j < error->s; j++) {
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

        fprintf(stderr, "%s: %s:%d:%d %s\n", RED("error"), path, line, col + 1, error->msg);
        fprintf(stderr, "%*s |\n", width, "");
        fprintf(stderr, "%d | %.*s\n", line, end - start, buff + start);
        fprintf(stderr, "%*s | %*s", width, "", col, "");
        for (int j = error->s; j < error->e || j == error->s; j++)
            fprintf(stderr, "^");
        fprintf(stderr, "\n");
        if (error->help)
            fprintf(stderr, "help: %s\n", error->help);
    }
    fprintf(stderr, "%s: aborting due to %d error%s\n", RED("error"),
            (int)ura.errors_count, ura.errors_count > 1 ? "s" : "");
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