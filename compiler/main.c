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

// structs, enums
typedef struct Token Token;
typedef struct Node Node;
typedef enum Type Type;

// stupid implicit declaration error
void error(char *msg);
Token *get_var(char *name);
Token *evaluate(Node *node);

enum Type
{
    eof_,
    // operators
    add_,
    sub_,
    mul_,
    div_,
    // parents
    lparent_,
    rparent_,
    // assignement
    assign_,
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
    // Data types
    char_,
    int_,
    float_,
    identifier_,
    // function
    func_dec_,
    func_call_,
    // built ins
    length_,
    printstring_,
};

struct Token
{
    Type type;
    char *name;
    uintptr_t ptr;
    union
    {
        struct
        {
            char *char_;
            size_t index_;
        };
        long long int_;
        uint32_t float_;
    };
};

struct Node
{
    Node *left, *right;
    Token *token;
};

struct
{
    Type type;
    char *name;
} DataTypes[] = {
    {char_, "char"},
    {int_, "int"},
    {float_, "float"},
    {0, 0},
};

struct
{
    Type type;
    char *name;
} Symbols[] = {
    // operators
    {add_, "+"},
    {sub_, "-"},
    {mul_, "*"},
    {div_, "/"},
    // parents
    {lparent_, "("},
    {rparent_, ")"},
    // assign
    {assign_, "= "},
    {add_assign_, "+="},
    {sub_assign_, "-="},
    {mul_assign_, "*="},
    {div_assign_, "/="},
    {0, 0},
};

struct
{
    Type type;
    char *name;
} Random[] = {
    {eof_, "EOF"},
    {identifier_, "identifier"},
    // function
    {func_dec_, "function declaration"},
    {func_call_, "function call"},
    {0, 0},
};

// Debuging
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "\033[0;31m");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\033[0m\n");
    exit(1);
};

char *type_to_string(Type type)
{
    for (int i = 0; DataTypes[i].name; i++)
        if (DataTypes[i].type == type)
            return DataTypes[i].name;
    for (int i = 0; Symbols[i].name; i++)
        if (Symbols[i].type == type)
            return Symbols[i].name;
    for (int i = 0; Random[i].name; i++)
        if (Random[i].type == type)
            return Random[i].name;
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
            case 'k':
            {
                Token *token = (Token *)va_arg(args, Token *);
                if (token)
                {
                    fprintf(stdout, "type: %s, ", type_to_string(token->type));
                    if (token->name)
                        fprintf(stdout, "name: %s, ", token->name);
                    switch (token->type)
                    {
                    case char_:
                        fprintf(stdout, "value: %s, LC: %zu, ", token->char_, token->index_);
                        break;
                    case int_:
                        fprintf(stdout, "value: %d, ", token->int_);
                        break;
                    case float_:
                        fprintf(stdout, "value: %zu (%f), ", token->float_, *(float *)(&token->float_));
                        break;
                    default:
                        // ft_putstr(stdout, "Unkown");
                        break;
                    }
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
        else
            fprintf(stdout, "%c", conv[i]);
        i++;
    }
#endif
}

// file
FILE *file;
char *text;

// tokens
Token **tokens;
int tk_len;
int tk_pos;

// variables
Token **variables;
int var_len;
int var_pos;
uintptr_t ptr;
size_t index_;

bool BuiltIns[100];

int asm_fd;
void print_asm(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(asm_fd, fmt, ap);
}

void output(Token *token)
{
    switch (token->type)
    {
    case char_:
    {
        BuiltIns[length_] = true;
        BuiltIns[printstring_] = true;
        debug("found %s, load string from STR%zu  \n", type_to_string(token->type), token->index_);
        print_asm("   lea     rax, STR%zu[rip]\n", token->index_);
        print_ssm("   mov QWORD PTR -8[rbp], rax\n");
        print_ssm("   mov rbx, rax\n");
        print_ssm("   call printstring\n");
        break;
    }
    case float_:
    case int_:
    case identifier_:
        break;
        break;
    default:
        error("Error in output unknown type");
        break;
    }
}

// utils
char *strjoin(char *left, char *right)
{
    char *res = calloc(strlen(left) + strlen(right) + 1, sizeof(char));
    strcpy(res, left);
    strcpy(res + strlen(res), right);
    return res;
}

// Tokens
void free_token(Token *token)
{
    debug("free token has type %s\n", type_to_string(token->type));
    if (token->name)
        free(token->name);
    if (token->type == char_)
        free(token->char_);
    free(token);
}

Token *new_token(int s, int e, Type type)
{
    Token *token = calloc(1, sizeof(Token));
    if (tk_pos + 10 > tk_len)
    {
        Token **tmp = calloc(tk_len * 2, sizeof(Token *));
        memcpy(tmp, tokens, tk_len * sizeof(Token *));
        free(tokens);
        tokens = tmp;
        tk_len *= 2;
    }
    token->type = type;
    switch (type)
    {
    case identifier_:
        token->name = calloc(e - s + 1, sizeof(char));
        strncpy(token->name, text + s, e - s);
        break;
    case char_:
        index_++;
        token->index_ = index_;
        token->char_ = calloc(e - s + 1, sizeof(char));
        strncpy(token->char_, text + s, e - s);
        break;
    case int_:
        while (s < e)
            token->int_ = 10 * token->int_ + text[s++] - '0';
        break;
    case float_:
        float f = 0.0;
        index_++;
        token->index_ = index_;
        while (s < e)
        {
            f = 10 * f + text[s++] - '0';
            if (text[s] == '.')
            {
                s++;
                break;
            }
        }
        while (s < e)
            f = f + (float)(text[s++] - '0') / 10;

        token->float_ = *(uint32_t *)(&f);
        break;
    default:
        break;
    }
    debug("new token %k\n", token);
    return (tokens[tk_pos++] = token);
}

Token *new_variable(Token *token)
{
    switch (token->type)
    {
    case int_:
    case float_:
        token->ptr = (ptr += 4);
        break;
    case char_:
        token->ptr = (ptr += 8);
        break;
    default:
        break;
    }
    if (var_pos + 10 > var_len)
    {
        Token **tmp = calloc(var_len * 2, sizeof(Token *));
        memcpy(tmp, variables, var_len * sizeof(Token *));
        free(variables);
        variables = tmp;
        var_len *= 2;
    }
    return (variables[var_pos++] = token);
}

void build_tokens()
{
    Token *token;
    int s = 0;
    int e = s;

    // Expect main label
    while (isspace(text[e]))
        e++;
    if (strncmp(&text[e], "main:\n", strlen("main:\n")))
        error("main label with new line is required\n");
    e += strlen("main:\n");
    while (text[e])
    {
        token = NULL;
        s = e;
        if (isspace(text[e]))
        {
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
        for (int i = 0; Symbols[i].name; i++)
        {
            if (strncmp(Symbols[i].name, text + e, strlen(Symbols[i].name)) == 0)
            {
                token = new_token(s, e + strlen(Symbols[i].name), Symbols[i].type);
                e += strlen(Symbols[i].name);
                break;
            }
        }
        if (token && token->type)
        {
            token = NULL;
            continue;
        }
        while (isalpha(text[e]))
            e++;
        if (e > s)
        {
            new_token(s, e, identifier_);
            continue;
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
            if (e > s)
            {
                new_token(s, e, type);
                continue;
            }
        }
        if (strchr("\"\'", text[e]))
        {
            char quote = text[e++];
            while (text[e] && text[e] != quote)
                e++;
            if (text[e++] != quote)
                error("Syntax");
            new_token(s + 1, e - 1, char_);
            continue;
        }
        error("tokenizing");
    }
    new_token(0, 0, eof_);
}

// Tree
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
    if (node)
    {
        int curr = level;
        while (curr)
        {
            debug("    ");
            curr--;
        }
        debug("%k\n", node->token);
        print_node(node->left, level + 1);
        print_node(node->right, level + 1);
    }
}

Node *new_node(Token *token, Node *left, Node *right)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    new->left = left;
    new->right = right;
    return new;
}

Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();

bool check(Type to_find, ...)
{
    va_list ap;
    va_start(ap, to_find);
    while (1)
    {
        Type type = va_arg(ap, Type);
        if (type == to_find)
            return true;
        if (type == 0)
            break;
    }
    return false;
}