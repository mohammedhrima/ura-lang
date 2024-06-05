#ifndef HEADER_H
#define HEADER_H

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
#define SPLIT GREEN "===============================" RESET

// DEBUGGING
#define RLOG(log, msg, ...)                   \
    {                                         \
        printf("%s%8s%s: ", RED, log, RESET); \
        printf(msg, ##__VA_ARGS__);           \
    }

#define CLOG(log, msg, ...)                    \
    {                                          \
        printf("%s%8s%s: ", CYAN, log, RESET); \
        printf(msg, ##__VA_ARGS__);            \
    }

#define GLOG(log, msg, ...)                     \
    {                                           \
        printf("%s%8s%s: ", GREEN, log, RESET); \
        printf(msg, ##__VA_ARGS__);             \
    }

// STRUCTS, ENUMS
typedef enum Type
{
    assign_ = 11,
    equal_,
    add_,
    sub_,
    mul_,
    div_,
    int_,
    name_,
    end_,
} Type;

typedef struct sType
{
    char *value;
    Type type;
} sType;

typedef struct Int
{
    long long value;
    int power;
    struct Int *next;
} Int;


typedef struct Token
{
    // if has name
    char *name;
    size_t ptr;
    size_t index;

    bool declare;
    Type type;
    Int _int;
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

typedef struct Inst
{
    size_t index;
    Node *node;
    // char *cmd;
    // Token *left;
    // Token *right;
} Inst;

// GLOBALS
extern sType *symbTypes;
extern sType *dataTypes;
extern sType *randTypes;
extern bool error;
extern size_t exe_pos;
extern size_t tk_pos;
extern size_t tk_len;
extern Token **tokens;

// FUNCTIONS
int tokenize(char *input);
Node *expr();
Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();
Node *new_node(Token *token);
void print_token(Token *token, bool end);
void print_node(Node *node, char *side, int space);
void free_node(Node *node);
void free_tokens();
char *to_string(Type type);

#endif