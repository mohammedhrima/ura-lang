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
typedef struct Token Token;
typedef struct Node Node;
typedef enum Type Type;
#define DEBUG 1

void error(char *msg);
Token *get_var(char *name);

enum Type
{
    eof_ = 11,
    // operators
    add_,
    sub_,
    mul_,
    div_,
    lparent_,
    rparent_,
    assign_,
    // vars
    data_type_,
    char_,
    int_,
    float_,
    void_,
    // function
    func_dec_,
    func_call_,
};

char *type_to_string(Type type)
{
    struct
    {
        Type type;
        char *name;
    } Types[] = {
        {eof_, "EOF"},
        {add_, "+"},
        {sub_, "-"},
        {mul_, "*"},
        {div_, "/"},
        {lparent_, "("},
        {rparent_, ")"},
        {assign_, "="},
        {data_type_, "data_type_"},
        {char_, "char_"},
        {int_, "int_"},
        {float_, "float_"},
        {void_, "void_"},
        {func_dec_, "function declaration"},
        {func_call_, "function call"},
        {0, 0}};

    for (int i = 0; Types[i].name; i++)
        if (Types[i].type == type)
            return Types[i].name;
    // printf("-> %d\n", type);
    error("error unkown type\n");
    return NULL;
}

enum BuiltIn_enum
{
    // built in
    strlen_ = 0,
    putstr_,
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
        // TODO: protect from overflow
        int int_;
        uint32_t float_;
    };
};

struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

// globals
int asm_fd;
FILE *fp;
size_t size;
char *text;
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

// assembly fd
int asm_fd;
size_t rsp;
size_t Label;

// data types size
size_t num_bit_size;
size_t char_bit_size;
size_t float_bit_pres;

Type types[255] = {
    ['+'] = add_,
    ['-'] = sub_,
    ['*'] = mul_,
    ['/'] = div_,
    ['('] = lparent_,
    [')'] = rparent_,
    ['='] = assign_,
};

// struct
// {
//     char *string;
//     Type type;
// } DataTypes[] = {
//     {"int ", int_},
//     {"char ", char_},
// };

// built-ins assembly
bool BuiltIns[100];

char *strjoin(char *left, char *right)
{
    char *res = calloc(strlen(left) + strlen(right) + 1, sizeof(char));
    strcpy(res, left);
    strcpy(res + strlen(res), right);
    return res;
}

// built in functions
void error(char *msg)
{
    // TODO: free memory before exiting
    dprintf(2, "\033[0;31mError: %s\033[0m\n", msg);
    exit(1);
}
void debug(char *conv, ...)
{
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
                dprintf(fd, "%x", (size_t)va_arg(args, void *));
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
                        dprintf(fd, "value: %zu, ", token->float_);
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
            dprintf(fd,"%c", conv[i]);
        i++;
    }
}

void output(Token *token)
{
    switch (token->type)
    {
    case char_:
    {
        BuiltIns[strlen_] = true;
        BuiltIns[putstr_] = true;
        printf("found %s, load string from STR%zu  \n", type_to_string(token->type), token->index_);
        dprintf(asm_fd, "   lea     rax, STR%zu[rip]\n", token->index_);
        dprintf(asm_fd, "   mov QWORD PTR -8[rbp], rax\n");
        dprintf(asm_fd, "   mov rbx, rax\n");
        dprintf(asm_fd, "   call ft_putstr\n");
        break;
    }
    case int_:
    case void_:
        break;
        break;
    default:
        error("Error in output unknown type");
        break;
    }
}

void free_token(Token *token)
{
    printf("free token has type %s\n", type_to_string(token->type));
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
    char *value = NULL;
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
    case data_type_:
    case void_:
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
        index_++;
        tokens[tk_pos]->index_ = index_;
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

// build tokens
void build_tokens()
{
    Token *token;
    int start = 0;
    char *DataTypes[] = {
        "char ",
        "int ",
        "float ",
        NULL};

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
        for (int i = 0; DataTypes[i]; i++)
        {
            if (strncmp(DataTypes[i], text + txt_pos, strlen(DataTypes[i])) == 0)
            {
                // TODO: verfy this error message
                if (tk_pos > 1 && tokens[tk_pos - 1]->type == data_type_)
                    error("Unexpected data type after data type");
                token = new_token(txt_pos, txt_pos + strlen(DataTypes[i]) - 1, data_type_);
                txt_pos += strlen(DataTypes[i]) - 2;
                while (isspace(text[txt_pos]))
                    txt_pos++;
                if (!isalpha(text[txt_pos++]))
                    error("Expected identifier after data type");
            }
        }
        if (token && token->type)
        {
            token = NULL;
            continue;
        }
        if (strchr("=+/*-()", text[txt_pos]))
        {
            new_token(0, 0, types[text[txt_pos++]]);
            continue;
        }
        while (isalpha(text[txt_pos]))
            txt_pos++;
        if (txt_pos > start)
        {
            new_token(start, txt_pos, void_);
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
        free_token(node->token);
        free(node);
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
// Node *parents();
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
    while (check(tokens[tk_pos]->type, assign_, 0))
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = add_sub();
        // printf("found %s\n", type_to_string(node->token->type));
        // printf("    left : %s, %s\n", type_to_string(node->left->token->type), node->left->token->name);
        // printf("    right: %s, %ld\n", type_to_string(node->right->token->type), node->right->token->int_);
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    while (check(tokens[tk_pos]->type, add_, sub_, 0))
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = mul_div();
        left = node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = prime();
    while (check(tokens[tk_pos]->type, mul_, div_, 0))
    {
        Node *node = new_node(tokens[tk_pos++]);
        node->left = left;
        node->right = prime();
        left = node;
    }
    return left;
}

Node *prime()
{
    // printf("call prime\n");
    switch (tokens[tk_pos]->type)
    {
    case char_:
    case int_:
    case float_:
    case eof_:
    {
        return new_node(tokens[tk_pos++]);
    }
    case data_type_:
    {
        // TODO: check if variable already exists
        Type type;
        if (strcmp(tokens[tk_pos]->name, "int") == 0)
            type = int_;
        else if (strcmp(tokens[tk_pos]->name, "float") == 0)
            type = float_;
        else if (strcmp(tokens[tk_pos]->name, "char") == 0)
            type = char_;
        else
            error("in prime in data type");
        tk_pos++;
        Node *node = prime();
        if (get_var(node->token->name))
            error("Variable already exists");
        node->token->type = type;
        new_variable(node->token);
        return node;
    }
    case void_:
    {
        Node *node = new_node(tokens[tk_pos++]);
        if (tokens[tk_pos]->type == lparent_)
        {
            node->token->type = func_call_;
            skip(lparent_);
            /*
                add each param on left
                and keep linking list from right
            */
            Node *tmp = node;
            tmp->left = expr();
            skip(rparent_);
            return node;
        }
        return node;
    }
    case lparent_:
    {
        skip(lparent_);
        Node *node = expr();
        skip(rparent_);
        return node;
    }
    default:
        error("in prime");
        break;
    }
    return NULL;
}

Token *get_var(char *name)
{
    // printf("there is %d variables: ", var_pos);
    // for(int i = 0; i < var_pos; i++)
    //     printf("%s ", VARIABLES[i]->name);
    // printf("\n");
    // printf("%s\n", VARIABLES[0]->name);
    if (name == NULL)
        error("in get_var");
    for (int i = 0; i < var_pos; i++)
    {
        // printf("get_var %d\n", i);
        if (
            variables[i]->name &&
            strcmp(variables[i]->name, name) == 0)
        {
            // printf("find variable %s\n", name);
            return variables[i];
        }
    }
    return NULL;
}

Token *evaluate(Node *node)
{
    Token *to_find = NULL;
    Token *left = NULL;
    Token *right = NULL;
    Type type = node->token->type;
    switch (type)
    {
    case void_:
    case char_:
    case int_:
    case float_:
        return node->token;
    case assign_:
        /*
            - always left should be a variable
            - check if variable is does exists
        */
        // TODO: split assignement and initializing
        // TODO: deep copy and shalow copy
        // check the compatibility
        printf("%s\n", type_to_string(node->left->token->type));
        left = evaluate(node->left);
        right = evaluate(node->right);
        // assert(0);
        // if(!left)
        //     error("assignement, left is null");
        debug("assign %k with %k\n", left, right);

        if (!left->name || left->type != right->type)
            error("Invalid assignment");
        if (left->type == void_ && !(to_find = get_var(left->name)))
            error("Undeclared variable");
        else
            to_find = left;
        if (to_find->type == float_)
        {
            ptr += 4;
            to_find->ptr = ptr;
            to_find->float_ = right->float_;
            to_find->index_ = right->index_;
            printf("new variable has name %s and value %zu in\n", to_find->name, to_find->float_);
        }
        else if (to_find->type == int_)
        {
            ptr += 4;
            to_find->ptr = ptr;
            to_find->int_ = right->int_;
            printf("new variable has name %s and value %d\n", to_find->name, to_find->int_);
        }
        else if (left->type == char_)
        {
            ptr += 8;
            to_find->ptr = ptr;
            to_find->index_ = right->index_;
            printf("variable has name %s and value in .STR%zu\n", to_find->name, right->index_);
        }
        else if (!to_find)
            error("Undeclared variable");
#if 0
        for (int i = 0; i < var_pos; i++)
            printf("%s\n", VARIABLES[i]->name);
            // exit(0);
#endif
        switch (to_find->type)
        {
        case int_:
            // printf("%ld", node->right->token->int_);
            dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], %ld\n", to_find->ptr, to_find->int_);
            break;
        case float_:
            // TODO: check xmms
            dprintf(asm_fd, "   movss   xmm1, DWORD PTR .FLT%zu[rip]\n", to_find->index_);
            dprintf(asm_fd, "   movss   DWORD PTR -%zu[rbp], xmm1\n", to_find->ptr);
            break;
        case char_:
            dprintf(asm_fd, "   lea     rax, .STR%zu[rip]\n", right->index_);
            dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], rax\n", to_find->ptr);
            break;
        default:
            error("add assembly for this one");
            break;
        }
        break;
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        Token *left = evaluate(node->left);
        Token *right = evaluate(node->right);
        if (!left->name && !right->name)
        {
            /*
                TODO: there could be no declaed variable
                in case of sending param to function
            */
            if (left->type == void_ && !(left = get_var(left->name)))
                error("Undeclared variable, left");
            if (right->type == void_ && !(right = get_var(right->name)))
                error("Undeclared variable, right");
            debug("do %s between %k with %k\n", type_to_string(node->token->type), left, right);

            if (left->type == right->type)
            {
                node->token->type = left->type;
                switch (left->type)
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
                    node->token->type = float_;
                    node->token->float_ = *(uint32_t *)(&res);
                    node->token->index_ = index_++;
                    break;
                case char_:
                    left->index_ = 0;
                    right->index_ = 0;
                    if (type == add_)
                        node->token->char_ = strjoin(left->char_, right->char_);
                    else
                        error("invalid operation for characters");
                    node->token->type = char_;
                    node->token->index_ = index_++;
                    break;
                default:
                    error("math operation");
                    break;
                }
                return node->token;
            }
            else
            {
                // TODO: handle this case
            }
        }
        else
        {
            error("write the assembly this operation");
        }
        break;
    }
    case func_call_:
    {
        printf("found function call has name '%s'\n", node->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            printf("found output\n");
            output(evaluate(node->left));
            // dprintf(asm_fd, "   ");
        }
        break;
    }
    default:
        break;
    }
    printf("\n");
    return NULL;
}

int main(void)
{
    // opening file
#if 1
    asm_fd = open("file.s", O_CREAT | O_TRUNC | O_RDWR, 0777);
#else
    asm_fd = 1;
#endif
    fp = fopen("file.hr", "r");
    if (fp == NULL || asm_fd < 0)
        error("Opening file");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (!(text = calloc(size + 1, sizeof(char))))
        error("Allocation");
    fseek(fp, 0, SEEK_SET);
    fread(text, size, sizeof(char), fp);
    fclose(fp);
    printf("%s\n\n", text);

    index_ = 1;
    tk_len = var_len = 100;
    tokens = calloc(tk_len, sizeof(Token *));
    variables = calloc(tk_len, sizeof(Token *));
    build_tokens();
    free(text);
    tk_pos = 0;

    rsp = 30;
    Label = 2; // label from where to start, TODO: verify all label then set start label
    // write the assembly
    dprintf(asm_fd, ".section	.note.GNU-stack,\"\",@progbits\n");
    dprintf(asm_fd, ".intel_syntax noprefix\n");
    dprintf(asm_fd, ".text\n");
    dprintf(asm_fd, ".globl	main\n\n");
    dprintf(asm_fd, "main:\n");
    dprintf(asm_fd, "   push    rbp\n");
    dprintf(asm_fd, "   mov     rbp, rsp\n");
    dprintf(asm_fd, "   sub     rsp, %zu\n", rsp);
    // TODO: verify rsp position

    Node *main = new_node(NULL);
    Node *curr = main;
    while (tokens[tk_pos]->type != eof_)
    {
        curr->left = expr();
        evaluate(curr->left);
        curr->right = new_node(NULL);
        curr = curr->right;
    }
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
    if (BuiltIns[strlen_])
    {
        dprintf(asm_fd, "ft_putstr:\n");
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
    if (BuiltIns[putstr_])
    {
        dprintf(asm_fd, "ft_strlen:\n");
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
    dprintf(asm_fd, "\n");

    // clear everything
    while (main)
    {
        Node *tmp = main->right;
        free_node(main->left);
        free(main);
        main = tmp;
    }
    // free_node(curr);
    close(asm_fd);
}