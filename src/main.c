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

// clang-format off
const char *to_string(Type type) {
    char *types[END + 1] = {
        [IDENTIFIER] = "IDENTIFER",
        [VOID] = "VOID", [I32] = "I32", [BOOL] = "BOOL",

        [LPARENT] = "LPARENT", [RPARENT] = "LPARENT",
        [DOTS] = "DOTS",

        [ASSIGN] = "ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL", 
        [DIV] = "DIV", [MOD] = "MOD",

        [GT] = "GT", [LT] = "LT", [GE] = "GE", 
        [LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

        [FDEC] = "FDEC", [ARGS] = "ARGS", [COMA] = "COMA",
        [RETURN] = "RETURN", 
        [FCALL] = "FCALL",

        [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",

        [DEC_VAR] = "DEC_VAR", [LOAD_VAR] = "LOAD_VAR",
        [END] = "END",
    };
    if (types[type] == NULL)
        return "UNKNOWN";
    return types[type];
}
// clang-format on

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

int _eprint(char *file, int line, char *fmt, ...) {
    ura.errors_count++;
    va_list args;
    va_start(args, fmt);
    int r = fprintf(stderr, RED("%s:%d") " ", file, line);
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

    print_helper(elems, node->left, depth + 2);
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
void new_file(char *filename) {
    uraFile *file = ura_alloc(1, sizeof(uraFile));
    if (file == NULL) {
        eprint("calloc failed\n");
        return;
    }
    file->filename = filename;
    push_back(ura.files, file);

    File fp = fopen(filename, "r");
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
}

void close_file(uraFile *file) {
    free(file->filename);
    free(file->content);
    free(file);
}

void free_token(Token *token) {
    free(token->name);
    free(token);
}

void free_node(Node *node) {
    if (!node)
        return;
    for (size_t i = 0; i < node->children_count; i++)
        free_node(node->children[i]);
    free_node(node->left);
    if (node->token->type != FCALL)
        free_node(node->right);
    free(node->children);
    free(node->functions);
    free(node->variables);
    free(node);
}

void ura_clean(void) {
    free_node(ura.ast);
    for (size_t i = 0; i < ura.tokens_count; i++)
        free_token(ura.tokens[i]);
    for (size_t i = 0; i < ura.files_count; i++)
        close_file(ura.files[i]);
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
    switch (type) {
    case IDENTIFIER: {
        // if(e - s == 0) break;
        struct {
            char *value;
            Token token;
        } keywords[] = {
            { "i32", { .type = I32, .is_type = true } },
            { "b1", { .type = BOOL, .is_type = true } },
            { "True", { .type = BOOL, .b1 = { .value = true } } },
            { "False", { .type = BOOL, .b1 = { .value = false } } },
            { "fn", { .type = FDEC } },
            { "return", { .type = RETURN } },
            { "if", { .type = IF } },
            { "elif", { .type = ELIF } },
            { "else", { .type = ELSE } },
            { NULL },
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

void tokenize(char *content) {
    ura.curr_content = content;
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
        // clang-format off
        struct {
            char *value;
            Type type;
        } specials[] = {
            {"(", LPARENT}, {")", RPARENT}, {":", DOTS}, 
            {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
            {">=", GE}, {"<=", LE}, {">", GT}, {"<", LT},
            {"==", EQ}, {"!=", NQ},
            {"=", ASSIGN},
            {NULL, NONE}
        };
        // clang-format on
        for (int i = 0; specials[i].value; i++) {
            size_t len = strlen(specials[i].value);
            if (strncmp(specials[i].value, content + e, len) == 0) {
                parse_token(specials[i].type, 0, 0, space);
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
    return new;
}

Node *prime_node(void) {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) {
    case I32:
    case BOOL: {
        return new_node(token);
    }
    case IDENTIFIER: {
        Token *next_token =
            peek(0)->is_type && includes(peek(0)->type, I32, BOOL, 0) ? peek(0) : NULL;
        if (next_token) {
            next();
            token->type = next_token->type;
            node = new_node(new_token(DEC_VAR, token->space));
            node->left = new_node(token);
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

        Token *ret_token =
            peek(0)->is_type && includes(peek(0)->type, I32, BOOL, 0) ? peek(0) : NULL;
        if (ret_token) {
            next();
            node->token->ret_type = ret_token->type;
        } else
            node->token->ret_type = VOID;

        if (next()->type != DOTS)
            eprint("Expected : after function declaration\n");

        Node *last = NULL;
        while (inside(node->token->space)) {
            last = expr_node(0);
            push_back(node->children, last);
        }

        if (last == NULL || last->token->type != RETURN) {
            Node *ret = new_node(new_token(RETURN, node->token->space));
            ret->left = new_node(new_token(ret_token->type, node->token->space));
            push_back(node->children, ret);
        }
        exit_scope();
        return node;
    }
    case LPARENT: {
        next(); // skip (
        node = expr_node(0);
        if (next()->type != RPARENT)
            eprint("Expected )\n");
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
        while (inside(node->token->space))
            push_back(node->children, expr_node(0));

        Node *curr = node;
        while (inside(node->token->space - TAB)) { // TODO: this might overflow
            Token *next_token = peek(0);
            if (!includes(next_token->type, ELIF, ELSE, 0))
                break;

            curr->right = new_node(next());
            curr = curr->right;

            if (next_token->type == ELIF) {
                curr->left = expr_node(0); // condition
                if (next()->type != DOTS) {
                    eprint("expected dots after elif\n");
                    exit(0);
                }
                while (inside(curr->token->space))
                    push_back(curr->children, expr_node(0));
            } else if (next_token->type == ELSE) {
                if (next()->type != DOTS) {
                    eprint("expected dots after elif\n");
                    exit(0);
                }
                while (inside(curr->token->space))
                    push_back(curr->children, expr_node(0));
            }
        }

        return node;
    }
    default: { // TODO: replace this with unexpected token
        eprint("handle this case %t\n", token->type);
        break;
    }
    }
    return NULL;
}

Node *expr_node(int min_op) {
    Node *left = prime_node();
    while (true) {
        // clang-format off
        int prec[END + 1] = {
            [ASSIGN] = 1,

            [GT] = 8, [LT] = 8,
            [GE] = 8, [LE] = 8,
            [EQ] = 7, [NQ] = 7,

            [ADD] = 10, [SUB] = 10, 
            [MUL] = 11, [DIV] = 11, [MOD] = 11,
        };
        // clang-format on
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

Token *find_variable(char *name) {
    for (size_t i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        for (size_t i = 0; i < scope->variables_count; i++) {
            Node *curr = scope->variables[i];
            if (strcmp(curr->token->name, name) == 0)
                return scope->variables[i]->token;
        }
    }
    eprint("variable '%s' not found", name);
    exit(1);
    return NULL;
}

bool match(Node *op, Node *left, Node *right) {
    return false;
}

void analyze(Node *node) {
    if (ura.errors_count)
        return;
    switch (node->token->type) {
    case IDENTIFIER: {
        node->left = new_node(find_variable(node->token->name));
        // TODO: handle not found
        node->token->type = LOAD_VAR;
        break;
    }
    case DEC_VAR: {
        // TODO: later on try declaring structs/enum at the bottom
        declare_variable(node->left);
        break;
    } // clang-format off
    case I32: case BOOL: {
        // clang-format on
        break;
    }
    case ASSIGN: {
        if (node->left->token->type == DEC_VAR)
            analyze(node->left);
        else
            node->left->token = find_variable(node->left->token->name);
        analyze(node->right);
        break;
    } // clang-format off
    case GT: case LT: case GE: case LE: case EQ: case NQ:
    case SUB: case ADD: case MUL: case DIV: case MOD: {
        // clang-format on
        analyze(node->left);
        analyze(node->right);
        // TODO: check compatibility
        break;
    }
    case FCALL: {
        node->right = find_function(node->token->name);
        // TODO: check compatibility
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze(node->left->children[i]);
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
    case RETURN: {
        // TODOL check compatibility
        analyze(node->left);
        break;
    } // clang-format off
    case IF: {
        // clang-format on
        enter_scope(node);
        // TODO: check condition type is boolean
        analyze(node->left);
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
        node->left->token->llvm.elem = create_variable(node->left);
        node->token = node->left->token;
        break;
    } // clang-format off
    case I32: case BOOL :{
        // clang-format on
        node->token->llvm.elem = create_value(node->token);
        break;
    }
    case LOAD_VAR: {
        Token *var = node->left->token;
        node->token->llvm.elem = create_load(var);
        node->token->type = var->type;
        break;
    } // clang-format off
    case SUB: case ADD: case MUL: case DIV: case MOD: {
        // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        node->token->llvm.elem = create_math_op(node->left->token, node->token, node->right->token);
        break;
    } // clang-format off
    case GT: case LT: case GE: case LE: case EQ: case NQ: {
        // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        node->token->llvm.elem =
            create_comparision_op(node->left->token, node->token, node->right->token);
        break;
    }
    case ASSIGN: {
        if (node->left->token->type == DEC_VAR)
            code_gen(node->left);
        code_gen(node->right);
        if (ura.errors_count)
            break;
        // TODO: check compatibility
        node->token->llvm.elem = create_assign(node->left->token, node->right->token);
        break;
    }
    case FDEC: {
        enter_scope(node);

        Token *token = node->token;
        if (token->llvm.func_type)
            return;
        create_function(node);
        create_entry(token);

        // extract parameters as variables
        for (size_t i = 0; i < node->left->children_count; i++) {
            code_gen(node->left->children[i]);
            create_param(node->token, node->left->children[i]->token, i);
        }
        // code gen children
        gen_body(node);
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
        Bloc end = create_bloc("endif");
        for (Node *curr = node; curr; curr = curr->right) {
            enter_scope(curr);
            if (curr->token->type == ELSE) {
                gen_body(curr);
                create_branch(end);
                exit_scope();
                break;
            }
            Bloc then = create_bloc("then");
            Bloc next;
            if (curr->right)
                next = create_bloc(curr->right->token->type == ELSE ? "else" : "elif");
            else
                next = end;


            code_gen(curr->left);
            create_condition_branch(curr->left->token->llvm.elem, then, next);
            gen_body(curr);
            create_branch(end);
            exit_scope();
            position_at(next);
            position_last(end);
        }
        break;
    }
    default:
        eprint("handle this case %t\n", node->token->type);
        exit(1);
        break;
    }
}

void generate_asm(void) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    asm_init("ura-module");
    for (size_t i = 0; i < ura.ast->children_count; i++)
        code_gen(ura.ast->children[i]);
    exit_scope();
    asm_finalize("build/out.ll");
}

/*
TODO:
    + start creating an abstraction on top of llvm
    + if/elif/else
    + while
    + break/continue
    + ref arguments
    + struct
    + function inside function
*/

void print_nodes(char *text) {
    if (ura.errors_count)
        return;
    print(text);
    for (size_t i = 0; i < ura.ast->children_count; i++)
        print_node(ura.ast->children[i]);
}

int main() {
    new_file(strdup("./file.ura"));
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
        print(GREEN("============TOKENIZE============\n"));
        tokenize(file->content);
        generate_ast();
        print_nodes(GREEN("============AST=================\n"));
        generate_ir();
        print_nodes(GREEN("============IR==================\n"));
#if 1
        generate_asm();
        print_nodes(GREEN("============ASM==================\n"));
#endif
        print(GREEN("============CLEANING============\n"));
        ura_clean();
    }
    free(ura.tokens);
    free(ura.files);
}
