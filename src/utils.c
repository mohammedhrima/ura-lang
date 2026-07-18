#include "header.h"

char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
	    [INT] = "INT", [BOOL] = "BOOL", [LONG] = "LONG", [FLOAT] = "FLOAT",
	    [FDEC] = "FDEC", [FCALL] = "CALL", [END] = "END", [LPAR] = "LPAR",
	    [RPAR] = "RPAR", [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE",
	    [WHILE] = "WHILE", [FOR] = "FOR", [TO] = "TO", [LOOP] = "LOOP",
	    [STEP] = "STEP", [IN] = "IN", [BREAK] = "BRK", [CONTINUE] = "CONT",
	    [MATCH] = "MATCH", [CASE] = "CASE", [DEFAULT] = "DEFAULT",
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
	    [NULLABLE] = "NULLABLE", [OPTIONAL] = "OPTIONAL",
	    //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
	    [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
	    [ENUM_DEF] = "ENUM_DEF", [ENUM_CALL] = "ENUM_CALL", [TUPLE] = "TUPLE",
	    [TUPLE_UNPACK] = "TUPLE_UNPACK", [LBRA] = "LBRA", [RBRA] = "RBRA",
	    [ARRAY] = "ARRAY", [LIST] = "LIST", [LIST_TYPE] = "LIST_TYPE",
	    [FN_TYPE] = "FN_TYPE", [RANGE] = "RANGE",
	    [DOT] = "DOT", [SYNTAX_ERROR] = "SYNTAX_ERROR", [MODULE] = "MODULE",
	    [OPERATOR] = "OPERATOR_KW", [PUB] = "PUB",
	    [DOUBLE_DOTS] = "DOUBLE_DOTS", [CLEAN] = "CLEAN", [NEW] = "NEW",
	};

	TODO(!res[type], "handle this case %d\n", type);
	return res[type];
}

char *type_name(Type type) {
	switch (type) {
	case INT:        return "int";
	case LONG:       return "long";
	case SHORT:      return "short";
	case CHAR:       return "char";
	case CHARS:      return "chars";
	case BOOL:       return "bool";
	case FLOAT:      return "float";
	case VOID:       return "void";
	case ARRAY_TYPE: return "array";
	case FN_TYPE:    return "fn";
	default:         return to_string(type);
	}
}

int _vprint(File out, const char *conv, va_list args) {
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

int _debug(char *conv, ...) {
	va_list args;
	va_start(args, conv);
	int res = _vprint(stdout, conv, args);
	va_end(args);
	return res;
}

double clock_now() {
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC, &ts);
   return ts.tv_sec + ts.tv_nsec / 1e9;
}

char *signal_name(int sig) {
   switch (sig) {
      case SIGSEGV: return "SIGSEGV";
      case SIGABRT: return "SIGABRT";
      case SIGTRAP: return "SIGTRAP";
      case SIGILL:  return "SIGILL";
      case SIGFPE:  return "SIGFPE";
      case SIGBUS:  return "SIGBUS";
      default:      return "signal";
   }
}

char *format(const char *fmt, ...) {
	char  *buf  = NULL;
	size_t size = 0;
	File   out  = open_memstream(&buf, &size);
	if (CHECK(!out, "format: open_memstream failed")) return NULL;

	va_list ap;
	va_start(ap, fmt);
	_vprint(out, fmt, ap);
	va_end(ap);
	fclose(out);
	return buf;
}

void print_escaped(char c) {
	switch (c) {
	case '\n': fputs("\\n", stdout); break;
	case '\t': fputs("\\t", stdout); break;
	case '\r': fputs("\\r", stdout); break;
	case '\\': fputs("\\\\", stdout); break;
	case '"':  fputs("\\\"", stdout); break;
	default:   putchar(c);
	}
}

void print_node_label(Node *node) {
	char *spelling[END + 1] = {
		[INT] = "int", [LONG] = "long", [SHORT] = "short", [BOOL] = "bool",
		[CHAR] = "char", [CHARS] = "chars", [FLOAT] = "float", [VOID] = "void",
		[ADD] = "+", [SUB] = "-", [MUL] = "*", [DIV] = "/", [MOD] = "%",
		[EQUAL] = "==", [NOT_EQUAL] = "!=", [LESS] = "<", [GREAT] = ">", [LESS_EQUAL] = "<=", [GREAT_EQUAL] = ">=",
		[AND] = "and", [OR] = "or", [NOT] = "not", [BAND] = "&", [BOR] = "|", [BXOR] = "^", [BNOT] = "~", [LSHIFT] = "<<", [RSHIFT] = ">>",
		[ASSIGN] = "=", [ADD_ASSIGN] = "+=", [SUB_ASSIGN] = "-=", [MUL_ASSIGN] = "*=", [DIV_ASSIGN] = "/=", [MOD_ASSIGN] = "%=",
		[IF] = "if", [ELIF] = "elif", [ELSE] = "else", [WHILE] = "while", [MATCH] = "match", [CASE] = "case", [DEFAULT] = "default",
		[BREAK] = "break", [CONTINUE] = "continue", [RETURN] = "return", [OUTPUT] = "output", [REF] = "ref", [AS] = "cast",
	};
	Token *token = node->token;
	switch (token->type) {
	case INT:   printf("int %lld", (long long)token->Int.value); return;
	case LONG:  printf("long %lld", token->Long.value); return;
	case SHORT: printf("short %d", token->Short.value); return;
	case FLOAT: printf("float %g", token->Float.value); return;
	case BOOL:  printf("bool %s", token->Bool.value ? "True" : "False"); return;
	case CHAR: {
		printf("char '");
		print_escaped(token->Char.value);
		printf("'");
		return;
	}
	case CHARS: {
		printf("chars \"");
		char *value = token->Chars.value;
		for (int i = 0; value[i]; i++)
			print_escaped(value[i]);
		printf("\"");
		return;
	}
	case ID:    printf("%s", token->name); break;
	case FCALL: printf("call %s", token->name); break;
	case FDEC: {
		printf("fn %s(", token->name);
		for (int i = 0; i < token->Fn.params_count; i++) {
			Token *param = token->Fn.params[i];
			printf("%s%s", i ? ", " : "", param->name);
			if (param->ret_type)
				printf(" : %s", spelling[param->ret_type] ? spelling[param->ret_type] : to_string(param->ret_type));
		}
		printf(")");
		break;
	}
	default:    printf("%s", spelling[token->type] ? spelling[token->type] : to_string(token->type));
	}
	if (token->ret_type)
		printf(" : %s", spelling[token->ret_type] ? spelling[token->ret_type] : to_string(token->ret_type));
}

void print_subtree(Node *node, char *prefix, bool is_last, char *role) {
	printf("%s%s", prefix, is_last ? "└─ " : "├─ ");
	if (role) printf("%s ", role);
	print_node_label(node);
	putchar('\n');
	char *child_prefix = format("%s%s", prefix, is_last ? "   " : "│  ");
	print_children(node, child_prefix);
	free(child_prefix);
}

void print_children(Node *node, char *prefix) {
	int    capacity = node->children_count + (node->left ? node->left->children_count : 0) + 2;
	Node  *edge_node[capacity];
	char  *edge_role[capacity];
	int    count = 0;
	Token *token = node->token;
	switch (token->type) {
	case IF:
	case ELIF: {
		edge_role[count] = "condition";
		edge_node[count++] = node->left;
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		if (node->right) {
			edge_role[count] = NULL;
			edge_node[count++] = node->right;
		}
		break;
	}
	case WHILE: {
		edge_role[count] = "condition";
		edge_node[count++] = node->left;
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case MATCH: {
		edge_role[count] = "subject";
		edge_node[count++] = node->left;
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case CASE: {
		for (int i = 0; node->left && i < node->left->children_count; i++) {
			edge_role[count] = "value";
			edge_node[count++] = node->left->children[i];
		}
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
		break;
	}
	case BREAK:
	case CONTINUE:
		break;
	case AS: {
		edge_role[count] = NULL;
		edge_node[count++] = node->left;
		break;
	}
	default: {
		if (node->left) {
			edge_role[count] = NULL;
			edge_node[count++] = node->left;
		}
		if (node->right) {
			edge_role[count] = NULL;
			edge_node[count++] = node->right;
		}
		for (int i = 0; i < node->children_count; i++) {
			edge_role[count] = NULL;
			edge_node[count++] = node->children[i];
		}
	}
	}
	for (int i = 0; i < count; i++)
		print_subtree(edge_node[i], prefix, i == count - 1, edge_role[i]);
}

void print_ast(Node *head) {
	for (int i = 0; i < head->children_count; i++) {
		print_node_label(head->children[i]);
		putchar('\n');
		print_children(head->children[i], "");
		if (i < head->children_count - 1)
			putchar('\n');
	}
}

void pnode(Node *node, char *indent) {
	if (!node || !node->token || !ura.enable_debug) return;
	Node **subs     = NULL;
	int    subs_count    = 0;
	int    subs_size = 0;

#define push(n)                                                             \
	do {                                                                     \
		resize_array(subs, Node *);                                           \
		subs[subs_count++] = (n);                                             \
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
	for (int i = 0; i < subs_count; i++) {
		Node *child = subs[i];
		if (!child || !child->token || !child->token->type) continue;

		int         is_last = (i == subs_count - 1);
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

bool _check(char *filename, char *funcname, int line, bool cond, char *fmt, ...) {
	if (!cond) return cond;
	ura.error_count++;
	fprintf(stderr, RED("ura error: %s:%s:%d "), filename, funcname, line);
	va_list ap;
	va_start(ap, fmt);
	_vprint(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

void decolor(char *s) {
	int w = 0;
	for (int i = 0; s[i]; ) {
		if (s[i] == '\033' && s[i + 1] == '[') {
			i += 2;
			while (s[i] && s[i] != 'm') i++;
			if (s[i] == 'm') i++;
		} else
			s[w++] = s[i++];
	}
	s[w] = '\0';
}

void render_caret(File out, Token *token) {
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
	for (int i = line_no; i >= 10; i /= 10)
		gutter++;

	fprintf(out, "%*s \033[2m%s:%d:%d\033[0m\n", gutter, "", token->source->filename, line_no, col);
	fprintf(out, "%*s " BLUE("|") "\n", gutter, "");
	fprintf(out, BLUE("%*d |") " %.*s\n", gutter, line_no, line_end - line_start, content + line_start);
	fprintf(out, "%*s " BLUE("|") " ", gutter, "");
	for (int i = 0; i < col - 1; i++)
		fputc(content[line_start + i] == '\t' ? '\t' : ' ', out);
	fprintf(out, "\033[1;31m");
	for (int i = 0; i < span; i++)
		fputc('^', out);
	fprintf(out, RESET "\n");
}

void parse_error(Token *token, const char *fmt, ...) {
	ura.error_count++;
	ura.found_error = true;
	if (ura.error_count > ura.max_errors) {
		fprintf(stderr, RED("error: ") "Too many errors, stopping\n");
		return;
	}

	char  *buf = NULL;
	size_t len = 0;
	File   ms  = open_memstream(&buf, &len);
	fprintf(ms, RED("error: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(ms, fmt, ap);
	va_end(ap);
	fputc('\n', ms);
	render_caret(ms, token);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void tokenize_error(int line, int s, int e, const char *fmt, ...) {
	Source *src = ura.sources[ura.sources_pos];
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

void *allocate(int len, int size) {
	void *res = calloc(len, size);
	TODO(!res, "allocate did failed");
	return res;
}

void new_source(char *file_name) {
	// TODO: handle imported same file in multiple files
	Source *src   = allocate(1, sizeof(Source));
	src->filename = file_name;
	src->dirname = dirname(file_name);
	debug("open file %s\n", file_name);

	file_name = realpath(file_name, NULL);
	if (!file_name) {
		parse_error(NULL, "Cannot find file '%s'", src->filename);
		return;
	}
	File file = fopen(file_name, "r");
	if (!file) {
		parse_error(NULL, "Cannot open file '%s'", file_name);
		free(file_name);
		return;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	src->content = allocate((size + 1), sizeof(char));
	fread(src->content, size, sizeof(char), file);
	fclose(file);
	free(file_name);

	resize_array(ura.sources, Source *);
	ura.sources[ura.sources_count++] = src;
	ura.sources_pos++;
}

void exit_source() {
	ura.sources_pos--;
}

int parse_escape_seq(char *input, int s, int e, char *buf, int *ptr) {
	int j   = *ptr;
	int ret = s + 1;
	switch (input[s + 1]) {
	case 'n':  buf[j++] = '\n'; break;  // newline
	case 't':  buf[j++] = '\t'; break;  // tab
	case 'r':  buf[j++] = '\r'; break;  // carriage return
	case 'b':  buf[j++] = '\b'; break;  // backspace
	case 'f':  buf[j++] = '\f'; break;  // form feed
	case 'v':  buf[j++] = '\v'; break;  // vertical tab
	case 'a':  buf[j++] = '\a'; break;  // alert (bell)
	case '\\': buf[j++] = '\\'; break; 	// backslash
	case '"':  buf[j++] = '"' ; break;  // double quote
	case '\'': buf[j++] = '\''; break; 	// single quote
	case '?':  buf[j++] = '\?'; break;  // question mark (trigraph)
	case '0': {
		// three-digit octal: \0NN
		if (s + 2 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[j++] = (char)octal;
				ret      = s + 3;
				break;
			}
			buf[j++] = '\0';
			break;
		}
		// two-digit octal: \0N
		else if (s + 1 < e && isdigit(input[s + 2])) {
			int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[j++]  = (char)octal;
			ret       = s + 2;
			break;
		}
		// plain null
		buf[j++] = '\0';
		break;
	}
	case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
		// three-digit octal: \NNN
		if (s + 3 < e && isdigit(input[s + 2]) && isdigit(input[s + 3])) {
			int octal = (input[s + 1] - '0') * 64 + (input[s + 2] - '0') * 8 + (input[s + 3] - '0');
			if (octal <= 255) {
				buf[j++] = (char)octal;
				ret      = s + 3;
				break;
			}
			buf[j++] = input[s];
			break; // invalid, keep backslash
		}
		// two-digit octal
		else if (s + 2 < e && isdigit(input[s + 2])) {
			int octal = (input[s + 1] - '0') * 8 + (input[s + 2] - '0');
			buf[j++]  = (char)octal;
			ret       = s + 2;
			break;
		}
		// single-digit octal
		buf[j++] = (char)(input[s + 1] - '0');
		break;
	}
	case 'x': { // Hexadecimal: \xFF
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
			buf[j++] = (char)hex;
			ret      = s + 3;
			break;
		}
		buf[j++] = input[s]; // invalid hex escape, keep backslash
		break;
	}
	case 'u': { // \uXXXX — not fully implemented yet
		buf[j++] = input[s];
		break;
	}
	case 'U': { // \UXXXXXXXX — not fully implemented yet
		buf[j++] = input[s];
		break;
	}
	default: { // unknown escape, keep backslash
		buf[j++] = input[s]; 
		break;
	}
	}
	*ptr = j;
	return ret;
}

bool lex_spaces(char *content, int *i, int *line, int *indent, int default_indent)
{
	char c = content[*i];
	if (!isspace(c)) 
	 	return false;
	 
	if (c == '\n') {
		(*line)++;
		(*indent)    = default_indent;
	}
	else {
		int j = *i;
		while (j > 0 && (content[j - 1] == ' ' || content[j - 1] == '\t'))
			j--;
		if (j == 0 || content[j - 1] == '\n')
			(*indent) += (c == '\t') ? TAB : 1;
	}
	(*i)++;

	return true;
}

bool lex_multi_comment(char *content, int *i, int *line, int indent, int default_indent)
{
	(void)indent;
	(void)default_indent;
	int s = *i;
	if (strncmp(content + (*i), "/*", 2) != 0)
		return false;
	
	(*i) += 2;
	while (content[(*i)] && content[(*i) + 1] && strncmp(content + (*i), "*/", 2)) {
		if (content[(*i)] == '\n') (*line)++;
		(*i)++;
	}
	if (strncmp(content + (*i), "*/", 2) != 0) {
		tokenize_error((*line), s, s + 2, "Unterminated block comment, expected '*/'");
		return true;
	}
	(*i) += 2;
	return true;
}

bool lex_comment(char *content, int *i, int line, int indent, int default_indent)
{
	(void)line;
	(void)indent;
	(void)default_indent;
	if (strncmp(content + (*i), "//", 2) != 0)
		return false;

	while (content[(*i)] && content[(*i)] != '\n')
		(*i)++;
	return true;
}

bool lex_chars(char *content, int *i, int line, int indent, int default_indent)
{
	(void)default_indent;
	int s = *i;
	if (content[(*i)] != '\"')
		return false;

	(*i)++;
	while (content[(*i)] && content[(*i)] != '\"') {
		if (content[(*i)] == '\\' && content[(*i) + 1]) (*i)++;
		(*i)++;
	}
	if (content[(*i)] != '\"') {
		tokenize_error(line, s, s + 1, "Unterminated string literal, expected '\"'");
		return true;
	}
	(*i)++;
	parse_token(line, s + 1, (*i) - 1, CHARS, indent);
	return true;
}

bool lex_char(char *content, int *i, int line, int indent, int default_indent)
{
	(void)default_indent;
	int s = *i;
	if (content[(*i)] != '\'')
		return false;

	(*i)++;
	if (content[(*i)] == '\\' && content[(*i) + 1]) (*i)++;
	if (content[(*i)] && content[(*i)] != '\'')     (*i)++;
	if (content[(*i)] != '\'')                 {
		tokenize_error(line, s, s + 1, "Unterminated character literal, expected \"'\"");
		return true;
	}
	(*i)++;
	parse_token(line, s + 1, (*i) - 1, CHAR, indent);
	return true;
}

bool lex_number(char *content, int *i, int line, int indent, int default_indent)
{
	(void)default_indent;
	int s = *i;
	if (!isdigit(content[(*i)])) return false;
	while (isdigit(content[(*i)])) (*i)++;
	if (content[(*i)] == '.' && isdigit(content[(*i) + 1])) {
		(*i)++;
		while (isdigit(content[(*i)])) (*i)++;
		parse_token(line, s, *i, FLOAT, indent);
	}
	else
		parse_token(line, s, *i, INT, indent);
	return true;
}

bool lex_use(char *content, int *i, int s, int line, int indent, int default_indent)
{
	(void)indent;
	(void)default_indent;
	if (!((*i) - s == 3 && strncmp(content + s, "use", 3) == 0 && isspace(content[(*i)])))
		return false;
	while (isspace(content[(*i)]))
		(*i)++;
	if (content[(*i)] != '\"') {
		tokenize_error(line, (*i), (*i) + 1, "Expected '\"' after 'use'");
		return true;
	}
	(*i)++;
	{
		int start = (*i);
		while (content[(*i)] && content[(*i)] != '\"' && content[(*i)] != '\n')
			(*i)++;
		if (content[(*i)] != '\"') {
			tokenize_error(line, start - 1, start, "Unterminated 'use' path, expected closing '\"'");
			return true;
		}
		(*i)++;
		char *use = strndup(content + start, (*i) - start - 1);
		char *tmp = format("%s.ura", use);
		free(use);
		use = tmp;
		if (use[0] == '@') {
			char *rest = use + 1;
			if (*rest == '/') rest++;
			tmp = format("%s/%s", ura.lib, rest);
			free(use);
			use = tmp;
		} else if (use[0] != '/') {
			tmp            = format("%s/%s", ura.sources[ura.sources_pos]->dirname, use);
			free(use);
			use = tmp;
		}
		new_source(use);
	}
	return true;
}

bool lex_link(char *content, int *i, int s, int line, int indent, int default_indent)
{
	(void)indent;
	(void)default_indent;
	if (!((*i) - s == 4 && strncmp(content + s, "link", 4) == 0 && isspace(content[(*i)])))
		return false;
	while (isspace(content[(*i)]))
		(*i)++;
	if (content[(*i)] != '\"') {
		tokenize_error(line, (*i), (*i) + 1, "Expected '\"' after 'link'");
		return true;
	}
	(*i)++;
	int link_s = (*i);
	while (content[(*i)] && content[(*i)] != '\"' && content[(*i)] != '\n')
		(*i)++;
	if (content[(*i)] != '\"') {
		tokenize_error(line, link_s - 1, link_s,
							"Unterminated 'link' path, expected closing '\"'");
		return true;
	}
	(*i)++;
	return true;
}

bool lex_identifier(char *content, int *i, int line, int indent, int default_indent)
{
	int s = *i;
	if (!(isalpha(content[*i]) || strchr("@$_", content[*i])))
		return false;
	while (content[*i] && (isalnum(content[*i]) || strchr("@$_", content[*i])))
		(*i)++;
	if (lex_use(content, i, s, line, indent, default_indent)) {
		if (ura.error_count) return true;
		ura.calling_use++;
		tokenize(indent);
		ura.calling_use--;
	} else if (!lex_link(content, i, s, line, indent, default_indent)) {
		parse_token(line, s, *i, ID, indent);
	}
	return true;
}

bool lex_symbol(char *content, int *i, int line, int *indent, int default_indent)
{
	(void)default_indent;
	int s = *i;
	static const Keyword specials[] = {
		{"...", VARIADIC, 0, 0},   {"..", RANGE, 0, 0},       {".", DOT, 0, 0},
		{"::", DOUBLE_DOTS, 0, 0}, {":", DOTS, 0, 0},         {"+=", ADD_ASSIGN, 0, 0},
		{"-=", SUB_ASSIGN, 0, 0},  {"*=", MUL_ASSIGN, 0, 0},  {"/=", DIV_ASSIGN, 0, 0},
		{"%=", MOD_ASSIGN, 0, 0},  {"!=", NOT_EQUAL, 0, 0},   {"!", NOT, 0, 0},
		{"==", EQUAL, 0, 0},       {"<<", LSHIFT, 0, 0},      {">>", RSHIFT, 0, 0},
		{"<=", LESS_EQUAL, 0, 0},  {">=", GREAT_EQUAL, 0, 0}, {"<", LESS, 0, 0},
		{">", GREAT, 0, 0},        {"=", ASSIGN, 0, 0},       {"+", ADD, 0, 0},
		{"-", SUB, 0, 0},          {"*", MUL, 0, 0},          {"/", DIV, 0, 0},
		{"%", MOD, 0, 0},          {"(", LPAR, 0, 0},         {")", RPAR, 0, 0},
		{"[", LBRA, 0, 0},         {"]", RBRA, 0, 0},         {",", COMA, 0, 0},
		{"&&", AND, 0, 0},         {"||", OR, 0, 0},          {"&", BAND, 0, 0},
		{"|", BOR, 0, 0},          {"^", BXOR, 0, 0},         {"~", BNOT, 0, 0},
		{"?", OPTIONAL, 0, 0},
	};
	for (size_t j = 0; j < sizeof(specials) / sizeof(*specials); j++) {
		size_t len = strlen(specials[j].name);
		if (strncmp(specials[j].name, content + (*i), len) == 0) {
			(*i) += len;
			parse_token(line, s, (*i), specials[j].type, *indent);
			if (includes(specials[j].type, DOTS, 0)) (*indent) += TAB;
			return true;
		}
	}
	return false;
}

Token *new_token(Type type, int indent) {
	Token *token = allocate(1, sizeof(Token));
	token->type  = type;
	token->indent = ((indent + TAB / 2) / TAB) * TAB;
	resize_array(ura.tokens, Token *);
	ura.tokens[ura.tokens_count++] = token;
	return token;
}

Token *parse_token(int line, int s, int e, Type type, int indent) {
	Source *src = ura.sources[ura.sources_pos - 1];
	Token *new       = new_token(type, indent);
	char *input      = src->content;
	new->line        = line;
	new->source      = src;
	new->start_index = s;
	new->end_index   = e;

	switch (type) {
	case INT: {
		while (s < e) new->Int.value = new->Int.value * 10 + input[s++] - '0';
		break;
	}
	case FLOAT: {
		// TODO: to be checked
		char buf[64] = {0};
		int  len     = e - s;
		if (len > 63) len = 63;
		strncpy(buf, input + s, len);
		new->Float.value = (float)atof(buf);
		break;
	}
	case ID: {
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);

		if (strcmp(new->name, "True") == 0 || strcmp(new->name, "False") == 0) {
			new->type       = BOOL;
			new->Bool.value = (new->name[0] == 'T');
			set_name(new, NULL);
			break;
		}

		static const Keyword keywords[] = {
			{"int", INT, 1, 1}, {"bool", BOOL, 1, 1}, {"chars", CHARS, 1, 1},
			{"char", CHAR, 1, 1}, {"float", FLOAT, 1, 1}, {"void", VOID, 1, 1},
			{"long", LONG, 1, 1}, {"pointer", CHARS, 1, 1}, {"short", SHORT, 1, 1},
			{"if", IF, 0, 0}, {"elif", ELIF, 0, 0}, {"else", ELSE, 0, 0},
			{"while", WHILE, 0, 0}, {"loop", LOOP, 0, 0}, {"for", FOR, 0, 0},
			{"to", TO, 0, 0}, {"step", STEP, 0, 0}, {"in", IN, 0, 0},
			{"fn", FDEC, 0, 0}, {"return", RETURN, 0, 0}, {"break", BREAK, 0, 0},
			{"continue", CONTINUE, 0, 0}, {"match", MATCH, 0, 0},
			{"case", CASE, 0, 0}, {"default", DEFAULT, 0, 0}, {"ref", REF, 0, 0},
			{"struct", STRUCT_DEF, 0, 0}, {"enum", ENUM_DEF, 0, 0},
			{"proto", PROTO, 0, 0}, {"mod", MODULE, 0, 0},
			{"operator", OPERATOR, 0, 0}, {"as", AS, 0, 0}, {"pub", PUB, 0, 0},
			{"clean", CLEAN, 0, 0}, {"new", NEW, 0, 0}, {"and", AND, 0, 1},
			{"or", OR, 0, 1}, {"is", EQUAL, 0, 1}, {"not", NOT, 0, 1},
			{"typeof", TYPEOF, 0, 1}, {"sizeof", SIZEOF, 0, 1},
			{"stack", STACK, 0, 1}, {"heap", HEAP, 0, 1},
			{"array", ARRAY_TYPE, 0, 1}, {"List", LIST_TYPE, 0, 1},
			{"null", NULLABLE, 0, 1},
		};

		for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
			if (strcmp(new->name, keywords[i].name) != 0) continue;
			new->type   = keywords[i].type;
			new->is_dec = keywords[i].is_dec;
			if (keywords[i].is_dec || keywords[i].clear_name) set_name(new, NULL);
			break;
		}
		break;
	}
	case FDEC: {
		if (e <= s) break;
		new->name = allocate(e - s + 1, sizeof(char));
		strncpy(new->name, input + s, e - s);
		break;
	}
	case CHARS: {
		if (e <= s) {
			new->Chars.value = allocate(1, sizeof(char));
			break;
		}
		int len          = e - s;
		new->Chars.value = allocate(len + 1, sizeof(char));
		char *value      = new->Chars.value;
		int   j          = 0;
		for (int i = 0; i < len && s < e; i++, s++) {
			if (s + 1 < e && input[s] == '\\') s = parse_escape_seq(input, s, e, value, &j);
			else value[j++] = input[s];
		}
		if (j < len) {
			char *resized = allocate(j + 1, sizeof(char));
			strcpy(resized, value);
			new->Chars.value = resized;
		}
		break;
	}
	case CHAR: {
		char buf[4] = {0};
		int  j      = 0;
		if (e == s + 1) {
			buf[j++] = input[s];
		} else {
			parse_escape_seq(input, s, e, buf, &j);
			if (buf[0] == '\\' && input[s + 1] != '\\')
				tokenize_error(line, s, e, "Unknown escape character: \\%c", input[s + 1]);
		}
		new->Char.value = buf[0];
		break;
	}
	default: break;
	}
	return new;
}

void set_name(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

Token *next() {
	Token *token = peek(0);
	if (ura.exe_pos + 1 < ura.tokens_count) ura.exe_pos++;
	return token;
}

Token *peek(int index) {
	int pos = ura.exe_pos + index;
	if (pos >= ura.tokens_count) pos = ura.tokens_count - 1;
	return ura.tokens[pos];
}

Token *find(Type type, ...) {
	va_list ap;
	va_start(ap, type);
	while (type && ura.tokens[ura.exe_pos]) {
		if (type == ura.tokens[ura.exe_pos]->type) return ura.tokens[ura.exe_pos++];
		type = va_arg(ap, Type);
	}
	return NULL;
}

int get_operation_precedence(Type type)
{
   switch(type)
   {
   case ASSIGN:      return 1;
   case ADD_ASSIGN:  return 1;
   case SUB_ASSIGN:  return 1;
   case MUL_ASSIGN:  return 1;
   case DIV_ASSIGN:  return 1;
   case MOD_ASSIGN:  return 1;
   case OR:          return 2;
   case RANGE:       return 2;
   case AND:         return 3;
   case BOR:         return 4;
   case BXOR:        return 5;
   case BAND:        return 6;
   case EQUAL:       return 7;
   case NOT_EQUAL:   return 7;
   case LESS:        return 8;
   case GREAT:       return 8;
   case LESS_EQUAL:  return 8;
   case GREAT_EQUAL: return 8;
   case LSHIFT:      return 9;
   case RSHIFT:      return 9;
   case ADD:         return 10;
   case SUB:         return 10;
   case MUL:         return 11;
   case DIV:         return 11;
   case MOD:         return 11;
   case AS:          return 12;
   default:
      break;
   }
   return 0;
}

bool includes(Type to_find, ...) {
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

bool within(int indent) {
	Token *curr = ura.tokens[ura.exe_pos];
	return !ura.found_error && curr->indent > indent && curr->type != END;
}

void parser_recover(int indent) {
	int start_line = ura.exe_pos < ura.tokens_count ? ura.tokens[ura.exe_pos]->line : -1;
	while (ura.exe_pos < ura.tokens_count && ura.tokens[ura.exe_pos]->type != END) {
		Token *t = ura.tokens[ura.exe_pos];
		if (t->indent <= indent && t->line != start_line) break;
		ura.exe_pos++;
	}
	ura.found_error = false;
}

Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

Node *syntax_error() {
	 return new_node(new_token(SYNTAX_ERROR, -1));
}

void enter_scope(Node *node) {
	ura.scopes_count++;
	debug(CYAN("Enter Scope: %k index %d\n"), node->token, ura.scopes_count);
	resize_array(ura.scopes, Node *);
	ura.scopes[ura.scopes_count] = node;
	ura.scope                = ura.scopes[ura.scopes_count];
}

void exit_scope() {
	if (CHECK(ura.scopes_count < 0, "No active scope to exit")) return;
	debug(CYAN("Exit Scope: %k index %d\n"), ura.scopes[ura.scopes_count]->token, ura.scopes_count);
	ura.scopes[ura.scopes_count] = NULL;
	ura.scopes_count--;
	ura.scope = ura.scopes[ura.scopes_count];
}

Node *enclosing_continue() {
	for (int i = ura.scopes_count; i >= 1; i--) {
		if (!ura.scopes[i]) continue;
		if (includes(ura.scopes[i]->token->type, WHILE, LOOP, FOR, 0)) return ura.scopes[i];
		if (ura.scopes[i]->token->type == FDEC)  return NULL;
	}
	return NULL;
}

Node *enclosing_break() {
	for (int i = ura.scopes_count; i >= 1; i--) {
		if (!ura.scopes[i]) continue;
		if (includes(ura.scopes[i]->token->type, WHILE, LOOP, FOR, MATCH, 0)) return ura.scopes[i];
		if (ura.scopes[i]->token->type == FDEC) return NULL;
	}
	return NULL;
}

void analyze_block(Node *node) {
	analyze(node->left);
	enter_scope(node);
	for (int i = 0; i < node->children_count; i++)
		analyze(node->children[i]);
	exit_scope();
	analyze(node->right);
}

void analyze_match(Node *node) {
	analyze(node->left);
	enter_scope(node);
	for (int i = 0; i < node->children_count; i++) {
		Node *branch = node->children[i];
		if (branch->left)
			for (int j = 0; j < branch->left->children_count; j++)
				analyze(branch->left->children[j]);
		enter_scope(branch);
		for (int j = 0; j < branch->children_count; j++)
			analyze(branch->children[j]);
		exit_scope();
	}
	exit_scope();
}

void type_check_block(Node *node) {
	type_check(node->left);
	if (node->left && node->left->token->ret_type != BOOL)
		parse_error(node->token, "The '%s' condition must be a bool, got %s",
		            node->token->name, type_name(node->left->token->ret_type));
	for (int i = 0; i < node->children_count; i++)
		type_check(node->children[i]);
	type_check(node->right);
}

void type_check_match(Node *node) {
	type_check(node->left);
	Type subject = node->left->token->ret_type;
	for (int i = 0; i < node->children_count; i++) {
		Node *branch = node->children[i];
		if (branch->left)
			for (int j = 0; j < branch->left->children_count; j++) {
				Node *value = branch->left->children[j];
				type_check(value);
				if (subject && value->token->ret_type && value->token->ret_type != subject)
					parse_error(value->token, "This case value is %s but the subject is %s; they must be the same type",
					            type_name(value->token->ret_type), type_name(subject));
			}
		for (int j = 0; j < branch->children_count; j++)
			type_check(branch->children[j]);
	}
}

void declare_variable(Token *token) {
	for (int i = 0; i < ura.scope->variables_count; i++)
		if (strcmp(ura.scope->variables[i]->name, token->name) == 0) {
			parse_error(token, "Redeclaration of variable '%s'", token->name);
			return;
		}
	resize_array(ura.scope->variables, Token *);
	ura.scope->variables[ura.scope->variables_count++] = token;
}

Token *find_variable(char *name, bool *captured) {
	bool passed_function = false;
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->variables_count; j++)
			if (scope->variables[j]->name && strcmp(scope->variables[j]->name, name) == 0) {
				if (captured) *captured = passed_function && scope->token->type == FDEC;
				return scope->variables[j];
			}
		if (scope->token->type == FDEC) passed_function = true;
	}
	if (captured) *captured = false;
	return NULL;
}

void declare_function(Node *fn) {
	for (int i = 0; i < ura.scope->functions_count; i++)
		if (strcmp(ura.scope->functions[i]->token->name, fn->token->name) == 0) {
			parse_error(fn->token, "Redeclaration of function '%s'", fn->token->name);
			return;
		}
	resize_array(ura.scope->functions, Node *);
	ura.scope->functions[ura.scope->functions_count++] = fn;
}

Node *find_function(char *name) {
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->functions_count; j++)
			if (scope->functions[j]->token->name && strcmp(scope->functions[j]->token->name, name) == 0)
				return scope->functions[j];
	}
	return NULL;
}

void parse_type(Token *target) {
	if (peek(0)->type == FDEC) {
		next();
		target->ret_type = FN_TYPE;
		if (!find(LPAR, 0)) {
			parse_error(peek(0), "Expected '(' in function type");
			return;
		}
		while (!ura.found_error && peek(0)->type != RPAR) {
			Token *param = new_token(ID, 0);
			parse_type(param);
			resize_array(target->Fn.params, Token *);
			target->Fn.params[target->Fn.params_count++] = param;
			while (find(COMA, 0));
		}
		if (!find(RPAR, 0))
			parse_error(peek(0), "Expected ')' in function type");
		target->Fn.ret = new_token(ID, 0);
		parse_type(target->Fn.ret);
	} else if (is_data_type(peek(0))) {
		target->ret_type = next()->type;
		int depth = 0;
		while (peek(0)->type == LBRA && peek(1)->type == RBRA) {
			find(LBRA, 0);
			find(RBRA, 0);
			depth++;
		}
		if (depth > 0) {
			target->Array.sub_type = target->ret_type;
			target->Array.depth    = depth;
			target->ret_type       = ARRAY_TYPE;
		}
	} else {
		parse_error(peek(0), "Expected a type");
	}
}

void parse_block(Node *node, int indent) {
	while (within(indent)) {
		resize_array(node->children, Node*);
		node->children[node->children_count++] = expr_node(0);
	}
}

Node *match_node(Node *node) {
	Token *token = node->token;
	node->left = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'match' body");
	while (within(token->indent) && includes(peek(0)->type, CASE, DEFAULT, 0)) {
		Token *keyword = next();
		Node  *branch  = new_node(keyword);
		if (keyword->type == CASE) {
			Node *values = new_node(keyword);
			while (!ura.found_error && peek(0)->type != DOTS) {
				resize_array(values->children, Node *);
				values->children[values->children_count++] = expr_node(0);
				if (!find(COMA, 0)) break;
			}
			if (values->children_count == 0)
				parse_error(keyword, "Expected an expression after 'case'");
			branch->left = values;
		}
		if (!find(DOTS, 0))
			parse_error(keyword, "Expected ':' to open the '%s' body", keyword->name);
		parse_block(branch, keyword->indent);
		resize_array(node->children, Node *);
		node->children[node->children_count++] = branch;
		if (keyword->type == DEFAULT) break;
	}
	return node;
}

Node *if_node(Node *node) {
	Token *token = node->token;
	node->left = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'if' body");
	parse_block(node, token->indent);
	Node *tail = node;
	while (includes(peek(0)->type, ELIF, ELSE, 0) && peek(0)->indent == token->indent) {
		Token *keyword = next();
		Node  *branch  = new_node(keyword);
		if (keyword->type == ELIF)
			branch->left = expr_node(0);
		if (!find(DOTS, 0))
			parse_error(keyword, "Expected ':' to open the '%s' body", keyword->name);
		parse_block(branch, keyword->indent);
		tail->right = branch;
		tail        = branch;
		if (keyword->type == ELSE) break;
	}
	return node;
}

Node *while_node(Node *node) {
	Token *token = node->token;
	node->left = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'while' body");
	parse_block(node, token->indent);
	return node;
}

Node *for_node(Node *node) {
	Token *token = node->token;
	Token *iter  = find(ID, 0);
	if (!iter) {
		parse_error(token, "Expected a loop variable after 'for'");
		return syntax_error();
	}
	iter->is_dec = true;
	node->left   = new_node(iter);
	if (!find(IN, 0))
		parse_error(token, "Expected 'in' after 'for %s'", iter->name);
	node->right = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'for' body");
	parse_block(node, token->indent);
	return node;
}

Node *ref_node(Node *node) {
	Token *token = node->token;
	bool nullable = peek(0)->type == OPTIONAL;
	if (nullable) find(OPTIONAL, 0);
	Token *name = peek(0);
	if (!name || name->type != ID) {
		parse_error(token, "Expected a variable after 'ref'");
		return syntax_error();
	}
	if (is_data_type(peek(1)) || (peek(1)->type == FDEC && peek(2)->type == LPAR)) {
		find(ID, 0);
		name->is_dec      = true;
		name->is_ref      = true;
		name->is_nullable = nullable;
		parse_type(name);
		if (!nullable && peek(0)->type != ASSIGN) {
			parse_error(name, "A reference must be bound when declared (use 'ref?' for an optional reference)");
			return syntax_error();
		}
		node->token = name;
		return node;
	}
	node->left = prime_node();
	return node;
}

Node *id_node(Node *node) {
	Token *token = node->token;
	if (peek(0)->type == LPAR) {
		if (strcmp(token->name, "main") == 0)
			return fdec_node(node);
		if (strcmp(token->name, "output") == 0)
			return output_node(node);
		return fcall_node(node);
	}
	if (peek(0)->type == LBRA)
		return access_node(node);
	if (is_data_type(peek(0)) || (peek(0)->type == FDEC && peek(1)->type == LPAR)) {
		token->is_dec = true;
		parse_type(token);
	}
	if (!token->is_dec && peek(0)->type == OPTIONAL) {
		find(OPTIONAL, 0);
		token->is_nullable = true;
	}
	return node;
}

Node *fdec_node(Node *node) {
	node->token->type = FDEC;
	enter_scope(node);
	if (!find(LPAR, 0))
		parse_error(node->token, "Expected '(' after function %s", node->token->name);
	while (!ura.found_error && peek(0)->type != RPAR) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			break;
		}
		bool   is_ref = find(REF, 0) != NULL;
		Token *param  = find(ID, 0);
		if (!param) {
			parse_error(node->token, "Expected parameter name in function %s", node->token->name);
			break;
		}
		parse_type(param);
		param->is_param = true;
		param->is_dec   = true;
		param->is_ref   = is_ref;
		resize_array(node->token->Fn.params, Token *);
		node->token->Fn.params[node->token->Fn.params_count++] = param;
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, "Expected ')' after function %s", node->token->name);

	if(strcmp(node->token->name, "main") == 0) {
		node->token->ret_type = INT;
	}
	else if(is_data_type(peek(0))) {
		node->token->ret_type = next()->type;
	}
	else
		parse_error(node->token, "Expected <data type> after function %s", node->token->name);
	if (!node->token->is_proto) {
		if (!find(DOTS, 0))
			parse_error(node->token, "Expected ':' after function %s", node->token->name);

		parse_block(node, node->token->indent);
	}

	exit_scope();
	return node;
}

Node *fcall_node(Node *node) {
	node->token->type = FCALL;
	if (!find(LPAR, 0))
		parse_error(node->token, "Expected '(' after %s", node->token->name);
	while (!ura.found_error && peek(0)->type != RPAR) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, "Expected ')' after %s arguments", node->token->name);
	return node;
}

Node *output_node(Node *node) {
	node->token->type     = OUTPUT;
	node->token->ret_type = VOID;
	if (!find(LPAR, 0))
		parse_error(node->token, "Expected '(' after output");
	while (!ura.found_error && peek(0)->type != RPAR) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, "Expected ')' after output arguments");
	return node;
}

Node *access_node(Node *node) {
	while (peek(0)->type == LBRA) {
		Token *bracket = next();
		bracket->type  = ACCESS;
		Node  *access  = new_node(bracket);
		access->left   = node;
		access->right  = expr_node(0);
		if (!find(RBRA, 0))
			parse_error(bracket, "Expected ']' after array index");
		if (find(OPTIONAL, 0))
			bracket->is_nullable = true;
		node = access;
	}
	return node;
}

Node *array_lit_node(Node *node) {
	node->token->type = ARRAY_LIT;
	while (!ura.found_error && peek(0)->type != RBRA) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RBRA, 0))
		parse_error(node->token, "Expected ']' to close array literal");
	return node;
}

Node *array_ctor_node(Node *node) {
	Type sub = node->token->type;
	node->token->type           = ARRAY;
	node->token->is_dec         = false;
	node->token->ret_type       = ARRAY_TYPE;
	node->token->Array.sub_type = sub;
	int depth = 0;
	while (peek(0)->type == LBRA) {
		find(LBRA, 0);
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		if (!find(RBRA, 0))
			parse_error(node->token, "Expected ']' after array size");
		depth++;
	}
	node->token->Array.depth = depth;
	return node;
}

bool is_data_type(Token *token) {
	return token->is_dec && includes(token->type, DATA_TYPES, 0); 
}

TypeRef to_llvm_type(Type type) {
   switch (type) {
   case INT:   return ura.i32;
   case LONG:  return ura.i64;
   case SHORT: return ura.i16;
   case CHAR:  return ura.i8;
   case BOOL:  return ura.i1;
   case CHARS: return LLVMPointerType(ura.i8, 0);
   case FLOAT: return ura.f32;
   case VOID:  return ura.vd;
   default: TODO(1, "to_llvm_type: unhandled type %t", type); return NULL;
   }
}

TypeRef array_type(Type sub, int depth) {
   TypeRef elem      = depth <= 1 ? to_llvm_type(sub) : array_type(sub, depth - 1);
   TypeRef fields[2] = { LLVMPointerType(elem, 0), ura.i64 };
   return LLVMStructTypeInContext(ura.context, fields, 2, 0);
}

TypeRef llvm_type_of(Token *token) {
   if (token->ret_type == ARRAY_TYPE) return array_type(token->Array.sub_type, token->Array.depth);
   if (token->ret_type != FN_TYPE) return to_llvm_type(token->ret_type);
   int      n      = token->Fn.params_count;
   TypeRef *params = n ? allocate(n, sizeof(TypeRef)) : NULL;
   for (int i = 0; i < n; i++) params[i] = llvm_type_of(token->Fn.params[i]);
   TypeRef ft = LLVMFunctionType(llvm_type_of(token->Fn.ret), params, n, 0);
   free(params);
   return LLVMPointerType(ft, 0);
}

Value default_value(Token *token) {
   if (includes(token->ret_type, NUMERIC_TYPES, 0)) return LLVMConstInt(llvm_type_of(token), 0, false);
   if (token->ret_type == FLOAT)                    return LLVMConstReal(llvm_type_of(token), 0.0);
   return LLVMConstNull(llvm_type_of(token));
}

void setup_paths(char *path_name) {
   char *slash = strrchr(path_name, '/');
   if (slash) {
      ura.dir = strdup(path_name);
      ura.dir[slash - path_name] = '\0';
      ura.base = strdup(slash + 1);
   } else {
      ura.dir = strdup(".");
      ura.base = strdup(path_name);
   }
   char *dot = strrchr(ura.base, '.');
   if (dot) *dot = '\0';
   ura.build_dir = format("%s/build", ura.dir);
   mkdir(ura.build_dir, 0755);
   ura.ll_path = format("%s/%s.ll", ura.build_dir, ura.base);
}

void init_module(char *name) {
   ura.context = LLVMContextCreate();
   ura.module  = LLVMModuleCreateWithNameInContext(name, ura.context);
   ura.builder = LLVMCreateBuilderInContext(ura.context);
   ura.vd  = LLVMVoidTypeInContext(ura.context);
   ura.i1  = LLVMInt1TypeInContext(ura.context);
   ura.i8  = LLVMInt8TypeInContext(ura.context);
   ura.i16 = LLVMInt16TypeInContext(ura.context);
   ura.i32 = LLVMInt32TypeInContext(ura.context);
   ura.i64 = LLVMInt64TypeInContext(ura.context);
   ura.f32 = LLVMFloatTypeInContext(ura.context);
   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();
   char *triple = LLVMGetDefaultTargetTriple();
   LLVMSetTarget(ura.module, triple);
   LLVMDisposeMessage(triple);

   if (!ura.enable_san) return;
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Debug Info Version", 18,
                     LLVMValueAsMetadata(LLVMConstInt(ura.i32, 3, 0)));
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13,
                     LLVMValueAsMetadata(LLVMConstInt(ura.i32, 4, 0)));
   ura.debug_builder = LLVMCreateDIBuilder(ura.module);
   char *src   = ura.sources[0]->filename;
   char *slash = strrchr(src, '/');
   char *file  = slash ? slash + 1 : src;
   ura.debug_file = LLVMDIBuilderCreateFile(ura.debug_builder, file, strlen(file),
                                            ura.dir, strlen(ura.dir));
   ura.debug_compile_unit = LLVMDIBuilderCreateCompileUnit(
       ura.debug_builder, LLVMDWARFSourceLanguageC, ura.debug_file, "ura", 3, 0, "", 0, 0, "", 0,
       LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);
   ura.debug_scope = ura.debug_compile_unit;
}

void finalize_module(char *ll_path) {
   char *error = NULL;
   PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
   if (ura.flags) {
      Error err = LLVMRunPasses(ura.module, ura.flags, NULL, opts);
      if (err) {
         char *msg = LLVMGetErrorMessage(err);
         CHECK(1, "optimizer error: %s", msg);
         LLVMDisposeErrorMessage(msg);
      }
   }
   if (ura.debug_builder) {
      LLVMDIBuilderFinalize(ura.debug_builder);
      LLVMDisposeDIBuilder(ura.debug_builder);
      ura.debug_builder = NULL;
   }
   if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
      CHECK(1, "module verification failed:\n%s", error);
   LLVMDisposeMessage(error);
   LLVMDisposePassBuilderOptions(opts);
   LLVMPrintModuleToFile(ura.module, ll_path, NULL);
}

void debug_enter_function(Token *token) {
   token->llvm.prev_block = LLVMGetInsertBlock(ura.builder);
   token->llvm.prev_scope = ura.debug_scope;
   token->llvm.prev_loc   = ura.debug_builder ? LLVMGetCurrentDebugLocation2(ura.builder) : NULL;
   Block entry = LLVMAppendBasicBlockInContext(ura.context, token->llvm.elem, "entry");
   LLVMPositionBuilderAtEnd(ura.builder, entry);
   if (ura.enable_san) {
      unsigned kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
      LLVMAddAttributeAtIndex(token->llvm.elem, LLVMAttributeFunctionIndex,
                              LLVMCreateEnumAttribute(ura.context, kind, 0));
   }
   if (!ura.debug_builder) return;
   MetadataRef di_type = LLVMDIBuilderCreateSubroutineType(
       ura.debug_builder, ura.debug_file, NULL, 0, LLVMDIFlagZero);
   MetadataRef di_func = LLVMDIBuilderCreateFunction(
       ura.debug_builder, ura.debug_compile_unit, token->name, strlen(token->name),
       token->name, strlen(token->name), ura.debug_file, token->line, di_type, 0, 1,
       token->line, LLVMDIFlagZero, 0);
   LLVMSetSubprogram(token->llvm.elem, di_func);
   ura.debug_scope = di_func;
   MetadataRef loc = LLVMDIBuilderCreateDebugLocation(ura.context, token->line, 0, di_func, NULL);
   LLVMSetCurrentDebugLocation2(ura.builder, loc);
}

void debug_exit_function(Token *token) {
   if (token->llvm.prev_block) LLVMPositionBuilderAtEnd(ura.builder, token->llvm.prev_block);
   if (!ura.debug_builder) return;
   ura.debug_scope = token->llvm.prev_scope;
   LLVMSetCurrentDebugLocation2(ura.builder, token->llvm.prev_loc);
}

void set_debug_location(Token *token) {
   if (!ura.debug_builder || !ura.debug_scope) return;
   MetadataRef loc = LLVMDIBuilderCreateDebugLocation(ura.context, token->line, 0, ura.debug_scope, NULL);
   LLVMSetCurrentDebugLocation2(ura.builder, loc);
}

Value get_or_declare(char *name, TypeRef fn_type) {
   Value fn = LLVMGetNamedFunction(ura.module, name);
   if (!fn) fn = LLVMAddFunction(ura.module, name, fn_type);
   return fn;
}

void guard(Token *op, Value is_bad, char *what) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block trap = LLVMAppendBasicBlockInContext(ura.context, fn, "trap");
   Block cont = LLVMAppendBasicBlockInContext(ura.context, fn, "cont");
   LLVMBuildCondBr(ura.builder, is_bad, trap, cont);

   LLVMPositionBuilderAtEnd(ura.builder, trap);
   char  *text = NULL;
   size_t tlen = 0;
   File   ms   = open_memstream(&text, &tlen);
   fprintf(ms, RED("runtime error: ") "%s\n", what);
   render_caret(ms, op);
   fclose(ms);
   if (ura.no_color) decolor(text);
   Value   msg  = LLVMBuildGlobalStringPtr(ura.builder, text, "trap_msg");
   TypeRef i8p  = LLVMPointerType(ura.i8, 0);

   TypeRef write_params[3] = { ura.i32, i8p, ura.i64 };
   TypeRef write_ty        = LLVMFunctionType(ura.i64, write_params, 3, 0);
   Value   write_fn        = get_or_declare("write", write_ty);
   Value   write_args[3]   = { LLVMConstInt(ura.i32, 2, 0), msg, LLVMConstInt(ura.i64, strlen(text), 0) };
   LLVMBuildCall2(ura.builder, write_ty, write_fn, write_args, 3, "");
   free(text);

   TypeRef exit_ty      = LLVMFunctionType(ura.vd, (TypeRef[]){ ura.i32 }, 1, 0);
   Value   exit_fn      = get_or_declare("exit", exit_ty);
   Value   exit_args[1] = { LLVMConstInt(ura.i32, 1, 0) };
   LLVMBuildCall2(ura.builder, exit_ty, exit_fn, exit_args, 1, "");

   LLVMBuildUnreachable(ura.builder);
   LLVMPositionBuilderAtEnd(ura.builder, cont);
}

void guard_nonzero(Token *op, Value divisor) {
   TypeRef ty     = LLVMTypeOf(divisor);
   bool    fp     = LLVMGetTypeKind(ty) == FloatType || LLVMGetTypeKind(ty) == DoubleType;
   Value   iszero = fp
      ? LLVMBuildFCmp(ura.builder, LLVMRealOEQ, divisor, LLVMConstReal(ty, 0.0), "iszero")
      : LLVMBuildICmp(ura.builder, LLVMIntEQ,   divisor, LLVMConstInt(ty, 0, 0), "iszero");
   guard(op, iszero, includes(op->type, MOD, MOD_ASSIGN, 0) ? "Modulo by zero" : "Division by zero");
}

void guard_nonnull(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = LLVMBuildICmp(ura.builder, LLVMIntEQ, ptr, null, "isnull");
   guard(op, isnull, "Call to a null function value");
}

void guard_bound(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = LLVMBuildICmp(ura.builder, LLVMIntEQ, ptr, null, "unbound");
   char *msg    = format("reference '%s' used before it was bound - assign '%s = ref <target>' first", op->name, op->name);
   guard(op, isnull, msg);
   free(msg);
}

void guard_index(Token *op, Value idx, Value slice) {
   Value len   = LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len");
   Value idx64 = LLVMBuildIntCast2(ura.builder, idx, ura.i64, 1, "idx");
   Value low   = LLVMBuildICmp(ura.builder, LLVMIntSLT, idx64, LLVMConstInt(ura.i64, 0, 0), "oob.low");
   Value high  = LLVMBuildICmp(ura.builder, LLVMIntSGE, idx64, len, "oob.high");
   guard(op, LLVMBuildOr(ura.builder, low, high, "oob"), "array index out of bounds");
}

void guard_slice(Token *op, Value start, Value end, Value len) {
   Value lo  = LLVMBuildICmp(ura.builder, LLVMIntSLT, start, LLVMConstInt(ura.i64, 0, 0), "s.lo");
   Value hi  = LLVMBuildICmp(ura.builder, LLVMIntSGT, end, len, "e.hi");
   Value ord = LLVMBuildICmp(ura.builder, LLVMIntSGT, start, end, "s.gt");
   Value bad = LLVMBuildOr(ura.builder, LLVMBuildOr(ura.builder, lo, hi, "b"), ord, "bad");
   guard(op, bad, "slice range out of bounds");
}

void analyze_binop(Node *node) {
   analyze(node->left);
   analyze(node->right);
}

void analyze_fdec(Node *node) {
   Token *token = node->token;
   enter_scope(node);
   for (int i = 0; i < token->Fn.params_count; i++)
      declare_variable(token->Fn.params[i]);
   for (int i = 0; i < node->children_count; i++)
      if (node->children[i]->token->type == FDEC)
         declare_function(node->children[i]);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   exit_scope();
}

void analyze_id(Node *node) {
   Token *token = node->token;
   if (token->is_dec) { declare_variable(token); return; }
   bool   captured = false;
   Token *decl     = find_variable(token->name, &captured);
   if (decl) {
      if (captured) {
         parse_error(token, "Cannot use '%s' from an enclosing function - pass it as a parameter", token->name);
         return;
      }
      token->ret_type = decl->ret_type;
      if (decl->ret_type == FN_TYPE) token->Fn = decl->Fn;
      if (decl->ret_type == ARRAY_TYPE) token->Array = decl->Array;
      return;
   }
   Node *fn = find_function(token->name);
   if (!fn) { parse_error(token, "Undeclared variable '%s'", token->name); return; }
   token->type             = FN_TYPE;
   token->ret_type         = FN_TYPE;
   token->Fcall.ptr        = fn;
   token->Fn.params        = fn->token->Fn.params;
   token->Fn.params_count  = fn->token->Fn.params_count;
   token->Fn.ret           = new_token(ID, 0);
   token->Fn.ret->ret_type = fn->token->ret_type;
}

void analyze_fcall(Node *node) {
   Token *token = node->token;
   Token *var = find_variable(token->name, NULL);
   if (var && var->ret_type == FN_TYPE) {
      token->Fcall.var = var;
      for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
      token->ret_type = var->Fn.ret->ret_type;
      return;
   }
   Node *fn = find_function(token->name);
   if (!fn) { parse_error(token, "Undeclared function '%s'", token->name); return; }
   token->Fcall.ptr = fn;
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   token->ret_type = fn->token->ret_type;
}

void type_check_fcall(Node *node) {
   Token *token = node->token;
   bool indirect = token->Fcall.var != NULL;
   if (!indirect && !token->Fcall.ptr) return;
   Token *fn = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   bool bad_count = fn->is_variadic ? node->children_count < fn->Fn.params_count
                                    : node->children_count != fn->Fn.params_count;
   if (bad_count) {
      parse_error(token, "Wrong number of arguments to '%s'", token->name);
      return;
   }
   for (int i = 0; i < fn->Fn.params_count; i++) {
      if (node->children[i]->token->ret_type &&
         node->children[i]->token->ret_type != fn->Fn.params[i]->ret_type) {
         parse_error(node->children[i]->token, "Argument %d type mismatch in call to '%s'", i + 1, token->name);
         return;
      }
      if (fn->Fn.params[i]->is_ref && node->children[i]->token->type != REF) {
         parse_error(node->children[i]->token, "Argument %d to '%s' must be passed by reference (ref x)", i + 1, token->name);
         return;
      }
      if (!fn->Fn.params[i]->is_ref && node->children[i]->token->type == REF) {
         parse_error(node->children[i]->token, "Argument %d to '%s' does not take a reference", i + 1, token->name);
         return;
      }
   }
   token->ret_type = indirect ? fn->Fn.ret->ret_type : fn->ret_type;
}

void type_check_fdec(Node *node) {
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
}

void type_check_binop(Node *node) {
   Token *token = node->token;
   type_check(node->left);
   type_check(node->right);
   Type lt = node->left->token->ret_type;
   Type rt = node->right->token->ret_type;
   if (node->left->token->is_dec && node->left->token->is_ref) {
      if (node->right->token->type != REF)
         parse_error(token, "A reference must be bound to a variable (ref x)");
      else if (lt && rt && lt != rt)
         parse_error(token, "Reference type mismatch: expected %s, got %s", type_name(lt), type_name(rt));
      token->ret_type = lt;
      return;
   }
   if (lt && rt && lt != rt) {
      parse_error(token, "Type mismatch between operands");
      return;
   }
   if (lt == FLOAT && includes(token->type, BITWISE_TYPE, 0)) {
      parse_error(token, "Bitwise and shift operators require integer operands");
      return;
   }
   if (includes(token->type, LOGIC_TYPE, 0)) {
      if (lt != BOOL || rt != BOOL) {
         parse_error(token, "Operators 'and'/'or' need bool operands");
         return;
      }
      token->ret_type = BOOL;
   } else
      token->ret_type = includes(token->type, COMPARISON_OPS, 0) ? BOOL : lt;
}

void emit_signature(Node *fn) {
   Token *token = fn->token;
   if (token->llvm.func_type) return;
   int      n      = token->Fn.params_count;
   TypeRef *params = NULL;
   if (n > 0) {
      params = allocate(n, sizeof(TypeRef));
      for (int i = 0; i < n; i++) {
         TypeRef pt = llvm_type_of(token->Fn.params[i]);
         params[i] = token->Fn.params[i]->is_ref ? LLVMPointerType(pt, 0) : pt;
      }
   }
   token->llvm.func_type = LLVMFunctionType(to_llvm_type(token->ret_type), params, n, token->is_variadic);
   token->llvm.elem      = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
   free(params);
}

Value address_of(Node *node) {
   Token *token = node->token;
   if (token->type == ACCESS) return access_ptr(node);
   if (token->is_dec) {
      TypeRef t = llvm_type_of(token);
      if (token->is_ref) t = LLVMPointerType(t, 0);
      return token->llvm.elem = LLVMBuildAlloca(ura.builder, t, token->name);
   }
   Token *decl = find_variable(token->name, NULL);
   if (decl->is_ref) {
      Value ptr = LLVMBuildLoad2(ura.builder, LLVMPointerType(to_llvm_type(decl->ret_type), 0), decl->llvm.elem, "ref");
      if (token->is_nullable) guard_bound(token, ptr);
      return ptr;
   }
   return decl->llvm.elem;
}

Value access_ptr(Node *node) {
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   code_gen(node->right);
   Value   idx   = node->right->token->llvm.elem;
   if (node->token->is_nullable) guard_index(node->token, idx, slice);
   Token  *arr   = node->left->token;
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   return LLVMBuildGEP2(ura.builder, elem, data, &idx, 1, "arr.at");
}

void code_gen_slice(Node *node) {
   Token *arr   = node->left->token;
   Node  *range = node->right;
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   code_gen(range->left);
   code_gen(range->right);
   Value   start = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i64, 1, "start");
   Value   end   = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i64, 1, "end");
   if (node->token->is_nullable)
      guard_slice(node->token, start, end, LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len"));
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   Value   ptr   = LLVMBuildGEP2(ura.builder, elem, data, &start, 1, "slice.data");
   Value   len   = LLVMBuildSub(ura.builder, end, start, "slice.len");
   node->token->llvm.elem = make_slice(arr->Array.sub_type, arr->Array.depth, ptr, len);
}

void code_gen_access(Node *node) {
   if (node->right->token->type == RANGE) { code_gen_slice(node); return; }
   Value   ptr  = access_ptr(node);
   TypeRef elem = node->token->ret_type == ARRAY_TYPE
                  ? array_type(node->token->Array.sub_type, node->token->Array.depth)
                  : to_llvm_type(node->token->ret_type);
   node->token->llvm.elem = LLVMBuildLoad2(ura.builder, elem, ptr, "idx");
}

void code_gen_array_lit(Node *node) {
   Token  *token = node->token;
   int     n     = node->children_count;
   int     depth = token->Array.depth;
   TypeRef elem  = depth > 1 ? array_type(token->Array.sub_type, depth - 1)
                             : to_llvm_type(token->Array.sub_type);
   Value   len   = LLVMConstInt(ura.i64, n, 0);
   Value   data  = LLVMBuildArrayAlloca(ura.builder, elem, len, "arr");
   for (int i = 0; i < n; i++) {
      code_gen(node->children[i]);
      Value idx = LLVMConstInt(ura.i64, i, 0);
      Value gep = LLVMBuildGEP2(ura.builder, elem, data, &idx, 1, "arr.init");
      LLVMBuildStore(ura.builder, node->children[i]->token->llvm.elem, gep);
   }
   TypeRef slice = array_type(token->Array.sub_type, depth);
   Value   agg   = LLVMGetUndef(slice);
   agg = LLVMBuildInsertValue(ura.builder, agg, data, 0, "arr.ptr");
   agg = LLVMBuildInsertValue(ura.builder, agg, len,  1, "arr.len");
   token->llvm.elem = agg;
}

void type_check_array_lit(Node *node) {
   if (node->children_count == 0) {
      parse_error(node->token, "Empty array literal has no element type");
      return;
   }
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   Token *first = node->children[0]->token;
   for (int i = 1; i < node->children_count; i++) {
      Token *e = node->children[i]->token;
      if (e->ret_type != first->ret_type ||
          (first->ret_type == ARRAY_TYPE &&
           (e->Array.sub_type != first->Array.sub_type || e->Array.depth != first->Array.depth)))
         parse_error(e, "Array elements must all be the same type");
   }
   node->token->ret_type = ARRAY_TYPE;
   if (first->ret_type == ARRAY_TYPE) {
      node->token->Array.sub_type = first->Array.sub_type;
      node->token->Array.depth    = first->Array.depth + 1;
   } else {
      node->token->Array.sub_type = first->ret_type;
      node->token->Array.depth    = 1;
   }
}

void type_check_access(Node *node) {
   type_check(node->left);
   type_check(node->right);
   Token *arr = node->left->token;
   if (arr->ret_type != ARRAY_TYPE) {
      parse_error(node->token, "Cannot index '%s', it is not an array", type_name(arr->ret_type));
      return;
   }
   if (node->right->token->type == RANGE) {
      node->token->ret_type       = ARRAY_TYPE;
      node->token->Array.sub_type = arr->Array.sub_type;
      node->token->Array.depth    = arr->Array.depth;
      return;
   }
   if (!includes(node->right->token->ret_type, NUMERIC_TYPES, 0))
      parse_error(node->token, "Array index must be an integer, got %s", type_name(node->right->token->ret_type));
   node->token->Array.sub_type = arr->Array.sub_type;
   node->token->Array.depth    = arr->Array.depth - 1;
   node->token->ret_type       = node->token->Array.depth > 0 ? ARRAY_TYPE : arr->Array.sub_type;
}

void type_check_array_ctor(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      type_check(node->children[i]);
      if (!includes(node->children[i]->token->ret_type, NUMERIC_TYPES, 0))
         parse_error(node->children[i]->token, "Array size must be an integer");
   }
}

Value make_slice(Type sub, int depth, Value data, Value len) {
   Value agg = LLVMGetUndef(array_type(sub, depth));
   agg = LLVMBuildInsertValue(ura.builder, agg, data, 0, "arr.ptr");
   agg = LLVMBuildInsertValue(ura.builder, agg, len,  1, "arr.len");
   return agg;
}

Value array_calloc(TypeRef elem, Value count, Value esz) {
   TypeRef i8p = LLVMPointerType(ura.i8, 0);
   TypeRef cty = LLVMFunctionType(i8p, (TypeRef[]){ ura.i64, ura.i64 }, 2, 0);
   Value   mem = LLVMBuildCall2(ura.builder, cty, get_or_declare("calloc", cty), (Value[]){ count, esz }, 2, "heap");
   return LLVMBuildBitCast(ura.builder, mem, LLVMPointerType(elem, 0), "arr");
}

Value build_array(Type sub, Value *dims, int depth, bool heap) {
   Value   n    = dims[0];
   TypeRef elem = depth == 1 ? to_llvm_type(sub) : array_type(sub, depth - 1);
   Value   esz  = LLVMConstInt(ura.i64, LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), elem), 0);
   if (depth == 1) {
      if (heap) return make_slice(sub, 1, array_calloc(elem, n, esz), n);
      Value data  = LLVMBuildArrayAlloca(ura.builder, elem, n, "arr");
      Value bytes = LLVMBuildMul(ura.builder, n, esz, "bytes");
      LLVMBuildMemSet(ura.builder, data, LLVMConstInt(ura.i8, 0, 0), bytes, 0);
      return make_slice(sub, 1, data, n);
   }
   TypeRef inner = elem;
   Value   data  = heap ? array_calloc(inner, n, esz)
                        : LLVMBuildArrayAlloca(ura.builder, inner, n, "arr");
   Value   fn    = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Value   slot  = LLVMBuildAlloca(ura.builder, ura.i64, "i");
   LLVMBuildStore(ura.builder, LLVMConstInt(ura.i64, 0, 0), slot);
   Block   cond  = LLVMAppendBasicBlockInContext(ura.context, fn, "arr.cond");
   Block   body  = LLVMAppendBasicBlockInContext(ura.context, fn, "arr.body");
   Block   end   = LLVMAppendBasicBlockInContext(ura.context, fn, "arr.end");
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   Value   i     = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
   LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntSLT, i, n, "more"), body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   Value   sub_arr = build_array(sub, dims + 1, depth - 1, heap);
   i = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
   Value   gep = LLVMBuildGEP2(ura.builder, inner, data, &i, 1, "arr.slot");
   LLVMBuildStore(ura.builder, sub_arr, gep);
   LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, i, LLVMConstInt(ura.i64, 1, 0), "next"), slot);
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
   return make_slice(sub, depth, data, n);
}

void code_gen_array_ctor(Node *node) {
   Token *token = node->token;
   int    depth = token->Array.depth;
   Value *dims  = allocate(depth, sizeof(Value));
   for (int i = 0; i < depth; i++) {
      code_gen(node->children[i]);
      dims[i] = LLVMBuildIntCast2(ura.builder, node->children[i]->token->llvm.elem, ura.i64, 1, "n");
   }
   token->llvm.elem = build_array(token->Array.sub_type, dims, depth, token->is_heap);
   free(dims);
}

void free_array(Value slice, Type sub, int depth) {
   Value data = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   if (depth > 1) {
      Value   len   = LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len");
      TypeRef inner = array_type(sub, depth - 1);
      Value   fn    = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
      Value   slot  = LLVMBuildAlloca(ura.builder, ura.i64, "i");
      LLVMBuildStore(ura.builder, LLVMConstInt(ura.i64, 0, 0), slot);
      Block   cond  = LLVMAppendBasicBlockInContext(ura.context, fn, "free.cond");
      Block   body  = LLVMAppendBasicBlockInContext(ura.context, fn, "free.body");
      Block   end   = LLVMAppendBasicBlockInContext(ura.context, fn, "free.end");
      LLVMBuildBr(ura.builder, cond);
      LLVMPositionBuilderAtEnd(ura.builder, cond);
      Value   i     = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
      LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntSLT, i, len, "more"), body, end);
      LLVMPositionBuilderAtEnd(ura.builder, body);
      Value   gep   = LLVMBuildGEP2(ura.builder, inner, data, &i, 1, "free.slot");
      free_array(LLVMBuildLoad2(ura.builder, inner, gep, "inner"), sub, depth - 1);
      i = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
      LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, i, LLVMConstInt(ura.i64, 1, 0), "next"), slot);
      LLVMBuildBr(ura.builder, cond);
      LLVMPositionBuilderAtEnd(ura.builder, end);
   }
   Value   ptr = LLVMBuildBitCast(ura.builder, data, LLVMPointerType(ura.i8, 0), "free.ptr");
   TypeRef fty = LLVMFunctionType(ura.vd, (TypeRef[]){ LLVMPointerType(ura.i8, 0) }, 1, 0);
   LLVMBuildCall2(ura.builder, fty, get_or_declare("free", fty), (Value[]){ ptr }, 1, "");
}

void code_gen_typeof(Node *node) {
   char *name = type_name(node->left->token->ret_type);
   node->token->llvm.elem = LLVMBuildGlobalStringPtr(ura.builder, name, "typeof");
}

void code_gen_sizeof(Node *node) {
   TypeRef t = llvm_type_of(node->left->token);
   unsigned long long sz = LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), t);
   node->token->llvm.elem = LLVMConstInt(ura.i32, sz, 0);
}

void code_gen_clean(Node *node) {
   Token  *arr   = node->left->token;
   Value   slot  = address_of(node->left);
   TypeRef sty   = array_type(arr->Array.sub_type, arr->Array.depth);
   Value   slice = LLVMBuildLoad2(ura.builder, sty, slot, "arr");
   free_array(slice, arr->Array.sub_type, arr->Array.depth);
   LLVMBuildStore(ura.builder, LLVMConstNull(sty), slot);
}

void code_gen_literal(Node *node) {
   Token *token = node->token;
   switch (token->type) {
      case INT:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0); break;
      case BOOL:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Bool.value, 0); break;
      case CHARS: token->llvm.elem = LLVMBuildGlobalStringPtr(ura.builder, token->Chars.value, "str"); break;
      case CHAR:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Char.value, 0); break;
      case FLOAT: token->llvm.elem = LLVMConstReal(to_llvm_type(token->ret_type), token->Float.value); break;
   }
}

void code_gen_fdec(Node *node) {
   Token *token = node->token;
   emit_signature(node);
   if (token->is_proto) return;
   debug_enter_function(token);
   enter_scope(node);
   for (int i = 0; i < token->Fn.params_count; i++) {
      Token  *param = token->Fn.params[i];
      TypeRef pt    = llvm_type_of(param);
      if (param->is_ref) pt = LLVMPointerType(pt, 0);
      param->llvm.elem = LLVMBuildAlloca(ura.builder, pt, param->name);
      LLVMBuildStore(ura.builder, LLVMGetParam(token->llvm.elem, i), param->llvm.elem);
   }
   for (int i = 0; i < node->children_count; i++) {
      set_debug_location(node->children[i]->token);
      code_gen(node->children[i]);
   }
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder))) {
      if (token->ret_type == VOID) LLVMBuildRetVoid(ura.builder);
      else LLVMBuildRet(ura.builder, default_value(token));
   }
   exit_scope();
   debug_exit_function(token);
}

void code_gen_id(Node *node) {
   Token *token = node->token;
   if (token->is_dec) {
      TypeRef t = llvm_type_of(token);
      if (token->is_ref) t = LLVMPointerType(t, 0);
      token->llvm.elem = LLVMBuildAlloca(ura.builder, t, token->name);
      Value init = token->is_ref ? LLVMConstNull(t) : default_value(token);
      LLVMBuildStore(ura.builder, init, token->llvm.elem);
      return;
   }
   Token  *decl = find_variable(token->name, NULL);
   TypeRef t    = llvm_type_of(decl);
   if (!decl->is_ref) {
      token->llvm.elem = LLVMBuildLoad2(ura.builder, t, decl->llvm.elem, token->name);
      return;
   }
   Value ptr = LLVMBuildLoad2(ura.builder, LLVMPointerType(t, 0), decl->llvm.elem, "ref");
   if (token->is_nullable) guard_bound(token, ptr);
   token->llvm.elem = LLVMBuildLoad2(ura.builder, t, ptr, token->name);
}

void code_gen_fcall(Node *node) {
   Token *token    = node->token;
   bool   indirect = token->Fcall.var != NULL;
   Token *fn       = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   if (!indirect) emit_signature(token->Fcall.ptr);
   int    n    = node->children_count;
   Value *args = NULL;
   if (n > 0) {
      args = allocate(n, sizeof(Value));
      for (int i = 0; i < n; i++) {
         code_gen(node->children[i]);
         args[i] = node->children[i]->token->llvm.elem;
      }
   }
   if (indirect) {
      TypeRef ptr_type = llvm_type_of(fn);
      Value   fn_ptr   = LLVMBuildLoad2(ura.builder, ptr_type, fn->llvm.elem, "fn");
      guard_nonnull(token, fn_ptr);
      char   *name     = fn->Fn.ret->ret_type == VOID ? "" : "call";
      token->llvm.elem = LLVMBuildCall2(ura.builder, LLVMGetElementType(ptr_type), fn_ptr, args, n, name);
   } else {
      char *name       = fn->ret_type == VOID ? "" : "call";
      token->llvm.elem = LLVMBuildCall2(ura.builder, fn->llvm.func_type, fn->llvm.elem, args, n, name);
   }
   free(args);
}

void code_gen_body(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      code_gen(node->children[i]);
      if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder))) break;
   }
}

void code_gen_loop(Node *node) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "loop.body");
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "loop.end");
   node->token->llvm.start = body;
   node->token->llvm.end   = end;
   LLVMBuildBr(ura.builder, body);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, body);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void analyze_for(Node *node) {
   enter_scope(node);
   analyze(node->right);
   Token *var = node->left->token;
   Token *it  = node->right->token;
   if (it->type == RANGE)
      var->ret_type = INT;
   else if (it->ret_type == ARRAY_TYPE) {
      var->Array       = it->Array;
      var->Array.depth = it->Array.depth - 1;
      var->ret_type    = it->Array.depth > 1 ? ARRAY_TYPE : it->Array.sub_type;
   }
   declare_variable(var);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   exit_scope();
}

void type_check_for(Node *node) {
   type_check(node->right);
   Token *iter = node->right->token;
   if (iter->type != RANGE && iter->ret_type != ARRAY_TYPE)
      parse_error(node->token, "'for %s in ...' expects a range (a..b) or an array", node->left->token->name);
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
}

void code_gen_for_array(Node *node) {
   Token  *var  = node->left->token;
   Token  *arr  = node->right->token;
   code_gen(node->right);
   Value   slice = arr->llvm.elem;
   Value   data  = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   Value   len   = LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len");
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   Value   fn    = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Value   idx   = LLVMBuildAlloca(ura.builder, ura.i64, "idx");
   LLVMBuildStore(ura.builder, LLVMConstInt(ura.i64, 0, 0), idx);
   Value   xslot = LLVMBuildAlloca(ura.builder, elem, var->name);
   var->llvm.elem = xslot;
   Block   cond  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.cond");
   Block   body  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.body");
   Block   inc   = LLVMAppendBasicBlockInContext(ura.context, fn, "for.inc");
   Block   end   = LLVMAppendBasicBlockInContext(ura.context, fn, "for.end");
   node->token->llvm.start = inc;
   node->token->llvm.end   = end;
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   Value   i = LLVMBuildLoad2(ura.builder, ura.i64, idx, "i");
   LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntSLT, i, len, "more"), body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   Value   gep = LLVMBuildGEP2(ura.builder, elem, data, &i, 1, "elem");
   LLVMBuildStore(ura.builder, LLVMBuildLoad2(ura.builder, elem, gep, "x"), xslot);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, inc);
   LLVMPositionBuilderAtEnd(ura.builder, inc);
   Value   iv = LLVMBuildLoad2(ura.builder, ura.i64, idx, "i");
   LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, iv, LLVMConstInt(ura.i64, 1, 0), "next"), idx);
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_for(Node *node) {
   if (node->right->token->type != RANGE) { code_gen_for_array(node); return; }
   Token *var   = node->left->token;
   Node  *range = node->right;
   code_gen(range->left);
   code_gen(range->right);
   Value a    = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i32, 1, "a");
   Value b    = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i32, 1, "b");
   Value asc  = LLVMBuildICmp(ura.builder, LLVMIntSLT, a, b, "asc");
   Value step = LLVMBuildSelect(ura.builder, asc, LLVMConstInt(ura.i32, 1, 0), LLVMConstInt(ura.i32, -1, 1), "step");
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Value slot = LLVMBuildAlloca(ura.builder, ura.i32, var->name);
   LLVMBuildStore(ura.builder, a, slot);
   var->llvm.elem = slot;
   Block cond = LLVMAppendBasicBlockInContext(ura.context, fn, "for.cond");
   Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "for.body");
   Block inc  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.inc");
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.end");
   node->token->llvm.start = inc;
   node->token->llvm.end   = end;
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   Value i = LLVMBuildLoad2(ura.builder, ura.i32, slot, var->name);
   LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntNE, i, b, "more"), body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, inc);
   LLVMPositionBuilderAtEnd(ura.builder, inc);
   Value iv = LLVMBuildLoad2(ura.builder, ura.i32, slot, var->name);
   LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, iv, step, "next"), slot);
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_while(Node *node) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block cond = LLVMAppendBasicBlockInContext(ura.context, fn, "while.cond");
   Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "while.body");
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "while.end");
   node->token->llvm.start = cond;   
   node->token->llvm.end   = end;    
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   code_gen(node->left);
   LLVMBuildCondBr(ura.builder, node->left->token->llvm.elem, body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_match(Node *node) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "match.end");
   node->token->llvm.end = end;                       // break target
   code_gen(node->left);                              
   Value subject = node->left->token->llvm.elem;
   bool  fp      = node->left->token->ret_type == FLOAT;
   enter_scope(node);
   for (int i = 0; i < node->children_count; i++) {
      Node *branch = node->children[i];
      if (branch->token->type == DEFAULT) {
         enter_scope(branch); code_gen_body(branch); exit_scope();
         if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
            LLVMBuildBr(ura.builder, end);
         break;
      }
      Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "case.body");
      Block next = i + 1 < node->children_count ? LLVMAppendBasicBlockInContext(ura.context, fn, "case.next") : end;
      Value cond = NULL;
      for (int j = 0; j < branch->left->children_count; j++) {
         code_gen(branch->left->children[j]);
         Value val = branch->left->children[j]->token->llvm.elem;
         Value eq  = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOEQ, subject, val, "feq")
                        : LLVMBuildICmp(ura.builder, LLVMIntEQ, subject, val, "eq");
         cond = cond ? LLVMBuildOr(ura.builder, cond, eq, "case.or") : eq;
      }
      LLVMBuildCondBr(ura.builder, cond, body, next);
      LLVMPositionBuilderAtEnd(ura.builder, body);
      enter_scope(branch); code_gen_body(branch); exit_scope();
      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
         LLVMBuildBr(ura.builder, end);
      LLVMPositionBuilderAtEnd(ura.builder, next);
   }
   exit_scope();
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_if(Node *node) {
   Value fn  = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block end = LLVMAppendBasicBlockInContext(ura.context, fn, "endif");
   for (Node *cur = node; cur; cur = cur->right) {
      if (cur->token->type == ELSE) {
         enter_scope(cur);
         code_gen_body(cur);
         exit_scope();
         if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
            LLVMBuildBr(ura.builder, end);
         break;
      }
		// elif
      Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "then");
      Block next = cur->right ? LLVMAppendBasicBlockInContext(ura.context, fn, "next") : end;
      code_gen(cur->left);
      LLVMBuildCondBr(ura.builder, cur->left->token->llvm.elem, body, next);
      LLVMPositionBuilderAtEnd(ura.builder, body);
      enter_scope(cur);
      code_gen_body(cur);
      exit_scope();
      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
         LLVMBuildBr(ura.builder, end);
      LLVMPositionBuilderAtEnd(ura.builder, next);
   }
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_assign(Node *node) {
   Token *token = node->token;
   Value dest = address_of(node->left);
   code_gen(node->right);
   LLVMBuildStore(ura.builder, node->right->token->llvm.elem, dest);
   token->llvm.elem = node->right->token->llvm.elem;
}

void code_gen_binop(Node *node) {
   Token *token = node->token;
   code_gen(node->left);
   code_gen(node->right);
   Value left  = node->left->token->llvm.elem;
   Value right = node->right->token->llvm.elem;
   Value res   = NULL;
   bool  fp    = node->left->token->ret_type == FLOAT;
   if (includes(token->type, DIV, MOD, 0)) guard_nonzero(token, right);
   switch (token->type) {
      case ADD: res = fp ? LLVMBuildFAdd(ura.builder, left, right, "fadd") : LLVMBuildAdd(ura.builder, left, right, "add"); break;
      case SUB: res = fp ? LLVMBuildFSub(ura.builder, left, right, "fsub") : LLVMBuildSub(ura.builder, left, right, "sub"); break;
      case MUL: res = fp ? LLVMBuildFMul(ura.builder, left, right, "fmul") : LLVMBuildMul(ura.builder, left, right, "mul"); break;
      case DIV: res = fp ? LLVMBuildFDiv(ura.builder, left, right, "fdiv") : LLVMBuildSDiv(ura.builder, left, right, "div"); break;
      case MOD: res = fp ? LLVMBuildFRem(ura.builder, left, right, "frem") : LLVMBuildSRem(ura.builder, left, right, "mod"); break;
      case EQUAL:       res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOEQ, left, right, "feq") : LLVMBuildICmp(ura.builder, LLVMIntEQ,  left, right, "eq"); break;
      case NOT_EQUAL:   res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealUNE, left, right, "fne") : LLVMBuildICmp(ura.builder, LLVMIntNE,  left, right, "ne"); break;
      case LESS:        res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOLT, left, right, "flt") : LLVMBuildICmp(ura.builder, LLVMIntSLT, left, right, "lt"); break;
      case GREAT:       res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOGT, left, right, "fgt") : LLVMBuildICmp(ura.builder, LLVMIntSGT, left, right, "gt"); break;
      case LESS_EQUAL:  res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOLE, left, right, "fle") : LLVMBuildICmp(ura.builder, LLVMIntSLE, left, right, "le"); break;
      case GREAT_EQUAL: res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOGE, left, right, "fge") : LLVMBuildICmp(ura.builder, LLVMIntSGE, left, right, "ge"); break;
      case AND: res = LLVMBuildAnd(ura.builder, left, right, "and"); break;
      case OR:  res = LLVMBuildOr(ura.builder,  left, right, "or"); break;
      case BAND:   res = LLVMBuildAnd(ura.builder, left, right, "band"); break;
      case BOR:    res = LLVMBuildOr(ura.builder,  left, right, "bor"); break;
      case BXOR:   res = LLVMBuildXor(ura.builder, left, right, "bxor"); break;
      case LSHIFT: res = LLVMBuildShl(ura.builder, left, right, "shl"); break;
      case RSHIFT: res = LLVMBuildAShr(ura.builder, left, right, "shr"); break;
      default: break;
   }
   token->llvm.elem = res;
}

void code_gen_compound(Node *node) {
   Value   dest = address_of(node->left);
   code_gen(node->right);
   Value   r   = node->right->token->llvm.elem;
   TypeRef t   = to_llvm_type(node->left->token->ret_type);
   Value   cur = LLVMBuildLoad2(ura.builder, t, dest, "cur");
   Value   res;
   bool    fp  = node->left->token->ret_type == FLOAT;
   switch (node->token->type) {
      case ADD_ASSIGN: res = fp ? LLVMBuildFAdd(ura.builder, cur, r, "fadd") : LLVMBuildAdd(ura.builder, cur, r, "add"); break;
      case SUB_ASSIGN: res = fp ? LLVMBuildFSub(ura.builder, cur, r, "fsub") : LLVMBuildSub(ura.builder, cur, r, "sub"); break;
      case MUL_ASSIGN: res = fp ? LLVMBuildFMul(ura.builder, cur, r, "fmul") : LLVMBuildMul(ura.builder, cur, r, "mul"); break;
      case DIV_ASSIGN: guard_nonzero(node->token, r); res = fp ? LLVMBuildFDiv(ura.builder, cur, r, "fdiv") : LLVMBuildSDiv(ura.builder, cur, r, "div"); break;
      case MOD_ASSIGN: guard_nonzero(node->token, r); res = fp ? LLVMBuildFRem(ura.builder, cur, r, "frem") : LLVMBuildSRem(ura.builder, cur, r, "mod"); break;
      default: res = r; break;
   }
   LLVMBuildStore(ura.builder, res, dest);
   node->token->llvm.elem = res;
}

void code_gen_output(Node *node) {
   char  *fmt   = allocate(node->children_count * 4 + 8, 1);
   int    fc    = 0;
   Value *args  = allocate(node->children_count + 1, sizeof(Value));
   int    nargs = 0;

   for (int i = 0; i < node->children_count; i++) {
      Node *arg = node->children[i];
      code_gen(arg);
      Value v = arg->token->llvm.elem;
      switch (arg->token->ret_type) {
         case INT: fmt[fc++] = '%'; fmt[fc++] = 'd'; args[nargs++] = v; break;
         case SHORT: fmt[fc++] = '%'; fmt[fc++] = 'd'; args[nargs++] = LLVMBuildSExt(ura.builder, v, ura.i32, "s2i"); break;
         case LONG: fmt[fc++] = '%'; fmt[fc++] = 'l'; fmt[fc++] = 'l'; fmt[fc++] = 'd'; args[nargs++] = v; break;
         case CHAR: fmt[fc++] = '%'; fmt[fc++] = 'c'; args[nargs++] = LLVMBuildSExt(ura.builder, v, ura.i32, "c2i"); break;
         case CHARS: fmt[fc++] = '%'; fmt[fc++] = 's'; args[nargs++] = v; break;
         case BOOL: {
            Value ts = LLVMBuildGlobalStringPtr(ura.builder, "True", "true_str");
            Value fs = LLVMBuildGlobalStringPtr(ura.builder, "False", "false_str");
            fmt[fc++] = '%'; fmt[fc++] = 's';
            args[nargs++] = LLVMBuildSelect(ura.builder, v, ts, fs, "bool_str");
            break;
         }
         case FLOAT:
            fmt[fc++] = '%'; fmt[fc++] = 'f';
            args[nargs++] = LLVMBuildFPExt(ura.builder, v, LLVMDoubleTypeInContext(ura.context), "f2d");
            break;
         default: fmt[fc++] = '?'; break;
      }
   }

   TypeRef i8p       = LLVMPointerType(ura.i8, 0);
   TypeRef printf_ty = LLVMFunctionType(ura.i32, (TypeRef[]){ i8p }, 1, 1);
   Value   printf_fn = get_or_declare("printf", printf_ty);
   Value  *call_args = allocate(nargs + 1, sizeof(Value));
   call_args[0]      = LLVMBuildGlobalStringPtr(ura.builder, fmt, "fmt");
   memcpy(call_args + 1, args, nargs * sizeof(Value));
   node->token->llvm.elem = LLVMBuildCall2(ura.builder, printf_ty, printf_fn, call_args, nargs + 1, "");
   free(fmt);
   free(args);
   free(call_args);
}

void free_token(Token *token) {
   if (!token) return;
   free(token->name);
   free(token->Chars.value);
   free(token->llvm.dims);
   free(token);
}

void free_node(Node *node) {
   if (!node) return;
   if (!includes(node->token->type, BREAK, CONTINUE, 0))
      free_node(node->left);
   free_node(node->right);
   for (int i = 0; i < node->children_count; i++)
      free_node(node->children[i]);
   free(node->children);
   free(node->variables);
   free(node->functions);
   free(node->structs);
   free(node->modules);
   free(node);
}

void free_memory() {
   free_node(ura.head);
   for (int i = 0; i < ura.tokens_count; i++)
      free_token(ura.tokens[i]);
   free(ura.tokens);
   for (int i = 0; i < ura.sources_count; i++) {
      free(ura.sources[i]->content);
      free(ura.sources[i]);
   }
   free(ura.sources);
   free(ura.scopes);
   if (ura.context) {
      LLVMDisposeBuilder(ura.builder);
      LLVMDisposeModule(ura.module);
      LLVMContextDispose(ura.context);
   }
}
