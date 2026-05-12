#include "header.h"

bool  found_error;        // scoped: "current parse path is broken" (cleared by parser_recover)
int   error_count;        // persistent: total errors reported across the run
int   max_errors = 25;    // cap before bailing entirely
bool  enable_debug = true;
bool  enable_san;
bool  enable_prep;
char *flags;
char *ura_lib;
const char *ura_target_os =
#if defined(__APPLE__)
    "macos";
#elif defined(__linux__)
    "linux";
#else
    "unknown";
#endif

EXPAND(Source **, sources);
EXPAND(Token **, tokens);
int exe_count;
EXPAND(Node **, scopes);
Node *scope;

Node *ura_scope;
char *current_gen_module;

// Per-file path state, populated by setup_paths(), released by free_memory().
char            *dir;
char            *base;
char            *build_dir;
char            *ll_path;

Context          context;
Module           module;
Builder          builder;
TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;

LLVMDIBuilderRef debug_builder;
LLVMMetadataRef  debug_compile_unit;
LLVMMetadataRef  debug_file;
LLVMMetadataRef  debug_scope;

// ============================================================================
// ERROR CHECK, DEBUGGING, MEMORY
// ============================================================================
void *allocate(int len, int size) {
	void *res = calloc(len, size);
	TODO(!res, "allocate did failed");
	return res;
}

char *generate_list_source(const char *elem, const char *sname) {
	char *src = allocate(LIST_SOURCE_MAX, sizeof(char));
	snprintf(src, LIST_SOURCE_MAX,
	         "struct %s:\n"
	         "   data array[%s]\n"
	         "   __len int\n"
	         "   __cap int\n"
	         "\n"
	         "   operator delete() void:\n"
	         "      free(self.data as pointer)\n"
	         "\n"
	         "   fn push(e %s) void:\n"
	         "      if self.__len >= self.__cap:\n"
	         "         if self.__cap == 0: self.__cap = 8\n"
	         "         else: self.__cap *= 2\n"
	         "         self.data = realloc(self.data as pointer, self.__cap * sizeof(%s))\n"
	         "      self.data[self.__len] = e\n"
	         "      self.__len += 1\n"
	         "\n"
	         "   fn pop() %s:\n"
	         "      self.__len -= 1\n"
	         "      return self.data[self.__len]\n"
	         "\n"
	         "   fn len() int:\n"
	         "      return self.__len\n"
	         "\n"
	         "   fn cap() int:\n"
	         "      return self.__cap\n"
	         "\n"
	         "   fn foreach(cb fn(%s) void) void:\n"
	         "      i int = 0\n"
	         "      while i < self.__len:\n"
	         "         cb(self.data[i])\n"
	         "         i += 1\n",
	         sname, elem, elem, elem, elem, elem);
	return src;
}

char *open_file(char *path_name) {
	char *file_name = realpath(path_name, NULL);
	if (!file_name) {
		parse_error(NULL, "cannot find file '%s'", path_name);
		return NULL;
	}
	File file = fopen(file_name, "r");
	if (!file) {
		parse_error(NULL, "cannot open file '%s'", file_name);
		free(file_name);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *input = allocate((size + 1), sizeof(char));
	fread(input, size, sizeof(char), file);
	fclose(file);
	free(file_name);
	return input;
}

Source *new_source(char *file_name) {
	Source *src   = allocate(1, sizeof(Source));
	src->filename = file_name;
	src->content  = open_file(file_name);
	resize_array(sources, Source *, sources_size, sources_count);
	sources[sources_count++] = src;
	return src;
}

void free_token(Token *token) {
	free(token->name);
	free(token->Chars.value);
	free(token->llvm.dims);
	free(token);
}

void free_node(Node *node) {
	if (!node) return;
	// SYNTAX_ERROR is a shared static sentinel — pointer-compare so we
	// don't dereference node->token, which has already been freed by
	// free_memory()'s earlier free_token() loop.
	if (node == syntax_error_node) return;
	for (int i = 0; i < node->children_count; i++)
		free_node(node->children[i]);
	free_node(node->left);
	free_node(node->right);
	free(node->children);
	free(node->functions);
	free(node->variables);
	free(node->structs);
	free(node->modules);
	free(node->auto_cleans);
	free(node);
}

void free_memory() {
	for (int i = 0; tokens && i < tokens_count; i++) {
		free_token(tokens[i]);
	}
	free(tokens);
	free(scopes);
	free_node(ura_scope);
	free(dir);
	free(base);
	free(build_dir);
	free(ll_path);
	for (int i = 0; sources && i < sources_count; i++) {
		free(sources[i]->content);
		free(sources[i]);
	}
	free(sources);
}

int _vprint(File out, char *conv, va_list args) {
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
				if (token->ret_type)    fprintf(out, "ret [%s] ", to_string(token->ret_type));
				if (token->is_variadic) fprintf(out, "variadic ");
				break;
			}
			default: TODO(1, "invalid format specifier [%c]", conv[i]); break;
			}
		}
	}
	return res;
}

bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...) {
	if (!cond) return cond;
	found_error = true;
	error_count++;
	fprintf(stderr, RED("ura_error: %s %s:%d "), funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	_vprint(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

void parse_error(Token *token, const char *fmt, ...) {
	found_error = true;
	error_count++;
	if (error_count > max_errors) {
		fprintf(stderr, RED("error: ") "too many errors, stopping\n");
		exit(1);
	}

	fprintf(stderr, RED("error: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);

	if (!token || !token->source || !token->source->content) return;

	char *content = token->source->content;
	int   s       = token->start_index;
	int   e       = token->end_index;
	if (e <= s) e = s + 1;

	int line_start = s;
	while (line_start > 0 && content[line_start - 1] != '\n')
		line_start--;
	int line_end = e;
	while (content[line_end] && content[line_end] != '\n')
		line_end++;

	int col     = s - line_start + 1;
	int span    = e - s;
	int line_no = token->line;
	int gutter  = 1;
	for (int n = line_no; n >= 10; n /= 10)
		gutter++;

	fprintf(stderr, "%*s " BLUE("-->") " %s:%d:%d\n", gutter, "", token->source->filename, line_no,
	        col);
	fprintf(stderr, "%*s " BLUE("|") "\n", gutter, "");
	fprintf(stderr, BLUE("%*d |") " %.*s\n", gutter, line_no, line_end - line_start,
	        content + line_start);
	fprintf(stderr, "%*s " BLUE("|") " ", gutter, "");
	for (int i = 0; i < col - 1; i++)
		fputc(content[line_start + i] == '\t' ? '\t' : ' ', stderr);
	fprintf(stderr, "\033[1;31m");
	for (int i = 0; i < span; i++)
		fputc('^', stderr);
	fprintf(stderr, RESET "\n");
}

void tokenize_error(Source *src, int line, int s, int e, const char *fmt, ...) {
	Token tok       = {0};
	tok.source      = src;
	tok.line        = line;
	tok.start_index = s;
	tok.end_index   = e;

	va_list ap;
	va_start(ap, fmt);
	char *msg = NULL;
	int   len = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	if (len >= 0) {
		msg = allocate(len + 1, sizeof(char));
		va_start(ap, fmt);
		vsnprintf(msg, len + 1, fmt, ap);
		va_end(ap);
	}
	parse_error(&tok, "%s", msg ? msg : fmt);
	free(msg);
}

int _debug(char *conv, ...) {
	va_list args;
	va_start(args, conv);
	int res = _vprint(stdout, conv, args);
	va_end(args);
	return res;
}

void pnode(Node *node, char *indent) {
	if (!node || !node->token || !enable_debug) return;
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
		if (node->left) push(node->left);
		for (int i = 0; i < node->children_count; i++)
			push(node->children[i]);
		if (node->right) push(node->right);
	} else {
		if (node->left)  push(node->left);
		if (node->right) push(node->right);
		for (int i = 0; i < node->children_count; i++)
			push(node->children[i]);
	}
	for (int i = 0; i < node->modules_count; i++)
		push(node->modules[i]);
	for (int i = 0; i < node->structs_count; i++)
		push(node->structs[i]);
	for (int i = 0; i < node->functions_count; i++)
		push(node->functions[i]);
	for (int i = 0; i < count; i++) {
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int         is_last = (i == count - 1);
		const char *bar     = is_last ? "   " : "│  ";

		char       *new_indent = format("%s%s", indent, bar);

		char       *connector = is_last ? "└──" : "├──";
		debug("%s%s", indent, connector);
		pnode(child, new_indent);
		free(new_indent);
	}
	free(subs);
#undef push
}

// ============================================================================
// PARSING
// ============================================================================
int parse_escape_seq(char *input, int s, int e, char *buf, int *j) {
	switch (input[s + 1]) {
	case 'n':  buf[(*j)++] = '\n'; return s + 1;  // newline
	case 't':  buf[(*j)++] = '\t'; return s + 1;  // tab
	case 'r':  buf[(*j)++] = '\r'; return s + 1;  // carriage return
	case 'b':  buf[(*j)++] = '\b'; return s + 1;  // backspace
	case 'f':  buf[(*j)++] = '\f'; return s + 1;  // form feed
	case 'v':  buf[(*j)++] = '\v'; return s + 1;  // vertical tab
	case 'a':  buf[(*j)++] = '\a'; return s + 1;  // alert (bell)
	case '\\': buf[(*j)++] = '\\'; return s + 1; // backslash
	case '"':  buf[(*j)++] = '"'; return s + 1;   // double quote
	case '\'': buf[(*j)++] = '\''; return s + 1; // single quote
	case '?':  buf[(*j)++] = '\?'; return s + 1;  // question mark (trigraph)
	case '0':  {
		// three-digit octal: \0NN
		if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[(*j)++] = (char)octal;
				return s + 3;
			}
			buf[(*j)++] = '\0';
			return s + 1;
		}
		// two-digit octal: \0N
		else if (s + 1 < e && isdigit(input[s + 2])) {
			int octal   = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[(*j)++] = (char)octal;
			return s + 2;
		}
		// plain null
		buf[(*j)++] = '\0';
		return s + 1;
	}
	case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
		// three-digit octal: \NNN
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[(*j)++] = (char)octal;
				return s + 3;
			}
			buf[(*j)++] = input[s];
			return s + 1; // invalid, keep backslash
		}
		// two-digit octal
		else if (s + 2 < e && isdigit(input[s + 2])) {
			int octal   = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[(*j)++] = (char)octal;
			return s + 2;
		}
		// single-digit octal
		buf[(*j)++] = (char)(input[s + 1] - '0');
		return s + 1;
	}
	case 'x': // Hexadecimal: \xFF
	{
		if (s + 3 < e && isxdigit(input[s + 2]) && isxdigit(input[s + 3])) {
			int  hex = 0;
			char c1  = input[s + 2];
			char c2  = input[s + 3];
			if (c1 >= '0' && c1 <= '9')      hex += (c1 - '0') * 16;
			else if (c1 >= 'a' && c1 <= 'f') hex += (c1 - 'a' + 10) * 16;
			else if (c1 >= 'A' && c1 <= 'F') hex += (c1 - 'A' + 10) * 16;
			if (c2 >= '0' && c2 <= '9')      hex += (c2 - '0');
			else if (c2 >= 'a' && c2 <= 'f') hex += (c2 - 'a' + 10);
			else if (c2 >= 'A' && c2 <= 'F') hex += (c2 - 'A' + 10);
			buf[(*j)++] = (char)hex;
			return s + 3;
		}
		buf[(*j)++] = input[s]; // invalid hex escape, keep backslash
		return s + 1;
	}
	case 'u': // \uXXXX — not fully implemented yet
		buf[(*j)++] = input[s];
		return s + 1;
	case 'U': // \UXXXXXXXX — not fully implemented yet
		buf[(*j)++] = input[s];
		return s + 1;
	default:
		buf[(*j)++] = input[s]; // unknown escape, keep backslash
		return s + 1;
	}
}

char *format(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);
	if (CHECK(len < 0, "format: vsnprintf measure failed")) return NULL;

	char *buf = allocate(len + 1, sizeof(char));
	va_start(ap, fmt);
	vsnprintf(buf, len + 1, fmt, ap);
	va_end(ap);
	return buf;
}

char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
	    [INT] = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
	    [FDEC] = "FDEC", [FCALL] = "CALL", [END] = "END", [LPAR] = "LPAR",
	    [RPAR] = "RPAR", [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
	    [WHILE] = "WHILE", [FOR] = "FOR", [TO] = "TO",
	    [STEP] = "STEP", [IN] = "IN", [BREAK] = "BRK", [CONTINUE] = "CONT",
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
	    [ARRAY_TYPE] = "ARRAY_TYPE", [ARRAY_LIT] = "ARRAY_LIT",
	    [NULLABLE] = "NULLABLE",
	    //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
	    [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
	    [ENUM_DEF] = "ENUM_DEF", [ENUM_CALL] = "ENUM_CALL", [TUPLE] = "TUPLE",
	    [TUPLE_UNPACK] = "TUPLE_UNPACK", [LBRA] = "LBRA", [RBRA] = "RBRA",
	    [ARRAY] = "ARRAY", [LIST] = "LIST", [LIST_TYPE] = "LIST_TYPE",
	    [FN_TYPE] = "FN_TYPE",
	    [DOT] = "DOT", [SYNTAX_ERROR] = "SYNTAX_ERROR", [MODULE] = "MODULE",
	    [OPERATOR] = "OPERATOR_KW", [PUB] = "PUB",
	    [DOUBLE_DOTS] = "DOUBLE_DOTS", [DELETE] = "DELETE",
	};

	if (CHECK(!res[type], "handle this case %d\n", type)) {
		// SEG();
		exit(1);
	}
	return res[type];
}

bool includes(Type to_find, ...) {
	if (found_error) return false;
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

// ============================================================================
// INTERMEDIATE REPRESENTATION
// ============================================================================
void add_variable(Node *parent, Token *token) {
	resize_array(parent->variables, Token *, parent->variables_size, parent->variables_count);
	parent->variables[parent->variables_count++] = token;
}

Token *new_variable(Token *token) {
	debug(CYAN("new variable [%k] in scope %k\n"), token, scope->token);
	for (int i = 0; i < scope->variables_count; i++) {
		Token *curr = scope->variables[i];
		if (strcmp(curr->name, token->name) == 0) {
			parse_error(token, "redefinition of '%s'", token->name);
			return token;
		}
	}
	for (int j = scopes_count - 1; j > 0; j--) {
		Node *outer = scopes[j];
		for (int i = 0; i < outer->variables_count; i++) {
			Token *curr = outer->variables[i];
			if (curr->is_global && strcmp(curr->name, token->name) == 0) {
				parse_error(token, "'%s' shadows a global of the same name", token->name);
				return token;
			}
		}
	}
	if (scopes_count == 1) token->is_global = true;
	add_variable(scope, token);
	return token;
}

// Silent lookup — returns NULL on miss without firing an error.
// Caller (typically ir_id) decides whether to error or fall back to a
// function-reference lookup.
Token *get_variable(char *name) {
	debug(CYAN("get variable [%s] from scope %k, has %d vars\n"), name, scope->token,
	      scope->variables_count);
	for (int j = scopes_count; j > 0; j--) {
		Node *scope = scopes[j];
		for (int i = 0; i < scope->variables_count; i++) {
			CHECK(scope->variables[i] == NULL, "unexpected error variables");
			CHECK(scope->variables[i]->name == NULL, "unexpected error name");
			if (strcmp(scope->variables[i]->name, name) == 0) return scope->variables[i];
		}
	}
	return NULL;
}

void add_function(Node *parent, Node *node) {
	resize_array(parent->functions, Node *, parent->functions_size, parent->functions_count);
	parent->functions[parent->functions_count++] = node;
}

Node *new_function(Node *node) {
	for (int i = 0; i < scope->functions_count; i++) {
		Node *func = scope->functions[i];
		if (strcmp(func->token->name, node->token->name) == 0) {
			parse_error(node->token, "redefinition of function '%s'", node->token->name);
			return node;
		}
	}
	add_function(scope, node);
	return node;
}

Node *find_function(char *name) {
	for (int j = scopes_count; j > 0; j--) {
		Node *sc = scopes[j];
		for (int i = 0; i < sc->functions_count; i++)
			if (strcmp(sc->functions[i]->token->name, name) == 0) return sc->functions[i];
	}
	if (current_gen_module) {
		char *qname = format("%s.%s", current_gen_module, name);
		for (int j = scopes_count; j > 0; j--) {
			Node *sc = scopes[j];
			for (int i = 0; i < sc->functions_count; i++) {
				if (strcmp(sc->functions[i]->token->name, qname) == 0) {
					free(qname);
					return sc->functions[i];
				}
			}
		}
		free(qname);
	}
	return NULL;
}

Node *get_function(char *name) {
	Node *f = find_function(name);
	if (f) return f;
	parse_error(NULL, "function '%s' not found", name);
	return NULL;
}

Node *new_struct(Node *node) {
	debug(CYAN("new struct [%s] in scope %k\n"), node->token->name, scope->token);
	for (int i = 0; i < scope->structs_count; i++) {
		Token *curr = scope->structs[i]->token;
		if (strcmp(curr->name, node->token->name) == 0) {
			parse_error(node->token, "redefinition of struct '%s'", node->token->name);
			return node;
		}
	}
	resize_array(scope->structs, Node *, scope->structs_size, scope->structs_count);
	scope->structs[scope->structs_count++] = node;
	return node;
}

void add_struct(Node *parent, Node *node) {
	resize_array(parent->structs, Node *, parent->structs_size, parent->structs_count);
	parent->structs[parent->structs_count++] = node;
}

void add_auto_clean(Node *parent, Value value, Node *type) {
	resize_array(parent->auto_cleans, AutoClean, parent->auto_cleans_size,
	             parent->auto_cleans_count);
	parent->auto_cleans[parent->auto_cleans_count].value = value;
	parent->auto_cleans[parent->auto_cleans_count].type  = type;
	parent->auto_cleans_count++;
}

Node *get_struct(char *name) {
	debug(CYAN("get struct [%s] from scope %k\n"), name, scope->token);
	for (int j = scopes_count; j > 0; j--) {
		Node *node = scopes[j];
		TODO(node == NULL, RED("Error accessing NULL, %d"), j);
		for (int i = 0; i < node->structs_count; i++)
			if (strcmp(node->structs[i]->token->name, name) == 0) return node->structs[i];
	}
	return NULL;
}

const char *type_to_ura_name(Type type) {
	switch (type) {
	case INT:   return "int";
	case LONG:  return "long";
	case SHORT: return "short";
	case CHAR:  return "char";
	case CHARS: return "chars";
	case BOOL:  return "bool";
	case FLOAT: return "float";
	case PTR:   return "pointer";
	case VOID:  return "void";
	default:    return NULL;
	}
}

Type get_ret_type(Node *node) {
	if (!node || !node->token) return 0;
	Token *token = node->token;
	Node  *left  = node->left;
	Node  *right = node->right;
	if (token->ret_type) return token->ret_type;
	if (includes(token->type, INT, BOOL, CHAR, FLOAT, LONG, VOID, PTR, CHARS, STRUCT_CALL, 0))
		return token->type;
	if (token->type == FCALL)                                         return token->Fcall.ptr ? token->Fcall.ptr->token->ret_type : 0;
	if (includes(token->type, MATH_TYPE, ASSIGN, ASSIGNS_OP, 0))      return get_ret_type(left);
	if (includes(token->type, COMPARISON_OPS, AND, OR, NOT, BNOT, 0)) return BOOL;
	if (token->type == AS)
		return right ? (right->token->ret_type ? right->token->ret_type : right->token->type) : 0;
	if (token->type == RETURN) return get_ret_type(left);
	if (token->type == DOT)    return get_ret_type(right);
	if (token->type == ACCESS) {
		Type left_type = get_ret_type(left);
		if (left_type == CHARS || left_type == STACK) return CHAR;
		return left_type;
	}
	if (token->type == ID) return token->type != ID ? token->type : 0;
	TODO(1, "handled this case [%s]", to_string(token->type));
	return 0;
}

void set_ret_type(Node *node) {
	if (!node || !node->token) return;
	Type t = get_ret_type(node);
	if (t) node->token->ret_type = t;
	if (node->token->type == DOT && node->token->ret_type == STRUCT_CALL && node->right)
		node->token->Struct.ptr = node->right->token->Struct.ptr;
	if (node->token->type == FCALL && node->token->ret_type == STRUCT_CALL && node->token->Fcall.ptr)
		node->token->Struct.ptr = node->token->Fcall.ptr->token->Struct.ptr;
}

bool compatible(Token *left, Token *right) {
	Type lt = left->ret_type ? left->ret_type : left->type;
	Type rt = right->ret_type ? right->ret_type : right->type;

	if (lt == rt)                                                 return true;
	if ((lt == CHARS && rt == PTR) || (lt == PTR && rt == CHARS)) return true;
	if (lt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && right->Array.sub_type == CHAR)
		return true;
	if (rt == CHARS && includes(lt, ARRAY_TYPE, ARRAY, 0) && left->Array.sub_type == CHAR)
		return true;
	if (lt == ARRAY_TYPE && rt == ARRAY) return true;
	if (lt == ARRAY && rt == ARRAY_TYPE) return true;
	// pointer/chars <-> array[T]: all pointers at LLVM level
	// TODO: it should be checked, right might be array of something else than CHAR
	if ((includes(lt, PTR, CHARS, 0) && includes(rt, ARRAY_TYPE, ARRAY, 0)) ||
	    (includes(rt, PTR, CHARS, 0) && includes(lt, ARRAY_TYPE, ARRAY, 0)))
		return true;
	bool lt_numeric = includes(lt, NUMERIC_TYPES, 0);
	bool rt_numeric = includes(rt, NUMERIC_TYPES, 0);
	if (lt_numeric && rt_numeric && lt == rt)   return true;
	if (lt == STRUCT_CALL && rt == STRUCT_CALL) return left->Struct.ptr == right->Struct.ptr;
	if (lt == FN_TYPE && rt == FN_TYPE) {
		if (left->Fn.params_count != right->Fn.params_count) return false;
		for (int i = 0; i < left->Fn.params_count; i++)
			if (!compatible(left->Fn.params[i], right->Fn.params[i])) return false;
		// ret may be NULL if void; treat both-NULL as compatible
		if (!left->Fn.ret && !right->Fn.ret) return true;
		if (!left->Fn.ret || !right->Fn.ret) return false;
		return compatible(left->Fn.ret, right->Fn.ret);
	}
	return false;
}

// ============================================================================
// ASSEMBLY GENERATION
// ============================================================================

void setup_paths(char *path_name) {
	char *tmp_dir  = strdup(path_name);
	char *tmp_base = strdup(path_name);
	dir            = strdup(dirname(tmp_dir));
	base           = strdup(basename(tmp_base));
	free(tmp_dir);
	free(tmp_base);

	char *dot_ext = strrchr(base, '.');
	if (dot_ext) *dot_ext = '\0';

	build_dir = format("%s/build", dir);
	mkdir(build_dir, 0755);
	char *base_ll = format("%s.ll", base);
	ll_path       = format("%s/%s", build_dir, base_ll);
	free(base_ll);
}

void init(char *name) {
	context = LLVMContextCreate();
	module  = LLVMModuleCreateWithNameInContext(name, context);
	builder = LLVMCreateBuilderInContext(context);

	vd  = LLVMVoidTypeInContext(context);
	f32 = LLVMFloatTypeInContext(context);
	i1  = LLVMInt1TypeInContext(context);
	i2  = LLVMIntTypeInContext(context, 2);
	i4  = LLVMIntTypeInContext(context, 4);
	i8  = LLVMInt8TypeInContext(context);
	i16 = LLVMInt16TypeInContext(context);
	i32 = LLVMInt32TypeInContext(context);
	i64 = LLVMInt64TypeInContext(context);
	p8  = LLVMPointerType(i8, 0);
	p32 = LLVMPointerType(i32, 0);

	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();
	LLVMInitializeNativeAsmParser();
#if defined(__APPLE__)
	LLVMSetTarget(module, "arm64-apple-macosx16.0.0");
#elif defined(__linux__)
	LLVMSetTarget(module, "x86_64-pc-linux-gnu");
#else
	LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
#endif

	// if (enable_san)
	// {
	LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Debug Info Version", 18,
	                  LLVMValueAsMetadata(LLVMConstInt(i32, 3, 0)));
	LLVMAddModuleFlag(module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13,
	                  LLVMValueAsMetadata(LLVMConstInt(i32, 4, 0)));
	// }

	// Debug info
	debug_builder       = LLVMCreateDIBuilder(module);
	char *base          = strrchr(name, '/');
	char *filename_only = base ? base + 1 : name;
	char *dir           = allocate(URA_MAX_SIZE, 1);
	strcpy(dir, ".");
	if (base) {
		size_t len = base - name;
		strncpy(dir, name, len);
		dir[len] = '\0';
	}
	debug_file = LLVMDIBuilderCreateFile(debug_builder, filename_only, strlen(filename_only), dir,
	                                     strlen(dir));
	debug_compile_unit = LLVMDIBuilderCreateCompileUnit(
	    debug_builder, LLVMDWARFSourceLanguageC, debug_file, "ura", 3, 0, "", 0, 0, "", 0,
	    LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);

	debug_scope = debug_compile_unit;
	free(dir);
}

void finalize(char *output) {
	char *error = NULL;

	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();

	LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();

	if (flags) {
		LLVMErrorRef err = LLVMRunPasses(module, flags, NULL, options);
		if (err) {
			char *msg = LLVMGetErrorMessage(err);
			CHECK(1, "Optimizer Error: %s\n", msg);
			LLVMDisposeErrorMessage(msg);
			found_error = true;
			return;
		}
	}

	LLVMDIBuilderFinalize(debug_builder);
	LLVMDisposeDIBuilder(debug_builder);
	debug_builder = NULL;

	if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
		CHECK(1, "Module verification failed:\n%s\n", error);
		LLVMDisposeMessage(error);
		LLVMDisposePassBuilderOptions(options);
		found_error = true;
	} else {
		LLVMDisposePassBuilderOptions(options);
	}

	LLVMPrintModuleToFile(module, output, NULL);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMContextDispose(context);
}

void set_debug_location(Token *token) {
	if (!token || !debug_builder || !debug_scope)              return;
	if (!includes(token->type, ACCESS, FDEC, PROTO, FCALL, 0)) return;
	LLVMMetadataRef loc =
	    LLVMDIBuilderCreateDebugLocation(context, token->line, 0, debug_scope, NULL);
	LLVMSetCurrentDebugLocation2(builder, loc);
}

TypeRef get_llvm_type(Token *token) {
	Type type = token->type;
	if (token->ret_type)     type = token->ret_type;
	if (type == STRUCT_DEF)  return token->llvm.struct_type;
	if (type == TUPLE)       return token->llvm.struct_type;
	if (type == STRUCT_CALL) {
		if (CHECK(!token->Struct.ptr, "STRUCT_CALL: Struct.ptr is NULL for %k", token))
			return LLVMVoidTypeInContext(context);
		return get_llvm_type(token->Struct.ptr->token);
	}
	if (includes(type, ARRAY, ARRAY_TYPE, 0)) {
		TypeRef base;
		if (token->Array.sub_type == STRUCT_CALL && token->Array.struct_ptr)
			base = get_llvm_type(token->Array.struct_ptr->token);
		else {
			Token tmp = {.type = token->Array.sub_type};
			base      = get_llvm_type(&tmp);
		}
		return LLVMPointerType(base, 0);
	}
	if (type == FN_TYPE) {
		TypeRef ret_t = token->Fn.ret ? get_llvm_type(token->Fn.ret) : vd;
		int n = token->Fn.params_count;
		TypeRef *ptypes = NULL;
		if (n > 0) {
			ptypes = allocate(n, sizeof(TypeRef));
			for (int i = 0; i < n; i++) ptypes[i] = get_llvm_type(token->Fn.params[i]);
		}
		TypeRef ft = LLVMFunctionType(ret_t, ptypes, n, 0);
		free(ptypes);
		return LLVMPointerType(ft, 0);
	}
	// if (type == FCALL) return get_llvm_type(token->Fcall.ptr->token);
	TypeRef res[END] = {
	    [INT] = i32,  [CHAR] = i8,   [CHARS] = p8,  [BOOL] = i1,  [VOID] = vd,
	    [LONG] = i64, [FLOAT] = f32, [ACCESS] = i8, [SHORT] = i2, [NULLABLE] = p8,
	};
	TODO(!res[type], "handle this case %k", token);
	return res[type];
}

Value _get_default_value(Token *token) {
	TypeRef type = get_llvm_type(token);

	if (token->is_ref) return LLVMConstNull(LLVMPointerType(type, 0));

	if (includes(token->type, NUMERIC_TYPES, 0))     return LLVMConstInt(type, 0, false);
	if (token->type == FLOAT)                        return LLVMConstReal(type, 0.0);
	if (includes(token->type, CHARS, ARRAY_TYPE, 0)) return LLVMConstNull(type);
	if (token->type == FN_TYPE)                      return LLVMConstNull(type);
	TODO(1, "handle this case %k", token);
	return NULL;
}

Value read_value(Token *token) {
	if (token->llvm.is_loaded) return token->llvm.elem;
	// Already computed values: function returns, literals, allocated buffers
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

void write_value(Token *token, Value val) {
	Value dest = token->llvm.elem;
	if (token->is_ref) {
		TypeRef type = get_llvm_type(token);
		dest         = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), token->llvm.elem, "ref_ptr");
	}
	LLVMBuildStore(builder, val, dest);
}

void ensure_loaded(Node *node) {
	Token *token = node->token;

	if (token->is_ref)                                        return;
	if (includes(token->type, MATH_TYPE, 0))                  return;
	if (includes(token->type, DATA_TYPES, 0) && !token->name) return;
	if (token->llvm.is_loaded)                                return;
	if (includes(token->type, STACK, HEAP, FCALL, 0))         return;

	if (token->name || includes(token->type, ACCESS, DOT, 0)) {
		Token *new          = copy_token(token);
		new->llvm.elem      = read_value(token);
		new->llvm.is_loaded = true;
		node->token         = new;
	}
}

void _const_value(Token *token) {
	TypeRef   type  = get_llvm_type(token);
	long long value = 0;

	switch (token->type) {
	case INT:   value = (long long)token->Int.value; break;
	case BOOL:  value = (long long)token->Bool.value; break;
	case CHAR:  value = (int)token->Char.value; break;
	case FLOAT: {
		token->llvm.elem      = LLVMConstReal(type, (double)token->Float.value);
		token->llvm.is_loaded = true;
		return;
	}
	case CHARS: {
		// TODO: to be checked
		char       name[200];
		char      *processed;
		static int index = 0;
		snprintf(name, sizeof(name), "STR%d", index++);
		processed = allocate(strlen(token->Chars.value) * 2 + 1, 1);
		int j     = 0;
		for (int i = 0; token->Chars.value[i]; i++) {
			if (token->Chars.value[i] == '\\' && token->Chars.value[i + 1]) {
				switch (token->Chars.value[i + 1]) {
				case 'n':  processed[j++] = '\n'; i++; break;
				case 't':  processed[j++] = '\t'; i++; break;
				case 'r':  processed[j++] = '\r'; i++; break;
				case '0':  processed[j++] = '\0'; i++; break;
				case '\\': processed[j++] = '\\'; i++; break;
				case '\"': processed[j++] = '\"'; i++; break;
				case '\'': processed[j++] = '\''; i++; break;
				default:   processed[j++] = token->Chars.value[i]; break;
				}
			} else {
				processed[j++] = token->Chars.value[i];
			}
		}
		token->llvm.elem = LLVMBuildGlobalStringPtr(builder, processed, name);
		free(processed);
		return;
	}
	default: CHECK(1, "handle this case %s", to_string(token->type)); return;
	}
	token->llvm.elem = LLVMConstInt(type, value, 0);
}

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

TypeRef func_ret_type(Token *token) {
	if (token->ret_type == TUPLE) {
		int      n  = token->Tuple.types_count;
		TypeRef *ft = allocate(n, sizeof(TypeRef));
		for (int i = 0; i < n; i++)
			ft[i] = get_llvm_type(token->Tuple.types[i]);
		TypeRef st              = LLVMStructTypeInContext(context, ft, n, 0);
		token->llvm.struct_type = st;
		free(ft);
		return st;
	}
	if (token->is_proto && token->ret_type == STRUCT_CALL) return i64;
	if (token->ret_type == STRUCT_CALL && token->is_ref)
		return LLVMPointerType(get_llvm_type(token), 0);
	return get_llvm_type(token);
}

TypeRef *func_param_types(Node *fdec, int *out_count) {
	Token *token       = fdec->token;
	int    fixed_count = fdec->left->children_count;
	// Ura-defined variadics carry an extra i32 arg-count slot; libc protos
	// use their real signature (no count slot) because the count would be
	// consumed by the callee's first vararg slot under SysV ABI.
	bool   add_count   = token->is_variadic && !token->is_proto;
	int    total       = fixed_count + (add_count ? 1 : 0);
	*out_count         = total;
	if (fixed_count == 0 && !add_count) return NULL;

	TypeRef *types = allocate(total + 2, sizeof(TypeRef));
	for (int i = 0; i < fixed_count; i++) {
		Token *param = fdec->left->children[i]->token;
		if (token->is_proto && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0)) types[i] = i64;
		else if (param->is_ref)                                                   types[i] = LLVMPointerType(get_llvm_type(param), 0);
		else types[i] = get_llvm_type(param);
	}
	if (add_count) types[fixed_count] = i32;
	return types;
}

// TODO: delete this one because I'm already handling function existence
Value _add_function(char *name, TypeRef function_type) {
	Value f = LLVMGetNamedFunction(module, name);
	if (f) return f;
	return LLVMAddFunction(module, name, function_type);
}

LLVMIntPredicate icmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMIntSLT;
	case GREAT:       return LLVMIntSGT;
	case EQUAL:       return LLVMIntEQ;
	case LESS_EQUAL:  return LLVMIntSLE;
	case GREAT_EQUAL: return LLVMIntSGE;
	default:          return LLVMIntNE;
	}
}

LLVMRealPredicate fcmp_predicate(Type op) {
	switch (op) {
	case LESS:        return LLVMRealOLT;
	case GREAT:       return LLVMRealOGT;
	case EQUAL:       return LLVMRealOEQ;
	case LESS_EQUAL:  return LLVMRealOLE;
	case GREAT_EQUAL: return LLVMRealOGE;
	default:          return LLVMRealONE;
	}
}

int is_float_value(Value v) {
	LLVMTypeKind k = LLVMGetTypeKind(LLVMTypeOf(v));
	return k == LLVMFloatTypeKind || k == LLVMDoubleTypeKind;
}

Type assign_base_op(Type assign_op) {
	switch (assign_op) {
	case ADD_ASSIGN: return ADD;
	case SUB_ASSIGN: return SUB;
	case MUL_ASSIGN: return MUL;
	case DIV_ASSIGN: return DIV;
	default:         return MOD;
	}
}

TypeRef _named_struct_type(char *name, TypeRef *element_types, unsigned element_count, int packed) {
	TypeRef type = LLVMStructCreateNamed(context, name);
	LLVMStructSetBody(type, element_types, element_count, packed);
	return type;
}

void _branch(Block bloc) {
	if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) LLVMBuildBr(builder, bloc);
}

Block _append_block(char *name) {
	Value parent = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
	return LLVMAppendBasicBlockInContext(context, parent, name);
}

Value struct_field_ptr(Token *struct_tok, int field_index, char *name) {
	TypeRef struct_type = get_llvm_type(struct_tok);
	Value   indices[]   = {
	        LLVMConstInt(i32, 0, 0),
	        LLVMConstInt(i32, field_index, 0),
   };
	return LLVMBuildGEP2(builder, struct_type, struct_tok->llvm.elem, indices, 2, name);
}

Value allocate_stack(Value size, TypeRef element_type, char *name) {
	Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, 0, 0)};
	if (LLVMIsConstant(size)) {
		unsigned long long n     = LLVMConstIntGetZExtValue(size);
		TypeRef            atype = LLVMArrayType(element_type, n);
		Value              alc   = LLVMBuildAlloca(builder, atype, name);
		return LLVMBuildGEP2(builder, atype, alc, indices, 2, name);
	}
	Value alc = LLVMBuildArrayAlloca(builder, element_type, size, name);
	return LLVMBuildGEP2(builder, element_type, alc, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef element_type, char *name) {
	Value calloc_func = LLVMGetNamedFunction(module, "calloc");
	if (!calloc_func) {
		TypeRef params[]  = {i64, i64};
		TypeRef func_type = LLVMFunctionType(p8, params, 2, 0);
		calloc_func       = _add_function("calloc", func_type);
	}
	TargetData td        = LLVMGetModuleDataLayout(module);
	size_t     elem_size = LLVMABISizeOfType(td, element_type);
	Value      count_i64;
	unsigned   width = LLVMGetIntTypeWidth(LLVMTypeOf(count));
	if (width < 64)      count_i64 = LLVMBuildZExt(builder, count, i64, "count");
	else if (width > 64) count_i64 = LLVMBuildTrunc(builder, count, i64, "count");
	else count_i64 = count;
	Value   size_i64    = LLVMConstInt(i64, elem_size, 0);
	Value   args[]      = {count_i64, size_i64};
	TypeRef calloc_type = LLVMGlobalGetValueType(calloc_func);
	return LLVMBuildCall2(builder, calloc_type, calloc_func, args, 2, name);
}

Value build_binary_op(Type op, Value l, Value r) {
	char *name = to_string(op);
	if (is_float_value(l)) {
		if (includes(op, COMPARISON_OPS, 0))
			return LLVMBuildFCmp(builder, fcmp_predicate(op), l, r, name);
		switch (op) {
		case ADD: return LLVMBuildFAdd(builder, l, r, name);
		case SUB: return LLVMBuildFSub(builder, l, r, name);
		case MUL: return LLVMBuildFMul(builder, l, r, name);
		case DIV: return LLVMBuildFDiv(builder, l, r, name);
		default:  TODO(1, "build_binary_op: unhandled float op %s", name); return NULL;
		}
	}
	if (includes(op, COMPARISON_OPS, 0))
		return LLVMBuildICmp(builder, icmp_predicate(op), l, r, name);
	switch (op) {
	case ADD:    return LLVMBuildAdd(builder, l, r, name);
	case SUB:    return LLVMBuildSub(builder, l, r, name);
	case MUL:    return LLVMBuildMul(builder, l, r, name);
	case DIV:    return LLVMBuildSDiv(builder, l, r, name);
	case MOD:    return LLVMBuildSRem(builder, l, r, name);
	case AND:    return LLVMBuildAnd(builder, l, r, name);
	case OR:     return LLVMBuildOr(builder, l, r, name);
	case BAND:   return LLVMBuildAnd(builder, l, r, name);
	case BOR:    return LLVMBuildOr(builder, l, r, name);
	case BXOR:   return LLVMBuildXor(builder, l, r, name);
	case LSHIFT: return LLVMBuildShl(builder, l, r, name);
	case RSHIFT: return LLVMBuildAShr(builder, l, r, name);
	default:     TODO(1, "build_binary_op: unhandled op %s", name); return NULL;
	}
}

static LLVM ensure_func_resolved(Node *callee) {
	if (callee && callee->token && !callee->token->llvm.func_type)
		resolve_func_type(callee);
	return callee->token->llvm;
}

void resolve_func_type(Node *fdec) {
	Token *token = fdec->token;
	if (token->llvm.func_type) return;

	TypeRef  ret_type = func_ret_type(token);
	int      pcount;
	TypeRef *param_types = func_param_types(fdec, &pcount);
	TypeRef  func_type   = LLVMFunctionType(ret_type, param_types, pcount, token->is_variadic);

	Value    existing     = LLVMGetNamedFunction(module, token->name);
	token->llvm.elem      = existing ? existing : _add_function(token->name, func_type);
	token->llvm.func_type = func_type;
}

void emit_func_body(Node *fdec) {
	Token *token = fdec->token;
	if (token->is_proto) return;

	if (enable_san) {
		unsigned int kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
		AttributeRef attr = LLVMCreateEnumAttribute(context, kind, 0);
		LLVMAddAttributeAtIndex(token->llvm.elem, LLVMAttributeFunctionIndex, attr);
	}

	char       *fname = token->name;
	MetadataRef di_type =
	    LLVMDIBuilderCreateSubroutineType(debug_builder, debug_file, NULL, 0, LLVMDIFlagZero);
	MetadataRef di_func = LLVMDIBuilderCreateFunction(
	    debug_builder, debug_compile_unit, fname, strlen(fname), fname, strlen(fname), debug_file,
	    token->line, di_type, 0, 1, token->line, LLVMDIFlagZero, 0);
	LLVMSetSubprogram(token->llvm.elem, di_func);
	debug_scope = di_func;
	Block entry = LLVMAppendBasicBlockInContext(context, token->llvm.elem, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
	MetadataRef debug_location =
	    LLVMDIBuilderCreateDebugLocation(context, token->line, 0, di_func, NULL);
	LLVMSetCurrentDebugLocation2(builder, debug_location);

	for (int i = 0; i < fdec->left->children_count; i++) {
		Token *p     = fdec->left->children[i]->token;
		Value  param = LLVMGetParam(token->llvm.elem, i);
		LLVMSetValueName(param, p->name);
		_alloca(p);
		p->is_dec = false;
		LLVMBuildStore(builder, param, p->llvm.elem);
	}

#if USING_HOIST
	for (int i = 0; i < fdec->children_count; i++)
		hoist_allocas(fdec->children[i]);
#endif
	for (int i = 0; i < fdec->children_count; i++) {
		asm_gen(fdec->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
}

Node *find_enclosing(Type type) {
	for (int i = scopes_count; i >= 0; i--)
		if (scopes[i] && scopes[i]->token->type == type) return scopes[i];
	return NULL;
}

// ============================================================================
// IR / ASM SUPPORT HELPERS (called by gen.c case dispatchers)
// ============================================================================

Node *find_op_overload(Token *left, Token *right, Type op) {
	if (!left->Struct.ptr) return NULL;
	Type  rt = right->ret_type ? right->ret_type : right->type;
	char *suffix =
	    (rt == STRUCT_CALL && right->Struct.ptr) ? right->Struct.ptr->token->name : to_string(rt);
	char *base = format("%s" OP_PREFIX "%s", left->Struct.ptr->token->name, to_string(op));
	char *name = format("%s.%s", base, suffix);
	free(base);
	Node *func = find_function(name);
	free(name);
	return func;
}

void ir_access_list_struct(Node *node, Node *st) {
	Token *dot_tok               = new_token(DOT, node->left->token->space);
	Node  *dot                   = new_node(dot_tok);
	dot->left                    = node->left;
	Token *field_tok             = copy_token(st->children[0]->token);
	field_tok->Struct.index      = 0;
	dot->right                   = new_node(field_tok);
	dot->token->ret_type         = st->children[0]->token->ret_type;
	dot->token->type             = DOT;
	dot->token->Array.sub_type   = st->children[0]->token->Array.sub_type;
	dot->token->Array.depth      = st->children[0]->token->Array.depth;
	dot->token->Array.struct_ptr = st->children[0]->token->Array.struct_ptr;
	node->left->token->used++;
	node->left = dot;
	Type type  = dot->token->ret_type ? dot->token->ret_type : dot->token->type;
	int  depth = dot->token->Array.depth;
	if (!includes(type, ARRAY_TYPE, ARRAY, 0)) return;
	if (depth > 1)                             {
		node->token->ret_type         = ARRAY;
		node->token->Array.sub_type   = dot->token->Array.sub_type;
		node->token->Array.depth      = depth - 1;
		node->token->Array.struct_ptr = dot->token->Array.struct_ptr;
	} else {
		node->token->ret_type = dot->token->Array.sub_type;
		if (node->token->ret_type == STRUCT_CALL)
			node->token->Struct.ptr = dot->token->Array.struct_ptr;
	}
}

void ir_method_call_args(Node *node, Node *func) {
	Node *dec_args = func->left;
	if (dec_args && node->left) {
		int  got = node->left->children_count - 1; // exclude obj
		int  exp = dec_args->children_count - 1;   // exclude self
		bool ok  = func->token->is_variadic ? (got >= exp) : (got == exp);
		if (!ok) {
			parse_error(node->token,
			            func->token->is_variadic ? "'%s' expects at least %d arguments, got %d"
			                                     : "'%s' expects %d arguments, got %d",
			            node->token->name, exp, got);
			return;
		}
	}
	for (int i = 0; !found_error && i < node->left->children_count - 1; i++) {
		Node *carg = node->left->children[i];
		ir_gen(carg);
		if (carg->token->type == ID) {
			parse_error(carg->token, "undeclared variable '%s'", carg->token->name);
			break;
		}
		carg->token->used++;
		Token *src = carg->token;
		if (i >= dec_args->children_count - 1) continue;
		Type param_type   = dec_args->children[i]->token->type;
		Type arg_type     = src->type;
		bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
		bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
		if (param_is_int && arg_is_int && param_type != arg_type) {
			Token *as_tok           = new_token(AS, src->space);
			as_tok->ret_type        = param_type;
			Node *tgt               = new_node(new_token(param_type, src->space));
			Node *as_n              = new_node(as_tok);
			as_n->left              = carg;
			as_n->right             = tgt;
			node->left->children[i] = as_n;
		} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
			parse_error(carg->token, "'%s' argument %d: cannot pass %s as %s", node->token->name,
			            i + 1, to_string(arg_type), to_string(param_type));
			break;
		}
	}
}

void ir_method_call(Node *node) {
	Node *obj_node = node->left->children[node->left->children_count - 1];
	ir_gen(obj_node);
	if (found_error) return;
	Token *obj = obj_node->token;
	obj->Struct.ptr->token->used++;
	char *struct_name = obj->Struct.ptr->token->name;
	char *qname       = format("%s.%s", struct_name, node->token->name);
	setName(node->token, qname);
	free(qname);
	node->token->is_method_call = false;
	Node *func                  = get_function(node->token->name);
	if (!func)               return;
	if (func->token->is_pub) {
		parse_error(node->token, "'%s' is a `pub fn` — call it with '::' instead of '.'",
		            node->token->name);
		return;
	}
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);
	obj_node->token->used++;
	ir_method_call_args(node, func);
	free_node(func);
	set_ret_type(node);
}

void ir_regular_call_args(Node *node, Node *func) {
	Node *call_args = node->left;
	Node *dec_args  = func->left;
	if (call_args && dec_args) {
		int  got = call_args->children_count;
		int  exp = dec_args->children_count;
		bool ok  = func->token->is_variadic ? (got >= exp) : (got == exp);
		if (!ok) {
			parse_error(node->token,
			            func->token->is_variadic ? "'%s' expects at least %d arguments, got %d"
			                                     : "'%s' expects %d arguments, got %d",
			            node->token->name, exp, got);
			return;
		}
	}
	for (int i = 0; !found_error && call_args && i < call_args->children_count; i++) {
		Node *carg = call_args->children[i];
		ir_gen(carg);
		carg->token->used++;
		Token *src = carg->token;
		if (src->type == ID) {
			parse_error(carg->token, "undeclared variable '%s'", carg->token->name);
			break;
		}
		if (i >= dec_args->children_count) continue;
		bool param_is_ref = dec_args->children[i]->token->is_ref;
		if (param_is_ref &&
		    (src->type == FCALL || (src->type != DOT && src->type != ACCESS && !src->name))) {
			parse_error(carg->token, "'%s': ref parameter requires a named variable",
			            dec_args->children[i]->token->name);
			break;
		}
		Type param_type   = dec_args->children[i]->token->type;
		Type arg_type     = src->type;
		bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
		bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
		if (param_is_int && arg_is_int && param_type != arg_type) {
			Token *as_tok          = new_token(AS, src->space);
			as_tok->ret_type       = param_type;
			Node *tgt              = new_node(new_token(param_type, src->space));
			Node *as_n             = new_node(as_tok);
			as_n->left             = carg;
			as_n->right            = tgt;
			call_args->children[i] = as_n;
		} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
			parse_error(carg->token, "'%s' argument %d: cannot pass %s as %s — use 'value as %s'",
			            node->token->name, i + 1, to_string(arg_type), to_string(param_type),
			            to_string(param_type));
			break;
		}
	}
}

void ir_regular_call(Node *node) {
	Node *func = get_function(node->token->name);
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);
	ir_regular_call_args(node, func);
	free_node(func);
	set_ret_type(node);
}

void ir_indirect_call(Node *node, Token *fn_var) {
	node->token->is_method_call = false;
	node->token->is_static_call = false;
	// Stash the fn variable so codegen can load it.
	node->token->Statement.ptr  = fn_var;

	int got = node->left ? node->left->children_count : 0;
	int exp = fn_var->Fn.params_count;
	if (got != exp) {
		parse_error(node->token, "'%s' expects %d arguments, got %d", node->token->name, exp, got);
		return;
	}

	for (int i = 0; i < got; i++) {
		Node  *carg = node->left->children[i];
		ir_gen(carg);
		if (found_error) return;
		Token *src   = carg->token;
		Token *param = fn_var->Fn.params[i];
		if (!compatible(param, src)) {
			parse_error(carg->token, "'%s' arg %d: cannot pass %s as %s", node->token->name, i + 1,
			            to_string(src->ret_type ? src->ret_type : src->type),
			            to_string(param->ret_type ? param->ret_type : param->type));
			return;
		}
	}

	// Set return type from signature
	if (fn_var->Fn.ret) {
		node->token->ret_type = fn_var->Fn.ret->ret_type ? fn_var->Fn.ret->ret_type
		                                                  : fn_var->Fn.ret->type;
	}
}

void ir_static_call(Node *node) {
	Node *func = get_function(node->token->name);
	if (!func)                return;
	if (!func->token->is_pub) {
		parse_error(node->token, "'%s' is not a `pub fn` — call it with '.' instead of '::'",
		            node->token->name);
		return;
	}
	func->token->used++;
	node->token->Fcall.ptr = func;
	if (func->token->ret_type == STRUCT_CALL) {
		node->token->ret_type   = STRUCT_CALL;
		node->token->Struct.ptr = func->token->Struct.ptr;
	} else node->token->ret_type = func->token->ret_type;
	for (int i = 0; i < node->left->children_count; i++)
		ir_gen(node->left->children[i]);
}

bool try_module_call(Node *node) {
	if (!node->token->is_method_call || !node->left || node->left->children_count == 0) return false;
	Node *last = node->left->children[node->left->children_count - 1];
	if (last->token->type != ID || !last->token->name) return false;
	char *qname = format("%s.%s", last->token->name, node->token->name);
	Node *func  = find_function(qname);
	if (!func) {
		free(qname);
		return false;
	}
	// Module functions are accessed via `::`, not `.`. Same rule as
	// struct `pub fn`. Reject the dot form here so the user gets a
	// helpful message instead of a silent dispatch.
	parse_error(node->token,
	            "'%s' is a module function — call it with '::' instead of '.'", qname);
	free(qname);
	return true;
}

void gen_struct_declaration(Token *token) {
	if (!token->is_dec)   return;
	if (token->is_global) {
		if (token->used == 0) {
			token->is_dec = false;
			return;
		}
		TypeRef type     = get_llvm_type(token);
		token->llvm.elem = LLVMAddGlobal(module, type, token->name);
		LLVMSetInitializer(token->llvm.elem, LLVMConstNull(type));
	} else if (token->is_ref) {
	} else {
		_alloca(token);
		TypeRef type = get_llvm_type(token);
		LLVMBuildStore(builder, LLVMConstNull(type), token->llvm.elem);
	}
	token->is_dec = false;
}

void gen_primitive_declaration(Token *token) {
	if (token->is_dec) {
		if (token->is_global) {
			if (token->used == 0) {
				token->is_dec = false;
				return;
			}
			TypeRef type     = get_llvm_type(token);
			token->llvm.elem = LLVMAddGlobal(module, type, token->name);
			LLVMSetInitializer(token->llvm.elem, _get_default_value(token));
		} else {
			_alloca(token);
			LLVMBuildStore(builder, _get_default_value(token), token->llvm.elem);
		}
		token->is_dec = false;
		return;
	}
	if (!token->name) _const_value(token);
}

void propagate_dims(Token *dst, Token *src, Node *lhs_node) {
	dst->llvm.dims_count = 0;
	for (int i = 0; i < src->llvm.dims_count; i++) {
		resize_array(dst->llvm.dims, Value, dst->llvm.dims_size, dst->llvm.dims_count);
		dst->llvm.dims[dst->llvm.dims_count++] = src->llvm.dims[i];
	}
	if (lhs_node->token->type != DOT || !lhs_node->right) return;
	Node *sd = lhs_node->left->token->Struct.ptr;
	if (!sd) return;
	int idx = lhs_node->right->token->Struct.index;
	if (idx < 0 || idx >= sd->children_count) return;
	Token *field           = sd->children[idx]->token;
	field->llvm.dims_count = 0;
	for (int i = 0; i < src->llvm.dims_count; i++) {
		resize_array(field->llvm.dims, Value, field->llvm.dims_size, field->llvm.dims_count);
		field->llvm.dims[field->llvm.dims_count++] = src->llvm.dims[i];
	}
}

Value emit_copy_construct(Token *param, Token *arg) {
	if (!param || param->type != STRUCT_CALL) return NULL;
	bool arg_is_struct = arg->type == STRUCT_CALL || arg->ret_type == STRUCT_CALL;
	if (!arg_is_struct) return NULL;
	Node *st_node = param->Struct.ptr;
	if (!st_node) return NULL;
	char *cp_name = format("%s" OP_PREFIX "ASSIGN.%s", st_node->token->name, st_node->token->name);
	Value copy_op = LLVMGetNamedFunction(module, cp_name);
	free(cp_name);
	if (!copy_op) return NULL;
	TypeRef st_type = get_llvm_type(param);
	Value   tmp     = LLVMBuildAlloca(builder, st_type, "copy");
	LLVMBuildStore(builder, LLVMConstNull(st_type), tmp);
	Value src_ptr;
	if (LLVMGetTypeKind(LLVMTypeOf(arg->llvm.elem)) == LLVMPointerTypeKind) {
		src_ptr = arg->llvm.elem;
	} else {
		Value spill = LLVMBuildAlloca(builder, st_type, "spill");
		LLVMBuildStore(builder, arg->llvm.elem, spill);
		src_ptr = spill;
	}
	Value cargs[] = {src_ptr, tmp};
	LLVMBuildCall2(builder, LLVMGlobalGetValueType(copy_op), copy_op, cargs, 2, "");
	return LLVMBuildLoad2(builder, st_type, tmp, "copy_val");
}

Value marshal_arg_for_op(Token *param, Node *arg) {
	bool param_is_ref = param ? param->is_ref : false;
	bool arg_is_ref   = arg->token->is_ref;
	if (param_is_ref && arg_is_ref) {
		TypeRef type = get_llvm_type(arg->token);
		return LLVMBuildLoad2(builder, LLVMPointerType(type, 0), arg->token->llvm.elem, "ref_arg");
	}
	if (param_is_ref && !arg_is_ref) {
		if (LLVMGetTypeKind(LLVMTypeOf(arg->token->llvm.elem)) == LLVMPointerTypeKind)
			return arg->token->llvm.elem;
		TypeRef type = get_llvm_type(arg->token);
		Value   tmp  = LLVMBuildAlloca(builder, type, "tmp_op");
		LLVMBuildStore(builder, arg->token->llvm.elem, tmp);
		return tmp;
	}
	ensure_loaded(arg);
	return arg->token->llvm.elem;
}

Value gen_operator_call(Node *node, Node *left, Node *right, bool try_copy_ctor) {
	Node *func    = node->token->Fcall.ptr;
	LLVM  srcFunc = ensure_func_resolved(func);
	if (CHECK(!srcFunc.func_type, "operator: func_type NULL for '%s'", node->token->name))
		return NULL;
	if (CHECK(!srcFunc.elem, "operator: elem NULL for '%s'", node->token->name)) return NULL;
	asm_gen(left);
	asm_gen(right);
	Token *param        = func->left->children_count >= 2 ? func->left->children[0]->token : NULL;
	Value  rhs_val      = NULL;
	bool   param_is_ref = param ? param->is_ref : false;
	if (try_copy_ctor && !param_is_ref && !right->token->is_ref)
		rhs_val = emit_copy_construct(param, right->token);
	if (!rhs_val) rhs_val = marshal_arg_for_op(param, right);
	Value args[2] = {rhs_val, left->token->llvm.elem};
	return LLVMBuildCall2(builder, srcFunc.func_type, srcFunc.elem, args, 2, "");
}

Value asm_assign_cast(Node *left, Node *right, Value val) {
	if (includes(right->token->type, STACK, HEAP, 0) && left->token->type == DOT) {
		TypeRef field_type = get_llvm_type(left->token);
		val                = LLVMBuildBitCast(builder, val, field_type, "field_cast");
	}
	if (val && LLVMGetTypeKind(LLVMTypeOf(val)) == PointerType) {
		TypeRef dest_type = get_llvm_type(left->token);
		if (dest_type && LLVMGetTypeKind(dest_type) == PointerType && LLVMTypeOf(val) != dest_type)
			val = LLVMBuildBitCast(builder, val, dest_type, "ptr_cast");
	}
	return val;
}

Value asm_as_int_to_int(Value src, TypeRef stype, TypeRef ttype) {
	unsigned sb = LLVMGetIntTypeWidth(stype);
	unsigned tb = LLVMGetIntTypeWidth(ttype);
	if (sb > tb) return LLVMBuildTrunc(builder, src, ttype, "as");
	if (sb < tb) return LLVMBuildSExt(builder, src, ttype, "as");
	return src;
}

Value asm_collect_dims(Node *node) {
	Value total = LLVMConstInt(i32, 1, 0);
	int   depth = node->token->Array.depth;
	for (int i = 0; i < depth; i++) {
		asm_gen(node->children[i]);
		ensure_loaded(node->children[i]);
		Value dv = node->children[i]->token->llvm.elem;
		resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
		             node->token->llvm.dims_count);
		node->token->llvm.dims[node->token->llvm.dims_count++] = dv;
		total = LLVMBuildMul(builder, total, dv, "dim");
	}
	return total;
}

Value asm_total_bytes(Value total, TypeRef elem_t) {
	TargetData td        = LLVMGetModuleDataLayout(module);
	size_t     elem_size = LLVMABISizeOfType(td, elem_t);
	return LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");
}

TypeRef asm_array_lit_elem_type(Node *node) {
	TypeRef elem_t;
	Type    et = node->token->Array.sub_type;
	if (et == STRUCT_CALL && node->token->Array.struct_ptr)
		elem_t = get_llvm_type(node->token->Array.struct_ptr->token);
	else {
		Token tmp = {.type = et};
		elem_t    = get_llvm_type(&tmp);
	}
	for (int d = 1; d < node->token->Array.depth; d++)
		elem_t = LLVMPointerType(elem_t, 0);
	return elem_t;
}

void asm_dot_propagate_field_dims(Token *struct_tok, int field_index, Token *target) {
	if (!struct_tok->Struct.ptr) return;
	Node *sd = struct_tok->Struct.ptr;
	if (field_index < 0 || field_index >= sd->children_count) return;
	Token *field_def = sd->children[field_index]->token;
	if (field_def->llvm.dims_count == 0) return;
	target->llvm.dims_count = 0;
	for (int d = 0; d < field_def->llvm.dims_count; d++) {
		resize_array(target->llvm.dims, Value, target->llvm.dims_size, target->llvm.dims_count);
		target->llvm.dims[target->llvm.dims_count++] = field_def->llvm.dims[d];
	}
}

void asm_access_struct_field(Node *node) {
	Token *struct_tok      = node->left->token;
	int    field_idx       = node->right->token->Struct.index;
	node->token->llvm.elem = struct_field_ptr(struct_tok, field_idx, node->right->token->name);
}

bool asm_access_multidim(Node *node, Value left_value, Value right_ref) {
	int left_depth = node->left->token->llvm.dims_count;
	if (left_depth <= 1) return false;
	Value stride = LLVMConstInt(i32, 1, 0);
	for (int d = 1; d < left_depth; d++)
		stride = LLVMBuildMul(builder, stride, node->left->token->llvm.dims[d], "stride");
	Value   flat_idx = LLVMBuildMul(builder, right_ref, stride, "flat_idx");
	TypeRef base_t;
	if (node->left->token->Array.sub_type == STRUCT_CALL && node->left->token->Array.struct_ptr)
		base_t = get_llvm_type(node->left->token->Array.struct_ptr->token);
	else {
		Token tmp = {.type = node->left->token->Array.sub_type};
		base_t    = get_llvm_type(&tmp);
	}
	Value indices[]               = {flat_idx};
	node->token->llvm.elem        = LLVMBuildGEP2(builder, base_t, left_value, indices, 1, "row");
	node->token->llvm.is_loaded   = true;
	node->token->ret_type         = ARRAY;
	node->token->Array.sub_type   = node->left->token->Array.sub_type;
	node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
	node->token->Array.depth      = left_depth - 1;
	node->token->llvm.dims_count  = 0;
	for (int d = 1; d < left_depth; d++) {
		resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
		             node->token->llvm.dims_count);
		node->token->llvm.dims[node->token->llvm.dims_count++] = node->left->token->llvm.dims[d];
	}
	return true;
}

TypeRef asm_access_element_type(Node *node) {
	Type left_elem =
	    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (left_elem == CHARS) {
		node->token->ret_type = CHAR;
		return i8;
	}
	if (left_elem == ARRAY_TYPE || left_elem == ARRAY || node->left->token->type == HEAP ||
	    node->left->token->type == ARRAY) {
		Type    et = node->left->token->Array.sub_type;
		TypeRef element_type;
		if (et == STRUCT_CALL && node->left->token->Array.struct_ptr) {
			element_type            = get_llvm_type(node->left->token->Array.struct_ptr->token);
			node->token->Struct.ptr = node->left->token->Array.struct_ptr;
		} else {
			Token tmp    = {.type = et};
			element_type = get_llvm_type(&tmp);
		}
		node->token->ret_type = et;
		return element_type;
	}
	node->token->ret_type = node->left->token->type;
	return get_llvm_type(node->left->token);
}

Value asm_access_left_value(Node *node) {
	Value lv;
	if (node->left->token->is_ref) {
		TODO(1, "stop");
		lv = read_value(node->left->token);
	} else {
		ensure_loaded(node->left);
		lv            = node->left->token->llvm.elem;
		TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(lv));
		if (kind == PointerType && node->left->token->name && !node->left->token->llvm.is_loaded &&
		    node->left->token->type != STACK && node->left->token->type != DOT)
			lv = LLVMBuildLoad2(builder, p8, lv, "ptr_load");
	}
	return lv;
}

Value marshal_fcall_arg(Token *param, Node *arg, bool is_proto_call) {
	bool param_is_ref = param ? param->is_ref : false;
	bool arg_is_ref   = arg->token->is_ref;
	if (param_is_ref && arg_is_ref) {
		TypeRef type = get_llvm_type(arg->token);
		return LLVMBuildLoad2(builder, LLVMPointerType(type, 0), arg->token->llvm.elem, "ref_arg");
	}
	if (param_is_ref && !arg_is_ref) return arg->token->llvm.elem;
	if (!param_is_ref && arg_is_ref) return read_value(arg->token);
	Value val = emit_copy_construct(param, arg->token);
	if (!val) {
		ensure_loaded(arg);
		val = arg->token->llvm.elem;
	}
	bool param_is_struct = param && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0);
	if (is_proto_call && param_is_struct) {
		TypeRef st_type = get_llvm_type(param);
		Value   st_ptr  = LLVMBuildAlloca(builder, st_type, "st_slot");
		LLVMBuildStore(builder, val, st_ptr);
		Value i64p = LLVMBuildAlloca(builder, i64, "i64_slot");
		LLVMBuildMemCpy(builder, i64p, 0, st_ptr, 0, LLVMConstInt(i64, 4, 0));
		val = LLVMBuildLoad2(builder, i64, i64p, "i64_arg");
	}
	return val;
}

void schedule_temp_cleanup(Token *token) {
	if (token->ret_type != STRUCT_CALL || !token->Struct.ptr) return;
	TypeRef st_type = get_llvm_type(token);
	Value   tmp     = LLVMBuildAlloca(builder, st_type, "tmp_struct");
	LLVMBuildStore(builder, token->llvm.elem, tmp);
	add_auto_clean(scope, tmp, token->Struct.ptr);
}

void call_delete(char *type_name, Value self_ptr) {
	char *qname = format("%s.delete", type_name);
	Value fn    = LLVMGetNamedFunction(module, qname);
	free(qname);
	if (!fn) return;
	Value args[] = {self_ptr};
	LLVMBuildCall2(builder, LLVMGlobalGetValueType(fn), fn, args, 1, "");
}

void emit_scope_clean(Node *scope_node, int from, Token *skip) {
	for (int i = from; i < scope_node->variables_count; i++) {
		Token *var = scope_node->variables[i];
		if (var == skip)              continue;
		if (!var->llvm.elem)          continue;
		if (var->is_ref)              continue;
		if (var->type == STRUCT_CALL) call_delete(var->Struct.ptr->token->name, var->llvm.elem);
	}
	for (int i = 0; i < scope_node->auto_cleans_count; i++)
		call_delete(scope_node->auto_cleans[i].type->token->name, scope_node->auto_cleans[i].value);
	scope_node->auto_cleans_count = 0;
}

void asm_fcall_static(Node *node) {
	Node  *func  = node->token->Fcall.ptr;
	LLVM   srcFn = ensure_func_resolved(func);
	int    argc  = node->left->children_count;
	Value *args  = allocate(argc + 1, sizeof(Value));
	for (int i = 0; i < argc; i++) {
		asm_gen(node->left->children[i]);
		ensure_loaded(node->left->children[i]);
		args[i] = node->left->children[i]->token->llvm.elem;
	}
	node->token->llvm.elem = LLVMBuildCall2(builder, srcFn.func_type, srcFn.elem, args, argc, "");
	if (node->token->ret_type != VOID) node->token->llvm.is_loaded = true;
	free(args);
	schedule_temp_cleanup(node->token);
}

void asm_fcall_marshal_args(Node *node, Value *args, int *count_out, bool is_proto) {
	bool   is_variadic = node->token->Fcall.ptr->token->is_variadic;
	int    count       = node->left->children_count;
	Node **argNodes    = node->left->children;
	Node  *dec_args    = node->token->Fcall.ptr->left;
	int    fixed       = is_variadic ? dec_args->children_count : count;
	for (int i = 0; i < fixed; i++) {
		asm_gen(argNodes[i]);
		Token *param = (i < dec_args->children_count) ? dec_args->children[i]->token : NULL;
		args[i]      = marshal_fcall_arg(param, argNodes[i], is_proto);
	}
	if (is_variadic) {
		// Ura-defined variadics: prepend the i32 arg-count so the callee can
		// walk its varargs. Libc protos (printf/scanf/etc.) use real C ABI —
		// no count, varargs go straight after the fixed args.
		bool add_count     = !is_proto;
		int  variadic_count = count - fixed;
		int  out_idx       = fixed;
		if (add_count) {
			args[out_idx++] = LLVMConstInt(i32, variadic_count, 0);
		}
		for (int i = fixed; i < count; i++) {
			asm_gen(argNodes[i]);
			ensure_loaded(argNodes[i]);
			args[out_idx++] = argNodes[i]->token->llvm.elem;
		}
		if (add_count) count++;
	}
	*count_out = count;
}

void asm_fcall_unpack_proto_struct(Node *node) {
	Value   i64_ret = node->token->llvm.elem;
	TypeRef st_type = get_llvm_type(node->token);
	Value   i64p    = LLVMBuildAlloca(builder, i64, "ret_i64");
	LLVMBuildStore(builder, i64_ret, i64p);
	Value st_ptr = LLVMBuildAlloca(builder, st_type, "ret_struct");
	LLVMBuildMemCpy(builder, st_ptr, 0, i64p, 0, LLVMConstInt(i64, 4, 0));
	node->token->llvm.elem = LLVMBuildLoad2(builder, st_type, st_ptr, "ret_struct_val");
}

// Indirect call: callee is a fn-typed local variable (Statement.ptr).
// Build the LLVM function type from the variable's FN_TYPE signature,
// load the stored function pointer, and dispatch.
void asm_fcall_indirect(Node *node) {
	Token *fn_var = node->token->Statement.ptr;
	if (!fn_var || !fn_var->llvm.elem) return;

	// Build LLVM function type from the signature.
	int      n      = fn_var->Fn.params_count;
	TypeRef  ret_t  = fn_var->Fn.ret ? get_llvm_type(fn_var->Fn.ret) : vd;
	TypeRef *ptypes = NULL;
	if (n > 0) {
		ptypes = allocate(n, sizeof(TypeRef));
		for (int i = 0; i < n; i++) ptypes[i] = get_llvm_type(fn_var->Fn.params[i]);
	}
	TypeRef ftype = LLVMFunctionType(ret_t, ptypes, n, 0);

	// Load fn pointer from the variable's storage.
	Value fn_ptr = LLVMBuildLoad2(builder, LLVMPointerType(ftype, 0), fn_var->llvm.elem, "fn_ptr");

	// Marshal args.
	int    arg_count = node->left ? node->left->children_count : 0;
	Value *args      = NULL;
	if (arg_count > 0) {
		args = allocate(arg_count, sizeof(Value));
		for (int i = 0; i < arg_count; i++) {
			Node *carg = node->left->children[i];
			asm_gen(carg);
			ensure_loaded(carg);
			args[i] = carg->token->llvm.elem;
		}
	}

	const char *name      = (ret_t != vd) ? "indirect_call" : "";
	node->token->llvm.elem = LLVMBuildCall2(builder, ftype, fn_ptr, args, arg_count, name);
	free(args);
	free(ptypes);
}

void asm_fcall_instance(Node *node) {
	LLVM   srcFunc  = ensure_func_resolved(node->token->Fcall.ptr);
	bool   is_proto = node->token->Fcall.ptr->token->is_proto;
	int    count    = node->left->children_count;
	Value *args     = NULL;
	if (count) {
		args = allocate(count + 3, sizeof(Value));
		asm_fcall_marshal_args(node, args, &count, is_proto);
	}
	if (CHECK(!srcFunc.func_type, "FCALL: func_type NULL '%s'", node->token->name)) {
		free(args);
		return;
	}
	if (CHECK(!srcFunc.elem, "FCALL: elem NULL '%s'", node->token->name)) {
		free(args);
		return;
	}
	char *name = node->token->Fcall.ptr->token->ret_type != VOID ? node->token->name : "";
	node->token->llvm.elem =
	    LLVMBuildCall2(builder, srcFunc.func_type, srcFunc.elem, args, count, name);
	free(args);
	if (is_proto && node->token->Fcall.ptr->token->ret_type == STRUCT_CALL)
		asm_fcall_unpack_proto_struct(node);
	schedule_temp_cleanup(node->token);
}

void append_string_literal_to_fmt(const char *s, char *fmt, int *fc) {
	for (int i = 0; s[i]; i++) {
		if (s[i] == '%') {
			fmt[(*fc)++] = '%';
			fmt[(*fc)++] = '%';
		} else if (s[i] == '\\' && s[i + 1]) {
			switch (s[++i]) {
			case 'n':  fmt[(*fc)++] = '\n'; break;
			case 't':  fmt[(*fc)++] = '\t'; break;
			case 'r':  fmt[(*fc)++] = '\r'; break;
			case '\\': fmt[(*fc)++] = '\\'; break;
			case '"':  fmt[(*fc)++] = '"'; break;
			default:
				fmt[(*fc)++] = '\\';
				fmt[(*fc)++] = s[i];
				break;
			}
		} else fmt[(*fc)++] = s[i];
	}
}

void append_struct_with_output_op(Token *tok, char *fmt, int *fc, Value *args, int *nargs, Node *sd,
                                  Value out_fn) {
	// If `tok` came from an FCALL/op result, llvm.elem holds a struct VALUE
	// rather than a pointer. The output overload expects `ref self`, so we
	// need an addressable temp whenever the operand is a literal struct value.
	Value   self_ptr = tok->llvm.elem;
	if (self_ptr && LLVMGetTypeKind(LLVMTypeOf(self_ptr)) == StructType) {
		TypeRef val_type = LLVMTypeOf(self_ptr);
		Value   tmp      = LLVMBuildAlloca(builder, val_type, "out_arg_tmp");
		LLVMBuildStore(builder, self_ptr, tmp);
		self_ptr = tmp;
	}
	TypeRef fn_type  = LLVMGlobalGetValueType(out_fn);
	Value   result   = LLVMBuildCall2(builder, fn_type, out_fn, &self_ptr, 1, "output_op");
	TypeRef ret_type = LLVMGetReturnType(fn_type);
	if (LLVMGetTypeKind(ret_type) == PointerType) {
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 's';
		args[(*nargs)++] = result;
		return;
	}
	if (LLVMGetTypeKind(ret_type) != StructType) return;
	Value tmp = LLVMBuildAlloca(builder, ret_type, "out_tmp");
	LLVMBuildStore(builder, result, tmp);
	Node *ret_sd = NULL;
	for (int si = scopes_count; si > 0 && !ret_sd; si--)
		for (int sj = 0; sj < scopes[si]->structs_count; sj++)
			if (scopes[si]->structs[sj]->token->llvm.struct_type == ret_type) {
				ret_sd = scopes[si]->structs[sj];
				break;
			}
	if (!ret_sd) return;
	add_auto_clean(scope, tmp, ret_sd);
	Token ftok      = *tok;
	ftok.Struct.ptr = ret_sd;
	ftok.llvm.elem  = tmp;
	ftok.type       = STRUCT_CALL;
	(void)sd;
	append_output_arg(&ftok, fmt, fc, args, nargs);
}

void append_struct_default_fmt(Token *tok, char *fmt, int *fc, Value *args, int *nargs, Node *sd) {
	fmt[(*fc)++] = '{';
	fmt[(*fc)++] = ' ';
	for (int i = 0; i < sd->children_count; i++) {
		Token *field          = sd->children[i]->token;
		int    field_name_len = strlen(field->name);
		memcpy(fmt + *fc, field->name, field_name_len);
		*fc += field_name_len;
		fmt[(*fc)++]   = ':';
		fmt[(*fc)++]   = ' ';
		Token ftok     = *field;
		ftok.llvm.elem = struct_field_ptr(tok, i, field->name);
		append_output_arg(&ftok, fmt, fc, args, nargs);
		if (i < sd->children_count - 1) {
			fmt[(*fc)++] = ',';
			fmt[(*fc)++] = ' ';
		}
	}
	fmt[(*fc)++] = ' ';
	fmt[(*fc)++] = '}';
}

Type append_resolve_type(Token *tok) {
	Type type = tok->type ? tok->type : tok->ret_type;
	switch (type) {
	case INT: case LONG: case SHORT: case CHAR: case CHARS: case BOOL:
	case FLOAT:
	case STRUCT_CALL: return type;
	case FCALL:       return tok->ret_type;
	default:          return tok->ret_type ? tok->ret_type : type;
	}
}

void append_output_arg(Token *tok, char *fmt, int *fc, Value *args, int *nargs) {
	if (tok->type == CHARS && !tok->name) {
		append_string_literal_to_fmt(tok->Chars.value, fmt, fc);
		return;
	}
	Type type = append_resolve_type(tok);
	switch (type) {
	case INT: case SHORT:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 'd';
		args[(*nargs)++] = read_value(tok);
		return;
	case BOOL: {
		Value bv         = read_value(tok);
		Value ts         = LLVMBuildGlobalStringPtr(builder, "True", "true_str");
		Value fs         = LLVMBuildGlobalStringPtr(builder, "False", "false_str");
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 's';
		args[(*nargs)++] = LLVMBuildSelect(builder, bv, ts, fs, "bool_str");
		return;
	}
	case LONG:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 'l';
		fmt[(*fc)++]     = 'l';
		fmt[(*fc)++]     = 'd';
		args[(*nargs)++] = read_value(tok);
		return;
	case CHAR:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 'c';
		args[(*nargs)++] = read_value(tok);
		return;
	case CHARS:
		fmt[(*fc)++]     = '%';
		fmt[(*fc)++]     = 's';
		args[(*nargs)++] = read_value(tok);
		return;
	case FLOAT:
		fmt[(*fc)++] = '%';
		fmt[(*fc)++] = 'f';
		args[(*nargs)++] =
		    LLVMBuildFPExt(builder, read_value(tok), LLVMDoubleTypeInContext(context), "f2d");
		return;
	case STRUCT_CALL: {
		Node *sd        = tok->Struct.ptr;
		char *out_name  = format("%s" OP_PREFIX "output", sd->token->name);
		Value out_fn    = LLVMGetNamedFunction(module, out_name);
		free(out_name);
		// Fallback: prep files emit the output overload as a flat `<Struct>_output`.
		// The flat fn may not be pre-declared in LLVM (used=0 at IR time since
		// the formatter is the only caller) — resolve it on demand from scope.
		if (!out_fn) {
			char *flat_name = format("%s_output", sd->token->name);
			out_fn          = LLVMGetNamedFunction(module, flat_name);
			if (!out_fn) {
				Node *fn = find_function(flat_name);
				if (fn) {
					resolve_func_type(fn);
					out_fn = LLVMGetNamedFunction(module, flat_name);
				}
			}
			free(flat_name);
		}
		if (out_fn) append_struct_with_output_op(tok, fmt, fc, args, nargs, sd, out_fn);
		else append_struct_default_fmt(tok, fmt, fc, args, nargs, sd);
		return;
	}
	default: fmt[(*fc)++] = '?'; return;
	}
}

int output_format_capacity(int argc, Node **argv) {
	int cap = 64;
	for (int i = 0; i < argc; i++) {
		if (argv[i]->token->type == CHARS && !argv[i]->token->name)
			cap += strlen(argv[i]->token->Chars.value) * 2 + 4;
		else cap += 128;
	}
	return cap;
}

void if_chain_branch(Node *curr, Block if_start, Block end) {
	Block start = curr->token->type == IF ? if_start : curr->token->llvm.bloc;
	Block then  = _append_block(curr->token->type == IF ? "if.then" : "elif.then");
	Block next;
	if (curr->right)
		next = _append_block(curr->right->token->type == ELSE ? "if.else" : "elif.start");
	else next = end;
	curr->token->llvm.then = then;
	LLVMPositionBuilderAtEnd(builder, start);
	asm_gen(curr->left);
	ensure_loaded(curr->left);
	LLVMBuildCondBr(builder, curr->left->token->llvm.elem, then, next);
	LLVMPositionBuilderAtEnd(builder, then);
	for (int i = 0; i < curr->children_count; i++) {
		asm_gen(curr->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
	_branch(end);
	if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
		curr->right->token->llvm.bloc = next;
}

void if_chain_else(Node *curr, Block end) {
	LLVMPositionBuilderAtEnd(builder, curr->token->llvm.bloc);
	for (int i = 0; i < curr->children_count; i++) {
		asm_gen(curr->children[i]);
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
	}
	_branch(end);
}

void gen_if_chain(Node *node, Block if_start, Block end) {
	Node *curr = node;
	while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
		if (includes(curr->token->type, IF, ELIF, 0)) if_chain_branch(curr, if_start, end);
		else if (curr->token->type == ELSE)           if_chain_else(curr, end);
		curr = curr->right;
	}
}

void asm_return_delete_chain(Node *fdec) {
	if (fdec->left->children_count < 1) return;
	Token *self_tok = fdec->left->children[0]->token;
	if (self_tok->type != STRUCT_CALL || !self_tok->is_ref) return;
	Node   *sd       = self_tok->Struct.ptr;
	TypeRef st_type  = sd->token->llvm.struct_type;
	TypeRef ptr_type = LLVMPointerType(st_type, 0);
	Value   self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
	for (int i = 0; i < sd->children_count; i++) {
		Token *field = sd->children[i]->token;
		if (field->type != STRUCT_CALL) continue;
		Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
		Value field_ptr = LLVMBuildGEP2(builder, st_type, self_ptr, indices, 2, field->name);
		call_delete(field->Struct.ptr->token->name, field_ptr);
	}
}

void asm_return_main_globals(void) {
	for (int i = 0; i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
		if (child->token->used == 0)                                       continue;
		call_delete(child->token->Struct.ptr->token->name, child->token->llvm.elem);
	}
}

void asm_return_tuple(Node *node, Node *fdec) {
	TypeRef tuple_type = fdec->token->llvm.struct_type;
	Value   agg        = LLVMGetUndef(tuple_type);
	for (int i = 0; i < node->children_count; i++) {
		asm_gen(node->children[i]);
		ensure_loaded(node->children[i]);
		agg = LLVMBuildInsertValue(builder, agg, node->children[i]->token->llvm.elem, i, "");
	}
	LLVMBuildRet(builder, agg);
}

void asm_return_value(Node *node, Node *fdec) {
	if (!node->left) return;
	Token *ret_tok = node->left->token;
	if (ret_tok->type == VOID) {
		LLVMBuildRetVoid(builder);
		return;
	}
	asm_gen(node->left);
	if (fdec->token->ret_type == STRUCT_CALL && !fdec->token->is_ref) {
		LLVMBuildRet(builder, read_value(ret_tok));
		return;
	}
	if (fdec->token->ret_type == STRUCT_CALL && fdec->token->is_ref) {
		TypeRef st_type  = get_llvm_type(ret_tok);
		TypeRef ptr_type = LLVMPointerType(st_type, 0);
		Value   ptr      = LLVMBuildLoad2(builder, ptr_type, ret_tok->llvm.elem, "ret_ptr");
		LLVMBuildRet(builder, ptr);
		return;
	}
	ensure_loaded(node->left);
	LLVMBuildRet(builder, node->left->token->llvm.elem);
}

void gen_struct_emit_nested(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Token *ft = node->children[i]->token;
		if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0) {
			ft->Struct.ptr->token->used++;
			asm_gen(ft->Struct.ptr);
		}
		if (includes(ft->type, ARRAY_TYPE, ARRAY, 0) && ft->Array.sub_type == STRUCT_CALL &&
		    ft->Array.struct_ptr && ft->Array.struct_ptr->token->used == 0) {
			ft->Array.struct_ptr->token->used++;
			asm_gen(ft->Array.struct_ptr);
		}
	}
}

void gen_struct_build_type(Node *node) {
	int      pos   = node->children_count;
	TypeRef *types = allocate(pos + 1, sizeof(TypeRef));
	for (int i = 0; i < pos; i++)
		types[i] = get_llvm_type(node->children[i]->token);
	char *struct_name             = format("struct.%s", node->token->name);
	node->token->llvm.struct_type = _named_struct_type(struct_name, types, pos, 0);
	free(struct_name);
	free(types);
}

void gen_struct_emit_delete(Node *node) {
	if (node->token->has_clean) return;
	TypeRef st_type     = node->token->llvm.struct_type;
	TypeRef ptr_type    = LLVMPointerType(st_type, 0);
	TypeRef lc_params[] = {ptr_type};
	TypeRef lc_fn_type  = LLVMFunctionType(vd, lc_params, 1, 0);
	char   *fname       = format("%s.delete", node->token->name);
	Value   fn          = _add_function(fname, lc_fn_type);
	free(fname);
	Block entry = LLVMAppendBasicBlockInContext(context, fn, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
	LLVMSetCurrentDebugLocation2(builder, NULL);
	Value self = LLVMGetParam(fn, 0);
	for (int i = 0; i < node->children_count; i++) {
		Token *field = node->children[i]->token;
		if (field->type != STRUCT_CALL) continue;
		Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
		Value field_ptr = LLVMBuildGEP2(builder, st_type, self, indices, 2, field->name);
		call_delete(field->Struct.ptr->token->name, field_ptr);
	}
	LLVMBuildRetVoid(builder);
	node->token->has_clean = true;
}

void gen_struct_predeclare_methods(Node *node) {
	for (int i = 0; i < node->functions_count; i++)
		resolve_func_type(node->functions[i]);
}

// ============================================================================
// PREPROCESSED SOURCE OUTPUT (-prep flag)
//
// The -prep dump is meant to be a flat, C-style, re-parseable Ura source.
// Conventions:
//   * Struct methods are HOISTED to free functions named `<Struct>_<method>`.
//   * Operator overloads become `<Struct>_<op>_<RHS>` with a uniform RHS suffix.
//   * Method calls are flattened: `s.assign(x)` -> `String_assign(ref s, x)`.
//   * Operator infix is flattened: `a + b` -> `String_add_String(ref a, ref b)`.
//   * `output(s)` for any struct with operator output is wrapped:
//       `output(String_output(ref s))`.
//   * Ref params/locals are written `ref name type`.
// ============================================================================

static void emit_indent(File f, int depth) {
	for (int i = 0; i < depth * TAB; i++)
		fputc(' ', f);
}

static const char *op_symbol(Type type) {
	switch (type) {
	case ADD:         return "+";
	case SUB:         return "-";
	case MUL:         return "*";
	case DIV:         return "/";
	case MOD:         return "%";
	case EQUAL:       return "==";
	case NOT_EQUAL:   return "!=";
	case LESS:        return "<";
	case GREAT:       return ">";
	case LESS_EQUAL:  return "<=";
	case GREAT_EQUAL: return ">=";
	case AND:         return "and";
	case OR:          return "or";
	case NOT:         return "not";
	case BAND:        return "&";
	case BOR:         return "|";
	case BXOR:        return "^";
	case BNOT:        return "~";
	case LSHIFT:      return "<<";
	case RSHIFT:      return ">>";
	case ASSIGN:      return "=";
	case ADD_ASSIGN:  return "+=";
	case SUB_ASSIGN:  return "-=";
	case MUL_ASSIGN:  return "*=";
	case DIV_ASSIGN:  return "/=";
	case MOD_ASSIGN:  return "%=";
	default:          return "?";
	}
}

// Lowercase op suffix used in flat operator function names: ADD -> "add", LT -> "lt".
// Look up an operator overload registered on a struct by op-type and RHS-type-name.
// Returns the matching function node, or NULL if none.
static Node *prep_find_struct_op(Node *struct_def, const char *op_kw, const char *rhs_id) {
	if (!struct_def) return NULL;
	for (int i = 0; i < struct_def->functions_count; i++) {
		Node *fn = struct_def->functions[i];
		if (!fn->token->name) continue;
		if (op_kw && rhs_id) {
			char *want = format("%s" OP_PREFIX "%s.%s",
			    struct_def->token->name, op_kw, rhs_id);
			if (strcmp(fn->token->name, want) == 0) { free(want); return fn; }
			free(want);
		}
	}
	return NULL;
}

// Decode an IR-mangled fdec name into the prep flat name.
// Inputs like:
//   "String.assign"                      -> "String_assign"
//   "String.delete"                      -> "String_delete"
//   "String.operator.output"             -> "String_output"
//   "String.operator.ASSIGN.String"      -> "String_assign_String"
//   "String.operator.ADD.CHARS"          -> "String_add_chars"
// Caller frees the returned string.
static char *prep_fn_flat_name(const char *mangled) {
	if (!mangled) return strdup("?");
	// Find ".operator." substring.
	const char *opmark = strstr(mangled, OP_PREFIX);
	if (!opmark) {
		// Plain method: "<Struct>.<method>" -> "<Struct>_<method>"
		char *copy = strdup(mangled);
		for (char *p = copy; *p; p++) if (*p == '.') *p = '_';
		return copy;
	}
	// Split: <struct> ".operator." <op_kw> [ "." <rhs> ]
	int   struct_len = (int)(opmark - mangled);
	char *struct_part = strndup(mangled, struct_len);
	const char *after = opmark + strlen(OP_PREFIX);
	const char *dot   = strchr(after, '.');
	char *op_kw       = dot ? strndup(after, dot - after) : strdup(after);
	const char *rhs   = dot ? dot + 1 : NULL;

	// Lowercase op_kw to suffix form.
	// op_kw arrives as the to_string() name (e.g. "ASSIGN", "ADD", "LT", "EQ"...).
	const char *suffix = NULL;
	if      (strcmp(op_kw, "ASSIGN")  == 0) suffix = "assign";
	else if (strcmp(op_kw, "ADD")     == 0) suffix = "add";
	else if (strcmp(op_kw, "SUB")     == 0) suffix = "sub";
	else if (strcmp(op_kw, "MUL")     == 0) suffix = "mul";
	else if (strcmp(op_kw, "DIV")     == 0) suffix = "div";
	else if (strcmp(op_kw, "MOD")     == 0) suffix = "mod";
	else if (strcmp(op_kw, "EQ")      == 0) suffix = "eq";
	else if (strcmp(op_kw, "NEQ")     == 0) suffix = "ne";
	else if (strcmp(op_kw, "LT")      == 0) suffix = "lt";
	else if (strcmp(op_kw, "GT")      == 0) suffix = "gt";
	else if (strcmp(op_kw, "LE")      == 0) suffix = "le";
	else if (strcmp(op_kw, "GE")      == 0) suffix = "ge";
	else if (strcmp(op_kw, "BAND")    == 0) suffix = "band";
	else if (strcmp(op_kw, "BOR")     == 0) suffix = "bor";
	else if (strcmp(op_kw, "BXOR")    == 0) suffix = "bxor";
	else if (strcmp(op_kw, "LSHIFT")  == 0) suffix = "lshift";
	else if (strcmp(op_kw, "RSHIFT")  == 0) suffix = "rshift";
	else if (strcmp(op_kw, "ADD_ASS") == 0) suffix = "add_assign";
	else if (strcmp(op_kw, "SUB_ASS") == 0) suffix = "sub_assign";
	else if (strcmp(op_kw, "MUL_ASS") == 0) suffix = "mul_assign";
	else if (strcmp(op_kw, "DIV_ASS") == 0) suffix = "div_assign";
	else if (strcmp(op_kw, "MOD_ASS") == 0) suffix = "mod_assign";
	else if (strcmp(op_kw, "output")  == 0) suffix = "output";
	else if (strcmp(op_kw, "delete")  == 0) suffix = "delete";
	else                                    suffix = op_kw;

	// Lowercase RHS only for primitive type names; STRUCT_CALL stays original case.
	char *rhs_norm = NULL;
	if (rhs) {
		if      (strcmp(rhs, "INT")   == 0) rhs_norm = strdup("int");
		else if (strcmp(rhs, "LONG")  == 0) rhs_norm = strdup("long");
		else if (strcmp(rhs, "SHORT") == 0) rhs_norm = strdup("short");
		else if (strcmp(rhs, "CHAR")  == 0) rhs_norm = strdup("char");
		else if (strcmp(rhs, "CHARS") == 0) rhs_norm = strdup("chars");
		else if (strcmp(rhs, "BOOL")  == 0) rhs_norm = strdup("bool");
		else if (strcmp(rhs, "FLOAT") == 0) rhs_norm = strdup("float");
		else if (strcmp(rhs, "PTR")   == 0) rhs_norm = strdup("pointer");
		else                                rhs_norm = strdup(rhs);
	}

	char *out = rhs_norm
	    ? format("%s_%s_%s", struct_part, suffix, rhs_norm)
	    : format("%s_%s", struct_part, suffix);

	free(struct_part);
	free(op_kw);
	free(rhs_norm);
	return out;
}

static void emit_expr(File f, Node *node);

static void emit_type_name(File f, Token *tok) {
	Type t = tok->ret_type ? tok->ret_type : tok->type;
	switch (t) {
	case INT:   fprintf(f, "int"); break;
	case LONG:  fprintf(f, "long"); break;
	case SHORT: fprintf(f, "short"); break;
	case CHAR:  fprintf(f, "char"); break;
	case CHARS: fprintf(f, "chars"); break;
	case BOOL:  fprintf(f, "bool"); break;
	case FLOAT: fprintf(f, "float"); break;
	case VOID:  fprintf(f, "void"); break;
	case PTR:   fprintf(f, "pointer"); break;
	case STRUCT_CALL:
		if (tok->Struct.ptr) fprintf(f, "%s", tok->Struct.ptr->token->name);
		else fprintf(f, "%s", tok->name ? tok->name : "?");
		break;
	case ARRAY_TYPE: case ARRAY: {
		fprintf(f, "array");
		int depth = tok->Array.depth > 0 ? tok->Array.depth : 1;
		for (int i = 0; i < depth; i++) fputc('[', f);
		const char *en = type_to_ura_name(tok->Array.sub_type);
		if (en) fprintf(f, "%s", en);
		else if (tok->Array.sub_type == STRUCT_CALL && tok->Array.struct_ptr)
			fprintf(f, "%s", tok->Array.struct_ptr->token->name);
		for (int i = 0; i < depth; i++) fputc(']', f);
		break;
	}
	case FN_TYPE: {
		fprintf(f, "fn(");
		for (int i = 0; i < tok->Fn.params_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_type_name(f, tok->Fn.params[i]);
		}
		fprintf(f, ") ");
		if (tok->Fn.ret) emit_type_name(f, tok->Fn.ret);
		else fprintf(f, "void");
		break;
	}
	default: fprintf(f, "%s", to_string(t)); break;
	}
}

static void emit_chars_literal(File f, const char *s) {
	fprintf(f, "\"");
	for (int i = 0; s[i]; i++) {
		switch (s[i]) {
		case '\n': fprintf(f, "\\n"); break;
		case '\t': fprintf(f, "\\t"); break;
		case '\r': fprintf(f, "\\r"); break;
		case '\\': fprintf(f, "\\\\"); break;
		case '"':  fprintf(f, "\\\""); break;
		default:   fputc(s[i], f); break;
		}
	}
	fprintf(f, "\"");
}

static void emit_char_literal(File f, int c) {
	fprintf(f, "'");
	switch (c) {
	case '\n': fprintf(f, "\\n"); break;
	case '\t': fprintf(f, "\\t"); break;
	case '\r': fprintf(f, "\\r"); break;
	case '\\': fprintf(f, "\\\\"); break;
	case '\'': fprintf(f, "\\'"); break;
	case '\0': fprintf(f, "\\0"); break;
	default:   fputc(c, f); break;
	}
	fprintf(f, "'");
}

static void emit_expr_call(File f, Node *node, Token *tok) {
	// Method call: `<recv>.method(args)` -> `<Struct>_method(ref <recv>, args...)`.
	// In the post-IR AST, the call's last argument is the receiver (STRUCT_CALL),
	// and tok->name is the IR-mangled `<Struct>.<method>` form.
	if (node->left && tok->name && strchr(tok->name, '.') && !tok->is_static_call) {
		int last = node->left->children_count - 1;
		if (last >= 0 && node->left->children[last]->token->type == STRUCT_CALL) {
			char *flat = prep_fn_flat_name(tok->name);
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, node->left->children[last]);
			for (int i = 0; i < last; i++) {
				fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
			fprintf(f, ")");
			free(flat);
			return;
		}
	}
	// Static dispatch: `Type::method(args)` -> `Type_method(args)`.
	if (tok->is_static_call && tok->name && strchr(tok->name, '.')) {
		char *flat = prep_fn_flat_name(tok->name);
		fprintf(f, "%s(", flat);
		if (node->left)
			for (int i = 0; i < node->left->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
		fprintf(f, ")");
		free(flat);
		return;
	}
	fprintf(f, "%s(", tok->name);
	if (node->left)
		for (int i = 0; i < node->left->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->left->children[i]);
		}
	fprintf(f, ")");
}

// Always-parenthesize binary-op operand when it's itself a binary op. This
// preserves precedence after the prep round-trip without maintaining a full
// precedence table — slightly noisier output, but unambiguous.
static bool is_binop_token_type(Type t) {
	return includes(t,
	    ADD, SUB, MUL, DIV, MOD,
	    EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
	    AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, 0);
}

static void emit_binop_operand(File f, Node *child) {
	bool wrap = child && child->token && is_binop_token_type(child->token->type)
	    && !child->token->Fcall.ptr;
	if (wrap) fprintf(f, "(");
	emit_expr(f, child);
	if (wrap) fprintf(f, ")");
}

static void emit_expr_binop(File f, Node *node, Token *tok) {
	if (tok->Fcall.ptr) {
		// Operator overload resolved by typechecker.
		// Emit `<Struct>_<op>_<RHS>(ref lhs, rhs)`.
		char *flat = prep_fn_flat_name(tok->Fcall.ptr->token->name);
		fprintf(f, "%s(ref ", flat);
		emit_expr(f, node->left);
		fprintf(f, ", ");
		emit_expr(f, node->right);
		fprintf(f, ")");
		free(flat);
		return;
	}
	emit_binop_operand(f, node->left);
	fprintf(f, " %s ", op_symbol(tok->type));
	emit_binop_operand(f, node->right);
}

static void emit_expr(File f, Node *node) {
	if (!node || !node->token) return;
	Token *tok = node->token;
	switch (tok->type) {
	case INT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%lld", (long long)tok->Int.value);
		break;
	case LONG:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%lld", tok->Long.value);
		break;
	case SHORT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%d", tok->Short.value);
		break;
	case FLOAT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%f", tok->Float.value);
		break;
	case BOOL:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%s", tok->Bool.value ? "True" : "False");
		break;
	case CHAR:
		if (tok->name) fprintf(f, "%s", tok->name);
		else emit_char_literal(f, (unsigned char)tok->Char.value);
		break;
	case CHARS:
		if (tok->name) fprintf(f, "%s", tok->name);
		else emit_chars_literal(f, tok->Chars.value ? tok->Chars.value : "");
		break;
	case NULLABLE:    fprintf(f, "null"); break;
	case VOID:        fprintf(f, "void"); break;
	case STRUCT_CALL: fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case ARRAY_TYPE:  if (tok->name) fprintf(f, "%s", tok->name); break;
	case ID:          fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case FN_TYPE:
		// Fn-typed value: either a local variable (use its name) or a top-level
		// function reference resolved by ir_id (use the resolved fn's name).
		if (tok->Fcall.ptr && tok->Fcall.ptr->token->name)
			fprintf(f, "%s", tok->Fcall.ptr->token->name);
		else if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "?");
		break;
	case DOT:
		emit_expr(f, node->left);
		fprintf(f, ".");
		if (node->right && node->right->token->name) fprintf(f, "%s", node->right->token->name);
		break;
	case ACCESS:
		emit_expr(f, node->left);
		fprintf(f, "[");
		emit_expr(f, node->right);
		fprintf(f, "]");
		break;
	case DOUBLE_DOTS:
		// `Type::method` becomes `Type_method` in the flat prep form.
		emit_expr(f, node->left);
		fprintf(f, "_");
		emit_expr(f, node->right);
		break;
	case AS: {
		// Parenthesize the value if it's any binary op so `(a+b) as long`
		// doesn't degrade into `a+b as long` (which would cast only `b`).
		bool needs_parens = node->left && node->left->token && includes(
		    node->left->token->type,
		    ADD, SUB, MUL, DIV, MOD,
		    EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
		    AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, 0);
		if (needs_parens) fprintf(f, "(");
		emit_expr(f, node->left);
		if (needs_parens) fprintf(f, ")");
		fprintf(f, " as ");
		emit_type_name(f, node->right->token);
		break;
	}
	case NOT:  fprintf(f, "not "); emit_expr(f, node->left); break;
	case BNOT: fprintf(f, "~"); emit_expr(f, node->left); break;
	case TYPEOF:
		fprintf(f, "typeof(");
		emit_expr(f, node->left);
		fprintf(f, ")");
		break;
	case SIZEOF:
		fprintf(f, "sizeof(");
		if (node->left && node->left->token) {
			const char *tn = type_to_ura_name(node->left->token->type);
			if (tn) fprintf(f, "%s", tn);
			else emit_expr(f, node->left);
		}
		fprintf(f, ")");
		break;
	case REF:       fprintf(f, "ref "); emit_expr(f, node->left); break;
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:    emit_expr_binop(f, node, tok); break;
	case FCALL:     emit_expr_call(f, node, tok); break;
	case HEAP:      case STACK: {
		// `stack[T](dims)` with `Array.depth` opening/closing brackets — preserves
		// the multi-dimensional form: depth=2 -> `stack[[T]](r, c)`.
		fprintf(f, "%s", tok->type == HEAP ? "heap" : "stack");
		int depth = tok->Array.depth > 0 ? tok->Array.depth : 1;
		for (int i = 0; i < depth; i++) fputc('[', f);
		const char *en = type_to_ura_name(tok->Array.sub_type);
		if (en) fprintf(f, "%s", en);
		else if (tok->Array.sub_type == STRUCT_CALL && tok->Array.struct_ptr)
			fprintf(f, "%s", tok->Array.struct_ptr->token->name);
		else fprintf(f, "?");
		for (int i = 0; i < depth; i++) fputc(']', f);
		fprintf(f, "(");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, ")");
		break;
	}
	case ARRAY_LIT:
		fprintf(f, "[");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, "]");
		break;
	default: fprintf(f, "?"); break;
	}
}

static bool prep_fdec_has_self(Node *node) {
	if (!node->left || node->left->children_count == 0) return false;
	Token *last = node->left->children[node->left->children_count - 1]->token;
	return last->type == STRUCT_CALL && last->name && strcmp(last->name, "self") == 0;
}

// Print a parameter or local declaration name + type, using `ref name type` form
// when is_ref is set.
static void emit_param_decl(File f, Token *p) {
	if (p->is_ref) fprintf(f, "ref %s ", p->name);
	else fprintf(f, "%s ", p->name);
	emit_type_name(f, p);
}

// Emit destructor calls for every struct-typed local in `scope_node` that has a
// destructor defined. Skip the variable in `skip` (used when returning a
// struct so the caller owns the result). Mirrors emit_scope_clean in the asm
// path, but emitted as visible Ura source.
static void emit_struct_destructors(File f, Node *scope_node, int depth, Token *skip) {
	for (int i = 0; i < scope_node->variables_count; i++) {
		Token *v = scope_node->variables[i];
		if (!v || v == skip)                                     continue;
		if (v->is_ref || v->is_param)                            continue;
		if (v->type != STRUCT_CALL || !v->Struct.ptr)            continue;
		if (!v->Struct.ptr->token->has_clean)                    continue;
		emit_indent(f, depth);
		fprintf(f, "%s_delete(ref %s)\n", v->Struct.ptr->token->name, v->name);
	}
}

static void emit_node(File f, Node *node, int depth);

// Emit one fdec.
//   `flat_name`: when non-NULL, emit this flat name instead of decoding tok->name
//                (used when the caller is hoisting a struct method to top level).
//   `struct_owner`: when non-NULL, this fdec was a struct method. If it had self,
//                   inject `ref self <Struct>` as the first parameter.
static void emit_fdec(File f, Node *node, int depth,
                      const char *flat_name, Node *struct_owner) {
	Token *tok      = node->token;
	bool   has_self = prep_fdec_has_self(node);
	emit_indent(f, depth);
	if (tok->is_proto) fprintf(f, "proto ");

	const char *name_to_emit;
	char       *owned = NULL;
	if (flat_name) {
		name_to_emit = flat_name;
	} else if (struct_owner) {
		owned        = prep_fn_flat_name(tok->name);
		name_to_emit = owned;
	} else if (tok->name && strchr(tok->name, '.')) {
		owned        = prep_fn_flat_name(tok->name);
		name_to_emit = owned;
	} else {
		name_to_emit = tok->name ? tok->name : "?";
	}
	fprintf(f, "fn %s(", name_to_emit);

	int  printed = 0;
	bool need_self_inject = struct_owner && has_self;
	if (need_self_inject) {
		fprintf(f, "ref self %s", struct_owner->token->name);
		printed = 1;
	}
	if (node->left) {
		int end = node->left->children_count - (has_self ? 1 : 0);
		for (int i = 0; i < end; i++) {
			Token *p = node->left->children[i]->token;
			if (printed > 0) fprintf(f, ", ");
			emit_param_decl(f, p);
			printed++;
		}
	}
	if (tok->is_variadic) {
		if (printed > 0) fprintf(f, ", ");
		fprintf(f, "...");
	}
	fprintf(f, ") ");
	if (tok->is_ref) fprintf(f, "ref ");
	if (tok->ret_type == TUPLE) {
		fprintf(f, "(");
		for (int i = 0; i < tok->Tuple.types_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_type_name(f, tok->Tuple.types[i]);
		}
		fprintf(f, ")");
	} else if (tok->ret_type == STRUCT_CALL && tok->Struct.ptr)
		fprintf(f, "%s", tok->Struct.ptr->token->name);
	else if (tok->ret_type) {
		const char *tn = type_to_ura_name(tok->ret_type);
		fprintf(f, "%s", tn ? tn : to_string(tok->ret_type));
	}
	if (tok->is_proto) {
		fprintf(f, "\n");
		free(owned);
		return;
	}
	fprintf(f, ":\n");
	// Emit body. Inject struct-destructor calls right before any `return`
	// statement we encounter, and once more at the very end if the function
	// fell through (void / implicit return).
	bool ends_with_return = false;
	for (int i = 0; i < node->children_count; i++) {
		Node *child = node->children[i];
		if (child && child->token && child->token->type == RETURN) {
			// Determine which struct local (if any) is being returned — skip
			// its destructor since ownership transfers to the caller.
			Token *ret_skip = NULL;
			if (child->left && child->left->token
			    && child->left->token->type == STRUCT_CALL)
				ret_skip = child->left->token;
			emit_struct_destructors(f, node, depth + 1, ret_skip);
			ends_with_return = (i == node->children_count - 1);
		}
		emit_node(f, child, depth + 1);
	}
	if (!ends_with_return)
		emit_struct_destructors(f, node, depth + 1, NULL);
	fprintf(f, "\n");
	free(owned);
}

// Emit only struct fields. Methods are hoisted; the caller emits them after.
static void emit_struct_def(File f, Node *node, int depth) {
	emit_indent(f, depth);
	fprintf(f, "struct %s:\n", node->token->name);
	for (int i = 0; i < node->children_count; i++) {
		Token *field = node->children[i]->token;
		emit_indent(f, depth + 1);
		emit_param_decl(f, field);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
}

// Emit hoisted methods (if any) for a struct as top-level free functions.
static void emit_struct_methods(File f, Node *struct_def) {
	for (int i = 0; i < struct_def->functions_count; i++)
		emit_fdec(f, struct_def->functions[i], 0, NULL, struct_def);
}

static void emit_assign_node(File f, Node *node, int depth) {
	Token *tok  = node->token;
	Node  *left = node->left;
	emit_indent(f, depth);

	// LHS is a declaration: emit `name type [= rhs]`.
	// `tok->is_dec` is stamped at parse time when this ASSIGN's LHS was a fresh
	// declaration (not a re-assignment). is_dec on the LHS token alone is
	// unreliable post-IR because ir_id re-points re-uses to the decl token.
	if (tok->type == ASSIGN && tok->is_dec && left && left->token) {
		Token *lt = left->token;
		if (lt->is_ref) fprintf(f, "ref %s ", lt->name);
		else fprintf(f, "%s ", lt->name);
		emit_type_name(f, lt);
		fprintf(f, " = ");
		emit_expr(f, node->right);
		fprintf(f, "\n");
		return;
	}

	// Operator-overloaded assign: always emit as a flat call. The op's RHS
	// param is ref-typed, and the parser rejects `ref <fcall_result>`. When
	// the RHS isn't a "simple" named lvalue (literal, ID, field, index), we
	// hoist it into a fresh `__prep_tmpN <type>` local first and pass the
	// temp by ref.
	Token *rt = node->right ? node->right->token : NULL;
	bool rhs_simple = rt && (
	    rt->name != NULL
	    || includes(rt->type,
	        INT, LONG, SHORT, FLOAT, BOOL, CHAR, CHARS, NULLABLE, DOT, ACCESS, 0));
	if (tok->Fcall.ptr && rt && rt->type == FCALL) rhs_simple = false;
	if (tok->Fcall.ptr && includes(rt ? rt->type : 0,
	    ADD, SUB, MUL, DIV, MOD,
	    EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL,
	    AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, AS, 0))
		rhs_simple = false;

	if (tok->Fcall.ptr) {
		char *flat = prep_fn_flat_name(tok->Fcall.ptr->token->name);
		if (rhs_simple) {
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, node->left);
			fprintf(f, ", ");
			emit_expr(f, node->right);
			fprintf(f, ")\n");
		} else {
			// Resolve the RHS function's second-parameter type for the temp
			// decl. tok->Fcall.ptr->left->children = [rhs_param, self].
			Node  *op_fn   = tok->Fcall.ptr;
			Token *rhs_param = NULL;
			if (op_fn->left && op_fn->left->children_count >= 1)
				rhs_param = op_fn->left->children[0]->token;
			static int prep_tmp_id = 0;
			char *tmp_name = format("__prep_tmp%d", prep_tmp_id++);
			fprintf(f, "%s ", tmp_name);
			if (rhs_param) emit_type_name(f, rhs_param);
			else fprintf(f, "chars");
			fprintf(f, " = ");
			emit_expr(f, node->right);
			fprintf(f, "\n");
			emit_indent(f, depth);
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, node->left);
			fprintf(f, ", %s)\n", tmp_name);
			free(tmp_name);
		}
		free(flat);
		return;
	}

	// Plain assign / compound-assign on existing variables.
	emit_expr(f, node->left);
	fprintf(f, " %s ", op_symbol(tok->type));
	emit_expr(f, node->right);
	fprintf(f, "\n");
}

static void emit_output_arg(File f, Node *arg) {
	Token *t = arg->token;
	if (t->type == STRUCT_CALL && t->Struct.ptr) {
		Node *out_fn = prep_find_struct_op(t->Struct.ptr, "output", NULL);
		if (!out_fn) {
			// Try generic search by name suffix.
			char *want = format("%s" OP_PREFIX "output", t->Struct.ptr->token->name);
			for (int i = 0; i < t->Struct.ptr->functions_count; i++) {
				Node *fn = t->Struct.ptr->functions[i];
				if (fn->token->name && strcmp(fn->token->name, want) == 0) {
					out_fn = fn;
					break;
				}
			}
			free(want);
		}
		if (out_fn) {
			char *flat = prep_fn_flat_name(out_fn->token->name);
			fprintf(f, "%s(ref ", flat);
			emit_expr(f, arg);
			fprintf(f, ")");
			free(flat);
			return;
		}
	}
	emit_expr(f, arg);
}

static void emit_node(File f, Node *node, int depth) {
	if (!node || !node->token) return;
	Token *tok = node->token;
	switch (tok->type) {
	case PROTO:
		if (node->left && node->left->token->type == FDEC)
			emit_node(f, node->left, depth);
		break;
	case FDEC:       emit_fdec(f, node, depth, NULL, NULL); break;
	case STRUCT_DEF:
		emit_struct_def(f, node, depth);
		emit_struct_methods(f, node);
		break;
	case STRUCT_CALL:
		if (tok->is_dec) {
			emit_indent(f, depth);
			emit_param_decl(f, tok);
			fprintf(f, "\n");
		}
		break;
	case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
	case DIV_ASSIGN:
	case MOD_ASSIGN: emit_assign_node(f, node, depth); break;
	case INT:        case LONG: case SHORT: case FLOAT: case BOOL: case CHAR:
	case CHARS:      case ARRAY_TYPE: case PTR:
		if (tok->is_dec) {
			emit_indent(f, depth);
			emit_param_decl(f, tok);
			fprintf(f, "\n");
		}
		break;
	case RETURN:
		emit_indent(f, depth);
		// Tuple return: children hold each returned value.
		if (node->children_count > 0) {
			fprintf(f, "return ");
			for (int i = 0; i < node->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->children[i]);
			}
			fprintf(f, "\n");
			break;
		}
		if (node->left && node->left->token && node->left->token->type == VOID) {
			fprintf(f, "return\n");
			break;
		}
		fprintf(f, "return ");
		emit_expr(f, node->left);
		fprintf(f, "\n");
		break;
	case TUPLE_UNPACK:
		emit_indent(f, depth);
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_param_decl(f, node->children[i]->token);
		}
		fprintf(f, " = ");
		emit_expr(f, node->left);
		fprintf(f, "\n");
		break;
	case IF: case ELIF:
		emit_indent(f, depth);
		fprintf(f, "%s ", tok->type == IF ? "if" : "elif");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		if (node->right) emit_node(f, node->right, depth);
		break;
	case ELSE:
		emit_indent(f, depth);
		fprintf(f, "else:\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	case WHILE:
		emit_indent(f, depth);
		fprintf(f, "while ");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	case FOR: {
		// children[0..2]: from, to, step ; children[3..]: body
		emit_indent(f, depth);
		fprintf(f, "for %s from ", node->left->token->name);
		if (node->children_count > 0) emit_expr(f, node->children[0]);
		fprintf(f, " to ");
		if (node->children_count > 1) emit_expr(f, node->children[1]);
		bool default_step = node->children_count > 2
		    && node->children[2]->token->type == INT
		    && node->children[2]->token->name == NULL
		    && node->children[2]->token->Int.value == 1;
		if (node->children_count > 2 && !default_step) {
			fprintf(f, " step ");
			emit_expr(f, node->children[2]);
		}
		fprintf(f, ":\n");
		for (int i = 3; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	}
	case BREAK:    emit_indent(f, depth); fprintf(f, "break\n"); break;
	case CONTINUE: emit_indent(f, depth); fprintf(f, "continue\n"); break;
	case ENUM_DEF:
		emit_indent(f, depth);
		fprintf(f, "enum %s:\n", tok->name);
		emit_indent(f, depth + 1);
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			fprintf(f, "%s", node->children[i]->token->name);
		}
		fprintf(f, "\n");
		break;
	case MODULE:
		// Modules dissolve into flat top-level functions named `<mod>_<fn>`,
		// matching the call-site mangling in emit_expr_call. The `mod`
		// keyword itself disappears in the prep.
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth);
		for (int i = 0; i < node->functions_count; i++)
			emit_fdec(f, node->functions[i], depth, NULL, NULL);
		break;
	case FCALL:
		emit_indent(f, depth);
		emit_expr(f, node);
		fprintf(f, "\n");
		break;
	case OUTPUT:
		emit_indent(f, depth);
		fprintf(f, "output(");
		if (node->left) {
			for (int i = 0; i < node->left->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_output_arg(f, node->left->children[i]);
			}
		}
		fprintf(f, ")\n");
		break;
	default:
		emit_indent(f, depth);
		fprintf(f, "?\n");
		break;
	}
}

void emit_prep_file(Node *scope_node, char *path) {
	File f = fopen(path, "w");
	if (CHECK(!f, "cannot open %s for writing", path)) return;
	for (int i = 0; i < scope_node->children_count; i++)
		emit_node(f, scope_node->children[i], 0);
	fclose(f);
}
