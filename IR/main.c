#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

#define TOKENIZE 1

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#endif

#if IR
#define ASM 0
#define OPTIMIZE 0
#endif

typedef enum
{
    assign_ = 11, add_assign_, sub_assign_, mul_assign_, div_assign_, lpar_, rpar_, 
    mul_, add_, sub_, div_, equal_, not_equal_, less_, more_, less_equal_, more_equal_,
    id_, int_, bool_, string_, void_, coma_, fcall_, fdec_, ret_,
    if_, elif_, else_, while_, dots_, cmp_, jne_, je_, jmp_, bloc_, arg_, dest_,
    end_,
} Type;

typedef struct
{
    char *value;
    Type type;
} Specials;

typedef struct
{
    Type type;

    char *name;

    int reg;
    char c;
    size_t ptr;
    size_t index;
    int space;

    bool declare;
    bool remove;
    bool isbuiltin;
    bool isarg;

    char *arg_reg;

    struct
    {
        // integer
        struct
        {
            long long value;
            int power;
            struct Int *next;
        } Int;
        // boolean
        struct
        {
            bool value;
            char c;
        } Bool;
        // string
        struct
        {
            char *value;
        } String;
    };
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

typedef struct
{
    Token *token;
    Token *left;
    Token *right;
} Inst;

// TOKENIZE GLOBALS
Token **tokens;
int tk_size;
int tk_pos;
int exe_pos;

// INSTRUCTIONS
Inst **first_insts;
Inst **insts;
int inst_size;
int inst_pos;

// STACK POINTER
size_t ptr = 0;

// DEBUG
Specials *specials = (Specials[])
{
    {"+=", add_assign_}, {"-=", sub_assign_}, {"*=", mul_assign_},
    {"/=", div_assign_}, {"!=", not_equal_},{"==", equal_}, {"<=", less_equal_}, 
    {">=", more_equal_}, {"<", less_}, {">", more_}, {"=", assign_}, {"+", add_},
    {"-", sub_}, {"*", mul_}, {"/", div_}, {"(", lpar_}, {")", rpar_}, {",", coma_},
    {":", dots_}, {"if", if_}, {"elif", elif_}, {"else", else_}, {"while", while_}, 
    {"func", fdec_}, {"return", ret_}, {0, (Type)0},
};

void ptoken(Token *token)
{
    printf("token ");
    switch (token->type)
    {
    case void_: case string_: case int_: case bool_:
    {
        Type type = token->type;
        printf("[%s]", (type == string_ ? "string" : type == int_    ? "int" :
                        type == bool_   ? "bool"   : type == void_   ? "void" :
                        NULL));
        if (token->name)
        {
            printf(" name [%s]", token->name);
            if (token->declare) printf(" [declare]");
        }
        else if(token->type == int_)
            printf(" value [%lld]", token->Int.value);
        else if(token->type == string_)
            printf(" value [%s]", token->String.value);
        else if(token->type == bool_)
            printf(" value [%d]", token->Bool.value);
        break;
    }
    case fcall_: printf("[func call] name [%s]", token->name); break;
    case fdec_: printf("[func dec] name [%s]", token->name); break;
    case id_: printf("[id] name [%s]", token->name); break;
    case end_: printf("[end]"); break;
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
    case bool_: return "BOOL";
    case void_: return "VOID";
    case id_: return "ID";
    case coma_: return "COMA";

    case assign_: return "ASSIGN";
    case add_assign_: return "ADD ASSIGN";
    case sub_assign_: return "SUB ASSIGN";
    case mul_assign_: return "MUL ASSIGN";
    case div_assign_: return "DIV ASSIGN";

    case lpar_: return "LPARENT";
    case rpar_: return "RPARENT";
    case fcall_: return "FUNC CALL";
    case fdec_: return "FUNC DECL";

    case jne_: return "JNE";
    case je_: return "JE";
    case jmp_: return "JMP";
    case cmp_: return "CMP";
    // case push_: return "PUSH";
    case arg_: return "ARG";
    case dest_: return "DEST";

    case while_: return "WHILE";
    case if_: return "IF";
    case elif_: return "ELIF";
    case else_: return "ELSE";
    case dots_: return "DOTS";
    case bloc_: return "BLOC";
    case ret_: return "RETURN";
    case end_: return "END";
    }
    return NULL;
}

void free_token(Token *token)
{
    if(token->name)
        free(token->name);
    if(token->String.value)
        free(token->String.value);
    if(token->arg_reg)
        free(token->arg_reg);
    free(token);    
}

#if AST
void clear(Node *head, char *input)
#else
void clear(char *input)
#endif
{
#if TOKENIZE
    for (int i = 0; i < tk_pos; i++)
        free_token(tokens[i]);
#endif

#if AST
    free_node(head);
#endif

#if IR
    for (int i = 0; i < inst_pos; i++)
        free(first_insts[i]);
    free(first_insts);
#endif
    free(input);
}

bool check_type(Type *types, Type type)
{
    for (int i = 0; types[i]; i++)
        if (types[i] == type) return true;
    return false;
}

void copy_insts()
{
    if (insts)
        free(insts);
    // TODO: protect it if no instruction created
    insts = calloc(inst_size + 1, sizeof(Inst *));
    int j = 0;
    for (int i = 0; i < inst_pos; i++)
    {
        if (!first_insts[i]->token->remove)
            insts[j++] = first_insts[i];
    }
}

char sign(Token *token)
{
    // return (token->isarg ? ' ' : '-');
    // return (token->isarg ? 1 : -1) * token->ptr 
    return ' ';
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
#define jmp(fmt, ...)  pasm("jmp     " fmt, __VA_ARGS__)
#define push(fmt, ...) pasm("push    " fmt, __VA_ARGS__)
#define call(func)     pasm("call    %s\n", func)

#define math_op(op, fmt, ...) \
do { switch(op) { \
        case add_: pasm("add     " fmt, __VA_ARGS__); break; \
        case sub_: pasm("sub     " fmt, __VA_ARGS__); break; \
        case mul_: pasm("mul     " fmt, __VA_ARGS__); break; \
        case div_: pasm("div     " fmt, __VA_ARGS__); break; \
        default: break; \
    } } while (0)

#define relational_op(op, fmt, ...) \
do { switch(op) { \
        case equal_:      pasm("sete    " fmt, __VA_ARGS__); break; \
        case not_equal_:  pasm("setne   " fmt, __VA_ARGS__); break; \
        case less_:       pasm("setl    " fmt, __VA_ARGS__); break; \
        case less_equal_: pasm("setle   " fmt, __VA_ARGS__); break; \
        case more_:       pasm("setg    " fmt, __VA_ARGS__); break; \
        case more_equal_: pasm("setge   " fmt, __VA_ARGS__); break; \
        default: break; \
    } } while (0)

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
    case fcall_: case id_: case jmp_: case bloc_:
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
    if(token == NULL) return NULL;
    Token *new = calloc(1, sizeof(Token));
    memcpy(new, token, sizeof(Token));
    if(token->name) // TODO: check all data that can be copied
        new->name = strdup(token->name);
    if(token->String.value)
        new->String.value = strdup(token->String.value);
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
            if(new_line && input[i] == ' ') space++;
            else if(!new_line && input[i] == ' ') space = 0;
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

