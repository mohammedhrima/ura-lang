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

#define TOKENIZE 1

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#endif

#if IR
#define ASM 0
#endif

typedef enum
{
    assign_ = 11,
    lpar_,
    rpar_,
    mul_,
    add_,
    coma_,
    sub_,
    div_,
    int_,
    bool_,
    id_,
    fcall_,
    if_,
    dots_,
    end_,
} Type;

typedef struct
{
    char *value;
    Type type;
} Specials;

extern Specials *specials;

typedef struct
{
    Type type;

    // {
        int  reg;
        char c;
        size_t ptr;

        char *name;
        bool declare;
    // };

    union
    {
        // integer
        struct
        {
            long long value;
            int power;
            struct Int *next;
        } Int;
        // boolean
        struct
        {
            bool value;
            char c;
        } Bool;
    };
} Token;

typedef struct Node
{
    struct Node *left;
    struct Node *right;
    Token *token;
} Node;

typedef struct
{
    // Token *token;
    Type type;

    // registers Token*
    Token *token;
    Token *left;
    Token *right;

    // TODO: all of them to be removed
    // int value;
    // char *name;
    // bool declare;
    // bool remove;
    // size_t ptr;
    char c;

} Inst;

// GLOBALS
// extern Token **tokens;
// extern int tk_size;
// extern int tk_pos;
// extern Inst **insts;
// extern int inst_size;
// extern int inst_pos;
// extern Inst **regs;
// extern size_t stack_ptr;

// char *open_file(char *filename);
// void free_node(Node *node);
// char *to_string(Type type);
// void print_token(Token *token);
// void print_node(Node *node, char *side, int space);

#endif