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

// GLOBAL
extern Wolfc obj;

// PARSE
void tokenize();
void generate_ast();
void setName(Token *token, char *name);
void setReg(Token *token, char *creg);
bool within_space(size_t space);
void add_token(Token *token);
Node *new_node(Token *token);
Token *find(Type type, ...);
void parse();

// AST
Node *expr();
Node *assign();
Node *logic();
Node *equality();
Node *comparison();
Node *add_sub();
Node *mul_div();
Node *dot();
Node *sign();
Node *prime();


// GENERATE
void generate_ir();
void optimize_ir();
void generate_asm();
void generate();

// UTILS
const char *to_string(Type type);
void open_file();
void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(size_t line, size_t len, size_t size);
int debug(char *conv, ...);
int pnode(Node *node, char *side, size_t space);
