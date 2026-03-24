#include "header.h"

// icmp_predicate
LLVMIntPredicate icmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMIntSLT;
	case GREAT:       return LLVMIntSGT;
	case EQUAL:       return LLVMIntEQ;
	case LESS_EQUAL:  return LLVMIntSLE;
	case GREAT_EQUAL: return LLVMIntSGE;
	default:          return LLVMIntNE; // NOT_EQUAL
	}
}

// fcmp_predicate
LLVMRealPredicate fcmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMRealOLT;
	case GREAT:       return LLVMRealOGT;
	case EQUAL:       return LLVMRealOEQ;
	case LESS_EQUAL:  return LLVMRealOLE;
	case GREAT_EQUAL: return LLVMRealOGE;
	default:          return LLVMRealONE; // NOT_EQUAL
	}
}

// is_float_value
int is_float_value(Value v) {
	LLVMTypeKind k = LLVMGetTypeKind(LLVMTypeOf(v));
	return k == LLVMFloatTypeKind || k == LLVMDoubleTypeKind;
}

// assign_base_op
Type assign_base_op(Type assign_op) {
	switch (assign_op) {
	case ADD_ASSIGN: return ADD;
	case SUB_ASSIGN: return SUB;
	case MUL_ASSIGN: return MUL;
	case DIV_ASSIGN: return DIV;
	default:         return MOD; // MOD_ASSIGN
	}
}

// _branch
void _branch(Block bloc) {
	if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) LLVMBuildBr(builder, bloc);
}

// _append_block
Block _append_block(char *name) {
	Value parent = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
	return LLVMAppendBasicBlockInContext(context, parent, name);
}

// _named_struct_type
TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed) {
	TypeRef type = LLVMStructCreateNamed(context, name);
	LLVMStructSetBody(type, element_types, element_count, packed);
	return type;
}

// _add_function
Value _add_function(char *name, TypeRef function_type) {
	Value f = LLVMGetNamedFunction(module, name);
	if (f) return f;
	return LLVMAddFunction(module, name, function_type);
}

// set_debug_location
void set_debug_location(Token *token) {
	if (!token || !debug_builder || !debug_scope)              return;
	if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
	LLVMMetadataRef loc =
	    LLVMDIBuilderCreateDebugLocation(context, token->line, 0, debug_scope, NULL);
	LLVMSetCurrentDebugLocation2(builder, loc);
}

// struct_field_ptr
Value struct_field_ptr(Token *struct_tok, int field_index, char *name) {
	TypeRef struct_type = get_llvm_type(struct_tok);
	Value   indices[]   = {
       LLVMConstInt(i32, 0, 0),
       LLVMConstInt(i32, field_index, 0),
   };
	return LLVMBuildGEP2(builder, struct_type, struct_tok->llvm.elem, indices, 2, name);
}

// load_value
Value load_value(Token *token) {
	if (token->llvm.is_loaded) return token->llvm.elem;
	// Already-computed values: function returns, literals, allocated buffers
	if (includes(token->type, MATH_TYPE, FCALL, STACK, HEAP, 0)) return token->llvm.elem;
	if (!token->name && !includes(token->type, DOT, ACCESS, 0))  return token->llvm.elem;

	// Scalar ref: double-deref (alloca-of-ptr → ptr → value)
	if (token->is_ref && token->type != STRUCT_CALL) {
		TypeRef type = get_llvm_type(token);
		Value   ptr  = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), token->llvm.elem, "ref_ptr");
		return LLVMBuildLoad2(builder, type, ptr, "ref_val");
	}

	char *name = token->name;
	if (token->type == DOT)    name = to_string(DOT);
	if (token->type == ACCESS) name = to_string(ACCESS);
	return LLVMBuildLoad2(builder, get_llvm_type(token), token->llvm.elem, name ? name : "");
}

// new_token
Token *new_token(Type type, int space) {
	Token *token = allocate(1, sizeof(Token));
	token->type  = type;
	token->space = ((space + TAB / 2) / TAB) * TAB;
	add_token(token);
	return token;
}

// unuse
void unuse(Node *node) {
	if (!node || !node->token) return;

	if (node->left && node->left->token) {
		if (node->left->token->used > 0)    node->left->token->used--;
	}
	if (node->right && node->right->token) {
		if (node->right->token->used > 0)     node->right->token->used--;
	}
	for (int i = 0; i < node->children_count; i++)
		if (node->children[i] && node->children[i]->token)
			if (node->children[i]->token->used > 0) node->children[i]->token->used--;
}

// resolve_path
char *resolve_path(char *path) {
	if (path == NULL) return NULL;
	char *cleaned = allocate(strlen(path) + 5, 1);
	if (!cleaned) return NULL;
	size_t i = 0, j = 0;
	while (path[i]) {
		cleaned[j++] = path[i++];
		while (path[i] == '/') {
			if (cleaned[j - 1] != '/') cleaned[j++] = '/';
			i++;
		}
	}
	if (j > 1 && cleaned[j - 1] == '/') j--;
	cleaned[j] = '\0';
	return cleaned;
}

// allocate_func
void *allocate_func(int line, int len, int size) {
	void *res = calloc(len, size);
	check(!res, "allocate did failed in line %d", line);
	return res;
}

// free_token
void free_token(Token *token) {
	free(token->name);
	free(token->Chars.value);
	free(token);
}

// free_node
void free_node(Node *node) {
	if (!node) return;
	for (int i = 0; i < node->children_count; i++)
		free_node(node->children[i]);
	free_node(node->left);
	free_node(node->right);
	free(node->children);
	free(node->functions);
	free(node->variables);
	free(node->structs);
	free(node);
}

// free_local_memory
void free_local_memory() {
	for (int i = 0; tokens && tokens[i]; i++) {
		free_token(tokens[i]);
		tokens[i] = NULL;
	}
	tcount = 0;
	ecount = 0;
}

void free_global_memory() {
	for (int i = 0; links && links[i]; i++) {
		free(links[i]);
		links[i] = NULL;
	}
	lcount = 0;
}

// add_token
void add_token(Token *token) {
	resize_array(tokens, Token *, tsize, tcount);
	tokens[tcount++] = token;
}

// new_node
Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

// add_child
Node *add_child(Node *node, Node *child) {
	if (child) {
		resize_array(node->children, Node *, node->children_size, node->children_count);
		child->token->space                    = node->token->space + TAB;
		node->children[node->children_count++] = child;
	}
	return child;
}

// includes
bool includes(Type to_find, ...) {
	if (found_error) return false;
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

// setName
void setName(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

// to_string
char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
	    [INT] = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
	    [FDEC] = "FDEC", [FCALL] = "CALL", [END] = "END", [LPAR] = "LPAR",
	    [RPAR] = "RPAR", [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
	    [WHILE] = "WHILE", [BREAK] = "BRK", [CONTINUE] = "CONT",
	    [SHORT] = "SHORT", [RETURN] = "RET", [BAND] = "BAND", [BOR] = "BOR",
	    [BXOR] = "BXOR", [BNOT] = "BNOT", [LSHIFT] = "LSHIFT",
	    [RSHIFT] = "RSHIFT", [ADD] = "ADD", [SUB] = "SUB", [MUL] = "MUL",
	    [DIV] = "DIV", [ASSIGN] = "ASSIGN", [ADD_ASSIGN] = "ADD_ASS",
	    [SUB_ASSIGN] = "SUB_ASS", [MUL_ASSIGN] = "MUL_ASS",
	    [DIV_ASSIGN] = "DIV_ASS", [MOD_ASSIGN] = "MOD_ASS", [ACCESS] = "ACC",
	    [MOD] = "MOD", [COMA] = "COMA", [REF] = "REF", [EQUAL] = "EQ",
	    [NOT_EQUAL] = "NEQ", [LESS] = "LT", [GREAT] = "GT",
	    [LESS_EQUAL] = "LE", [NOT] = "NOT", [GREAT_EQUAL] = "GE",
	    [AND] = "AND", [OR] = "OR", [DOTS] = "DOTS", [PROTO] = "PROT",
	    [VARIADIC] = "VAR", [TYPEOF] = "TYPEOF", [SIZEOF] = "SIZEOF",
	    [OUTPUT] = "OUTPUT", [ARGS] = "ARGS", [CHILDREN] = "CHILDREN",
	    [AS] = "AS", [STACK] = "STACK", [HEAP] = "HEAP",
	    [ARRAY_TYPE] = "ARRAY_TYPE", [NULLABLE] = "NULLABLE",
	    //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
	    [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
	    [ENUM_DEF] = "ENUM_DEF", [ENUM_CALL] = "ENUM_CALL", [TUPLE] = "TUPLE",
	    [TUPLE_UNPACK] = "TUPLE_UNPACK", [LBRA] = "LBRA", [RBRA] = "RBRA",
	    [ARRAY] = "ARRAY", [DOT] = "DOT", [SYNTAX_ERROR] = "SYNTAX_ERROR",
	    [MODULE] = "MODULE",
	};

	if (check(!res[type], "handle this case %d\n", type)) {
		// seg();
		exit(1);
	}
	return res[type];
}

// new_struct
Node *new_struct(Node *node) {
	debug(CYAN("new struct [%s] in scope %k\n"), node->token->name, scope->token);
	for (int i = 0; i < scope->structs_count; i++) {
		Token *curr = scope->structs[i]->token;
		bool   cond = (strcmp(curr->name, node->token->name) == 0);
		check(cond, "Redefinition of %s", node->token->name);
	}
	resize_array(scope->structs, Node *, scope->structs_size, scope->structs_count);
	scope->structs[scope->structs_count++] = node;
	return node;
}

// add_struct
void add_struct(Node *parent, Node *node) {
	resize_array(parent->structs, Node *, parent->structs_size, parent->structs_count);
	parent->structs[parent->structs_count++] = node;
}

// syntax_error
Node *syntax_error() {
	found_error = true;
	static Node *node;
	if (node == NULL) node = new_node(new_token(SYNTAX_ERROR, -1));
	return node;
	return node;
}

// add_variable
void add_variable(Node *parent, Token *token) {
	resize_array(parent->variables, Token *, parent->variables_size, parent->variables_count);
	parent->variables[parent->variables_count++] = token;
}

// new_variable
Token *new_variable(Token *token) {
	debug(CYAN("new variable [%k] in scope %k\n"), token, scope->token);
	for (int i = 0; i < scope->variables_count; i++) {
		Token *curr = scope->variables[i];
		bool   cond = (strcmp(curr->name, token->name) == 0);
		check(cond, "Redefinition of %s", token->name);
	}
	add_variable(scope, token);
	return token;
}

// copy_token
Token *copy_token(Token *token) {
	if (token == NULL) return NULL;
	Token *new = allocate(1, sizeof(Token));
	memcpy(new, token, sizeof(Token));
	new->name = NULL;
	if (token->name)        setName(new, token->name);
	if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
	add_token(new);
	return new;
}

// vprint_
int vprint_(File out, char *conv, va_list args) {
	int res = 0;

	for (int i = 0; conv[i]; i++) {
		if (conv[i] != '%') {
			res += fprintf(out, "%c", conv[i]);
			continue;
		}
		i++;
		int left_align = 0;
		if (conv[i] == '-') {
			left_align = 1;
			i++;
		}
		int width = 0;
		while (isdigit(conv[i])) {
			width = width * 10 + (conv[i] - '0');
			i++;
		}
		int prec = -1;
		if (conv[i] == '.') {
			i++;
			prec = 0;
			while (conv[i] >= '0' && conv[i] <= '9') {
				prec = prec * 10 + (conv[i] - '0');
				i++;
			}
		}
		if (strncmp(conv + i, "zu", 2) == 0) {
			res += fprintf(out, "%d", va_arg(args, int));
			i++;
		} else if (strncmp(conv + i, "lld", 3) == 0) {
			res += fprintf(out, "%lld", va_arg(args, long long));
			i += 2;
		} else {
			switch (conv[i]) {
			case 'c': res += fprintf(out, "%c", va_arg(args, int)); break;
			case 's': {
				char *str = va_arg(args, char *);
				if (!str) str = "(null_str)";
				if (left_align)
					res += (prec >= 0) ? fprintf(out, "%-*.*s", width, prec, str)
					                   : fprintf(out, "%-*s", width, str);
				else
					res += (prec >= 0) ? fprintf(out, "%*.*s", width, prec, str)
					                   : fprintf(out, "%*s", width, str);
				break;
			}
			case 'p': res += fprintf(out, "%p", va_arg(args, void *)); break;
			case 'x':
				res += (prec >= 0) ? fprintf(out, "%.*x", prec, va_arg(args, unsigned int))
				                   : fprintf(out, "%x", va_arg(args, unsigned int));
				break;
			case 'X':
				res += (prec >= 0) ? fprintf(out, "%.*X", prec, va_arg(args, unsigned int))
				                   : fprintf(out, "%X", va_arg(args, unsigned int));
				break;
			case 'd':
				res += (prec >= 0) ? fprintf(out, "%.*d", prec, va_arg(args, int))
				                   : fprintf(out, "%d", va_arg(args, int));
				break;
			case 'f':
				res += (prec >= 0) ? fprintf(out, "%.*f", prec, va_arg(args, double))
				                   : fprintf(out, "%f", va_arg(args, double));
				break;
			case '%': res += fprintf(out, "%%"); break;
			case 't': res += fprintf(out, "%s", to_string((Type)va_arg(args, Type))); break;
			case 'k': {
				Token *token = va_arg(args, Token *);
				if (!token) {
					fprintf(out, "(null)");
					break;
				}

				fprintf(out, "[%s] ", to_string(token->type));

				switch (token->type) {
				case VOID: case CHARS: case CHAR: case INT: case BOOL: case FLOAT:
				case LONG: {
					if (token->name) {
						fprintf(out, "%s ", token->name);
						break;
					}
					if (token->type == VOID) break;
					// print_value inline
					switch (token->type) {
					case INT:   fprintf(out, "[%lld] ", (long long)token->Int.value); break;
					case LONG:  fprintf(out, "[%lld] ", token->Long.value); break;
					case BOOL:  fprintf(out, "[%s] ", token->Bool.value ? "True" : "False"); break;
					case FLOAT: fprintf(out, "[%f] ", token->Float.value); break;
					case CHAR:  {
						fprintf(out, "[");
						char c = token->Char.value;
						switch (c) {
						case '\n': fprintf(out, "\\n"); break;
						case '\t': fprintf(out, "\\t"); break;
						case '\r': fprintf(out, "\\r"); break;
						case '\\': fprintf(out, "\\\\"); break;
						case '\"': fprintf(out, "\\\""); break;
						default:   fprintf(out, "%c", c); break;
						}
						fprintf(out, "] ");
						break;
					}
					case CHARS: {
						fprintf(out, "[\"");
						char *str = token->Chars.value;
						if (str)
							for (int j = 0; str[j]; j++) {
								switch (str[j]) {
								case '\n': fprintf(out, "\\n"); break;
								case '\t': fprintf(out, "\\t"); break;
								case '\r': fprintf(out, "\\r"); break;
								case '\\': fprintf(out, "\\\\"); break;
								case '\"': fprintf(out, "\\\""); break;
								default:   fprintf(out, "%c", str[j]); break;
								}
							}
						fprintf(out, "\"] ");
						break;
					}
					case ADD:       case SUB:
					case NOT_EQUAL: fprintf(out, "%s ", to_string(token->type)); break;
					default:        break;
					}
					break;
				}
				case STRUCT_CALL:
					fprintf(out, "name [%s] ", token->name);
					fprintf(out, "st_name [%s] ", token->Struct.ptr->token->name);
					break;
				case STRUCT_DEF: case FCALL: case FDEC:
				case ID:         fprintf(out, "%s ", token->name); break;
				default:         break;
				}

				if (token->is_ref)      fprintf(out, "ref ");
				if (token->ir_bound)    fprintf(out, "bound ");
				if (token->retType)     fprintf(out, "ret [%s] ", to_string(token->retType));
				if (token->is_variadic) fprintf(out, "variadic ");
				break;
			}
			default: todo(1, "invalid format specifier [%c]", conv[i]); break;
			}
		}
	}
	return res;
}

// _debug
int _debug(char *conv, ...) {
	va_list args;
	va_start(args, conv);
	int res = vprint_(stdout, conv, args);
	va_end(args);
	return res;
}

// _check
bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...) {
	if (!cond) return cond;
	found_error = true;
	fprintf(stderr, RED("ura_error: %s %s:%d "), funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	vprint_(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	// seg();
	return cond;
}

// pnode
void pnode(Node *node, char *indent) {
	if (!node || !node->token || !DEBUG) return;
	Node **subs     = NULL;
	int    count    = 0;
	int    capacity = 0;

#define push(n)                                                                                    \
	do {                                                                                            \
		resize_array(subs, Node *, capacity, count);                                                 \
		subs[count++] = (n);                                                                         \
	} while (0)

	debug("%k\n", node->token);
	if (includes(node->token->type, IF, ELIF, ELSE, 0)) {
		if (node->left)                                    push(node->left);
		for (int i = 0; i < node->children_count; i++)
			push(node->children[i]);
		if (node->right) push(node->right);
	} else {
		if (node->left)  push(node->left);
		if (node->right) push(node->right);
		for (int i = 0; i < node->children_count; i++)
			push(node->children[i]);
	}
	for (int i = 0; i < count; i++) {
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int         is_last = (i == count - 1);
		const char *bar     = is_last ? "   " : "│  ";

		char        new_indent[4096];
		snprintf(new_indent, sizeof(new_indent), "%s%s", indent, bar);

		char *connector = is_last ? "└──" : "├──";
		debug("%s%s", indent, connector);
		pnode(child, new_indent);
	}
	free(subs);
#undef push
}

// strjoin
char *strjoin(char *str0, char *str1, char *str2) {
	int   len0 = str0 ? strlen(str0) : 0;
	int   len1 = str1 ? strlen(str1) : 0;
	int   len2 = str2 ? strlen(str2) : 0;
	char *res  = allocate(len0 + len1 + len2 + 1, 1);
	if (str0) strcpy(res, str0);
	if (str1) strcpy(res + len0, str1);
	if (str2) strcpy(res + len0 + len1, str2);
	return res;
}

// is_data_type
bool is_data_type(Token *token) { return includes(token->type, DATA_TYPES, 0); }

// copy_node
Node *copy_node(Node *node) {
	Node *new  = allocate(1, sizeof(Node));
	new->token = copy_token(node->token);
	if (node->left)  new->left = copy_node(node->left);
	if (node->right) new->right = copy_node(node->right);
	for (int i = 0; i < node->children_count; i++)
		add_child(new, copy_node(node->children[i]));
	for (int i = 0; i < node->structs_count; i++)
		add_struct(new, node->structs[i]);
	for (int i = 0; i < node->variables_count; i++)
		add_variable(new, copy_token(node->variables[i]));
	return new;
}

// hoist_allocas
void hoist_allocas(Node *node) {
	if (!node) return;
	Token *tok = node->token;

	// don't recurse into nested functions
	if (tok->type == FDEC) return;

	if (includes(tok->type, INT, LONG, SHORT, CHARS, CHAR, BOOL, ARRAY_TYPE, 0) && tok->is_dec) {
		if (!tok->llvm.elem)                                                                       _alloca(tok);
	} else if (tok->type == STRUCT_CALL && tok->is_dec && !tok->is_ref) {
		if (!tok->llvm.elem) {
			TypeRef struct_type = get_llvm_type(tok);
			tok->llvm.elem      = LLVMBuildAlloca(builder, struct_type, tok->name);
		}
	}

	if (node->left)  hoist_allocas(node->left);
	if (node->right) hoist_allocas(node->right);
	for (int i = 0; i < node->children_count; i++)
		hoist_allocas(node->children[i]);
}

// load_if_necessary
void load_if_necessary(Node *node) {
	Token *token = node->token;

	if (token->is_ref)                                        return;
	if (includes(token->type, MATH_TYPE, 0))                  return;
	if (includes(token->type, DATA_TYPES, 0) && !token->name) return;
	if (token->llvm.is_loaded)                                return;
	if (includes(token->type, STACK, HEAP, FCALL, 0))         return;

	if (token->name || includes(token->type, ACCESS, DOT, 0)) {
		Token *new          = copy_token(token);
		new->llvm.elem      = load_value(token);
		new->llvm.is_loaded = true;
		node->token         = new;
	}
}

// _alloca
void _alloca(Token *token) {
	TypeRef type = get_llvm_type(token);
	if (token->is_ref) type = LLVMPointerType(type, 0);

	Block current = LLVMGetInsertBlock(builder);
	Value func    = LLVMGetBasicBlockParent(current);
	Block entry   = LLVMGetEntryBasicBlock(func);
	// Walk past leading allocas to find the insertion point
	Value inst = LLVMGetFirstInstruction(entry);
	while (inst && LLVMGetInstructionOpcode(inst) == LLVMAlloca)
		inst = LLVMGetNextInstruction(inst);

	// Position before the first non-alloca instruction (safe even when it's a terminator)
	if (inst) LLVMPositionBuilderBefore(builder, inst);
	else LLVMPositionBuilderAtEnd(builder, entry);

	token->llvm.elem = LLVMBuildAlloca(builder, type, token->name);

	LLVMPositionBuilderAtEnd(builder, current);
}

// get_llvm_type
TypeRef get_llvm_type(Token *token) {
	Type type = token->type;
	if (token->retType)      type = token->retType;
	if (type == STRUCT_DEF)  return token->llvm.stType;
	if (type == TUPLE)       return token->llvm.stType;
	if (type == STRUCT_CALL) {
		if (check(!token->Struct.ptr,
		          "STRUCT_CALL: Struct.ptr is NULL for token '%s' type=%d retType=%d",
		          token->name ? token->name : "(null)", token->type, token->retType))
			return LLVMVoidTypeInContext(context);
		return get_llvm_type(token->Struct.ptr->token);
	}
	if (includes(type, ARRAY, ARRAY_TYPE, 0)) {
		TypeRef base;
		if (token->Array.elem_type == STRUCT_CALL && token->Array.struct_ptr)
			base = get_llvm_type(token->Array.struct_ptr->token);
		else {
			Token tmp = {.type = token->Array.elem_type};
			base      = get_llvm_type(&tmp);
		}
		return LLVMPointerType(base, 0); // flat allocation: always single ptr to base
	}
	// if (type == FCALL)
	//    return get_llvm_type(token->Fcall.ptr->token);
	TypeRef res[END] = {
	    [INT] = i32,  [CHAR] = i8,   [CHARS] = p8,  [BOOL] = i1,  [VOID] = vd,
	    [LONG] = i64, [FLOAT] = f32, [ACCESS] = i8, [SHORT] = i2, [NULLABLE] = p8,
	};
	if (check(!res[type], "handle this case [%s]", to_string(type))) {
		seg();
	}
	return res[type];
}

// _get_default_value
Value _get_default_value(Token *token) {
	TypeRef type = get_llvm_type(token);

	if (token->is_ref) return LLVMConstNull(LLVMPointerType(type, 0));

	if (includes(token->type, NUMERIC_TYPES, 0))     return LLVMConstInt(type, 0, false);
	if (token->type == FLOAT)                        return LLVMConstReal(type, 0.0);
	if (includes(token->type, CHARS, ARRAY_TYPE, 0)) return LLVMConstNull(type);
	check(1, "handle this case %s", to_string(token->type));
	return NULL;
}
