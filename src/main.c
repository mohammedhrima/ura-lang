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
    };
    union {
        Node *right;
		Node *args;
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
    { "char", { .type = INT, .is_type = true, .shift = 8 } },
    { "i8", { .type = INT, .is_type = true, .shift = 8 } },
    { "i32", { .type = INT, .is_type = true, .shift = 32 } },
    { "num", { .type = INT, .is_type = true } },

    { "True", { .type = INT, .num = { .value = true } } },
    { "False", { .type = INT, .num = { .value = false } } },

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
            s += 2; // skip /*
            while (buff[e] != '\0' && !strncmp2(buff + e, "*/", 2))
                e++;
            // TODO:
            //  if (!check_multi_comment(s, e, line))
            //     break;
            e += 2;
            continue;
        }

        if (buff[s] != '\0' && strchr2("\"\'", buff[s])) {
            char c = buff[s];
            e++;
            char str[3] = { [0] = '\n', [1] = c };
            // print("2. hellooo\n");
            while (!strchr2(str, buff[e])) {
                if (buff[e] == '\\' && !strchr2("\n", buff[e + 1]))
                    e++;
                e++;
            }
            // TODO:
            // if (!check_quote(s, e, line))
            //     continue;
            e++; // skip last ' or "
            if (c == '"')
                parse_string(s, e, space);
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
                new_token(type, NULL);
                if (type == DOTS)
                    space += TAB;
                e += len;
                break;
            }
        }
        if (e != s) {
            continue;
        }
        eprint("unknow charcter <%c>\n", buff[s]);
        exit(1);
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
    ura.scopes[ura.scopes_count] = NULL;
    ura.scopes_count--;
    if (ura.scopes_count >= 0)
        ura.scope = ura.scopes[ura.scopes_count];
    else
        ura.scope = NULL;
}

Node *parse_type(int pos) {
    Token *token = peek(pos);
    if (token->is_type) {
        next();
        return new_node(token);
    }
    return NULL;
}

Node *parse_prime() {
    Token *token = next();
    Node *node = NULL;
    switch (token->type) {
    case INT: {
        return new_node(token);
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
        } else
            node = new_node(token);
        return node;
    }
    case FN_DEC: {
        node = new_node(token);
        token = next(); // TODO: expect ID
        node->token->name = token->name;
        next(); // SKIP (
        node->right = new_node(new_token(ARGS, token));
        while (!includes(peek(0)->type, RPARENT, LAST)) {
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
                break;
            } else {
                eprint("%s:%d error %k\n", FILE, LINE, token);
                exit(1);
            }
            if (peek(0)->type == COMA) // TODO: to be improved
                next();
        }
        print(">>> %k\n", peek(0));
        assert(peek(0)->type == RPARENT);
        next();                     // SKIP )
        node->ret_type = parse_prime(); // return type
        assert(!node->left || node->left->token->is_type);
        assert(peek(0)->type == DOTS);
        next(); // SKIP :
        enter_scope(node);
        while (inside(node->token->space)) {
            append(node->children, parse_expr(0));
        }
        exit_scope();
        return node;
    }
    case RETURN: {
        node = new_node(token);
        node->left = parse_expr(0);
        return node;
    }
    case IF: {
        node = new_node(token);
        enter_scope(node);
        node->left = parse_expr(0);
        if (peek(0)->type != DOTS) {
            eprint("expect ':'\n");
            exit(1);
        }
        next(); // TODO: expect ':'
        while (inside(node->token->space)) {
            append(node->children, parse_expr(0));
        }
        exit_scope();
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
    eprint("'%s' not found\n", name);
    return NULL;
}

void analyze(Node *node) {
    switch (node->token->type) { // clang-format off
    case INT: case VAR: case PARAM: { // clang-format on
        break;
    }
    case ID: {
        Node *var = find_variable(node->token->name);
        if (var) {
            // node->token->type = VAR;
            // node->token->ucg = var->token->ucg;
            // node->left = var;
            *node = *var;
        }
        break;
    }
    case DECLARE: { // right is the data type
                    // TODO: protect from redeclaring
        analyze(node->var->ret_type);
        break;
    }
    case FN_DEC: {
        enter_scope(node);
        if (node->ret_type)
            analyze(node->ret_type);
		for(int i = 0; i < node->args->children_count; i++) {
			Node *arg = node->args->children[i];
			analyze(arg);
		}
		
        for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
        exit_scope();
        break;
    }
    case RETURN: {
        if (node->left) {
            analyze(node->left);
            // assert(node->left-)
        } else {
            // TODO: return void
        }
        break;
    }
    case IF: {
        analyze(node->cond); // TODO: make sure it's boolean
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
        // TODO: check compatibility
        analyze(node->left);
        analyze(node->right);
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
void codegen(Node *node) {
    Ucg *ucg = NULL;
    switch (node->token->type) {
    case VAR: {
        // node->token->ucg = node->left->token->ucg;
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
    case DECLARE: {
        codegen(node->var->data_type); // data type

        char *name = node->var->token->name;
        Ucg *type = node->var->data_type->token->ucg;
        ucg = ucg_var(name, type);
        node->var->token->ucg = ucg;
        break;
    }
    case FN_DEC: {
        Ucg *ret_type = NULL;
        if (node->left) {
            // TODO: add return type
            codegen(node->left);
            ret_type = node->left->token->ucg;
        } else
            ret_type = ucg_void_type();

		expand(Ucg*, params);
		zero_init(params);

		// Ucg *args = NULL;
		if(node->args->children_count != 0) {
			Ucg *arg = arena_alloc(node->args->children_count, sizeof(Ucg));
			// for
			// for(int i = 0; i < )
		}
        ucg = ucg_fn(node->token->name, ret_type, NULL, 0);
        for (int i = 0; i < node->children_count; i++)
            codegen(node->children[i]);

        ucg_end(ucg);
        node->token->ucg = ucg;
        break;
    }
    case RETURN: {
        // TODO: check if there is node left
        codegen(node->left);
        ucg_ret(node->left->token->ucg);
        break;
    }
    case IF: {
        codegen(node->cond);
        node->token->ucg = ucg_if();
        ucg_then(node->token->ucg, node->cond->token->ucg);
        for (int i = 0; i < node->children_count; i++)
            codegen(node->children[i]);
        ucg_end(node->token->ucg);
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
        node->token->ucg = ucg_op(node->token->type, // clang-format off
			node->left->token->ucg, 
			node->right->token->ucg
		); // clang-format on
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
    ura.tree = new_node(new_token(SCOPE, NULL));
    ura.tree->token->name = "ura-scope";
    enter_scope(ura.tree);
    while (peek(0)->type != LAST) {
        // Node *file = ura.tree->children[i]; // File node / empty children
        // enter_scope(file);
        // parse_structs(file);
        append(ura.tree->children, parse_expr(0));
        // handle_global_scope(file);
        // exit_scope();
    }
    exit_scope();
    // check_errors(); // the tree is broken: don't analyze it
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#        if ANALYZE
    print(GREEN("======================================\n"));
    print(GREEN("[ANALYZE]:\n"));
    enter_scope(ura.tree);
    for (int i = 0; i < ura.tree->children_count; i++) {
        analyze(ura.tree->children[i]);
    }
    exit_scope();
    // check_errors(); // the tree is not valid code: don't generate it
    // print(GREEN("======================================\n"));
    for (int i = 0; i < ura.tree->children_count; i++) {
        print("%n", ura.tree->children[i]);
    }

#            if ASM
    print(GREEN("======================================\n"));
    print(GREEN("[CODE GEN]:\n"));
    ucg_start("ura");
    // enter_scope(ura.tree);
    for (int i = 0; i < ura.tree->children_count; i++) {
        codegen(ura.tree->children[i]);
    }
    // exit_scope();
    print(GREEN("======================================\n"));
    print(GREEN("[URA IR]:\n"));
    print_ucgs();
    ucg_compile();
    ucg_path("build/out.ll");
    ucg_gen_ll();
    ucg_gen_bin();
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

void check_errors(void) {
    if (ura.errors_count == 0)
        return;
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