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
typedef struct StrToken StrToken;

// stupid implicit declaration error
void error(char *msg);
Token *get_var(char *name);
Token *evaluate(Node *node);

enum Type
{
    eof_ = 11,
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

struct StrToken
{
    Type type;
    char *name;
};

StrToken DataTypes[] = {
    // Data types
    {char_, "char"},
    {int_, "int"},
    {float_, "float"},
    {0, 0}};

StrToken Symbols[] = {
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
    {0, 0}};

StrToken Random[] = {
    {eof_, "EOF"},
    {identifier_, "identifier"},
    // function
    {func_dec_, "function declaration"},
    {func_call_, "function call"},
    {0, 0}};

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
    error("error unkown type\n");
    return NULL;
}

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

// file
FILE *file;
char *text;
size_t txt_len;
int txt_pos;

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

// assembly
int asm_fd;
size_t rsp;
// size_t Label;
bool BuiltIns[100];

char *strjoin(char *left, char *right)
{
    char *res = calloc(strlen(left) + strlen(right) + 1, sizeof(char));
    strcpy(res, left);
    strcpy(res + strlen(res), right);
    return res;
}

// debug
void error(char *msg)
{
    // TODO: free memory before exiting
    dprintf(2, "\033[0;31mError: %s\033[0m\n", msg);
    exit(1);
}

void debug(char *conv, ...)
{
#if DEBUG
    size_t len = strlen(conv);
    size_t i = 0;

    va_list args;
    va_start(args, conv);
    int fd = 1;
    while (i < len)
    {
        if (conv[i] == '%')
        {
            i++;
            switch (conv[i])
            {
            case 'c':
                dprintf(fd, "%c", va_arg(args, int));
                break;
            case 's':
                dprintf(fd, "%s", va_arg(args, char *));
                break;
            case 'p':
                dprintf(fd, "%p", (size_t)(va_arg(args, void *)));
                break;
            case 'x':
                dprintf(fd, "%x", (size_t)va_arg(args, void *));
                break;
            case 'X':
                dprintf(fd, "%X", (size_t)va_arg(args, void *));
                break;
            case 'd':
                dprintf(fd, "%d", (int)va_arg(args, int));
                break;
            case 'f':
                dprintf(fd, "%f", va_arg(args, double));
                break;
            case '%':
                dprintf(fd, "%%");
                break;
            case 'k':
            {
                Token *token = (Token *)va_arg(args, Token *);
                if (token)
                {
                    dprintf(fd, "type: %s, ", type_to_string(token->type));
                    if (token->name)
                        dprintf(fd, "name: %s, ", token->name);
                    switch (token->type)
                    {
                    case char_:
                        dprintf(fd, "value: %s, LC: %zu, ", token->char_, token->index_);
                        break;
                    case int_:
                        dprintf(fd, "value: %d, ", token->int_);
                        break;
                    case float_:
                        dprintf(fd, "value: %zu (%f), ", token->float_, *(float *)(&token->float_));
                        break;
                    default:
                        // ft_putstr(fd, "Unkown");
                        break;
                    }
                }
                else
                    dprintf(fd, "(null)");
                break;
            }
            default:
                error("in debug function");
                break;
            }
        }
        else
            dprintf(fd, "%c", conv[i]);
        i++;
    }
#endif
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
        dprintf(asm_fd, "   lea     rax, STR%zu[rip]\n", token->index_);
        dprintf(asm_fd, "   mov QWORD PTR -8[rbp], rax\n");
        dprintf(asm_fd, "   mov rbx, rax\n");
        dprintf(asm_fd, "   call printstring\n");
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

// Token
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
    if (tk_pos + 10 > tk_len)
    {
        Token **tmp = calloc(tk_len * 2, sizeof(Token *));
        memcpy(tmp, tokens, tk_len * sizeof(Token *));
        free(tokens);
        tokens = tmp;
        tk_len *= 2;
    }
    tokens[tk_pos] = calloc(1, sizeof(Token));
    tokens[tk_pos]->type = type;
    switch (type)
    {
    case eof_:
    case add_:
    case sub_:
    case mul_:
    case div_:
    case lparent_:
    case rparent_:
    case assign_:
        break;
    case identifier_:
        tokens[tk_pos]->name = calloc(e - s + 1, sizeof(char));
        strncpy(tokens[tk_pos]->name, text + s, e - s);
        break;
    case char_:
        index_++;
        tokens[tk_pos]->index_ = index_;
        tokens[tk_pos]->char_ = calloc(e - s + 1, sizeof(char));
        strncpy(tokens[tk_pos]->char_, text + s, e - s);
        break;
    case int_:
        while (s < e)
            tokens[tk_pos]->int_ = 10 * tokens[tk_pos]->int_ + text[s++] - '0';
        break;
    case float_:
        float f = 0.0;
        index_++;
        tokens[tk_pos]->index_ = index_;
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

        tokens[tk_pos]->float_ = *(uint32_t *)(&f);
        break;
    default:
        break;
    }
    debug("new token %k\n", tokens[tk_pos]);
    return tokens[tk_pos++];
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
    int start;

    // Expect main label
    while (isspace(text[txt_pos]))
        txt_pos++;
    if (strncmp(&text[txt_pos], "main:\n", strlen("main:\n")))
        error("main label with new line is required\n");
    txt_pos += strlen("main:\n");
    while (text[txt_pos])
    {
        token = NULL;
        start = txt_pos;
        if (isspace(text[txt_pos]))
        {
            txt_pos++;
            continue;
        }
        // TODO: protect it from segfault
        if (strncmp(text + txt_pos, "/*", 2) == 0)
        {
            txt_pos += 2;
            while (text[txt_pos + 1] && strncmp(text + txt_pos, "*/", 2))
                txt_pos++;
            if (!text[txt_pos + 1])
                error("Expected end of comment");
            txt_pos += 2;
            continue;
        }
        for (int i = 0; Symbols[i].name; i++)
        {
            if (strncmp(Symbols[i].name, text + txt_pos, strlen(Symbols[i].name)) == 0)
            {
                token = new_token(txt_pos, txt_pos + strlen(Symbols[i].name), Symbols[i].type);
                txt_pos += strlen(Symbols[i].name);
                break;
            }
        }
        if (token && token->type)
        {
            token = NULL;
            continue;
        }
        while (isalpha(text[txt_pos]))
            txt_pos++;
        if (txt_pos > start)
        {
            new_token(start, txt_pos, identifier_);
            continue;
        }
        if (isdigit(text[txt_pos]))
        {
            Type type = int_;
            while (isdigit(text[txt_pos]))
                txt_pos++;
            if (text[txt_pos] == '.')
            {
                type = float_;
                txt_pos++;
            }
            while (isdigit(text[txt_pos]))
                txt_pos++;
            if (txt_pos > start)
            {
                new_token(start, txt_pos, type);
                continue;
            }
        }
        if (strchr("\"\'", text[txt_pos]))
        {
            char quote = text[txt_pos++];
            while (text[txt_pos] && text[txt_pos] != quote)
                txt_pos++;
            if (text[txt_pos++] != quote)
                error("Syntax");
            new_token(start + 1, txt_pos - 1, char_);
            continue;
        }
        error("tokenizing");
    }
    new_token(0, 0, eof_);
}

// build tree
void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        // free_token(node->token);
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
Node *prime();

void skip(Type type)
{
    if (tokens[tk_pos]->type != type)
        error("in skip");
    tk_pos++;
}

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

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = add_sub();
    if (check(tokens[tk_pos]->type, assign_, 0))
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = assign();
        return node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    // Node *tmp = left;
    if (check(tokens[tk_pos]->type, add_, sub_, 0))
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = add_sub();
        return node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    if (check(tokens[tk_pos]->type, mul_, div_, 0))
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = mul_div();
        return node;
    }
    return left;
}

Node *prime()
{
    Node *node = NULL;
    Token *token = tokens[tk_pos];
    switch (token->type)
    {
    case identifier_:
    {
        // TODO: check if left name is also data type name
        token = tokens[tk_pos++];
        for (int i = 0; DataTypes[i].name; i++)
        {
            if (strcmp(DataTypes[i].name, token->name) == 0)
            {
                Type type = DataTypes[i].type;
                if (tokens[tk_pos]->type != identifier_)
                    error("Expected identifier after data type");
                node = new_node(tokens[tk_pos++]);
                node->token->type = type;
                if (get_var(node->token->name))
                    error("redefinition of variable");
                new_variable(node->token);
                return node;
            }
        }
        if (token->type == lparent_)
        {
            // TODO: get function
            node->token->type = func_call_;
            skip(lparent_);
            node->left = expr();
            skip(rparent_);
        }
        token = get_var(token->name);
        if (!token)
            error("Undeclared variable");
        node = new_node(token);

        return node;
    }
    case lparent_:
    {
        skip(lparent_);
        node = expr();
        skip(rparent_);
        return node;
    }
    case char_:
    case int_:
    case float_:
    case eof_:
    {
        return new_node(tokens[tk_pos++]);
    }
    default:
        printf("type: %s\n", type_to_string(token->type));
        error("in prime");
        break;
    }
    return node;
}

Node *initialize()
{
    // Label = 2; // label from where to start, TODO: verify all label then set start label
    // write the assembly
    dprintf(asm_fd, ".section	.note.GNU-stack,\"\",@progbits\n");
    dprintf(asm_fd, ".intel_syntax noprefix\n");
    dprintf(asm_fd, ".text\n");
    dprintf(asm_fd, ".globl	main\n\n");
    dprintf(asm_fd, "main:\n");
    dprintf(asm_fd, "   push    rbp\n");
    dprintf(asm_fd, "   mov     rbp, rsp\n");
    dprintf(asm_fd, "   sub     rsp, %zu\n", rsp);
    Node *node = new_node(NULL);
    Node *curr = node;

    while (tokens[tk_pos]->type != eof_)
    {
        curr->left = expr();
        print_node(curr->left, 0);
        evaluate(curr->left);
        curr->right = new_node(NULL);
        curr = curr->right;
    }
    return node;
}

void finalize(Node *node)
{
    // TODO: check exit status if changed
    dprintf(asm_fd, "   mov     rax, 0\n");
    dprintf(asm_fd, "   leave\n");
    dprintf(asm_fd, "   ret\n\n");
    for (int i = 0; i < tk_pos; i++)
    {
        // test char variable before making any modification
        if (!tokens[i]->name && tokens[i]->index_ && tokens[i]->type == char_)
            dprintf(asm_fd, "STR%zu:\n   .string    \"%s\"\n", tokens[i]->index_, tokens[i]->char_);
        if (!tokens[i]->name && tokens[i]->index_ && tokens[i]->type == float_)
            dprintf(asm_fd, "FLT%zu:\n   .long  %zu\n", tokens[i]->index_, tokens[i]->float_);
    }
    if (BuiltIns[printstring_])
    {
        dprintf(asm_fd, "printstring:\n");
        dprintf(asm_fd, "   push rbp\n");
        dprintf(asm_fd, "   mov rbp, rsp\n");
        dprintf(asm_fd, "   /* char *str */\n");
        dprintf(asm_fd, "   mov QWORD PTR -8[rbp], rbx\n");
        dprintf(asm_fd, "   /* fd */\n");
        dprintf(asm_fd, "   mov rdi, 1\n");
        dprintf(asm_fd, "   /* pointer */\n");
        dprintf(asm_fd, "   mov rsi, rbx\n");
        dprintf(asm_fd, "   call ft_strlen\n");
        dprintf(asm_fd, "   /* lenght */\n");
        dprintf(asm_fd, "   mov rdx, rax\n");
        dprintf(asm_fd, "   call write@PLT\n");
        dprintf(asm_fd, "   mov rsp, rbp\n");
        dprintf(asm_fd, "   pop rbp\n");
        dprintf(asm_fd, "   ret\n\n");
    }
    if (BuiltIns[length_])
    {
        dprintf(asm_fd, "length:\n");
        dprintf(asm_fd, "   push rbp\n");
        dprintf(asm_fd, "   mov rbp, rsp\n");
        dprintf(asm_fd, "   /* char *str */\n");
        dprintf(asm_fd, "   mov QWORD PTR -8[rbp], rbx\n");
        dprintf(asm_fd, "   /* size_t i = 0 */\n");
        dprintf(asm_fd, "   mov QWORD PTR -16[rbp], 0\n");
        dprintf(asm_fd, "   jmp .L2\n");
        dprintf(asm_fd, ".L3:\n");
        dprintf(asm_fd, "   /* i++ */\n");
        dprintf(asm_fd, "   add QWORD PTR -16[rbp], 1\n");
        dprintf(asm_fd, ".L2:\n");
        dprintf(asm_fd, "   mov rax, QWORD PTR -8[rbp]\n");
        dprintf(asm_fd, "   mov rdx, QWORD PTR -16[rbp]\n");
        dprintf(asm_fd, "   /* str + i */\n");
        dprintf(asm_fd, "   add rax, rdx\n");
        dprintf(asm_fd, "   movzx rax, BYTE PTR[rax]\n");
        dprintf(asm_fd, "   cmp rax, 0\n");
        dprintf(asm_fd, "   jne .L3\n");
        dprintf(asm_fd, "   /* return i */\n");
        dprintf(asm_fd, "   mov rax, QWORD PTR -16[rbp]\n");
        dprintf(asm_fd, "   mov rsp, rbp\n");
        dprintf(asm_fd, "   pop rbp\n");
        dprintf(asm_fd, "   ret\n\n");
    }

    // clear everything
    while (node)
    {
        Node *tmp = node->right;
        free_node(node->left);
        free(node);
        node = tmp;
    }
    // free_node(curr);
    close(asm_fd);
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
    Token *left, *right, *to_find;
    Type type = node->token->type;
    switch (type)
    {
    case identifier_:
    case char_:
    case float_:
    case int_:
        break;
    case assign_:
    {
        // TODO: assign / initializing
        // TODO: deep / shalow copy
        left = evaluate(node->left);
        dprintf(asm_fd, "   /* assign %s */\n", left->name);
        right = evaluate(node->right);
        debug("assign %k and %k \n", left, right);
        if (!left->name || left->type != right->type)
            error("Invalid assignement");
        node->token = left;
        switch (left->type)
        {
        case int_:
            dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], ", left->ptr);
            if (right->ptr)
                dprintf(asm_fd, "QWORD PTR -%zu[rbp]\n", right->ptr);
            else
                dprintf(asm_fd, "%d\n", right->int_);
            break;
#if 0
            case float_:
                // TODO: check xmms, with multiple variables
                dprintf(asm_fd, "   movss   xmm1, DWORD PTR .FLT%zu[rip]\n", right->index_);
                dprintf(asm_fd, "   movss   DWORD PTR -%zu[rbp], xmm1\n", to_find->ptr);
                break;
            case char_:
                dprintf(asm_fd, "   lea     rax, .STR%zu[rip]\n", right->index_);
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], rax\n", to_find->ptr);
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
        /*
            allocate in stack
            add left value
            and right value
        */
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
                // node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
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
                node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
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
                node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
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
            switch (node->token->type)
            {
            case int_:
                node->token->ptr = (ptr += 4);
                // set left
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], ", node->token->ptr);
                if (left->ptr)
                    dprintf(asm_fd, "QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    dprintf(asm_fd, "%d\n", left->int_);

                // set right
                char *str = type == add_   ? "add"
                            : type == sub_ ? "sub"
                            : type == mul_ ? "mul"
                                           : NULL;
                dprintf(asm_fd, "   %s     QWORD PTR -%zu[rbp], ", str, node->token->ptr);
                if (right->ptr)
                    dprintf(asm_fd, "QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    dprintf(asm_fd, "%d\n", right->int_);
                break;
            case float_:
                
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
        break;
    }
    if (ptr + 10 > rsp)
    {
        // TODO: protect this line from being printed in wrong place
        // after label for example
        rsp += 30;
        dprintf(asm_fd, "   sub     rsp, 30\n");
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
    asm_fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0777);
    free(filename);
#else
    asm_fd = open("file.s", O_CREAT | O_TRUNC | O_RDWR, 0777);
#endif
    if (file == NULL || asm_fd < 0)
        error("Opening file");
    fseek(file, 0, SEEK_END);
    txt_len = ftell(file);
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
    tk_pos = 0;
    // TODO: verify rsp position
    rsp = 30;
    Node *node = initialize();
    finalize(node);
    debug("\n");
}