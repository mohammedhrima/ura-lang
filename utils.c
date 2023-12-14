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
    uintptr_t addr;
    struct
    {
        char *character;
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
int var_len;
int var_pos;
uintptr_t ptr;

Type twine(Type type)
{
    switch (type)
    {
    case num_var_:
        return num_val_;
    case char_var_:
        return num_var_;
    default:
        break;
    }
    return 0;
}
