#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

typedef enum Type
{
    assign_ = '=',
    add_ = '+',
    sub_ = '-',
    mul_ = '*',
    div_ = '/',
    int_ = 'i',
    lpar_ = '(',
    rpar_ = ')',
    coma_ = ',',
    id_ = 'I',
    fcall_ = 'c',
    arg_ = 'a',
    end_ = 'e',
} Type;

typedef struct Token
{
    Type type;
    int value;
    char *name;
    bool declare;
    // int reg;
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

typedef struct Inst
{
    // Token *token;
    Type type;
    // registers
    int r1;
    int r2;
    int r3;

    int value;
    char *name;
    bool remove;

    // Token *left;
    // Token *right;
} Inst;

// GLOBALS
extern Token **tokens;
extern int tk_size;
extern int tk_pos;
extern Inst **insts;
extern int inst_size;
extern int inst_pos;

char *open_file(char *filename);
void free_node(Node *node);
char *to_string(Type type);
void clear(Node *head, char *input);
void print_token(Token *token);
void print_node(Node *node, char *side, int space);

#endif

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
    case fcall_:
    {
        printf("[func call] name [%s]", token->name);
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
        if (node->token)
        {
            printf("node: ");
            print_token(node->token);
            print_node(node->left, "LEFT ", space + 5);
            print_node(node->right, "RIGHT", space + 5);
        }
        else
            printf("\n");
    }
}

// UTILS
char *open_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input = calloc(size + 1, sizeof(char));
    fread(input, size, sizeof(char), file);
    fclose(file);
    return input;
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
    case lpar_:
        return "LPARENT";
    case rpar_:
        return "RPARENT";
    case fcall_:
        return "FUNC CALL";
    case end_:
        return "END";
    default:
        break;
    }
    return NULL;
}

void clear(Node *head, char *input)
{
    free_node(head);
    for (int i = 0; i < tk_pos; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        free(tokens[i]);
    }
    for (int i = 0; i < inst_pos; i++)
    {
        // if(insts)
        free(insts[i]);
    }
    free(insts);
    free(input);
}

// TOKENIZE
Token **tokens;
int tk_size;
int tk_pos;
int exe_pos;

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
    if (type == int_)
    {
        while (s < e)
            new->value = new->value * 10 + input[s++] - '0';
    }
    else if (type == id_ || type == fcall_)
    {
        new->name = calloc(e - s + 1, sizeof(char));
        strncpy(new->name, input + s, e - s);
    }
    add_token(new);
    return (new);
}

void tokenize(char *input)
{
    struct
    {
        char *value;
        Type type;
    } specials[] = {
        {"=", assign_},
        {"+", add_},
        {"-", sub_},
        {"*", mul_},
        {"/", div_},
        {"(", lpar_},
        {")", rpar_},
        {",", coma_},
        {0, (Type)0},
    };

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

// ABSTRACT SYNTAX TREE
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
    Token *token = check((Type[]){int_, id_, lpar_, rpar_, 0});
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
                node->token->declare = true;
            }
            else
            {
                printf("Error");
                exit(1);
            }
        }
        else if (token->type == lpar_)
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
                    if (check((Type[]){coma_, 0}))
                        ;
                    curr->right = new_node(NULL);
                    curr = curr->right;
                }
            }
        }
    }
    return node;
}

// VARIABLES
Token **vars;
int var_size;
int var_pos;

void new_variable(Token *token)
{
    if (vars == NULL)
    {
        var_size = 10;
        vars = calloc(var_size, sizeof(Token *));
    }
    else if (var_pos + 1 == var_size)
    {
        Token **tmp = calloc(var_size * 2, sizeof(Token *));
        memcpy(tmp, vars, var_pos * sizeof(Token *));
        free(vars);
        vars = tmp;
    }
    vars[var_pos++] = token;
}

// INSTRUCTIONS
Inst **insts;
int inst_size;
int inst_pos;

void add_inst(Inst *inst)
{
    if (insts == NULL)
    {
        inst_size = 100;
        insts = calloc(inst_size, sizeof(Inst *));
    }
    else if (inst_pos + 1 == inst_size)
    {
        Inst **tmp = calloc(inst_size * 2, sizeof(Inst *));
        memcpy(tmp, insts, inst_pos * sizeof(Inst *));
        free(insts);
        insts = tmp;
        inst_size *= 2;
    }
    insts[inst_pos++] = inst;
}

Inst *regs[100];
int reg = 1;

Inst *new_inst(Node *node)
{
    Inst *new = calloc(1, sizeof(Inst));
    if (node->token->type == int_)
    {
        new->type = node->token->type;
        new->value = node->token->value;
        new->r1 = reg++;
        if (node->token->name)
        {
            for (int i = 0; i < inst_pos; i++)
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
    add_inst(new);
    return new;
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

bool is_operation(Type type)
{
    switch (type)
    {
    case add_:
    case sub_:
    case mul_:
    case div_:
    case assign_:
        return true;
    default:
        break;
    }
    return false;
}

void print_ir()
{
    printf(SPLIT);
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (insts[i]->remove)
            continue;
        switch (insts[i]->type)
        {
        case add_:
        case sub_:
        case mul_:
        case div_:
        case assign_:
        {
            // printf("%s: ", to_string(insts[i]->type));
            printf("r%.2d: ", insts[i]->r1);
            printf("%s  r%d  ", to_string(insts[i]->type), insts[i]->r2);
            if (regs[insts[i]->r3]->name || is_operation(regs[insts[i]->r3]->type))
            {
#if 1
                printf("r%d\n", insts[i]->r3);
#else
                printf("r%d ", insts[i]->r3);
                if (regs[insts[i]->r3]->name)
                    printf(" has name %s", regs[insts[i]->r3]->name);
                else if (is_operation(insts[i]->type))
                    printf("OP %s", to_string(insts[i]->type));
                printf("\n");
#endif
            }
            else
                printf("%d\n", regs[insts[i]->r3]->value);
            break;
        }
        case int_:
        {
            if (regs[insts[i]->r1]->name)
                printf("r%.2d: declare %s\n", insts[i]->r1, insts[i]->name);
            else
            {
                continue;
                // printf("r%.2d: value   %d (%s)\n",insts[i]->r1, insts[i]->value, to_string(insts[i]->type));
            }
            break;
        }
        default:
            break;
        }
        j++;
    }
    printf("%.2d Instructions on total\n", j);
    printf(SPLIT);
}

bool is_math_operation(Type type)
{
    return type == add_ || type == sub_ || type == mul_ || type == div_;
};

int op_index;
void optimize_ir1()
{
    printf("OPTIMIZATION %d (calculate operations on numbers)\n", ++op_index);
    bool op = true;
    int i = 0;
    while (i < inst_pos)
    {
        if (is_math_operation(insts[i]->type))
        {
            int c = insts[i]->r1;
            int l = insts[i]->r2;
            int r = insts[i]->r3;
            if (
                regs[l]->type == int_ && regs[r]->type == int_ &&
                !regs[l]->name && !regs[r]->name)
            {
                switch (insts[i]->type)
                {
                case add_:
                    insts[i]->value = regs[l]->value + regs[r]->value;
                    break;
                case sub_:
                    insts[i]->value = regs[l]->value - regs[r]->value;
                    break;
                case mul_:
                    insts[i]->value = regs[l]->value * regs[r]->value;
                    break;
                case div_:
                    insts[i]->value = regs[l]->value / regs[r]->value;
                    break;
                default:
                    break;
                }
                insts[i]->type = int_;
                regs[l]->remove = true;
                regs[r]->remove = true;
            }
        }
        i++;
    }
}

void optimize_ir2()
{
    printf("OPTIMIZATION %d (remove reassigned variables)\n", ++op_index);
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
                    // if used some where
                    if (insts[j]->r2 == insts[i]->r1 || insts[j]->r3 == insts[i]->r1)
                        break;
                }
                j++;
            }
            if (j == inst_pos)
            {
#if 0
                int l = insts[i]->r2;
                char *name = regs[l]->name;
                printf("%s<%s> Not found%s\n", RED, name, RESET);
                exit(1);
#endif
            }
        }
        i++;
    }
}

int main()
{
    char *input = open_file("002.w");
    if (input == NULL)
    {
        printf("Error: openning file\n");
        exit(1);
    }
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
    // GENERATE IR
    curr = head;
    while (curr->left)
    {
        generate(curr->left);
        curr = curr->right;
    }
    print_ir();
    optimize_ir1();
    print_ir();
    optimize_ir2();
    print_ir();
    clear(head, input);
}