#include "./include/header.h"

bool found_error;

Token **tokens;
int tk_pos;
int tk_len;

char *input;
Node *global;
int exe_pos;
Inst **OrgInsts;
Inst **insts;

Node **Gscoop;
Node *scoop;
int scoopSize;
int scoopPos;

int ptr;
#if defined(__APPLE__)
    struct __sFILE *asm_fd;
#elif defined(__linux__)
    struct _IO_FILE *asm_fd;
#endif


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

Node *symbol(Token *token)
{
    // Token *struct_token = NULL;
    Node *node;
    if (token->declare)
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
    return new_node(token);
}

Node *prime()
{
    Node *node = NULL;
    Token *token;
    // if ((token = find(STRUCT_DEF, 0)))
    // {
    //     ptoken(token);
    //     todo(1, "wtf 1");
    //     return struct_def(new_node(token));
    // } 
    //else
    if ((token = find(ID, INT, CHARS, CHAR, FLOAT, BOOL, 0))) return symbol(token);
    // else if ((token = find(REF, 0)))
    // {
    //     node = prime(); // TODO: check it
    //     check(!node->token->declare, "must be variable declaration after ref");
    //     node->token->is_ref = true;
    //     return node;
    // }
    else if ((token = find(FDEC, 0))) return func_dec(new_node(token));
    else if ((token = find(RETURN, 0)))
    {
        // TODO: check if return type is compatible with function
        // in current scoop
        node = new_node(token);
        node->left = expr();
        return node;
    }
    else if ((token = find(LPAR, 0)))
    {
        if (tokens[exe_pos]->type != RPAR) node = expr();
        check(!find(RPAR, 0), "expected right par");
        return node;
    }
    else check(1, "Unexpected token has type %s\n", to_string(tokens[exe_pos]->type));
    return new_node(tokens[exe_pos]);
}

Token *func_dec_ir(Node *node)
{
    new_function(node);
    enter_scoop(node);
    // int tmp_ptr = ptr;
    // ptr = 0;

    Inst* inst = NULL;
    if (!node->token->is_proto) inst = new_inst(node->token);

    // parameters
    // Node *curr = node->left;
    // for (int i = 0, r = 0; curr && i < curr->cpos && !found_error; i++)
    // {
    //     Node *child = curr->children[i];
    //     set_func_dec_regs(child->token, &r, node->token->is_proto);
    // }

    // if (node->token->is_proto) set_remove(node);
    // code bloc
    for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
    {
        Node *child = node->children[i];
        generate_ir(child);
    }

    if (!node->token->is_proto)
    {
        // TODO: if RETURN not found add it
        Token *new = new_token(END_BLOC, node->token->space);
        new->name = strdup(node->token->name);
        new_inst(new);
        // node->token->ptr = ptr;
        // ptr = tmp_ptr;
    }
    exit_scoop();
    if (!node->token->is_proto) return inst->token;
    return NULL;
}

Token *func_call_ir(Node *node)
{
    if (strcmp(node->token->name, "output") == 0)
    {
        todo(1, "implement this");
#if 0
        // setReg(node->token, "eax");
        setName(node->token, "printf");
        Node *fcall = node;

        Node *assign = new_node(new_token(ASSIGN, node->token->space));
        Token *_register = new_token(CHARS, fcall->token->space + TAB);
        // _register->creg = strdup("rdi");
        Token *varg = new_token(CHARS, fcall->token->space + TAB);
        // varg->index = ++str_index;
        varg->Chars.value = strdup("\"");

        new_inst(_register);
        assign->left = new_node(_register);
        assign->right = new_node(varg);
        assign->token->ir_reg = _register->ir_reg;

        generate_ir(assign);
        free_node(assign);

        for (int i = 0; !found_error && i < fcall->cpos; i++)
        {
            Node *carg = fcall->children[i];
            Token *var = generate_ir(carg);
            if (check(var->type == ID, "Indeclared variable %s", carg->token->name)) break;
            Token *src = new_token(INT, var->space);

            // int j = i + 1;
            // if (j < regLen)
            // {
            //     // added because unfction declaration params do have ptrs
            //     // TODO: add other data type and math operations
            //     src->ptr = 0;
            //     switch (var->type)
            //     {
            //     case CHARS:
            //     {
            //         setReg(src, rregs[j]);
            //         char *tmp = strjoin(varg->Chars.value, "%s", NULL);
            //         free(varg->Chars.value);
            //         varg->Chars.value = tmp;
            //         break;
            //     }
            //     case INT:
            //     {
            //         setReg(src, eregs[j]);
            //         char *tmp = strjoin(varg->Chars.value, "%d", NULL);
            //         free(varg->Chars.value);
            //         varg->Chars.value = tmp;
            //         break;
            //     }
            //     // case CHAR:  setReg(src, eregs[j]); break;
            //     // case FLOAT: setReg(src, rregs[j]); break; // TODO: to be checked
            //     // case BOOL:  setReg(src, eregs[j]); break;
            //     default: todo(1, "set ir_reg for %s", to_string(src->type));
            //     };
            // }
            // else
            // {
            //     todo(1, "implement assigning function argument using PTR");
            // }
            new_inst(src);
            assign = new_node(new_token(ASSIGN, node->token->space));
            assign->left = new_node(src);
            assign->right = new_node(var);
            assign->token->ir_reg = src->ir_reg;

            debug(RED);
            pnode(assign, NULL, 0);
            debug(RESET);

            generate_ir(assign);
            free_node(assign);
        }
        char *tmp = strjoin(varg->Chars.value, "\"", NULL);
        free(varg->Chars.value);
        varg->Chars.value = tmp;
#endif
    }
    else
    {
        Node *func = get_function(node->token->name);
        if (!func) return NULL;
        node->token->Fcall.ptr = func->token;

        func = copy_node(func);
        node->token->retType = func->token->retType;

        // setReg(node->token, func->token->creg);
        Node *fdec = func->left;
        Node *fcall = node;

        for (int i = 0; !found_error && i < fcall->cpos && i < fdec->cpos; i++)
        {
            Node *darg = fdec->children[i];
            Node *carg = fcall->children[i]; // will always be ID

            Token *src = generate_ir(carg);

            if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
            Token *dist = copy_token(darg->token);
           // set_func_call_regs(&r, src, dist, node);
        }
        free_node(func);
    }
    Inst* inst = new_inst(node->token);
    return inst->token;
}

Token *op_ir(Node *node)
{
    Token *left = generate_ir(node->left);
    Token *right = generate_ir(node->right);
    if (!right || !right) return NULL;

    // check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
    //       to_string(node->token->type), to_string(left->type), to_string(right->type));
    switch (node->token->type)
    {
    case ASSIGN:
    {
        node->token->ir_reg = left->ir_reg;
        node->token->retType = getRetType(node);
        // if (left->is_ref) // ir_reg, ptr
        // {
        //     if (right->is_ref) // ir_reg, ptr
        //     {
        //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
        //         if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
        //         else node->token->assign_type = REF_HOLD_REF;
        //     }
        //     else if (right->ptr || right->creg) // ptr
        //     {
        //         if (left->has_ref) node->token->assign_type = REF_ID;
        //         else node->token->assign_type = REF_HOLD_ID;
        //     }
        //     else // ir_reg, value
        //     {
        //         debug("line %d: %n\n", LINE, node);
        //         // if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
        //         node->token->assign_type = REF_VAL;
        //     }
        //     left->has_ref = true;
        // }
        // else if (left->ptr || left->creg) // ir_reg, ptr
        // {
        //     if (right->is_ref) // ir_reg, ptr
        //     {
        //         if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
        //         node->token->assign_type = ID_REF;
        //     }
        //     else if (right->ptr) // ptr
        //         node->token->assign_type = ID_ID;
        //     else // ir_reg, value
        //         node->token->assign_type = ID_VAL;
        // }
        // else if (left->type == STRUCT_CALL)
        // {
        //     stop(1, "check this");
        //     debug(">> %k\n", left);
        //     debug(">> %k\n", right);
        //     // TODO: check compatibility
        //     for (int i = 0; i < left->Struct.pos; i++)
        //     {
        //         Node *tmp = new_node(new_token(ASSIGN, node->token->space));
        //         tmp->left = new_node(left->Struct.attrs[i]);
        //         tmp->right = new_node(right->Struct.attrs[i]);
        //         op_ir(tmp);
        //         free_node(tmp);
        //     }
        //     return NULL;
        //     // exit(1);
        // }
        // else
        // {
        //     pnode(node, NULL, 0);
        //     debug("<%k>\n", left);
        //     debug("<%k>\n", right);
        //     // todo(1, "Invalid assignment");
        // }
        break;
    }
    case ADD: case SUB: case MUL: case DIV:
    {
        // TODO: to be checked
        // node->token->retType = getRetType(node);
        // if (node->token->retType  == INT) setReg(node->token, "eax");
        // else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
        // else 
        // check(1, "handle this case");
        break;
    }
    case NOT_EQUAL: case EQUAL: case LESS:
    case MORE: case LESS_EQUAL: case MORE_EQUAL:
    {
        node->token->retType = BOOL;
        // node->token->index = ++bloc_index;
        break;
    }
    default: check(1, "handle [%s]", to_string(node->token->type)); break;
    }
    Inst *inst = new_inst(node->token);
    inst->left = left;
    inst->right = right;

    return node->token;
}

Token *generate_ir(Node *node)
{
    if (found_error) return NULL;
    switch (node->token->type)
    {
    case ID:
    {
        Token *find = get_variable(node->token->name);
        if (find) return find;
        return node->token;
    }
    case INT: case BOOL: case CHAR: case STRUCT_CALL:
    case FLOAT: case LONG: case CHARS: case PTR:
    {
        new_inst(node->token);
        if (!node->token->declare) return node->token;
        // variable declaration
        new_variable(node->token);
        #if 0
        return inialize_variable(node);
        #else
        return node->token;
        #endif    
    }
    case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
    case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
    case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
    {
        // check if right is DEFAULT, then initlize it, and return left
        return op_ir(node);
    }
    // case IF:    return if_ir(node);
    // case WHILE: return while_ir(node);
    case FDEC:  return func_dec_ir(node);
    case FCALL: return func_call_ir(node);
    case RETURN:
    {
        Token *left = generate_ir(node->left);
        new_inst(node->token)->left = left;
        return node->token;
    }
    case BREAK:
    {
        for (int i = scoopPos; i >= 0; i--)
        {
            Node *scoop = Gscoop[i];
            if (strcmp(scoop->token->name, "while") == 0)
            {
                Token *token = copy_token(node->token);
                token->type = JMP;
                token->index = scoop->token->index;
                setName(token, "endwhile");
                return new_inst(token)->token;
                break;
            }
        }
        // TODO: invalid syntax, break should be inside whie loop
        break;
    }
    case CONTINUE:
    {
        for (int i = scoopPos; i >= 0; i--)
        {
            Node *scoop = Gscoop[i];
            if (strcmp(scoop->token->name, "while") == 0)
            {
                Token *token = copy_token(node->token);
                token->type = JMP;
                token->index = scoop->token->index;
                setName(token, "while");
                return new_inst(token)->token;
                break;
            }
        }
        // TODO: invalid syntax, break should be inside whie loop
        break;
    }
    case DOT:
    {
        Token *left = generate_ir(node->left);
        Token *right = node->right->token;
        if (check(left->type == ID, "undeclared variable %s", left->name)) break;

        debug(RED SPLIT RESET);
        for (int i = 0; i < left->Struct.pos; i++)
        {
            Token *attr = left->Struct.attrs[i];
            debug("%k\n", attr);
            char *to_find = strjoin(left->name, ".", right->name);
            if (strcmp(to_find, attr->name) == 0)
            {
                free(to_find);
                return attr;
            }
            free(to_find);
        }
        debug(RED SPLIT RESET);

        todo(1, "%s has no attribute %s", left->name, right->name);
        return right;
        break;
    }
    case ACCESS:
    {
        debug("line %d: %n\n", LINE, node);
        Token *left = generate_ir(node->left);
        Token *right = generate_ir(node->right);
        // TODO: check if right is a number
        // TODO: check if left is aan iterable data type

        Inst *inst = new_inst(node->token);
        // inst->token->creg = strdup("rax");
        inst->token->is_ref = true;
        inst->token->has_ref = true;
        switch (left->type)
        {
        case CHARS:
        {
            inst->token->offset = sizeof(char);
            inst->token->retType = CHAR;
            break;
        }
        case INT:
        {
            inst->token->offset = sizeof(int);
            inst->token->retType = INT;
            break;
        }
        default: todo(1, "handle this case");
        }
        inst->left = left;
        inst->right = right;
        return node->token;
    }
    case STRUCT_DEF: return node->token;
    case DEFAULT:
    {
        todo(1, "who is looking for default ?");
        return node->token;
    }
    default:
    {
        check(1, "handle this case %s", to_string(node->token->type));
        return node->token;
    }
    }
    return NULL;
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
    while (tokens[exe_pos]->type != END && !found_error) 
        add_child(global, expr());
    print_ast(global);
    if (found_error) return;

#if IR
    debug(GREEN BOLD"GENERATE INTERMEDIATE REPRESENTATIONS:\n" RESET);
    for (int i = 0; !found_error && i < global->cpos; i++) 
        generate_ir(global->children[i]);
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
    compile(argv[1]);
    free_memory();
}