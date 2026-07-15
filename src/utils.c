#include "header.h"

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

	TODO(!res[type], "handle this case %d\n", type);
	return res[type];
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

void pnode(Node *node, char *indent) {
	if (!node || !node->token || !ura.enable_debug) return;
	Node **subs     = NULL;
	int    subs_count    = 0;
	int    subs_size = 0;

#define push(n)                                                                                    \
	do {                                                                                            \
		resize_array(subs, Node *, subs_size, subs_count);                                           \
		subs[subs_count++] = (n);                                                                    \
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
	fprintf(stderr, RED("ura error: %s %s:%d "), funcname, filename, line);
	va_list ap;
	va_start(ap, fmt);
	_vprint(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	return cond;
}

void parse_error(Token *token, const char *fmt, ...) {
	ura.error_count++;
	if (ura.error_count > ura.max_errors) {
		fprintf(stderr, RED("error: ") "too many errors, stopping\n");
		return;
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
	// TODO: understand this logic
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
		parse_error(NULL, "cannot find file '%s'", file_name);
		return;
	}
	File file = fopen(file_name, "r");
	if (!file) {
		parse_error(NULL, "cannot open file '%s'", file_name);
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

	resize_array(ura.sources, Source *, ura.sources_size, ura.sources_count);
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

bool lex_multi_coment(char *content, int *i, int *line, int indent, int default_indent)
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
		tokenize_error((*line), s, s + 2, "unterminated block comment, expected '*/'");
		return true;
	}
	(*i) += 2;
	return true;
}

bool lex_coment(char *content, int *i, int line, int indent, int default_indent)
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
		tokenize_error(line, s, s + 1, "unterminated string literal, expected '\"'");
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
		tokenize_error(line, s, s + 1, "unterminated character literal, expected \"'\"");
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
		tokenize_error(line, (*i), (*i) + 1, "expected '\"' after `use`");
		return true;
	}
	(*i)++;
	{
		int start = (*i);
		while (content[(*i)] && content[(*i)] != '\"' && content[(*i)] != '\n')
			(*i)++;
		if (content[(*i)] != '\"') {
			tokenize_error(line, start - 1, start, "unterminated `use` path, expected closing '\"'");
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
		tokenize_error(line, (*i), (*i) + 1, "expected '\"' after `link`");
		return true;
	}
	(*i)++;
	int link_s = (*i);
	while (content[(*i)] && content[(*i)] != '\"' && content[(*i)] != '\n')
		(*i)++;
	if (content[(*i)] != '\"') {
		tokenize_error(line, link_s - 1, link_s,
							"unterminated `link` path, expected closing '\"'");
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
		{"...", VARIADIC, 0, 0},   {".", DOT, 0, 0},         {"::", DOUBLE_DOTS, 0, 0},
		{":", DOTS, 0, 0},         {"+=", ADD_ASSIGN, 0, 0}, {"-=", SUB_ASSIGN, 0, 0},
		{"*=", MUL_ASSIGN, 0, 0},  {"/=", DIV_ASSIGN, 0, 0}, {"%=", MOD_ASSIGN, 0, 0},
		{"!=", NOT_EQUAL, 0, 0},   {"!", NOT, 0, 0},         {"==", EQUAL, 0, 0},
		{"<<", LSHIFT, 0, 0},      {">>", RSHIFT, 0, 0},     {"<=", LESS_EQUAL, 0, 0},
		{">=", GREAT_EQUAL, 0, 0}, {"<", LESS, 0, 0},        {">", GREAT, 0, 0},
		{"=", ASSIGN, 0, 0},       {"+", ADD, 0, 0},         {"-", SUB, 0, 0},
		{"*", MUL, 0, 0},          {"/", DIV, 0, 0},         {"%", MOD, 0, 0},
		{"(", LPAR, 0, 0},         {")", RPAR, 0, 0},        {"[", LBRA, 0, 0},
		{"]", RBRA, 0, 0},         {",", COMA, 0, 0},        {"&&", AND, 0, 0},
		{"||", OR, 0, 0},          {"&", BAND, 0, 0},        {"|", BOR, 0, 0},
		{"^", BXOR, 0, 0},         {"~", BNOT, 0, 0},
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
	resize_array(ura.tokens, Token *, ura.tokens_size, ura.tokens_count);
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
			{"int", INT, 1, 1},           {"bool", BOOL, 1, 1},			{"chars", CHARS, 1, 1},       
			{"char", CHAR, 1, 1},			{"float", FLOAT, 1, 1},       {"void", VOID, 1, 1},
			{"long", LONG, 1, 1},         {"pointer", CHARS, 1, 1},		{"short", SHORT, 1, 1},       
			{"if", IF, 0, 0},					{"elif", ELIF, 0, 0},         {"else", ELSE, 0, 0},
			{"while", WHILE, 0, 0},       {"for", FOR, 0, 0},				{"to", TO, 0, 0},
			{"step", STEP, 0, 0},			{"in", IN, 0, 0},             {"fn", FDEC, 0, 0},
			{"return", RETURN, 0, 0},     {"break", BREAK, 0, 0},			{"continue", CONTINUE, 0, 0}, 
			{"ref", REF, 0, 0},				{"struct", STRUCT_DEF, 0, 0}, {"enum", ENUM_DEF, 0, 0},
			{"proto", PROTO, 0, 0},       {"mod", MODULE, 0, 0},			{"operator", OPERATOR, 0, 0}, 
			{"as", AS, 0, 0},					{"pub", PUB, 0, 0},           {"delete", DELETE, 0, 0},
			{"and", AND, 0, 1},           {"or", OR, 0, 1},					{"is", EQUAL, 0, 1},          
			{"not", NOT, 0, 1},				{"typeof", TYPEOF, 0, 1},     {"sizeof", SIZEOF, 0, 1},
			{"stack", STACK, 0, 1},       {"heap", HEAP, 0, 1},			{"array", ARRAY_TYPE, 0, 1},  
			{"List", LIST_TYPE, 0, 1},		{"null", NULLABLE, 0, 1},
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
				tokenize_error(line, s, e, "unknown escape character: \\%c", input[s + 1]);
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

Token *next() { return ura.tokens[ura.exe_pos++]; }

Token *peek(int index) { return ura.tokens[ura.exe_pos + index];}

Token *find(Type type, ...) {
	if (ura.error_count) return NULL;
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
   case ASSIGN: return 10;
   case ADD:    return 20;
   case SUB:    return 20;
   case MUL:    return 30;
   case DIV:    return 30;
   case MOD:    return 30;
   default:
      break;
   }
   return 0;
}

bool includes(Type to_find, ...) {
	if (ura.error_count) return false;
	va_list ap;
	Type    current;
	va_start(ap, to_find);
	while ((current = va_arg(ap, Type)) != 0)
		if (current == to_find) return true;
	return false;
}

bool within(int indent) {
	Token *curr = ura.tokens[ura.exe_pos];
	return !ura.error_count && curr->indent > indent && curr->type != END;
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
	resize_array(ura.scopes, Node *, ura.scopes_size, ura.scopes_count);
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

void declare_variable(Token *token) {
	for (int v = 0; v < ura.scope->variables_count; v++)
		if (CHECK(strcmp(ura.scope->variables[v]->name, token->name) == 0,
		          "redeclaration of variable '%s'", token->name))
			return;
	resize_array(ura.scope->variables, Token *, ura.scope->variables_size,
	             ura.scope->variables_count);
	ura.scope->variables[ura.scope->variables_count++] = token;
}

Token *find_variable(char *name) {
	for (int s = ura.scopes_count; s >= 0; s--) {
		Node *scope = ura.scopes[s];
		if (!scope) continue;
		for (int v = 0; v < scope->variables_count; v++)
			if (scope->variables[v]->name && strcmp(scope->variables[v]->name, name) == 0)
				return scope->variables[v];
	}
	return NULL;
}

void declare_function(Node *fn) {
	for (int f = 0; f < ura.scope->functions_count; f++)
		if (CHECK(strcmp(ura.scope->functions[f]->token->name, fn->token->name) == 0,
		          "redeclaration of function '%s'", fn->token->name))
			return;
	resize_array(ura.scope->functions, Node *, ura.scope->functions_size,
	             ura.scope->functions_count);
	ura.scope->functions[ura.scope->functions_count++] = fn;
}

Node *find_function(char *name) {
	for (int s = ura.scopes_count; s >= 0; s--) {
		Node *scope = ura.scopes[s];
		if (!scope) continue;
		for (int f = 0; f < scope->functions_count; f++)
			if (scope->functions[f]->token->name && strcmp(scope->functions[f]->token->name, name) == 0)
				return scope->functions[f];
	}
	return NULL;
}

Node *fdec_node(Node *node) {
	node->token->type = FDEC;
	enter_scope(node);
	if (!find(LPAR, 0))
		parse_error(node->token, "expected '(' after function %s", node->token->name);
	while (!ura.error_count && peek(0)->type != RPAR) {
		Token *param = find(ID, 0);
		if (!param) {
			parse_error(node->token, "expected parameter name in function %s", node->token->name);
			break;
		}
		if (!is_data_type(peek(0))) {
			parse_error(param, "expected data type for parameter %s", param->name);
			break;
		}
		param->ret_type = next()->type;
		param->is_param = true;
		param->is_dec   = true;
		resize_array(node->token->Fn.params, Token *, node->token->Fn.params_size,
		             node->token->Fn.params_count);
		node->token->Fn.params[node->token->Fn.params_count++] = param;
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, "expected ')' after function %s", node->token->name);

	if(strcmp(node->token->name, "main") == 0) {
		node->token->ret_type = INT;
	}
	// TODO: expect data type
	else if(is_data_type(peek(0))) {
		node->token->ret_type = next()->type;
	}
	else
		parse_error(node->token, "expected <data type> after function %s", node->token->name);
	if (!find(DOTS, 0))
		parse_error(node->token, "expected ':' after function %s", node->token->name);

	while(within(node->token->indent)) {
		resize_array(node->children, Node*, node->children_size, node->children_count);
		node->children[node->children_count++] = expr_node(0);
	}

	exit_scope();
	return node;
}

Node *fcall_node(Node *node) {
	node->token->type = FCALL;
	if (!find(LPAR, 0))
		parse_error(node->token, "expected '(' after %s", node->token->name);
	while (!ura.error_count && peek(0)->type != RPAR) {
		resize_array(node->children, Node *, node->children_size, node->children_count);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, "expected ')' after %s arguments", node->token->name);
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
   case VOID:  return ura.vd;
   default: TODO(1, "to_llvm_type: unhandled type %t", type); return NULL;
   }
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
}

void finalize_module(char *ll_path) {
   char *error = NULL;
   if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
      CHECK(1, "module verification failed:\n%s", error);
   LLVMDisposeMessage(error);
   LLVMPrintModuleToFile(ura.module, ll_path, NULL);
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
