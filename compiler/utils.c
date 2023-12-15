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
    num_var_,
    char_var_,
    void_var_,
    // values
    num_val_,
    char_val_,
    // function
    func_dec_,
    func_call_,
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
            char *character;
            size_t LC;
        };
        long number;
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
// Token **labels;
int var_len;
int var_pos;
uintptr_t ptr;
size_t LC;

Type twine(Type type)
{
    switch (type)
    {
    case num_var_:
        return num_val_;
    case char_var_:
        return char_val_;
    default:
        break;
    }
    return 0;
}

char *strjoin(char *left, char *right)
{
    char *res = calloc(strlen(left) + strlen(right) + 1, sizeof(char));
    strcpy(res, left);
    strcpy(res + strlen(res), right);
    return res;
}