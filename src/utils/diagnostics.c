#include "header.h"

char *to_string(Type type) {
	char *res[END + 1] = {
	    [ID] = "ID",              [CHAR] = "CHAR",             [CHARS] = "CHARS",
	    [PTR] = "PTR",
	    [I8] = "I8",              [I16] = "I16",               [I32] = "I32",
	    [I64] = "I64",            [U8] = "U8",                 [U16] = "U16",
	    [U32] = "U32",            [U64] = "U64",               [F32] = "F32",
	    [F64] = "F64",            [VOID] = "VOID",             [BOOL] = "BOOL",
	    [FDEC] = "FDEC",          [END] = "END",               [LPAR] = "LPAR",
	    [FCALL] = "CALL",
	    [IF] = "IF",              [RPAR] = "RPAR",             [ELIF] = "ELIF",
	    [FOR] = "FOR",            [ELSE] = "ELSE",             [WHILE] = "WHILE",
	    [BY] = "BY",              [LOOP] = "LOOP",
	    [IN] = "IN",              [BREAK] = "BRK",             [CONTINUE] = "CONT",
	    [CASE] = "CASE",          [MATCH] = "MATCH",           [DEFAULT] = "DEFAULT",
	    [BAND] = "BAND",          [RETURN] = "RET",            [BOR] = "BOR",
	    [BXOR] = "BXOR",          [BNOT] = "BNOT",
	    [ADD] = "ADD",            [LSHIFT] = "LSHIFT",         [RSHIFT] = "RSHIFT",
	    [SUB] = "SUB",            [MUL] = "MUL",               [DIV] = "DIV",
	    [ASSIGN] = "ASSIGN",      [ADD_ASSIGN] = "ADD_ASS",    [SUB_ASSIGN] = "SUB_ASS",
	    [MUL_ASSIGN] = "MUL_ASS", [DIV_ASSIGN] = "DIV_ASS",    [MOD_ASSIGN] = "MOD_ASS",
	    [BAND_ASSIGN] = "AND_ASS", [BOR_ASSIGN] = "OR_ASS",   [BXOR_ASSIGN] = "XOR_ASS",
	    [LSHIFT_ASSIGN] = "SHL_ASS", [RSHIFT_ASSIGN] = "SHR_ASS",
	    [MOD] = "MOD",            [COMA] = "COMA",             [ACCESS] = "ACC",
	    [REF] = "REF",            [EQUAL] = "EQ",              [NOT_EQUAL] = "NEQ",
	    [LESS] = "LT",            [GREAT] = "GT",              [LESS_EQUAL] = "LE",
	    [NOT] = "NOT",            [AND] = "AND",               [GREAT_EQUAL] = "GE",
	    [OR] = "OR",              [DOTS] = "DOTS",             [PROTO] = "PROT",
	    [VARIADIC] = "VAR",       [TYPEOF] = "TYPEOF",         [SIZEOF] = "SIZEOF",
	    [ARGS] = "ARGS",          [OUTPUT] = "OUTPUT",         [ERRPUT] = "ERRPUT",
	    [CHILDREN] = "CHILDREN",
	    [AS] = "AS",              [NULL_LIT] = "NULL_LIT",    [FALLBACK] = "FALLBACK",     [ARRAY_LIT] = "ARRAY_LIT",
	    [ARRAY_TYPE] = "ARRAY_TYPE",
	    [OPTIONAL] = "OPTIONAL",  [STRUCT_DEF] = "STRUCT_DEF", [STRUCT_CALL] = "STRUCT_CALL",
	    [TUPLE] = "TUPLE",        [ENUM_DEF] = "ENUM_DEF",     [ENUM_CALL] = "ENUM_CALL",
	    [LBRA] = "LBRA",          [RBRA] = "RBRA",             [TUPLE_UNPACK] = "TUPLE_UNPACK",
	    [LIST] = "LIST",          [ARRAY] = "ARRAY",           [LIST_TYPE] = "LIST_TYPE",
	    [DOT] = "DOT",            [RANGE] = "RANGE",           [FN_TYPE] = "FN_TYPE",
	    [MODULE] = "MODULE",      [OPERATOR] = "OPERATOR_KW",  [SYNTAX_ERROR] = "SYNTAX_ERROR",
	    [PUB] = "PUB",            [CLEAN] = "CLEAN",           [DOUBLE_DOTS] = "DOUBLE_DOTS",
	    [NEW] = "NEW",
	    //[TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW", [USE] = "USE",
	};

	TODO(!res[type], "handle this case %d\n", type);
	return res[type];
}

char *type_name(Type type) {
	switch (type) {
	case I8:         return "i8";
	case I16:        return "i16";
	case I32:        return "i32";
	case I64:        return "i64";
	case U8:         return "u8";
	case U16:        return "u16";
	case U32:        return "u32";
	case U64:        return "u64";
	case CHAR:       return "char";
	case PTR:        return "pointer";
	case BOOL:       return "bool";
	case F32:        return "f32";
	case F64:        return "f64";
	case VOID:       return "void";
	case ARRAY_TYPE: return "array";
	case FN_TYPE:    return "fn";
	default:         return type ? to_string(type) : "unknown";
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
			case 'x': {
				unsigned int value = va_arg(args, unsigned int);
				res += (prec >= 0) ? fprintf(out, "%.*x", prec, value)
				                   : fprintf(out, "%x", value);
				break;
			}
			case 'X': {
				unsigned int value = va_arg(args, unsigned int);
				res += (prec >= 0) ? fprintf(out, "%.*X", prec, value)
				                   : fprintf(out, "%X", value);
				break;
			}
			case 'd': {
				int value = va_arg(args, int);
				res += (prec >= 0) ? fprintf(out, "%.*d", prec, value)
				                   : fprintf(out, "%d", value);
				break;
			}
			case 'f': {
				double value = va_arg(args, double);
				res += (prec >= 0) ? fprintf(out, "%.*f", prec, value)
				                   : fprintf(out, "%f", value);
				break;
			}
			case '%': res += fprintf(out, "%%"); break;
			case 't': {
				Type type = (Type)va_arg(args, Type);
				res += fprintf(out, "%s", to_string(type));
				break;
			}
			case 'k': {
				Token *token = va_arg(args, Token *);
				if (!token) {
					fprintf(out, "(null)");
					break;
				}

				fprintf(out, "[%s] ", to_string(token->type));

				switch (token->type) {
				case VOID: case CHARS: case CHAR: case I32: case BOOL: case F32:
				case I64: {
					if (token->name) {
						fprintf(out, "%s ", token->name);
						break;
					}
					if (token->type == VOID) break;
					switch (token->type) {
					case I32:   fprintf(out, "[%lld] ", (long long)token->Int.value);
						break;
					case I64:  fprintf(out, "[%lld] ", token->Long.value); break;
					case BOOL: {
						char *text = token->Bool.value ? "True" : "False";
						fprintf(out, "[%s] ", text);
						break;
					}
					case F32: fprintf(out, "[%f] ", token->Float.value); break;
					case CHAR:  {
						fprintf(out, "[");
						fprint_escaped(out, token->Char.value);
						fprintf(out, "] ");
						break;
					}
					case CHARS: {
						fprintf(out, "[\"");
						char *str = token->Chars.value;
						if (str)
							for (int j = 0; str[j]; j++)
								fprint_escaped(out, str[j]);
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

void fprint_escaped(File out, char c) {
	switch (c) {
	case '\n': fputs("\\n", out); break;
	case '\t': fputs("\\t", out); break;
	case '\r': fputs("\\r", out); break;
	case '\\': fputs("\\\\", out); break;
	case '"':  fputs("\\\"", out); break;
	default:   fputc(c, out);
	}
}

void print_escaped(char c) {
	fprint_escaped(stdout, c);
}

static char *spelling[END + 1] = {
		[I8] = "i8",         [I16] = "i16",         [I32] = "i32",
		[I64] = "i64",       [U8] = "u8",           [U16] = "u16",
		[U32] = "u32",       [U64] = "u64",         [F32] = "f32",
		[F64] = "f64",       [BOOL] = "bool",       [CHAR] = "char",
		[PTR] = "pointer",   [VOID] = "void",       [ADD] = "+",
		[SUB] = "-",         [MUL] = "*",           [DIV] = "/",
		[MOD] = "%",         [EQUAL] = "==",        [NOT_EQUAL] = "!=",
		[LESS] = "<",        [GREAT] = ">",         [LESS_EQUAL] = "<=",
		[OR] = "or",         [AND] = "and",         [GREAT_EQUAL] = ">=",
		[BOR] = "|",         [BAND] = "&",          [NOT] = "not",
		[BXOR] = "^",        [BNOT] = "~",          [LSHIFT] = "<<",
		[ASSIGN] = "=",      [RSHIFT] = ">>",       [ADD_ASSIGN] = "+=",
		[SUB_ASSIGN] = "-=", [MUL_ASSIGN] = "*=",   [DIV_ASSIGN] = "/=",
		[MOD_ASSIGN] = "%=", [BAND_ASSIGN] = "&=",  [BOR_ASSIGN] = "|=",
		[BXOR_ASSIGN] = "^=", [LSHIFT_ASSIGN] = "<<=", [RSHIFT_ASSIGN] = ">>=",
		[IF] = "if",         [ELIF] = "elif",
		[ELSE] = "else",     [WHILE] = "while",     [MATCH] = "match",
		[CASE] = "case",     [BREAK] = "break",     [DEFAULT] = "default",
		[RETURN] = "return", [OUTPUT] = "output",   [ERRPUT] = "errput",
		[CONTINUE] = "continue",
		[REF] = "ref",       [AS] = "cast",         [FOR] = "for",
		[LOOP] = "loop",     [RANGE] = "range",     [ACCESS] = "index",
		[ARRAY] = "array",   [ARRAY_LIT] = "array", [ARRAY_TYPE] = "array",
		[NEW] = "new",       [TYPEOF] = "typeof",   [SIZEOF] = "sizeof",
		[CLEAN] = "clean",     [TRY] = "try",         [CATCH] = "catch",
		[THROW] = "throw",     [MODULE] = "mod",
};

char *spell(Type type) {
	return spelling[type] ? spelling[type] : to_string(type);
}

void print_node_label(Node *node) {
	Token *token = node->token;
	switch (token->type) {
	case I32:   printf("int %lld", (long long)token->Int.value); return;
	case I64:  printf("long %lld", token->Long.value); return;
	case I16: printf("short %d", token->Short.value); return;
	case F32: printf("float %g", token->Float.value); return;
	case BOOL:  printf("bool %s", token->Bool.value ? "True" : "False"); return;
	case CHAR: {
		printf("char '");
		print_escaped(token->Char.value);
		printf("'");
		return;
	}
	case CHARS: {
		printf("char[] \"");
		char *value = token->Chars.value;
		for (int i = 0; value[i]; i++)
			print_escaped(value[i]);
		printf("\"");
		return;
	}
	case ID:    printf("%s", token->name); break;
	case FCALL: printf("call %s", token->name); break;
	case FDEC: {
		printf("%sfn %s(", token->is_proto ? "proto " : "", token->name);
		for (int i = 0; i < token->Fn.params_count; i++) {
			Token *param = token->Fn.params[i];
			printf("%s%s", i ? ", " : "", param->name);
			if (param->ret_type)
				printf(" : %s", spell(param->ret_type));
		}
		if (token->is_variadic)
			printf("%s...", token->Fn.params_count ? ", " : "");
		printf(")");
		break;
	}
	case STRUCT_DEF: printf("struct %s", token->name); return;
	case MODULE:     printf("mod %s", token->name); return;
	case NEW:        printf("new %s", token->Struct.name); return;
	case DOT:   printf(".%s", token->name); break;
	default:    printf("%s", spell(token->type));
	}
	if (token->ret_type == ARRAY_TYPE && token->Array.sub_type) {
		char *t = strdup(type_name(token->Array.sub_type));
		for (int i = 0; i < token->Array.depth; i++) {
			char *n = format("%s[]", t);
			free(t);
			t = n;
		}
		printf(" : %s", t);
		free(t);
	} else if (token->ret_type)
		printf(" : %s", spell(token->ret_type));
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
	case TRY: {
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
	case CATCH: {
		if (node->left) {
			edge_role[count] = "binds";
			edge_node[count++] = node->left;
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
	for (int i = 0; i < count; i++) {
		Node *node = edge_node[i];
		bool is_last = i == count - 1;
		char *role = edge_role[i];
		printf("%s%s", prefix, is_last ? "└─ " : "├─ ");
		if (role) printf("%s ", role);
		print_node_label(node);
		putchar('\n');
		char *child_prefix = format("%s%s", prefix, is_last ? "   " : "│  ");
		print_children(node, child_prefix);
		free(child_prefix);
	}
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

void decolor(char *text) {
	int write = 0;
	for (int read = 0; text[read]; ) {
		bool is_escape = text[read] == '\033' && text[read + 1] == '[';
		if (!is_escape) {
			text[write++] = text[read++];
			continue;
		}
		read += 2;
		while (text[read] && text[read] != 'm') read++;
		if (text[read] == 'm') read++;
	}
	text[write] = '\0';
}

void render_caret(File out, Token *token, const char *color) {
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

	char *name = token->source->filename;
	char *lib  = strstr(name, "/ura-lib/");
	if (lib) name = lib + 1;
	fprintf(out, "%*s \033[2m%s:%d:%d\033[0m\n", gutter, "", name, line_no, col);
	fprintf(out, "%*s " BLUE("|") "\n", gutter, "");
	fprintf(out, BLUE("%*d |") " %.*s\n", gutter, line_no, line_end - line_start, content + line_start);
	fprintf(out, "%*s " BLUE("|") " ", gutter, "");
	for (int i = 0; i < col - 1; i++)
		fputc(content[line_start + i] == '\t' ? '\t' : ' ', out);
	fprintf(out, "%s", color);
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
	render_caret(ms, token, CARET_ERR);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void parse_warn(Token *token, const char *fmt, ...) {
	if (token && token->no_warn) return;
	char  *buf = NULL;
	size_t len = 0;
	File   ms  = open_memstream(&buf, &len);
	fprintf(ms, YELLOW("warning: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(ms, fmt, ap);
	va_end(ap);
	fputc('\n', ms);
	render_caret(ms, token, CARET_WARN);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void parse_note(Token *token, const char *fmt, ...) {
	char  *buf = NULL;
	size_t len = 0;
	File   ms  = open_memstream(&buf, &len);
	fprintf(ms, BLUE("note: "));
	va_list ap;
	va_start(ap, fmt);
	vfprintf(ms, fmt, ap);
	va_end(ap);
	fputc('\n', ms);
	render_caret(ms, token, CARET_WARN);
	fclose(ms);

	if (ura.no_color) decolor(buf);
	fputs(buf, stderr);
	free(buf);
}

void tokenize_error(int line, int s, int e, const char *fmt, ...) {
	Source *src = ura.current;
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
