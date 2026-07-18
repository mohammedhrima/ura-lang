#include "header.h"

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

bool lex_spaces(char *src, int *i, int *line, int *indent, int base)
{
	char c = src[*i];
	if (!isspace(c)) 
	 	return false;
	 
	if (c == '\n') {
		(*line)++;
		(*indent)    = base;
	}
	else {
		int j = *i;
		while (j > 0 && (src[j - 1] == ' ' || src[j - 1] == '\t'))
			j--;
		if (j == 0 || src[j - 1] == '\n')
			(*indent) += (c == '\t') ? TAB : 1;
	}
	(*i)++;

	return true;
}

bool lex_multi_comment(char *src, int *i, int *line)
{
	int s = *i;
	if (strncmp(src + (*i), "/*", 2) != 0)
		return false;
	
	(*i) += 2;
	while (src[(*i)] && src[(*i) + 1] && strncmp(src + (*i), "*/", 2)) {
		if (src[(*i)] == '\n') (*line)++;
		(*i)++;
	}
	if (strncmp(src + (*i), "*/", 2) != 0) {
		tokenize_error((*line), s, s + 2, ERR_UNTERM_BLOCK_COMMENT);
		return true;
	}
	(*i) += 2;
	return true;
}

bool lex_comment(char *src, int *i)
{
	if (strncmp(src + (*i), "//", 2) != 0)
		return false;

	while (src[(*i)] && src[(*i)] != '\n')
		(*i)++;
	return true;
}

bool lex_chars(char *src, int *i, int line, int indent)
{
	int s = *i;
	if (src[(*i)] != '\"')
		return false;

	(*i)++;
	while (src[(*i)] && src[(*i)] != '\"') {
		if (src[(*i)] == '\\' && src[(*i) + 1]) (*i)++;
		(*i)++;
	}
	if (src[(*i)] != '\"') {
		tokenize_error(line, s, s + 1, ERR_UNTERM_STRING_LITERAL);
		return true;
	}
	(*i)++;
	parse_token(line, s + 1, (*i) - 1, CHARS, indent);
	return true;
}

bool lex_char(char *src, int *i, int line, int indent)
{
	int s = *i;
	if (src[(*i)] != '\'')
		return false;

	(*i)++;
	if (src[(*i)] == '\\' && src[(*i) + 1]) (*i)++;
	if (src[(*i)] && src[(*i)] != '\'')     (*i)++;
	if (src[(*i)] != '\'')                 {
		tokenize_error(line, s, s + 1, ERR_UNTERM_CHAR_LITERAL);
		return true;
	}
	(*i)++;
	parse_token(line, s + 1, (*i) - 1, CHAR, indent);
	return true;
}

bool lex_number(char *src, int *i, int line, int indent)
{
	int s = *i;
	if (!isdigit(src[(*i)])) return false;
	while (isdigit(src[(*i)])) (*i)++;
	if (src[(*i)] == '.' && isdigit(src[(*i) + 1])) {
		(*i)++;
		while (isdigit(src[(*i)])) (*i)++;
		parse_token(line, s, *i, FLOAT, indent);
	}
	else
		parse_token(line, s, *i, INT, indent);
	return true;
}

bool lex_use(char *src, int *i, int s, int line)
{
	bool is_len   = (*i) - s == 3;
	bool is_word  = strncmp(src + s, "use", 3) == 0;
	bool is_space = isspace(src[(*i)]);
	if (!is_len || !is_word || !is_space) return false;
	while (isspace(src[(*i)]))
		(*i)++;
	if (src[(*i)] != '\"') {
		tokenize_error(line, (*i), (*i) + 1, "Expected '\"' after 'use'");
		return true;
	}
	(*i)++;
	{
		int start = (*i);
		while (src[(*i)] && src[(*i)] != '\"' && src[(*i)] != '\n')
			(*i)++;
		if (src[(*i)] != '\"') {
			tokenize_error(line, start - 1, start, ERR_UNTERM_USE_PATH);
			return true;
		}
		(*i)++;
		char *use = strndup(src + start, (*i) - start - 1);
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

bool lex_link(char *src, int *i, int s, int line)
{
	bool is_len   = (*i) - s == 4;
	bool is_word  = strncmp(src + s, "link", 4) == 0;
	bool is_space = isspace(src[(*i)]);
	if (!is_len || !is_word || !is_space) return false;
	while (isspace(src[(*i)]))
		(*i)++;
	if (src[(*i)] != '\"') {
		tokenize_error(line, (*i), (*i) + 1, "Expected '\"' after 'link'");
		return true;
	}
	(*i)++;
	int link_s = (*i);
	while (src[(*i)] && src[(*i)] != '\"' && src[(*i)] != '\n')
		(*i)++;
	if (src[(*i)] != '\"') {
		tokenize_error(line, link_s - 1, link_s, ERR_UNTERM_LINK_PATH);
		return true;
	}
	(*i)++;
	return true;
}

bool lex_identifier(char *src, int *i, int line, int indent, int base)
{
	int s = *i;
	if (!(isalpha(src[*i]) || strchr("@$_", src[*i])))
		return false;
	while (src[*i] && (isalnum(src[*i]) || strchr("@$_", src[*i])))
		(*i)++;
	if (lex_use(src, i, s, line)) {
		if (ura.error_count) return true;
		ura.calling_use++;
		tokenize(indent);
		ura.calling_use--;
	} else if (!lex_link(src, i, s, line)) {
		parse_token(line, s, *i, ID, indent);
	}
	return true;
}

bool lex_symbol(char *src, int *i, int line, int *indent)
{
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
		if (strncmp(specials[j].name, src + (*i), len) == 0) {
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
			{"int", INT, 1, 1},       {"bool", BOOL, 1, 1},       {"chars", CHARS, 1, 1},
			{"char", CHAR, 1, 1},     {"void", VOID, 1, 1},       {"float", FLOAT, 1, 1},
			{"long", LONG, 1, 1},     {"short", SHORT, 1, 1},     {"pointer", CHARS, 1, 1},
			{"if", IF, 0, 0},         {"elif", ELIF, 0, 0},       {"else", ELSE, 0, 0},
			{"for", FOR, 0, 0},       {"loop", LOOP, 0, 0},       {"while", WHILE, 0, 0},
			{"to", TO, 0, 0},         {"in", IN, 0, 0},           {"step", STEP, 0, 0},
			{"fn", FDEC, 0, 0},       {"break", BREAK, 0, 0},     {"return", RETURN, 0, 0},
			{"case", CASE, 0, 0},     {"match", MATCH, 0, 0},     {"continue", CONTINUE, 0, 0},
			{"ref", REF, 0, 0},       {"default", DEFAULT, 0, 0}, {"struct", STRUCT_DEF, 0, 0},
			{"mod", MODULE, 0, 0},    {"proto", PROTO, 0, 0},     {"enum", ENUM_DEF, 0, 0},
			{"as", AS, 0, 0},         {"pub", PUB, 0, 0},         {"operator", OPERATOR, 0, 0},
			{"new", NEW, 0, 0},       {"and", AND, 0, 1},         {"clean", CLEAN, 0, 0},
			{"or", OR, 0, 1},         {"not", NOT, 0, 1},         {"is", EQUAL, 0, 1},
			{"stack", STACK, 0, 1},   {"typeof", TYPEOF, 0, 1},   {"sizeof", SIZEOF, 0, 1},
			{"heap", HEAP, 0, 1},     {"List", LIST_TYPE, 0, 1},  {"array", ARRAY_TYPE, 0, 1},
			{"null", NULLABLE, 0, 1},
		};

		for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
			if (strcmp(new->name, keywords[i].name) != 0) continue;
			new->type   = keywords[i].type;
			new->is_dec = keywords[i].is_dec;
			if (keywords[i].is_dec || keywords[i].clear_name) 
				set_name(new, NULL);
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
			bool is_escape = s + 1 < e && input[s] == '\\';
			if (is_escape) s = parse_escape_seq(input, s, e, value, &j);
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
				tokenize_error(line, s, e, ERR_UNKNOWN_ESCAPE, input[s + 1]);
		}
		new->Char.value = buf[0];
		break;
	}
	default: break;
	}
	return new;
}
