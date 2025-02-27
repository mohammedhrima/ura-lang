// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

// MACROS
#define EXIT_STATUS 0
#define SPLIT "=================================================\n"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"
#define LINE __LINE__
#define FUNC __func__
#define FILE __FILE__

#define TOKENIZE 1
#define TAB 4

#if TOKENIZE
#define AST 1
#endif

#if AST
#define IR 1
#else
#define IR 0
#endif

// curent max value 4
#define WITH_COMMENTS 1

#if IR
#define BUILTINS 0
#ifndef OPTIMIZE
#define OPTIMIZE 1
#endif
#define ASM 1
#else
#define ASM 0
#endif

#ifndef DEBUG
#define DEBUG 1
#endif

// STRUCTS
typedef enum
{
   START = 1,
   ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
   EQUAL, NOT_EQUAL, LESS_EQUAL, MORE_EQUAL, LESS, MORE,
   ADD, SUB, MUL, DIV, MOD,
   AND, OR,
   RPAR, LPAR, COMA, DOT, DOTS,
   RETURN,
   IF, ELIF, ELSE, WHILE,
   FDEC, FCALL,
   VOID, INT, CHARS, CHAR, BOOL, FLOAT,
   STRUCT, ID, BLOC, END_BLOC,
   JNE, JE, JMP,
   PUSH, POP,
   END
} Type;

// PROTOTYPES
void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
#define check(cond, fmt, ...) check_error(__FILE__, __func__, __LINE__, cond, fmt, ##__VA_ARGS__)
bool includes(Type *types, Type type);
