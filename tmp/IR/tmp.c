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
    assign_ = 11, add_assign_, sub_assign_, mul_assign_, div_assign_, 
    lpar_, rpar_, 
    mul_, add_, sub_, div_, 
    equal_, not_equal_, less_, more_, less_equal_, more_equal_,
    id_, int_, bool_, string_, void_,
    coma_,
    fcall_, fdec_, ret_,
    if_, elif_, else_, while_, dots_,

    cmp_, jne_, je_, jmp_, bloc_, mov_,
    end_,
} Type;

typedef struct
{
    char *value;
    Type type;
} Specials;

extern Specials *specials;

typedef struct
{
    Type type;
    // Type subtype;

    int reg;
    char c;
    size_t ptr;
    size_t index;
    int space;

    char *name;
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

// STACK POINTER
size_t ptr = 0;
size_t arg_ptr = 0;
// bool inside_function;

// DEBUG
Specials *specials = (Specials[])
{
    {"+=", add_assign_}, {"-=", sub_assign_}, {"*=", mul_assign_}, 
    {"/=", div_assign_}, {"!=", not_equal_},{"==", equal_},{"<=", less_equal_}, 
    {">=", more_equal_}, {"<", less_}, {">", more_}, {"=", assign_}, 
    {"+", add_}, {"-", sub_}, {"*", mul_}, {"/", div_}, {"(", lpar_}, 
    {")", rpar_}, {",", coma_}, {":", dots_}, {"if", if_}, {"elif", elif_},
    {"else", else_}, {"while", while_}, {"func", fdec_}, {"return", ret_},
    {0, (Type)0},
};

void ptoken(Token *token)
{
    printf("token ");
    switch (token->type)
    {
    case void_:
    case string_:
    case int_:
    case bool_:
    {
        Type type = token->type;
        printf("[%s]",(type == string_ ? "string" : type == int_    ? "int" :
                        type == bool_   ? "bool" : type == void_   ? "void" :
                        NULL));
        if (token->name)
        {
            printf(" name [%s]", token->name );
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
    // case arg_: return "ARG";
    // case dest_: return "DEST";

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
void clear(
    Node *head, char *input)
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

char sign(Token *token)
{
    // return (token->isarg ? ' ' : '-');
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
        default: break; \
    } \
} while (0)
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
    if(node->left)
        new->left = copy_node(node->left);
    if(node->right)
        new->right = copy_node(node->right);
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

Specials DataTypes[] = 
{
    {"int", int_}, {"bool", bool_}, {"string", string_}, {"void", void_}, {0, 0}
};

Node *prime()
{
    Node *node = NULL;
    Token *token;
    if ((token  = check(int_, bool_, string_, void_, id_, lpar_, rpar_, 0)))
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
                if(strcmp(node->token->name, "main") == 0)
                {
                    if(!check(rpar_, 0))
                    {
                        // TODO: error
                    }
                    if(!check(dots_, 0))
                    {
                        // TODO: error
                    }
                    node->token->type = fdec_;
                }
                else
                {
                    char *regs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
                    int i = 0;
                    size_t arg_ptr = 8;

                    node->token->type = fcall_;
                    Node *curr = node;
                    while (!check(rpar_, 0))
                    {
                        Node *arg = new_node(new_token(NULL, 0, 0, 0, arg_));
                        arg->left = expr();
                        arg->right = new_node(new_token(NULL, 0, 0, 0, dest_));
                        if(regs[i])
                            arg->right->token->arg_reg = strdup(regs[i++]);
                        else
                            arg->right->token->ptr = (arg_ptr += 8);
                        
                        curr->left = arg;
                        // curr->left->token->isarg = true;
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
    }
    else if((token = check(fdec_, 0)))
    {
        node = new_node(token);
        node->left = new_node(NULL);
        node->left->left = prime();
        if(!node->left->left->token || !node->left->left->token->declare)
        {
            printf("Error: expected datatype after func declaration\n");
            exit(1);
        }
        node->token->name = node->left->left->token->name;
        node->left->left->token->name = NULL;
        // TODO: those errors should be checked
        if(!check(lpar_, 0))
        {
            printf("Error: expected ( after function declaration\n");
            exit(1);
        }
        if(!check(rpar_, 0))
        {
            // TODO: working on passing arguemnts
            // inside_function = true;
            arg_ptr = 8;

            // Node *args[100]; // TODO: to be cheked
            // int arg_pos;

            Node *curr = node->left;
            while(!check_type((Type[]){rpar_, end_, 0}, tokens[exe_pos]->type))
            {
                curr->right = new_node(NULL);
                curr = curr->right;
                curr->left = expr();
                // args[arg_pos++] = prime();
                if(tokens[exe_pos]->type != rpar_ && !check(coma_, 0))
                {
                    printf("Error: expected ',' between arguments\n");
                    exit(1);
                }
            }
            // while(arg_pos > 0)
            // {
            //     curr->right = new_node(NULL);
            //     curr = curr->right;
            //     curr->left = args[--arg_pos];
            //     // curr->left->token->isarg = true;
            // }
            // inside_function = false;
        }
        if(!check(rpar_, 0))
        {
            printf("Error: expected )\n");
            exit(1);
        }
        if(!check(dots_, 0))
        {
            printf("Error: expected :\n");
            exit(1);
        }

        node->token->type = fdec_;
        Node *curr = node;
        while
        (
            tokens[exe_pos]->space > node->token->space &&
            tokens[exe_pos]->type != end_
        )
        {
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = expr();
        }
        curr->right = new_node(NULL);
        Token *ret_token = new_token(NULL, 0, 0, node->token->space + 1, ret_);
        curr->right->left = new_node(ret_token);
        return node;
    }
    // TODO: handle error parsing
    else if((token  = check(if_, 0)))
    {
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
            check_type((Type[]){elif_, else_, 0}, tokens[exe_pos]->type) &&
            tokens[exe_pos]->space == node->token->space
        )
        {
            token = tokens[exe_pos++];
            curr->right = new_node(NULL);
            curr = curr->right;
            curr->left = new_node(token);
            if(token->type == elif_)
            {
                Node *tmp0 = curr->left;
                tmp0->left = expr();
                if(!check(dots_, 0))
                {
                    printf("Error: expected dots");
                    exit(1);
                }
                tmp0->right = new_node(NULL);
                tmp0 = tmp0->right;
                while(tokens[exe_pos]->space > token->space)
                {
                    tmp0->left = expr();
                    tmp0->right = new_node(NULL);
                    tmp0 = tmp0->right;
                }
            }
            else if(token->type == else_)
            {
                if(!check(dots_, 0))
                {
                    printf("Error: expected dots");
                    exit(1);
                }
                Node *tmp0 = curr->left;

                tmp0->right = new_node(NULL);
                tmp0 = tmp0->right;
                while(tokens[exe_pos]->space > token->space)
                {
                    tmp0->left = expr();
                    tmp0->right = new_node(NULL);
                    tmp0 = tmp0->right;
                }
                break;
            }
        }

    }
    else if((token = check(while_, 0)))
    {
        node = new_node(token);
        node->left = expr();
        if(!check(dots_, 0))
        {
            printf("Error: expected :\n");
            exit(1);
        }
        Node *tmp = node;
        while
        (
            tokens[exe_pos]->type != end_  &&
            tokens[exe_pos]->space > token->space
        )
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
    }
    else if(tokens[exe_pos]->type == end_);
    else
    {
        printf("Prime: Unexpected token <%s>\n", to_string(tokens[exe_pos]->type));
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
            // if(token->isarg)
            //     token->ptr = (arg_ptr += 8);
            // else
                token->ptr = (ptr += 8);
            token->reg = ++reg_pos;
        }
    }
    else
    {
        switch (token->type)
        {
        case add_: case sub_: case mul_: case div_: case equal_:
        case less_: case more_: case less_equal_: case more_equal_:
        case not_equal_: case fcall_: 
            token->reg = ++reg_pos;
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
    // printf("gen-ir: %s\n", to_string(node->token->type));
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
        Node *tmp = node;
        Node *curr = node->left;

        // condition
        Token *result = generate_ir(curr->left); // TODO: check if it's boolean

        node->token->type = jne_;
        node->token->name = strdup("endif");
        node->token->index = ++bloc_index;

        Token *lastInst = copy_token(node->token);
        new_inst(lastInst); // jne to endif

        curr = curr->right;
        while(curr->left) // if code bloc
        {
            generate_ir(curr->left);
            curr = curr->right;
        }
        Inst *endInst = NULL;
        if(node->right)
        {
            endInst = new_inst(new_token("endif", 0, 5, node->token->space, jmp_));
            endInst->token->index = node->token->index;
        }

        curr = node->right;
        while(curr)
        {
            if(curr->left->token->type == elif_)
            {
                curr->left->token->index = ++bloc_index;
                curr->left->token->type = bloc_;
                curr->left->token->name = strdup("elif");

                {
                    free(lastInst->name);
                    lastInst->name = strdup("elif");
                    lastInst->index = curr->left->token->index;
                    lastInst = copy_token(lastInst);
                }

                new_inst(curr->left->token);
                Node *tmp = curr->left;
                generate_ir(tmp->left); // elif condition, TODO: check is boolean
                
                new_inst(lastInst);

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
                new_inst(curr->left->token);

                {  
                    free(lastInst->name);
                    lastInst->name = strdup("else");
                    lastInst->index = curr->left->token->index;
                    lastInst = copy_token(lastInst);
                }

                Node *tmp = curr->left;
                tmp = tmp->right;
                while(tmp->left)
                {
                    generate_ir(tmp->left);
                    tmp = tmp->right;
                }
                break;
            }
            if(curr->right)
            {
                endInst = new_inst(new_token("endif", 0, 5, node->token->space, jmp_));
                endInst->token->index = node->token->index;
            }
            curr = curr->right;
        }

        Token *new = new_token("endif", 0, 5, node->token->space, bloc_);

        new->index = node->token->index;
        new_inst(new);
        // free_token(lastInst);
        return node->left->token;
        break;
    }
    case while_:
    {
        // condition

        node->token->type = bloc_;
        node->token->name = strdup("while");
        node->token->index = ++bloc_index;
        inst = new_inst(node->token);

        Token *result = generate_ir(node->left); // TODO: check if it's boolean

        Token *end = copy_token(node->token);
        end->type = jne_;
        if(end->name) free(end->name);
        end->name = strdup("endwhile");
        new_inst(end);

        Node *curr = node->right;
        while(curr) // if code bloc
        {
            generate_ir(curr->left);
            curr = curr->right;
        }

        Token *lastInst = copy_token(node->token);
        lastInst->type = jmp_;
        free(lastInst->name);
        lastInst->name = strdup("while");
        new_inst(lastInst); // jne to endif

        lastInst = copy_token(node->token);
        lastInst->type = bloc_;
        free(lastInst->name);
        lastInst->name = strdup("endwhile");
        new_inst(lastInst); // jne to endif
        break;
    }
    case arg_:
    {
        Token *left = generate_ir(node->left);
        // Token *right = generate_ir(node->right);
        printf("has left: ");
        ptoken(left);
        if(node->right->token->arg_reg)
            printf("mov to %s\n", node->right->token->arg_reg);
        else
            printf("mov to %zu[rbp]\n", node->right->token->ptr);
        Inst *inst = new_inst(node->token);
        inst->left = left;
        inst->right = node->right->token;
        // printf("has right: ");
        // ptoken(right);
        // exit(1);
        return left;
        break;
    }
    case fdec_:
    {
        Token *fcall = copy_token(node->token);
        fcall->type = fdec_;
        inst = new_inst(fcall);
        Node *curr;
        
        pnode(node, NULL, 0);

        // inside_function = true;
        if(node->left) // arguments
        {
            curr = node->left->right;
            while(curr && curr->left)
            {
                Token *arg = generate_ir(curr->left);
                curr = curr->right;
            }
            // inside_function = false;
        }

        curr = node->right;
        while(curr)
        {
            generate_ir(curr->left);
            curr = curr->right;
        }
        // new_inst(new_token(NULL, 0, 0, node->token->space, ret_)); // TODO: to be checked
        break;
    }
    case ret_:
    {
        inst = new_inst(node->token);
        break;
    }
    case fcall_:
    {
        // pnode(node, NULL, 0);
        size_t tmp_arg_ptr = arg_ptr;
        size_t tmp_ptr = ptr;
        arg_ptr = 8;
        ptr = 8;
        if(strcmp(node->token->name, "output") == 0)
        {
            Node *curr = node;
            char *fname = NULL;
            while(curr->left)
            {
#if 0
                pnode(curr->left, NULL, 0);
#else
                Token *left = generate_ir(curr->left);
                fname = NULL;
                switch(left->type)
                {
                    case string_: fname = "_putstr"; break;
                    case int_:    fname = "_putnbr"; break;
                    default: break;
                }
                if(fname)
                {
                    inst = new_inst(new_token(fname, 0, strlen(fname), node->token->space, fcall_));
                    inst->token->isbuiltin = true;
                    inst->left = left;
                }
#endif
                curr = curr->right;
            }
        }
        else {
            Node *curr = node;
            while(curr->left)
            {
                generate_ir(curr->left);
                curr = curr->right;
            }
            new_inst(node->token);
        }
        arg_ptr = tmp_arg_ptr;
        ptr = tmp_ptr;
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
    default: break;
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
            printf("rxx: call %s\n", curr->name);
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
                printf("r%.2d: declare %s", curr->reg, curr->name);
            else if(curr->type == int_)
                printf("r%.2d: value %lld", curr->reg, curr->Int.value);
            else if(curr->type == bool_)
                printf("r%.2d: value %s", curr->reg, curr->Bool.value ? "True" : "False");
            else if(curr->type == string_)
                printf("r%.2d: value %s in STR%zu", curr->reg, curr->String.value, 
                                                     (curr->index = ++str_index));
            // if(curr->isarg)
            //     printf(" [argument]");
            printf("\n");
            break;
        }
        // case arg_:
        // {

        //     break;
        // }
        case jne_: printf("rxx: jne %s%zu\n", curr->name, curr->index); break;
        case jmp_: printf("rxx: jmp %s%zu\n", curr->name, curr->index); break;
        case bloc_: printf("rxx: %s%zu (bloc)\n", curr->name, curr->index); break;
        case fdec_: printf("rxx: %s (func dec)\n", curr->name); break;
        case ret_: printf("rxx: return\n"); break;
        default: 
            printf("%sPrint IR: Unkown inst [%s]%s\n", RED, to_string(curr->type), RESET);
            break;
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
                    default: break;
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
                    if(!insts[j]->left || !insts[j]->right || !insts[i]->token)
                    {
                        j++;
                        continue;
                    }
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
                mov("rax, QWORD PTR %c%zu[rbp]\n", sign(right), right->ptr);
                mov("QWORD PTR %c%zu[rbp], rax\n", sign(left), left->ptr);
            }
            else if(right->c)
                mov("QWORD PTR %c%zu[rbp], r%cx\n", sign(left), left->ptr, right->c);
            else
            {
                switch (right->type)
                {
                case int_:
                    mov("QWORD PTR %c%zu[rbp], %lld\n", sign(left), left->ptr, right->Int.value);
                    break;
                case bool_:
                    mov("QWORD PTR %c%zu[rbp], %d\n", sign(left), left->ptr, right->Bool.value);
                    break;
                case string_:
                    lea("rdi, .STR%zu[rip]\n", right->index);
                    call("_strdup");
                    mov("QWORD PTR %c%zu[rbp], rax\n", sign(left), left->ptr);
                    break;
                default:
                    break;
                }
            }
            break;
        }
        // case push_:
        // {
        //     break;
        // }
        case int_: case bool_: case string_:
        {
            if (curr->declare)
            {
                // if(curr->isarg)
                //     pasm("/*arg %s in %zu[rbp] */\n", curr->name, curr->ptr + 8);
                // else
                {
                    pasm("/*declare %s*/\n", curr->name);
                    mov("QWORD PTR %c%zu[rbp], 0\n", sign(curr), curr->ptr);
                }
            }
            else
            {
                // if(curr->isarg) // TODO: to be checked
                //     push("%ld\n", curr->Int.value);
                // if(curr->type == string_ && !curr->name)
                    ; // pasm(".STR%zu: .string %s\n", curr->index, curr->String.value);
            } 
            break;
        }
        case add_: case sub_: case mul_: case div_: // TODO: check all math_op operations
        {
            curr->c = 'a';
            if (left->ptr)
                mov("r%cx, QWORD PTR %c%zu[rbp]\n", curr->c, sign(left), left->ptr);
            else if (left->c && left->c != curr->c)
                mov("r%cx, r%cx\n", curr->c, left->c);
            else if (!left->c)
                mov("r%cx, %lld\n", curr->c, left->Int.value);

            if (right->ptr)
                math_op(curr->type, "r%cx, QWORD PTR %c%zu[rbp]\n", curr->c, sign(right), right->ptr);
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
                mov("r%cx, QWORD PTR %c%zu[rbp]\n", curr->c, sign(left), left->ptr);
            else if (left->c && left->c != curr->c)
                mov("r%cx, r%cx\n", curr->c, left->c);
            else if (!left->c)
                mov("r%cx, %lld\n", curr->c, left->Int.value);

            if (right->ptr)
                mov("rbx, QWORD PTR %c%zu[rbp]\n", sign(right), right->ptr);
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
                    mov("rdi, QWORD PTR %c%zu[rbp]\n", sign(left), left->ptr);
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
        case fdec_:
        {
            pasm("%s:\n", curr->name);
            // if(strcmp(curr->name, "main") == 0)
            {
                push("rbp\n", "");
                mov("rbp, rsp\n","");
                pasm("sub     rsp, %zu\n", ptr + 8);
            }
            break;
        }
        case jne_:
        {
            cmp("al, 1\n", "");
            jne(".%s%zu\n", curr->name, curr->index);
            break;
        }
        case jmp_:
        {
            jmp(".%s%zu\n", curr->name, curr->index);
            break;
        }
        case bloc_:
        {
            pasm(".%s%zu:\n", curr->name, curr->index);
            break;
        }
        case ret_:
        {
            pasm("leave\n");
            pasm("ret\n");
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

#if OPTIMIZE
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