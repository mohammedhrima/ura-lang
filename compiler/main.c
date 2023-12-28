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
    neg_,
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
    Node *left;
    Node *right;
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
    {neg_, "negative"},
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
                        if (token->index_)
                            fprintf(stdout, "in FLT%zu, ", token->index_);
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

// negative
// Token *minus_int;
// Token *minus_float;

// variables
Token **variables;
int var_len;
int var_pos;
uintptr_t ptr;
size_t index_;

bool BuiltIns[100];

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
    switch (token->type)
    {
    case char_:
    {
        BuiltIns[length_] = true;
        BuiltIns[printstring_] = true;
        debug("found %s, load string from STR%zu  \n", type_to_string(token->type), token->index_);
        print_asm("   lea     rax, STR%zu[rip]\n", token->index_);
        print_asm("   mov QWORD PTR -8[rbp], rax\n");
        print_asm("   mov rbx, rax\n");
        print_asm("   call printstring\n");
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
            new_token(s, e, type);
            continue;
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

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    return new;
}

/*

main:
    int x
    int y
    x = 10
    y = 5 + x + 2 + 1
    x = x + y

main:
    int x = 10
    int y
    x = 10
    y = 5 + x + 2 +1
    x = x + 5

main:
    int x = 10
    int y
    x = 10
    y = 5 + x + 2 +1
    x = x + 5

main:
    int x
    int y
    x = 10
    y = 5 + x + 2 +1
    x = x + y

*/

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
        Node *node = new_node(new_token(0, 0, neg_));
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
                debug("find %s\n", type_to_string(DataTypes[i].type));

                node = new_node(expect(identifier_, 0));
                node->token->type = type;
                if (get_var(node->token->name))
                    error("redefinition of variable");
                new_variable(node->token);
                return node;
            }
        }
#if 0
        if (token->type == lparent_)
        {
            // TODO: get function
            node->token->type = func_call_;
            expect(lparent_);
            node->left = expr();
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
    // Label = 2; // label from where to start, TODO: verify all label then set start label
    // write the assembly
    print_asm(".section	.note.GNU-stack,\"\",@progbits\n");
    print_asm(".intel_syntax noprefix\n");
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
            print_asm("STR%zu:\n   .string    \"%s\"\n", tokens[i]->index_, tokens[i]->char_);
        if (!tokens[i]->name && tokens[i]->index_ && tokens[i]->type == float_)
            print_asm("FLT%zu:/* %f */\n   .long  %zu\n", tokens[i]->index_, *((float *)(&tokens[i]->float_)), tokens[i]->float_);
    }
    if (BuiltIns[printstring_])
    {
        print_asm("printstring:\n");
        print_asm("   push rbp\n");
        print_asm("   mov rbp, rsp\n");
        print_asm("   /* char *str */\n");
        print_asm("   mov QWORD PTR -8[rbp], rbx\n");
        print_asm("   /* fd */\n");
        print_asm("   mov rdi, 1\n");
        print_asm("   /* pointer */\n");
        print_asm("   mov rsi, rbx\n");
        print_asm("   call ft_strlen\n");
        print_asm("   /* lenght */\n");
        print_asm("   mov rdx, rax\n");
        print_asm("   call write@PLT\n");
        print_asm("   mov rsp, rbp\n");
        print_asm("   pop rbp\n");
        print_asm("   ret\n\n");
    }
    if (BuiltIns[length_])
    {
        print_asm("length:\n");
        print_asm("   push rbp\n");
        print_asm("   mov rbp, rsp\n");
        print_asm("   /* char *str */\n");
        print_asm("   mov QWORD PTR -8[rbp], rbx\n");
        print_asm("   /* size_t i = 0 */\n");
        print_asm("   mov QWORD PTR -16[rbp], 0\n");
        print_asm("   jmp .L2\n");
        print_asm(".L3:\n");
        print_asm("   /* i++ */\n");
        print_asm("   add QWORD PTR -16[rbp], 1\n");
        print_asm(".L2:\n");
        print_asm("   mov rax, QWORD PTR -8[rbp]\n");
        print_asm("   mov rdx, QWORD PTR -16[rbp]\n");
        print_asm("   /* str + i */\n");
        print_asm("   add rax, rdx\n");
        print_asm("   movzx rax, BYTE PTR[rax]\n");
        print_asm("   cmp rax, 0\n");
        print_asm("   jne .L3\n");
        print_asm("   /* return i */\n");
        print_asm("   mov rax, QWORD PTR -16[rbp]\n");
        print_asm("   mov rsp, rbp\n");
        print_asm("   pop rbp\n");
        print_asm("   ret\n\n");
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
        break;
    case neg_:
    {
        // TODO: negative float has a different behaviou !!!
        left = evaluate(node->left);
        if (left->type != int_ && left->type != float_)
            error("Invalid unary operation 0");

        if (!left->name)
        {
            node->token->type = left->type;
            switch (left->type)
            {
            case int_:
                // TODO: prtect int_MIN
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
            Node *curr = new_node(new_token(0, 0, mul_));
            curr->left = new_node(left);
            if (left->type == int_)
            {
                curr->right = new_node(new_token(0, 0, int_));
                curr->right->token->int_ = -1;
            }
            else if (left->type == float_)
            {
                curr->right = new_node(new_token(0, 0, float_));
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
        // TODO: assign / initializing
        // TODO: deep / shallow copy
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
#if 0
            case char_:
                print_asm( "   lea     rax, .STR%zu[rip]\n", right->index_);
                print_asm( "   mov     QWORD PTR -%zu[rbp], rax\n", to_find->ptr);
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
                left->index_ = 0;
                right->index_ = 0;
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
            default:
                error("math operation 1");
                break;
            }
        }
        break;
    }
    case func_call_:
    {
        debug("found function call has name '%s'\n", node->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            debug("found output\n");
            output(evaluate(node->left));
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