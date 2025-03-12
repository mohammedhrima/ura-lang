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
extern bool found_error;
extern char *input;
extern Token **tokens;
extern Node *head;
extern size_t exe_pos;
extern Inst **OrgInsts;
extern Inst **insts;
extern Scoop *scoop;
extern size_t ptr;

void open_file(char *filename);
Token* new_token(char *input, size_t s, size_t e, Type type, size_t space);
void tokenize();
void generate_ast();
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
void generate();
Inst *new_inst(Token *token);
void setName(Token *token, char *name);
void setReg(Token *token, char *creg);
bool within_space(size_t space);
void add_token(Token *token);
Node *new_node(Token *token);
Token *find(Type type, ...);
const char *to_string(Type type);
void enter_scoop(char *name);
void check_error(const char *filename, const char *funcname, int line, bool cond, char *fmt, ...);
void free_memory();
void *allocate_func(size_t line, size_t len, size_t size);
int debug(char *conv, ...);
int pnode(Node *node, char *side, size_t space);
int ptoken(Token *token);
void enter_scoop(char *name);
void exit_scoop();
void clone_insts();
Node *new_function(Node *node);
Node *get_function(char *name);
Token *get_variable(char *name);
Token *new_variable(Token *token);
void print_ir();
bool compatible(Token *left, Token *right);