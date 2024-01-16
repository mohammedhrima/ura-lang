// c headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdarg.h>

#ifndef DEBUG
#define DEBUG 1
#endif
#define EXIT_STATUS 0

#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

// typedefs
typedef struct Token Token;
typedef struct Node Node;
typedef struct Label Label;
typedef enum Type Type;

// stupid implicit declaration error
Token *get_var(char *name);
Token *evaluate(Node *node);
void print_node(Node *node, int level);

/*
    TODOS:
        - expect expression after operator
        - handle not
        - assignment operators
        - expect new line after dots_
*/

// STRUCTURES / ENUMS
enum Type
{
    eof_,
    none_,
    // math operators
    add_,
    sub_,
    mul_,
    div_,
    mod_,
    // logic operators
    and_,
    or_,
    // comparision operators
    not_,
    not_equal_,
    equal_,
    less_than_,
    grea_than_,
    less_than_equal_,
    grea_than_equal_,
    // parents
    lparent_,
    rparent_,
    // assignment
    assign_,
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
#if 0
    // memory
    fix_,
    dyn_,
#endif
    // Data types
    void_,
    char_,
    int_,
    float_,
    bool_,
    identifier_,
    neg_,
    // functions
    main_,
    func_dec_,
    func_call_,
    coma_,
    // statements / loops
    if_,
    elif_,
    else_,
    while_,
    dots_,
};

struct Token
{
    char *name;
    Type type;
    Type sub_type;
    uintptr_t ptr;
    size_t level;
    union
    {
        // INTEGER
        long long int_;
        // FLOAT
        uint32_t float_;
        // CHARACTER
        struct
        {
            char *char_;
            size_t index_;
        };
        // BOOLEAN
        struct
        {
            bool bool_;
            char c;
        };
    };
};

struct Label
{
    // NAME
    char *name;
    // NODE
    Node *node;
    // VARIABLES
    Token **VARIABLES;
    int var_len;
    int var_pos;

    // FUNCTIONS
    Node **FUNCTIONS;
    int func_len;
    int func_pos;
};

struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

// GLOBALS
struct
{
    Type type;
    char *name;
} DataTypes[] = {
    {void_, "void"},
    // char
    {char_, "char"},
    {char_, "fix:char"},
    {char_, "dyn:char"},
    // int
    {int_, "int"},
    // float
    {float_, "float"},
    // bool
    {bool_, "bool"},
    {0, 0},
};

struct
{
    Type type;
    char *name;
} Symbols[] = {
    // comparision operators
    {less_than_equal_, "<="},
    {grea_than_equal_, ">="},
    {not_equal_, "!="},
    {not_, "!"},
    {equal_, "=="},
    {less_than_, "<"},
    {grea_than_, ">"},
    // assign
    {assign_, "="},
#if 1
    {add_assign_, "+="},
    {sub_assign_, "-="},
    {mul_assign_, "*="},
    {div_assign_, "/="},
#endif
    // math operators
    {add_, "+"},
    {sub_, "-"},
    {mul_, "*"},
    {div_, "/"},
    {mod_, "%"},
    // parents, coma
    {lparent_, "("},
    {rparent_, ")"},
    {coma_, ","},
    // logic
    {and_, "&&"},
    {or_, "||"},
    {dots_, ":"},
    {0, 0},
};

struct
{
    Type type;
    char *name;
} Specials[] = {
    {and_, "and"},
    {or_, "or"},
    {if_, "if"},
    {else_, "else"},
    {elif_, "elif"},
    {while_, "while"},
    {func_dec_, "func"},
    {none_, "none"},
    {0, 0},
    {identifier_, "identifier"},
    {func_call_, "fcall"},
    {neg_, "negative"},
    {eof_, "EOF"},
};

// FILES
FILE *file;
FILE *asm_fd;
char *text;

// TOKENS
Token **TOKENS;
int tk_len;
int tk_pos;
int exe_pos;

// GLOBALS
size_t Label_index;
size_t index_;
uintptr_t ptr;
size_t rsp;

// LABELS
Label **LABELS;
Label *LABEL;
int lb_len;
int lb_pos;

// DEBUGING
void error(char *fmt, ...)
{
    // TODO: update error messages
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s", RED);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "%s\n", RESET);
    exit(1);
};

char *type_to_string(Type type)
{
    for (int i = 0; i < sizeof(DataTypes) / sizeof(*DataTypes); i++)
        if (DataTypes[i].type == type)
            return DataTypes[i].name;
    for (int i = 0; i < sizeof(Symbols) / sizeof(*Symbols); i++)
        if (Symbols[i].type == type)
            return Symbols[i].name;
    for (int i = 0; i < sizeof(Specials) / sizeof(*Specials); i++)
        if (Specials[i].type == type)
            return Specials[i].name;
    error("error unkown type (%d | %c)\n", type, type);
    return NULL;
}

void debug(char *conv, ...)
{
#if DEBUG
    size_t len = strlen(conv);
    size_t i = 0;

    va_list args;
    va_start(args, conv);
    while (i < len)
    {
        if (conv[i] == '%')
        {
            i++;
            if (strncmp(conv + i, "zu", 2) == 0)
            {
                fprintf(stdout, "%zu", va_arg(args, size_t));
                i++;
            }
            else
            {
                switch (conv[i])
                {
                case 'c':
                    fprintf(stdout, "%c", va_arg(args, int));
                    break;
                case 's':
                    fprintf(stdout, "%s", va_arg(args, char *));
                    break;
                case 'p':
                    fprintf(stdout, "%p", (size_t)(va_arg(args, void *)));
                    break;
                case 'x':
                    fprintf(stdout, "%x", (size_t)va_arg(args, void *));
                    break;
                case 'X':
                    fprintf(stdout, "%X", (size_t)va_arg(args, void *));
                    break;
                case 'd':
                    fprintf(stdout, "%d", (int)va_arg(args, int));
                    break;
                case 'f':
                    fprintf(stdout, "%f", va_arg(args, double));
                    break;
                case '%':
                    fprintf(stdout, "%%");
                    break;
                case 't':
                    fprintf(stdout, "%s", type_to_string((Type)va_arg(args, Type)));
                    break;
                case 'k':
                {
                    Token *token = (Token *)va_arg(args, Token *);
                    if (token)
                    {
                        fprintf(stdout, "%s ", type_to_string(token->type));
#if 0
                    if (token->sub_type)
                        fprintf(stdout, "subtype: %s, ", type_to_string(token->sub_type));
#endif
                        if (token->name)
                            fprintf(stdout, "<name:%5s> ", token->name);
#if 1
                        else
#endif
                            switch (token->type)
                            {
                            case char_:
                                fprintf(stdout, "'%s' ", token->char_);
                                if (token->index_)
                                    fprintf(stdout, "STR%zu, ", token->index_);
                                break;
                            case int_:
                                fprintf(stdout, "%d ", token->int_);
                                break;
                            case float_:
                                fprintf(stdout, "%zu (%.2f) ", token->float_,
                                        *(float *)(&token->float_));
                                if (token->index_)
                                    fprintf(stdout, "FLT%zu ", token->index_);
                                break;
                            case bool_:
                                fprintf(stdout, "%s ", token->bool_ ? "True" : "False");
                                break;
                            default:
                                break;
                            }
                        fprintf(stdout, "in LVL%zu", token->level);
                    }
                    else
                        fprintf(stdout, "(null)");
                    break;
                }
                default:
                    error("in debug function");
                    break;
                }
            }
        }
        else
            fprintf(stdout, "%c", conv[i]);
        i++;
    }
#endif
}

void visualize()
{
#if 0
    for (int i = 0; i < lb_pos; i++)
    {
        Label *curr = LABELS[i];
        debug("%sLabel: %s\n", GREEN, curr->name);
        debug("     variables:\n");
        for (int i = 0; i < curr->var_pos; i++)
            debug("         %s\n", curr->VARIABLES[i]->name);
        debug("     functions:\n");
        for (int i = 0; i < curr->func_pos; i++)
            debug("         %s\n", curr->FUNCTIONS[i]->token->name);
    }
    debug("%s\n", RESET);
#endif
}

// UTILS
void print_asm(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(asm_fd, fmt, ap);
}

char *strjoin(char *string1, char *string2, char *string3, char *string4)
{
    size_t len = 0;
    len = string1 ? len + strlen(string1) : len;
    len = string2 ? len + strlen(string2) : len;
    len = string3 ? len + strlen(string3) : len;
    len = string4 ? len + strlen(string4) : len;

    char *res = calloc(len + 1, sizeof(char));
    string1 &&strcpy(res, string1);
    string2 &&strcpy(res + strlen(res), string2);
    string3 &&strcpy(res + strlen(res), string3);
    string4 &&strcpy(res + strlen(res), string4);
    return res;
}

// BUILTINS
void output(Token *token)
{
    debug("output %k\n", token);
    switch (token->type)
    {
    case char_:
    {
        print_asm("   /* call _putstr */\n");
        if (token->ptr)
        {
            print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   mov     rdi, rax\n", token->ptr);
            print_asm("   call    _putstr\n");
        }
        else if (token->index_)
        {
            print_asm("   lea     rax, STR%zu[rip]\n", token->index_);
            print_asm("   mov     rdi, rax\n", token->ptr);
            print_asm("   call    _putstr\n");
        }
        else
            error("output char");
        break;
    }
    case int_:
    {
        print_asm("   /* call _putnbr */\n");
        if (token->ptr)
        {
            print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   mov     rdi, rax\n");
            print_asm("   call    _putnbr\n");
        }
        else if (token->index_)
        {
            // token->ptr = (ptr += 8);
            // print_asm("   mov   QWORD PTR -%zu[rbp], %lld\n", token->ptr, token->int_);
            print_asm("   mov   rax, %lld\n", token->ptr, token->int_);
            print_asm("   mov   rdi, rax\n");
            print_asm("   call  _putnbr\n");
        }
        else
            error("output int");
        break;
    }
    case bool_:
    {
        print_asm("   /* call _putbool */\n");
        if (token->ptr)
            print_asm("   movzx   eax, BYTE PTR -%zu[rbp]\n", token->ptr);
        else if (token->c)
            print_asm("   movzx   eax, %cl\n", token->c);
        else
            error("output bool");
        print_asm("   mov	   edi, eax\n");
        print_asm("   call	   _putbool\n");
        break;
    }
    case float_:
        // TODO: handle float
        error("Error in output float not handled yet");
        break;
    case identifier_:
        break;
    default:
        error("Error in output unknown type");
        break;
    }
}

void enter_label(Node *node)
{
    if (lb_len == 0)
    {
        lb_len = 20;
        LABELS = calloc(lb_len, sizeof(Label *));
    }
    else if (lb_pos + 2 > lb_len)
    {
        lb_len *= 2;
        LABELS = realloc(LABELS, lb_len * sizeof(Label *));
    }
    char *name = strdup(node->token->name);
    int curr = lb_pos;
    while (curr > 0)
    {
        char *tmp = strjoin(LABELS[curr]->name, name, NULL, NULL);
        free(name);
        name = tmp;
        curr--;
    }
    Label *new = calloc(1, sizeof(Label));
    new->name = name;
    new->node = node;
    new->var_len = 100;
    new->func_len = 100;
    new->VARIABLES = calloc(new->var_len, sizeof(Token *));
    new->FUNCTIONS = calloc(new->func_len, sizeof(Node *));
    debug("         Enter Label: %s \n", name);
    lb_pos++;
    LABELS[lb_pos] = new;
    LABEL = LABELS[lb_pos];
}

void exit_label(Node *node)
{
    debug("         Exit Label: %s \n", LABEL->name);
    if (LABEL->node != node)
        error("in label exit\n");
    free(LABEL->VARIABLES);
    free(LABEL->FUNCTIONS);
    free(LABEL->name);
    free(LABEL);
    LABELS[lb_pos] = NULL;
    lb_pos--;
    LABEL = LABELS[lb_pos];
    debug("         Curr Label: %s \n", LABEL->name);
}

// HANDLE VARIABLES
Token *new_variable(Token *token)
{
    switch (token->type)
    {
    case char_:
        token->ptr = (ptr += 8);
        break;
    case int_:
        token->ptr = (ptr += 8);
        print_asm("   mov     QWORD PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
        break;
    case float_:
        token->ptr = (ptr += 4);
        print_asm("   mov     DWORD PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
        break;
    case bool_:
        token->ptr = (ptr += 1);
        print_asm("   mov     BYTE PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
    default:
        break;
    }
#if 1
    if (LABEL->var_pos + 2 > LABEL->var_len)
    {
#if 1
        LABEL->var_len *= 2;
        LABEL->VARIABLES = realloc(LABEL->VARIABLES, LABEL->var_len * sizeof(Token *));
#else
        Token **tmp = calloc(LABEL->var_len * 2, sizeof(Token *));
        memcpy(tmp, LABEL->VARIABLES, LABEL->var_pos * sizeof(Token *));
        free(LABEL->VARIABLES);
        LABEL->VARIABLES = tmp;
        LABEL->var_len *= 2;
#endif
    }
    debug("new variable %k in ptr: %zu\n", token, token->ptr);
    return (LABEL->VARIABLES[LABEL->var_pos++] = token);
#endif
    // return token;
}

Token *get_var(char *name)
{
#if 1
    debug("label pos: %d\n", lb_pos);
    debug("get_var %s from %s label\n", name, LABEL->name);
    for (int j = lb_pos; j > 0; j--)
    {
        Label *curr = LABELS[j];
        for (int i = 0; i < curr->var_pos; i++)
        {
            Token *var = curr->VARIABLES[i];
            if (var->name && strcmp(var->name, name) == 0)
                return var;
        }
    }
#endif
    return NULL;
}

// HANDLE FUNCTIONS
Node *new_func(Node *node)
{
    // error("New func must be reviewed\n");

#if 1
    // Label *CURR = LABELS[lb_pos];
    if (LABEL->func_pos + 2 > LABEL->func_len)
    {
#if 1
        LABEL->func_len *= 2;
        LABEL->FUNCTIONS = realloc(LABEL->FUNCTIONS, LABEL->func_len * sizeof(Node *));
#else
        Node **tmp = calloc(LABEL->func_len * 2, sizeof(Node *));
        memcpy(tmp, LABEL->FUNCTIONS, LABEL->func_pos * sizeof(Node *));
        free(LABEL->FUNCTIONS);
        LABEL->FUNCTIONS = tmp;
        LABEL->func_len *= 2;
#endif
    }
    debug("new function, name: %s, return type: %t, in Label %s\n",
          node->token->name, node->token->type, LABEL->name[0] ? LABEL->name : "global");
    return (LABEL->FUNCTIONS[LABEL->func_pos++] = node);
#endif
    return node;
}

Node *get_func(char *name)
{
#if 1
    debug("label pos: %d\n", lb_pos);
    debug("get_func %s from %s label\n", name, LABEL->name[0] ? LABEL->name : "global");
    for (int j = lb_pos; j > 0; j--)
    {
        Label *curr = LABELS[j];
        debug("loop: get_func from label %s, has %d functions\n",
              curr->name[0] ? curr->name : "global", curr->func_pos);
        for (int i = 0; i < curr->func_pos; i++)
        {
            Node *func = curr->FUNCTIONS[i];
            if (func->token->name && strcmp(func->token->name, name) == 0)
                return func;
        }
    }
#endif
    return NULL;
}

// HANDLE TOKENS
Token *new_token(int s, int e, Type type, Type sub_type, size_t level)
{
    if (tk_len == 0)
    {
        tk_len = 100;
        TOKENS = calloc(tk_len, sizeof(Token *));
    }
    else if (tk_pos + 2 > tk_len)
    {
#if 1
        tk_len *= 2;
        TOKENS = realloc(TOKENS, tk_len * sizeof(Token *));
#else
        Token **tmp = calloc(tk_len * 2, sizeof(Token *));
        memcpy(tmp, TOKENS, tk_len * sizeof(Token *));
        free(TOKENS);
        TOKENS = tmp;
        tk_len *= 2;
#endif
    }
    Token *token = calloc(1, sizeof(Token));
    token->type = type;
    token->sub_type = sub_type;
    token->level = level;
    switch (type)
    {
    case identifier_:
        token->name = calloc(e - s + 1, sizeof(char));
        strncpy(token->name, text + s, e - s);
        if (strcmp(token->name, "True") == 0)
        {
            token->type = bool_;
            token->bool_ = true;
            break;
        }
        else if (strcmp(token->name, "False") == 0)
        {
            token->type = bool_;
            token->bool_ = false;
            break;
        }
        for (int i = 0; DataTypes[i].name; i++)
            if (strcmp(DataTypes[i].name, token->name) == 0)
            {
                token->type = DataTypes[i].type;
                goto CLABEL0;
            }
        for (int i = 0; Specials[i].name; i++)
            if (strcmp(Specials[i].name, token->name) == 0)
            {
                token->type = Specials[i].type;
                goto CLABEL0;
            }
    CLABEL0:
        break;
    case char_:
        token->index_ = index_;
        index_++;
        token->char_ = calloc(e - s + 1, sizeof(char));
        strncpy(token->char_, text + s, e - s);
        break;
    case int_:
        while (s < e)
            token->int_ = 10 * token->int_ + text[s++] - '0';
        break;
    case float_:
        float f = 0.0;
        token->index_ = index_;
        index_++;
        while (s < e && isdigit(text[s]))
            f = 10 * f + text[s++] - '0';
        s++;
        while (s < e && isdigit(text[s]))
            f = f + (float)(text[s++] - '0') / 10;
        token->float_ = *(uint32_t *)(&f);
        break;
    default:
        break;
    }
    debug("token %k \n", token);
    return (TOKENS[tk_pos++] = token);
}

void build_tokens()
{
    int e = 0;
    size_t level = 0;
    while (text[e])
    {
        Token *token = NULL;
        Type sub_type = 0;
        int s = e;
        if (text[e] == '\n')
        {
            level = 0;
            while (text[e] == '\n')
                e++;
            s = e;
            while (text[e] != '\n' && isspace(text[e]))
                e++;
            level = e - s;
            debug("LEVEL%d\n", level);
            continue;
        }
        if (isspace(text[e]) && text[e] != '\n')
        {
            while (isspace(text[e]) && text[e] != '\n')
                e++;
            continue;
        }
        if (strlen(text + e) > 1 && strncmp(text + e, "/*", 2) == 0)
        {
            e += 2;
            while (text[e + 1] && strncmp(text + e, "*/", 2))
                e++;
            if (!text[e + 1])
                error("Expected end of comment");
            e += 2;
            continue;
        }
#if 0
        // TODO: protect it from stupid parsing errors
        if (strncmp(text + e, "fix:", 4) == 0)
        {
            sub_type = fix_;
            e += 4;
        }
        else if (strncmp(text + e, "dyn:", 4) == 0)
        {
            sub_type = dyn_;
            e += 4;
        }
#endif
        for (int i = 0; Symbols[i].name; i++)
        {
            if (strncmp(Symbols[i].name, text + e, strlen(Symbols[i].name)) == 0)
            {
                token = new_token(s, e + strlen(Symbols[i].name), Symbols[i].type, sub_type, level);
                e += strlen(Symbols[i].name);
                break;
            }
        }
        if (token && token->type)
        {
            token = NULL;
            continue;
        }
        if (isalpha(text[e]))
        {
            while (isalnum(text[e]))
                e++;
            if (e > s)
            {
                new_token(s, e, identifier_, sub_type, level);
                continue;
            }
        }
        if (isdigit(text[e]))
        {
            Type type = int_;
            while (isdigit(text[e]))
                e++;
            if (text[e] == '.')
            {
                type = float_;
                e++;
            }
            while (isdigit(text[e]))
                e++;
            new_token(s, e, type, sub_type, level);
            continue;
        }
        if (strchr("\"\'", text[e]))
        {
            char quote = text[e++];
            while (text[e] && text[e] != quote)
                e++;
            if (text[e++] != quote)
                error("Syntax");
            new_token(s + 1, e - 1, char_, sub_type, level);
            continue;
        }

        error("tokenizing: %s", text + s);
    }
    new_token(0, 0, eof_, none_, level);
}

// ABSTRACT TREE
void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

void print_node(Node *node, int level)
{
    Node *tmp = NULL;
    int curr = level;
    if (node)
    {
        while (curr)
        {
            debug("   ");
            curr--;
        }
        switch (node->token->type)
        {
        case if_:
        case elif_:
        case else_:
        case while_:
        {
            debug("%s%t %s (%t)%s\n", GREEN, node->token->type,
                  node->token->name, node->token->sub_type, RESET);

            tmp = node->left;
            if (node->token->type != else_) // condition
                print_node(tmp->left, level + 1);
            // code bloc
            tmp = node->left->right;
            while (tmp)
            {
                print_node(tmp->left, level + 1);
                tmp = tmp->right;
            }
            // elif / else nodes
            tmp = node;
            while (node->token->type == if_ && tmp->right)
            {
                print_node(tmp->right, level);
                tmp = tmp->right;
            }
            break;
        }
        case func_call_:
        case func_dec_:
        {
            debug("%s%t %s (%t)%s\n", GREEN, node->token->type,
                  node->token->name, node->token->sub_type, RESET);
            // arguments
            tmp = node->left;
            while (tmp)
            {
                print_node(tmp->left, level + 1);
                tmp = tmp->right;
            }
            // code bloc
            tmp = node->right;
            while (tmp)
            {
                print_node(tmp->left, level + 1);
                tmp = tmp->right;
            }
            break;
        }
        default:
        {
            debug("%k\n", node->token);
            print_node(node->left, level + 1);
            print_node(node->right, level + 1);
            break;
        }
        }
    }
}

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    debug("new node %k\n", new->token);
    return new;
}

// TODO: check if can be removed
Node *copy_node(Node *node)
{
    Node *new = calloc(1, sizeof(Node));
    memcpy(new, node, sizeof(Node));
    if (node->left)
        new->left = copy_node(node->left);
    if (node->right)
        new->right = copy_node(node->right);
    return new;
}

Node *expr();
Node *assign();     // = += -= *= /=
Node *logic();      // || or && and
Node *equality();   // ==  !=
Node *comparison(); // < > <= >=
Node *add_sub();    // + -
Node *mul_div();    // * /
Node *unary();      // -
Node *prime();

Token *check(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == TOKENS[exe_pos]->type)
            return TOKENS[exe_pos++];
        type = va_arg(ap, Type);
    }
    return NULL;
}

Token *expect(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == TOKENS[exe_pos]->type)
            return TOKENS[exe_pos++];
        type = va_arg(ap, Type);
    }
    error("Unexpected %t\n", type);
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = logic();
    Token *token;

    if (token = check(assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, 0))
    {
        // TODO: expect identifier at left
        Node *right = assign();
        Node *tmp = right;
        if (token->type != assign_)
        {
            tmp = new_node(new_token(0, 0, token->type == add_assign_ ? add_ : token->type == sub_assign_ ? sub_
                                                                           : token->type == mul_assign_   ? mul_
                                                                           : token->type == div_assign_   ? div_
                                                                                                          : 0,
                                     0, token->level));
            tmp->left = new_node(new_token(0, 0, 0, 0, 0));
            memcpy(tmp->left->token, left->token, sizeof(Token));
            tmp->left->token->type = identifier_;
            tmp->right = right;
        }
        token->type = assign_;
        Node *node = new_node(token);
        node->left = left;
        node->right = tmp;
        return node;
    }
    return left;
}

Node *logic()
{
    Node *left = equality();
    Token *token;

    if (token = check(or_, and_, 0))
    {
        Node *node = new_node(token);
        node->token->index_ = Label_index++;
        node->left = left;
        node->right = logic();
        return node;
    }
    return left;
}

Node *equality()
{
    Node *left = comparison();
    Token *token;
    if (token = check(equal_, not_equal_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = equality();
        left = node;
    }
    return left;
}

Node *comparison()
{
    Node *left = add_sub();
    Token *token;
    if (token = check(less_than_, grea_than_,
                      less_than_equal_, grea_than_equal_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = comparison();
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    Token *token;
    if (token = check(add_, sub_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        return node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = unary();
    Token *token;

    if (token = check(mod_, 0))
    {
        Node *right = mul_div();
        Node *div = new_node(new_token(0, 0, div_, none_, token->level));

        div->left = copy_node(left);
        div->right = copy_node(right);
        Node *mul = new_node(new_token(0, 0, mul_, none_, token->level));
        mul->left = right;
        mul->right = div;

        token->type = sub_;
        Node *node = new_node(token);
        node->left = left;
        node->right = mul;
        return node;
    }
    if (token = check(mul_, div_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        return node;
    }
    return left;
}

// TODO: handle negative number / be carefull of casting long to in for example
Node *unary()
{
    Token *token = check(add_, sub_, 0);
    Node *left = prime();
    if (token && token->type == sub_)
    {
#if 0
        if (minus_int == NULL)
        {
            minus_int = calloc(1, sizeof(Token));
            minus_int->type = int_;
            minus_int->int_ = -1;
            minus_float = calloc(1, sizeof(Token));
            minus_float->type = float_;
            float f = -1.0;
            minus_float->float_ = *(uint32_t *)(&f);
            minus_float->index_ = index_++;
        }
#endif
        Node *node = new_node(new_token(0, 0, neg_, none_, left->token->level));
        node->left = left;
        return node;
    }
    return left;
}

Node *prime()
{
    Node *node = NULL;
    Token *token = NULL;
    Type sub_type = none_;
    if (token = check(identifier_, 0))
    {
        if (check(lparent_, 0))
        {
            debug("found function call\n");
            node = new_node(token);
            node->token->type = func_call_;
            node->token->sub_type = sub_type;
            Node *tmp;
            if (TOKENS[exe_pos]->type != rparent_)
            {
                // function arguments
                node->left = new_node(NULL);
                tmp = node->left;
                // arguments
                while (TOKENS[exe_pos]->type != rparent_ && TOKENS[exe_pos]->type != eof_)
                {
                    tmp->left = expr();
                    if (TOKENS[exe_pos]->type == rparent_ || TOKENS[exe_pos]->type == eof_)
                        break;
                    expect(coma_);
                    tmp->right = new_node(NULL);
                    tmp = tmp->right;
                }
            }
            expect(rparent_);
            if (strcmp(node->token->name, "main") == 0)
            {
                node->token->type = func_dec_;
                expect(dots_);
                tmp = node;
                // main's code bloc
                while (TOKENS[exe_pos]->level > node->token->level && TOKENS[exe_pos]->type != eof_)
                {
                    debug("loop\n");
                    tmp->right = new_node(NULL);
                    tmp = tmp->right;
                    tmp->left = expr();
                }
            }
            return node;
        }
        node = new_node(token);
    }
    else if (check(lparent_, 0))
    {
        node = expr();
        expect(rparent_);
    }
    else if (token = check(char_, int_, float_, bool_, 0))
    {
        if (token->name)
        {
            Type type = token->type;
            token = expect(identifier_);
            token->type = type;
            token->sub_type = sub_type;
        }
        node = new_node(token);
    }
    else if (token = check(eof_))
        node = new_node(token);
    else if (token = check(if_, 0))
    {
        free(token->name);
        token->name = "";
        node = new_node(token);
        node->token->index_ = Label_index++; // current label index
        Label_index++;
        node->token->sub_type = sub_type;

        /*
            cur Label = node->token->index_
            end Label = node->token->index_ - 1
        */
        node->left = new_node(NULL);
        Node *tmp = node->left;

        // the condition bloc
        tmp->left = expr();
        expect(dots_);
        // statement bloc
        while (TOKENS[exe_pos]->level > node->token->level && TOKENS[exe_pos]->type != eof_)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
        // elif
        Node *tmp1 = node;
        while (token = check(elif_, else_, 0))
        {
            free(token->name);
            token->name = "";

            tmp = tmp1;
            tmp->right = new_node(token);
            tmp1 = tmp1->right; // set second tmp

            tmp = tmp->right;
            tmp->token->index_ = Label_index++;
            tmp->token->sub_type = sub_type;

            tmp->left = new_node(NULL);
            tmp = tmp->left;
            if (token->type != else_)
                tmp->left = expr(); // condition
            expect(dots_);
            // code bloc
            while (TOKENS[exe_pos]->level > node->token->level && TOKENS[exe_pos]->type != eof_)
            {
                tmp->right = new_node(NULL);
                tmp = tmp->right;
                tmp->left = expr();
            }
            if (token->type == else_)
                break;
        }
    }
    else if (token = check(while_, 0))
    {
        free(token->name);
        token->name = "";
        node = new_node(token);
        node->token->index_ = Label_index++;
        Label_index++;
        node->token->sub_type = sub_type;
        node->left = new_node(NULL);
        Node *tmp = node->left;

        tmp->left = expr();
        expect(dots_);

        tmp = node->left;
        while (TOKENS[exe_pos]->level > node->token->level && TOKENS[exe_pos]->type != eof_)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
    }
    else if (token = check(func_dec_, 0))
    {
        if (!(token = check(int_, float_, bool_, char_, void_, 0)))
            error("Expected a valid datatype for function declaration");
        Type type = token->type;
        if (!(token = expect(identifier_, 0)))
            error("Expected name for function declaration");
        token->type = func_dec_;
        token->sub_type = type;
        node = new_node(token);
        char *new_name = strjoin(node->token->name, "_", NULL, NULL);
        free(node->token->name);
        node->token->name = new_name;
        Node *tmp;
        expect(lparent_);
        if (TOKENS[exe_pos]->type != rparent_)
        {
            // function arguments
            node->left = new_node(NULL);
            tmp = node->left;
            // arguments
            while (TOKENS[exe_pos]->type != rparent_ && TOKENS[exe_pos]->type != eof_)
            {
                tmp->left = expr();
                if (TOKENS[exe_pos]->type == rparent_ || TOKENS[exe_pos]->type == eof_)
                    break;
                expect(coma_);
                tmp->right = new_node(NULL);
                tmp = tmp->right;
            }
        }
        expect(rparent_);

        expect(dots_);
        tmp = node;
        // code bloc
        while (TOKENS[exe_pos]->level > node->token->level && TOKENS[exe_pos]->type != eof_)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
        // print_node(node, 0);
        // exit(1);
    }
    else // TODO: add Unexpected error message here
        error("%s in prime", token ? type_to_string(token->type) : "(null)");
    return node;
}

void initialize()
{
    print_asm(".intel_syntax noprefix\n");
    print_asm(".include \"import/header.s\"\n\n");
    print_asm(".text\n");
    print_asm(".globl	main\n");

    Node *curr = new_node(NULL);
    Node *head = curr;
    curr->left = expr();
    while (TOKENS[exe_pos]->type != eof_)
    {
        curr->right = new_node(NULL);
        curr = curr->right;
        curr->left = expr();
    };
    debug("%sABSTRACT TREE:%s\n", GREEN, RESET);
    // print_node(head, 0);
    curr = head;
    while (curr)
    {
        print_node(curr->left, 0);
        curr = curr->right;
    }
#if 1
    Node *new = new_node(new_token(0, 0, 0, 0, 0));
    new->token->name = "";
    enter_label(new);
    curr = head;
    while (curr)
    {
        evaluate(curr->left);
        curr = curr->right;
    }
    // exit_label(new);
#endif
    free_node(new);
    free_node(head);
}

void finalize()
{
    for (int i = 0; i < tk_pos; i++)
    {
        // test char variable before making any modification
        if (!TOKENS[i]->name && TOKENS[i]->index_ && TOKENS[i]->type == char_)
            print_asm("STR%zu: .string \"%s\"\n", TOKENS[i]->index_, TOKENS[i]->char_);
        if (!TOKENS[i]->name && TOKENS[i]->index_ && TOKENS[i]->type == float_)
            print_asm("FLT%zu: .long %zu /* %f */\n", TOKENS[i]->index_,
                      *((float *)(&TOKENS[i]->float_)),
                      TOKENS[i]->float_);
    }
    print_asm(".section	.note.GNU-stack,\"\",@progbits\n");
}

Token *evaluate(Node *node)
{
    Token *left = NULL, *right = NULL;
    Type type = node->token->type;
    switch (type)
    {
    case identifier_:
    {
        left = get_var(node->token->name);
        if (!left)
            error("Undeclared variable '%s'", node->token->name);
        node->token = left;
        break;
    }
    case char_:
    case float_:
    case int_:
    case bool_:
    {
        if (node->token->name)
        {
            if (get_var(node->token->name))
                error("redefinition of variable");
            new_variable(node->token);
        }
        break;
    }
    case neg_:
    {
        // TODO: negative float has a different behaviour !!!
        left = evaluate(node->left);
        if (left->type != int_ && left->type != float_)
            error("Invalid unary operation 0");

        if (!left->name)
        {
            node->token->type = left->type;
            switch (left->type)
            {
            case int_:
                // TODO: protect INT_MIN
                node->token->int_ = -1 * left->int_;
                break;
            case float_:
                float f = -1 * (*(float *)(&left->float_));
                left->float_ = *(uint32_t *)(&f);
                node->token = left;
                break;
            default:
                break;
            }
        }
        else
        {
            Node *curr = new_node(new_token(0, 0, mul_, none_, left->level));
            curr->left = new_node(left);
            if (left->type == int_)
            {
                curr->right = new_node(new_token(0, 0, int_, none_, left->level));
                curr->right->token->int_ = -1;
            }
            else if (left->type == float_)
            {
                curr->right = new_node(new_token(0, 0, float_, none_, left->level));
                float f = -1.0;
                curr->right->token->float_ = *(uint32_t *)(&f);
                curr->right->token->index_ = index_++;
            }
            else
                error("Invalid unary operation 1");
            node->token = evaluate(curr);
        }
        break;
    }
    case assign_:
    {
        /*
            TODOS:
                + assign / initializing
                + deep / shallow copy
        */
        left = evaluate(node->left);
        right = evaluate(node->right);
        debug("assign %k and %k \n", left, right);
        if (!left->name || left->type != right->type || !left->ptr)
            error("Invalid assignement");
        node->token = left;
        switch (left->type)
        {
        case int_:
            if (right->ptr)
            {
                print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     QWORD PTR -%zu[rbp], rax /* assign %s */\n",
                          left->ptr, left->name);
            }
            else
                print_asm("   mov     QWORD PTR -%zu[rbp], %d /* assign %s */\n",
                          left->ptr, right->int_, left->name);
            break;
        case float_:
            // TODO: check xmms, with multiple LABELS[lb_pos]->VARIABLES
            if (right->ptr)
            {
                print_asm("   movss   xmm1, DWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1 /* assign %s */\n",
                          left->ptr, left->name);
            }
            else
            {
                print_asm("   movss   xmm1, DWORD PTR FLT%zu[rip]\n", right->index_);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1 /* assign %s */\n",
                          left->ptr, left->name);
            }
            break;
        case char_:
            /*
                TODO:
                    + check right has ptr, then load value from there
                    + strdup from right
            */
#if 0
            if (left->sub_type == fix_)
            {
#endif
            if (right->index_)
                print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
            else
                print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
#if 0
            }
            else if (left->sub_type == dyn_)
            {
                if (right->index_)
                    print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
                else
                    print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     rdi, rax\n");
                print_asm("   call    _strdup\n");
            }

            else
                error("in assign char\n");
#endif
            print_asm("   mov     QWORD PTR -%zu[rbp], rax /* assign  %s */\n", left->ptr,
                      left->name);
            break;
        case bool_:
            if (right->ptr)
            {
                print_asm("   mov     al, BYTE PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     BYTE PTR -%zu[rbp], al /* assign  %s */\n", left->ptr,
                          left->name);
            }
            else if (right->c)
                print_asm("   mov     BYTE PTR -%zu[rbp], %cl\n", left->ptr, right->c);
            else
                print_asm("   mov     BYTE PTR -%zu[rbp], %d /* assign  %s */\n", left->ptr,
                          right->bool_, left->name);
            break;
        default:
            error("add assembly for this one 0");
            break;
        }
        break;
    }
    // arithmetic operators
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != right->type)
            error("Uncompatible type in math operation");
        node->token->type = left->type;
        // has no name // optimization
        if (!left->name && !right->name)
        {
            debug("0. do %t between %k with %k\n", type, left, right);
            left->index_ = 0;
            right->index_ = 0;
            switch (node->token->type)
            {
            case int_:
                if (type == add_)
                    node->token->int_ = left->int_ + right->int_;
                else if (type == sub_)
                    node->token->int_ = left->int_ - right->int_;
                else if (type == mul_)
                    node->token->int_ = left->int_ * right->int_;
                else if (type == div_)
                {
                    if (right->int_ == 0)
                        error("can't devide by 0 (int)");
                    node->token->int_ = left->int_ / right->int_;
                }
                break;
            case float_:
                node->token->index_ = index_++;
                float l = *(float *)(&left->float_);
                float r = *(float *)(&right->float_);
                float res;
                if (type == add_)
                    res = l + r;
                else if (type == sub_)
                    res = l - r;
                else if (type == mul_)
                    res = l * r;
                else if (type == div_)
                {
                    if (r == 0)
                        error("can't devide by 0 (float)");
                    res = l / r;
                }
                node->token->float_ = *(uint32_t *)(&res);
                break;
            case char_:
                node->token->index_ = index_++;
                // node->token->sub_type = dyn_;
                if (type == add_)
                    node->token->char_ = strjoin(left->char_, right->char_, NULL, NULL);
                else
                    error("invalid math operation for characters");
                break;
            default:
                error("math operation 0");
                break;
            }
        }
        else
        {
            debug("1. do %t between %k with %k\n", type, left, right);
            char *str;
            switch (node->token->type)
            {
            case int_:
                node->token->ptr = (ptr += 8);
                // set left
                print_asm("   mov     rax, ");
                if (left->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    print_asm("%d\n", left->int_);
                // set right
                print_asm("   %s", type == add_   ? "add     rax, "
                                   : type == sub_ ? "sub     rax, "
                                   : type == mul_ ? "imul    rax, "
                                   : type == div_ ? "cdq\n   mov     rbx, "
                                                  : NULL);
                if (right->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    print_asm("%d\n", right->int_);
                if (type == div_)
                    print_asm("   idiv    rbx\n");
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
                break;
            case float_:
                node->token->ptr = (ptr += 4);
                // set left
                print_asm("   movss   xmm1, ");
                if (left->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", left->index_);
                else
                    print_asm("%zu\n", left->float_);
                // set right
                print_asm("   %s", type == add_   ? "addss   xmm1, "
                                   : type == sub_ ? "subss   xmm1, "
                                   : type == mul_ ? "mulss   xmm1, "
                                   : type == div_ ? "divss   xmm1, "
                                                  : NULL);
                if (right->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                else
                    print_asm("%zu\n", right->float_);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1\n", node->token->ptr);
                break;
            case char_:
                if (type != add_)
                    error("math operation 2");
                // node->token->sub_type = dyn_;
                node->token->ptr = (ptr += 8);
                if (left->ptr)
                    print_asm("   mov     rdi, QWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", left->index_);
                    print_asm("   mov     rdi, rax\n");
                }
                else
                    error("in char joining 1");

                if (right->ptr)
                    print_asm("   mov     rsi, QWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
                    print_asm("   mov     rsi, rax\n");
                }
                else
                    error("in char joining 2");
                print_asm("   call	  _strjoin\n");
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);

                break;
            default:
                error("math operation 1");
                break;
            }
        }
        break;
    }
    // logic operators
    case not_:
        error("handle not logic operator");
        break;
    case not_equal_:
    case equal_:
    case less_than_:
    case grea_than_:
    case less_than_equal_:
    case grea_than_equal_:
    {
        node->token->type = bool_;
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != right->type)
            error("Uncompatible type in logic operation");
        // has no name // optimization
        if (!left->ptr && !right->ptr)
        {
            debug("0. do %t between %k with %k\n", type, left, right);
            left->index_ = 0;
            right->index_ = 0;
            switch (left->type)
            {
            case int_:
                if (type == equal_)
                    node->token->bool_ = (left->int_ == right->int_);
                else if (type == not_equal_)
                    node->token->bool_ = (left->int_ != right->int_);
                else if (type == less_than_)
                    node->token->bool_ = (left->int_ < right->int_);
                else if (type == grea_than_)
                    node->token->bool_ = (left->int_ > right->int_);
                else if (type == less_than_equal_)
                    node->token->bool_ = (left->int_ <= right->int_);
                else if (type == grea_than_equal_)
                    node->token->bool_ = (left->int_ >= right->int_);

                break;
            case float_:
                if (type == equal_)
                    node->token->bool_ = (left->float_ == right->float_);
                else if (type == not_equal_)
                    node->token->bool_ = (left->float_ != right->float_);
                else if (type == less_than_)
                    node->token->bool_ = (left->float_ < right->float_);
                else if (type == grea_than_)
                    node->token->bool_ = (left->float_ > right->float_);
                else if (type == less_than_equal_)
                    node->token->bool_ = (left->float_ <= right->float_);
                else if (type == grea_than_equal_)
                    node->token->bool_ = (left->float_ >= right->float_);
                break;
            case char_:
                if (type == equal_)
                    node->token->bool_ = (strcmp(left->char_, right->char_) == 0);
                else if (type == not_equal_)
                    node->token->bool_ = (strcmp(left->char_, right->char_) != 0);
                else
                    error("Invalid logic operation on char");
                break;
            default:
                error("logic operation 0");
                break;
            }
        }
        else
        {
            debug("1. do %t between %k with %k\n", type, left, right);
#define BOOL_PTR 0
#if BOOL_PTR
            node->token->ptr = (ptr += 1);
#else
            node->token->c = 'a';
#endif
            char *str;
            switch (left->type)
            {
            // TODO: handle heap allocated LABELS[lb_pos]->VARIABLES
            case int_:
                // set left
                print_asm("   mov     rax, ");
                if (left->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    print_asm("%d\n", left->int_);

                print_asm("   cmp     rax, ");
                if (right->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    print_asm("%d\n", right->int_);

                print_asm("   %s   al\n", type == equal_             ? "sete "
                                          : type == equal_           ? "setne"
                                          : type == less_than_       ? "setl "
                                          : type == less_than_equal_ ? "setle"
                                          : type == grea_than_       ? "setg "
                                          : type == grea_than_equal_ ? "setge"
                                                                     : NULL);

#if BOOL_PTR
                print_asm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            case float_:
                if ((type == less_than_ && (type = grea_than_)) ||
                    (type == less_than_equal_ && (type = grea_than_equal_)))
                {
                    Token *tmp = left;
                    left = right;
                    right = tmp;
                }
                print_asm("   movss   xmm0, ");
                if (left->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", left->index_);
                else
                    print_asm("%zu\n", left->float_);

                print_asm("   %s xmm0, ", type != equal_ ? "comiss " : "ucomiss");
                if (right->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                else
                    print_asm("%zu\n", right->float_);

                print_asm("   %s   al\n",
                          type == grea_than_         ? "seta "
                          : type == grea_than_equal_ ? "setnb"
                          : type == equal_           ? "setnp"
                          : type == not_equal_       ? "setp "
                                                     : NULL);
                if (type == equal_)
                {
                    print_asm("   mov	  edx, 0\n");
                    print_asm("   movss	  xmm0, ");
                    if (left->ptr)
                        print_asm("DWORD PTR -%zu[rbp]\n", left->ptr);
                    else if (left->index_)
                        print_asm("DWORD PTR FLT%zu[rip]\n", left->index_);
                    else
                        print_asm("%zu\n", left->float_);
                    print_asm("   ucomiss xmm0, ");
                    if (right->ptr)
                        print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                    else if (right->index_)
                        print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                    else
                        print_asm("%zu\n", right->float_);
                    print_asm("   cmovne  eax, edx\n");
                }
#if BOOL_PTR
                print_asm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            case char_:
                if (type != equal_)
                    error("logic operation 3");
                if (left->ptr)
                    print_asm("   mov     rsi, QWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", left->index_);
                    print_asm("   mov     rsi, rax\n");
                }
                else
                    error("in char equal");

                if (right->ptr)
                    print_asm("   mov     rdi, QWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
                    print_asm("   mov     rdi, rax\n");
                }
                else
                    error("in char equal 2");
                print_asm("   call	  _strcmp\n");
#if BOOL_PTR
                print_asm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            default:
                error("logic operation 4");
                break;
            }
        }
        break;
    }
    case and_:
    case or_:
    {
        Node *tmp = node;
        // print_node(node, 0);
        // last right is last node in or/and
        int i = 0;
        while (tmp->token->type == and_ || tmp->token->type == or_)
        {
            left = evaluate(tmp->left);
            if (left->type != bool_)
                error("0.Expected boolean value");

            print_asm("   /* %s operation %d */\n", type_to_string(node->token->type), i++);
            if (left->ptr)
                print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
            else if (left->c)
                print_asm("   cmp     %cl, 1\n", left->c);
            else
            {
                print_asm("   mov     al, %d\n", left->bool_);
                print_asm("   cmp     al, 1\n");
            }
            if (tmp->token->type == or_)
                print_asm("   je      %s%zu\n", LABEL->name, node->token->index_);
            else if (tmp->token->type == and_)
                print_asm("   jne     %s%zu\n", LABEL->name, node->token->index_);
            tmp = tmp->right;
            if (tmp->token->type == and_ || tmp->token->type == or_)
                print_asm("%s%zu:\n", LABEL->name, tmp->token->index_);
        }
        right = evaluate(tmp);
        if (right->type != bool_)
            error("0.Expected boolean value");
        print_asm("%s%zu:\n", LABEL->name, node->token->index_);
        node->token->c = 'a';
        node->token->type = bool_;
        break;
    }
#if 1
    case if_:
    {
        Node *curr = node->left;
        size_t end_index = node->token->index_ + 1;
        left = evaluate(curr->left);
        if (left->type != bool_)
            error("Expected a valid condition in if statement");

        print_asm("%s%zu: %43s\n", LABEL->name, node->token->index_, "/* if statement */");
        if (left->ptr)
            print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
        else if (left->c)
            print_asm("   cmp     %cl, 1\n", left->c);
        else
        {
            print_asm("   mov     al, %d\n", left->bool_);
            print_asm("   cmp     al, 1\n");
        }
        if (node->right)
            print_asm("   jne     %s%zu %39s\n", LABEL->name, node->right->token->index_, "/* jmp next statement */");
        else
            print_asm("   jne     %s%zu %15s\n", LABEL->name, end_index, "/* jmp end statemnt */");
        curr = curr->right;
        // if statment bloc
        while (curr)
        {
            evaluate(curr->left);
            curr = curr->right;
        }
        curr = node->left;
        if (node->right)
            print_asm("   jmp     %s%zu %38s\n", LABEL->name, end_index, "/* jmp end statement */");

        // elif / else statement
        curr = node->right;
        while (curr)
        {

            if (curr->token->type == elif_)
            {
                Node *tmp0 = curr->left;
                // evaluate elif
                print_asm("%s%zu: %45s\n", LABEL->name, curr->token->index_, "/* elif statement */");
                Node *tmp = tmp0;

                left = evaluate(tmp->left);
                if (left->type != bool_)
                    error("Expected a valid condition in elif statement");
                if (left->ptr)
                    print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
                else if (left->c)
                    print_asm("   cmp     %cl, 1\n", left->c);
                else
                {
                    print_asm("   mov     al, %d\n", left->bool_);
                    print_asm("   cmp     al, 1\n");
                }
                if (curr->right)
                    print_asm("   jne     %s%zu %39s\n", LABEL->name,
                              curr->right->token->index_, "/* jmp next statement */");
                else
                    print_asm("   jne     %s%zu %38s\n", LABEL->name, end_index, "/* jmp end statemnt */");
                tmp = tmp->right;
                while (tmp)
                {
                    evaluate(tmp->left);
                    tmp = tmp->right;
                }
                if (curr->right)
                    print_asm("   jmp     %s%zu %38s\n", LABEL->name,
                              end_index, "/* jmp end statement */");
            }
            else if (curr->token->type == else_)
            {
                Node *tmp = curr->left->right;
                print_asm("%s%zu: %45s\n", LABEL->name, curr->token->index_, "/* else statement */");
                while (tmp)
                {
                    evaluate(tmp->left);
                    tmp = tmp->right;
                }
            }
            curr = curr->right;
        }
        // end statement bloc
        print_asm("%s%zu: %44s\n", LABEL->name, end_index, "/* end statement */");

        break;
    }
    case while_:
    {
        Node *curr = node->left;
        print_asm("   jmp     %s%zu %46s\n", LABEL->name,
                  node->token->index_ - 1, "/* jmp to while loop condition*/");
        // while loop bloc
        print_asm("%s%zu: %44s\n", LABEL->name, node->token->index_, "/* while loop bloc*/");
        Node *tmp = curr->right;
        while (tmp)
        {
            evaluate(tmp->left);
            tmp = tmp->right;
        }
        // while loop condition
        print_asm("%s%zu: %53s\n", LABEL->name, node->token->index_ - 1,
                  "/* while loop condition */");
        left = evaluate(curr->left);
        if (left->type != bool_)
            error("Expected a valid condition in if statment");
        if (left->ptr)
            print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
        else if (left->c)
            print_asm("   cmp     %cl, 1\n", left->c);
        else
        {
            print_asm("   mov     al, %d\n", left->bool_);
            print_asm("   cmp     al, 1\n");
        }
        print_asm("   je      %s%zu %43s\n", LABEL->name, node->token->index_,
                  "/* je to while loop bloc*/");
        break;
    }
#endif
    case func_call_:
    {
        debug("found function call has name '%s'\n", node->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            // debug("found output\n");
            Node *tmp = node->left;
            while (tmp)
            {
                output(evaluate(tmp->left));
                tmp = tmp->right;
            }
        }
        else
        {
            char *name = strjoin(node->token->name, "_", NULL, NULL);
            Node *func;
            if (!(func = get_func(name)))
            {
                visualize();
                error("Undeclared function '%s'\n", node->token->name);
            }
            if (func->left)
            {
                // push arguments:
                int stack_len = 100;
                int stack_pos = 0;
                Token **stack = calloc(stack_len, sizeof(Token *));
                Node *tmp = func->left;
                while (tmp)
                {
                    stack[stack_pos++] = get_var(tmp->left->token->name);
                    if (stack_pos + 2 > stack_len)
                    {
                        stack = realloc(stack, 2 * stack_len * sizeof(Token *));
                        stack_len *= 2;
                    }
                    tmp = tmp->right;
                }
                stack_pos--;
                tmp = func->left;
                while (stack_pos >= 0)
                {
                    Token *token = stack[stack_pos];
                    // check argument type if variable or value....
                    Token *arg = tmp->left->token;
                    if (arg->type != token->type)
                        error("Incompatible type in function call\n");
                    if (token->ptr)
                        print_asm("   push    QWORD PTR -%zu[rbp]\n", token->ptr);
                    stack_pos--;
                    tmp = tmp->right;
                }
                free(stack);
            }
            print_asm("   call    %s\n", name);
            free(name);
        }
        break;
    }
    case func_dec_:
    {
        new_func(node);
        enter_label(node);
        char *name = node->token->name;
        print_asm("%s:\n", name);
        print_asm("   push    rbp\n");
        print_asm("   mov     rbp, rsp\n");
        print_asm("   sub     rsp, %zu\n", rsp);

        // arguments
        Node *tmp = node->left;
        while (tmp)
        {
            Token *token = evaluate(tmp->left);
            // TODO: I did only test for integers, check other data types
            print_asm("   mov	  rax, QWORD PTR %zu[rbp]\n", token->ptr + 8);
            print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", token->ptr);
            tmp = tmp->right;
        }

        // code bloc
        tmp = node->right;
        while (tmp)
        {
            if (tmp->left->token->type == func_dec_)
                print_asm("   jmp end_%s\n", tmp->left->token->name);

            evaluate(tmp->left);
            tmp = tmp->right;
        }
        print_asm("   leave\n");
        print_asm("   ret\n");
        print_asm("end_%s:\n\n", name);
        exit_label(node);

        break;
    }
    default:
        error("in evaluate %t", type);
        break;
    }
    if (ptr + 10 > rsp)
    {
        // TODO: protect this line from being printed in wrong place
        // after label for example
        rsp += 30;
        print_asm("   sub     rsp, 30\n");
    }
    return node->token;
}

int main(int argc, char **argv)
{
    // TODO: check if file ends with .w
    if (argc != 2)
        error("require one file.w as argument\n");
    file = fopen(argv[1], "r");
    asm_fd = fopen("file.s", "w");

    if (!file || !asm_fd)
        error("Opening file");
    fseek(file, 0, SEEK_END);
    size_t txt_len = ftell(file);
    if (!(text = calloc(txt_len + 1, sizeof(char))))
        error("Allocation");
    fseek(file, 0, SEEK_SET);
    fread(text, txt_len, sizeof(char), file);
    fclose(file);
    debug("%s\n\n", text);
    Label_index = 1;
    index_ = 1;

    build_tokens();
    debug("\n");
    free(text);
    rsp = 30;
    initialize();
    finalize();
    debug("\nresult: (exiting with %d)\n", EXIT_STATUS);
    exit(EXIT_STATUS);
}