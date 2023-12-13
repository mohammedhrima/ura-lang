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
    char *character;
    long number;
    uintptr_t addr;
};

struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

void error(char *msg)
{
    // free memory before exiting
    dprintf(2, "\033[0;31mError: %s\n", msg);
    exit(1);
}

char *type_to_string(Type type)
{
    switch (type)
    {
    case eof_:
        return "EOF";
    case add_:
        return "+";
    case sub_:
        return "-";
    case mul_:
        return "*";
    case div_:
        return "/";
    case lparent_:
        return "(";
    case rparent_:
        return ")";
    case assign_:
        return "=";
    case void_var_:
        return "void_var";
    case num_val_:
        return "num_val";
    case char_val_:
        return "char_val";
    case num_var_:
        return "num_var";
    case char_var_:
        return "char_var";
    case func_call_:
        return "function call";
    case func_dec_:
        return "function declaration";
    default:
        return "Unknown type";
    }
    return NULL;
}

Type twine(Type type)
{
    switch (type)
    {
    case num_var_:
        return num_val_;
    case char_var_:
        return num_var_;
    default:
        error("in twine");
        break;
    }
    return 0;
}