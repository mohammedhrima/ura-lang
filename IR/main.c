#include "utils.c"

#if TOKENIZE
void add_token(Token *token)
{
    if (tokens == NULL)
    {
        tk_size = 100;
        tokens = calloc(tk_size, sizeof(Token *));
    }
    else if (tk_pos + 1 == tk_size)
    {
        Token **tmp = calloc(tk_size * 2, sizeof(Token *));
        memcpy(tmp, tokens, tk_pos * sizeof(Token *));
        tk_size *= 2;
        free(tokens);
        tokens = tmp;
    }
    tokens[tk_pos++] = token;
}

Token *new_token(char *input, int s, int e, int space, Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    new->space = space;
    switch (type)
    {
    case int_:
    {
        while (s < e)
            new->Int.value = new->Int.value * 10 + input[s++] - '0';
        break;
    }
    case string_:
    {
        new->String.value = calloc(e - s + 1, sizeof(char));
        strncpy(new->String.value, input + s, e - s);
        break;
    }
    case fcall_:
    case id_:
    case jmp_:
    case bloc_:
    {
        new->name = calloc(e - s + 1, sizeof(char));
        strncpy(new->name, input + s, e - s);
        if (strcmp(new->name, "True") == 0)
        {
            free(new->name);
            new->name = NULL;
            new->type = bool_;
            new->Bool.value = true;
        }
        else if (strcmp(new->name, "False") == 0)
        {
            free(new->name);
            new->name = NULL;
            new->type = bool_;
            new->Bool.value = false;
        }
        break;
    }
    default:
        break;
    }
    add_token(new);
    return (new);
}

Token *copy_token(Token *token)
{
    Token *new = calloc(1, sizeof(Token));
    memcpy(new, token, sizeof(Token));
    if(token->name)
        new->name = strdup(token->name);
    add_token(new);
    return new;
}

void tokenize(char *input)
{
    int i = 0;
    int space = 0;
    bool new_line = true;
    while (input[i])
    {
        int s = i;
        if (isspace(input[i]))
        {
            if(input[i] == '\n')
            {
                new_line = true;
                space = 0;
            }
            if(new_line && input[i] == ' ')
                space++;
            else if(!new_line && input[i] == ' ')
                space = 0;
            i++;
            continue;
        }
        else if(input[i] == '#') // TODO: handle new lines inside comment
        {
            i++;
            while(input[i] && input[i] != '#')
                i++;
            if(input[i] != '#')
            {
                printf("Error: expected '#'\n");
                exit(1);
            }
            i++;
            continue;
        }
        new_line = false;
        bool found = false;
        for (int j = 0; specials[j].value; j++)
        {
            if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
            {
                new_token(NULL, 0, 0, space, specials[j].type);
                found = true;
                i += strlen(specials[j].value);
                break;
            }
        }
        if (found)
            continue;
        if(input[i] == '\"')
        {
            i++;
            while(input[i] && input[i] != '\"')
                i++;
            if(input[i] != '\"')
            {
                printf("Error: expected '\"'\n");
                exit(1);
            }
            i++;
            new_token(input, s, i, space, string_);
            continue;
        }
        if (isalpha(input[i]))
        {
            while (isalnum(input[i]))
                i++;
            new_token(input, s, i, space, id_);
            continue;
        }
        if (isdigit(input[i]))
        {
            while (isdigit(input[i]))
                i++;
            new_token(input, s, i, space, int_);
            continue;
        }
        if(input[i])
        {
            printf("Syntax Error: <%c>\n", input[i]);
            exit(1);
        }
    }
    new_token(NULL, 0, 0, space, end_);
}
#endif

#if AST
Node *expr();
Node *assign();
Node *equality();
Node *comparison();
Node *add_sub();
Node *mul_div();
Node *prime();

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    printf("new node has ");
    if(token)
        ptoken(token);
    else
        printf("NULL\n");
    return new;
}

Node *copy_node(Node *node)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = copy_token(node->token);
    return new;
}

Token *check(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == tokens[exe_pos]->type)
            return tokens[exe_pos++];
        type = va_arg(ap, Type);
    }
    return NULL;
};

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = equality();
    Token *token;
#if 1
    while ((token = check(assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, 0)))
    {
        Node *node = new_node(token);
        node->token->space = left->token->space;
        Node *right = equality();
        switch(token->type)
        {
            case add_assign_:
            {
                node->token->type = assign_;
                Node *tmp = new_node(new_token(NULL, 0, 0, node->token->space, add_));
                tmp->left = copy_node(left);
                tmp->left->token->declare = false;
                tmp->right = right;
                right = tmp;
                break;
            }
            default:
                break;
        }
        node->left = left;
        node->right = right;
        left = node;
    }
#else
    if ((token = check((Type[]){assign_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = assign();
        left = node;
    }
#endif
    return left;
}

Node *equality()
{
    Node *left = comparison();
    Token *token;
    while ((token = check(equal_, not_equal_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = comparison();
        left = node;
    }
    return left;
}

Node *comparison()
{
    Node *left = add_sub();
    Token *token;
    while ((token = check(less_, more_, less_equal_, more_equal_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    Token *token;
#if 1
    while ((token = check(add_, sub_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
#else
    if ((token = check((Type[]){add_, sub_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        left = node;
    }
#endif
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    Token *token = NULL;
#if 1
    while ((token = check(mul_, div_, 0)))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = prime();
        left = node;
    }
#else
    if ((token = check((Type[]){mul_, div_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
#endif
    return left;
}

Specials DataTypes[] = {{"int", int_}, {"bool", bool_}, {"string", string_}, {0, 0}};

Node *prime()
{
    Node *node = NULL;
    Token *token;
    if ((token  = check(int_, bool_, string_, id_, lpar_, rpar_, 0)))
    {
        bool found = false;
        for (int i = 0; token->type == id_ && DataTypes[i].value; i++)
        {
            if (strcmp(token->name, DataTypes[i].value) == 0)
            {
                Type type = DataTypes[i].type;
                int space = token->space;
                token = check(id_, 0);
                if (token)
                {
                    node = new_node(token);
                    node->token->type = type;
                    node->token->space = space;
                    node->token->declare = true;
                    return node;
                }
                else
                {
                    printf("Error");
                    exit(1);
                }
            }
        }
        if (token->type == lpar_)
        {
            node = expr();
            if (!check(rpar_, 0))
            {
                printf("Error: Expected )\n");
                exit(1);
            }
        }
        else
        {
            node = new_node(token);
            if (check(lpar_, 0)) // TODO: only if is identifier
            {
                node->token->type = fcall_;
                Node *curr = node;
                while (!check(rpar_, 0))
                {
                    curr->left = expr();
                    if (!check(coma_, 0))
                    {
                        // TODO: syntax error
                    }
                    curr->right = new_node(NULL);
                    curr = curr->right;
                }
            }
        }
    }
    else if((token  = check(if_, 0)))
    {
        /*
            + if:
                + LEFT : 
                    + LEFT : condition
                    + RIGHT: code bloc
                + RIGHT:
                    + LEFT :
                        + elif:
                            + LEFT : condition
                            + RIGHT: code bloc
                    + else:
                        + RIGHT: 
                            + LEFT + code bloc

        */
#if 0
        node = new_node(token);
        node->left = new_node(NULL);
        Node *curr = node->left;
        curr->left = expr();
        if(!check(dots_, 0))
        {
            printf("Error: expected : but found <%s>\n", to_string(tokens[exe_pos]->type));
            exit(1);
        }
        curr->right = new_node(NULL);
        curr = curr->right;
        while
        (
            tokens[exe_pos]->type != end_ && 
            tokens[exe_pos]->space > node->token->space
        )
        {
            curr->left = expr();
            curr->right = new_node(NULL);
            curr = curr->right;
        }
        while((token = check(elif_, 0)))
        {
            if(token->space != node->token->space)
            {
                printf("Error: misplaced elif statement\n");
                exit(1);
            }

            curr->left = new_node(token);
            curr->right = new_node(NULL);

            Node *tmp0 = curr->left;
            tmp0->left = expr();
            if(!check(dots_, 0))
            {
                printf("Error: expected : after elif\n");
                exit(1);
            }
            tmp0->right = new_node(NULL);
            Node *tmp1 = tmp0->right;
            while
            (
                tokens[exe_pos]->type != end_ && 
                tokens[exe_pos]->space > node->token->space
            )
            {
                tmp1->left = expr();
                tmp1 = tmp1->right;
            }
            curr = curr->right;
        }
        if((token = check(else_, 0)))
        {
            if(!check(dots_, 0))
            {
                printf("Error: expected ':' after else\n");
                exit(1);
            }
            if(token->space != node->token->space)
            {
                printf("Error: misplaced else statement\n");
                exit(1);
            }
            curr->right = new_node(NULL);
            
            curr = curr->right;
            curr->left = new_node(token);
            curr = curr->left;
            while(tokens[exe_pos]->type != end_ && tokens[exe_pos]->space > node->token->space)
            {
                curr->left = expr();
                curr->right = new_node(NULL);
                curr = curr->right;
            }
        }
#endif
        node = new_node(token);
        
        Node *tmp = node;
        tmp->left = new_node(NULL);
        tmp = tmp->left;

        tmp->left = expr(); // if condition
        if(!check(dots_, 0))
            ; // TODO: expected ':' after condition
        
        tmp->right = new_node(NULL);
        tmp = tmp->right;
        while(tokens[exe_pos]->space > node->token->space) // if bloc code
        {
            tmp->left = expr();
            tmp->right = new_node(NULL);
            tmp = tmp->right;
        }

        Node *curr = node;
        while
        (
            (token = check(elif_, 0)) && 
            token->space > node->token->space
        )
        {                
            curr->right = new_node(NULL);
            tmp = curr->right;
            tmp->left = new_node(token);
            tmp = tmp->left;
            tmp->left = expr(); // elif condition
            if(!check(dots_, 0))
                ; // TODO: expected ':' after condition
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            while(tokens[exe_pos]->space > node->token->space)
            {
                tmp->left = expr();
                tmp->right = new_node(NULL);
                tmp = tmp->right;
            }
            curr = curr->right;
        }
        if((token = check(else_, 0))) // TODO: check space
        {
            if(!check(dots_, 0))
                ; // Error: expected ':'
            curr->left = new_node(token);
            curr = curr->left;
            while(tokens[exe_pos]->space > node->token->space)
            {
                curr->left = expr();
                curr->right = new_node(NULL);
                curr = curr->right;
            }
            curr->right = new_node(NULL);
        }

    }
    else if(tokens[exe_pos]->type == end_);
    else
    {
        printf("Error: Unexpected token <%s>\n", to_string(tokens[exe_pos]->type));
        exit(1);
    }
    return node;
}
#endif

#if IR

void add_inst(Inst *inst)
{
    if (first_insts == NULL)
    {
        inst_size = 100;
        first_insts = calloc(inst_size, sizeof(Inst *));
    }
    else if (inst_pos + 1 == inst_size)
    {
        Inst **tmp = calloc(inst_size * 2, sizeof(Inst *));
        memcpy(tmp, first_insts, inst_pos * sizeof(Inst *));
        free(first_insts);
        first_insts = tmp;
        inst_size *= 2;
    }
    first_insts[inst_pos++] = inst;
}

void allocate_reg(Inst *inst, int pos)
{
    inst->token->reg = pos;
    if (regs == NULL)
    {
        reg_size = 10;
        regs = calloc(reg_size, sizeof(Inst *));
    }
    else if (reg_pos + 1 == reg_size)
    {
        Inst **tmp = calloc(reg_size * 2, sizeof(Inst *));
        memcpy(tmp, regs, reg_pos * sizeof(Inst *));
        reg_size *= 2;
        free(regs);
        regs = tmp;
    }
    // TODO: remove regs
    regs[inst->token->reg] = inst;
}

size_t ptr = 0;
Inst *new_inst(Token *token)
{
    printf("new instruction has type %s\n", to_string(token->type));
    Inst *new = calloc(1, sizeof(Inst));
    new->token = token;
    if (token->name && token->declare)
    {
        for (int i = 0; i < inst_pos; i++)
        {
            if (
                first_insts[i]->token->name &&
                !strcmp(first_insts[i]->token->name, token->name))
            {
                printf("%sRedefinition of %s%s\n", RED, token->name, RESET);
                exit(1);
            }
        }
        if (token->declare)
        {
            token->ptr = (ptr += 8);
            allocate_reg(new, ++reg_pos);
        }
    }
    else
    {
        switch (token->type)
        {
        case add_: case sub_: case mul_: case div_: case equal_:
        case less_: case more_: case less_equal_: case more_equal_:
        case not_equal_: case fcall_:
            allocate_reg(new, ++reg_pos);
            break;
        case if_:
            break;
        default:
            break;
        }
    }
    add_inst(new);
    return new;
}

Token *get_variable(char *name)
{
    for (int i = 0; first_insts[i]; i++)
    {
        if (first_insts[i]->token->name && strcmp(first_insts[i]->token->name, name) == 0)
            return first_insts[i]->token;
    }
    printf("%s%s Not found%s\n", RED, name, RESET);
    exit(1);
    return NULL;
}

size_t bloc_index;
size_t str_index;
Token *generate_ir(Node *node)
{
    Inst *inst = NULL;
    switch (node->token->type)
    {
    case id_:
    {
        Token *token = get_variable(node->token->name);
        return token;
        break;
    }
    case if_:
    {
        // printf("handle if\n"), exit(1);
        Node *tmp = node;
        Node *curr = node->left;

        // condition
        Token *result = generate_ir(curr->left); // TODO: check if it's boolean

        Node *lastnode = node;

        lastnode->token->type = jne_;
        lastnode->token->name = strdup("endif");
        lastnode->token->index = ++bloc_index;
        new_inst(lastnode->token); // jne to endif

        curr = curr->right;
        while(curr->left) // if code bloc
        {
            generate_ir(curr->left);
            curr = curr->right;
        }

        curr = node->right;
        while(curr)
        {
            if(curr->left->token->type == elif_)
            {
                curr->left->token->index = ++bloc_index;
                curr->left->token->type = bloc_;
                curr->left->token->name = strdup("elif");

                if(strcmp(lastnode->token->name, "endif") == 0)
                {
                    free(lastnode->token->name);
                    lastnode->token->name = strdup("elif");
                    lastnode->token->index = curr->left->token->index;
                }
                new_inst(curr->left->token);
                Node *tmp = curr->left;
                generate_ir(tmp->left); // elif condition, TODO: check is boolean

                lastnode = tmp->left;
                lastnode->token->type = jne_;
                lastnode->token->name = strdup("endif");
                lastnode->token->index = node->token->index;

                tmp = tmp->right;
                while(tmp->left)
                {
                    generate_ir(tmp->left);
                    tmp = tmp->right;
                }
            }
            else if(curr->left->token->type == else_)
            {
                curr->left->token->index = ++bloc_index;
                curr->left->token->type = bloc_;
                curr->left->token->name = strdup("else");

                if(strcmp(lastnode->token->name, "endif") == 0)
                {
                    free(lastnode->token->name);
                    lastnode->token->name = strdup("else");
                    lastnode->token->index = curr->left->token->index;
                }
                new_inst(curr->left->token);
                Node *tmp = curr->left;
                tmp = tmp->right;
                while(tmp->left)
                {
                    generate_ir(tmp->left);
                    tmp = tmp->right;
                }
            }
            curr = curr->right;
        }
        
        Token *new = NULL;
#if 0
        curr = node->right;
        if(curr && curr->left->token->type == elif_)
        {
            curr = curr->left;
            free(node->token->name);
            node->token->name = strdup("elif");
            new_inst(new_token("elif", 0, 4, node->token->space, bloc_));

            while(curr && curr->left->token->type == elif_)
            {

                curr = curr->right;
            }
        }
#endif
#if 0
        if(curr && curr->left->token->type == else_)
        {
            // TODO: add . before all Labels
            new = new_token("endif", 0, 5, node->token->space, jmp_);
            new->index = node->token->index;
            new_inst(new);

            if(strcmp(node->token->name, "endif") == 0)
            {
                free(node->token->name);
                node->token->name = strdup("else");
            }
            new = new_token("else", 0, 4, node->token->space, bloc_);
            new->index = node->token->index;
            new_inst(new);

            curr = curr->left;
            while(curr->left)
            {
                generate_ir(curr->left);
                curr = curr->right;
            }
        }
#endif
        new = new_token("endif", 0, 5, node->token->space, bloc_);
        new->index = node->token->index;
        new_inst(new);

        return node->left->token;
        break;
    }
    case fcall_:
    {
        pnode(node, NULL, 0);
        if(strcmp(node->token->name, "output") == 0)
        {
            Node *curr = node;
            char *fname = NULL;
            while(curr->left)
            {
                Token *left = generate_ir(curr->left);
                fname = NULL;
                switch(left->type)
                {
                    case string_: fname = "_putstr"; break;
                    case int_:    fname = "_putnbr"; break;
                    default:
                        break;
                }
                if(fname)
                {
                    inst = new_inst(new_token(fname, 0, strlen(fname), node->token->space, fcall_));
                    inst->token->isbuiltin = true;
                    inst->left = left;
                }
                curr = curr->right;
            }
        }
        else {

        }
        return node->token;
        break;
    }
    case bool_: case int_: case string_:
    {
        inst = new_inst(node->token);
        break;
    }
    case add_: case sub_: case div_: case mul_: case assign_: case not_equal_:
    case equal_: case less_: case more_: case less_equal_: case more_equal_:
    {
        Token *left = generate_ir(node->left);
        Token *right = generate_ir(node->right);
        inst = new_inst(node->token);
        inst->left = left;
        inst->right = right;
        break;
    }
    default:
        break;
    }
    return inst->token;
}

void print_ir()
{
    printf(SPLIT);
    int j = 0;
    for (int i = 0; insts[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;
        if (curr->remove)
            continue;
        switch (curr->type)
        {
        case assign_:
        {
            curr->reg = left->reg;
            printf("r%.2d: %s ", curr->reg, to_string(curr->type));
            printf("%s in (%d) to ", left->name, left->reg);
            if (right->reg)
                printf("r%.2d", right->reg);
            else
            {
                switch (right->type)
                {
                case int_: printf("%lld", right->Int.value); break;
                case bool_: printf("%s", right->Bool.value ? "True" : "False"); break;
                default: break;
                }
            }
            printf("\n");
            break;
        }
        case fcall_:
        {
            printf("call %s\n", curr->name);
            break;
        }
        case add_: case sub_: case mul_: case div_: case equal_:
        case less_: case more_: case less_equal_: case more_equal_:
        case not_equal_:
        {
            printf("r%.2d: %s ", curr->reg, to_string(curr->type));
            if (left->reg)
                printf("r%.2d", left->reg);
            else
                switch(left->type)
                {
                    case int_: printf("%lld", left->Int.value); break;
                    case string_: printf("%s", left->String.value); break;
                    default: break;
                }

            if (left->name)
                printf(" (%s)", left->name);
            printf(" to ");
            if (right->reg)
                printf("r%.2d", right->reg);
            else
                switch(right->type)
                {
                    case int_: printf("%lld", right->Int.value); break;
                    case string_: printf("%s", right->String.value); break;
                    default: break;
                }

            if (right->name)
                printf(" (%s)", right->name);
            printf("\n");
            break;
        }
        case int_: case bool_: case string_:
        {
            if (curr->declare)
                printf("r%.2d: declare %s\n", curr->reg, curr->name);
            else if(curr->type == int_)
                printf("r%.2d: value %lld\n", curr->reg, curr->Int.value);
            else if(curr->type == bool_)
                printf("r%.2d: value %s\n", curr->reg, curr->Bool.value ? "True" : "False");
            else if(curr->type == string_)
                printf("r%.2d: value %s in STR%zu\n", curr->reg, curr->String.value, 
                                                     (curr->index = ++str_index));
            break;
        }
        case jne_:
        {
            printf("rxx: jne %s%zu\n", curr->name, curr->index);
            break;
        }
        case jmp_:
        {
            printf("rxx: jmp %s%zu\n", curr->name, curr->index);
            break;
        }
        case bloc_:
        {
            printf("rxx: %s%zu (bloc)\n", curr->name, curr->index);
            break;
        }
        default:
        {
            printf("%sPrint IR: Unkown inst [%s]%s\n", RED, to_string(curr->type), RESET);
            break;
        }
        }
        j++;
    }
    printf("%.2d Instructions on total\n", j);
#if 0
    printf(SPLIT);
    for (int i = 1; regs[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;

        if (curr->type == int_ && !curr->name)
            printf("r%.2d: value  %d ", curr->reg, curr->value);
        else
            printf("r%.2d: %s r%d r%d ", curr->reg, to_string(curr->type), left->reg, right->reg);
        if (curr->remove)
            printf("remove");
        printf("\n");
    }
#endif
    printf(SPLIT);
}
#endif

char *strjoin(Token *left, Token *right)
{
    size_t len = strlen(left->String.value) + strlen(right->String.value);
    char *res = calloc(len + 1, sizeof(char));
    strcpy(res, left->String.value);
    strcpy(res + strlen(res), right->String.value);
    return res;
}

#define MAX_OPTIMIZATION 4
bool optimize_ir(int op_index)
{
#if 1
    bool did_optimize = false;
    switch (op_index)
    {
    case 0:
    {
        printf("OPTIMIZATION %d (calculate operations on constant type 0)\n", op_index);
        for (int i = 0; insts[i]; i++)
        {
            Token *token = insts[i]->token;
            Token *left = insts[i]->left;
            Token *right = insts[i]->right;
            if (check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i]->token->type))
            {
                if
                (
                    // TODO: check it left nad right are compatible
                    check_type((Type[]){int_, string_, 0}, left->type) &&
                    check_type((Type[]){int_, string_, 0}, right->type) &&
                    !left->name && !right->name
                )
                {
                    switch(left->type)
                    {
                    case int_:
                        switch (token->type)
                        {
                        case add_: token->Int.value = left->Int.value + right->Int.value; break;
                        case sub_: token->Int.value = left->Int.value - right->Int.value; break;
                        case mul_: token->Int.value = left->Int.value * right->Int.value; break;
                        case div_: token->Int.value = left->Int.value / right->Int.value; break;
                        default: break;
                        }
                        break;
                    case string_:
                        switch(token->type)
                        {
                        case add_: token->String.value = strjoin(left, right); break;
                        default:
                            printf("Error: Invalid %s op in string\n", to_string(token->type)); break;
                        }
                    default:
                        break;
                    }
                    token->type = left->type;
                    left->remove = true;
                    right->remove = true;
                    copy_insts();
                    i = 0;
                    did_optimize = true;
                }
            }
            if (token->type == int_ && !token->name)
            {
                // printf("found to remove in r%d\n", token->r1);
                token->remove = true;
                copy_insts();
                i = 0;
                did_optimize = true;
            }
        }
        break;
    }
    case 1:
    {
        // TODO: do comparision operation on numbers etc...
        printf("OPTIMIZATION %d (calculate operations on numbers type 1)\n", op_index);
        int i = 1;
        while (insts[i])
        {
            Token *token = insts[i]->token;
            Token *left = insts[i]->left;
            Token *right = insts[i]->right;
            if (
                //  TODO: handle string also here X'D ma fiyach daba
                check_type((Type[]){add_, sub_, mul_, div_, 0}, token->type) &&
                insts[i - 1]->token->type == add_ &&
                left == insts[i - 1]->token &&
                !insts[i - 1]->right->name &&
                !right->name)
            {
                // printf("%sfound %s\n", RED, RESET);
                token->remove = true;
                switch(token->type)
                {
                case add_: insts[i - 1]->right->Int.value += right->Int.value; break;
                case sub_: insts[i - 1]->right->Int.value -= right->Int.value; break;
                case mul_: insts[i - 1]->right->Int.value *= right->Int.value; break;
                case div_: insts[i - 1]->right->Int.value /= right->Int.value; break;
                default: break;
                }
                // printf("value is %lld\n", insts[i - 1]->right->Int.value);
                if (insts[i + 1]->left == token) insts[i + 1]->left = insts[i - 1]->token;
                i = 1;
                copy_insts();
                did_optimize = true;
                continue;
            }
            i++;
        }
        break;
    }
    case 2:
    {
        printf("OPTIMIZATION %d (remove reassigned variables)\n", op_index);
        for (int i = 0; insts[i]; i++)
        {
            if (insts[i]->token->declare)
            {
                int j = i + 1;
                while (insts[j] && insts[j]->token->space == insts[i]->token->space)
                {
                    ptoken(insts[j]->token);
                    if (insts[j]->token->type == assign_ && insts[j]->left == insts[i]->token)
                    {
                        insts[i]->token->declare = false;
                        insts[i]->token->remove = true;
                        did_optimize = true;
                        break;
                    }
                    if 
                    (
                        insts[j]->left && insts[j]->left &&
                        (
                        insts[j]->left->reg == insts[i]->token->reg || 
                        insts[j]->right->reg == insts[i]->token->reg
                        )
                    )
                        break;
                    j++;
                }
            }
            else if (insts[i]->token->type == assign_)
            {
                int j = i + 1;
                while (insts[j] && insts[j]->token->space == insts[i]->token->space)
                {
                    if 
                    (
                        insts[j]->token->type == assign_ && 
                        insts[j]->left == insts[i]->left
                    )
                    {
                        insts[i]->token->remove = true;
                        did_optimize = true;
                        break;
                    }
                    // if used some where
                    else if 
                    (
                        insts[j]->left->reg == insts[i]->token->reg || 
                        insts[j]->right->reg == insts[i]->token->reg
                    )
                        break;
                    j++;
                }
            }
        }
        break;
    }
    case MAX_OPTIMIZATION - 1:
    {
        printf("OPTIMIZATION %d (remove unused instructions)\n", op_index);
        for(int i = 1; insts[i]; i++)
        {
            if
            (
                check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i - 1]->token->type) &&
                insts[i]->left->reg != insts[i - 1]->token->reg && 
                insts[i]->right->reg != insts[i - 1]->token->reg
            )
            {
                did_optimize = true;
                insts[i - 1]->token->remove = true;
                copy_insts();
                i = 1;
            }
        }
        break;
    }
    default:
        break;
    }
    return did_optimize;
#endif
}

void generate_asm()
{
    for (int i = 0; insts[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;
        switch (curr->type)
        {
        case assign_:
        {
            // TODO: check incompatible type
            curr->ptr = left->ptr;
            pasm("/*assign %s*/\n", left->name);
            if (right->ptr)
            {
                mov("rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                mov("QWORD PTR -%zu[rbp], rax\n", left->ptr);
            }
            else if(right->c)
                mov("QWORD PTR -%zu[rbp], r%cx\n", left->ptr, right->c);
            else
            {
                switch (right->type)
                {
                case int_:
                    mov("QWORD PTR -%zu[rbp], %lld\n", left->ptr, right->Int.value);
                    break;
                case bool_:
                    mov("QWORD PTR -%zu[rbp], %d\n", left->ptr, right->Bool.value);
                    break;
                case string_:
                    lea("rdi, .STR%zu[rip]\n", right->index);
                    call("_strdup");
                    mov("QWORD PTR -%zu[rbp], rax\n", left->ptr);
                    break;
                default:
                    break;
                }
            }
            break;
        }
        case int_: case bool_: case string_:
        {
            if (curr->declare)
            {
                // curr->ptr = (ptr += 8);
                // pasm("r%.2d: declare %s\n", curr->reg, curr->name);
                pasm("/*declare %s*/\n", curr->name);
                mov("QWORD PTR -%zu[rbp], 0\n", curr->ptr);
            }
            else if(curr->type == string_ && !curr->name)
                ; // pasm(".STR%zu: .string %s\n", curr->index, curr->String.value);
            break;
        }
        case add_: case sub_: case mul_: case div_: // TODO: check all math_op operations
        {
            curr->c = 'a';
            if (left->ptr)
                mov("r%cx, QWORD PTR -%zu[rbp]\n", curr->c, left->ptr);
            else if (left->c && left->c != curr->c)
                mov("r%cx, r%cx\n", curr->c, left->c);
            else if (!left->c)
                mov("r%cx, %lld\n", curr->c, left->Int.value);

            if (right->ptr)
                math_op(curr->type, "r%cx, QWORD PTR -%zu[rbp]\n", curr->c, right->ptr);
            else if (right->c) // TODO: to be checked
                math_op(curr->type, "r%cx, r%cx\n", curr->c, right->c);
            else if (!right->c)
                math_op(curr->type, "r%cx, %lld\n", curr->c, right->Int.value);
            break;
        }
        case equal_: case not_equal_: case less_:
        case less_equal_: case more_: case more_equal_:
        {
            curr->c = 'a';
            if (left->ptr)
                mov("r%cx, QWORD PTR -%zu[rbp]\n", curr->c, left->ptr);
            else if (left->c && left->c != curr->c)
                mov("r%cx, r%cx\n", curr->c, left->c);
            else if (!left->c)
                mov("r%cx, %lld\n", curr->c, left->Int.value);

            if (right->ptr)
                mov("rbx, QWORD PTR -%zu[rbp]\n", right->ptr);
            else if (right->c && right->c != 'b') // TODO: to be checked
                mov("rbx, r%cx\n", right->c);
            else if (!right->c)
                mov("rbx, %lld\n", right->Int.value);
            cmp("rax, rbx\n","");
            relational_op(curr->type, "%cl\n", curr->c);
            curr->type = bool_;
            break;
        }
        case fcall_:
        {
            if(curr->isbuiltin)
            {
                if(left->ptr)
                    mov("rdi, QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    switch(left->type)
                    {
                        case int_:    mov("rdi, %lld\n", left->Int.value); break;
                        case string_: lea("rdi, .STR%zu[rip]\n", left->index); break;
                        default: break;
                    }
            }
            call(curr->name);
            break;
        }
        case jne_:
        {
            cmp("al, 1\n", "");
            jne("%s%zu\n", curr->name, curr->index);
            break;
        }
        case jmp_:
        {
            jmp("%s%zu\n", curr->name, curr->index);
            break;
        }
        case bloc_:
        {
            pasm("%s%zu:\n", curr->name, curr->index);
            break;
        }
        default:
            printf("%sGenerate asm: Unkown Instruction [%s]%s\n", 
                RED, to_string(curr->type), RESET);
            break;
        }
    }
}

void initialize()
{
    pasm(".intel_syntax noprefix\n");
    pasm(".include \"./import/header.s\"\n\n");
    pasm(".text\n");
    pasm(".globl	main\n");
    pasm("main:\n");
    pasm("push    rbp\n");
    mov("rbp, rsp\n","");
    pasm("sub     rsp, %zu\n", ptr + 8);
}

void finalize()
{
    pasm("leave\n");
    pasm("ret\n");
#if 1
    for (int i = 0; tokens[i]; i++)
    {
        Token *curr = tokens[i];
        // test char variable before making any modification
        if (curr->type == string_ && !curr->name && !curr->ptr)
            pasm(".STR%zu: .string %s\n", curr->index, curr->String.value);
#if 0
        if (!curr->name && curr->float_index_ && curr->type == float_)
            pasm("FLT%zu: .long %zu /* %f */\n", curr->float_index_, *((float *)(&curr->float_)),
                 curr->float_);
#endif
    }
#endif
    pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
}

int main(int argc, char **argv)
{
    char *input = open_file("file.w");
    if (input == NULL)
    {
        printf("Error: openning file\n");
        exit(1);
    }
    char *output = "file.s";
    asm_fd = fopen(output, "w+");

#if TOKENIZE
    tokenize(input);
    for (int i = 0; i < tk_pos; i++)
        ptoken(tokens[i]);
    printf(SPLIT);
#endif


#if AST
    Node *head = new_node(NULL);
    Node *curr = head;
    curr->left = expr();
    while (curr->left)
    {
        curr->right = new_node(NULL);
        curr = curr->right;
        curr->left = expr();
    }
    curr = head;
    while (curr->left)
    {
        pnode(curr->left, NULL, 0);
        curr = curr->right;
    }
#endif

    if (tk_pos > 1)
    {
#if IR
        curr = head;
        while (curr->left)
        {
            generate_ir(curr->left);
            curr = curr->right;
        }
        copy_insts();
        print_ir();
#if 0
        int i = 0;
        bool optimized = false;
        while(i < MAX_OPTIMIZATION)
        {
            optimized = optimize_ir(i++) || optimized;
            copy_insts();
            print_ir();
            if(i == MAX_OPTIMIZATION && optimized)
            {
                optimized = false;
                i = 0;
            }
        }
#endif

#endif

#if ASM
        initialize();
        generate_asm();
        finalize();
#endif
    }

#if AST
    clear(head, input);
#else
    clear(input);
#endif

}