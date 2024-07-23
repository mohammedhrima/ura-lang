#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

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
#define ASM 1
#define OPTIMIZE 0
#endif

typedef enum
{
    assign_ = 11, add_assign_, sub_assign_, mul_assign_, div_assign_, 
    lpar_, rpar_, 
    mul_, add_, sub_, div_, 
    equal_, not_equal_, less_, more_, less_equal_, more_equal_,
    id_, int_, bool_, string_, void_,
    coma_,
    fcall_, fdec_, ret_,
    if_, elif_, else_, while_, dots_, 
    cmp_, jne_, je_, jmp_, bloc_,
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
    // Type subtype;

    int reg;
    char c;
    size_t ptr;
    size_t index;
    int space;

    char *name;
    bool declare;
    bool remove;
    bool isbuiltin;
    bool isarg;

    struct
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
        // string
        struct
        {
            char *value;
        } String;
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
    // registers Token*
    Token *token;
    Token *left;
    Token *right;
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