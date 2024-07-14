#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define SPLIT "=================================================\n"
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
    lpar_ = '(',
    rpar_ = ')',
    coma_ = ',',
    id_ = 'I',
    fcall_ = 'c',
    arg_ = 'a',
    end_ = 'e',
} Type;

typedef struct Token
{
    Type type;
    int value;
    char *name;
    bool declare;
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
    // registers
    int r1;
    int r2;
    int r3;

    int value;
    char *name;
    bool declare;
    bool remove;

    // Token *left;
    // Token *right;
} Inst;

// GLOBALS
extern Token **tokens;
extern int tk_size;
extern int tk_pos;
extern Inst **insts;
extern int inst_size;
extern int inst_pos;

char *open_file(char *filename);
void free_node(Node *node);
char *to_string(Type type);
void clear(Node *head, char *input);
void print_token(Token *token);
void print_node(Node *node, char *side, int space);


#endif