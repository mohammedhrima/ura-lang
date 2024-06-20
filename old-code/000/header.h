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

// MACROS
#ifndef DEBUG
#define DEBUG 1
#endif
#define EXIT_STATUS 0
#define YELLOW "\033[1;33m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

// typedefs
typedef struct Token Token;
typedef struct Node Node;
typedef struct Label Label;
typedef enum Type Type;

// stupid implicit declaration error
Token *get_var(char *name);
Token *evaluate(Node *node);
void pnode(Node *node, int level);
Node *expr();
Node *assign();     // = += -= *= /=
Node *logic();      // || or && and
Node *equality();   // ==  !=
Node *comparison(); // < > <= >=
Node *add_sub();    // + -
Node *mul_div();    // * /
Node *unary();      // -
Node *brackets();   // []
Node *prime();

// STRUCTURES / ENUMS
enum Type
{
    eof_ = 11,
    none_,
    // math operators
    add_,
    sub_,
    mul_,
    div_,
    mod_,
    // logic operators
    and_,
    or_,
    // comparision operators
    not_,
    not_equal_,
    equal_,
    less_than_,
    grea_than_,
    less_than_equal_,
    grea_than_equal_,
    // parents
    lparent_,
    rparent_,
    // bracket
    lbracket_,
    rbracket_,
    // assignment
    assign_,
    add_assign_,
    sub_assign_,
    mul_assign_,
    div_assign_,
#if 0
    // memory
    fix_,
    dyn_,
#endif
    // Data types
    void_,
    char_,
    chars_,
    int_,
    float_,
    bool_,
    identifier_,
    neg_,
    array_,
    ref_,
    // functions
    func_dec_,
    func_call_,
    coma_,
    // statements / loops
    if_,
    elif_,
    else_,
    while_,
    continue_,
    break_,
    dots_,
    return_,
    class_,
};

// GLOBALS
struct
{
    Type type;
    char *name;
} DataTypes[] = {
    {void_, "void"},
    {chars_, "chars"},
    {char_, "char"},
#if 0
    {chars_, "fix:char"},
    {chars_, "dyn:char"},
#endif
    {int_, "int"},
    {float_, "float"},
    {bool_, "bool"},
    {array_, "array"},
    {ref_, "ref"},
    {none_, NULL},
};

struct
{
    Type type;
    char *name;
} Symbols[] = {
    // comparision operators
    {less_than_equal_, "<="},
    {grea_than_equal_, ">="},
    {not_equal_, "!="},
    {not_, "!"},
    {equal_, "=="},
    {less_than_, "<"},
    {grea_than_, ">"},
    // assign
    {assign_, "="},
    {add_assign_, "+="},
    {sub_assign_, "-="},
    {mul_assign_, "*="},
    {div_assign_, "/="},
    // math operators
    {add_, "+"},
    {sub_, "-"},
    {mul_, "*"},
    {div_, "/"},
    {mod_, "%"},
    // parents, coma
    {lparent_, "("},
    {rparent_, ")"},
    {coma_, ","},
    // brackets
    {lbracket_, "["},
    {rbracket_, "]"},
    // logic
    {and_, "&&"},
    {or_, "||"},
    {dots_, ":"},
    {none_, NULL},
};

struct
{
    Type type;
    char *name;
} Specials[] = {
    {not_, "not"},
    {and_, "and"},
    {or_, "or"},
    {if_, "if"},
    {else_, "else"},
    {elif_, "elif"},
    {while_, "while"},
    {break_, "break"},
    {continue_, "continue"},
    {func_dec_, "func"},
    {return_, "return"},
    {class_, "class"},
    {none_, "none"},
    {(Type)0, NULL},
    {identifier_, "identifier"},
    {func_call_, "fcall"},
    {neg_, "negative"},
    {eof_, "EOF"},
};

struct Token
{
    char *name;
    Type type;
    uintptr_t ptr;
    size_t col;
    bool is_ref;
    bool has_ref;
    struct
    {
        // INTEGER
        long long int_;
        // FLOAT
        struct
        {
            uint32_t float_;
            size_t float_index_;
        };
        // CHARACTER
        struct
        {
            char *chars_;
            size_t chars_index_;
        };
        // ARRAY
        struct
        {
            /*
                rbp - 16: pointer
                rbp -  8: len
            */
            size_t depth;
            Type child_type;
            size_t arrlen_;
        };
        // BOOLEAN
        struct
        {
            bool bool_;
            char c;
        };
        size_t label_index_;
        // FUNCTION
        Type ret_type;
    };
};

struct Label
{
    // NAME
    char *name;
    // NODE
    Node *node;
    // VARIABLES
    Token **VARIABLES;
    int var_len;
    int var_pos;
    // FUNCTIONS
    Node **FUNCTIONS;
    int func_len;
    int func_pos;
};

struct Node
{
    Node *left;
    Node *right;
    Token *token;
};

// FILES
FILE *file;
FILE *asm_fd;
char *text;

// TOKENS
Token **TOKENS;
int tk_len;
int tk_pos;
int exe_pos;

// GLOBALS
size_t Label_index;
size_t index_;
uintptr_t ptr;
size_t rsp;

// LABELS
Label **LABELS;
int lb_len;
int lb_pos;

#endif