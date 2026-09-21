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

        [VAR_DEC] = "VAR_DEC", [VAR] = "VAR", [VAR_LOAD] = "VAR_LOAD",

        [VOID] = "VOID", [BOOL] = "BOOL", [I8] = "I8", [I32] = "I32",
        [CHARS] = "CHARS",
        [REF] = "REF",

        [STRUCT_DEC] = "STRUCT_DEC", [DOT] = "DOT", [ATTR] = "ATTR",

        [OWN] = "OWN", [DREF] = "DREF",

        [LPARENT] = "LPARENT", [RPARENT] = "RPARENT",
        [DOTS] = "DOTS", [COMA] = "COMA",

        [ASSIGN] = "ASSIGN",
        [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
        [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
        [MOD_ASSIGN] = "MOD_ASSIGN",

        [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
        [DIV] = "DIV", [MOD] = "MOD",

        [GT] = "GT", [LT] = "LT", [GE] = "GE",
        [LE] = "LE", [EQ] = "EQ", [NQ] = "NQ",

        [AND] = "AND", [OR] = "OR",

        [PROTO] = "PROTO",
        [FN_DEC] = "FN_DEC", [ARGS] = "ARGS", [VARIADIC] = "VARIADIC",
        [FN_CALL] = "FN_CALL", [RETURN] = "RETURN",

        [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
        [WHILE] = "WHILE", [BRK] = "BRK", [CNT] = "CNT",

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
        macro("%c", int);
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
        if (strncmp(fmt + i, "%zu", 3) == 0) {
            size_t value = va_arg(args, size_t);
            r += fprintf(fp, "%zu", value);
            i += 3;
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
            case I8: {
                if (token->name || token->is_type)
                    break;
                r += fprintf(fp, " value (%c)", token->i8.value);
                break;
            }
            case CHARS: {
                if (token->name || token->is_type)
                    break;
                r += fprintf(fp, " value (%s)", token->chars.value);
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
        print_helper(elems, node->left, depth + 3);
    if (!includes(node->token->type, FN_CALL, 0))
        print_helper(elems, node->right, depth + 3);
    for (size_t i = 0; i < node->children_count; i++)
        print_helper(elems, node->children[i], depth + 3);
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
            print("%s", still_open(&elems, i, level) ? "│ " : " ");
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
    free(token->chars.value);
    free(token);
}

void free_node(Node *node) {
    free(node->children);
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
// gen_tokens
Token *new_token(Type type, size_t space) {
    Token *new = ura_alloc(1, sizeof(Token));
    new->type = type;
    new->space = space;
    push_back(ura.tokens, new);
    return new;
}


// TODO: parse for char type also
char *parse_escaped(char *input, size_t s, size_t e) {
    char *res = ura_alloc(e - s + 1, sizeof(char));
    size_t r = 0;
    while (s < e) {
        char c = 0;
        // clang-format off
        if(s < e && input[s] == '\\')
        {
            switch(input[s + 1]) { 
                case 'n':  c = '\n'; break;
                case 't':  c = '\t'; break;
                case 'r':  c = '\r'; break;
                case 'b':  c = '\b'; break;
                case 'f':  c = '\f'; break;
                case 'v':  c = '\v'; break;
                case 'a':  c = '\a'; break;
                case '\\': c = '\\'; break;
                case '"':  c = '\"'; break;
                case '\'': c = '\''; break;
                case '?':  c = '\?'; break;
                // TODO: add octal stuff etc...
                default: break;
            }
            if(c) s += 2;
            else c = input[s++];
        } 
        else c = input[s++];
        // clang-format on
        res[r++] = c;
    }
    return res;
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
            { "b1", { .type = BOOL, .is_type = true } },
            { "char", { .type = I8, .is_type = true } },
            { "i8", { .type = I8, .is_type = true } },
            { "i32", { .type = I32, .is_type = true } },
            { "chars", { .type = CHARS, .is_type = true } },
            { "ref", { .type = REF } },

            { "True", { .type = BOOL, .b1 = { .value = true } } },
            { "False", { .type = BOOL, .b1 = { .value = false } } },

            { "struct", { .type = STRUCT_DEC } },
            { "own", { .type = OWN } }, { "dref", { .type = DREF } },
            { "and", { .type = AND } }, { "or", { .type = OR } },
            { "proto", { .type = PROTO } }, { "fn", { .type = FN_DEC } },
            { "return", { .type = RETURN } },
            { "if", { .type = IF } }, { "elif", { .type = ELIF } },
            { "else", { .type = ELSE } },
            { "while", { .type = WHILE } }, { "break", { .type = BRK } },
            { "continue", { .type = CNT } },

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
    case CHARS: {
        new->chars.value = parse_escaped(ura.curr_content, s + 1, e - 1);
        break;
    }
    case I8: {
        // TODO: handle special characters later on
        new->i8.value = ura.curr_content[s + 1];
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

void gen_tokens(uraFile *file) {
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

        while (content[s] == '\"' && content[e] && (e == s || content[e] != '\"'))
            e++;
        if (e != s) // found CHARS
        {
            e++;
            parse_token(CHARS, s, e, space);
            continue;
        }

        // TODO: to be fixed later
        while (content[s] == '\'' && content[e] && (e == s || content[e] != '\''))
            e++;
        if (e != s) // found CHARS
        {
            e++;
            parse_token(I8, s, e, space);
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
            {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},
            {"/=", DIV_ASSIGN}, {"%=", MOD_ASSIGN},
            {"...", VARIADIC}, {".", DOT},
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
        if (child->token->type == ASSIGN && child->left->token->type == VAR_DEC) {
            Node *dec = child->left;
            push_back(parent->children, dec);
            child->left = dec->left;
        }
        push_back(parent->children, child);
    }
}

Node *find_in_children(Node *parent, Type type, char *name) {
    if (!parent)
        return NULL;
    for (size_t i = 0; i < parent->children_count; i++) {
        Node *curr = parent->children[i];
        if (type == VAR) {
            if (curr->token->type != VAR_DEC)
                continue;
            if (strcmp(curr->left->token->name, name) == 0)
                return curr->left;
        } else {
            if (type == FN_CALL) {
                if (!includes(curr->token->type, FN_DEC, PROTO, 0))
                    continue;
            } else if (curr->token->type != type)
                continue;
            if (strcmp(curr->token->name, name) == 0)
                return curr;
        }
    }
    return NULL;
}

Node *find_by_type(Type type, char *name) {
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        Node *found = NULL;

        if (includes(scope->token->type, FN_DEC, PROTO, 0))
            found = find_in_children(scope->left, type, name);
        if (!found)
            found = find_in_children(scope, type, name);
        if (found)
            return found;
    }
    eprint("'%s' %t not found\n", name, type);
    exit(1);
    return NULL;
}

Node *is_data_type(Token *token) {
    if (token->is_type && includes(token->type, BOOL, I8, I32, CHARS, 0))
        return new_node(token);
    if (token->type == IDENTIFIER)
        return find_by_type(STRUCT_DEC, token->name);
    return NULL;
}

Node *parse_type(void) {
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
        Node *type = is_data_type(peek(p));
        if (type == NULL) {
            eprint("expected a data type after ref, got %k\n", peek(p));
            exit(1);
        }

        node->left = type;
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
    Node *type = is_data_type(peek(0));
    if (type != NULL)
        next();
    return type;
}

Node *prime_node(void) {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) {
    case IDENTIFIER: {
        Node *next_elem = parse_type();
        if (next_elem) {
            node = new_node(new_token(VAR_DEC, token->space));
            token->type = VAR;
            node->left = new_node(token);
            node->left->left = next_elem;
            return node;
        }
        if (peek(0)->type == LPARENT) // FN_CALL
        {
            next();
            token->type = FN_CALL;
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
    // values
    case BOOL: case I8: case I32: case CHARS: case VARIADIC: { // clang-format on
        return new_node(token);
    }
    case STRUCT_DEC: {
        node = new_node(token);

        node->token->is_type = true;
        if (peek(0)->type != IDENTIFIER) {
            eprint("Expected identifier after struct declaration\n");
            exit(1);
        }
        node->token->name = strdup(next()->name); // get struct name
        if (peek(0)->type != DOTS) {
            eprint("Expected )\n");
            exit(1);
        }
        enter_scope(node);
        next(); // skip ':'
        while (inside(node->token->space)) {
            Node *attr = expr_node(0);

            if (includes(attr->token->type, VAR_DEC, FN_DEC, 0)) {
                push_back(node->children, attr);
            } else {
                eprint("invalid attribute\n");
                exit(1);
            }
        }
        exit_scope();
        return node;
    } // clang-format off
    case OWN: case DREF: { // clang-format on
        node = new_node(token);
        node->left = prime_node(); // TODO: expect identifier
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
    } // clang-format off
    case SUB: case ADD: { // clang-format on
        node = new_node(token);
        node->right = prime_node();
        node->left = new_node(new_token(I32, node->token->space));
        node->left->token->i32.value = token->type == SUB ? -1 : 1;
        node->token->type = MUL;
        return node;
    } // clang-format off
    case PROTO: case FN_DEC: { // clang-format on
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
            if (arg->token->type == VARIADIC) {
                // push_back(node->left->children, arg);
                node->token->is_variadic = true;
                break;
            }
            if (arg->token->type != VAR_DEC) {
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


        node->right = parse_type();

        if (node->token->type == FN_DEC) { // : and bloc only for fn
            if (peek(0)->type != DOTS) {
                eprint("Expected : after function declaration\n");
                exit(1);
            }
            next();
            parse_bloc(node);
        }

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
            [ADD_ASSIGN] = 1, [SUB_ASSIGN] = 1,
            [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1,
            [MOD_ASSIGN] = 1,
            
            [OR] = 11, [AND] = 12,

            [EQ] = 21, [NQ] = 21,
            [GT] = 22, [LT] = 22,
            [GE] = 22, [LE] = 22,

            [ADD] = 30, [SUB] = 30,
            [MUL] = 31, [DIV] = 31, [MOD] = 31,

            [DOT] = 41,
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

void gen_ast(void) {
    if (ura.errors_count)
        return;
    ura.ast = new_node(new_token(IDENTIFIER, 0));
    ura.ast->token->name = strdup("ura-scope");
    enter_scope(ura.ast);
    while (!includes(peek(0)->type, END, 0) && !ura.errors_count) {
        Node *child = expr_node(0);
        push_back(ura.ast->children, child);
    }
    exit_scope();
}

// INTERMEDIATE REPRESENTATION
void enter_scope(Node *node) {
    Token *token = node->token;
    print("%s: %t %s\n", CYAN("enter scope"), token->type, token->name);
    ura.scope = node;
    push_back(ura.scopes, node);
}

void exit_scope() {
    Token *token = ura.scope->token;
    print("%s: %t %s\n", CYAN("exit scope"), token->type, token->name);
    ura.scopes_count--;
    ura.scopes[ura.scopes_count] = NULL;
    if (ura.scopes_count == 0)
        ura.scope = NULL;
    else
        ura.scope = ura.scopes[ura.scopes_count - 1];
}


Node *pointer_type(Node *node) {
    Node *left = node->left;
    switch (node->token->type) {
    case VAR_LOAD: {
        Node *type = left->left;
        return type->token->type == REF ? type->left : NULL;
    }
    case OWN: {
        return left->token->type == VAR_LOAD ? left->left->left : NULL;
    }
    default:
        break;
    }
    return NULL;
}

Node *type_of(Node *node) {
    switch (node->token->type) {
    case VAR: return node->left;
    case VAR_LOAD: return node->left->left;
    case DOT: {
        Node *struct_dec = node->left->left->left;
        size_t index = node->right->token->i32.value;
        return struct_dec->children[index]->left->left;
    }
    case FN_CALL: return node->right->right;
    case DREF:
        return pointer_type(node->left);
    case OWN: {
        if (node->right == NULL) {
            node->right = new_node(new_token(REF, node->token->space));
            node->right->left = type_of(node->left);
        }
        return node->right;
    } // clang-format off
    case BOOL: case I8: case I32: case CHARS: { // clang-format on
        return node; 
    } // clang-format off
    case ADD: case SUB: case MUL: case DIV: case MOD: {
        return type_of(node->left); // TODO: to be checked later
    }
    default:
        // TODO: comparisons and and/or need a BOOL type node
        return NULL;
    }
}

// NULL means void, so two NULLs are the same type
bool same_type(Node *left, Node *right) {
    if (left == NULL || right == NULL)
        return left == right;
    if (left->token->type != right->token->type)
        return false;
    if (left->token->type == STRUCT_DEC)
        return strcmp(left->token->name, right->token->name) == 0;
    if (left->token->type == REF)
        return same_type(left->left, right->left);
    return true;
}

bool same_params(Node *left, Node *right) {
    if (left->token->is_variadic != right->token->is_variadic)
        return false;
    if (left->left->children_count != right->left->children_count)
        return false;
    for (size_t i = 0; i < left->left->children_count; i++) {
        // ARGS -> VAR_DEC -> VAR -> type
        Node *lchild = left->left->children[i]->left->left;
        Node *rchild = right->left->children[i]->left->left;
        if (!same_type(lchild, rchild))
            return false;
    }
    return true;
}

bool same_signature(Node *left, Node *right) {
    return same_params(left, right) && same_type(left->right, right->right);
}

bool args_fit(Node *fdec, Node *call) {
    size_t want = fdec->left->children_count;
    size_t got = call->left->children_count;
    if (fdec->token->is_variadic ? got < want : got != want)
        return false;
    for (size_t i = 0; i < want; i++) {
        Node *param = fdec->left->children[i]->left->left;
        if (!same_type(param, type_of(call->left->children[i])))
            return false;
    }
    return true;
}

// pick the overload whose parameters match the call, innermost scope first
Node *find_function(Node *call) {
    char *name = call->token->name;
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        Node *found = NULL;
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *curr = scope->children[j];
            if (!includes(curr->token->type, FN_DEC, PROTO, 0))
                continue;
            if (strcmp(curr->token->name, name) != 0)
                continue;
            if (!args_fit(curr, call))
                continue;
          
            // proto printf(str chars, ...)
            // fn printf(str chars):
            // printf("hello")
            if (found) {
                eprint("ambiguous call to '%s'\n", name);
                exit(1);
            }
            found = curr;
        }
        if (found)
            return found;
    }
    eprint("no overload of '%s' takes these arguments\n", name);
    exit(1);
    return NULL;
}

// TODO: do the llvm renaming here
Node *check_ast(Node *parent, size_t i) {
    Node *node = parent->children[i];
    size_t count = node->children_count;

    if (ura.errors_count)
        return NULL;
    switch (node->token->type) {
    case FN_DEC: {
        break;
    }
    case PROTO: {
        for (size_t j = 0; j < parent->children_count; j++) {
            Node *other = parent->children[j];
            if (other == node) // never match yourself
                continue;
            if (other->token->type != FN_DEC)
                continue;
            if (!node->token->name || !other->token->name)
                continue;
            if (strcmp(node->token->name, other->token->name) != 0)
                continue;

            if (!same_params(node, other))
                continue;
            if (!same_type(node->right, other->right)) {
                eprint("'%s' can't be overloaded on its return type alone\n", node->token->name);
                return NULL;
            }
            return NULL; // keep fn, drop proto
        }
        break;
    }
    default:
        break;
    }
    return node;
}

void analyze_ast(Node *node) {
    if (ura.errors_count)
        return;
    switch (node->token->type) {
    case IDENTIFIER: {
        node->left = find_by_type(VAR, node->token->name);
        // TODO: handle not found
        node->token->type = VAR_LOAD;
        break;
    }
    case VAR_DEC: {
        // TODO: later on try declaring structs/enum at the bottom
        break;
    } // clang-format off
    case VAR:
    case BOOL: case I8: case I32: case CHARS: break; // clang-format on

    case STRUCT_DEC: {
        break;
    }
    case DOT: {
        analyze_ast(node->left);
        if (node->left->left->left->token->type == REF) {
            eprint("please deref ref\n");
            exit(1);
        }
        // TODO: to be protected etc...
        // we can only have identifer or a fdec
        if (node->right->token->type != IDENTIFIER)
            analyze_ast(node->right);
        Node *struct_dec = node->left->left->left;
        Node *attr = node->right;
        for (size_t i = 0; i < struct_dec->children_count; i++) {
            Node *child = struct_dec->children[i]->left; // VAR
            if (strcmp(child->token->name, attr->token->name) == 0) {
                attr->token->i32.value = i;
                attr->token->type = ATTR;
                break;
            }
        }
        // TODO: Not found
        break;
    }
    case OWN: {
        analyze_ast(node->left);
        break;
    }
    case DREF: {
        analyze_ast(node->left);
        // LOAD_VAR -> VAR -> type: the variable must be declared as a ref
        if (pointer_type(node->left) == NULL) {
            eprint("dref() expects a ref variable\n");
            exit(1);
        }
        break;
    } // clang-format off
    case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: 
    case DIV_ASSIGN: case MOD_ASSIGN: {
        analyze_ast(node->left);
        analyze_ast(node->right);
        
        node->left = node->left;
        Node *right = node->right;
        Type type = 0;
        switch(node->token->type) {
            case ADD_ASSIGN: type = ADD; break;
            case SUB_ASSIGN: type = SUB; break;
            case MUL_ASSIGN: type = MUL; break;
            case DIV_ASSIGN: type = DIV; break;
            case MOD_ASSIGN: type = MOD; break;
            default: {
                eprint("handle this case %t\n", node->token->type);
                exit(1);
                break;
            }
        }
        node->token->type = ASSIGN;
        // set the compatible math op
        node->right = new_node(new_token(type, node->token->space));
        node->right->left = node->left;
        node->right->right = right;
        break;
    }
    case ASSIGN:
    case ADD: case SUB: case MUL: case DIV: case MOD:
    case GT: case LT: case GE: case LE: case EQ: case NQ:
    case AND: case OR: { // clang-format on
        analyze_ast(node->left);
        analyze_ast(node->right);
        // TODO: check compatibility
        break;
    } // clang-format off
    case PROTO: case FN_DEC: { // clang-format on
        enter_scope(node);
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze_ast(node->left->children[i]);
        // code bloc
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
        exit_scope();
        break;
    }
    case FN_CALL: {
        for (size_t i = 0; i < node->left->children_count; i++)
            analyze_ast(node->left->children[i]);
        node->right = find_function(node);
        break;
    }
    case RETURN: {
        // TODOL check compatibility
        analyze_ast(node->left);
        break;
    }
    case IF: {
        enter_scope(node);
        analyze_ast(node->left); // TODO: check condition type is boolean
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
        Node *curr = node->right;
        while (curr) {
            enter_scope(curr);
            if (curr->left) // elif
                analyze_ast(curr->left);
            for (size_t i = 0; i < curr->children_count; i++)
                analyze_ast(curr->children[i]);
            curr = curr->right;
            exit_scope();
        }
        exit_scope();
        break;
    }
    case WHILE: {
        enter_scope(node);
        analyze_ast(node->left); // TODO: check condition type is boolean
        for (size_t i = 0; i < node->children_count; i++)
            analyze_ast(node->children[i]);
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

void gen_ir(void) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    // skip last one because it's ura scope
    // TODO: to be cheked later because
    // we might need t odeclare function
    // inside function
    size_t j = 0;
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        Node *child = check_ast(ura.ast, i);
        if (ura.errors_count)
            break;
        if (child)
            ura.ast->children[j++] = child;
    }
    ura.ast->children_count = j;
    print_nodes(GREEN("==========CHECK================\n"));
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        analyze_ast(ura.ast->children[i]);
        if (ura.errors_count)
            break;
    }
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
    case VAR_DEC: {
        node->left->token->llvm.elem = create_variable(node);
        break;
    }
    case VAR: {
        break;
    }
    case VAR_LOAD: {
        node->token->llvm.elem = create_load(node);
        break;
    } // clang-format off
    case BOOL: case I8: case I32: case CHARS: {
        // clang-format on
        node->token->llvm.elem = create_value(node);
        break;
    }
    case STRUCT_DEC: {
        // create_struct_body(node);
        break;
    }
    case DOT: {
        node->token->llvm.elem = create_attr(node);
        break;
    }
    case OWN: {
        node->token->llvm.elem = address_of(node->left);
        break;
    }
    case DREF: {
        code_gen(node->left);
        node->token->llvm.elem = create_dref(node->left);
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
        node->token->llvm.elem = create_math_op(node);
        break;
    } // clang-format off
    case GT: case LT: case GE: case LE: case EQ: case NQ: {
        // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        Node *left = node->left;
        Node *right = node->right;
        node->token->llvm.elem = create_comparision_op(node);
        break;
    } // clang-format off
    case AND: case OR: {
        // clang-format on
        code_gen(node->left);
        code_gen(node->right);
        // TODO: check compatibility
        Node *left = node->left;
        Node *right = node->right;
        node->token->llvm.elem = create_logic_op(node);
        break;
    }
    case PROTO: {
        break;
    }
    case FN_DEC: {
        enter_scope(node);

        // Token *token = node->token;
        // if (!token->llvm.func_type)
        //     create_function(node);
        create_entry(node);

        // extract parameters as variables
        for (size_t i = 0; i < node->left->children_count; i++) {
            Node *dec = node->left->children[i];
            code_gen(dec);
            create_param(node, dec, i);
        }
        // code gen children
        gen_body(node);
        create_default_return(node);
        exit_scope();
        break;
    }
    case FN_CALL: {
        for (size_t i = 0; i < node->left->children_count; i++) {
            code_gen(node->left->children[i]);
        }
        node->token->llvm.elem = create_function_call(node);
        break;
    }
    case RETURN: {
        code_gen(node->left);
        node->token->llvm.elem = create_return(node->left);
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

void gen_funcs_and_structs_asm(Node *node) {
    if (!node)
        return;
    Type type = node->token->type;
    if (includes(type, FN_DEC, PROTO, 0)) {
        create_function(node);
    } else if (includes(type, STRUCT_DEC, 0)) {
        create_struct(node);
    }

    for (size_t i = 0; i < node->children_count; i++)
        gen_funcs_and_structs_asm(node->children[i]);

    // for (size_t i = 0; i < node->children_count; i++)
    //     gen_funcs_and_structs_asm(node->children[i]);
}

void gen_asm(uraFile *file) {
    if (ura.errors_count)
        return;
    enter_scope(ura.ast);
    asm_init("ura-module");

    gen_funcs_and_structs_asm(ura.ast);
    for (size_t i = 0; i < ura.ast->children_count; i++) {
        code_gen(ura.ast->children[i]);
    }
    // for (size_t i = 0; i < ura.ast->functions_count; i++) {
    //     code_gen(ura.ast->functions[i]);
    // }
    exit_scope();
    asm_finalize(file->ll_path);
}

void gen_bin(uraFile *file) {
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
    [*] FN_DEC overwrite proto
    [*] access via '.' in struct
    [*] pass struct by reference to function
    [ ] polymorphism
    [ ] handle method in preprocessing
    [ ] struct method
    [ ] drop method
    [ ] String struct
    [ ] to_string method
    [ ] output (should handl also struct.to_string)

    [ ] access via '[]' in chars
    [ ] operators overload

    [ ] for loops over a range: for i in 0..10
    [ ] more integer types (i8, i64, unsigned) and casting with as
    [ ] arrays: declaration and indexing
    [ ] function inside function
    [ ] arena allocator (all stdup ... should use it)
*/

int main(int ac, char **av) {
    parse_arguments(ac, av);
    for (size_t i = 0; i < ura.files_count; i++) {
        uraFile *file = ura.files[i];
        print(GREEN("============TOKENIZE============\n"));
        gen_tokens(file);
        gen_ast();
        print_nodes(GREEN("============AST=================\n"));
#if 1
        gen_ir();
        print_nodes(GREEN("============IR==================\n"));
#    if 1
        gen_asm(file);
        print_nodes(GREEN("============ASM==================\n"));
        gen_bin(file);
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
