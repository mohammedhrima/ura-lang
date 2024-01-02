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
Token *get_var(char *name);
Token *evaluate(Node *node);

enum Type
{
    none_,
    eof_,
    // operators
    add_,
    sub_,
    mul_,
    div_,
    // comparision
    // TODO: expect expression after operator
    less_than_,
    more_than_,
    less_than_equal_,
    more_than_equal_,
    equal_,
    // parents
    lparent_,
    rparent_,
    // assignement
    assign_,
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
    // memory
    fix_,
    dyn_,
    // Data types
    char_,
    int_,
    float_,
    bool_,
    identifier_,
    neg_,
    // function
    func_dec_,
    func_call_,
    coma_,
};

struct Token
{
    Type type;
    Type sub_type;
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
        bool bool_;
    };
};

struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

struct
{
    Type type;
    char *name;
} DataTypes[] = {
    // chars
    {char_, "char"},
    {char_, "fix:char"},
    {char_, "dyn:char"},
    // int
    {int_, "int"},
    {int_, "fix:int"}, 
    {int_, "dyn:int"}, /* big Int */
    // float
    {float_, "float"},
    {float_, "fix:float"},
    {float_, "fix:float"}, /* big Float */
    // bool
    {bool_, "bool"},
    // {bool_, "fix:bool"},
    // {bool_, "dyn:bool"},
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
    // parents, coma
    {lparent_, "("},
    {rparent_, ")"},
    {coma_, ","},
    // assign
    {assign_, "= "},
    {add_assign_, "+="},
    {sub_assign_, "-="},
    {mul_assign_, "*="},
    {div_assign_, "/="},
    // comparision
    {less_than_, "<"},
    {more_than_, ">"},
    {less_than_equal_, "<="},
    {more_than_equal_, ">="},
    {equal_, "=="},
    {equal_, "is "},
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
    {neg_, "negative"},
    {fix_, "fix"},
    {dyn_, "dynamic"},
    {none_, "none"},
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
                    fprintf(stdout, "subtype: %s, ", type_to_string(token->sub_type));
                    if (token->name)
                        fprintf(stdout, "name: %s, ", token->name);
#if 1
                    else
#endif
                        switch (token->type)
                        {
                        case char_:
                            fprintf(stdout, "value: %s, ", token->char_);
                            if (token->index_)
                                fprintf(stdout, "in STR%zu, ", token->index_);
                            break;
                        case int_:
                            fprintf(stdout, "value: %d, ", token->int_);
                            break;
                        case float_:
                            fprintf(stdout, "value: %zu (%.2f), ", token->float_, *(float *)(&token->float_));
                            if (token->index_)
                                fprintf(stdout, "in FLT%zu, ", token->index_);
                            break;
                        case bool_:
                            fprintf(stdout, "value: %s, ", token->bool_ ? "True" : "False");
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
int exe_pos;

// variables
Token **variables;
int var_len;
int var_pos;
uintptr_t ptr;
size_t index_;
/*
TODO:
    protect Label index, putstr and punbr ...
    may cause problems
*/
// built ins
bool Len_built_in;
bool Putstr_built_in;
bool Putnbr_built_in;
bool Putchar_built_in;
size_t Label;

size_t rsp;
FILE *asm_fd;
void print_asm(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(asm_fd, fmt, ap);
}

void output(Token *token)
{
    // TODO: handle bool
    print_asm("   /* call output */\n");
    debug("output %k\n", token);
    switch (token->type)
    {
    case char_:
    {
        Putstr_built_in = true;

        if (token->ptr)
        {
            print_asm("   mov     rbx, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   call    putstr\n");
        }
        else
        {
            print_asm("   lea   rbx, STR%zu[rip]\n", token->index_);
            print_asm("   call  putstr\n");
        }
        break;
    }
    case int_:
    {
        Putnbr_built_in = true;
        if (token->ptr)
        {
            print_asm("   mov   rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   mov   rdi, rax\n");
            print_asm("   call  putnbr\n");
        }
        else
        {
            token->ptr = (ptr += 8);
            print_asm("   mov   QWORD PTR -%zu[rbp], %lld\n", token->ptr, token->int_);
            print_asm("   mov   rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   mov   rdi, rax\n");
            print_asm("   call  putnbr\n");
        }
        break;
    }
    case float_:
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

Token *new_token(int s, int e, Type type, Type sub_type)
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
    token->sub_type = sub_type;
    switch (type)
    {
    case identifier_:
        token->name = calloc(e - s + 1, sizeof(char));
        strncpy(token->name, text + s, e - s);
        if (strcmp(token->name, "True") == 0)
        {
            token->type = bool_;
            token->bool_ = true;
        }
        else if (strcmp(token->name, "False") == 0)
        {
            token->type = bool_;
            token->bool_ = false;
        }
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
        // exit(0);
        break;
    default:
        break;
    }
    debug("new token %k\n", token);
    return (tokens[tk_pos++] = token);
}

Token *new_variable(Token *token)
{
    // TODO: check if you can remove initilize instruction
    print_asm("   /* declare %s */\n", token->name);
    switch (token->type)
    {
    case char_:
        token->ptr = (ptr += 8);
        print_asm("\n");
        break;
    case int_:
        token->ptr = (ptr += 8);
        print_asm("   mov     QWORD PTR -%zu[rbp], %d\n", token->ptr, 0);
        break;
    case float_:
        token->ptr = (ptr += 4);
        print_asm("   mov     QWORD PTR -%zu[rbp], %d\n", token->ptr, 0);
        break;
#if 0
    case bool_:
        token->ptr = (ptr += 1);
        print_asm("   mov     BYTE PTR -%zu[rbp], %d\n", token->ptr, 0);
#endif
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
    debug("new variable %k\n", token);
    return (variables[var_pos++] = token);
}

void build_tokens()
{
    int e = 0;
    // Expect main label
    while (isspace(text[e]))
        e++;
    // TODO: main could any where
    if (strncmp(&text[e], "main:\n", strlen("main:\n")))
        error("main label with new line is required\n");
    e += strlen("main:\n");
    while (text[e])
    {
        Token *token = NULL;
        Type sub_type = fix_;
        int s = e;
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
        // TODO: protect it in parsing from stupid errors
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
        for (int i = 0; Symbols[i].name; i++)
        {
            if (strncmp(Symbols[i].name, text + e, strlen(Symbols[i].name)) == 0)
            {
                token = new_token(s, e + strlen(Symbols[i].name), Symbols[i].type, sub_type);
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
            new_token(s, e, identifier_, sub_type);
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
            new_token(s, e, type, sub_type);
            continue;
        }
        // TODO: update error message
        if (strchr("\"\'", text[e]))
        {
            char quote = text[e++];
            while (text[e] && text[e] != quote)
                e++;
            if (text[e++] != quote)
                error("Syntax");
            new_token(s + 1, e - 1, char_, sub_type);
            continue;
        }
        error("tokenizing");
    }
    new_token(0, 0, eof_, none_);
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

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    return new;
}

Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *unary();
Node *prime();

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
}

Token *expect(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == tokens[exe_pos]->type)
            return tokens[exe_pos++];
        type = va_arg(ap, Type);
    }
    error("Unexpected %s\n", type_to_string(type));
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add_sub();
    Token *token;
    if (token = check(assign_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = assign();
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    Token *token;
#if 0
    while (token = check(add_, sub_, 0))
    {
        Node *node = new_node(token);
        node->right = left;
        node->left = mul_div();
        left = node;
    }
#else
    if (token = check(add_, sub_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        return node;
    }
#endif
    return left;
}

Node *mul_div()
{
    Node *left = unary();
    Token *token;
#if 0
    while(token = check(mul_, div_, 0))
    {
        Node *node = new_node(token);
        node->right = left;
        node->left = unary();
        left = node;
    }
#else
    if (token = check(mul_, div_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        return node;
    }
#endif
    return left;
}

// TODO: handle negative number / be carefull of casting
Node *unary()
{
    Token *token = check(add_, sub_, 0);
    Node *left = prime();
    if (token && token->type == sub_)
    {
        // if (minus_int == NULL)
        // {
        //     minus_int = calloc(1, sizeof(Token));
        //     minus_int->type = int_;
        //     minus_int->int_ = -1;
        //     minus_float = calloc(1, sizeof(Token));
        //     minus_float->type = float_;
        //     float f = -1.0;
        //     minus_float->float_ = *(uint32_t *)(&f);
        //     minus_float->index_ = index_++;
        // }
        Node *node = new_node(new_token(0, 0, neg_, none_));
        node->left = left;
        return node;
    }
    return left;
}

Node *prime()
{
    Node *node = NULL;
    Token *token = NULL;
    if (token = check(identifier_, 0))
    {
        for (int i = 0; DataTypes[i].name; i++)
        {
            if (strcmp(DataTypes[i].name, token->name) == 0)
            {
                Type type = DataTypes[i].type;
                Type sub_type = token->sub_type;
                debug("find %s\n", type_to_string(DataTypes[i].type));
                node = new_node(expect(identifier_, 0));
                node->token->type = type;
                node->token->sub_type = sub_type;
                if (get_var(node->token->name))
                    error("redefinition of variable");
                new_variable(node->token);
                return node;
            }
        }
#if 1
        Token *tmp_token;
        if (tmp_token = check(lparent_, 0))
        {
            debug("found function\n");
            /*
                TODO: split
                    + function call
                    + function declaration
            */
            node = new_node(token);
            node->token->type = func_call_;
            node->left = expr();
            Node *tmp_node = node;
            while (tmp_token = check(coma_))
            {
                tmp_node->right = new_node(tmp_token);
                tmp_node->right->left = expr();
                tmp_node = tmp_node->right;
            }
            expect(rparent_);
            return node;
        }
#endif
        char *name = token->name;
        token = get_var(name);
        if (!token)
            error("Undeclared variable '%s'", name);
        node = new_node(token);
    }
    else if (token = check(lparent_, 0))
    {
        // expect(lparent_);
        node = expr();
        expect(rparent_);
    }
    else if (token = check(char_, int_, float_, eof_, 0))
        node = new_node(token);
    else
        error("found %s in prime", token ? type_to_string(token->type) : "(null)");
    return node;
}

void initialize()
{
    // TODO: verify all label then set start label
    // write the assembly
    print_asm(".section	.note.GNU-stack,\"\",@progbits\n");
    print_asm(".intel_syntax noprefix\n");
    print_asm(".include \"import/header.s\"\n\n");
    print_asm(".text\n");
    print_asm(".globl	main\n\n");
    print_asm("main:\n");
    print_asm("   push    rbp\n");
    print_asm("   mov     rbp, rsp\n");
    print_asm("   sub     rsp, %zu\n", rsp);

    Node *curr = NULL;
    while (tokens[exe_pos]->type != eof_)
    {
        curr = expr();
        print_node(curr, 0);
        evaluate(curr);
        free_node(curr);
    }
}

void finalize()
{
    // TODO: check exit status if changed
#if 0
    print_asm("   mov     rax, 0\n");
#else
    print_asm("\n");
#endif
    print_asm("   leave\n");
    print_asm("   ret\n\n");
    for (int i = 0; i < tk_pos; i++)
    {
        // test char variable before making any modification
        if (!tokens[i]->name && tokens[i]->index_ && tokens[i]->type == char_)
            print_asm("STR%zu: .string \"%s\"\n", tokens[i]->index_, tokens[i]->char_);
        if (!tokens[i]->name && tokens[i]->index_ && tokens[i]->type == float_)
            print_asm("FLT%zu: .long %zu /* %f */\n", tokens[i]->index_, *((float *)(&tokens[i]->float_)), tokens[i]->float_);
    }
}

Token *get_var(char *name)
{
    for (int i = 0; i < var_pos; i++)
        if (variables[i]->name && strcmp(variables[i]->name, name) == 0)
            return variables[i];
    return NULL;
}

Token *evaluate(Node *node)
{
    Token *left = NULL, *right = NULL;
    Type type = node->token->type;
    switch (type)
    {
    case identifier_:
    case char_:
    case float_:
    case int_:
#if 0
    case bool_:
#endif
        break;
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
            Node *curr = new_node(new_token(0, 0, mul_, none_));
            curr->left = new_node(left);
            if (left->type == int_)
            {
                curr->right = new_node(new_token(0, 0, int_, none_));
                curr->right->token->int_ = -1;
            }
            else if (left->type == float_)
            {
                curr->right = new_node(new_token(0, 0, float_, none_));
                float f = -1.0;
                curr->right->token->float_ = *(uint32_t *)(&f);
                curr->right->token->index_ = index_++;
            }
            else
                error("Invalid unary operation 1");
            print_node(curr, 0);
            node->token = evaluate(curr);
            free_node(curr);
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
        if (!left->name || left->type != right->type)
            error("Invalid assignement");
        node->token = left;
        print_asm("   /* assign to %s */\n", left->name);
        switch (left->type)
        {
        case int_:
            if (right->ptr)
            {
                print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", left->ptr);
            }
            else
                print_asm("   mov     QWORD PTR -%zu[rbp], %d\n", left->ptr, right->int_);
            break;
        case float_:
            // TODO: check xmms, with multiple variables
            if (right->ptr)
            {
                print_asm("   movss   xmm1, DWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1\n", left->ptr);
            }
            else
            {
                print_asm("   movss   xmm1, DWORD PTR FLT%zu[rip]\n", right->index_);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1\n", left->ptr);
            }
            break;
        case char_:
            // TODO: check if it has ptr, then load value from there
            print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
            print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", left->ptr);
            break;
#if 0
        case bool_:
            if (right->ptr)
            {
                // TODO: test this one
                print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", left->ptr);
            }
            else
                print_asm("   movzx  BYTE PTR %zu[rbp], %d", left->ptr, right->bool_);
            break;
#endif
        default:
            error("add assembly for this one 0");
            break;
        }
        break;
    }
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
        if (!left->ptr && !right->ptr)
        {
            debug("0. do %s between %k with %k\n", type_to_string(type), left, right);
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
            // TODO: handle strings that get concatinated in run time
            case char_:
                node->token->index_ = index_++;
                node->token->sub_type = dyn_;
                if (type == add_)
                    node->token->char_ = strjoin(left->char_, right->char_);
                else
                    error("invalid operation for characters");
                break;
            default:
                error("math operation 0");
                break;
            }
        }
        else
        {
            // TODO: addition for dynamic strings
            debug("1. do %s between %k with %k\n", type_to_string(type), left, right);
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
                str = type == add_   ? "add     rax, "
                      : type == sub_ ? "sub     rax, "
                      : type == mul_ ? "imul    rax, "
                      : type == div_ ? "cdq\n   mov     rbx, "
                                     : NULL;
                print_asm("   %s", str);
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
                str = type == add_   ? "addss   xmm1, "
                      : type == sub_ ? "subss   xmm1, "
                      : type == mul_ ? "mulss   xmm1, "
                      : type == div_ ? "divss   xmm1, "
                                     : NULL;
                print_asm("   %s", str);
                if (right->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                else
                    print_asm("%zu\n", right->float_);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1\n", node->token->ptr);
                break;
            case char_:
                // allocate in heap and copy both strings

                break;
            default:
                error("math operation 1");
                break;
            }
        }
        break;
    }
    case equal_:
    case less_than_:
    case more_than_:
    case less_than_equal_:
    case more_than_equal_:
    {

        break;
    }
    case func_call_:
    {
        debug("found function call has name '%s'\n", node->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            debug("found output\n");
            Node *tmp = node;
            while (tmp)
            {
                output(evaluate(tmp->left));
                // debug("%k\n",tmp->token);
                // printf("loop\n");
                tmp = tmp->right;
            }
            // exit(0);
        }
        break;
    }
    default:
        error("in evaluate %s", type_to_string(type));
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
    if (argc != 2)
    {
        // TODO: check if file ends with .hr
        error("require one file.hr as argument\n");
    }
#if 1
    file = fopen(argv[1], "r");
#else
    file = fopen("file.hr", "r");
#endif

#if 0
    char *filename = strdup(argv[1]);
    filename[strlen(filename) - 2] = 's';
    filename[strlen(filename) - 1] = 0;
    
    // opening file
    debug("open %s\n", argv[1]);
    debug("write to %s\n", filename);
    asm_fd = fopen(filename, "w");
    free(filename);
#else
    asm_fd = fopen("file.s", "w");
#endif
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
    Label = 1;
    index_ = 1;
    tk_len = var_len = 100;
    tokens = calloc(tk_len, sizeof(Token *));
    variables = calloc(tk_len, sizeof(Token *));
    build_tokens();
    debug("\n");
    free(text);
    // TODO: verify rsp position
    rsp = 30;
    initialize();
    finalize();
    debug("\n");
}