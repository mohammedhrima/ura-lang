#include "./include/header.h"

bool found_error;
bool did_pasm;
char *input;
Token **tokens;
Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos;

int ptr;
struct _IO_FILE *asm_fd;
int str_index;
int bloc_index;

char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9", NULL};
char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9", NULL};
int regLen = sizeof(eregs) / sizeof(eregs[0]);

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
// Tokenization - Convert source text to token stream
// Syntax Analysis - Build abstract syntax tree from tokens
void tokenize(char *filename)
{
    if (found_error) return;
#if DEBUG
    debug(GREEN BOLD"TOKENIZE: [%s]\n" RESET, filename);
#endif
    char *input = open_file(filename);
    if (!input) return;

    struct { char *value; Type type; } specials[] = {
        {".", DOT}, {":", DOTS}, {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN},
        {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN}, {"!=", NOT_EQUAL},
        {"==", EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
        {"<", LESS}, {">", MORE}, {"=", ASSIGN}, {"+", ADD}, {"-", SUB},
        {"*", MUL}, {"/", DIV}, {"%", MOD}, {"(", LPAR}, {")", RPAR},
        {"[", LBRA}, {"]", RBRA}, {",", COMA}, {"&&", AND}, {"||", OR},
        {0, (Type)0}
    };

    int space = 0;
    int line = 0;
    bool new_line = true;
    for (int i = 0; input[i] && !found_error; )
    {
        int s = i;
        if (isspace(input[i]))
        {
            if (input[i] == '\n')
            {
                line++;
                new_line = true;
                space = 0;
            }
            else if (new_line)
            {
                if (input[i] == '\t') space += TAB;
                else space++;
            }
            i++;
            continue;
        }
        else if (strncmp(input + i, "/*", 2) == 0)
        {
            i += 2;
            while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2))
            {
                if (input[i] == '\n') line++;
                i++;
            }
            check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'\n");
            i += 2;
            continue;
        }
        else if (strncmp(input + i, "//", 2) == 0)
        {
            while (input[i] && input[i] != '\n') i++;
            continue;
        }
        new_line = false;
        bool found = false;
        for (int j = 0; specials[j].value; j++)
        {
            if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
            {
                parse_token(NULL, 0, 0, specials[j].type, space, filename, line);
                found = true;
                i += strlen(specials[j].value);
                if (strcmp(specials[j].value, ":") == 0) space += TAB;
                break;
            }
        }
        if (found) continue;
        if (input[i] && input[i] == '\"')
        {
            i++;
            while (input[i] && input[i] != '\"') i++;
            check(input[i] != '\"', "Expected '\"'");
            i++;
            parse_token(input, s, i, CHARS, space, filename, line);
            continue;
        }
        if (input[i] && input[i] == '\'')
        {
            i++;
            if (input[i] && input[i] != '\'') i++;
            check(input[i] != '\'', "Expected '\''");
            i++;
            parse_token(input, s + 1, i, CHAR, space, filename, line);
            continue;
        }
        if (isalpha(input[i]) || strchr("@$-_", input[i]))
        {
            while (input[i] && (isalnum(input[i]) || strchr("@$-_", input[i]))) i++;
            if (strncmp(input + s, "use ", 4) == 0)
            {
                while (isspace(input[i])) i++;
                s = i;
                while (input[i] && !isspace(input[i])) i++;
                char *use = allocate(i - s + 1, sizeof(char));
                strncpy(use, input + s, i - s);
                char *tmp = strjoin(use, ".w", NULL);
                free(use);
                use = tmp;
                tokenize(use);
                free(use);
            }
            else
                parse_token(input, s, i, ID, space, filename, line);
            continue;
        }
        if (isdigit(input[i]))
        {
            while (isdigit(input[i])) i++;
            parse_token(input, s, i, INT, space, filename, line);
            continue;
        }
        check(input[i], "Syntax error <%c>\n", input[i]);
    }
    free(input);
}

Node *expr() {
    return assign();
}

AST_NODE(assign, logic, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, 0);
AST_NODE(logic, equality, AND, OR, 0);
AST_NODE(equality, comparison, EQUAL, NOT_EQUAL, 0); // TODO: handle ! operator
AST_NODE(comparison, add_sub, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0);
AST_NODE(add_sub, mul_div, ADD, SUB, 0);
AST_NODE(mul_div, dot, MUL, DIV, 0); // TODO: handle modulo %

Node *dot()
{
    Node *left = brackets();
    Token *token;
    while ((token = find(DOT, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        token = find(ID, 0);
        if (check(!token, "error, expected id after dot")) exit(1);
        node->right = new_node(token);
        left = node;
    }
    return left;
}

Node *brackets()
{
    Node *left = sign();
    Token *token;
    if ((token = find(LBRA, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = brackets();
        check(!find(RBRA, 0), "expected right bracket");
        return node;
    }
    return left;
}

Node *sign() // TODO: implement it
{
    return prime();
}

// Function Declaration:
//    + left children: arguments
//    + children     : code block
Node *func_dec(Node *node)
{
    bool is_proto = find(PROTO, 0) != NULL;

    Token *typeName = find(DATA_TYPES, 0);
    if (typeName->type == ID)
    {
        typeName = get_struct(typeName->name);
        todo(1, "handle function return struct properly");
    }
    Token *fname = find(ID, 0);
    if (check(!typeName || !fname, "expected data type and identifier after func declaration"))
        return node;
    node->token->retType = typeName->type;
    node->token->is_proto = is_proto;
    setName(node->token, fname->name);
    enter_scoop(node);

    check(!find(LPAR, 0), "expected ( after function declaration");
    node->left = new_node(new_token(CHILDREN, node->token->space));
    Token *last;
    while (!found_error && !(last = find(RPAR, END, 0)))
    {
        bool is_ref = find(REF, 0) != NULL;
        Token* data_type = find(DATA_TYPES, ID, 0);
        if (data_type && data_type->type == ID)
        {
            data_type = get_struct(data_type->name);
            if (data_type) data_type->type = STRUCT_CALL;
        }
        if (check(!data_type, "expected data type in function argument")) break;
        Token *name = find(ID, 0);
        if (check(!name, "expected identifier in function argument")) break;
        Node *curr;
        if (data_type->type == STRUCT_CALL)
        {
            curr = new_node(data_type);
            data_type->is_ref = is_ref;
            setName(data_type, name->name);
        }
        else
        {
            curr = new_node(name);
            name->is_ref = is_ref;
            name->type = data_type->type;
        }
        add_child(node->left, curr);
        find(COMA, 0); // TODO: check this later
    }
    check((!found_error && last->type != RPAR), "expected ) after function declaration");
    check((!found_error && !find(DOTS, 0)), "Expected : after function declaration");

    Node *child = NULL;
    while (within_space(node->token->space)) child = add_child(node, expr());
    if (!is_proto)
    {
        if (node->token->retType != VOID)
            check(!child || child->token->type != RETURN, "expected return statment");
        else
        {
            Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
            ret->left = new_node(new_token(INT, node->token->space + TAB));
            add_child(node, ret);
        }
    }
    exit_scoop();
    return node;
}

// Function call:
//    + children: Parameters
Node *func_call(Node *node)
{
    node->token->type = FCALL;
    Token *arg = NULL;
    Token *token = node->token;

    while (!found_error && !(arg = find(RPAR, END, 0)))
    {
        Node *curr = expr();
        curr->token->space = token->space;
        add_child(node, curr);
        find(COMA, 0);
    }
    check(!found_error && arg->type != RPAR, "expected ) after function call");
    return node;
}

// Function main:
//    + children: code bloc
Node *func_main(Node *node)
{
    check(!find(RPAR, 0), "expected ) after main declaration");
    check(!find(DOTS, 0), "expected : after main() declaration");

    enter_scoop(node);
    node->token->type = FDEC;
    node->token->retType = INT;

    Node *last = NULL;
    while (within_space(node->token->space))
    {
        last = expr();
        add_child(node, last);
    }
    if (!last || last->token->type != RETURN)
    {
        last = new_node(new_token(RETURN, node->token->space + TAB));
        last->left = new_node(new_token(INT, node->token->space + TAB));
        add_child(node, last);
    }
    exit_scoop();
    return node;
}

// Struct def Layout:
//    + children: attributes
Node *struct_def(Node *node)
{
    Token *name;
    if (check(!(name = find(ID, 0)), "expected identifier after struct definition")) return NULL;
    if (check(!find(DOTS, 0), "expected dots after struct definition")) return NULL;
    setName(node->token, NULL);
    node->token->Struct.name = strdup(name->name);
    while (within_space(node->token->space))
    {
        Token *attr = find(DATA_TYPES, ID, 0);
        Token *id = find(ID, 0);
        if (check(!attr, "expected data type followed by id"))
        {
            ptoken(tokens[exe_pos]);
            break;
        }
        if (check(!id, "expected id after data type")) break;

        if (attr->type == ID) // attribute is a struct
        {
            Token *st = get_struct(attr->name);
            if (check(!st, "Unkown data type [%s]\n", attr->name)) break;
            attr = st;
            char *name = id->name;
            id = attr;
            setName(id, name);
            id->type = STRUCT_CALL;

        }
        else id->type = attr->type;
        id->is_attr = true;
        add_attribute(node->token, id);
    }
    set_struct_size(node->token);
    new_struct(node->token);
    return node;
}

Node *symbol(Token *token)
{
    Token *struct_token = NULL;
    Node *node;
    if (token->type == ID && token->name && (struct_token =  is_struct(token)))
    {
        node = new_node(struct_token);
        token = find(ID, 0);
        if (check(!token, "Expected variable name after struct declaration\n"))
            debug(RED"found instead %k\n"RESET, tokens[exe_pos]);

        setName(node->token, token->name);
        node->token->type = STRUCT_CALL;
        node->token->is_data_type = true;
        return node;
    }
    else if (token->is_data_type)
    {
        Token *tmp = find(ID, 0);
        check(!tmp, "Expected variable name after [%s] symbol\n", to_string(token->type));
        setName(token, tmp->name);
        return new_node(token);
    }
    else if (token->type == ID && token->name && find(LPAR, 0))
    {
        node = new_node(token);
        if (strcmp(token->name, "main") == 0) return func_main(node);
        return func_call(node);
    }
    else if (token->type == ID && token->name && find(LBRA, 0))
    {
        node = new_node(tokens[exe_pos - 1]); // LBRA
        node->token->type = ACCESS;


        node->left = new_node(token);
        node->right = expr(); // TODO: should be integer or ID;

        check(!find(RBRA, 0), "expected right bra");
        // debug("%n\n", node);
        // exit(1);
        return node;
    }
    return new_node(token);
}

// if Layout:
//    + left    : condition
//    + children: code bloc
//    + right   : elif/else
Node *if_node(Node *node)
{
    enter_scoop(node);

    node->left = expr();  // condition, TODO: check if it's boolean
    node->left->token->is_cond = true;
    node->left->token->space = node->token->space;
    node->right = new_node(new_token(CHILDREN, node->token->space));

    check(!find(DOTS, 0), "Expected : after if condition\n", "");

    // code bloc
    while (within_space(node->token->space)) add_child(node, expr());
    while (includes(tokens[exe_pos]->type, ELSE, ELIF, 0) && within_space(node->token->space - TAB))
    {
        Token *token = find(ELSE, ELIF, 0);
        Node *curr = add_child(node->right, new_node(token));
        token->space -= TAB;
        if (token->type == ELIF)
        {
            curr->left = expr();
            curr->left->token->is_cond = true;
            check(!find(DOTS, 0), "expected : after elif condition");
            while (within_space(token->space)) add_child(curr, expr());
        }
        else if (token->type == ELSE)
        {
            check(!find(DOTS, 0), "expected : after else");
            while (within_space(token->space)) add_child(curr, expr());
            break;
        }
    }
    exit_scoop();
    return node;
}

// while Layout:
//    left     : condition
//    children : code bloc
Node *while_node(Node *node)
{
    enter_scoop(node);
    node->left = expr();  // condition, TODO: check if it's boolean
    node->left->token->is_cond = true;
    node->left->token->space = node->token->space;

    check(!find(DOTS, 0), "Expected : after while condition\n", "");
    while (within_space(node->token->space))
    {
        Token *token = find(CONTINUE, BREAK, 0);
        Node *child = NULL;
        if (token) child = new_node(token);
        else child = expr();
        add_child(node, child);
    }
    exit_scoop();
    return node;
}

Node *prime()
{
    Node *node = NULL;
    Token *token;
    if ((token = find(STRUCT_DEF))) return struct_def(new_node(token));
    else if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, 0))) return symbol(token);
    else if ((token = find(REF, 0)))
    {
        node = prime(); // TODO: check it
        check(!node->token->is_data_type, "must be variable declaration after ref");
        node->token->is_ref = true;
        return node;
    }
    else if ((token = find(FDEC, 0))) return func_dec(new_node(token));
    else if ((token = find(RETURN, 0)))
    {
        // TODO: check if return type is compatible with function
        // in current scoop
        node = new_node(token);
        node->left = expr();
        return node;
    }
    else if ((token = find(IF, 0))) return if_node(new_node(token));
    else if ((token = find(WHILE, 0))) return while_node(new_node(token));
    else if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);
    else if ((token = find(LPAR, 0)))
    {
        if (tokens[exe_pos]->type != RPAR) node = expr();
        check(!find(RPAR, 0), "expected right par");
        return node;
    }
    // else if((token = find(LBRA, 0)))
    // {
    //    if (tokens[exe_pos]->type != RPAR) node = expr();
    //    check(!find(RBRA, 0), "expected right bra");
    //    return node;
    // }
    else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
    return new_node(tokens[exe_pos]);
}

void compile(char *filename)
{
    tokenize(filename);
    new_token(END, -1);
    if (found_error) return;
    Node *global = new_node(new_token(ID, -TAB - 1));
    setName(global->token, ".global");
    enter_scoop(global);

#if DEBUG
    debug(GREEN BOLD"AST:\n" RESET);
#endif
    while (tokens[exe_pos]->type != END && !found_error) add_child(global, expr());
    print_ast(global);

    if (found_error) return;
#if IR
    debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
    for (int i = 0; !found_error && i < global->cpos; i++) generate_ir(global->children[i]);
    if (found_error) return;
    print_ir();
#endif
#if OPTIMIZE
    debug(GREEN BOLD"OPTIMIZE INTERMEDIATE REPRESENTATIONS:\n" RESET);
    copy_insts();
    while (OPTIMIZE && !found_error && optimize_ir()) copy_insts();
#if !DEUBG
    print_ir();
#endif
#endif
#if ASM
    copy_insts();
    debug(GREEN BOLD"GENERATE ASSEMBLY CODE:\n" RESET);
    generate_asm(filename);
#endif

    free_node(global);
}

int main(int argc, char **argv)
{
    check(argc < 2, "Invalid arguments");

    for (int i = 1; i < argc; i++)
    {
        compile(argv[i]);
        free_memory();
    }
}