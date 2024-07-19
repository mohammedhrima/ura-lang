#include "header.h"

// TOKENIZE GLOBALS
Token **tokens;
int tk_size;
int tk_pos;
int exe_pos;

// VARIABLES GLOBALS
Token **vars;
int var_size;
int var_pos;

// INSTRUCTIONS
Inst **first_insts;
Inst **insts;
int inst_size;
int inst_pos;

// SIMULATION
Inst **regs;
int reg_pos;
int reg_size;

// DEBUG
Specials *specials = (Specials[])
{
    {"+=", add_assign_}, {"-=", sub_assign_}, {"*=", mul_assign_}, {"/=", div_assign_},
    {"!=", not_equal_},{"==", equal_},{"<=", less_equal_}, {">=", more_equal_}, 
    {"<", less_}, {">", more_}, {"=", assign_}, {"+", add_}, {"-", sub_}, {"*", mul_}, 
    {"/", div_}, {"(", lpar_}, {")", rpar_}, {",", coma_}, {":", dots_}, {"if", if_}, 
    {0, (Type)0}
};

void ptoken(Token *token)
{
    printf("token ");
    switch (token->type)
    {
    case string_:
    case int_:
    case bool_:
    {
        if (token->name)
        {
            printf(
                "[%s] name [%s]",(token->type == string_ ? "string" :
                                  token->type == int_    ? "int" :
                                  token->type == bool_   ? "bool" :
                                  NULL),
                token->name
             );
            if (token->declare)
                printf(" [declare]");
        }
        else if(token->type == int_)
            printf("[int] value [%lld]", token->Int.value);
        else if(token->type == string_)
            printf("[string] value [%s]", token->String.value);
        else if(token->type == bool_)
            printf("[bool] value [%d]", token->Bool.value);
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
    case end_:
        printf("[end]");
        break;
    default:
    {
        for (int i = 0; specials[i].value; i++)
        {
            if (specials[i].type == token->type)
            {
                printf("[%s]", specials[i].value);
                break;
            }
        }
        break;
    }
    }
    printf(" space [%d]\n", token->space);
}

void pnode(Node *node, char *side, int space)
{
    if (node)
    {
        int i = 0;
        while (i < space)
            i += printf(" ");
        if (side)
            printf("%s: ", side);
        if (node->token)
        {
            printf("node: ");
            ptoken(node->token);
        }
        else
            printf("\n");
        pnode(node->left, "LEFT ", space + 2);
        pnode(node->right, "RIGHT", space + 2);
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
    case add_: return "ADD   ";
    case sub_: return "SUB   ";
    case mul_: return "MUL   ";
    case div_: return "DIV   ";

    case equal_: return "EQUAL";
    case not_equal_: return "NOT EQUAL";
    case less_: return "LESS THAN";
    case more_: return "MORE THAN";
    case more_equal_: return "MORE THAN OR EQUAL";
    case less_equal_: return "LESS THAN OR EQUAL";
    
    case int_: return "INT   ";
    case string_: return "STRING ";

    case assign_: return "ASSIGN";
    case lpar_: return "LPARENT";
    case rpar_: return "RPARENT";
    case fcall_: return "FUNC CALL";
    case if_: return "IF";
    case end_if_: return "END IF";
    case bloc_: return "BLOC";
    case end_: return "END";
    default:
        break;
    }
    return NULL;
}

#if AST
void clear(
    Node *head, char *input)
#else
void clear(char *input)
#endif
{
#if TOKENIZE
    for (int i = 0; i < tk_pos; i++)
    {
        if (tokens[i]->name)
            free(tokens[i]->name);
        else if (tokens[i]->String.value)
            free(tokens[i]->String.value);
        free(tokens[i]);
    }
#endif

#if AST
    free_node(head);
#endif

#if IR
    for (int i = 0; i < inst_pos; i++)
        free(first_insts[i]);
    free(first_insts);
    free(regs);
#endif
    free(input);
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

void copy_insts()
{
    if (insts)
        free(insts);
    insts = calloc(inst_size, sizeof(Inst *)); // TODO: protect it if no instruction created
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (!first_insts[i]->token->remove)
            insts[j++] = first_insts[i];
    }
}

FILE *asm_fd;
void pasm(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    bool cond;
    cond = !strchr(fmt, ':') && !strstr(fmt, ".section	.note.GNU-stack,\"\",@progbits");
    cond = cond && !strstr(fmt, ".intel_syntax noprefix") && !strstr(fmt, ".include");
    cond = cond && !strstr(fmt, ".text") && !strstr(fmt, ".globl	main");
    if (cond)
        fprintf(asm_fd, "   ");
    vfprintf(asm_fd, fmt, ap);
}

#define mov(fmt, ...)  pasm("mov     " fmt, __VA_ARGS__)
#define lea(fmt, ...)  pasm("lea     " fmt, __VA_ARGS__)
#define cmp(fmt, ...)  pasm("cmp     " fmt, __VA_ARGS__)
#define jne(fmt, ...)  pasm("jne     " fmt, __VA_ARGS__)
#define call(func) pasm("call    %s", func)

// Define the math macro
#define math_op(op, fmt, ...) \
do { \
    switch(op) { \
        case add_: pasm("add     " fmt, __VA_ARGS__); break; \
        case sub_: pasm("sub     " fmt, __VA_ARGS__); break; \
        case mul_: pasm("mul     " fmt, __VA_ARGS__); break; \
        case div_: pasm("div     " fmt, __VA_ARGS__); break; \
        default: break; \
    } \
} while (0)

#define relational_op(op, fmt, ...) \
do { \
    switch(op) { \
        case equal_:      pasm("sete    " fmt, __VA_ARGS__); break; \
        case not_equal_:  pasm("setne   " fmt, __VA_ARGS__); break; \
        case less_:       pasm("setl    " fmt, __VA_ARGS__); break; \
        case less_equal_: pasm("setle   " fmt, __VA_ARGS__); break; \
        case more_:       pasm("setg    " fmt, __VA_ARGS__); break; \
        case more_equal_: pasm("setge   " fmt, __VA_ARGS__); break; \
        default: \
            break; \
    } \
} while (0)