#include "header.h"

Ura ura;

// MEMORY/ERROR/LOGGING HANDLING
void *ura_alloc(size_t count, size_t size) {
    void *res = calloc(count, size);
    if (!res) {
        eprint("ura_alloc failed\n");
    }
    return res;
}

const char *to_string(Type type) {
    char *types[END + 1] = {
        // clang-format off
        [IDENTIFIER] = "IDENTIFER",
        [VOID] = "VOID", [I32] = "I32", [BOOL] = "BOOL",
        [REF] = "REF", [OWN] = "OWN", [DREF] = "DREF",

        [LPARENT] = "LPARENT", [RPARENT] = "RPARENT",
        [DOTS] = "DOTS",

        [ASSIGN] = "ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", 
        [DIV] = "DIV", [MOD] = "MOD",

        [GT] = "GT", [LT] = "LT", [GE] = "GE", 
        [LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

        [AND] = "AND", [OR] = "OR",

        [FDEC] = "FDEC", [ARGS] = "ARGS", [COMA] = "COMA",
        [RETURN] = "RETURN", 
        [FCALL] = "FCALL",

        [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
        [WHILE] = "WHILE", [BRK] = "BRK", [CNT] = "CNT",

        [DEC_VAR] = "DEC_VAR", [VAR] = "VAR",  [LOAD_VAR] = "LOAD_VAR",

        [END] = "END",
        // clang-format on
    };
    if (types[type] == NULL)
        return "UNKNOWN";
    return types[type];
}

int _print(File fp, const char *fmt, va_list args) {
    int r = 0;
    for (int i = 0; fmt[i];) {
        if (fmt[i] != '%') {
            r += fprintf(fp, "%c", fmt[i]);
            i++;
            continue;
        }

        int prefix = 0;
        if (strncmp(fmt + i, "%i", 2) == 0) {
            prefix = va_arg(args, int);
            i += 2;
            // don't continue
        }

        if (prefix > 0)
            r += fprintf(fp, "%*s", prefix, "");
        if (fmt[i] != '%') /* %i used on its own */
            continue;

#define macro(value, type)                             \
    if (strncmp(fmt + i, value, strlen(value)) == 0) { \
        r += fprintf(fp, value, va_arg(args, type));   \
        i += strlen(value);                            \
        continue;                                      \
    }

        macro("%s", char *);
        macro("%d", int);
        macro("%f", double); // TODO: to be checked
#undef macro

        if (strncmp(fmt + i, "%%", 2) == 0) {
            r += fprintf(fp, "%%");
            i += 2;
            continue;
        }
        if (strncmp(fmt + i, "%t", 2) == 0) {
            Type type = va_arg(args, Type);
            r += fprintf(fp, "%s", to_string(type));
            i += 2;
            continue;
        }
        if (strncmp(fmt + i, "%k", 2) == 0) {
            Token *token = va_arg(args, Token *);
            r += fprintf(fp, "%s", token ? to_string(token->type) : "(null token)");
            if (token->name)
                r += fprintf(fp, " name (%s)", token->name);
            switch (token->type) {
            case IDENTIFIER: {
                break;
            }
            case I32: {
                if (token->name || token->is_type)
                    break;
                r += fprintf(fp, " value (%ld)", token->i32.value);
                break;
            }
            case BOOL: {
                if (token->name || token->is_type)
                    break;
                r += fprintf(fp, " value (%s)", token->b1.value ? "True" : "False");
                break;
            }
            default:
                break;
            }
            r += fprintf(fp, " space (%ld)", token->space);
            if (token->ret_type)
                r += fprintf(fp, " ret (%s)", to_string(token->ret_type));
            i += 2;
            continue;
        }

        printf("%s:%d unhandled case %s\n", RED(FILE), LINE, fmt + i);
        break;
    }
    return r;
}

int _eprint(char *file, const char *func, int line, char *fmt, ...) {
    ura.errors_count++;
    va_list args;
    va_start(args, fmt);
    int r = fprintf(stderr, RED("%s:%d %s") " ", file, line, func);
    r += _print(stderr, fmt, args);
    va_end(args);
    return r;
}

int print(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = _print(stdout, fmt, args);
    va_end(args);
    return r;
}

char *format(char *fmt, ...) {
    char *buf = NULL;
    size_t size = 0;
    File out = open_memstream(&buf, &size);
    if (!out) {
        eprint("format: open_memstream failed\n");
        return NULL;
    }

    va_list ap;
    va_start(ap, fmt);
    _print(out, fmt, ap);
    va_end(ap);
    fclose(out);
    return buf;
}

// TODO: this code needs to be reviewed
typedef struct NodePrint NodePrint;
struct NodePrint {
    expand(Node *, nodes);
    expand(int, depths);
};

void print_helper(NodePrint *elems, Node *node, int depth) {
    if (!node)
        return;

    push_back(elems->nodes, node);
    push_back(elems->depths, depth);

    if (!includes(node->token->type, BRK, CNT, 0))
        print_helper(elems, node->left, depth + 2);
    if (!includes(node->token->type, FCALL, 0))
        print_helper(elems, node->right, depth + 2);
    for (size_t i = 0; i < node->children_count; i++)
        print_helper(elems, node->children[i], depth + 2);
}

int still_open(NodePrint *elems, size_t i, int level) {
    for (size_t j = i + 1; j < elems->nodes_count; j++)
        if (elems->depths[j] <= level)
            return elems->depths[j] == level;
    return 0;
}

void print_node(Node *node) {
    NodePrint elems = {};

    print_helper(&elems, node, 0);

    for (size_t i = 0; i < elems.nodes_count; i++) {
        int depth = elems.depths[i];
        for (int level = 1; level < depth; level++)
            print("%s", still_open(&elems, i, level) ? "│ " : "  ");
        if (depth)
            print("%s", still_open(&elems, i, depth) ? "├─" : "└─");
        print("%k\n", elems.nodes[i]->token);
    }

    free(elems.nodes);
    free(elems.depths);
}

// FILE HANDLING
void new_file(char *name) {
    uraFile *file = ura_alloc(1, sizeof(uraFile));
    if (file == NULL) {
        eprint("calloc failed\n");
        return;
    }
    // TODO: check this one when implementing arena allocator
    file->name = strdup(name);
    push_back(ura.files, file);

    char *slash = strrchr(name, '/');
    if (slash) {
        file->dir = strdup(name);
        file->dir[slash - name] = '\0';
        file->base = strdup(slash + 1);
    } else {
        file->dir = strdup(".");
        file->base = strdup(name);
    }
    char *dot = strrchr(file->base, '.');
    if (dot)
        *dot = '\0';
    file->build_dir = format("%s/build", file->dir);
    mkdir(file->build_dir, 0755);
    file->ll_path = format("%s/%s.ll", file->build_dir, file->base);

    File fp = fopen(name, "r");
    if (fp == NULL) {
        eprint("fopen failed\n");
        return;
    }
    // TODO: check those to if they failed
    fseek(fp, sizeof(char), SEEK_END);
    file->len = (size_t)ftell(fp);
    rewind(fp);
    file->content = ura_alloc(file->len + 1, sizeof(char));
    fread(file->content, file->len, sizeof(char), fp);
    fclose(fp);
}

void close_file(uraFile *file) {
    free(file->name);
    free(file->dir);
    free(file->base);
    free(file->build_dir);
    free(file->ll_path);
    free(file->content);
    free(file);
}

void free_token(Token *token) {
    free(token->name);
    free(token);
}

void free_node(Node *node) {
    free(node->children);
    free(node->functions);
    free(node->variables);
    free(node);
}

void ura_clean(void) {
    for (size_t i = 0; i < ura.tokens_count; i++)
        free_token(ura.tokens[i]);
    for (size_t i = 0; i < ura.files_count; i++)
        close_file(ura.files[i]);
    for (size_t i = 0; i < ura.nodes_count; i++)
        free_node(ura.nodes[i]);
}
// TOKENIZE
Token *new_token(Type type, size_t space) {
    Token *new = ura_alloc(1, sizeof(Token));
    new->type = type;
    new->space = space;
    push_back(ura.tokens, new);
    return new;
}

Token *parse_token(Type type, size_t s, size_t e, size_t space) {
    Token *new = new_token(type, (space / TAB + (space % TAB != 0 ? 1 : 0)));
    new->type = type;
    space = new->space;
    switch (type) {
    case IDENTIFIER: {
        // if(e - s == 0) break;
        struct {
            char *value;
            Token token;
        } keywords[] = {
            // clang-format off
            { "i32", { .type = I32, .is_type = true } },
            { "b1", { .type = BOOL, .is_type = true } },
            { "True", { .type = BOOL, .b1 = { .value = true } } },
            { "False", { .type = BOOL, .b1 = { .value = false } } },
            { "fn", { .type = FDEC } }, { "return", { .type = RETURN } },
            { "if", { .type = IF } }, { "elif", { .type = ELIF } },
            { "else", { .type = ELSE } },
            { "while", { .type = WHILE } }, { "break", { .type = BRK } },
            { "continue", { .type = CNT } },
            { "own", { .type = OWN } }, { "ref", { .type = REF } },
            { "dref", { .type = DREF } },
            {"and", { .type = AND } }, {"or", { .type = OR } },
            { NULL },
            // clang-format on
        };
        size_t i = 0;
        for (; keywords[i].value; i++) {
            char *value = keywords[i].value;
            size_t len = strlen(value);
            if (e - s == len && strncmp(ura.curr_content + s, value, e - s) == 0) {
                *new = keywords[i].token;
                new->space = space;
                break;
            }
        }
        if (keywords[i].value)
            break;

        new->name = ura_alloc(e - s + 1, sizeof(char));
        strncpy(new->name, ura.curr_content + s, e - s);
        break;
    }
    case I32: {
        // TODO: check INT_MIN, INT_MAX boundries
        new->i32.value = atoi(ura.curr_content + s);
        break;
    }
    default:
        break;
    }
    print("new token %k\n", new);
    return new;
}

void tokenize(uraFile *file) {
    ura.curr_content = file->content;
    char *content = file->content;
    size_t s = 0;
    size_t e = 0;
    size_t space = 0;
    while (content && content[e]) {
        s = e;
        if (isspace(content[e])) {
            while (content[e] == '\n') {
                space = 0;
                e++;
            }
            if (content[s] == '\n')
                s = e - 1;
            while (isspace(content[e]) && content[e] != '\n') {
                if (content[s] == '\n')
                    space++;
                e++;
            }
            continue;
        }

        while (strncmp(content + s, "//", 2) == 0 && content[e] && content[e] != '\n')
            e++;
        if (e != s)
            continue;

        while (isalpha(content[s]) && (isalnum(content[e]) || content[e] == '_'))
            e++;
        if (e != s) // found identifier
        {
            parse_token(IDENTIFIER, s, e, space);
            continue;
        }

        // bool is_float = false;
        while (isdigit(content[e]))
            e++;
        if (e != s) {
            parse_token(I32, s, e, space);
            continue;
        }
        struct {
            char *value;
            Type type;
        } specials[] = {
            // clang-format off
            {"(", LPARENT}, {")", RPARENT}, {":", DOTS}, 
            {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
            {">=", GE}, {"<=", LE}, {">", GT}, {"<", LT},
            {"==", EQ}, {"!=", NQ},
            {"=", ASSIGN}, {",", COMA},
            {"&&", AND}, {"||", OR},
            {NULL, NONE}
            // clang-format on
        };
        for (int i = 0; specials[i].value; i++) {
            size_t len = strlen(specials[i].value);
            if (strncmp(specials[i].value, content + e, len) == 0) {
                parse_token(specials[i].type, 0, 0, space);
                if (specials[i].type == DOTS)
                    space += TAB;
                e += len;
            }
        }
        if (e != s)
            continue;
        eprint("handle this case: <%s>\n", content + e);
        break;
    }
    parse_token(END, 0, 0, 0);
}

// ABSTRACT SYNTAX TREE
bool includes(Type to_find, ...) {
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

bool inside(size_t space) {
    return ura.tokens[ura.exe_pos]->space > space && ura.errors_count == 0;
}

Token *peek(size_t index) {
    if (ura.exe_pos + index > ura.tokens_count) {
        eprint("index out of range\n");
        return NULL;
    }
    return ura.tokens[ura.exe_pos + index];
}

Token *expect(Type type) {
    return NULL;
}

Token *next(void) { // TODO: protect if next is END, don't advanced
    Token *token = peek(0);
    ura.exe_pos++;
    return token;
}

Node *new_node(Token *token) {
    Node *new = ura_alloc(1, sizeof(Node));
    new->token = token;
    print("new node %k\n", new->token);
    push_back(ura.nodes, new);
    return new;
}

void parse_bloc(Node *parent) {
    while (inside(parent->token->space)) {
        Node *child = expr_node(0);
        if (child->token->type == ASSIGN && child->left->token->type == DEC_VAR) {
            Node *dec = child->left;
            push_back(parent->children, dec);
            child->left = dec->left;
        }
        push_back(parent->children, child);
    }
}

// TODO: use it in return in FDEC parsing
Node *data_type_node(void) {
    Node *node = NULL;
    if (peek(0)->type == REF) {
        Node *node = new_node(peek(0));
        int p = 1;
        while (peek(p)->type == LPARENT)
            p++;
        if (peek(p)->type == REF) {
            eprint("a ref can't point to a ref: ref(ref(...)) is not allowed\n");
            exit(1);
        }

        int p2 = p;
        if (!peek(p)->is_type || !includes(peek(p)->type, I32, BOOL, 0)) {
            eprint("expected a data type after ref, got %k\n", peek(p));
            exit(1);
        }

        node->left = new_node(peek(p));
        p++; // skip the data type
        while (p2 > 1) {
            // print("%d: skip -> %k\n", LINE, peek(p));
            if (peek(p)->type != RPARENT) {
                eprint("expected closing )\n");
                exit(1);
            }
            p++;
            p2--;
        }
        ura.exe_pos += p;
        return node;
    }
    if (peek(0)->is_type && includes(peek(0)->type, I32, BOOL, 0))
        return new_node(next());
    return NULL;
}

Node *prime_node(void) {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) {
    case IDENTIFIER: {
        Node *next_elem = data_type_node();
        if (next_elem) {
            node = new_node(new_token(DEC_VAR, token->space));
            token->type = VAR;
            node->left = new_node(token);
            node->left->left = next_elem;
            return node;
        }
        if (peek(0)->type == LPARENT) // FCALL
        {
            next();
            token->type = FCALL;
            node = new_node(token);
            node->left = new_node(new_token(ARGS, node->token->space));
            while (!includes(peek(0)->type, RPARENT, 0)) {
                Node *arg = expr_node(0);
                push_back(node->left->children, arg);
                if (!includes(peek(0)->type, RPARENT, COMA, 0)) {
                    eprint("expect ',' between arguments");
                    break;
                } else if (peek(0)->type == COMA)
                    next();
            }
            if (!includes(peek(0)->type, RPARENT, 0)) {
                eprint("expect ')' between arguments");
                return node;
            }
            next();
            return node;
        }
        return new_node(token);
    } // clang-format off
    case I32: case BOOL: {
        // clang-format on
        return new_node(token);
    } // clang-format off
    case OWN: case DREF: {
        // clang-format on
        node = new_node(token);
        node->left = prime_node(); // TODO: expect identifier
        return node;
    }
    case SUB:
    case ADD: {
        node = new_node(token);
        node->right = prime_node();
        node->left = new_node(new_token(I32, node->token->space));
        node->left->token->i32.value = token->type == SUB ? -1 : 1;
        node->token->type = MUL;
        return node;
    }
    case LPARENT: {
        node = expr_node(0);
        if (peek(0)->type != RPARENT) {
            eprint("Expected )\n");
            exit(1);
        }
        next();
        return node;
    }
    case FDEC: {
        node = new_node(token);
        if (peek(0)->type != IDENTIFIER) {
            eprint("Expected identifer after fn\n");
            free_node(node);
            return NULL;
        }
        node->token->name = strdup(next()->name);
        enter_scope(node);

        if (next()->type != LPARENT)
            eprint("Expected ( after function declaration\n");
        node->left = new_node(new_token(ARGS, node->token->space));
        while (!includes(peek(0)->type, RPARENT, 0)) {
            Node *arg = prime_node();
            if (arg->token->type != DEC_VAR) {
                eprint("expected valid arguments\n");
                exit(1);
            }
            push_back(node->left->children, arg);

            if (!includes(peek(0)->type, RPARENT, COMA, 0)) {
                eprint("expect ',' between arguments");
                break;
            } else if (peek(0)->type == COMA)
                next();
        }
        if (next()->type != RPARENT)
            eprint("Expected ) after function declaration: %t\n", peek(0)->type);


        node->right = data_type_node();

        if (peek(0)->type != DOTS) {
            eprint("Expected : after function declaration\n");
            exit(1);
        }
        next();
        parse_bloc(node);

        exit_scope();
        return node;
    }
    case RETURN: {
        node = new_node(token);
        node->left = expr_node(0);
        return node;
    }
    case IF: {
        node = new_node(token);
        node->left = expr_node(0); // condition
        if (peek(0)->type != DOTS) {
            eprint("expected ':' after if statement\n");
            exit(1);
        }
        next();
        parse_bloc(node);

        Node *curr = node;
        while (inside(node->token->space - 1)) {
            // TODO: space - 1 this might overflow
            // space is size_t
            Token *next_token = peek(0);
            if (!includes(next_token->type, ELIF, ELSE, 0))
                break;

            curr->right = new_node(next());
            curr = curr->right;

            if (curr->token->type == ELIF) {
                curr->left = expr_node(0); // condition
                if (next()->type != DOTS) {
                    eprint("expected dots after elif\n");
                    exit(0);
                }
                parse_bloc(curr);
            } else if (curr->token->type == ELSE) {
                if (next()->type != DOTS) {
                    eprint("expected dots after else\n");
                    exit(0);
                }
                parse_bloc(curr);
            }
        }
        return node;
    }
    case WHILE: {
        node = new_node(token);
        node->left = expr_node(0); // condition
        if (peek(0)->type != DOTS) {
            eprint("expected ':' after while loop\n");
            exit(1);
        }
        next();
        parse_bloc(node);
        return node;
    } // clang-format off
    case BRK: case CNT:
        // clang-format on
        return new_node(token);
    default: { // TODO: replace this with unexpected token
        eprint("handle this case %t\n", token->type);
        exit(1);
        break;
    }
    }
    return NULL;
}

Node *expr_node(int min_op) {
    Node *left = prime_node();
    while (true) {
        int prec[END + 1] = {
            // clang-format off
            [ASSIGN] = 1,

            [OR] = 2, [AND] = 3,

            [GT] = 8, [LT] = 8,
            [GE] = 8, [LE] = 8,
            [EQ] = 7, [NQ] = 7,

            [ADD] = 10, [SUB] = 10, 
            [MUL] = 11, [DIV] = 11, [MOD] = 11,
            // clang-format on
        };
        int op = prec[peek(0)->type];
        if (op <= min_op)
            break;
        Node *node = new_node(next());
        node->left = left;
        node->right = expr_node(op);

        left = node;
    }
    return left;
}


void generate_ast(void) {
    if (ura.errors_count)
        return;
    ura.ast = new_node(new_token(0, 0));
    while (!includes(peek(0)->type, END, 0) && !ura.errors_count) {
        Node *child = expr_node(0);
        push_back(ura.ast->children, child);
    }
}

// INTERMEDIATE REPRESENTATION
void enter_scope(Node *node) {
    print("%s: %s\n", CYAN("enter scope"), node->token->name);
    ura.scope = node;
    push_back(ura.scopes, node);
}

void exit_scope() {
    print("%s: %s\n", CYAN("exit scope"), ura.scope->token->name);
    ura.scopes_count--;
    ura.scopes[ura.scopes_count] = NULL;
    if (ura.scopes_count == 0)
        ura.scope = NULL;
    else
        ura.scope = ura.scopes[ura.scopes_count - 1];
}

void declare_function(Node *fn) {
    Token *new = fn->token;
    // for (int i = 0; i < ura.scope->functions_count; i++) {
    //     // Token *old = ura.scope->functions[i]->token;
    //     // if (strcmp(old->name, new->name) != 0)
    //     //     continue;
    //     // if (!old->is_proto && !new->is_proto) {
    //     //     // parse_error(new, ERR_REDECL_FUNCTION, new->name);
    //     //     // parse_note(old, NOTE_PREV_DECL, new->name);
    //     //     return;
    //     // }
    //     // if (!same_signature(old, new)) {
    //     //     // parse_error(new, ERR_SIG_CONFLICT, new->name, signature_diff(old, new));
    //     //     // parse_note(old, NOTE_PREV_DECL, new->name);
    //     //     return;
    //     // }
    //     // if (old->is_proto && !new->is_proto)
    //     //     ura.scope->functions[i] = fn;
    //     return;
    // }
    // resize_array(ura.scope->functions, Node *);
    // ura.scope->functions[ura.scope->functions_count++] = fn;
    push_back(ura.scope->functions, fn);
}

Node *find_function(char *name) {
    for (size_t i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (size_t i = 0; i < scope->functions_count; i++) {
            Node *curr = scope->functions[i];
            if (strcmp(curr->token->name, name) == 0)
                return scope->functions[i];
        }
    }
    eprint("function '%s' not found", name);
    exit(1);
    return NULL;
}

void declare_variable(Node *node) {
    // print(CYAN("declare %s\n"), node->token->name);
    push_back(ura.scope->variables, node);
}

Node *find_variable(char *name) {
    for (size_t i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (size_t i = 0; i < scope->variables_count; i++) {
            Node *curr = scope->variables[i];
            if (strcmp(curr->token->name, name) == 0)
                return scope->variables[i];
        }
    }
    eprint("variable '%s' not found", name);
    exit(1);
    return NULL;
}

bool match(Node *op, Node *left, Node *right) {
    return false;
}

Node *pointer_type(Node *node) {
    Node *left = node->left;
    switch (node->token->type) {
    case LOAD_VAR: {
        Node *type = left->left;
        return type->token->type == REF ? type->left : NULL;
    }
    case OWN: {
        return left->token->type == LOAD_VAR ? left->left->left : NULL;
    }
    default:
        break;
    }
    return NULL;
}

void analyze(Node *node) {
    if (ura.errors_count)
        return;
    switch (node->token->type) {
    case IDENTIFIER: {
        node->left = find_variable(node->token->name);
        // TODO: handle not found
        node->token->type = LOAD_VAR;
        break;
    }
    case DEC_VAR: {
        // TODO: later on try declaring structs/enum at the bottom
        declare_variable(node->left);
        break;
    } // clang-format off
    case VAR: case I32: case BOOL: break; 
    case OWN: analyze(node->left); break; // clang-format on
    case DREF: {
        analyze(node->left);
        // LOAD_VAR -> VAR -> type: the variable must be declared as a ref
        if (pointer_type(node->left) == NULL) {
            eprint("dref() expects a ref variable\n");
            exit(1);
        }
        break;
    }
    case ASSIGN: {
        analyze(node->right);
        analyze(node->left);
        break;
    } // clang-format off
    case AND: case OR:
    case ADD: case SUB: case MUL: case DIV: case MOD:
    case GT: case LT: case GE: case LE: case EQ: case NQ: { // clang-format on
        analyze(node->left);
        analyze(node->right);
        // TODO: check compatibility
        break;
    }
    case FDEC: {
        enter_scope(node);
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze(node->left->children[i]);
        // code bloc
        for (size_t i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    }
    case FCALL: {
        node->right = find_function(node->token->name);
        // TODO: check compatibility
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze(node->left->children[i]);
        break;
    }
    case RETURN: {
        // TODOL check compatibility
        analyze(node->left);
        break;
    }
    case IF: {
        enter_scope(node);
        analyze(node->left); // TODO: check condition type is boolean
        for (size_t i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        Node *curr = node->right;
        while (curr) {
            enter_scope(curr);
            if (curr->left) // elif
                analyze(curr->left);
            for (size_t i = 0; i < curr->children_count; i++)
                analyze(curr->children[i]);
            curr = curr->right;
            exit_scope();
        }
        exit_scope();
        break;
    }
    case WHILE: {
        enter_scope(node);
        analyze(node->left); // TODO: check condition type is boolean
        for (size_t i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    } // clang-format off
    case BRK: case CNT: {
        // clang-format on
        for (size_t i = ura.scopes_count - 1; i >= 0; i--) {
            Node *scope = ura.scopes[i];
            if (includes(scope->token->type, WHILE, 0)) {
                node->left = scope;
                break;
            }
        }
        // TODO: handle if not inside 'while' loop or 'case'
        break;
    }
    default:
        eprint("handle this case %t\n", node->token->type);
        exit(1);
        break;
    }
    // return node;
}

void type_check(Node *node) {
}

void generate_ir(void) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    // skip last one because it's ura scope
    // TODO: to be cheked later because
    // we might need t odeclare function
    // inside function
    for (size_t i = 0; i < ura.ast->children_count; i++)
        if (ura.ast->children[i]->token->type == FDEC)
            declare_function(ura.ast->children[i]);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        analyze(ura.ast->children[i]);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        type_check(ura.ast->children[i]);
    exit_scope();
}

// ASSEMBLY
void gen_body(Node *node) {
    for (size_t i = 0; i < node->children_count; i++) {
        code_gen(node->children[i]);
        if (is_bloc_terminated())
            break;
    }
}
void code_gen(Node *node) {
    if (ura.errors_count)
        return;
    switch (node->token->type) {
    case DEC_VAR: {
        Node *var = node->left;
        Node *type = var->left;
        node->left->token->llvm.elem = create_variable(var->token, type);
        break;
    }
    case VAR: {
        break;
    }
    case LOAD_VAR: {
        Node *var = node->left;
        Node *type = var->left;
        node->token->llvm.elem = create_load(var->token, type);
        break;
    } // clang-format off
    case I32: case BOOL: {
        // clang-format on
        node->token->llvm.elem = create_value(node->token);
        break;
    }
    case DREF: {
        code_gen(node->left);
        Node *left = node->left;
        Node *ref = pointer_type(left);
        node->token->llvm.elem = create_dref(left->token->llvm.elem, ref);
        break;
    }
    case OWN: {
        node->token->llvm.elem = address_of(node->left);
        break;
    }
    case ASSIGN: {
        // code_gen(node->left);
        code_gen(node->right);
        if (ura.errors_count)
            break;
        // TODO: check compatibility
        node->token->llvm.elem = create_assign(node->left, node->right);
        break;
    } // clang-format off
    case ADD: case SUB: case MUL: case DIV: case MOD: { // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        Node *left = node->left;
        Node *right = node->right;
        node->token->llvm.elem = create_math_op(left->token, node->token, right->token);
        break;
    } // clang-format off
    case AND: case OR: {
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        Node *left = node->left;
        Node *right = node->right;
        node->token->llvm.elem = create_logic_op(left->token, node->token, right->token);
        break;
    }
    case GT: case LT: case GE: case LE: case EQ: case NQ: { // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        Node *left = node->left;
        Node *right = node->right;
        node->token->llvm.elem = create_comparision_op(left->token, node->token, right->token);
        break;
    }
    case FDEC: {
        enter_scope(node);

        Token *token = node->token;
        // if (!token->llvm.func_type)
        //     create_function(node);
        create_entry(token);

        // extract parameters as variables
        for (size_t i = 0; i < node->left->children_count; i++) {
            Node *dec = node->left->children[i];
            code_gen(dec);
            create_param(node->token, dec->left->token, i);
        }
        // code gen children
        gen_body(node);
        create_default_return(node);
        exit_scope();
        break;
    }
    case FCALL: {
        for (size_t i = 0; i < node->left->children_count; i++) {
            code_gen(node->left->children[i]);
        }
        node->token->llvm.elem = create_function_call(node);
        break;
    }
    case RETURN: {
        code_gen(node->left);
        node->token->llvm.elem = create_return(node->left->token);
        break;
    }
    case IF: {
        Bloc end = create_label("endif");
        for (Node *curr = node; curr; curr = curr->right) {
            enter_scope(curr);
            if (curr->token->type == ELSE) {
                gen_body(curr);
                create_jmp(end);
                exit_scope();
                break;
            }
            Bloc then = create_label("then");
            Bloc next;
            if (curr->right)
                next = create_label(curr->right->token->type == ELSE ? "else" : "elif");
            else
                next = end;

            code_gen(curr->left);
            create_jmp_condition(curr->left->token->llvm.elem, then, next);
            gen_body(curr);
            create_jmp(end);
            exit_scope();
            create_at(next);
            create_last_label(end);
        }
        break;
    }
    case WHILE: {
        Bloc cond = create_label("cond");
        Bloc then = create_label("then");
        Bloc end = create_label("endwhile");
        node->token->llvm.cond = cond;
        node->token->llvm.then = then;
        node->token->llvm.end = end;

        enter_scope(node);
        create_jmp(cond);
        code_gen(node->left);
        create_jmp_condition(node->left->token->llvm.elem, then, end);

        gen_body(node);
        create_jmp(cond);
        exit_scope();

        create_at(end);
        // create_last_label(end);
        break;
    }
    case BRK: {
        create_jmp_out(node->left->token->llvm.end);
        break;
    }
    case CNT: {
        create_jmp_out(node->left->token->llvm.cond);
        break;
    }
    default:
        eprint("handle this case %t\n", node->token->type);
        exit(1);
        break;
    }
}

void generate_asm(uraFile *file) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    asm_init("ura-module");
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        if (ura.ast->children[i]->token->type == FDEC) {
            create_function(ura.ast->children[i]);
        }
    }
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        code_gen(ura.ast->children[i]);
    }
    exit_scope();
    asm_finalize(file->ll_path);
}

void compile_executable(uraFile *file) {
    if (ura.errors_count)
        return;

    char *argv[] = { "clang", file->ll_path, "-o", ura.exec, NULL };

    pid_t pid = fork();
    if (pid < 0) {
        eprint("fork failed\n");
        return;
    }
    if (pid == 0) {
        execvp(argv[0], argv);
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        eprint("clang failed to compile %s\n", file->ll_path);
}


void print_nodes(char *text) {
    if (ura.errors_count)
        return;
    print(text);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        print_node(ura.ast->children[i]);
}

void parse_arguments(int ac, char **av) {
    if (ac < 2) {
        eprint("expected argument: ura <file_name>.ura\n");
        return;
    }
    ura.exec = "exe.out";
    for (int i = 1; i < ac && !ura.errors_count; i++) {
        char *arg = av[i];
        if (strcmp(arg, "-o") == 0) {
            if (i + 1 >= ac) {
                eprint("expected argument: ura <file_name>.ura\n");
                return;
            }
            ura.exec = av[++i]; // output file
        } else {
            size_t n = strlen(arg);
            bool is_ura = n > 4 && strcmp(arg + n - 4, ".ura") == 0;
            if (!is_ura) {
                eprint("Invalid file '%s'\n", arg);
                return;
            }
            new_file(arg);
        }
    }
}

/*
TODO:
    + logical and / or (short-circuit)
    + compound assignment: += -= *= /= %=
    + string literals and output()
    + for loops over a range: for i in 0..10
    + more integer types (i8, i64, unsigned) and casting with as
    + arrays: declaration and indexing
    + struct
    + function inside function
*/

int main(int ac, char **av) {
    parse_arguments(ac, av);
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
        print(GREEN("============TOKENIZE============\n"));
        tokenize(file);
        generate_ast();
        print_nodes(GREEN("============AST=================\n"));
#if 1
        generate_ir();
        print_nodes(GREEN("============IR==================\n"));
#    if 1
        generate_asm(file);
        print_nodes(GREEN("============ASM==================\n"));
        compile_executable(file);
#    endif
#endif
        print(GREEN("============CLEANING============\n"));
        ura_clean();
    }
    // TODO: to be check when compiling multiple files
    free(ura.tokens);
    free(ura.files);
    free(ura.nodes);
    return ura.errors_count != 0;
}
