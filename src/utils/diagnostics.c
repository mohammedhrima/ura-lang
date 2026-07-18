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

char *array_type_label(Token *token) {
	char *s = strdup(type_name(token->Array.sub_type));
	for (int i = 0; i < token->Array.depth; i++) {
		char *n = format("%s[]", s);
		free(s);
		s = n;
	}
	return s;
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
		[FOR] = "for", [LOOP] = "loop", [RANGE] = "range", [ACCESS] = "index",
		[ARRAY] = "array", [ARRAY_LIT] = "array", [ARRAY_TYPE] = "array",
		[TYPEOF] = "typeof", [SIZEOF] = "sizeof", [NEW] = "new", [CLEAN] = "clean",
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
	case STRUCT_DEF: printf("struct %s", token->name); return;
	case DOT:   printf(".%s", token->name); break;
	default:    printf("%s", spelling[token->type] ? spelling[token->type] : to_string(token->type));
	}
	if (token->ret_type == ARRAY_TYPE && token->Array.sub_type) {
		char *t = array_type_label(token);
		printf(" : %s", t);
		free(t);
	} else if (token->ret_type)
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
