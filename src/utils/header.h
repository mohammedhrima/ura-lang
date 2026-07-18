#pragma once

#include "../header.h"

double clock_now();
char *signal_name(int sig);
char *format(const char *fmt, ...);
void *allocate(int len, int size);
void new_source(char *file_name);
bool includes(Type to_find, ...);
void free_token(Token *token);
void free_node(Node *node);
void free_memory();
char *to_string(Type type);
char *type_name(Type type);
int _vprint(File out, const char *conv, va_list args);
int _debug(char *conv, ...);
void fprint_escaped(File out, char c);
void print_escaped(char c);
char *array_type_label(Token *token);
char *spell(Type type);
void print_node_label(Node *node);
void print_subtree(Node *node, char *prefix, bool is_last, char *role);
void print_children(Node *node, char *prefix);
void print_ast(Node *head);
void pnode(Node *node, char *indent);
bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...);
void decolor(char *s);
void render_caret(File out, Token *token);
bool is_dir(char *path);
char *exe_dir();
char *find_ura_lib();
void ura_lib_missing(int line, int s, int e);
void parse_error(Token *token, const char *fmt, ...);
void parse_warn(Token *token, const char *fmt, ...);
void tokenize_error(int line, int s, int e, const char *fmt, ...);
