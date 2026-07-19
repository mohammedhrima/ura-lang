#pragma once

#include "../header.h"

void tokenize(int base);
int parse_escape_seq(char *input, int s, int e, char *buf, int *ptr);
bool lex_spaces(char *src, int *i, int *line, int *indent, int base);
bool lex_multi_comment(char *src, int *i, int *line);
bool lex_comment(char *src, int *i);
bool lex_chars(char *src, int *i, int line, int indent);
bool lex_char(char *src, int *i, int line, int indent);
bool lex_number(char *src, int *i, int line, int indent);
bool lex_use(char *src, int *i, int s, int line);
bool lex_link(char *src, int *i, int s, int line);
bool lex_identifier(char *src, int *i, int line, int indent, int base);
bool lex_symbol(char *src, int *i, int line, int *indent);
Token *new_token(Type type, int indent);
Token *parse_token(int line, int s, int e, Type type, int indent);
void set_name(Token *token, char *name);
Token *next();
Token *peek(int index);
Token *find(Type type, ...);
int get_operation_precedence(Type type);
bool within(int indent);
void parser_recover(int indent);
Node *new_node(Token *token);
Node *syntax_error();
void parse_type(Token *target);
void parse_block(Node *node, int indent);
Node *match_node(Node *node);
Node *if_node(Node *node);
Node *while_node(Node *node);
Node *for_node(Node *node);
Node *ref_node(Node *node);
Node *id_node(Node *node);
Node *fdec_node(Node *node);
Node *struct_node(Node *node);
Node *fcall_node(Node *node);
Node *output_node(Node *node);
Node *access_node(Node *node);
Node *array_lit_node(Node *node);
Node *array_ctor_node(Node *node);
Node *prime_node();
Node *expr_node(int min_op);
void inject_self(Node *fn, Node *owner);
Node *clean_node(Node *node, Node *owner);
