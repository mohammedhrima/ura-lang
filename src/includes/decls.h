#pragma once

// ----------------------------------------------------------------------------
// File Management
// ----------------------------------------------------------------------------
char *open_file(char *filename);
char *resolve_path(char *path);
bool  add_file(char *filename);
Node *syntax_error();


// ----------------------------------------------------------------------------
// Memory
// ----------------------------------------------------------------------------
void  *allocate_func(int line, int len, int size);
void   free_token(Token *token);
void   free_node(Node *node);
void   free_memory();
Token *copy_token(Token *token);
Node  *copy_node(Node *node);

// ----------------------------------------------------------------------------
// Error Handling
// ----------------------------------------------------------------------------
bool check_error(char *filename, char *funcname, int line, bool cond, char *fmt, ...);

// ----------------------------------------------------------------------------
// Parsing
// ----------------------------------------------------------------------------
void   tokenize(char *filename);
Token *new_token(Type type, int space);
Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space);
void   add_token(Token *token);
Node  *new_node(Token *token);
Node  *add_child(Node *node, Node *child);
void   enter_scope(Node *node);
void   exit_scope();
bool   includes(Type to_find, ...);
void   setName(Token *token, char *name);
char  *to_string(Type type);
Token *find(Type type, ...);
bool   within(int space);
bool   is_data_type(Token *token);

void   add_struct(Node *b, Node *node);
Node  *new_struct(Node *node);
Node  *get_struct(char *name);
void add_method(Node *b, Node *node);
Token *is_struct(Token *token);
void   add_attribute(Token *obj, Token *attr);

Node *expr_node();
Node *assign_node();
Node *logic_and_node();
Node *logic_or_node();
Node *bitor_node();
Node *bitxor_node();
Node *bitnot_node();
Node *bitand_node();
Node *equality_node();
Node *comparison_node();
Node *shift_node();
Node *add_sub_node();
Node *mul_div_node();
Node *as_node();
Node *unary_node();
Node *access_node();
Node *prime_node();
Node *optimize_ir(Node *node, bool *changed);

// ----------------------------------------------------------------------------
// IR / Scope / Variable Management
// ----------------------------------------------------------------------------
void   gen_ir(Node *node);
void   add_variable(Node *b, Token *token);
Token *new_variable(Token *token);
Token *get_variable(char *name);
void   add_function(Node *b, Node *node);
Node  *new_function(Node *node);
Node  *get_function(char *name);

// ----------------------------------------------------------------------------
// ASM / Code Generation
// ----------------------------------------------------------------------------
void    gen_asm(Node *node);
void    init(char *name);
void    finalize(char *output);
void    load_if_necessary(Node *node);
void    _alloca(Token *token);
void    initialize_variable(Token *token);
TypeRef get_llvm_type(Token *token);
void    _const_value(Token *token);
Value   _get_default_value(Token *token);
Value   _build_return(Token *token);
void    _load(Token *to, Token *from);
bool    compatible(Token *left, Token *right);

// ----------------------------------------------------------------------------
// Debugging
// ----------------------------------------------------------------------------
int  debug_(char *conv, ...);
void pnode(Node *node, char *indent);

// ----------------------------------------------------------------------------
// String Utilities
// ----------------------------------------------------------------------------
char *strjoin(char *str0, char *str1, char *str2);

// ----------------------------------------------------------------------------
// Other utilities
// ----------------------------------------------------------------------------
Value   allocate_stack(Value size, TypeRef elementType, char *name);
Value   allocate_heap(Value count, TypeRef elementType, char *name);
void   set_ret_type(Node *node);
void **resize_array(void **array, int *len, int pos, int element_size);
