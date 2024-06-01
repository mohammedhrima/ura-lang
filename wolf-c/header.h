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

// STRUCTS, ENUMS
typedef enum Type
{
    assing_ = '=',
    add_ = '+',
    sub_ = '-',
    mul_ = '*',
    div_ = '/',
    lpar_ = '(',
    rpar_ = ')',
    int_ = 'i',
    float_ = 'f',
    char_ = 'c',
    string_ = 's',
    array_ = 'a',
    bool_ = 'b',
    name_ = 'n',
    end_ = 'e',
} Type;

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
    char *name;
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
void print_token(Token *token);
void print_node(Node *node, char *side, int space);
void pasm(char *fmt, ...);

Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();

#endif