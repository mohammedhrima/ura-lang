#pragma once

// HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include "macros.h"
#include "structs.h"

// PROTOTYPES
// void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
// #define check(cond, fmt, ...) check_error(__FILE__, __func__, __LINE__, cond, fmt, ##__VA_ARGS__)
// bool includes(Type *types, Type type);

// AST
// Node *expr();
// Node *assign();
// Node *logic();
// Node *equality();
// Node *comparison();
// Node *add_sub();
// Node *mul_div();
// Node *dot();
// Node *sign();
// Node *prime();

// GLOBAL
extern Wolfc obj;

// PARSE
void tokenize();
void generate_ast();

// GENERATE
void generate_ir();
void optimize_ir();
void generate_asm();

// UTILS
void open_file();
void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(size_t line, size_t len, size_t size);
int debug(char *conv, ...);
