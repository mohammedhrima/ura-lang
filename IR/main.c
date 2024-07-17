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

Token *new_token(char *input, int s, int e, Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    switch (type)
    {
    case int_:
    {
        while (s < e)
            new->Int.value = new->Int.value * 10 + input[s++] - '0';
        break;
    }
    case id_:
    {
        new->name = calloc(e - s + 1, sizeof(char));
        strncpy(new->name, input + s, e - s);
        break;
    }
    default:
        break;
    }
    add_token(new);
    return (new);
}

void tokenize(char *input)
{
    int i = 0;
    while (input[i])
    {
        int s = i;
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        bool found = false;
        for (int j = 0; specials[j].value; j++)
        {
            if (strncmp(specials[j].value, input + i, strlen(specials[j].value)) == 0)
            {
                new_token(NULL, 0, 0, specials[j].type);
                found = true;
                i += strlen(specials[j].value);
                break;
            }
        }
        if (found)
            continue;
        if (isalpha(input[i]))
        {
            while (isalnum(input[i]))
                i++;
            new_token(input, s, i, id_);
            continue;
        }
        if (isdigit(input[i]))
        {
            while (isdigit(input[i]))
                i++;
            new_token(input, s, i, int_);
            continue;
        }
    }
    new_token(NULL, 0, 0, end_);
}
#endif

#if AST
Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    return new;
}

Token *check(Type *types)
{
    int i = 0;
    while (types[i])
    {
        if (tokens[exe_pos]->type == types[i])
            return tokens[exe_pos++];
        i++;
    }
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add_sub();
    Token *token = NULL;
#if 1
    while ((token = check((Type[]){assign_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
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

Node *add_sub()
{
    Node *left = mul_div();
    Token *token = NULL;
#if 1
    while ((token = check((Type[]){add_, sub_, 0})))
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
    while ((token = check((Type[]){mul_, div_, 0})))
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

Specials DataTypes[] = {
    {"int", int_},
    {"bool", bool_},
    {0, 0},
};

Node *prime()
{
    Node *node = NULL;
    Token *token = check((Type[]){int_, id_, lpar_, rpar_, 0});
    if (token)
    {
        bool found = false;
        for (int i = 0; token->type == id_ && DataTypes[i].value; i++)
        {
            if (strcmp(token->name, DataTypes[i].value) == 0)
            {
                Type type = DataTypes[i].type;
                token = check((Type[]){id_, 0});
                if (token)
                {
                    node = new_node(token);
                    node->token->type = type;
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
            if (!check((Type[]){rpar_, 0}))
            {
                printf("Error: Expected )\n");
                exit(1);
            }
        }
        else
        {
            node = new_node(token);
            if (check((Type[]){lpar_, 0}))
            {
                node->token->type = fcall_;
                Node *curr = node;
                while (!check((Type[]){rpar_, 0}))
                {
                    curr->left = expr();
                    if (!check((Type[]){coma_, 0}))
                    {
                        // TODO: syntax error
                    }
                    curr->right = new_node(NULL);
                    curr = curr->right;
                }
            }
        }
    }
    return node;
}
#endif

#if IR
void copy_insts()
{
    if (insts)
        free(insts);
    insts = calloc(inst_size, sizeof(Inst *)); // TODO: protect it if no instruction created
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        // if (!first_insts[i]->remove)
        insts[j++] = first_insts[i];
    }
}

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
    regs[inst->token->reg] = inst;
}

Inst *new_inst(Node *node)
{
    printf("new instruction has type %s\n", to_string(node->token->type));
    Inst *new = calloc(1, sizeof(Inst));
    new->token = node->token;
    // if (node->left) // it's handled int generate IR
    //     new->left = node->left->token;
    // if (node->right)
    //     new->right = node->right->token;
    if (node->token->name)
    {
        for (int i = 0; i < inst_pos; i++)
        {
            if (
                first_insts[i]->token->name &&
                !strcmp(first_insts[i]->token->name, node->token->name))
            {
                printf("%sRedefinition of %s%s\n", RED, node->token->name, RESET);
                exit(1);
            }
        }
        if (node->token->declare)
            allocate_reg(new, ++reg_pos);
    }
    // else
    // {
    //     switch(node->token->type)
    //     {
    //         case id_:
    //             break;
    //     }
    // }
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
    case int_:
    {
        inst = new_inst(node);
        break;
    }
    case assign_:
    {
        Token *left = generate_ir(node->left);
        Token *right = generate_ir(node->right);
        inst = new_inst(node);
        inst->left = left;
        inst->right = right;
        break;
    }
    default:
        break;
    }
    return inst->token;
}

bool check_type(Type *types, Type type)
{
    for (int i = 0; types[i]; i++)
    {
        if (types[i] == type)
            return true;
    }
    return false;
}

void print_ir()
{
    printf(SPLIT);
    int i;
    for (i = 0; insts[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;
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
                printf("%lld", right->Int.value);
            printf("\n");
            break;
        }
        case int_:
        {
            if (curr->declare)
                printf("r%.2d: declare %s\n", curr->reg, curr->name);
            break;
        }
        default:
            break;
        }
#if 0
        switch (insts[i]->type)
        {
        case add_:
        case sub_:
        case mul_:
        case div_:
        case assign_:
        {
            // printf("%s: ", to_string(curr->type));

            printf("r%.2d: ", curr->reg);
            char *name = regs[left->reg]->token->name;
            printf("%s (%s) r%d ", to_string(curr->type), name ? name : "", left->reg);
            Type type = regs[right->reg]->type;
            if (
                regs[right->reg]->name || 
                check_type((Type[]){add_, sub_, mul_, div_, assign_, 0}, type))
                printf("r%d\n", right->reg);
            else
                printf("%d\n", regs[right->reg]->token->Int.value);
            break;
        }
        case int_:
        {
            if (regs[curr->reg]->name && regs[curr->reg]->declare)
                printf("r%.2d: declare %s\n", curr->reg, curr->name);
            else
                continue;
            break;
        }
        default:
            break;
        }
        j++;
#endif
    }
    printf("%.2d Instructions on total\n", i);
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

int main(int argc, char **argv)
{
    char *input = open_file("file.w");
    if (input == NULL)
    {
        printf("Error: openning file\n");
        exit(1);
    }

#if TOKENIZE
    tokenize(input);
    for (int i = 0; i < tk_pos; i++)
        print_token(tokens[i]);
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
        print_node(curr->left, NULL, 0);
        curr = curr->right;
    }
#endif

#if IR
    curr = head;
    while (curr->left)
    {
        generate_ir(curr->left);
        curr = curr->right;
    }
    copy_insts();
    print_ir();
#endif

#if AST
    clear(head, input);
#else
    clear(input);
#endif
}