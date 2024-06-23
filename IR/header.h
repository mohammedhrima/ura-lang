#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define SPLIT "==================================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

typedef enum Type
{
    assign_ = '=',
    add_ = '+',
    sub_ = '-',
    mul_ = '*',
    div_ = '/',
    int_ = 'i',
    id_,
    end_ = 'e',
} Type;

typedef struct Token
{
    Type type;
    int value;
    char *name;
    // int reg;
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

typedef struct Inst
{
    // Token *token;
    Type type;
    int r1, r2, r3;
    int value;
    char *name;
    bool remove;

    // Token *left;
    // Token *right;
} Inst;

char *open_file(char *filename);

#endif