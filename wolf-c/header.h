#ifndef COMPILER_H
#define COMPILER_H

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

// COLORS
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define SPLIT GREEN "\n========================================================\n" RESET

// DEBUGGING
// #define DEBUG(func, line) printf(GREEN "%s:%d: " RESET, func, line)

#define RLOG(log, msg, ...)                   \
    do                                        \
    {                                         \
        printf("%s%8s%s: ", RED, log, RESET); \
        printf(msg, ##__VA_ARGS__);           \
    } while (0)

#define CLOG(log, msg, ...)                    \
    do                                         \
    {                                          \
        printf("%s%8s%s: ", CYAN, log, RESET); \
        printf(msg, ##__VA_ARGS__);            \
    } while (0)

#define GLOG(log, msg, ...)                     \
    do                                          \
    {                                           \
        printf("%s%8s%s: ", GREEN, log, RESET); \
        printf(msg, ##__VA_ARGS__);             \
    } while (0)
// STRUCTS, ENUMS
typedef enum Type
{
    assign_ = 11,
    less_,
    more_,
    equal_,
    add_,
    sub_,
    mul_,
    div_,
    lpar_,
    rpar_,
    int_,
    float_,
    char_,
    string_,
    array_,
    bool_,
    name_,
    end_,
    while_,
    for_,
    if_,
    else_,
    in_,
    semi_,
    dots_,
    incr_,
    decr_,
} Type;

typedef struct sType
{
    char *value;
    Type type;
} sType;

extern sType *dataTypes;
extern sType *blocTypes;
extern sType *randTypes;
extern sType *symbTypes;

typedef struct Int
{
    long long value;
    int power;
    struct Int *next;
} Int;

typedef struct Float
{
    uint32_t value;
    size_t index;
} Float;

typedef struct Char
{
    char value;
} Char;

typedef struct String
{
    char *value;
    size_t index;
} String;

typedef struct Array
{
    Type child_type;
    size_t depth;
} Array;

typedef struct Bool
{
    bool value;
    char c;
} Bool;

typedef struct Func
{
    char *name;
    Type ret_type;
} Func;

typedef struct Name
{
    char *name;
    size_t ptr;
} Name;

typedef struct Token
{
    bool remove;
    char *name;
    bool declaration;
    int space;
    size_t index;
    size_t ptr;
    Type type;
    Name _name;
    Int _int;
    Float _float;
    Char _char;
    String _string;
    Array _array;
    Bool _bool;
    Func _func;
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

// GLOBALS
extern Token **tokens;
extern size_t pos0;
extern size_t pos1;
extern bool error;
extern FILE *asm_fd;

// FUNCTIONS
char *to_string(Type type);
void free_tokens();
void free_node(Node *node);
void print_token(Token *token, bool end);
void print_node(Node *node, char *side, int space);

Node *expr();
Node *assign();
Node *relation();
Node *add_sub();
Node *mul_div();
Node *bloc();
Node *prime();

Token *evaluate(Node *node);

void pasm(char *fmt, ...);
void mov(char *fmt, ...);
void add(char *fmt, ...);
void sub(char *fmt, ...);
void mul__(char *fmt, ...);
void div__(char *fmt, ...);
void push(char *fmt);
void comment(char *fmt, ...);

#endif