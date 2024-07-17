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
    bool declare;
    bool remove;
    size_t ptr;
    char c;

    // Token *left;
    // Token *right;
} Inst;

typedef enum Inst_type
{
    ADD,
    SUB,
    MUL,
    DIV,
    MOV,
} Inst_type;

typedef struct Asm
{
    Inst_type type;

    bool is_reg;
    char reg;
    size_t ptr;

    struct Asm *left;
    struct Asm *right;
} Asm;

// GLOBALS
extern Token **tokens;
extern int tk_size;
extern int tk_pos;
extern Inst **insts;
extern int inst_size;
extern int inst_pos;
extern Inst **regs;
extern size_t stack_ptr;

char *open_file(char *filename);
void free_node(Node *node);
char *to_string(Type type);
void clear(Node *head, char *input);
void print_token(Token *token);
void print_node(Node *node, char *side, int space);

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
Inst **first_insts;
Inst **insts;
int inst_size;
int inst_pos;

void copy_insts()
{
    if (insts)
        free(insts);
    insts = calloc(inst_size, sizeof(Inst *)); // TODO: protect it if no instruction created
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (!first_insts[i]->remove)
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

Inst **regs;
int reg_pos;
int reg_size;

void add_reg(Inst *inst)
{
    inst->r1 = ++reg_pos;
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
    regs[inst->r1] = inst;
}

Inst *new_inst(Node *node)
{
    Inst *new = calloc(1, sizeof(Inst));
    if (node->token->type == int_)
    {
        new->type = node->token->type;
        new->value = node->token->value;
        if (node->token->name)
        {
            for (int i = 0; i < inst_pos; i++)
            {
                if (first_insts[i]->name && strcmp(first_insts[i]->name, node->token->name) == 0)
                {
                    printf("%sRedefinition of %s%s\n", RED, node->token->name, RESET);
                    exit(1);
                }
            }
            new->name = node->token->name;
            new->declare = node->token->declare;
        }
        add_reg(new);
    }
    else
    {
        new->type = node->token->type;
        if (node->token->type != assign_)
        {
            add_reg(new);
            regs[new->r1] = new;
        }
    }
    add_inst(new);
    return new;
}

Inst *get_variable(char *name)
{
    for (int i = 0; first_insts[i]; i++)
    {
        if (first_insts[i]->name && strcmp(first_insts[i]->name, name) == 0)
            return first_insts[i];
    }
    printf("%s%s Not found%s\n", RED, name, RESET);
    exit(1);
    return NULL;
}

int generate_ir(Node *node)
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
        int left = generate_ir(node->left);
        int right = generate_ir(node->right);
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
    int j = 0;
    for (int i = 0; insts[i]; i++)
    {
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
            char *name = regs[insts[i]->r2]->name;
            printf("%s (%s) r%d ", to_string(insts[i]->type), name ? name : "", insts[i]->r2);
            Type type = regs[insts[i]->r3]->type;
            if (regs[insts[i]->r3]->name || check_type((Type[]){add_, sub_, mul_, div_, assign_, 0}, type))
                printf("r%d\n", insts[i]->r3);
            else
                printf("%d\n", regs[insts[i]->r3]->value);
            break;
        }
        case int_:
        {
            if (regs[insts[i]->r1]->name && regs[insts[i]->r1]->declare)
                printf("r%.2d: declare %s\n", insts[i]->r1, insts[i]->name);
            else
                continue;
            break;
        }
        default:
            break;
        }
        j++;
    }
    printf("%.2d Instructions on total\n", j);
    printf(SPLIT);
    for (int i = 1; regs[i]; i++)
    {
        if (regs[i]->type == int_ && !regs[i]->name)
            printf("r%.2d: value  %d ", regs[i]->r1, regs[i]->value);
        else
            printf("r%.2d: %s r%d r%d ", regs[i]->r1, to_string(regs[i]->type), regs[i]->r2, regs[i]->r3);
        if (regs[i]->remove)
            printf("remove");
        printf("\n");
    }
    printf(SPLIT);
}

void optimize_ir()
{
    static int op_index;
    switch (op_index)
    {
    case 0:
    {
        printf("OPTIMIZATION %d (calculate operations on numbers type 0)\n", ++op_index);
        int i = 0;
        while (insts[i])
        {
            if (check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i]->type))
            {
                Inst *lreg = regs[insts[i]->r2];
                Inst *rreg = regs[insts[i]->r3];
                if (
                    lreg->type == int_ && rreg->type == int_ &&
                    !lreg->name && !rreg->name)
                {
                    switch (insts[i]->type)
                    {
                    case add_:
                        insts[i]->value = lreg->value + rreg->value;
                        break;
                    case sub_:
                        insts[i]->value = lreg->value - rreg->value;
                        break;
                    case mul_:
                        insts[i]->value = lreg->value * rreg->value;
                        break;
                    case div_:
                        insts[i]->value = lreg->value / rreg->value;
                        break;
                    default:
                        break;
                    }
                    insts[i]->type = int_;
                    lreg->remove = true;
                    rreg->remove = true;
                }
            }
            if (insts[i]->type == int_ && !insts[i]->name)
            {
                printf("found to remove in r%d\n", insts[i]->r1);
                insts[i]->remove = true;
            }
            i++;
        }
        break;
    }
    case 1:
    {
        printf("OPTIMIZATION %d (calculate operations on numbers type 1)\n", ++op_index);
        int i = 1;
        while (insts[i])
        {
            if (
                insts[i]->type == add_ &&
                insts[i - 1]->type == add_ &&
                insts[i]->r2 == insts[i - 1]->r1 &&
                !regs[insts[i - 1]->r3]->name &&
                !regs[insts[i]->r3]->name)
            {
                printf("found r%d\n", insts[i]->r1);
                insts[i]->remove = true;
                regs[insts[i - 1]->r3]->value = regs[insts[i - 1]->r3]->value + regs[insts[i]->r3]->value;
                if (insts[i + 1]->r2 == insts[i]->r1)
                {
                    insts[i + 1]->r2 = insts[i - 1]->r1;
                }
                i = 1;
                copy_insts();
                continue;
            }
            i++;
        }
        break;
    }
    case 2:
    {
        printf("OPTIMIZATION %d (remove reassigned variables)\n", ++op_index);
        for (int i = 0; insts[i]; i++)
        {
            if (insts[i]->declare)
            {
                int j = i + 1;
                while (insts[j])
                {
                    if (insts[j]->type == assign_ && insts[j]->r2 == insts[i]->r1)
                    {
                        insts[i]->declare = false;
                        insts[i]->remove = true;
                        break;
                    }
                    if (insts[j]->r2 == insts[i]->r1 || insts[j]->r3 == insts[i]->r1)
                        break;
                    j++;
                }
            }
            else if (insts[i]->type == assign_)
            {
                int j = i + 1;
                while (insts[j])
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
            }
        }
        break;
    }
    default:
        printf("Optimization %d, Not implemented\n", op_index);
        break;
    }
}

char gen_reg;
size_t stack_ptr;
void generate_asm()
{
    printf("GENERATE ASM\n");
    for (int i = 0; insts[i]; i++)
    {
        Inst *inst = insts[i];
        // printf("GEN: %s -> %s\n", to_string(insts[i]->type), to_string(inst->type));
        if (inst->declare)
        {
            inst->ptr = (stack_ptr += 8);
            printf("/*declare %s*/\n", inst->name);
            printf("mov QWORD PTR -%zu[rbp], 0\n", inst->ptr);
        }
        else
        {
            switch (inst->type)
            {
            case assign_:
            {
                Inst *left = regs[insts[i]->r2];
                Inst *right = regs[insts[i]->r3];
                printf("/*assign %s*/\n", left->name);
                if (!left->ptr)
                    left->ptr = (stack_ptr += 8);
                if (right->ptr)
                {
                    printf("mov rax, -%zu[rbp]\n", right->ptr);
                    printf("mov -%zu[rbp], rax\n", left->ptr);
                }
                else if (right->c)
                    printf("mov -%zu[rbp], r%cx\n", left->ptr, right->c);
                else
                    printf("mov -%zu[rbp], %d\n", left->ptr, right->value);
                break;
            }
            case add_:
            {
                Inst *left = regs[insts[i]->r2];
                Inst *right = regs[insts[i]->r3];

                inst->c = 'a';
                if (left->ptr)
                    printf("mov r%cx, -%zu[rbp]\n", inst->c, left->ptr);
                else if (left->c && left->c != inst->c)
                    printf("mov r%cx, r%cx\n", inst->c, left->c);
                else if (!left->c)
                    printf("mov r%cx, %d\n", inst->c, left->value);

                if (right->ptr)
                    printf("add r%cx, -%zu[rbp]\n", inst->c, right->ptr);
                else if (right->c) // TODO: to be checked
                    printf("add r%cx, r%cx\n", inst->c, right->c);
                else if (!right->c)
                    printf("add r%cx, %d\n", inst->c, right->value);
                break;
            }
            case int_:
            {
                break;
            }
            default:
                break;
            }
        }
    }
}

int main()
{
    char *input = open_file("file.w");
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
        generate_ir(curr->left);
        curr = curr->right;
    }

    copy_insts();
    print_ir();
    optimize_ir();

    copy_insts();
    print_ir();
    optimize_ir();

    copy_insts();
    print_ir();
    optimize_ir();

    copy_insts();
    print_ir();
    generate_asm();
    clear(head, input);
}