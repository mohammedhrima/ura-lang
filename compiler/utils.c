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
