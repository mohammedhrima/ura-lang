#pragma once

#if defined(__APPLE__)
typedef struct __sFILE *File;
#elif defined(__linux__)
typedef struct _IO_FILE *File;
#endif

#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define BOLD "\e[1m"
#define BLUE "\x1b[34m"
#define RESET "\033[0m"

#define LINE __LINE__
#define FUNC (char *)__func__
#define FILE (strrchr(__FILE__, '/') ? \
              strrchr(__FILE__, '/') + 1 : \
              (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 \
    : __FILE__))

#define TOKENIZE 1
#define TAB 3
#define AST 1
#define IR 1
#define OPTIMIZE 1
#define ASM 1

#ifndef DEBUG
#define DEBUG 1
#endif

#define allocate(len, size) allocate_func(LINE, len, size)
#define check(cond, fmt, ...) \
        check_error(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)
#define todo(cond, fmt, ...)                                          \
        if (check_error(FILE, FUNC, LINE, cond, fmt, ## __VA_ARGS__)) exit(1);
#define seg() raise(SIGSEGV)
#define expect_token(type, fmt, ...)          \
        {                                     \
           Token *find_token = find(type, 0); \
           if (!find_token)                   \
           {                                  \
              check(1, fmt, ## __VA_ARGS__);  \
              return syntax_error();          \
           }                                  \
        }

#if DEBUG
#define debug(fmt, ...) debug_(fmt, ## __VA_ARGS__)
#else
#define debug(fmt, ...)      \
        do                   \
        {                    \
        } while (0)
#endif

#define DATA_TYPES INT, BOOL, CHARS, CHAR, FLOAT, VOID, \
        LONG, PTR, SHORT, ARRAY_TYPE
#define LOGIC_TYPE AND, OR
#define MATH_TYPE      ADD, SUB, MUL, DIV, MOD, BAND, BOR, BXOR, LSHIFT, RSHIFT
#define COMPARISON_OPS EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL
#define BINARY_OPS     MATH_TYPE, AND, OR, COMPARISON_OPS
#define NUMERIC_TYPES  INT, LONG, SHORT, BOOL, CHAR
#define ASSIGNS_OP     ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN
#define USING_HOIST false
// O0 is basically "Literal Translation" (Debug Mode)
#define PASSES_O0 "default<O0>"
// Removes basic redundancies and promotes memory to registers (mem2reg).
#define PASSES_O1 "default<O1>"
// Most production code uses this. Adds vectorization and aggressive inlining.
#define PASSES_O2 "default<O2>"
// Maximum speed optimizations, including heavy loop unrolling.
#define PASSES_O3 "default<O3>"
// Similar to O2, but disables optimizations that increase binary size.
#define PASSES_Os "default<Os>"
// Shrinks the code as much as possible, even if it makes it slower.
#define PASSES_Oz "default<Oz>"

#define AST_NODE(name, child_func, ...)           \
        Node *name()                              \
        {                                         \
           Node  *left = child_func();            \
           Token *token;                          \
           while ((token = find(__VA_ARGS__, 0))) \
           {                                      \
              Node *node = new_node(token);       \
              node->left  = left;                 \
              node->right = child_func();         \
              left        = node;                 \
           }                                      \
           return left;                           \
        }
