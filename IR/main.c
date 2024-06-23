#include "header.h"

Token *tokens[100];
int tk_pos;
int exe_pos;
int reg = 1;

Token *new_token(char *input, int s, int e, Type type)
{
    Token *new = calloc(1, sizeof(Token));
    new->type = type;
    if (type == int_)
    {
        while (s < e)
            new->value = new->value * 10 + input[s++] - '0';
    }
    else if (type == id_)
    {
        new->name = calloc(e - s + 1, sizeof(char));
        strncpy(new->name, input + s, e - s);
    }
    return (tokens[tk_pos++] = new);
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
        if (strchr("+/*-()=", input[i]))
        {
            new_token(input, 0, 0, input[i]);
            i++;
            continue;
        }
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
    new_token(input, 0, 0, end_);
}

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
    while ((token = check((Type[]){assign_, 0})))
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
    Token *token = NULL;
    while ((token = check((Type[]){add_, sub_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    Token *token = NULL;
    while ((token = check((Type[]){mul_, div_, 0})))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = prime();
        left = node;
    }
    return left;
}

Node *prime()
{
    Node *node = NULL;
    Token *token = check((Type[]){int_, id_, 0});
    if (token)
    {
        if (token->type == id_ && strcmp(token->name, "int") == 0)
        {
            Type type = int_;
            token = check((Type[]){id_, 0});
            if (token)
            {
                node = new_node(token);
                node->token->type = type;
            }
            else
            {
                printf("Error");
                exit(1);
            }
        }
        else
            node = new_node(token);
    }
    return node;
}

// DEBUG
void print_token(Token *token)
{
    printf("token ");
    switch (token->type)
    {
    case int_:
    {
        if (token->name)
            printf("[int] name [%s]", token->name);
        else
            printf("[int] value [%d]", token->value);
        break;
    }
    case id_:
        printf("[id] name [%s]", token->name);
        break;
    default:
        printf("[%c]", token->type);
    }
    printf("\n");
}

void print_node(Node *node, char *side, int space)
{
    if (node)
    {
        int i = 0;
        while (i < space)
            i += printf(" ");
        if (side)
            printf("%s:", side);
        printf("node: ");
        print_token(node->token);
        print_node(node->left, "LEFT ", space + 4);
        print_node(node->right, "RIGHT", space + 4);
    }
}

void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

char *to_string(Type type)
{
    switch (type)
    {
    case add_:
        return "ADD   ";
    case sub_:
        return "SUB   ";
    case mul_:
        return "MUL   ";
    case div_:
        return "DIV   ";
    case int_:
        return "INT   ";
    case assign_:
        return "ASSIGN";
    case end_:
        return "END";
    default:
        break;
    }
    return NULL;
}

// Instructions
Inst *insts[100];
Inst *regs[100];
int inst_pos;

Inst *new_inst(Node *node)
{
    Inst *new = calloc(1, sizeof(Inst));
    if (node->token->type == int_)
    {
        new->type = node->token->type;
        new->r1 = reg++;
        new->value = node->token->value;
        if (node->token->name)
        {
            for (int i = 0; insts[i]; i++)
            {
                if (insts[i]->name && strcmp(insts[i]->name, node->token->name) == 0)
                {
                    printf("%sRedefinition of %s%s\n", RED, node->token->name, RESET);
                    exit(1);
                }
            }
            new->name = node->token->name;
        }
        regs[new->r1] = new;
    }
    else
    {
        new->type = node->token->type;
        if (node->token->type != assign_)
        {
            new->r1 = reg++;
            regs[new->r1] = new;
        }
    }
    // if (node->left)
    //     new->left = node->left->token;
    // if (node->right)
    //     new->right = node->right->token;
    return (insts[inst_pos++] = new);
}

Inst *get_variable(char *name)
{
    for (int i = 0; insts[i]; i++)
    {
        if (insts[i]->name && strcmp(insts[i]->name, name) == 0)
            return insts[i];
    }
    printf("%s%s Not found%s\n", RED, name, RESET);
    exit(1);
    return NULL;
}

int generate(Node *node)
{
    switch (node->token->type)
    {
    case id_:
    {
        Inst *inst = get_variable(node->token->name);
        return inst->r1;
        break;
    }
    case int_:
    {
        Inst *inst = new_inst(node);
        return inst->r1;
        break;
    }
    case assign_:
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        int left = generate(node->left);
        int right = generate(node->right);
        Inst *inst = new_inst(node);
        inst->r2 = left;
        inst->r3 = right;
        if (node->token->type == assign_)
            inst->r1 = left;
        return inst->r1;
        break;
    }
    default:
        break;
    }
    return -1;
}

void print_ir()
{
    printf("PRINT IR\n");
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (insts[i]->remove)
            continue;
#if 1
        switch (insts[i]->type)
        {
        case add_:
        case assign_:
        {
            printf("%2d: ", j++);
            printf("%s: ", to_string(insts[i]->type));
            printf("r%d: ", insts[i]->r1);
            printf("r%d %c ", insts[i]->r2, insts[i]->type);
            if (regs[insts[i]->r3]->name || regs[insts[i]->r3]->type == add_)
                printf("r%d\n", insts[i]->r3);
            else
                printf("%d\n", regs[insts[i]->r3]->value);
            break;
        }
        case int_:
        {
            // printf("r%d: ", insts[i]->r1);
            if (regs[insts[i]->r1]->name)
            {
                // printf("declare %s\n", insts[i]->name);
            }
            else
            {
                // printf("%s: ", to_string(insts[i]->type));
                // printf("r%d: value %d", insts[i]->r1, insts[i]->value);
            }
            break;
        }
        default:
            break;
        }

#else
        printf("%s: ", to_string(insts[i]->type));
        printf("r%d", insts[i]->r1);
        if (insts[i]->type == int_)
            printf(" = %d", insts[i]->value);
        else
            printf(" = r%d %c r%d", insts[i]->r2, insts[i]->type, insts[i]->r3);
#endif
        // printf("\n");
    }
}

void optimize_ir1()
{
    static int index;
    printf("FIRST OPTIMIZATION %d\n", ++index);
    // Inst *regs[100];
    bool op = true;
    int i = 0;
    while (i < inst_pos)
    {
        // regs[insts[i]->r1] = insts[i];
        if (insts[i]->type == add_)
        {
            int c = insts[i]->r1;
            int l = insts[i]->r2;
            int r = insts[i]->r3;
            bool cond = regs[l]->type == int_ && regs[r]->type == int_;
            cond = cond && !regs[l]->name && !regs[r]->name;
            if (cond)
            {
                printf("ADD is %d has left %d, right %d\n", c, l, r);
                insts[i]->type = int_;
                insts[i]->value = regs[l]->value + regs[r]->value;
                regs[l]->remove = true;
                regs[r]->remove = true;
            }
        }
        // else
        // regs[insts[i]->r1] = insts[i];
        i++;
    }
}

void optimize_ir2()
{
    static int index;
    printf("SECOND OPTIMIZATION %d\n", ++index);
    int i = 0;
    while (i < inst_pos)
    {
        if (insts[i]->type == assign_ && !insts[i]->remove)
        {
            int j = i + 1;
            while (j < inst_pos)
            {
                if (insts[j]->type == assign_ && insts[j]->r2 == insts[i]->r2)
                {
                    insts[i]->remove = true;
                    break;
                }
                else
                {
                    if (insts[j]->r2 == insts[i]->r1 || insts[j]->r3 == insts[i]->r1)
                        break;
                }
                j++;
            }
        }
        i++;
    }
}

void simulate_ir()
{
    if (!inst_pos)
        return;
    printf("SIMULATE IR (%d instructions)\n", inst_pos);
    int regs[100];
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (insts[i]->remove)
            continue;
        switch (insts[i]->type)
        {
        case assign_:
        {
            regs[insts[i]->r2] = regs[insts[i]->r3];
            break;
        }
        case int_:
        {
            regs[insts[i]->r1] = insts[i]->value;
            break;
        }
        case add_:
        {
            regs[insts[i]->r1] = regs[insts[i]->r2] + regs[insts[i]->r3];
            break;
        }
        case sub_:
        {
            regs[insts[i]->r1] = regs[insts[i]->r2] - regs[insts[i]->r3];
            break;
        }
        case mul_:
        {
            regs[insts[i]->r1] = regs[insts[i]->r2] * regs[insts[i]->r3];
            break;
        }
        case div_:
        {
            regs[insts[i]->r1] = regs[insts[i]->r2] / regs[insts[i]->r3];
            break;
        }
        default:
            break;
        }
    }
    for (int i = 0; insts[i]; i++)
    {
        if (insts[i]->name)
            printf("%s: %d\n", insts[i]->name, regs[insts[i]->r1]);
    }
}

int main()
{
    char *input = open_file("file.w");
    tokenize(input);
    for (int i = 0; i < tk_pos; i++)
        print_token(tokens[i]);
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
    printf(SPLIT);
#if 1
    printf("%s\n", input);
    curr = head;
    while (curr->left)
    {
        generate(curr->left);
        curr = curr->right;
    }
    printf(SPLIT);
    print_ir();

    printf(SPLIT);
    optimize_ir1();
    print_ir();

    printf(SPLIT);
    optimize_ir1();
    print_ir();

    printf(SPLIT);
    optimize_ir2();
    print_ir();

    printf(SPLIT);
    simulate_ir();
#endif
    free_node(head);
    for (int i = 0; i < tk_pos; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        free(tokens[i]);
    }
    for (int i = 0; i < inst_pos; i++)
        free(insts[i]);
    free(input);
}

/*
a ← 4         a is tagged as #1
b ← 5         b is tagged as #2
c ← a + b     c (#1 + #2) is tagged as #3
d ← 5         d is tagged as #2, the same as b
e ← a + d     e, being '#1 + #2' is tagged as #3

a = 4
b = 5
c = a + b
d = 5
e = a + d

struct Values
{
    int number;

    Value *left;
    Value *right;
    int op;
};


*/