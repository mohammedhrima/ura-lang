#include "header.h"

// Global variable definitions
bool             found_error;
bool             prep_mode;

Token          **tokens;
int              tcount;
int              tsize;
int              ecount;

Node            *ura_scope;
Node            *scope;
Node           **level_scope;

int              sccount;
int              scsize;

char           **used_files;
int              ucount;
int              usize;

Context          context;
Module           module;
Builder          builder;
TypeRef          vd, f32, i1, i2, i4, i8, i16, i32, i64, p8, p32;
char            *op_flags;
bool             enable_san;
bool             enable_debug = true;
char            *argv0;
char            *ura_lib;
int              calling_use;
char            *current_gen_module;

char           **links;
int              lcount;
int              lsize;
char           **src_files;
int              src_count;
int              src_size;

LLVMDIBuilderRef debug_builder;
LLVMMetadataRef  debug_compile_unit;
LLVMMetadataRef  debug_file;
LLVMMetadataRef  debug_scope;

const char      *type_to_ura_name(Type type) {
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

//
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
		default:  todo(1, "build_binary_op: unhandled float op %s", name); return NULL;
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
	default:     todo(1, "build_binary_op: unhandled op %s", name); return NULL;
	}
}

void _entry(Token *token) {
	Block entry = LLVMAppendBasicBlockInContext(context, token->llvm.elem, "entry");
	LLVMPositionBuilderAtEnd(builder, entry);
}

static int parse_escape_seq(char *input, int s, int e, char *buf, int *j) {
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

static bool match_table(Token *new, char *name, Types *table, bool is_dec) {
	for (int i = 0; table[i].name; i++) {
		if (strcmp(table[i].name, name) == 0) {
			new->type   = table[i].type;
			new->is_dec = is_dec;
			if (is_dec) setName(new, NULL);
			return true;
		}
	}
	return false;
}

Token *parse_token(char *filename, int line, char *input, int s, int e, Type type, int space) {
	Token *new    = new_token(type, space);
	new->line     = line;
	new->filename = filename;

	switch (type) {
	case INT: {
		while (s < e)
			new->Int.value = new->Int.value * 10 + input[s++] - '0';
		break;
	}
	case FLOAT: {
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

		struct {
			char *name;
			bool  value;
		} bools[] = {{"True", true}, {"False", false}, {0, 0}};
		for (int i = 0; bools[i].name; i++)
			if (strcmp(new->name, bools[i].name) == 0) {
				setName(new, NULL);
				new->type       = BOOL;
				new->Bool.value = bools[i].value;
				goto id_done;
			}

		Types data_types[] = {{"int", INT},     {"bool", BOOL}, {"chars", CHARS}, {"char", CHAR},
		                      {"float", FLOAT}, {"void", VOID}, {"long", LONG},   {"pointer", CHARS},
		                      {"short", SHORT}, {0, 0}};
		if (match_table(new, new->name, data_types, true)) goto id_done;

		Types keywords[] = {{"if", IF},         {"elif", ELIF},
		                    {"else", ELSE},     {"while", WHILE},
		                    {"fn", FDEC},       {"return", RETURN},
		                    {"break", BREAK},   {"continue", CONTINUE},
		                    {"ref", REF},       {"struct", STRUCT_DEF},
		                    {"enum", ENUM_DEF}, {"proto", PROTO},
		                    {"mod", MODULE},    {"operator", OPERATOR},
		                    {"as", AS},         {"pub", PUB},
		                    {"delete", DELETE}, {0, 0}};
		if (match_table(new, new->name, keywords, false)) goto id_done;

		Types keywords2[] = {{"and", AND},        {"or", OR},         {"is", EQUAL},
		                     {"not", NOT},        {"typeof", TYPEOF}, {"sizeof", SIZEOF},
		                     {"stack", STACK},    {"heap", HEAP},     {"array", ARRAY_TYPE},
		                     {"List", LIST_TYPE}, {"null", NULLABLE}, {0, 0}};
		if (match_table(new, new->name, keywords2, false)) setName(new, NULL);
	id_done:
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
		char value = 0;
		if (e == s + 1) value = input[s];
		else // special character
		{
			char escape_map[256] = {
			    ['n'] = '\n', ['t'] = '\t', ['r'] = '\r',  ['b'] = '\b', ['f'] = '\f',  ['v'] = '\v',
			    ['a'] = '\a', ['0'] = '\0', ['\\'] = '\\', ['"'] = '\"', ['\''] = '\'', ['?'] = '\?',
			};
			char c = escape_map[(unsigned char)input[s + 1]];
			check(!c && input[s + 1] != '0', "unknown escape character");
			value = c;
		}
		new->Char.value = value;
		break;
	}
	default: break;
	}
	return new;
}

// ----------------------------------------------------------------------------
// Tokenizer
// ----------------------------------------------------------------------------
void tokenize(char *file_name, int default_space) {
	if (found_error) return;
	char *filename = realpath(file_name, NULL);
	File  file     = fopen(filename, "r");
	if (check(!file, "openning %s", file_name)) return;
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *input = allocate((size + 1), sizeof(char));
	fread(input, size, sizeof(char), file);
	fclose(file);

	debug(GREEN("TOKENIZE: [%s]\n"), file_name);

	struct {
		char *value;
		Type  type;
	} specials[] = {
	    {"...", VARIADIC},  {".", DOT},         {"::", DOUBLE_DOTS}, {":", DOTS},
	    {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},  {"/=", DIV_ASSIGN},
	    {"%=", MOD_ASSIGN}, {"!=", NOT_EQUAL},  {"!", NOT},          {"==", EQUAL},
	    {"<<", LSHIFT},     {">>", RSHIFT},     {"<=", LESS_EQUAL},  {">=", GREAT_EQUAL},
	    {"<", LESS},        {">", GREAT},       {"=", ASSIGN},       {"+", ADD},
	    {"-", SUB},         {"*", MUL},         {"/", DIV},          {"%", MOD},
	    {"(", LPAR},        {")", RPAR},        {"[", LBRA},         {"]", RBRA},
	    {",", COMA},        {"&&", AND},        {"||", OR},          {"&", BAND},
	    {"|", BOR},         {"^", BXOR},        {"~", BNOT},         {0, (Type)0},
	};

	int  space    = default_space;
	int  line     = 1;
	bool new_line = true;

	resize_array(used_files, char *, usize, ucount);
	for (int i = 0; i < ucount; i++) {
		if (strcmp(file_name, used_files[i]) == 0) return;
	}
	used_files[ucount++] = file_name;

	for (int i = 0; input[i] && !found_error;) {
		int  s = i;
		char c = input[i];

		if (isspace(c)) {
			if (c == '\n') {
				line++;
				new_line = true;
				space    = default_space;
			} else if (new_line) space += (c == '\t') ? TAB : 1;
			i++;
			continue;
		}
		if (strncmp(input + i, "/*", 2) == 0) {
			i += 2;
			while (input[i] && input[i + 1] && strncmp(input + i, "*/", 2)) {
				if (input[i] == '\n') line++;
				i++;
			}
			check(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'");
			i += 2;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0) {
			while (input[i] && input[i] != '\n')
				i++;
			continue;
		}

		new_line = false;
		if (c == '\"') {
			i++;
			while (input[i] && input[i] != '\"') {
				if (input[i] == '\\') i++;
				i++;
			}
			check(input[i++] != '\"', "expected '\"'");
			parse_token(file_name, line, input, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'') {
			i++;
			if (input[i] == '\\')             i++;
			if (input[i] && input[i] != '\'') i++;
			check(input[i++] != '\'', "expected '\''");
			parse_token(file_name, line, input, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				check(input[i++] != '\"', "expected \" after use");
				int s = i;
				while (input[i] && input[i] != '\"')
					i++;
				check(input[i++] != '\"', "expected \" after use <filename>");

				char *use = strndup(input + s, i - s - 1);
				char *tmp = strjoin(use, ".ura", NULL);
				free(use);
				use = tmp;
				if (use[0] == '@') {
					tmp = strjoin(ura_lib, "/", use + 1);
					free(use);
					use = tmp;
				} else if (use[0] != '/') {
					char *dir_name = dirname(file_name);
					tmp            = strjoin(dir_name, "/", use);
					free(use);
					use = tmp;
				}
				calling_use++;
				tokenize(use, space);
				calling_use--;
			} else if (i - s == 4 && strncmp(input + s, "link", 4) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				check(input[i++] != '\"', "expected \" after link");
				int s = i;
				while (input[i] && input[i] != '\"')
					i++;
				check(input[i++] != '\"', "expected \" after link <filename>");
				char *raw = strndup(input + s, i - s - 1);
				// strip "@/" prefix: link "@/raylib" → "raylib" → env URA_LINK_raylib
				if (raw[0] == '@' && raw[1] == '/') {
					char *stripped = strdup(raw + 2);
					free(raw);
					raw = stripped;
				}
				for (int i = 0; i < lcount; i++)
					if (strcmp(links[i], raw) == 0) goto _continue;
				resize_array(links, char *, lsize, lcount);
				links[lcount++] = raw;
			} else parse_token(file_name, line, input, s, i, ID, space);
		_continue:
			continue;
		}
		if (isdigit(c)) {
			while (isdigit(input[i]))
				i++;
			if (input[i] == '.' && isdigit(input[i + 1])) {
				i++;
				while (isdigit(input[i]))
					i++;
				parse_token(file_name, line, input, s, i, FLOAT, space);
			} else parse_token(file_name, line, input, s, i, INT, space);
			continue;
		}
		bool found = false;
		for (int j = 0; specials[j].value; j++) {
			size_t len = strlen(specials[j].value);
			if (strncmp(specials[j].value, input + i, len) == 0) {
				parse_token(file_name, line, NULL, 0, 0, specials[j].type, space);
				i += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		check(1, "Syntax error <%c>", c);
	}
	if (!calling_use) new_token(END, -1);
	free(input);
}

void tokenize_string(char *input, char *fake_name, int default_space) {
	if (found_error) return;
	debug(GREEN("TOKENIZE: [%s]\n"), fake_name);

	struct {
		char *value;
		Type  type;
	} specials[] = {
	    {"...", VARIADIC},  {".", DOT},         {"::", DOUBLE_DOTS}, {":", DOTS},
	    {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},  {"/=", DIV_ASSIGN},
	    {"%=", MOD_ASSIGN}, {"!=", NOT_EQUAL},  {"!", NOT},          {"==", EQUAL},
	    {"<<", LSHIFT},     {">>", RSHIFT},     {"<=", LESS_EQUAL},  {">=", GREAT_EQUAL},
	    {"<", LESS},        {">", GREAT},       {"=", ASSIGN},       {"+", ADD},
	    {"-", SUB},         {"*", MUL},         {"/", DIV},          {"%", MOD},
	    {"(", LPAR},        {")", RPAR},        {"[", LBRA},         {"]", RBRA},
	    {",", COMA},        {"&&", AND},        {"||", OR},          {"&", BAND},
	    {"|", BOR},         {"^", BXOR},        {"~", BNOT},         {0, (Type)0},
	};

	int  space    = default_space;
	int  line     = 1;
	bool new_line = true;

	for (int i = 0; input[i] && !found_error;) {
		int  s = i;
		char c = input[i];

		if (isspace(c)) {
			if (c == '\n') {
				line++;
				new_line = true;
				space    = default_space;
			} else if (new_line) space += (c == '\t') ? TAB : 1;
			i++;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0) {
			while (input[i] && input[i] != '\n')
				i++;
			continue;
		}
		new_line = false;
		if (c == '\"') {
			i++;
			while (input[i] && input[i] != '\"') {
				if (input[i] == '\\') i++;
				i++;
			}
			check(input[i++] != '\"', "expected '\"'");
			parse_token(fake_name, line, input, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'') {
			i++;
			if (input[i] == '\\')             i++;
			if (input[i] && input[i] != '\'') i++;
			check(input[i++] != '\'', "expected '\''");
			parse_token(fake_name, line, input, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			parse_token(fake_name, line, input, s, i, ID, space);
			continue;
		}
		if (isdigit(c)) {
			while (isdigit(input[i]))
				i++;
			if (input[i] == '.' && isdigit(input[i + 1])) {
				i++;
				while (isdigit(input[i]))
					i++;
				parse_token(fake_name, line, input, s, i, FLOAT, space);
			} else parse_token(fake_name, line, input, s, i, INT, space);
			continue;
		}
		bool found = false;
		for (int j = 0; specials[j].value; j++) {
			size_t len = strlen(specials[j].value);
			if (strncmp(specials[j].value, input + i, len) == 0) {
				parse_token(fake_name, line, NULL, 0, 0, specials[j].type, space);
				i += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		check(1, "Syntax error <%c>", c);
	}
	// No END token — generated code is prepended to user code
}

Node *expr_node() { return assign_node(); }

AST_NODE(assign_node, logic_or_node, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
         MOD_ASSIGN)
AST_NODE(logic_or_node, logic_and_node, OR)
AST_NODE(logic_and_node, bitor_node, AND)
AST_NODE(bitor_node, bitxor_node, BOR)
AST_NODE(bitxor_node, bitand_node, BXOR)
AST_NODE(bitand_node, equality_node, BAND)
AST_NODE(equality_node, comparison_node, EQUAL, NOT_EQUAL)
AST_NODE(comparison_node, shift_node, LESS, GREAT, LESS_EQUAL, GREAT_EQUAL)
AST_NODE(shift_node, add_sub_node, LSHIFT, RSHIFT)
AST_NODE(add_sub_node, mul_div_node, ADD, SUB)
AST_NODE(mul_div_node, as_node, MUL, DIV, MOD)

Node *as_node() // as
{
	Node  *left = unary_node();
	Token *token;
	while ((token = find(AS, 0))) {
		Node *node  = new_node(token);
		node->left  = left;
		node->right = unary_node();
		Token *to   = node->right->token;
		if (check(to == NULL || !to->is_dec, "expected data type after as")) return syntax_error();
		to->is_dec = false;
		left       = node;
	}
	return left;
}

Node *unary_node() // - + (unary)
{
	Token *token;
	if ((token = find(ADD, SUB, 0))) {
		Node *node                    = new_node(token);
		node->left                    = unary_node();
		node->right                   = new_node(new_token(INT, token->space));
		node->right->token->filename  = token->filename;
		node->right->token->line      = token->line;
		node->right->token->Int.value = token->type == SUB ? -1 : 1;
		node->token->type             = MUL;
		return node;
	}
	return access_node();
}

Node *access_node() // . []
{
	Node  *left = prime_node();
	Token *token;
	while ((token = find(DOT, LBRA, DOUBLE_DOTS, 0))) {
		Node *node = new_node(token);
		switch (token->type) {
		case DOT: {
			node->left  = left;
			node->right = prime_node();

			check(!node->right, "expected (attribute/method call) after .");
			if (node->right->token->type == FCALL) {
				Node *func                  = node->right;
				func->token->is_method_call = true;
				add_child(func->left, left);
				node->right = NULL;
				node->left  = NULL;
				node        = func;
			}
			break;
		}
		case LBRA: {
			node->token->type = ACCESS;
			node->left        = left;
			node->right       = expr_node();
			check(!node->right, "expected something between []");
			expect_token(RBRA, "expected right bracket");
			break;
		}
		case DOUBLE_DOTS: {
			// Foo::method() — static dispatch
			Node *call = prime_node();
			if (!call || call->token->type != FCALL) {
				check(1, "expected function call after '::'");
				return syntax_error();
			}
			char *qname = strjoin(left->token->name, ".", call->token->name);
			setName(call->token, qname);
			free(qname);
			call->token->is_static_call = true;
			node                        = call;
			break;
		}
		default: {
			check(1, "Unexpected %k line %d", token, token->line);
			return syntax_error();
		}
		}

		left = node;
	}
	return left;
}

static Node *parse_var_dec(Token *token) {
	// Literal value: "hello", 1, 'c'
	if (token->type != ID && !token->is_dec && !token->name) return new_node(token);
	// int, char, chars, etc. declaration keyword
	if (token->is_dec) {
		if (check(tokens[ecount]->type == ID && tokens[ecount]->line == token->line,
		          "Unexpected id after data type %k", tokens[ecount]))
			return syntax_error();
		return new_node(token);
	}
	// TODO: this one should expected using expect

	// variable declaration: name type [ref] [= ...]
	if (token->type == ID && is_data_type(tokens[ecount]) && tokens[ecount]->line == token->line) {
		Token *tmp    = tokens[ecount++]; // skip data type
		bool   is_ref = find(REF, 0) != NULL;
		if (includes(tmp->type, ARRAY_TYPE, LIST_TYPE, 0)) {
			Type container = tmp->type;
			expect_token(LBRA, "expected [ after %s", container == LIST_TYPE ? "list" : "array");
			int depth = 1;
			while (find(LBRA, 0))
				depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			check(!elem_type, "expected element type in %s type",
			      container == LIST_TYPE ? "list" : "array");
			for (int i = 0; i < depth; i++)
				expect_token(RBRA, "expected ] in %s type", container == LIST_TYPE ? "list" : "array");
			if (container == LIST_TYPE) {
				// Rewrite to STRUCT_CALL pointing to __list_T
				const char *ename =
				    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
				char sname[256];
				snprintf(sname, sizeof(sname), "__list_%s", ename);
				for (int d = 1; d < depth; d++) {
					char tmp_name[256];
					snprintf(tmp_name, sizeof(tmp_name), "__list_%s", sname);
					snprintf(sname, sizeof(sname), "%s", tmp_name);
				}
				Node *list_st = get_struct(sname);
				if (check(!list_st, "list struct '%s' not found", sname)) return syntax_error();
				tmp->type       = STRUCT_CALL;
				tmp->Struct.ptr = list_st;
			} else {
				tmp->Array.elem_type = elem_type->type;
				tmp->Array.depth     = depth;
				tmp->retType         = container;
				if (elem_type->type == ID) {
					Node *sd = get_struct(elem_type->name);
					if (sd) {
						tmp->Array.elem_type  = STRUCT_CALL;
						tmp->Array.struct_ptr = sd;
					}
				}
			}
		}
		setName(tmp, token->name);
		tmp->is_dec = true;
		tmp->is_ref = is_ref;
		// Tuple unpack: a int, b chars = foo()
		if (!is_ref && tokens[ecount]->type == COMA && ecount + 2 < tcount &&
		    tokens[ecount + 1]->type == ID && is_data_type(tokens[ecount + 2])) {
			Token *tu_tok = new_token(TUPLE_UNPACK, token->space);
			Node  *tu     = new_node(tu_tok);
			add_child(tu, new_node(tmp)); // first decl
			while (tokens[ecount]->type == COMA) {
				ecount++; // consume ,
				Token *nname = find(ID, 0);
				Token *ntype = find(DATA_TYPES, 0);
				if (includes(ntype->type, ARRAY_TYPE, LIST_TYPE, 0)) {
					Type container = ntype->type;
					expect_token(LBRA, "expected [ after %s", container == LIST_TYPE ? "list" : "array");
					int depth = 1;
					while (find(LBRA, 0))
						depth++;
					Token *elem_type = find(DATA_TYPES, ID, 0);
					check(!elem_type, "expected element type in %s type",
					      container == LIST_TYPE ? "list" : "array");
					for (int i = 0; i < depth; i++)
						expect_token(RBRA, "expected ] in %s type",
						             container == LIST_TYPE ? "list" : "array");
					if (container == LIST_TYPE) {
						const char *ename =
						    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
						char sname[256];
						snprintf(sname, sizeof(sname), "__list_%s", ename);
						for (int d = 1; d < depth; d++) {
							char tn[256];
							snprintf(tn, sizeof(tn), "__list_%s", sname);
							snprintf(sname, sizeof(sname), "%s", tn);
						}
						Node *list_st = get_struct(sname);
						if (check(!list_st, "list struct '%s' not found", sname)) return syntax_error();
						ntype->type       = STRUCT_CALL;
						ntype->Struct.ptr = list_st;
					} else {
						ntype->Array.elem_type = elem_type->type;
						ntype->Array.depth     = depth;
						ntype->retType         = container;
						if (elem_type->type == ID) {
							Node *sd = get_struct(elem_type->name);
							if (sd) {
								ntype->Array.elem_type  = STRUCT_CALL;
								ntype->Array.struct_ptr = sd;
							}
						}
					}
				}
				setName(ntype, nname->name);
				ntype->is_dec = true;
				add_child(tu, new_node(ntype));
			}
			expect_token(ASSIGN, "expected = in tuple unpack");
			tu->left = expr_node();
			return tu;
		}
		return new_node(tmp);
	}

	// variable declaration (struct type): name StructName [ref] [= ...]
	Node *st_dec = NULL;
	bool  cond   = token->type == ID && tokens[ecount]->type == ID;
	if (cond && (st_dec = get_struct(tokens[ecount]->name))) {
		find(ID, 0); // skip struct data type
		bool is_ref = find(REF, 0) != NULL;
		if (is_ref) {
			expect_token(ASSIGN,
			             "'%s': ref must be initialized at "
			             "declaration, e.g: x int ref = a",
			             token->name);
			ecount--;
		}
		token->type       = STRUCT_CALL;
		token->is_dec     = true;
		token->is_ref     = is_ref;
		token->Struct.ptr = st_dec;
		return new_node(token);
	}

	// function call or main declaration
	if (token->type == ID && find(LPAR, 0)) {
		Node *node = new_node(token);
		if (strcmp(token->name, "main") == 0) {
			expect_token(RPAR, "expected ) after main declaration");
			expect_token(DOTS, "expected : after main() declaration");

			enter_scope(node);
			node->token->type    = FDEC;
			node->token->retType = INT;
			node->left           = new_node(new_token(ARGS, node->token->space));

			Node *last = NULL;
			while (within(node->token->space)) {
				last = expr_node();
				add_child(node, last);
			}
			if (!last || last->token->type != RETURN) {
				last       = new_node(new_token(RETURN, node->token->space + TAB));
				last->left = new_node(new_token(INT, node->token->space + TAB));
				add_child(node, last);
			}

			exit_scope();
			return node;
		} else if (strcmp(token->name, "output") == 0) {
			node              = new_node(token);
			node->token->type = OUTPUT;
			node->left        = new_node(new_token(ARGS, token->space));
			Token *end_tok    = NULL;
			while (!found_error && !(end_tok = find(RPAR, END, 0))) {
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			if (check(!found_error && end_tok->type != RPAR, "output: expected )"))
				return syntax_error();
			node->token->retType = VOID;
			return node;
		} else {
			node->token->type = FCALL;
			Token *arg        = NULL;
			node->left        = new_node(new_token(ARGS, node->token->space));

			while (!found_error && !(arg = find(RPAR, END, 0))) {
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			check(!found_error && arg->type != RPAR, "expected ) after function call");
		}
		return node;
	}
	// just an ID
	if (token->type == ID) return new_node(token);
	check(1, "Unexpected token has type %k line %d", tokens[ecount], tokens[ecount]->line);
	return syntax_error();
}

static Node *parse_fdec(Token *token);

static Node *parse_struct_def(Token *token) {
	Node  *node = new_node(token);
	Token *st_name;
	if (check(!(st_name = find(ID, 0)), "expected identifier after struct definition")) return NULL;
	expect_token(DOTS, "expected dots after struct definition\n");

	setName(node->token, st_name->name);
	node->token->type       = STRUCT_DEF;
	node->token->Struct.ptr = node;
	new_struct(node);
	enter_scope(node);

	while (within(node->token->space)) {
		// pub fn — static method, no self injected
		if (find(PUB, 0)) {
			Token *fdec_tok = find(FDEC, 0);
			if (!fdec_tok) {
				check(1, "struct '%s': expected fn after pub", node->token->name);
				exit_scope();
				return syntax_error();
			}
			fdec_tok->is_pub = true; // set before parse_fdec so self is NOT injected
			Node *child      = parse_fdec(fdec_tok);
			if (!child || child->token->type != FDEC) {
				check(1, "struct '%s': expected fn after pub", node->token->name);
				exit_scope();
				return syntax_error();
			}
			child->token->is_pub = true;
			char *qualified      = strjoin(node->token->name, ".", child->token->name);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
			continue;
		}
		Token *op_kw;
		if ((op_kw = find(OPERATOR, 0))) {
			Token *op_tok = find(ASSIGN, ADD, SUB, MUL, DIV, MOD, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
			                     DIV_ASSIGN, MOD_ASSIGN, EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL,
			                     GREAT_EQUAL, AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, DELETE, 0);
			// Check for named operators: operator output
			if (!op_tok && tokens[ecount]->type == ID && strcmp(tokens[ecount]->name, "output") == 0) {
				op_tok       = tokens[ecount++];
				op_tok->type = OUTPUT;
			}
			if (check(!op_tok, "struct '%s': expected operator symbol after 'operator'",
			          node->token->name)) {
				exit_scope();
				return syntax_error();
			}
			// operator delete() void: — destructor
			if (op_tok->type == DELETE) {
				Token *fdec_tok = new_token(FDEC, op_kw->space);
				setName(fdec_tok, "delete");
				Node *child = parse_fdec(fdec_tok);
				if (check(child->token->retType != VOID,
				          "struct '%s': operator delete must return void", node->token->name)) {
					exit_scope();
					return syntax_error();
				}
				if (check(child->left->children_count != 1,
				          "struct '%s': operator delete takes no parameters", node->token->name)) {
					exit_scope();
					return syntax_error();
				}
				char *qualified = strjoin(node->token->name, ".delete", "");
				setName(child->token, qualified);
				free(qualified);
				node->token->has_clean = true;
				new_function(child);
				continue;
			}
			// operator output() — custom printing
			if (op_tok->type == OUTPUT) {
				Token *fdec_tok = new_token(FDEC, op_kw->space);
				setName(fdec_tok, "output");
				Node *child     = parse_fdec(fdec_tok);
				char *qualified = strjoin(node->token->name, ".operator.output", "");
				setName(child->token, qualified);
				free(qualified);
				new_function(child);
				continue;
			}
			char  *op_base  = strjoin("operator.", to_string(op_tok->type), "");
			Token *fdec_tok = new_token(FDEC, op_kw->space);
			setName(fdec_tok, op_base);
			free(op_base);
			Node *child = parse_fdec(fdec_tok);
			// suffix with first param type for unique overload names
			// e.g. "operator.ASSIGN.CHARS" or "operator.ASSIGN.String"
			char *op_named;
			if (child->left && child->left->children_count >= 2) {
				Token *p      = child->left->children[0]->token;
				char  *suffix = (p->type == STRUCT_CALL && p->Struct.ptr) ? p->Struct.ptr->token->name
				                                                          : to_string(p->type);
				op_named      = strjoin(child->token->name, ".", suffix);
			} else {
				op_named = strjoin(child->token->name, "", "");
			}
			char *qualified = strjoin(node->token->name, ".", op_named);
			free(op_named);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
			continue;
		}
		Node *child = prime_node();
		if (!child)                     return syntax_error();
		if (child->token->type == FDEC) {
			resize_array(child->functions, Node *, child->functions_size, child->functions_count);
			char *qualified = strjoin(node->token->name, ".", child->token->name);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
		} else {
			if (check(!child->token->is_dec, "invalid attribute")) {
				exit_scope();
				return syntax_error();
			}
			child->token->is_dec = false;
			add_child(node, child);
		}
	}
	exit_scope();
	return node;
}

static Node *parse_enum_def(Token *token) {
	Token *ename;
	if (check(!(ename = find(ID, 0)), "expected identifier after enum")) return NULL;
	if (check(!find(DOTS, 0), "expected ':' after enum name"))           return NULL;

	setName(token, ename->name);
	token->type = ENUM_DEF;
	Node *node  = new_node(token);

	int   counter = 0;
	do {
		if (!within(token->space)) break;
		Token *var = find(ID, 0);
		if (!var) break;
		var->type      = INT;
		var->is_global = true;
		var->is_dec    = true;
		var->Int.value = counter++;
		new_variable(var);
		add_child(node, new_node(var));
	} while (find(COMA, 0));

	Token *next = tokens[ecount];
	if (next->type == ID && within(next->space))
		check(1, "enum '%s': expected ',' before '%s'", token->name, next->name);

	return node;
}

static Node *parse_if(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);

	node->left = expr_node();
	expect_token(DOTS, "expected : after if condition");

	while (within(node->token->space))
		add_child(node, expr_node());

	Node *curr = node;
	while (includes(tokens[ecount]->type, ELSE, ELIF, 0) && within(node->token->space - TAB)) {
		Token *tok = find(ELSE, ELIF, 0);

		curr->right = new_node(tok);
		curr        = curr->right;

		if (tok->type == ELIF) {
			enter_scope(curr);
			curr->left = expr_node();
			expect_token(DOTS, "expected : after elif condition");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
		} else if (tok->type == ELSE) {
			enter_scope(curr);
			expect_token(DOTS, "expected : after else");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
			break;
		}
	}
	exit_scope();
	return node;
}

static Node *parse_fdec(Token *token) {
	Node *node = new_node(token);
	if (!node->token->name || strcmp(node->token->name, "fn") == 0) {
		Token *fname = find(ID, 0);
		if (check(!fname, "expected identifier after fn declaration")) return syntax_error();
		setName(node->token, fname->name);
	}

	// Check if we're inside a struct scope (method)
	Node *struct_owner = NULL;
	Node *parent       = level_scope[sccount];
	if (parent && parent->token->type == STRUCT_DEF) struct_owner = parent;

	enter_scope(node);
	expect_token(LPAR, "expected ( after function declaration");

	node->left = new_node(new_token(ARGS, node->token->space));

	Token *last;
	while (!found_error && !(last = find(RPAR, END, 0))) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			last                     = find(RPAR, 0);
			check(!last, "expected ) after function ... in variadic function");
			break;
		} else {
			Token *name = find(ID, 0);
			if (check(!name, "expected identifier in function argument %s", node->token->name))
				return syntax_error();

			Token *data_type = find(DATA_TYPES, ID, 0);
			bool   is_ref    = find(REF, 0) != NULL;
			if (check(!data_type, "expected data type in function argument")) break;
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0))          {
				Type container = data_type->type;
				expect_token(LBRA, "expected [ after %s", container == LIST_TYPE ? "list" : "array");
				int depth = 1;
				while (find(LBRA, 0))
					depth++;
				Token *elem_type = find(DATA_TYPES, ID, 0);
				check(!elem_type, "expected element type in %s type",
				      container == LIST_TYPE ? "list" : "array");
				for (int j = 0; j < depth; j++)
					expect_token(RBRA, "expected ] in %s type",
					             container == LIST_TYPE ? "list" : "array");
				if (container == LIST_TYPE) {
					const char *ename =
					    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
					char sname[256];
					snprintf(sname, sizeof(sname), "__list_%s", ename);
					for (int d = 1; d < depth; d++) {
						char tn[256];
						snprintf(tn, sizeof(tn), "__list_%s", sname);
						snprintf(sname, sizeof(sname), "%s", tn);
					}
					Node *list_st = get_struct(sname);
					if (check(!list_st, "list struct '%s' not found", sname)) break;
					data_type->type       = STRUCT_CALL;
					data_type->Struct.ptr = list_st;
					setName(data_type, sname);
				} else {
					data_type->Array.elem_type = elem_type->type;
					data_type->Array.depth     = depth;
					data_type->retType         = container;
					if (elem_type->type == ID) {
						Node *sd = get_struct(elem_type->name);
						if (sd) {
							data_type->Array.elem_type  = STRUCT_CALL;
							data_type->Array.struct_ptr = sd;
						}
					}
				}
			}
			if (data_type->type == ID) {
				Node *to_find = get_struct(data_type->name);
				if (to_find) data_type->type = STRUCT_CALL;
			}
			Node *curr;
			if (data_type->type == STRUCT_CALL) {
				Node *st = get_struct(data_type->name);
				if (check(!st, "Unknown struct type '%s'", data_type->name)) break;
				data_type->Struct.ptr = st;
				curr                  = new_node(data_type);
				data_type->is_ref     = is_ref;
				setName(data_type, name->name);
			} else {
				curr         = new_node(name);
				name->is_ref = is_ref;
				name->type   = data_type->type;
				if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
					name->retType          = data_type->type;
					name->Array.elem_type  = data_type->Array.elem_type;
					name->Array.depth      = data_type->Array.depth;
					name->Array.struct_ptr = data_type->Array.struct_ptr;
				}
			}
			curr->token->is_dec   = true;
			curr->token->is_param = true;
			add_child(node->left, curr);
		}
		if (tokens[ecount]->type != RPAR) expect_token(COMA, "expected coma");
	}
	check(!found_error && last->type != RPAR, "expected ) after function declaration");

	// If inside a struct, prepend implicit 'self' parameter (skip for pub/static fns)
	if (struct_owner && !node->token->is_pub) {
		Token *self_tok = new_token(STRUCT_CALL, node->token->space);
		setName(self_tok, "self");
		self_tok->Struct.ptr = struct_owner;
		self_tok->is_dec     = true;
		self_tok->is_ref     = true;
		add_child(node->left, new_node(self_tok));
	}

	if (tokens[ecount]->type == LPAR) {
		// Tuple return type: fn foo(…) (int, chars):
		ecount++; // consume (
		node->token->retType = TUPLE;
		int tc               = 0;
		while (tokens[ecount]->type != RPAR && tc < 8) {
			if (includes(tokens[ecount]->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				Token *t         = tokens[ecount++];
				Type   container = t->type;
				expect_token(LBRA, "expected [ in tuple %s type",
				             container == LIST_TYPE ? "list" : "array");
				Token *et = find(DATA_TYPES, ID, 0);
				if (check(!et, "expected element type in tuple %s",
				          container == LIST_TYPE ? "list" : "array"))
					return syntax_error();
				expect_token(RBRA, "expected ] in tuple %s type",
				             container == LIST_TYPE ? "list" : "array");
				if (container == LIST_TYPE) {
					const char *ename = et->type == ID ? et->name : type_to_ura_name(et->type);
					char        sname[256];
					snprintf(sname, sizeof(sname), "__list_%s", ename);
					Node *list_st = get_struct(sname);
					if (check(!list_st, "list struct '%s' not found", sname)) return syntax_error();
					t->type       = STRUCT_CALL;
					t->Struct.ptr = list_st;
				} else {
					t->Array.elem_type = et->type;
					t->retType         = container;
					if (et->type == ID) {
						Node *sd = get_struct(et->name);
						if (sd) {
							t->Array.elem_type  = STRUCT_CALL;
							t->Array.struct_ptr = sd;
						}
					}
				}
				node->token->Tuple.types[tc++] = t;
			} else if (tokens[ecount]->type == ID) {
				Token *t  = tokens[ecount++];
				Node  *st = get_struct(t->name);
				if (check(!st, "unknown type '%s' in tuple", t->name)) return syntax_error();
				t->type                        = STRUCT_CALL;
				t->Struct.ptr                  = st;
				node->token->Tuple.types[tc++] = t;
			} else if (is_data_type(tokens[ecount])) {
				node->token->Tuple.types[tc++] = tokens[ecount++];
			} else {
				check(1, "unexpected token in tuple return type");
				return syntax_error();
			}
			if (tokens[ecount]->type == COMA) ecount++;
		}
		expect_token(RPAR, "expected ) after tuple return types");
		node->token->Tuple.count = tc;
	} else {
		if (check(!is_data_type(tokens[ecount]) && tokens[ecount]->type != ID,
		          "expected data type after fun declaration"))
			return syntax_error();
		Token *ret_tok = tokens[ecount++];
		if (ret_tok->type == ID) {
			Node *st = get_struct(ret_tok->name);
			if (check(!st, "unknown return type '%s'", ret_tok->name)) return syntax_error();
			node->token->retType    = STRUCT_CALL;
			node->token->Struct.ptr = st;
		} else node->token->retType = ret_tok->type;
	}
	node->token->is_ref = find(REF, 0) != NULL;

	if (!node->token->is_proto) {
		Token *next = find(DOTS, 0);
		if (check(!found_error && !next, "expected : after function declaration"))
			return syntax_error();
		Node *child = NULL;

		if (next->type == DOTS)
			while (within(node->token->space))
				child = add_child(node, expr_node());
		else {
			Token *retToken = copy_token(next);
			retToken->type  = RETURN;
			Node *retNode   = new_node(retToken);
			retNode->left   = expr_node();
			child           = add_child(node, retNode);
		}
		if (next->type == DOTS) {
			if (node->token->retType != VOID)
				check(!child || child->token->type != RETURN, "expected return statement %s",
				      node->token->name);
			else {
				Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
				ret->left = new_node(new_token(VOID, node->token->space + TAB));
				add_child(node, ret);
			}
		}
	}
	exit_scope();
	return node;
}

static Node *parse_return(Token *token) {
	// TODO: check if return type is compatible with function in current scope (done in gen_ir)
	Node *node = new_node(token);
	for (int i = sccount; i >= 0; i--) {
		Node *curr = level_scope[i];
		if (curr->token->type == FDEC) {
			if (curr->token->retType == VOID) {
				node->left              = copy_node(node);
				node->left->token->type = VOID;
			} else if (curr->token->retType == TUPLE) {
				add_child(node, expr_node()); // first value
				while (tokens[ecount]->type == COMA) {
					ecount++;
					add_child(node, expr_node());
				}
			} else node->left = expr_node();
			break;
		}
	}
	return node;
}

// ----------------------------------------------------------------------------
// Primary expression parser
// ----------------------------------------------------------------------------
Node *prime_node() {
	Node  *node = NULL;
	Token *token;

	if (find(PUB, 0)) {
		check(1, "'pub' is only valid inside struct definitions");
		return syntax_error();
	}
	if ((token = find(ID, DATA_TYPES, 0))) return parse_var_dec(token);
	if ((token = find(MODULE, 0)))         {
		Token *id = find(ID, 0);
		check(!id, "expect module identifier after 'mod'");
		expect_token(DOTS, "expect DOTs after mod name");
		setName(token, id->name);
		node = new_node(token);
		enter_scope(node);
		while (within(token->space)) {
			Node  *curr   = prime_node();
			Token *ctoken = curr->token;
			check(!includes(ctoken->type, STRUCT_DEF, FDEC, 0) && !ctoken->is_dec,
			      "only functions declaration"
			      "are allowed within module");
			add_child(node, curr);
		}
		exit_scope();

		return node;
	}
	if ((token = find(STRUCT_DEF, 0))) return parse_struct_def(token);
	if ((token = find(ENUM_DEF, 0)))   return parse_enum_def(token);
	if ((token = find(IF, 0)))         return parse_if(token);

	if ((token = find(WHILE, 0))) {
		// while layout:
		//    + left    : condition
		//    + children: code bloc
		node = new_node(token);
		enter_scope(node);
		node->left = expr_node();
		expect_token(DOTS, "expected : after if condition");
		while (within(node->token->space))
			add_child(node, expr_node());
		exit_scope();
		return node;
	}
	if ((token = find(BREAK, CONTINUE, 0))) return new_node(token);

	if ((token = find(STACK, HEAP, 0))) {
		node = new_node(token);
		expect_token(LBRA, "expected [ after stack");

		int depth = 1;
		while (find(LBRA, 0))
			depth++;

		Token *elem_type = find(DATA_TYPES, ID, 0);
		check(!elem_type, "expected element type in stack");

		for (int i = 0; i < depth; i++)
			expect_token(RBRA, "expected ] in stack type");

		expect_token(LPAR, "expected ( after stack[type]");

		node->token->retType         = ARRAY;
		node->token->Array.elem_type = elem_type->type;
		node->token->Array.depth     = depth;
		if (elem_type->type == ID) {
			Node *sd = get_struct(elem_type->name);
			if (sd) {
				node->token->Array.elem_type  = STRUCT_CALL;
				node->token->Array.struct_ptr = sd;
			}
		}

		for (int i = 0; i < depth; i++) {
			add_child(node, expr_node());
			if (i < depth - 1) expect_token(COMA, "expected , between dimensions");
		}
		expect_token(RPAR, "expected ) after stack size");
		return node;
	}
	if ((token = find(TYPEOF, SIZEOF, 0))) {
		char *msg  = token->type == TYPEOF ? "typeof" : "sizeof";
		Type  type = token->type == TYPEOF ? CHARS : INT;
		node       = new_node(token);
		expect_token(LPAR, "%s: expected (", msg);
		node->left = prime_node();
		expect_token(RPAR, "%s: expected )", msg);
		node->token->retType = type;
		return node;
	}
	if ((token = find(PROTO, 0))) {
		if (includes(tokens[ecount]->type, FDEC, STRUCT_DEF, 0)) tokens[ecount]->is_proto = true;
		else check(1, "expected <fn> or <struct> after proto");
		return expr_node();
	}

	if ((token = find(FDEC, 0)))   return parse_fdec(token);
	if ((token = find(RETURN, 0))) return parse_return(token);

	// ref expr — take address of variable
	if ((token = find(REF, 0))) {
		node       = new_node(token);
		node->left = prime_node();
		return node;
	}
	if ((token = find(NOT, 0))) {
		node       = new_node(token);
		node->left = expr_node();
		return node;
	}
	if ((token = find(BNOT, 0))) {
		node       = new_node(token);
		node->left = prime_node();
		return node;
	}
	if ((token = find(NULLABLE, 0))) return new_node(token);
	// Array literal: [expr, expr, ...]
	if ((token = find(LBRA, 0))) {
		token->type = ARRAY_LIT;
		node        = new_node(token);
		if (tokens[ecount]->type != RBRA) {
			add_child(node, expr_node());
			while (find(COMA, 0))
				add_child(node, expr_node());
		}
		expect_token(RBRA, "expected ] to close array literal");
		return node;
	}
	if ((token = find(LPAR, 0))) {
		if (tokens[ecount]->type != RPAR) node = expr_node();
		expect_token(RPAR, "expected right )");
		return node;
	}
	debug("%k\n", tokens[ecount - 1]);
	check(1, "Unexpected token has type %s line %d", to_string(tokens[ecount]->type),
	      tokens[ecount]->line);
	return syntax_error();
}

Node *optimize_ir(Node *node, bool *changed) {
	// TODO: remove unsed attributes
	if (!node || found_error) return NULL;

	node->left  = optimize_ir(node->left, changed);
	node->right = optimize_ir(node->right, changed);

	if (node->children_count) {
		int    j            = 0;
		Node **new_children = allocate(node->children_size, sizeof(Node *));
		for (int i = 0; i < node->children_count; i++) {
			Node *child = optimize_ir(node->children[i], changed);
			if (child) new_children[j++] = child;
		}
		node->children_count = j;
		node->children       = new_children;
	}

	if (!node->token) return node;
	switch (node->token->type) {
	case FDEC: case PROTO: {
		if (node->token->used == 0 && strcmp(node->token->name, "main") != 0) {
			debug(RED("remove %k\n"), node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
	// TODO: unused var/op/assign removal needs fixing — too aggressive, breaks 26 tests
	// Re-enable after fixing the `used` tracking in gen_ir
	default: break;
	}

	return node;
}

// ----------------------------------------------------------------------------
// gen_ir helpers — extracted from large switch cases
// ----------------------------------------------------------------------------

static void ir_method_call(Node *node) {
	// resolve object type
	Node *obj_node = node->left->children[node->left->children_count - 1];
	gen_ir(obj_node);
	if (found_error) return;
	Token *obj = obj_node->token;

	// build qualified name: "User.greet"
	obj->Struct.ptr->token->used++; // mark struct def as used
	char *struct_name = obj->Struct.ptr->token->name;
	char *qname       = strjoin(struct_name, ".", node->token->name);
	setName(node->token, qname);
	free(qname);
	node->token->is_method_call = false;

	Node *func = get_function(node->token->name);
	if (!func)               return;
	if (func->token->is_pub) {
		check(1, "'%s' is a pub fn — use '::'", node->token->name);
		return;
	}
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);

	// first arg already gen_ir'd (obj), do rest
	obj_node->token->used++;
	Node *dec_args = func->left;
	for (int i = 0; !found_error && i < node->left->children_count - 1; i++) {
		Node *carg = node->left->children[i];
		gen_ir(carg);
		if (check(carg->token->type == ID, "Undeclared variable %s", carg->token->name)) break;
		carg->token->used++;

		Token *src = carg->token;
		if (i < dec_args->children_count - 1) { // -1: last dec_arg is implicit self
			Type param_type   = dec_args->children[i]->token->type;
			Type arg_type     = src->type;
			bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
			bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
			if (param_is_int && arg_is_int && param_type != arg_type) {
				Token *as_tok           = new_token(AS, src->space);
				as_tok->retType         = param_type;
				Node *tgt_node          = new_node(new_token(param_type, src->space));
				Node *as_node_          = new_node(as_tok);
				as_node_->left          = carg;
				as_node_->right         = tgt_node;
				node->left->children[i] = as_node_;
			} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
				check(1, "'%s' argument %d: cannot pass %s as %s", node->token->name, i + 1,
				      to_string(arg_type), to_string(param_type));
				break;
			}
		}
	}
	free_node(func);
	set_ret_type(node);
}

static void ir_regular_call(Node *node) {
	Node *func = get_function(node->token->name);
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);

	Node *call_args = node->left;
	Node *dec_args  = func->left;
	// TODO: check number of arguments
	for (int i = 0; !found_error && call_args && i < call_args->children_count; i++) {
		Node *carg = call_args->children[i];
		gen_ir(carg);
		carg->token->used++;
		Token *src = carg->token;
		if (check(src->type == ID, "Undeclared variable %s", carg->token->name)) break;

		if (i < dec_args->children_count) {
			bool param_is_ref = dec_args->children[i]->token->is_ref;
			if (param_is_ref)
				if (check(src->type == FCALL || (src->type != DOT && src->type != ACCESS && !src->name),
				          "'%s': ref parameter requires a named variable",
				          dec_args->children[i]->token->name))
					break;

			Type param_type   = dec_args->children[i]->token->type;
			Type arg_type     = src->type;
			bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
			bool arg_is_int   = includes(arg_type, NUMERIC_TYPES, 0);
			if (param_is_int && arg_is_int && param_type != arg_type) {
				Token *as_tok          = new_token(AS, src->space);
				as_tok->retType        = param_type;
				Node *tgt_node         = new_node(new_token(param_type, src->space));
				Node *as_node          = new_node(as_tok);
				as_node->left          = carg;
				as_node->right         = tgt_node;
				call_args->children[i] = as_node;
			} else if (arg_type != 0 && !compatible(dec_args->children[i]->token, src)) {
				check(1, "'%s' argument %d: cannot pass %s as %s — use 'value as %s'",
				      node->token->name, i + 1, to_string(arg_type), to_string(param_type),
				      to_string(param_type));
				break;
			}
		}
	}
	free_node(func);
	set_ret_type(node);
}

static void ir_dot(Node *node) {
	gen_ir(node->left);
	if (found_error) return;
	node->left->token->used++;
	// Also mark the struct definition itself as used
	if (node->left->token->Struct.ptr) node->left->token->Struct.ptr->token->used++;
	Type  retType = 0;

	Node *src = NULL;
	if (node->left->token->type == STRUCT_CALL)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else if (node->left->token->retType == STRUCT_CALL && node->left->token->Struct.ptr)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else {
		check(1, "handle this case %s", to_string(node->left->token->type));
		return;
	}

	bool found = false;
	for (int i = 0; src && i < src->children_count; i++) {
		Node *src_child = src->children[i];
		if (strcmp(src_child->token->name, node->right->token->name) == 0) {
			found                            = true;
			retType                          = src_child->token->type;
			node->right->token->Struct.index = i;
			node->right->token->type         = src_child->token->type;
			node->right->token->retType      = src_child->token->retType;
			if (src_child->token->type == STRUCT_CALL)
				node->right->token->Struct.ptr = src_child->token->Struct.ptr;
			else if (src_child->token->type == ARRAY_TYPE) {
				node->right->token->Array.elem_type  = src_child->token->Array.elem_type;
				node->right->token->Array.depth      = src_child->token->Array.depth;
				node->right->token->Array.struct_ptr = src_child->token->Array.struct_ptr;
			}
			break;
		}
	}
	if (check(!found, "")) {
		debug(RED("%k doesn't have attribute %k\n"), node->left->token, node->right->token);
		return;
	}

	node->token->retType = retType;
	if (retType == STRUCT_CALL)     node->token->Struct.ptr = node->right->token->Struct.ptr;
	else if (retType == ARRAY_TYPE) {
		node->token->Array.elem_type  = node->right->token->Array.elem_type;
		node->token->Array.depth      = node->right->token->Array.depth;
		node->token->Array.struct_ptr = node->right->token->Array.struct_ptr;
	}
}

// ----------------------------------------------------------------------------
// Semantic analysis / name resolution pass
// ----------------------------------------------------------------------------

Node *find_function(char *name); // forward decl — defined after gen_ir

void  gen_ir(Node *node) {
   if (found_error) return;
   Node *left  = node->left;
   Node *right = node->right;
   switch (node->token->type) {

   case ID: {
      Token *find = get_variable(node->token->name);
      if (find) node->token = find;
      break;
   }

   case STRUCT_CALL: {
      Node *src               = get_struct(node->token->Struct.ptr->token->name);
      node->token->Struct.ptr = src;
      if (node->token->is_dec) {
         if (sccount == 1) {
            node->token->is_global = true;
         } else src->token->used++; // local declaration → always emit struct def
         new_variable(node->token);
      }
      break;
   }

   case NULLABLE: {
      node->token->retType = CHARS; // null is a null chars (i8*) pointer
      node->token->used++;
      break;
   }
   case INT:   case BOOL: case CHAR: case ARRAY_TYPE: case FLOAT: case LONG:
   case CHARS: case PTR: case VOID: case SHORT: {
      if (node->token->is_dec) {
         if (sccount == 1) {
            node->token->is_global = true;
         }
         new_variable(node->token);
      } else node->token->used++;
      break;
   }
   case TUPLE_UNPACK: {
      // Register each LHS variable, then process RHS
      for (int i = 0; i < node->children_count; i++) {
         Node *lhs = node->children[i];
         if (lhs->token->is_dec) {
            if (sccount == 1) lhs->token->is_global = true;
            new_variable(lhs->token);
         }
      }
      gen_ir(node->left); // RHS function call
      break;
   }
   case ASSIGN: {
      // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);
      // Struct operator overload: check for StructName.operator.ASSIGN.PARAMTYPE
      Type lt = left->token->retType ? left->token->retType : left->token->type;
      if (lt == STRUCT_CALL && left->token->Struct.ptr) {
         Type  rt           = right->token->retType ? right->token->retType : right->token->type;
         char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
			                          ? right->token->Struct.ptr->token->name
			                          : to_string(rt);
         char *op_base =
             strjoin(left->token->Struct.ptr->token->name, ".operator.", to_string(ASSIGN));
         char *op_name = strjoin(op_base, ".", param_suffix);
         free(op_base);
         Node *func = find_function(op_name);
         free(op_name);
         if (func) {
            node->token->Fcall.ptr = func;
            func->token->used++;
            break;
         }
      }
      if (!compatible(left->token, right->token)) {
         check(1, "type mismatch: cannot assign %k to %k — use '%k as %s'", right->token,
			       left->token, right->token, to_string(left->token->type));
         break;
      }
      node->token->retType = left->token->retType;
      break;
   }
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
   case MOD_ASSIGN: {
      gen_ir(left);
      gen_ir(right);
      node->token->retType = left->token->retType;
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      // Struct compound-assign operator overload dispatch
      {
         Type lt = left->token->retType ? left->token->retType : left->token->type;
         if (lt == STRUCT_CALL && left->token->Struct.ptr) {
            Type  rt = right->token->retType ? right->token->retType : right->token->type;
            char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
				                          ? right->token->Struct.ptr->token->name
				                          : to_string(rt);
            char *op_base      = strjoin(left->token->Struct.ptr->token->name, ".operator.",
				                              to_string(node->token->type));
            char *op_name      = strjoin(op_base, ".", param_suffix);
            free(op_base);
            Node *func = find_function(op_name);
            free(op_name);
            if (func) {
               node->token->Fcall.ptr = func;
               func->token->used++;
               node->token->retType = func->token->retType;
            }
         }
      }
      break;
   }
   case NOT: case BNOT: {
      // TODO: left must be boolean
      gen_ir(left);
      node->token->used++;
      node->left->token->used++;
      if (node->token->type == NOT) node->token->retType = BOOL;
      else node->token->retType = left->token->retType ? left->token->retType : left->token->type;
      break;
   }
   case ADD:    case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
   case LESS:   case GREAT: case LESS_EQUAL: case GREAT_EQUAL: case MOD:
   case AND:    case OR: case BAND: case BOR: case BXOR: case LSHIFT:
   case RSHIFT: {
      // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);
      check(!left || !left->token, "error in assignment, left is NULL");
      check(!right || !right->token, "error in assignment, right is NULL");
      if (includes(node->token->type, ADD, SUB, MUL, DIV, MOD, 0))
         node->token->retType = left->token->retType;
      else if (includes(node->token->type, COMPARISON_OPS, AND, OR, 0)) node->token->retType = BOOL;
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      set_ret_type(node);
      // Struct binary operator overload dispatch
      {
         Type lt = left->token->retType ? left->token->retType : left->token->type;
         if (lt == STRUCT_CALL && left->token->Struct.ptr) {
            Type  rt = right->token->retType ? right->token->retType : right->token->type;
            char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
				                          ? right->token->Struct.ptr->token->name
				                          : to_string(rt);
            char *op_base      = strjoin(left->token->Struct.ptr->token->name, ".operator.",
				                              to_string(node->token->type));
            char *op_name      = strjoin(op_base, ".", param_suffix);
            free(op_base);
            Node *func = find_function(op_name);
            free(op_name);
            if (func) {
               node->token->Fcall.ptr = func;
               func->token->used++;
               if (func->token->retType == STRUCT_CALL) {
                  node->token->retType    = STRUCT_CALL;
                  node->token->Struct.ptr = func->token->Struct.ptr;
               } else {
                  node->token->retType = func->token->retType;
               }
            }
         }
      }
      break;
   }
   case WHILE: {
      enter_scope(node);
      gen_ir(node->left); // condition
      node->left->token->used++;
      for (int i = 0; i < node->children_count; i++)
         gen_ir(node->children[i]);
      exit_scope();
      node->token->used++;
      break;
   }
   case IF: {
      enter_scope(node);
      Node *curr = node;
      while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
         if (includes(curr->token->type, IF, ELIF, 0)) {
            check(curr->left == NULL, "error");
            gen_ir(curr->left); // condition
            curr->left->token->used++;
         }
         for (int i = 0; i < curr->children_count; i++)
            gen_ir(curr->children[i]);
         curr = curr->right;
      }
      exit_scope();
      node->token->used++;
      break;
   }
   case FDEC: {
      if (!find_function(node->token->name)) new_function(node);
      enter_scope(node);
      Node **params = (node->left ? node->left->children : NULL);
      for (int i = 0; params && i < node->left->children_count && !found_error; i++) {
         gen_ir(params[i]);
         params[i]->token->used++;
      }
      for (int i = 0; !node->token->is_proto && i < node->children_count; i++)
         gen_ir(node->children[i]);
      exit_scope();
      break;
   }
   case FCALL: {
      if (node->token->is_static_call) {
         Node *func = get_function(node->token->name);
         if (check(!func, "no pub fn '%s'", node->token->name)) break;
         if (check(!func->token->is_pub, "'%s' is not a pub fn — use '.' for instance calls",
			           node->token->name))
            break;
         func->token->used++;
         node->token->Fcall.ptr = func;
         if (func->token->retType == STRUCT_CALL) {
            node->token->retType    = STRUCT_CALL;
            node->token->Struct.ptr = func->token->Struct.ptr;
         } else {
            node->token->retType = func->token->retType;
         }
         for (int i = 0; i < node->left->children_count; i++)
            gen_ir(node->left->children[i]);
      } else if (node->token->is_method_call && node->left) {
         ir_method_call(node);
      } else {
         ir_regular_call(node);
      }
      break;
   }
   case AS: {
      gen_ir(node->left);
      node->token->retType = node->right->token->type;
      break;
   }
   case DOT: {
      ir_dot(node);
      break;
   }
   case ACCESS: {
      gen_ir(node->left);
      gen_ir(node->right);
      if (found_error) break;
      node->right->token->used++;
      Type retType = 0;
      Type type = node->left->token->retType ? node->left->token->retType : node->left->token->type;
      int  depth = node->left->token->Array.depth;
      switch (type) {
      case CHARS: retType = CHAR; break;
      case STACK: case HEAP: case ARRAY_TYPE: case ARRAY:
         if (depth > 1) {
            // still multi-dim: return sub-array type, depth-1
            retType                       = ARRAY;
            node->token->Array.elem_type  = node->left->token->Array.elem_type;
            node->token->Array.depth      = depth - 1;
            node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
         } else {
            retType = node->left->token->Array.elem_type;
            if (retType == STRUCT_CALL)
               node->token->Struct.ptr = node->left->token->Array.struct_ptr;
         }
         break;
      case STRUCT_CALL: {
         // __list_* struct: transform v[i] → v.data[i]
         Node *st = node->left->token->Struct.ptr;
         if (st && strncmp(st->token->name, "__list_", 7) == 0) {
            // Build DOT node for .data (field 0) manually — no ir_dot call
            Token *dot_tok = new_token(DOT, node->left->token->space);
            Node  *dot     = new_node(dot_tok);
            dot->left      = node->left;
            // Set up right side (field token)
            Token *field_tok        = copy_token(st->children[0]->token); // data field
            field_tok->Struct.index = 0;
            dot->right              = new_node(field_tok);
            // Set DOT token metadata from the data field
            dot->token->retType          = st->children[0]->token->retType;
            dot->token->type             = DOT;
            dot->token->Array.elem_type  = st->children[0]->token->Array.elem_type;
            dot->token->Array.depth      = st->children[0]->token->Array.depth;
            dot->token->Array.struct_ptr = st->children[0]->token->Array.struct_ptr;
            node->left->token->used++;
            node->left = dot;
            // Resolve ACCESS return type from the data field's array type
            type  = dot->token->retType ? dot->token->retType : dot->token->type;
            depth = dot->token->Array.depth;
            if (includes(type, ARRAY_TYPE, ARRAY, 0)) {
               if (depth > 1) {
                  retType                       = ARRAY;
                  node->token->Array.elem_type  = dot->token->Array.elem_type;
                  node->token->Array.depth      = depth - 1;
                  node->token->Array.struct_ptr = dot->token->Array.struct_ptr;
               } else {
                  retType = dot->token->Array.elem_type;
                  if (retType == STRUCT_CALL)
                     node->token->Struct.ptr = dot->token->Array.struct_ptr;
               }
            }
         } else {
            check(1, "struct '%s' does not support [] access", st ? st->token->name : "(null)");
         }
         break;
      }
      default: check(1, "handle this case %s", to_string(node->left->token->type)); break;
      }
      node->token->retType = retType;
      break;
   }
   case RETURN: {
      if (node->children_count > 0) {
         // Tuple return: process each child expression
         for (int i = 0; i < node->children_count; i++)
            gen_ir(node->children[i]);
         node->token->retType = TUPLE;
         break;
      }
      gen_ir(node->left);
      node->token->retType = node->left->token->type;
      if (node->left->token->type == STRUCT_CALL)
         node->token->Struct.ptr = node->left->token->Struct.ptr;
      node->left->token->used++;
      break;
   }

   case OUTPUT: {
      for (int i = 0; i < node->left->children_count; i++) {
         gen_ir(node->left->children[i]);
         node->left->children[i]
             ->token->used++; // keep globals/vars alive (mirrors FCALL arg handling)
      }
      // output() compiles to printf internally — codegen declares printf as needed
      node->token->retType = VOID;
      break;
   }
   case TYPEOF: case SIZEOF: {
      // If the argument is an unresolved ID, check if it's a struct name
      if (node->left && node->left->token->type == ID && node->left->token->name) {
         Node *st = get_struct(node->left->token->name);
         if (st) {
            node->left->token->type       = STRUCT_CALL;
            node->left->token->Struct.ptr = st;
         }
      }
      gen_ir(node->left);
      Token *type_tok = node->left->token;
      Type   type     = type_tok->type ? type_tok->type : type_tok->retType;
      if (node->token->type == TYPEOF) {
         node->token->retType     = CHARS;
         node->token->Chars.value = strdup(to_string(type));
      } else {
         node->token->retType   = INT;
         node->token->Int.value = 0; // placeholder
      }
      node->token->used++;
      break;
   }
   case STACK: case HEAP: {
      for (int i = 0; i < node->children_count; i++) {
         gen_ir(node->children[i]);
         node->children[i]->token->used++;
      }
      node->token->used++;
      break;
   }
   case MODULE: {
      char *mname        = node->token->name;
      char *saved        = current_gen_module;
      current_gen_module = mname;
      for (int i = 0; i < node->functions_count; i++) {
         Node *fn    = node->functions[i];
         char *qname = strjoin(mname, ".", fn->token->name);
         setName(fn->token, qname);
         free(qname);
      }
      for (int i = 0; i < node->functions_count; i++)
         gen_ir(node->functions[i]);
      current_gen_module = saved;
      break;
   }
   case STRUCT_DEF: {
      // Pre-register all methods so they can reference each other
      for (int i = 0; node && i < node->functions_count; i++) {
         Node *fn = node->functions[i];
         if (!find_function(fn->token->name)) add_function(scope, fn);
      }
      for (int i = 0; node && i < node->functions_count; i++)
         gen_ir(node->functions[i]);
      break;
   }
   case ENUM_DEF: {
      for (int i = 0; i < node->children_count; i++)
         node->children[i]->token->used++;
      break;
   }
   case ARRAY_LIT: {
      for (int i = 0; i < node->children_count; i++)
         gen_ir(node->children[i]);
      if (node->children_count > 0) {
         Token *first             = node->children[0]->token;
         Type   ft                = first->retType ? first->retType : first->type;
         node->token->retType     = ARRAY;
         node->token->Array.depth = 1;
         if (includes(ft, ARRAY_LIT, ARRAY, ARRAY_TYPE, 0)) {
            node->token->Array.elem_type  = first->Array.elem_type;
            node->token->Array.struct_ptr = first->Array.struct_ptr;
            node->token->Array.depth      = first->Array.depth + 1;
         } else {
            node->token->Array.elem_type = ft;
            if (ft == STRUCT_CALL) node->token->Array.struct_ptr = first->Struct.ptr;
         }
      }
      break;
   }
   // ref expr — address-of
   case REF: {
      gen_ir(node->left);
      node->token->is_ref = true;
      Type lt = node->left->token->retType ? node->left->token->retType : node->left->token->type;
      node->token->retType = lt;
      break;
   }
   // [BREAK]    [CONTINUE]
   case BREAK:
   case CONTINUE: break;
   default:       {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
}

void enter_scope(Node *node) {
	sccount++;
	debug(CYAN("Enter Scope: %k index %d\n"), node->token, sccount);
	resize_array(level_scope, Node *, scsize, sccount);
	level_scope[sccount] = node;
	scope                = level_scope[sccount];
}

void exit_scope() {
	if (check(sccount < 0, "No active scope to exit")) return;
	debug(CYAN("Exit Scope: %k index %d\n"), level_scope[sccount]->token, sccount);
	level_scope[sccount] = NULL;
	sccount--;
	scope = level_scope[sccount];
}

Token *find(Type type, ...) {
	if (found_error) return NULL;
	va_list ap;
	va_start(ap, type);
	while (type) {
		if (type == tokens[ecount]->type) return tokens[ecount++];
		type = va_arg(ap, Type);
	}
	return NULL;
};

bool within(int space) {
	Token *curr = tokens[ecount];
	return !found_error && curr->space > space && curr->type != END;
}

Node *get_struct(char *name) {
	debug(CYAN("get struct [%s] from scope %k\n"), name, scope->token);
	for (int j = sccount; j > 0; j--) {
		Node *node = level_scope[j];
		todo(node == NULL, RED("Error accessing NULL, %d"), j);
		for (int i = 0; i < node->structs_count; i++)
			if (strcmp(node->structs[i]->token->name, name) == 0) return node->structs[i];
	}
	return NULL;
}

// IR / SCOPE / VARIABLE MANAGEMENT

Token *get_variable(char *name) {
	debug(CYAN("get variable [%s] from scope %k, has %d vars\n"), name, scope->token,
	      scope->variables_count);
	for (int j = sccount; j > 0; j--) {
		Node *scope = level_scope[j];
		for (int i = 0; i < scope->variables_count; i++) {
			check(scope->variables[i] == NULL, "unexpected error variables");
			check(scope->variables[i]->name == NULL, "unexpected error name");
			if (strcmp(scope->variables[i]->name, name) == 0) return scope->variables[i];
		}
	}
	check(1, "%s not found", name);
	// seg();
	return syntax_error()->token;
}

void add_function(Node *parent, Node *node) {
	resize_array(parent->functions, Node *, parent->functions_size, parent->functions_count);
	parent->functions[parent->functions_count++] = node;
}

Node *new_function(Node *node) {
	for (int i = 0; i < scope->functions_count; i++) {
		Node *func = scope->functions[i];
		bool  cond = strcmp(func->token->name, node->token->name) == 0;
		check(cond, "Redefinition of %s", node->token->name);
	}
	add_function(scope, node);
	return node;
}

// Returns the function node or NULL (no error) — for optional overload probing
Node *find_function(char *name) {
	for (int j = sccount; j > 0; j--) {
		Node *sc = level_scope[j];
		for (int i = 0; i < sc->functions_count; i++)
			if (strcmp(sc->functions[i]->token->name, name) == 0) return sc->functions[i];
	}
	if (current_gen_module) {
		char *qname = strjoin(current_gen_module, ".", name);
		for (int j = sccount; j > 0; j--) {
			Node *sc = level_scope[j];
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
	check(1, "'%s' Not found", name);
	return syntax_error();
}

// DEBUGGING AND ERROR HANDLING

// STRING

// OTHER UTILITIES

bool compatible(Token *left, Token *right) {
	Type lt = left->retType ? left->retType : left->type;
	Type rt = right->retType ? right->retType : right->type;

	if (lt == rt) return true;

	if ((lt == CHARS && rt == PTR) || (lt == PTR && rt == CHARS)) return true;

	if (lt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && right->Array.elem_type == CHAR)
		return true;
	if (rt == CHARS && includes(lt, ARRAY_TYPE, ARRAY, 0) && left->Array.elem_type == CHAR)
		return true;

	if (lt == ARRAY_TYPE && rt == ARRAY) return true;
	if (lt == ARRAY && rt == ARRAY_TYPE) return true;

	// pointer/chars ↔ array[T]: all pointers at LLVM level
	if ((includes(lt, PTR, CHARS, 0) && includes(rt, ARRAY_TYPE, ARRAY, 0)) ||
	    (includes(rt, PTR, CHARS, 0) && includes(lt, ARRAY_TYPE, ARRAY, 0)))
		return true;

	bool lt_numeric = includes(lt, NUMERIC_TYPES, 0);
	bool rt_numeric = includes(rt, NUMERIC_TYPES, 0);
	if (lt_numeric && rt_numeric && lt == rt) return true;

	if (lt == VOID || rt == VOID) return false;

	if (lt == STRUCT_CALL && rt == STRUCT_CALL) return left->Struct.ptr == right->Struct.ptr;
	return false;
}

Type get_ret_type(Node *node) {
	if (!node || !node->token) return 0;

	Token *token = node->token;
	Node  *left  = node->left;
	Node  *right = node->right;

	if (token->retType) return token->retType;

	// Scalar types and struct return themselves
	if (includes(token->type, INT, BOOL, CHAR, FLOAT, LONG, VOID, PTR, CHARS, STRUCT_CALL, 0))
		return token->type;
	if (token->type == FCALL) return token->Fcall.ptr ? token->Fcall.ptr->token->retType : 0;
	// Arithmetic, bitwise, and assign ops inherit the left operand type
	if (includes(token->type, MATH_TYPE, ASSIGN, ASSIGNS_OP, 0)) return get_ret_type(left);
	// Comparison and logical ops always yield bool
	if (includes(token->type, COMPARISON_OPS, AND, OR, NOT, BNOT, 0)) return BOOL;
	if (token->type == AS)
		return right ? (right->token->retType ? right->token->retType : right->token->type) : 0;
	if (token->type == RETURN) return get_ret_type(left);
	if (token->type == DOT)    return get_ret_type(right);
	if (token->type == ACCESS) {
		Type left_type = get_ret_type(left);
		if (left_type == CHARS || left_type == STACK) return CHAR;
		return left_type;
	}
	if (token->type == ID) return token->type != ID ? token->type : 0;
	todo(1, "handled this case [%s]", to_string(token->type));
	return 0;
}

void set_ret_type(Node *node) {
	if (!node || !node->token) return;
	Type t = get_ret_type(node);
	if (t) node->token->retType = t;

	if (node->token->type == DOT && node->token->retType == STRUCT_CALL && node->right)
		node->token->Struct.ptr = node->right->token->Struct.ptr;

	// For FCALL returning struct, copy Struct.ptr from the function declaration
	if (node->token->type == FCALL && node->token->retType == STRUCT_CALL && node->token->Fcall.ptr)
		node->token->Struct.ptr = node->token->Fcall.ptr->token->Struct.ptr;
}

// ----------------------------------------------------------------------------
// LLVM higher-level helpers (from llvm.c)
// ----------------------------------------------------------------------------
Value allocate_stack(Value size, TypeRef elementType, char *name) {
	Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, 0, 0)};

	if (LLVMIsConstant(size)) {
		unsigned long long constSize    = LLVMConstIntGetZExtValue(size);
		TypeRef            arrayType    = LLVMArrayType(elementType, constSize);
		Value              array_alloca = LLVMBuildAlloca(builder, arrayType, name);
		return LLVMBuildGEP2(builder, arrayType, array_alloca, indices, 2, name);
	}

	Value array_alloca = LLVMBuildArrayAlloca(builder, elementType, size, name);
	return LLVMBuildGEP2(builder, elementType, array_alloca, indices, 2, name);
}

Value allocate_heap(Value count, TypeRef elementType, char *name) {
	Value calloc_func = LLVMGetNamedFunction(module, "calloc");
	if (!calloc_func) {
		TypeRef params[]  = {i64, i64};
		TypeRef func_type = LLVMFunctionType(p8, params, 2, 0);
		calloc_func       = _add_function("calloc", func_type);
	}
	TargetData td        = LLVMGetModuleDataLayout(module);
	size_t     elem_size = LLVMABISizeOfType(td, elementType);

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
	default: check(1, "handle this case %s", to_string(token->type)); return;
	}
	token->llvm.elem = LLVMConstInt(type, value, 0);
}

Value _build_return(Token *token) {
	if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) {
		Value value = token->llvm.elem;
		if (token->type != VOID) {
			debug("build return %k\n", token);
			return LLVMBuildRet(builder, value);
		} else return LLVMBuildRetVoid(builder);
	}
	return NULL;
}

// ----------------------------------------------------------------------------
// ASM GENERATION
// ----------------------------------------------------------------------------
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
	char  dir[1024]     = ".";
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
}

void finalize(char *output) {
	char *error = NULL;

	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();

	LLVMPassBuilderOptionsRef options = LLVMCreatePassBuilderOptions();

	if (op_flags) {
		LLVMErrorRef err = LLVMRunPasses(module, op_flags, NULL, options);
		if (err) {
			char *msg = LLVMGetErrorMessage(err);
			check(1, "Optimizer Error: %s\n", msg);
			LLVMDisposeErrorMessage(msg);
			found_error = true;
			return;
		}
	}

	LLVMDIBuilderFinalize(debug_builder);
	LLVMDisposeDIBuilder(debug_builder);
	debug_builder = NULL;

	if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
		check(1, "Module verification failed:\n%s\n", error);
		LLVMDisposeMessage(error);
		LLVMDisposePassBuilderOptions(options);
		found_error = true;
		// return;
	} else {
		LLVMDisposePassBuilderOptions(options);
	}

	LLVMPrintModuleToFile(module, output, NULL);
	LLVMDisposeBuilder(builder);
	LLVMDisposeModule(module);
	LLVMContextDispose(context);
}

void emit_scope_clean(Node *scope, int from, Token *skip) {
	for (int i = from; i < scope->variables_count; i++) {
		Token *var = scope->variables[i];
		if (var == skip)     continue;
		if (!var->llvm.elem) continue;
		if (var->is_ref)     continue;

		if (var->type == STRUCT_CALL) {
			Node *sd       = var->Struct.ptr;
			char *qname    = strjoin(sd->token->name, ".delete", NULL);
			Value clean_fn = LLVMGetNamedFunction(module, qname);
			free(qname);
			if (!clean_fn) continue;
			Value args[] = {var->llvm.elem};
			LLVMBuildCall2(builder, LLVMGlobalGetValueType(clean_fn), clean_fn, args, 1, "");
		}
	}
	// Auto-free temp structs created in this scope
	for (int i = 0; i < scope->temps_count; i++) {
		Node *sd    = scope->temp_defs[i];
		char *qname = strjoin(sd->token->name, ".delete", NULL);
		Value fn    = LLVMGetNamedFunction(module, qname);
		free(qname);
		if (!fn) continue;
		Value args[] = {scope->temps[i]};
		LLVMBuildCall2(builder, LLVMGlobalGetValueType(fn), fn, args, 1, "");
	}
	scope->temps_count = 0;
}

void append_output_arg(Token *tok, char *fmt, int *functions_count, Value *args, int *nargs) {
	Type type = tok->type ? tok->type : tok->retType;

	// String literal: embed directly in the format string (escape % → %%)
	if (tok->type == CHARS && !tok->name) {
		const char *s = tok->Chars.value;
		for (int i = 0; s[i]; i++) {
			if (s[i] == '%') {
				fmt[(*functions_count)++] = '%'; // double % for printf
				fmt[(*functions_count)++] = '%';
			} else if (s[i] == '\\' && s[i + 1]) {
				switch (s[++i]) {
				case 'n':  fmt[(*functions_count)++] = '\n'; break;
				case 't':  fmt[(*functions_count)++] = '\t'; break;
				case 'r':  fmt[(*functions_count)++] = '\r'; break;
				case '\\': fmt[(*functions_count)++] = '\\'; break;
				case '"':  fmt[(*functions_count)++] = '"'; break;
				default:
					fmt[(*functions_count)++] = '\\';
					fmt[(*functions_count)++] = s[i];
					break;
				}
			} else fmt[(*functions_count)++] = s[i];
		}
		return;
	}

	// For expression nodes (DOT, FCALL, etc.), resolve to actual value type
	switch (type) {
	case INT: case LONG: case SHORT: case CHAR: case CHARS: case BOOL:
	case FLOAT:
	case STRUCT_CALL: break;
	case FCALL:       type = tok->retType; break;
	default:
		if (tok->retType) type = tok->retType;
		break;
	}

	switch (type) {
	case INT: case SHORT:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'd';
		args[(*nargs)++]          = load_value(tok);
		break;
	case BOOL: {
		Value bool_val            = load_value(tok);
		Value true_str            = LLVMBuildGlobalStringPtr(builder, "True", "true_str");
		Value false_str           = LLVMBuildGlobalStringPtr(builder, "False", "false_str");
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 's';
		args[(*nargs)++] = LLVMBuildSelect(builder, bool_val, true_str, false_str, "bool_str");
		break;
	}
	case LONG:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'l';
		fmt[(*functions_count)++] = 'l';
		fmt[(*functions_count)++] = 'd';
		args[(*nargs)++]          = load_value(tok);
		break;
	case CHAR:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'c';
		args[(*nargs)++]          = load_value(tok);
		break;
	case CHARS:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 's';
		args[(*nargs)++]          = load_value(tok);
		break;
	case FLOAT:
		fmt[(*functions_count)++] = '%';
		fmt[(*functions_count)++] = 'f';
		args[(*nargs)++] =
		    LLVMBuildFPExt(builder, load_value(tok), LLVMDoubleTypeInContext(context), "f2d");
		break;
	case STRUCT_CALL: {
		Node *sd       = tok->Struct.ptr;
		char *out_name = strjoin(sd->token->name, ".operator.output", NULL);
		Value out_fn   = LLVMGetNamedFunction(module, out_name);
		free(out_name);
		if (out_fn) {
			Value   self_ptr = tok->llvm.elem;
			TypeRef fn_type  = LLVMGlobalGetValueType(out_fn);
			Value   result   = LLVMBuildCall2(builder, fn_type, out_fn, &self_ptr, 1, "output_op");
			TypeRef ret_type = LLVMGetReturnType(fn_type);
			if (LLVMGetTypeKind(ret_type) == PointerType) {
				// Returns chars — use directly as %s
				fmt[(*functions_count)++] = '%';
				fmt[(*functions_count)++] = 's';
				args[(*nargs)++]          = result;
			} else if (LLVMGetTypeKind(ret_type) == StructType) {
				// Returns a struct (e.g. String) — spill to alloca and recurse
				Value tmp = LLVMBuildAlloca(builder, ret_type, "out_tmp");
				LLVMBuildStore(builder, result, tmp);
				// Find the struct def for the returned type by checking struct fields
				// The returned struct should have its own operator output
				// Look up by iterating known structs to find one matching ret_type
				Node *ret_sd = NULL;
				for (int si = sccount; si > 0 && !ret_sd; si--)
					for (int sj = 0; sj < level_scope[si]->structs_count; sj++)
						if (level_scope[si]->structs[sj]->token->llvm.stType == ret_type) {
							ret_sd = level_scope[si]->structs[sj];
							break;
						}
				if (ret_sd) {
					// Register temp for auto-cleanup
					if (scope->temps == NULL || scope->temps_count + 5 >= scope->temps_size) {
						int    new_size = scope->temps == NULL ? 10 : scope->temps_size * 2;
						Value *nt       = allocate(new_size, sizeof(Value));
						Node **nd       = allocate(new_size, sizeof(Node *));
						if (scope->temps) {
							memcpy(nt, scope->temps, scope->temps_count * sizeof(Value));
							memcpy(nd, scope->temp_defs, scope->temps_count * sizeof(Node *));
							free(scope->temps);
							free(scope->temp_defs);
						}
						scope->temps      = nt;
						scope->temp_defs  = nd;
						scope->temps_size = new_size;
					}
					scope->temps[scope->temps_count]     = tmp;
					scope->temp_defs[scope->temps_count] = ret_sd;
					scope->temps_count++;
					Token ftok      = *tok;
					ftok.Struct.ptr = ret_sd;
					ftok.llvm.elem  = tmp;
					ftok.type       = STRUCT_CALL;
					append_output_arg(&ftok, fmt, functions_count, args, nargs);
				}
			}
			break;
		}
		// Default: { field: value }
		fmt[(*functions_count)++] = '{';
		fmt[(*functions_count)++] = ' ';
		for (int i = 0; i < sd->children_count; i++) {
			Token *field          = sd->children[i]->token;
			int    functions_size = strlen(field->name);
			memcpy(fmt + *functions_count, field->name, functions_size);
			*functions_count += functions_size;
			fmt[(*functions_count)++] = ':';
			fmt[(*functions_count)++] = ' ';
			Token ftok                = *field;
			ftok.llvm.elem            = struct_field_ptr(tok, i, field->name);
			append_output_arg(&ftok, fmt, functions_count, args, nargs);
			if (i < sd->children_count - 1) {
				fmt[(*functions_count)++] = ',';
				fmt[(*functions_count)++] = ' ';
			}
		}
		fmt[(*functions_count)++] = ' ';
		fmt[(*functions_count)++] = '}';
		break;
	}
	default: fmt[(*functions_count)++] = '?'; break;
	}
}

void gen_asm(Node *node) {
	set_debug_location(node->token);
	Node *left  = node->left;
	Node *right = node->right;

	if (check(node->token->llvm.is_set, "already set")) return;
	switch (node->token->type) {
	case STRUCT_CALL: {
		if (node->token->is_dec) {
			if (node->token->is_global) {
				if (node->token->used == 0) {
					node->token->is_dec = false;
					return;
				}
				TypeRef type           = get_llvm_type(node->token);
				node->token->llvm.elem = LLVMAddGlobal(module, type, node->token->name);
				LLVMSetInitializer(node->token->llvm.elem, LLVMConstNull(type));
			} else if (node->token->is_ref) {
				// TypeRef type = get_llvm_type(node->token);
				// Value   null = LLVMConstNull(LLVMPointerType(type, 0));
				// LLVMBuildStore(builder, null, node->token->llvm.elem);
			} else {
				_alloca(node->token);
				TypeRef struct_type = get_llvm_type(node->token);
				LLVMBuildStore(builder, LLVMConstNull(struct_type), node->token->llvm.elem);
			}
			node->token->is_dec = false;
			return;
		}
		break;
	}
	case NULLABLE: {
		node->token->llvm.elem = LLVMConstNull(p8);
		break;
	}
	case INT:        case LONG: case SHORT: case CHARS: case CHAR: case BOOL:
	case ARRAY_TYPE: case FLOAT: {
		if (node->token->is_dec) {
			if (node->token->is_global) {
				if (node->token->used == 0) {
					node->token->is_dec = false;
					return;
				}
				TypeRef type           = get_llvm_type(node->token);
				node->token->llvm.elem = LLVMAddGlobal(module, type, node->token->name);
				LLVMSetInitializer(node->token->llvm.elem, _get_default_value(node->token));
			} else {
				_alloca(node->token);
				Value defval = _get_default_value(node->token);
				LLVMBuildStore(builder, defval, node->token->llvm.elem);
			}
			node->token->is_dec = false;
			return;
		} else if (node->token->name) return;
		_const_value(node->token);
		break;
	}
	case ASSIGN: {
		// Struct operator overload dispatch
		if (node->token->Fcall.ptr) {
			Node *func    = node->token->Fcall.ptr;
			LLVM  srcFunc = func->token->llvm;
			if (check(!srcFunc.funcType, "operator: funcType NULL for '%s'", node->token->name)) break;
			if (check(!srcFunc.elem, "operator: elem NULL for '%s'", node->token->name))         break;
			gen_asm(left);
			gen_asm(right);
			// Mirror FCALL ref/non-ref logic for the first (rhs) param
			bool param_is_ref =
			    func->left->children_count >= 2 && func->left->children[0]->token->is_ref;
			bool  arg_is_ref = right->token->is_ref;
			Value rhs_val;
			if (param_is_ref && arg_is_ref) {
				TypeRef type = get_llvm_type(right->token);
				rhs_val = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), right->token->llvm.elem,
				                         "ref_arg");
			} else if (param_is_ref && !arg_is_ref) {
				// if already a pointer (alloca) pass directly, else spill return value to temp
				if (LLVMGetTypeKind(LLVMTypeOf(right->token->llvm.elem)) == LLVMPointerTypeKind) {
					rhs_val = right->token->llvm.elem;
				} else {
					TypeRef type = get_llvm_type(right->token);
					Value   tmp  = LLVMBuildAlloca(builder, type, "tmp_op");
					LLVMBuildStore(builder, right->token->llvm.elem, tmp);
					rhs_val = tmp;
				}
			} else {
				load_if_necessary(right);
				rhs_val = right->token->llvm.elem;
			}
			// args: [rhs, self]  (self is last param per parse_fdec convention)
			Value args[2] = {rhs_val, left->token->llvm.elem};
			node->token->llvm.elem =
			    LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.elem, args, 2, "");
			break;
		}
		if (left->token->is_global && left->token->is_dec) {
			gen_asm(left);
			gen_asm(right);
			if (left->token->llvm.elem && right->token->llvm.elem &&
			    LLVMIsConstant(right->token->llvm.elem))
				LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
			return;
		}
		// ref binding: a = ref b → store b's address into a
		if (left->token->is_ref && right->token->type == REF) {
			if (!left->token->llvm.elem) _alloca(left->token);
			gen_asm(right);
			LLVMBuildStore(builder, right->token->llvm.elem, left->token->llvm.elem);
			left->token->is_dec = false;
			break;
		}

		gen_asm(left);
		gen_asm(right);

		// propagate multi-dim info from stack/heap allocation to the variable
		if (includes(right->token->type, STACK, HEAP, 0) && right->token->llvm.dims_count > 1) {
			// Propagate to the variable token
			left->token->llvm.dims_count = 0;
			for (int i = 0; i < right->token->llvm.dims_count; i++) {
				resize_array(left->token->llvm.dims, Value, left->token->llvm.dims_size,
				             left->token->llvm.dims_count);
				left->token->llvm.dims[left->token->llvm.dims_count++] = right->token->llvm.dims[i];
			}
			// If assigning to a struct field (DOT), store dims on the field definition
			if (left->token->type == DOT && left->right) {
				Node *sd = left->left->token->Struct.ptr;
				if (sd) {
					int idx = left->right->token->Struct.index;
					if (idx >= 0 && idx < sd->children_count) {
						Token *field           = sd->children[idx]->token;
						field->llvm.dims_count = 0;
						for (int i = 0; i < right->token->llvm.dims_count; i++) {
							resize_array(field->llvm.dims, Value, field->llvm.dims_size,
							             field->llvm.dims_count);
							field->llvm.dims[field->llvm.dims_count++] = right->token->llvm.dims[i];
						}
					}
				}
			}
		}

		TypeRef type = get_llvm_type(right->token);

		Value   val;
		if (right->token->is_ref) val = load_value(right->token);
		else {
			load_if_necessary(right);
			val = right->token->llvm.elem;
		}

		// Bitcast heap/stack result to match struct field type
		if (includes(right->token->type, STACK, HEAP, 0) && left->token->type == DOT) {
			TypeRef field_type = get_llvm_type(left->token);
			val                = LLVMBuildBitCast(builder, val, field_type, "field_cast");
		}
		// Bitcast pointer (i8*) to array[T] (T*) when assigning
		if (val && LLVMGetTypeKind(LLVMTypeOf(val)) == PointerType) {
			TypeRef dest_type = get_llvm_type(left->token);
			if (dest_type && LLVMGetTypeKind(dest_type) == PointerType && LLVMTypeOf(val) != dest_type)
				val = LLVMBuildBitCast(builder, val, dest_type, "ptr_cast");
		}

		if (left->token->is_ref) {
			Value ptr =
			    LLVMBuildLoad2(builder, LLVMPointerType(type, 0), left->token->llvm.elem, "ref_ptr");
			LLVMBuildStore(builder, val, ptr);
		} else LLVMBuildStore(builder, val, left->token->llvm.elem);
		node->token->llvm.elem = val;

		break;
	}
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN: {
		// Struct compound-assign operator overload dispatch
		if (node->token->Fcall.ptr) {
			Node *func    = node->token->Fcall.ptr;
			LLVM  srcFunc = func->token->llvm;
			if (check(!srcFunc.funcType, "operator: funcType NULL for '%s'", node->token->name)) break;
			if (check(!srcFunc.elem, "operator: elem NULL for '%s'", node->token->name))         break;
			gen_asm(left);
			gen_asm(right);
			bool param_is_ref =
			    func->left->children_count >= 2 && func->left->children[0]->token->is_ref;
			bool  arg_is_ref = right->token->is_ref;
			Value rhs_val;
			if (param_is_ref && arg_is_ref) {
				TypeRef type = get_llvm_type(right->token);
				rhs_val = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), right->token->llvm.elem,
				                         "ref_arg");
			} else if (param_is_ref && !arg_is_ref) {
				if (LLVMGetTypeKind(LLVMTypeOf(right->token->llvm.elem)) == LLVMPointerTypeKind)
					rhs_val = right->token->llvm.elem;
				else {
					TypeRef type = get_llvm_type(right->token);
					Value   tmp  = LLVMBuildAlloca(builder, type, "tmp_op");
					LLVMBuildStore(builder, right->token->llvm.elem, tmp);
					rhs_val = tmp;
				}
			} else {
				load_if_necessary(right);
				rhs_val = right->token->llvm.elem;
			}
			Value args[2] = {rhs_val, left->token->llvm.elem};
			node->token->llvm.elem =
			    LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.elem, args, 2, "");
			break;
		}
		gen_asm(left);
		gen_asm(right);

		TypeRef type        = get_llvm_type(left->token);
		Value   current_val = load_value(left->token);

		Value   right_val;
		if (right->token->is_ref) right_val = load_value(right->token);
		else {
			load_if_necessary(right);
			right_val = right->token->llvm.elem;
		}

		Value result = build_binary_op(assign_base_op(node->token->type), current_val, right_val);

		if (left->token->is_ref) {
			Value ptr =
			    LLVMBuildLoad2(builder, LLVMPointerType(type, 0), left->token->llvm.elem, "ref_ptr");
			LLVMBuildStore(builder, result, ptr);
		} else LLVMBuildStore(builder, result, left->token->llvm.elem);
		node->token->llvm.elem = result;
		break;
	}
	case NOT: case BNOT: {
		gen_asm(left);
		load_if_necessary(left);
		if (left->token->is_ref) {
			left->token->llvm.elem      = load_value(left->token);
			left->token->llvm.is_loaded = true;
		}
		node->token->llvm.elem =
		    LLVMBuildNot(builder, left->token->llvm.elem, to_string(node->token->type));
		break;
	}
	case ADD:    case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
	case LESS:   case GREAT: case LESS_EQUAL: case GREAT_EQUAL: case MOD:
	case AND:    case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT: {
		// Struct binary operator overload dispatch
		if (node->token->Fcall.ptr) {
			Node *func    = node->token->Fcall.ptr;
			LLVM  srcFunc = func->token->llvm;
			if (check(!srcFunc.funcType, "operator: funcType NULL for '%s'", node->token->name)) break;
			if (check(!srcFunc.elem, "operator: elem NULL for '%s'", node->token->name))         break;
			gen_asm(left);
			gen_asm(right);
			// right = operand (first param), left = self (last param)
			bool param_is_ref =
			    func->left->children_count >= 2 && func->left->children[0]->token->is_ref;
			bool  arg_is_ref = right->token->is_ref;
			Value rhs_val;
			if (param_is_ref && arg_is_ref) {
				TypeRef type = get_llvm_type(right->token);
				rhs_val = LLVMBuildLoad2(builder, LLVMPointerType(type, 0), right->token->llvm.elem,
				                         "ref_arg");
			} else if (param_is_ref && !arg_is_ref) {
				if (LLVMGetTypeKind(LLVMTypeOf(right->token->llvm.elem)) == LLVMPointerTypeKind)
					rhs_val = right->token->llvm.elem;
				else {
					TypeRef type = get_llvm_type(right->token);
					Value   tmp  = LLVMBuildAlloca(builder, type, "tmp_op");
					LLVMBuildStore(builder, right->token->llvm.elem, tmp);
					rhs_val = tmp;
				}
			} else {
				// non-ref param: use copy-constructor if struct has operator=
				bool param_is_struct_v = func->left->children_count >= 2 &&
				                         func->left->children[0]->token->type == STRUCT_CALL;
				bool arg_is_struct =
				    right->token->type == STRUCT_CALL || right->token->retType == STRUCT_CALL;
				bool did_copy = false;
				if (param_is_struct_v && arg_is_struct) {
					Node *st_node = func->left->children[0]->token->Struct.ptr;
					char *cp_name =
					    strjoin(st_node->token->name, ".operator.ASSIGN.", st_node->token->name);
					Value copy_op = LLVMGetNamedFunction(module, cp_name);
					free(cp_name);
					if (copy_op) {
						TypeRef st_type = get_llvm_type(func->left->children[0]->token);
						Value   tmp     = LLVMBuildAlloca(builder, st_type, "copy");
						LLVMBuildStore(builder, LLVMConstNull(st_type), tmp);
						Value src_ptr;
						if (LLVMGetTypeKind(LLVMTypeOf(right->token->llvm.elem)) == LLVMPointerTypeKind)
							src_ptr = right->token->llvm.elem;
						else {
							Value spill = LLVMBuildAlloca(builder, st_type, "spill");
							LLVMBuildStore(builder, right->token->llvm.elem, spill);
							src_ptr = spill;
						}
						Value cargs[] = {src_ptr, tmp};
						LLVMBuildCall2(builder, LLVMGlobalGetValueType(copy_op), copy_op, cargs, 2, "");
						rhs_val  = LLVMBuildLoad2(builder, st_type, tmp, "copy_val");
						did_copy = true;
					}
				}
				if (!did_copy) {
					load_if_necessary(right);
					rhs_val = right->token->llvm.elem;
				}
			}
			Value args[2] = {rhs_val, left->token->llvm.elem};
			node->token->llvm.elem =
			    LLVMBuildCall2(builder, srcFunc.funcType, srcFunc.elem, args, 2, "");
			if (func->token->retType != STRUCT_CALL && func->token->retType != VOID)
				node->token->llvm.is_loaded = true;
			break;
		}
		gen_asm(left);
		gen_asm(right);

		load_if_necessary(left);
		load_if_necessary(right);

		Value lref = left->token->llvm.elem;
		Value rref = right->token->llvm.elem;

		if (left->token->is_ref && !left->token->llvm.is_loaded)   lref = load_value(left->token);
		if (right->token->is_ref && !right->token->llvm.is_loaded) rref = load_value(right->token);

		// Pointer arithmetic: chars/ptr + int → GEP instead of add
		if (node->token->type == ADD && LLVMGetTypeKind(LLVMTypeOf(lref)) == LLVMPointerTypeKind) {
			Value idx              = LLVMGetTypeKind(LLVMTypeOf(rref)) == LLVMIntegerTypeKind &&
                             LLVMGetIntTypeWidth(LLVMTypeOf(rref)) < 64
			                             ? LLVMBuildSExt(builder, rref, i64, "idx")
			                             : rref;
			node->token->llvm.elem = LLVMBuildGEP2(builder, i8, lref, &idx, 1, "ADD");
		} else {
			node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
		}
		if (includes(node->token->type, COMPARISON_OPS, 0)) node->token->retType = BOOL;
		else node->token->retType = left->token->retType ? left->token->retType : left->token->type;
		break;
	}
	case STACK: {
		Token   tmp    = {.type = node->token->Array.elem_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int     depth  = node->token->Array.depth;

		// evaluate each dimension, compute total element count, store dims
		Value total = LLVMConstInt(i32, 1, 0);
		for (int i = 0; i < depth; i++) {
			gen_asm(node->children[i]);
			load_if_necessary(node->children[i]);
			Value dv = node->children[i]->token->llvm.elem;
			resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
			             node->token->llvm.dims_count);
			node->token->llvm.dims[node->token->llvm.dims_count++] = dv;
			total = LLVMBuildMul(builder, total, dv, "dim");
		}

		TargetData td        = LLVMGetModuleDataLayout(module);
		size_t     elem_size = LLVMABISizeOfType(td, elem_t);
		Value      total_bytes =
		    LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_stack(total_bytes, elem_t, "stack");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case HEAP: {
		Token   tmp    = {.type = node->token->Array.elem_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int     depth  = node->token->Array.depth;

		Value   total = LLVMConstInt(i32, 1, 0);
		for (int i = 0; i < depth; i++) {
			gen_asm(node->children[i]);
			load_if_necessary(node->children[i]);
			Value dv = node->children[i]->token->llvm.elem;
			resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
			             node->token->llvm.dims_count);
			node->token->llvm.dims[node->token->llvm.dims_count++] = dv;
			total = LLVMBuildMul(builder, total, dv, "dim");
		}

		TargetData td        = LLVMGetModuleDataLayout(module);
		size_t     elem_size = LLVMABISizeOfType(td, elem_t);
		Value      total_bytes =
		    LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_heap(total_bytes, elem_t, "heap");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case ARRAY_LIT: {
		int count = node->children_count;
		// Resolve element LLVM type
		TypeRef elem_t;
		Type    et = node->token->Array.elem_type;
		if (et == STRUCT_CALL && node->token->Array.struct_ptr)
			elem_t = get_llvm_type(node->token->Array.struct_ptr->token);
		else {
			Token tmp = {.type = et};
			elem_t    = get_llvm_type(&tmp);
		}
		// For depth > 1 (nested arrays), element is a pointer
		for (int d = 1; d < node->token->Array.depth; d++)
			elem_t = LLVMPointerType(elem_t, 0);

		// Allocate: calloc(count, sizeof(elem))
		Value cnt      = LLVMConstInt(i32, count, 0);
		Value raw_data = allocate_heap(cnt, elem_t, "array_lit");
		Value data     = LLVMBuildBitCast(builder, raw_data, LLVMPointerType(elem_t, 0), "lit_data");

		// Generate and store each element
		for (int i = 0; i < count; i++) {
			gen_asm(node->children[i]);
			load_if_necessary(node->children[i]);
			Value idx       = LLVMConstInt(i32, i, 0);
			Value indices[] = {idx};
			Value gep       = LLVMBuildGEP2(builder, elem_t, data, indices, 1, "lit_elem");
			LLVMBuildStore(builder, node->children[i]->token->llvm.elem, gep);
		}

		node->token->llvm.elem       = data;
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = cnt;
		break;
	}
	case FCALL: {
		// List built-in methods
		// Static call: Foo::method() — no self, explicit args only
		if (node->token->is_static_call) {
			Node  *func  = node->token->Fcall.ptr;
			LLVM   srcFn = func->token->llvm;
			int    argc  = node->left->children_count;
			Value *args  = allocate(argc + 1, sizeof(Value));
			for (int i = 0; i < argc; i++) {
				gen_asm(node->left->children[i]);
				load_if_necessary(node->left->children[i]);
				args[i] = node->left->children[i]->token->llvm.elem;
			}
			node->token->llvm.elem =
			    LLVMBuildCall2(builder, srcFn.funcType, srcFn.elem, args, argc, "");
			if (node->token->retType != VOID) node->token->llvm.is_loaded = true;
			free(args);
			// Track temp struct for auto-cleanup
			if (node->token->retType == STRUCT_CALL && node->token->Struct.ptr) {
				TypeRef st_type = get_llvm_type(node->token);
				Value   tmp     = LLVMBuildAlloca(builder, st_type, "tmp_struct");
				LLVMBuildStore(builder, node->token->llvm.elem, tmp);
				if (scope->temps == NULL || scope->temps_count + 5 >= scope->temps_size) {
					int    new_size = scope->temps == NULL ? 10 : scope->temps_size * 2;
					Value *nt       = allocate(new_size, sizeof(Value));
					Node **nd       = allocate(new_size, sizeof(Node *));
					if (scope->temps) {
						memcpy(nt, scope->temps, scope->temps_count * sizeof(Value));
						memcpy(nd, scope->temp_defs, scope->temps_count * sizeof(Node *));
						free(scope->temps);
						free(scope->temp_defs);
					}
					scope->temps      = nt;
					scope->temp_defs  = nd;
					scope->temps_size = new_size;
				}
				scope->temps[scope->temps_count]     = tmp;
				scope->temp_defs[scope->temps_count] = node->token->Struct.ptr;
				scope->temps_count++;
			}
			break;
		}
		LLVM   srcFunc     = node->token->Fcall.ptr->token->llvm;
		bool   is_variadic = node->token->Fcall.ptr->token->is_variadic;
		int    count       = node->left->children_count;
		Node **argNodes    = node->left->children;
		Node  *dec_args    = node->token->Fcall.ptr->left;
		Value *args        = NULL;

		if (count) {
			args = allocate(count + 3, sizeof(Value));

			int fixed_params = is_variadic ? node->token->Fcall.ptr->left->children_count : count;

			// handle fixed params (both variadic and non-variadic)
			for (int i = 0; i < fixed_params; i++) {
				gen_asm(argNodes[i]);
				bool param_is_ref =
				    (i < dec_args->children_count) && dec_args->children[i]->token->is_ref;
				bool arg_is_ref = argNodes[i]->token->is_ref;

				if (param_is_ref && arg_is_ref) {
					// ref → ref: load the pointer out of the ref's slot and pass it
					TypeRef type = get_llvm_type(argNodes[i]->token);
					args[i]      = LLVMBuildLoad2(builder, LLVMPointerType(type, 0),
					                              argNodes[i]->token->llvm.elem, "ref_arg");
				} else if (param_is_ref && !arg_is_ref) {
					// non-ref → ref param: pass the variable's alloca directly
					args[i] = argNodes[i]->token->llvm.elem;
				} else if (!param_is_ref && arg_is_ref) {
					// ref → non-ref param: double dereference to get the value
					args[i] = load_value(argNodes[i]->token);
				} else {
					// non-ref → non-ref
					// C++ copy-constructor semantics: if passing struct by value and a copy operator
					// exists, use it to deep-copy instead of a shallow bitcopy
					bool did_copy          = false;
					bool param_is_struct_v = (i < dec_args->children_count) &&
					                         dec_args->children[i]->token->type == STRUCT_CALL;
					bool arg_is_struct = argNodes[i]->token->type == STRUCT_CALL ||
					                     argNodes[i]->token->retType == STRUCT_CALL;
					if (param_is_struct_v && arg_is_struct) {
						Node *st_node = dec_args->children[i]->token->Struct.ptr;
						char *op_name =
						    strjoin(st_node->token->name, ".operator.ASSIGN.", st_node->token->name);
						Value copy_op = LLVMGetNamedFunction(module, op_name);
						free(op_name);
						if (copy_op) {
							TypeRef st_type = get_llvm_type(dec_args->children[i]->token);
							Value   tmp     = LLVMBuildAlloca(builder, st_type, "copy");
							LLVMBuildStore(builder, LLVMConstNull(st_type), tmp);
							// get source pointer — spill to alloca if it's a raw value (e.g. fn return)
							Value src_ptr;
							if (LLVMGetTypeKind(LLVMTypeOf(argNodes[i]->token->llvm.elem)) ==
							    LLVMPointerTypeKind) {
								src_ptr = argNodes[i]->token->llvm.elem;
							} else {
								Value spill = LLVMBuildAlloca(builder, st_type, "spill");
								LLVMBuildStore(builder, argNodes[i]->token->llvm.elem, spill);
								src_ptr = spill;
							}
							Value cargs[] = {src_ptr, tmp};
							LLVMBuildCall2(builder, LLVMGlobalGetValueType(copy_op), copy_op, cargs, 2,
							               "");
							args[i]  = LLVMBuildLoad2(builder, st_type, tmp, "copy_val");
							did_copy = true;
						}
					}
					if (!did_copy) {
						load_if_necessary(argNodes[i]);
						args[i] = argNodes[i]->token->llvm.elem;
					}

					// ARM64 ABI: proto calls pass small structs as i64 via memcpy
					bool is_proto_call = node->token->Fcall.ptr->token->is_proto;
					bool param_is_struct =
					    (i < dec_args->children_count) &&
					    includes(dec_args->children[i]->token->type, STRUCT_CALL, STRUCT_DEF, 0);
					if (is_proto_call && param_is_struct) {
						TypeRef st_type = get_llvm_type(dec_args->children[i]->token);
						Value   st_ptr  = LLVMBuildAlloca(builder, st_type, "st_slot");
						LLVMBuildStore(builder, args[i], st_ptr);
						Value i64p = LLVMBuildAlloca(builder, i64, "i64_slot");
						LLVMBuildMemCpy(builder, i64p, 0, st_ptr, 0, LLVMConstInt(i64, 4, 0));
						args[i] = LLVMBuildLoad2(builder, i64, i64p, "i64_arg");
					}
				}
			}

			if (is_variadic) {
				int variadic_count = count - fixed_params;
				// Always insert count before variadic args (Ura variadic convention)
				args[fixed_params] = LLVMConstInt(i32, variadic_count, 0);
				for (int i = fixed_params; i < count; i++) {
					gen_asm(argNodes[i]);
					load_if_necessary(argNodes[i]);
					args[i + 1] = argNodes[i]->token->llvm.elem;
				}
				count++;
			}
		}

		char   *name     = node->token->Fcall.ptr->token->retType != VOID ? node->token->name : "";
		TypeRef funcType = srcFunc.funcType;
		Value   elem     = srcFunc.elem;
		if (check(!srcFunc.funcType, "FCALL: funcType is NULL for '%s'", node->token->name)) break;
		if (check(!srcFunc.elem, "FCALL: elem is NULL for '%s'", node->token->name))         break;
		node->token->llvm.elem = LLVMBuildCall2(builder, funcType, elem, args, count, name);
		free(args);

		// ARM64 ABI: proto returning struct actually returns i64; convert back to struct
		bool proto_returns_struct = node->token->Fcall.ptr->token->is_proto &&
		                            node->token->Fcall.ptr->token->retType == STRUCT_CALL;
		if (proto_returns_struct) {
			Value   i64_ret = node->token->llvm.elem;
			TypeRef st_type = get_llvm_type(node->token);
			Value   i64p    = LLVMBuildAlloca(builder, i64, "ret_i64");
			LLVMBuildStore(builder, i64_ret, i64p);
			Value st_ptr = LLVMBuildAlloca(builder, st_type, "ret_struct");
			LLVMBuildMemCpy(builder, st_ptr, 0, i64p, 0, LLVMConstInt(i64, 4, 0));
			node->token->llvm.elem = LLVMBuildLoad2(builder, st_type, st_ptr, "ret_struct_val");
		}
		// Track temp struct for auto-cleanup in current scope
		if (node->token->retType == STRUCT_CALL && node->token->Struct.ptr) {
			TypeRef st_type = get_llvm_type(node->token);
			Value   tmp     = LLVMBuildAlloca(builder, st_type, "tmp_struct");
			LLVMBuildStore(builder, node->token->llvm.elem, tmp);
			if (scope->temps == NULL || scope->temps_count + 5 >= scope->temps_size) {
				int    new_size = scope->temps == NULL ? 10 : scope->temps_size * 2;
				Value *nt       = allocate(new_size, sizeof(Value));
				Node **nd       = allocate(new_size, sizeof(Node *));
				if (scope->temps) {
					memcpy(nt, scope->temps, scope->temps_count * sizeof(Value));
					memcpy(nd, scope->temp_defs, scope->temps_count * sizeof(Node *));
					free(scope->temps);
					free(scope->temp_defs);
				}
				scope->temps      = nt;
				scope->temp_defs  = nd;
				scope->temps_size = new_size;
			}
			scope->temps[scope->temps_count]     = tmp;
			scope->temp_defs[scope->temps_count] = node->token->Struct.ptr;
			scope->temps_count++;
		}
		break;
	}
	case FDEC: {
		char *fname       = node->token->name;
		int   fixed_count = node->left->children_count;
		if (!node->token->llvm.funcType) {
			TypeRef retType;
			if (node->token->retType == TUPLE) {
				int      n  = node->token->Tuple.count;
				TypeRef *ft = allocate(n, sizeof(TypeRef));
				for (int i = 0; i < n; i++)
					ft[i] = get_llvm_type(node->token->Tuple.types[i]);
				retType                  = LLVMStructTypeInContext(context, ft, n, 0);
				node->token->llvm.stType = retType;
				free(ft);
			} else if (node->token->is_proto && node->token->retType == STRUCT_CALL) retType = i64;
			else if (node->token->retType == STRUCT_CALL && node->token->is_ref)
				retType = LLVMPointerType(get_llvm_type(node->token), 0);
			else retType = get_llvm_type(node->token);

			TypeRef *paramTypes  = NULL;
			int      param_count = fixed_count;
			int      _count      = param_count;

			if (node->token->is_variadic) _count = fixed_count + 1;

			if (param_count) {
				paramTypes = allocate(_count + 2, sizeof(TypeRef));

				for (int i = 0; i < fixed_count; i++) {
					Token *param = node->left->children[i]->token;
					if (node->token->is_proto && includes(param->type, STRUCT_CALL, STRUCT_DEF, 0))
						paramTypes[i] = i64;
					else if (param->is_ref) paramTypes[i] = LLVMPointerType(get_llvm_type(param), 0);
					else paramTypes[i] = get_llvm_type(param);
				}

				if (node->token->is_variadic) paramTypes[fixed_count] = i32;
			}

			TypeRef funcType;
			if (strcmp(fname, "main") == 0) {
				bool os_used = false;
				for (int i = 0; i < ura_scope->children_count; i++) {
					Node *child = ura_scope->children[i];
					if (child->token->type == STRUCT_CALL && child->token->is_global &&
					    strcmp(child->token->name, "os") == 0 && child->token->used > 0) {
						os_used = true;
						break;
					}
				}
				if (os_used) {
					TypeRef argv_type  = LLVMPointerType(LLVMPointerType(i8, 0), 0);
					TypeRef main_pts[] = {i32, argv_type};
					funcType           = LLVMFunctionType(retType, main_pts, 2, false);
				} else funcType = LLVMFunctionType(retType, NULL, 0, false);
			} else funcType = LLVMFunctionType(retType, paramTypes, _count, node->token->is_variadic);
			Value existingFunc = LLVMGetNamedFunction(module, fname);
			if (existingFunc) node->token->llvm.elem = existingFunc;
			else node->token->llvm.elem = _add_function(fname, funcType);
			node->token->llvm.funcType = funcType;
		}

		enter_scope(node);
		if (!node->token->is_proto) {
			if (enable_san)
				LLVMAddAttributeAtIndex(
				    node->token->llvm.elem, LLVMAttributeFunctionIndex,
				    LLVMCreateEnumAttribute(
				        context, LLVMGetEnumAttributeKindForName("sanitize_address", 16), 0));

			// Debug info for this function
			LLVMMetadataRef di_func_type =
			    LLVMDIBuilderCreateSubroutineType(debug_builder, debug_file, NULL, 0, LLVMDIFlagZero);

			LLVMMetadataRef di_func =
			    LLVMDIBuilderCreateFunction(debug_builder, debug_compile_unit, fname, strlen(fname),
			                                fname, strlen(fname), debug_file, node->token->line,
			                                di_func_type, 0, 1, node->token->line, LLVMDIFlagZero, 0);
			LLVMSetSubprogram(node->token->llvm.elem, di_func);
			debug_scope = di_func;
			_entry(node->token);

			// set a valid location for param allocas
			LLVMMetadataRef entry_loc =
			    LLVMDIBuilderCreateDebugLocation(context, node->token->line, 0, di_func, NULL);
			LLVMSetCurrentDebugLocation2(builder, entry_loc);

			int param_idx = 0;
			for (int i = 0; i < fixed_count; i++) {
				Token *param_token = node->left->children[i]->token;
				Value  param       = LLVMGetParam(node->token->llvm.elem, param_idx++);
				LLVMSetValueName(param, param_token->name);
				_alloca(param_token);
				param_token->is_dec = false;
				LLVMBuildStore(builder, param, param_token->llvm.elem);
			}

			// Populate os.argc and os.argv from main params (only if os is used)
			if (strcmp(fname, "main") == 0) {
				for (int i = 0; i < ura_scope->children_count; i++) {
					Node *child = ura_scope->children[i];
					if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
					if (child->token->used == 0)                                       continue;
					if (strcmp(child->token->name, "os") != 0)                         continue;
					Value argc_val = LLVMGetParam(node->token->llvm.elem, 0);
					Value argv_val = LLVMGetParam(node->token->llvm.elem, 1);
					Value argc_ptr = struct_field_ptr(child->token, 0, "os.argc");
					LLVMBuildStore(builder, argc_val, argc_ptr);
					Value argv_ptr = struct_field_ptr(child->token, 1, "os.argv");
					LLVMBuildStore(builder, argv_val, argv_ptr);
					break;
				}
			}

#if USING_HOIST
			for (int i = 0; i < node->children_count; i++)
				hoist_allocas(node->children[i]);
#endif

			for (int i = 0; i < node->children_count; i++) {
				gen_asm(node->children[i]);
				if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
			}
		}
		exit_scope();
		debug_scope = debug_compile_unit;
		break;
	}
	case TUPLE_UNPACK: {
		// Evaluate RHS (function call returning a tuple/aggregate)
		gen_asm(node->left);
		Value tuple_val = node->left->token->llvm.elem;
		// Alloca each LHS variable and extract the corresponding field
		for (int i = 0; i < node->children_count; i++) {
			Node *lhs = node->children[i];
			gen_asm(lhs); // handles is_dec → _alloca, sets is_dec = false
			Value field = LLVMBuildExtractValue(builder, tuple_val, i, lhs->token->name);
			LLVMBuildStore(builder, field, lhs->token->llvm.elem);
		}
		break;
	}
	case WHILE: {
		enter_scope(node);
		Block start = _append_block("while.start");
		Block then  = _append_block("while.then");
		Block end   = _append_block("while.end");

		// Store blocks for break/continue
		node->token->llvm.start = start;
		node->token->llvm.then  = then;
		node->token->llvm.end   = end;

		_branch(start);

		LLVMPositionBuilderAtEnd(builder, start);
		gen_asm(node->left); // condition
		load_if_necessary(node->left);
		LLVMBuildCondBr(builder, node->left->token->llvm.elem, then, end);

		LLVMPositionBuilderAtEnd(builder, then);
		for (int i = 0; i < node->children_count; i++) {
			if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
			gen_asm(node->children[i]);
		}
		_branch(start);

		LLVMPositionBuilderAtEnd(builder, end);
		exit_scope();
		break;
	}
	case IF: {
		enter_scope(node);

		Block if_start = _append_block("if.start");
		Block end      = _append_block("if.end");

		// Store blocks for potential break/continue
		node->token->llvm.start = if_start;
		node->token->llvm.end   = end;

		_branch(if_start);

		Node *curr = node;

		while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
			if (includes(curr->token->type, IF, ELIF, 0)) {
				Block start;
				Block then;
				Block next;

				if (curr->token->type == IF) {
					start = if_start;
					then  = _append_block("if.then");
				} else {
					start = curr->token->llvm.bloc;
					then  = _append_block("elif.then");
				}

				// Store then block
				curr->token->llvm.then = then;

				if (curr->right) {
					if (curr->right->token->type == ELSE) next = _append_block("if.else");
					else next = _append_block("elif.start");
				} else next = end;

				LLVMPositionBuilderAtEnd(builder, start);
				gen_asm(curr->left); // condition
				load_if_necessary(curr->left);
				LLVMBuildCondBr(builder, curr->left->token->llvm.elem, then, next);

				LLVMPositionBuilderAtEnd(builder, then);
				for (int i = 0; i < curr->children_count; i++) {
					gen_asm(curr->children[i]);
					if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
				}
				_branch(end);

				if (curr->right && includes(curr->right->token->type, ELIF, ELSE, 0))
					curr->right->token->llvm.bloc = next;
			} else if (curr->token->type == ELSE) {
				LLVMPositionBuilderAtEnd(builder, curr->token->llvm.bloc);
				for (int i = 0; i < curr->children_count; i++) {
					gen_asm(curr->children[i]);
					if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
				}
				_branch(end);
			}
			curr = curr->right;
		}
		LLVMPositionBuilderAtEnd(builder, end);
		exit_scope();
		break;
	}
	case REF: {
		gen_asm(node->left);
		// Return the alloca pointer — don't load
		node->token->llvm.elem = node->left->token->llvm.elem;
		break;
	}
	case BREAK: {
		for (int i = sccount; i > 0; i--) {
			if (level_scope[i]->token->type == WHILE) {
				_branch(level_scope[i]->token->llvm.end);
				return;
			}
		}
		check(1, "break outside loop");
		break;
	}
	case CONTINUE: {
		for (int i = sccount; i > 0; i--) {
			if (level_scope[i]->token->type == WHILE) {
				_branch(level_scope[i]->token->llvm.start);
				return;
			}
		}
		check(1, "continue outside loop");
		break;
	}
	case RETURN: {
		Token *ret_skip =
		    (node->left && node->left->token->type == STRUCT_CALL && !node->left->token->is_ref)
		        ? node->left->token
		        : NULL;
		emit_scope_clean(scope, 0, ret_skip);

		// Auto-insert nested field delete calls for .delete methods before return
		{
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC) {
					fdec = level_scope[i];
					break;
				}
			if (fdec) {
				char *fn_name    = fdec->token->name;
				int   fn_len     = strlen(fn_name);
				bool  is_clean_f = fn_len > 7 && strcmp(fn_name + fn_len - 7, ".delete") == 0;
				int   fxd_count  = fdec->left->children_count;
				if (is_clean_f && fxd_count >= 1) {
					Token *self_tok = fdec->left->children[0]->token;
					if (self_tok->type == STRUCT_CALL && self_tok->is_ref) {
						Node   *sd       = self_tok->Struct.ptr;
						TypeRef st_type  = sd->token->llvm.stType;
						TypeRef ptr_type = LLVMPointerType(st_type, 0);
						Value   self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
						for (int i = 0; i < sd->children_count; i++) {
							Token *field = sd->children[i]->token;
							if (field->type != STRUCT_CALL) continue;
							Node *fsd   = field->Struct.ptr;
							char *qname = strjoin(fsd->token->name, ".delete", NULL);
							Value ffn   = LLVMGetNamedFunction(module, qname);
							free(qname);
							if (!ffn) continue;
							Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
							Value field_ptr =
							    LLVMBuildGEP2(builder, st_type, self_ptr, indices, 2, field->name);
							Value args[] = {field_ptr};
							LLVMBuildCall2(builder, LLVMGlobalGetValueType(ffn), ffn, args, 1, "");
						}
					}
				}
			}
		}

		// Inject ura_scope struct clean calls before main returns
		{
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC) {
					fdec = level_scope[i];
					break;
				}
			if (fdec && strcmp(fdec->token->name, "main") == 0) {
				for (int i = 0; i < ura_scope->children_count; i++) {
					Node *child = ura_scope->children[i];
					if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
					if (child->token->used == 0)                                       continue;
					char *qname   = strjoin(child->token->Struct.ptr->token->name, ".delete", NULL);
					Value clean_f = LLVMGetNamedFunction(module, qname);
					free(qname);
					if (!clean_f) continue;
					Value args[] = {child->token->llvm.elem};
					LLVMBuildCall2(builder, LLVMGlobalGetValueType(clean_f), clean_f, args, 1, "");
				}
			}
		}

		// Tuple return: pack multiple values into an anonymous struct
		{
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC) {
					fdec = level_scope[i];
					break;
				}
			if (fdec && fdec->token->retType == TUPLE && node->children_count > 0) {
				TypeRef tuple_type = fdec->token->llvm.stType;
				Value   agg        = LLVMGetUndef(tuple_type);
				for (int i = 0; i < node->children_count; i++) {
					gen_asm(node->children[i]);
					load_if_necessary(node->children[i]);
					Value v = node->children[i]->token->llvm.elem;
					agg     = LLVMBuildInsertValue(builder, agg, v, i, "");
				}
				LLVMBuildRet(builder, agg);
				break;
			}
		}

		if (node->left && node->left->token->type != VOID) {
			gen_asm(node->left);
			Token *ret_tok = node->left->token;

			// Find the enclosing FDEC to check its return type
			Node *fdec = NULL;
			for (int i = sccount; i >= 0; i--)
				if (level_scope[i] && level_scope[i]->token->type == FDEC) {
					fdec = level_scope[i];
					break;
				}

			bool fdec_returns_ref    = fdec && fdec->token->is_ref;
			bool fdec_returns_struct = fdec && fdec->token->retType == STRUCT_CALL;

			if (fdec_returns_struct && !fdec_returns_ref) {
				// return by value: load the full struct
				Value val = load_value(ret_tok);
				LLVMBuildRet(builder, val);
				break;
			} else if (fdec_returns_struct && fdec_returns_ref) {
				// return by ref: return the pointer directly (self->llvm.elem holds User*)
				// self is stored as alloca of ptr, so load the ptr
				TypeRef st_type  = get_llvm_type(ret_tok);
				TypeRef ptr_type = LLVMPointerType(st_type, 0);
				Value   ptr      = LLVMBuildLoad2(builder, ptr_type, ret_tok->llvm.elem, "ret_ptr");
				LLVMBuildRet(builder, ptr);
				break;
			}
			load_if_necessary(node->left);
		}
		if (node->left) _build_return(node->left->token);
		break;
	}
	case AS: {
		gen_asm(left);
		load_if_necessary(left);

		Value    source     = left->token->llvm.elem;
		TypeRef  sourceType = LLVMTypeOf(source);
		TypeRef  targetType = get_llvm_type(node->right->token);

		TypeKind sourceKind = LLVMGetTypeKind(sourceType);
		TypeKind targetKind = LLVMGetTypeKind(targetType);

		Value    result;

		bool     srcIsFloat = (sourceKind == FloatType || sourceKind == DoubleType);
		bool     tgtIsFloat = (targetKind == FloatType || targetKind == DoubleType);

		// Same type — no-op
		if (sourceType == targetType) result = source;
		// Integer → Integer (trunc / sign-extend / same)
		else if (sourceKind == IntegerType && targetKind == IntegerType) {
			unsigned srcBits = LLVMGetIntTypeWidth(sourceType);
			unsigned tgtBits = LLVMGetIntTypeWidth(targetType);
			if (srcBits > tgtBits)      result = LLVMBuildTrunc(builder, source, targetType, "as");
			else if (srcBits < tgtBits) result = LLVMBuildSExt(builder, source, targetType, "as");
			else result = source;
		}
		// Integer → Float / Double  (signed)
		else if (sourceKind == IntegerType && tgtIsFloat)
			result = LLVMBuildSIToFP(builder, source, targetType, "as");
		// Float / Double → Integer  (signed, truncating toward zero)
		else if (srcIsFloat && targetKind == IntegerType)
			result = LLVMBuildFPToSI(builder, source, targetType, "as");
		// Float ↔ Double
		else if (srcIsFloat && tgtIsFloat) {
			if (sourceKind == FloatType) result = LLVMBuildFPExt(builder, source, targetType, "as");
			else result = LLVMBuildFPTrunc(builder, source, targetType, "as");
		}
		// Integer → Pointer
		else if (sourceKind == IntegerType && targetKind == PointerType)
			result = LLVMBuildIntToPtr(builder, source, targetType, "as");
		// Pointer → Integer
		else if (sourceKind == PointerType && targetKind == IntegerType)
			result = LLVMBuildPtrToInt(builder, source, targetType, "as");
		// Pointer → Pointer  (bitcast)
		else if (sourceKind == PointerType && targetKind == PointerType)
			result = LLVMBuildBitCast(builder, source, targetType, "as");
		else {
			check(1, "unsupported cast from %d to %d", sourceKind, targetKind);
			result = source;
		}

		node->token->llvm.elem      = result;
		node->token->llvm.is_loaded = true;
		break;
	}
	case ENUM_DEF: {
		for (int i = 0; i < node->children_count; i++) {
			Token *var = node->children[i]->token;
			Value  g   = LLVMAddGlobal(module, i32, var->name);
			LLVMSetInitializer(g, LLVMConstInt(i32, (unsigned)var->Int.value, 0));
			LLVMSetGlobalConstant(g, 1);
			var->llvm.elem = g;
			// var->llvm.is_set = true;
			var->is_dec = false;
		}
		break;
	}
	case MODULE: {
		for (int i = 0; i < node->functions_count; i++)
			gen_asm(node->functions[i]);
		break;
	}
	case STRUCT_DEF: {
		if (node->token->used == 0) return;
		// Ensure nested struct field types are also emitted
		for (int i = 0; i < node->children_count; i++) {
			Token *ft = node->children[i]->token;
			if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0) {
				ft->Struct.ptr->token->used++;
				gen_asm(ft->Struct.ptr);
			}
			// Also handle array[StructType] fields
			if (includes(ft->type, ARRAY_TYPE, ARRAY, 0) && ft->Array.elem_type == STRUCT_CALL &&
			    ft->Array.struct_ptr && ft->Array.struct_ptr->token->used == 0) {
				ft->Array.struct_ptr->token->used++;
				gen_asm(ft->Array.struct_ptr);
			}
		}
		int      pos   = node->children_count;
		TypeRef *types = allocate((pos + 1), sizeof(TypeRef));
		int      j     = 0;
		for (int i = 0; i < pos; i++)
			types[j++] = get_llvm_type(node->children[i]->token);
		char *struct_name        = strjoin("struct.", node->token->name, NULL);
		node->token->llvm.stType = _named_struct_type(struct_name, types, j, 0);
		free(struct_name);
		free(types);

		TypeRef st_type     = node->token->llvm.stType;
		TypeRef ptr_type    = LLVMPointerType(st_type, 0);
		TypeRef lc_params[] = {ptr_type};
		TypeRef lc_fn_type  = LLVMFunctionType(vd, lc_params, 1, 0);

		if (!node->token->has_clean) {
			char *fname = strjoin(node->token->name, ".delete", NULL);
			Value fn    = _add_function(fname, lc_fn_type);
			free(fname);
			Block entry = LLVMAppendBasicBlockInContext(context, fn, "entry");
			LLVMPositionBuilderAtEnd(builder, entry);
			LLVMSetCurrentDebugLocation2(builder, NULL);
			Value self = LLVMGetParam(fn, 0);
			for (int i = 0; i < node->children_count; i++) {
				Token *field = node->children[i]->token;
				if (field->type != STRUCT_CALL) continue;
				Node *fsd   = field->Struct.ptr;
				char *qname = strjoin(fsd->token->name, ".delete", NULL);
				Value ffn   = LLVMGetNamedFunction(module, qname);
				free(qname);
				if (!ffn) continue;
				Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
				Value field_ptr = LLVMBuildGEP2(builder, st_type, self, indices, 2, field->name);
				Value args[]    = {field_ptr};
				LLVMBuildCall2(builder, LLVMGlobalGetValueType(ffn), ffn, args, 1, "");
			}
			LLVMBuildRetVoid(builder);
			node->token->has_clean = true;
		}

		// Pre-declare all method LLVM signatures (forward references)
		for (int i = 0; i < node->functions_count; i++) {
			Node *fn = node->functions[i];
			if (fn->token->llvm.funcType) continue;
			TypeRef retType;
			if (fn->token->retType == TUPLE) {
				int      n  = fn->token->Tuple.count;
				TypeRef *ft = allocate(n, sizeof(TypeRef));
				for (int k = 0; k < n; k++)
					ft[k] = get_llvm_type(fn->token->Tuple.types[k]);
				retType                = LLVMStructTypeInContext(context, ft, n, 0);
				fn->token->llvm.stType = retType;
				free(ft);
			} else if (fn->token->is_proto && fn->token->retType == STRUCT_CALL) retType = i64;
			else if (fn->token->retType == STRUCT_CALL && fn->token->is_ref)
				retType = LLVMPointerType(get_llvm_type(fn->token), 0);
			else retType = get_llvm_type(fn->token);
			int      pc = fn->left->children_count;
			int      ac = fn->token->is_variadic ? pc + 1 : pc;
			TypeRef *pt = pc ? allocate(ac + 2, sizeof(TypeRef)) : NULL;
			for (int j = 0; j < pc; j++) {
				Token *p = fn->left->children[j]->token;
				if (fn->token->is_proto && includes(p->type, STRUCT_CALL, STRUCT_DEF, 0)) pt[j] = i64;
				else if (p->is_ref)                                                       pt[j] = LLVMPointerType(get_llvm_type(p), 0);
				else pt[j] = get_llvm_type(p);
			}
			if (fn->token->is_variadic) pt[pc] = i32;
			TypeRef ft               = LLVMFunctionType(retType, pt, ac, fn->token->is_variadic);
			fn->token->llvm.elem     = _add_function(fn->token->name, ft);
			fn->token->llvm.funcType = ft;
			if (pt) free(pt);
		}
		// Process all method bodies
		for (int i = 0; i < node->functions_count; i++)
			gen_asm(node->functions[i]);
		break;
	}
	case DOT: {
		gen_asm(node->left);

		Token *struct_tok  = node->left->token;
		int    field_index = node->right->token->Struct.index;

		// If the struct is a ref, we need to load the pointer first
		// before indexing into it, otherwise we GEP into the alloca
		// that holds the pointer, not into the struct itself.
		if (struct_tok->is_ref) {
			TypeRef struct_type = get_llvm_type(struct_tok);
			Value   struct_ptr  = LLVMBuildLoad2(builder, LLVMPointerType(struct_type, 0),
			                                     struct_tok->llvm.elem, struct_tok->name);
			Value   indices[]   = {
             LLVMConstInt(i32, 0, 0),
             LLVMConstInt(i32, field_index, 0),
         };
			node->token->llvm.elem =
			    LLVMBuildGEP2(builder, struct_type, struct_ptr, indices, 2, node->right->token->name);
		} else {
			Value field_ptr = struct_field_ptr(struct_tok, field_index, node->right->token->name);
			node->token->llvm.elem = field_ptr;
		}
		// Propagate multi-dim info from struct field definition
		if (struct_tok->Struct.ptr) {
			Node *sd = struct_tok->Struct.ptr;
			if (field_index >= 0 && field_index < sd->children_count) {
				Token *field_def = sd->children[field_index]->token;
				if (field_def->llvm.dims_count > 0) {
					node->token->llvm.dims_count = 0;
					for (int d = 0; d < field_def->llvm.dims_count; d++) {
						resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
						             node->token->llvm.dims_count);
						node->token->llvm.dims[node->token->llvm.dims_count++] = field_def->llvm.dims[d];
					}
				}
			}
		}
		break;
	}
	case ACCESS: {
		// bool enable_bounds_check = false;
		gen_asm(node->left);

		if (node->left->token->type == STRUCT_CALL) {
			Token *struct_tok  = node->left->token;
			int    field_index = node->right->token->Struct.index;

			Value  field_ptr = struct_field_ptr(struct_tok, field_index, node->right->token->name);

			node->token->llvm.elem = field_ptr;
			/* retType already set by gen_ir; mark as NOT loaded so
			   load_if_necessary() will emit the load when reading. */
			break;
		}

		gen_asm(node->right);
		Value leftValue;
		if (node->left->token->is_ref) {
			todo(1, "stop");
			// If left is a ref, dereference it to get the actual array
			leftValue = load_value(node->left->token);
		} else {
			// todo(1, "stop");
			load_if_necessary(node->left);
			leftValue     = node->left->token->llvm.elem;
			TypeKind kind = LLVMGetTypeKind(LLVMTypeOf(leftValue));
			if (kind == PointerType && node->left->token->name && !node->left->token->llvm.is_loaded &&
			    node->left->token->type != STACK && node->left->token->type != DOT) {
				leftValue = LLVMBuildLoad2(builder, p8, leftValue, "ptr_load");
			}
		}

		// Load the index (handles refs)
		load_if_necessary(node->right);
		Value rightRef = node->right->token->llvm.elem;

		// multi-dim flat array: compute stride, emit GEP to sub-array, propagate dims
		int left_depth = node->left->token->llvm.dims_count;
		if (left_depth > 1) {
			Value stride = LLVMConstInt(i32, 1, 0);
			for (int d = 1; d < left_depth; d++)
				stride = LLVMBuildMul(builder, stride, node->left->token->llvm.dims[d], "stride");
			Value   flat_idx = LLVMBuildMul(builder, rightRef, stride, "flat_idx");

			TypeRef base_t;
			if (node->left->token->Array.elem_type == STRUCT_CALL &&
			    node->left->token->Array.struct_ptr)
				base_t = get_llvm_type(node->left->token->Array.struct_ptr->token);
			else {
				Token tmp = {.type = node->left->token->Array.elem_type};
				base_t    = get_llvm_type(&tmp);
			}
			Value indices[]             = {flat_idx};
			node->token->llvm.elem      = LLVMBuildGEP2(builder, base_t, leftValue, indices, 1, "row");
			node->token->llvm.is_loaded = true;
			node->token->retType        = ARRAY;
			node->token->Array.elem_type  = node->left->token->Array.elem_type;
			node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
			node->token->Array.depth      = left_depth - 1;
			node->token->llvm.dims_count  = 0;
			for (int d = 1; d < left_depth; d++) {
				resize_array(node->token->llvm.dims, Value, node->token->llvm.dims_size,
				             node->token->llvm.dims_count);
				node->token->llvm.dims[node->token->llvm.dims_count++] =
				    node->left->token->llvm.dims[d];
			}
			break;
		}

		TypeRef element_type;
		Type    left_elem_type =
          node->left->token->retType ? node->left->token->retType : node->left->token->type;
		if (left_elem_type == CHARS) {
			element_type         = i8;
			node->token->retType = CHAR;
		} else if (left_elem_type == ARRAY_TYPE || left_elem_type == ARRAY) {
			Type et = node->left->token->Array.elem_type;
			if (et == STRUCT_CALL && node->left->token->Array.struct_ptr) {
				element_type            = get_llvm_type(node->left->token->Array.struct_ptr->token);
				node->token->Struct.ptr = node->left->token->Array.struct_ptr;
			} else {
				Token tmp    = {.type = et};
				element_type = get_llvm_type(&tmp);
			}
			node->token->retType = et;
		} else if (node->left->token->type == HEAP || node->left->token->type == ARRAY) {
			check(1, "hello");
			Type et = node->left->token->Array.elem_type;
			if (et == STRUCT_CALL && node->left->token->Array.struct_ptr) {
				element_type            = get_llvm_type(node->left->token->Array.struct_ptr->token);
				node->token->Struct.ptr = node->left->token->Array.struct_ptr;
			} else {
				Token tmp    = {.type = et};
				element_type = get_llvm_type(&tmp);
			}
			node->token->retType = et;
		} else {
			element_type         = get_llvm_type(node->left->token);
			node->token->retType = node->left->token->type;
		}

		// Add bounds checking if enabled
		if (false) {
			// We need to track array sizes - for now we'll use a conservative approach
			// For CHARS (strings), we can use strlen at runtime
			// For STACK arrays, we need to store the size

			Value size_val = NULL;

			Type  left_elem_type =
             node->left->token->retType ? node->left->token->retType : node->left->token->type;
			if (left_elem_type == CHARS) {
				// For strings, we need to get the length
				// First check if it's a string literal or a variable
				if (node->left->token->name && !left->token->llvm.array_size) {
					// It's a variable - we need strlen
					Value strlen_func = LLVMGetNamedFunction(module, "strlen");
					if (!strlen_func) {
						TypeRef strlen_type = LLVMFunctionType(i64, (TypeRef[]){p8}, 1, false);
						strlen_func         = _add_function("strlen", strlen_type);
					}
					Value strlen_result = LLVMBuildCall2(builder, LLVMGlobalGetValueType(strlen_func),
					                                     strlen_func, (Value[]){leftValue}, 1, "strlen");
					size_val            = LLVMBuildTrunc(builder, strlen_result, i32, "size");
				} else if (node->left->token->llvm.array_size) {
					size_val = node->left->token->llvm.array_size;
				} else {
					// String literal - we know the size at compile time
					// This is handled in _chars function
					size_val = LLVMConstInt(i32, strlen(node->left->token->Chars.value), 0);
				}
			} else if (node->left->token->type == STACK) {
				// For STACK arrays, we should have stored the size
				// For now, use a placeholder - you'll need to enhance this
				size_val = LLVMConstInt(i32, 1000000, 0); // Conservative large value
			}

			if (size_val) {
				// Get bounds check function
				Value bounds_check = LLVMGetNamedFunction(module, "__bounds_check");

				// Create filename string
				static Value filename_str = NULL;

				if (!filename_str) {
					char filename[256] = {0};

					if (getcwd(filename, sizeof(filename)) != NULL) {
						size_t len = strlen(filename);
						snprintf(filename + len, sizeof(filename) - len, "/%s", "input_file");
						filename_str = LLVMBuildGlobalStringPtr(builder, filename, "filename");
					}
				}

				// Call bounds check: __bounds_check(index, size, line, filename)
				Value line_val = LLVMConstInt(i32, node->token->line, 0);
				LLVMBuildCall2(builder, LLVMGlobalGetValueType(bounds_check), bounds_check,
				               (Value[]){rightRef, size_val, line_val, filename_str}, 4, "");
			}
		}

		Value indices[] = {rightRef};
		Value gep       = LLVMBuildGEP2(builder, element_type, leftValue, indices, 1, "ACCESS");
		node->token->llvm.elem = gep;

		break;
	}
	case TYPEOF: case SIZEOF: {
		Token *type_tok = node->left->token;
		if (node->token->type == TYPEOF) {
			node->token->llvm.elem =
			    LLVMBuildGlobalStringPtr(builder, node->token->Chars.value, "typeof");
			node->token->llvm.is_loaded = true;
		} else {
			TypeRef    type             = get_llvm_type(type_tok);
			TargetData td               = LLVMGetModuleDataLayout(module);
			size_t     size             = LLVMABISizeOfType(td, type);
			node->token->Int.value      = (long long)size;
			node->token->llvm.elem      = LLVMConstInt(i32, size, 0);
			node->token->llvm.is_loaded = true;
		}
		break;
	}
	case OUTPUT: {
		int    argc = node->left->children_count;
		Node **argv = node->left->children;

		// Compute format string capacity: literals + format specifiers + struct expansion
		int fmt_cap = 64;
		for (int i = 0; i < argc; i++)
			if (argv[i]->token->type == CHARS && !argv[i]->token->name)
				fmt_cap += strlen(argv[i]->token->Chars.value) * 2 + 4;
			else fmt_cap += 128;

		char  *fmt             = allocate(fmt_cap, 1);
		Value *args            = allocate(256, sizeof(Value));
		int    functions_count = 0;
		int    nargs           = 0;

		for (int i = 0; i < argc; i++) {
			gen_asm(argv[i]);
			append_output_arg(argv[i]->token, fmt, &functions_count, args, &nargs);
		}
		fmt[functions_count] = '\0';

		Value printf_fn = LLVMGetNamedFunction(module, "printf");
		if (!printf_fn) {
			TypeRef params[]  = {p8, i32}; // fmt, nargs (language variadic convention)
			TypeRef printf_ft = LLVMFunctionType(i32, params, 2, 1);
			printf_fn         = _add_function("printf", printf_ft);
		}
		TypeRef printf_ft = LLVMGlobalGetValueType(printf_fn);
		// Follow the language's variadic calling convention: insert arg count between
		// fixed params and variadic args — same as FCALL does for variadic functions
		Value *call_args = allocate(nargs + 3, sizeof(Value));
		call_args[0]     = LLVMBuildGlobalStringPtr(builder, fmt, "output_fmt");
		call_args[1]     = LLVMConstInt(i32, nargs, 0);
		memcpy(call_args + 2, args, nargs * sizeof(Value));
		LLVMBuildCall2(builder, printf_ft, printf_fn, call_args, nargs + 2, "");
		free(fmt);
		free(args);
		free(call_args);
		break;
	}
	default:
		pnode(node, "");
		todo(1, "handle this case %s", to_string(node->token->type));
		// seg();
		break;
	}
}

// ============================================================================
// Preprocessed source output (-prep flag)
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

static void emit_expr(File f, Node *node);

static void emit_type_name(File f, Token *tok) {
	Type t = tok->retType ? tok->retType : tok->type;
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
		fprintf(f, "array[");
		const char *en = type_to_ura_name(tok->Array.elem_type);
		if (en) fprintf(f, "%s", en);
		else if (tok->Array.elem_type == STRUCT_CALL && tok->Array.struct_ptr)
			fprintf(f, "%s", tok->Array.struct_ptr->token->name);
		fprintf(f, "]");
		break;
	}
	default: fprintf(f, "%s", to_string(t)); break;
	}
}

static void emit_expr(File f, Node *node) {
	if (!node || !node->token) return;
	Token *tok = node->token;

	switch (tok->type) {
	case INT:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "%ld", tok->Int.value);
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
		else fprintf(f, "%s", tok->Bool.value ? "true" : "false");
		break;
	case CHAR:
		if (tok->name) fprintf(f, "%s", tok->name);
		else fprintf(f, "'%c'", tok->Char.value);
		break;
	case CHARS:
		if (tok->name) fprintf(f, "%s", tok->name);
		else {
			fprintf(f, "\"");
			const char *s = tok->Chars.value ? tok->Chars.value : "";
			for (int ci = 0; s[ci]; ci++) {
				switch (s[ci]) {
				case '\n': fprintf(f, "\\n"); break;
				case '\t': fprintf(f, "\\t"); break;
				case '\r': fprintf(f, "\\r"); break;
				case '\\': fprintf(f, "\\\\"); break;
				case '"':  fprintf(f, "\\\""); break;
				default:   fputc(s[ci], f); break;
				}
			}
			fprintf(f, "\"");
		}
		break;
	case NULLABLE:    fprintf(f, "null"); break;
	case VOID:        fprintf(f, "void"); break;
	case STRUCT_CALL: fprintf(f, "%s", tok->name ? tok->name : "?"); break;
	case ARRAY_TYPE:  if (tok->name) fprintf(f, "%s", tok->name); break;
	case ID:          fprintf(f, "%s", tok->name ? tok->name : "?"); break;
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
		emit_expr(f, node->left);
		fprintf(f, "::");
		emit_expr(f, node->right);
		break;
	case AS:
		emit_expr(f, node->left);
		fprintf(f, " as ");
		emit_type_name(f, node->right->token);
		break;
	case NOT:  fprintf(f, "not "); emit_expr(f, node->left); break;
	case BNOT: fprintf(f, "~"); emit_expr(f, node->left); break;
	case TYPEOF:
		fprintf(f, "typeof(");
		emit_expr(f, node->left);
		fprintf(f, ")");
		break;
	case SIZEOF:
		fprintf(f, "sizeof(");
		// sizeof argument is a type, not a value — print type name
		if (node->left && node->left->token) {
			const char *tn = type_to_ura_name(node->left->token->type);
			if (tn) fprintf(f, "%s", tn);
			else emit_expr(f, node->left);
		}
		fprintf(f, ")");
		break;
	case REF: fprintf(f, "ref "); emit_expr(f, node->left); break;
	// Binary operators
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:
		if (tok->Fcall.ptr) {
			// Operator overload — emit as Struct::method(args)
			char *oname = tok->Fcall.ptr->token->name;
			char *dot   = strchr(oname, '.');
			if (dot) {
				int plen = (int)(dot - oname);
				fprintf(f, "%.*s::%s(", plen, oname, dot + 1);
			} else fprintf(f, "%s(", oname);
			emit_expr(f, node->right);
			fprintf(f, ", ");
			emit_expr(f, node->left);
			fprintf(f, ")");
		} else {
			emit_expr(f, node->left);
			fprintf(f, " %s ", op_symbol(tok->type));
			emit_expr(f, node->right);
		}
		break;
	// Function calls
	case FCALL: {
		// Detect resolved method calls: qualified name "Struct.method" + last arg is self
		bool is_resolved_method = false;
		if (node->left && tok->name && strchr(tok->name, '.') && !tok->is_static_call) {
			int last = node->left->children_count - 1;
			if (last >= 0 && node->left->children[last]->token->type == STRUCT_CALL)
				is_resolved_method = true;
		}
		if (is_resolved_method) {
			// Method call: obj.method(args) — self is last child
			int last = node->left->children_count - 1;
			emit_expr(f, node->left->children[last]); // object
			char *dot = strrchr(tok->name, '.');
			fprintf(f, ".%s(", dot ? dot + 1 : tok->name);
			for (int i = 0; i < last; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
			fprintf(f, ")");
		} else if (tok->is_static_call && tok->name && strchr(tok->name, '.')) {
			// Static call: Struct::method(args)
			char *dotpos     = strchr(tok->name, '.');
			int   prefix_len = (int)(dotpos - tok->name);
			fprintf(f, "%.*s::%s(", prefix_len, tok->name, dotpos + 1);
			if (node->left) {
				for (int i = 0; i < node->left->children_count; i++) {
					if (i > 0) fprintf(f, ", ");
					emit_expr(f, node->left->children[i]);
				}
			}
			fprintf(f, ")");
		} else {
			fprintf(f, "%s(", tok->name);
			if (node->left) {
				for (int i = 0; i < node->left->children_count; i++) {
					if (i > 0) fprintf(f, ", ");
					emit_expr(f, node->left->children[i]);
				}
			}
			fprintf(f, ")");
		}
		break;
	}
	case HEAP: case STACK: {
		fprintf(f, "%s[", tok->type == HEAP ? "heap" : "stack");
		emit_type_name(f, tok);
		fprintf(f, "](");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, ")");
		break;
	}
	case ARRAY_LIT: {
		fprintf(f, "[");
		for (int i = 0; i < node->children_count; i++) {
			if (i > 0) fprintf(f, ", ");
			emit_expr(f, node->children[i]);
		}
		fprintf(f, "]");
		break;
	}
	default: fprintf(f, "/* ? %s */", to_string(tok->type)); break;
	}
}

static void emit_node(File f, Node *node, int depth) {
	if (!node || !node->token) return;
	Token *tok = node->token;

	switch (tok->type) {
	case PROTO: {
		// proto is on the child FDEC
		if (node->left && node->left->token->type == FDEC) emit_node(f, node->left, depth);
		break;
	}
	case FDEC: {
		emit_indent(f, depth);
		if (tok->is_proto) fprintf(f, "proto ");
		// Detect if this is a struct method (has self as last param)
		bool has_self = false;
		if (node->left && node->left->children_count > 0) {
			Token *last = node->left->children[node->left->children_count - 1]->token;
			if (last->type == STRUCT_CALL && last->name && strcmp(last->name, "self") == 0)
				has_self = true;
		}
		// Use short name for struct methods (strip "Struct." prefix)
		const char *fn_name = tok->name;
		if (has_self) {
			char *dot = strrchr(tok->name, '.');
			if (dot) fn_name = dot + 1;
		}
		// Detect operator methods: delete, operator.*, output
		bool is_operator = false;
		if (has_self && fn_name) {
			if (strcmp(fn_name, "delete") == 0 || strcmp(fn_name, "output") == 0 ||
			    strncmp(fn_name, "operator", 8) == 0)
				is_operator = true;
		}
		if (is_operator) {
			fprintf(f, "operator %s(", fn_name);
		} else {
			if (tok->is_pub) fprintf(f, "pub ");
			fprintf(f, "fn %s(", fn_name);
		}
		if (node->left) {
			int end = node->left->children_count;
			// Skip self param (implicit for all struct methods)
			if (has_self && end > 0) end--;
			int printed = 0;
			for (int i = 0; i < end; i++) {
				Token *p = node->left->children[i]->token;
				if (printed > 0) fprintf(f, ", ");
				fprintf(f, "%s ", p->name);
				emit_type_name(f, p);
				if (p->is_ref) fprintf(f, " ref");
				printed++;
			}
		}
		fprintf(f, ") ");
		// Return type
		if (tok->retType == STRUCT_CALL && tok->Struct.ptr)
			fprintf(f, "%s", tok->Struct.ptr->token->name);
		else if (tok->retType)
			fprintf(f, "%s",
			        type_to_ura_name(tok->retType) ? type_to_ura_name(tok->retType)
			                                       : to_string(tok->retType));
		if (tok->is_proto) {
			fprintf(f, "\n");
			break;
		}
		fprintf(f, ":\n");
		// Body
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		fprintf(f, "\n");
		break;
	}
	case STRUCT_DEF: {
		emit_indent(f, depth);
		fprintf(f, "struct %s:\n", tok->name);
		// Fields
		for (int i = 0; i < node->children_count; i++) {
			Token *field = node->children[i]->token;
			emit_indent(f, depth + 1);
			fprintf(f, "%s ", field->name);
			emit_type_name(f, field);
			fprintf(f, "\n");
		}
		fprintf(f, "\n");
		// Methods
		for (int i = 0; i < node->functions_count; i++)
			emit_node(f, node->functions[i], depth + 1);
		fprintf(f, "\n");
		break;
	}
	case STRUCT_CALL: {
		if (tok->is_dec) {
			emit_indent(f, depth);
			fprintf(f, "%s %s", tok->name, tok->Struct.ptr->token->name);
			if (tok->is_ref) fprintf(f, " ref");
			fprintf(f, "\n");
		}
		break;
	}
	case ASSIGN:     case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
	case DIV_ASSIGN: case MOD_ASSIGN: {
		emit_indent(f, depth);
		if (tok->Fcall.ptr) {
			// Operator overload — Struct::operator.OP.T(args)
			char *oname = tok->Fcall.ptr->token->name;
			char *dot   = strchr(oname, '.');
			if (dot) {
				int plen = (int)(dot - oname);
				fprintf(f, "%.*s::%s(", plen, oname, dot + 1);
			} else fprintf(f, "%s(", oname);
			emit_expr(f, node->right);
			fprintf(f, ", ");
			emit_expr(f, node->left);
			fprintf(f, ")\n");
		} else {
			emit_expr(f, node->left);
			fprintf(f, " %s ", op_symbol(tok->type));
			emit_expr(f, node->right);
			fprintf(f, "\n");
		}
		break;
	}
	case INT:   case LONG: case SHORT: case FLOAT: case BOOL: case CHAR:
	case CHARS: case ARRAY_TYPE: case PTR: {
		if (tok->is_dec) {
			emit_indent(f, depth);
			fprintf(f, "%s ", tok->name);
			emit_type_name(f, tok);
			if (tok->is_ref) fprintf(f, " ref");
			fprintf(f, "\n");
		}
		break;
	}
	case RETURN: {
		emit_indent(f, depth);
		// Skip implicit "return void" — the compiler auto-adds it
		if (node->left && node->left->token && node->left->token->type == VOID) break;
		fprintf(f, "return ");
		emit_expr(f, node->left);
		fprintf(f, "\n");
		break;
	}
	case IF: case ELIF: {
		emit_indent(f, depth);
		fprintf(f, "%s ", tok->type == IF ? "if" : "elif");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		if (node->right) emit_node(f, node->right, depth);
		break;
	}
	case ELSE: {
		emit_indent(f, depth);
		fprintf(f, "else:\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	}
	case WHILE: {
		emit_indent(f, depth);
		fprintf(f, "while ");
		emit_expr(f, node->left);
		fprintf(f, ":\n");
		for (int i = 0; i < node->children_count; i++)
			emit_node(f, node->children[i], depth + 1);
		break;
	}
	case BREAK: {
		emit_indent(f, depth);
		fprintf(f, "break\n");
		break;
	}
	case CONTINUE: {
		emit_indent(f, depth);
		fprintf(f, "continue\n");
		break;
	}
	case FCALL: {
		emit_indent(f, depth);
		emit_expr(f, node);
		fprintf(f, "\n");
		break;
	}
	case OUTPUT: {
		emit_indent(f, depth);
		fprintf(f, "output(");
		if (node->left) {
			for (int i = 0; i < node->left->children_count; i++) {
				if (i > 0) fprintf(f, ", ");
				emit_expr(f, node->left->children[i]);
			}
		}
		fprintf(f, ")\n");
		break;
	}
	default:
		emit_indent(f, depth);
		fprintf(f, "// ? %s\n", to_string(tok->type));
		break;
	}
}

void emit_prep_file(Node *scope, char *path) {
	File f = fopen(path, "w");
	if (!f) {
		check(1, "cannot open %s for writing", path);
		return;
	}
	fprintf(f, "// Preprocessed output — generated by ura -prep\n\n");
	for (int i = 0; i < scope->children_count; i++)
		emit_node(f, scope->children[i], 0);
	fclose(f);
}

// List type temp file paths (set by instantiate_list_types, used by compile)
static char list_tmp_files[64][URA_MAX_SIZE];
static int  list_tmp_count = 0;

void        instantiate_list_types(void) {
   // Scan token stream for LIST_TYPE, collect unique (elem_type, depth) pairs
   typedef struct {
      const char *base_name;
      int         depth;
   } ListEntry;
   ListEntry entries[64];
   int       entry_count = 0;

   for (int i = 0; i < tcount; i++) {
      if (tokens[i]->type != LIST_TYPE) continue;
      int j = i + 1;
      if (j >= tcount || tokens[j]->type != LBRA) continue;
      int depth = 0;
      while (j < tcount && tokens[j]->type == LBRA) {
         depth++;
         j++;
      }
      if (j >= tcount) continue;
      const char *base_name = tokens[j]->name ? tokens[j]->name : type_to_ura_name(tokens[j]->type);
      if (!base_name) continue;
      bool found = false;
      for (int k = 0; k < entry_count; k++)
         if (strcmp(entries[k].base_name, base_name) == 0 && entries[k].depth == depth) {
            found = true;
            break;
         }
      if (!found && entry_count < 64) {
         entries[entry_count].base_name = base_name;
         entries[entry_count].depth     = depth;
         entry_count++;
         // For depth > 1, ensure all intermediate depths exist
         for (int d = 1; d < depth; d++) {
            bool has = false;
            for (int k = 0; k < entry_count; k++)
               if (strcmp(entries[k].base_name, base_name) == 0 && entries[k].depth == d) {
                  has = true;
                  break;
               }
            if (!has && entry_count < 64) {
               entries[entry_count].base_name = base_name;
               entries[entry_count].depth     = d;
               entry_count++;
            }
         }
      }
   }
   list_tmp_count = 0;
   if (entry_count == 0) return;

   // Sort entries by depth ascending
   for (int i = 0; i < entry_count - 1; i++)
      for (int j = i + 1; j < entry_count; j++)
         if (entries[j].depth < entries[i].depth) {
            ListEntry tmp = entries[i];
            entries[i]    = entries[j];
            entries[j]    = tmp;
         }

   // Generate source and write to temp files
   for (int i = 0; i < entry_count; i++) {
      const char *base = entries[i].base_name;
      int         d    = entries[i].depth;
      char        elem_name[256];
      char        sname[256];
      if (d == 1) {
         snprintf(elem_name, sizeof(elem_name), "%s", base);
         snprintf(sname, sizeof(sname), "__list_%s", base);
      } else {
         snprintf(elem_name, sizeof(elem_name), "%s", base);
         for (int dd = 0; dd < d; dd++) {
            if (dd == 0) {
               snprintf(sname, sizeof(sname), "__list_%s", base);
            } else {
               snprintf(elem_name, sizeof(elem_name), "%s", sname);
               char tn[256];
               snprintf(tn, sizeof(tn), "__list_%s", sname);
               snprintf(sname, sizeof(sname), "%s", tn);
            }
         }
      }

      char *src = generate_list_source(elem_name, sname);
      debug(RED("GENERATED LIST SOURCE:\n%s\n"), src);

      // Prepend memory protos to the first file
      char tmppath[URA_MAX_SIZE];
      snprintf(tmppath, sizeof(tmppath), "/tmp/__ura_%s.ura", sname);
      File tmpf = fopen(tmppath, "w");
      if (tmpf) {
         if (i == 0) fputs("use \"@/memory\"\n\n", tmpf);
         fputs(src, tmpf);
         fclose(tmpf);
         snprintf(list_tmp_files[list_tmp_count++], URA_MAX_SIZE, "%s", tmppath);
      }
      free(src);
   }
}

char *compile(char *path_name) {
	debug(RED("===========================================\n"));
	debug(RED("START COMPILATION\n"));
	debug(RED("===========================================\n"));
	// reset ura_scope state for each file
	found_error = false;
	tcount      = 0;
	ecount      = 0;
	sccount     = 0;
	scope       = NULL;
	ura_scope   = NULL;

	char *file_name = realpath(path_name, NULL);
	if (check(file_name == NULL, "error resolving %s\n", path_name)) return NULL;
	// Two-pass: first tokenize to scan for list types, then retokenize with generated structs
	tokenize(file_name, 0);
	instantiate_list_types();
	if (list_tmp_count > 0) {
		// Reset token state and re-tokenize with generated structs first
		free_local_memory();
		ucount = 0;
		// Tokenize generated list struct files (includes memory.ura via use)
		for (int i = 0; i < list_tmp_count; i++) {
			calling_use++;
			tokenize(list_tmp_files[i], 0);
			calling_use--;
		}
		// Re-tokenize user file
		tokenize(file_name, 0);
	}

	for (int i = 0; tokens[i]; i++)
		debug(GREEN("%k\n"), tokens[i]);

#if AST
	if (found_error) return NULL;
	ura_scope = new_node(new_token(ID, -TAB));
	setName(ura_scope->token, "ura-scope");
	enter_scope(ura_scope);
	while (!find(END, 0) && !found_error)
		add_child(ura_scope, expr_node());
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");
	if (found_error) return NULL;
#endif

#if IR
	if (found_error) return NULL;
	// Pre-register all top-level functions for forward references
	for (int i = 0; !found_error && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type == FDEC && !find_function(child->token->name))
			add_function(scope, child);
	}
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_ir(ura_scope->children[i]);
	if (found_error) return NULL;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER IR\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

#if OPTIMIZE
	bool changed = true;
	int  op      = 0;
	while (changed) {
		changed = false;
		optimize_ir(ura_scope, &changed);
		if (changed) op++;
	}
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER OPTMIZING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; op && !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

#endif
#endif

	// build/  directory next to the source file
	char *dir  = dirname(strdup(path_name));
	char *base = basename(strdup(path_name));
	// strip .ura extension
	char *dot_ext = strrchr(base, '.');
	if (dot_ext) *dot_ext = '\0';

	// Preprocessed output mode
	if (prep_mode && !found_error) {
		char *build_dir = strjoin(dir, "/build", NULL);
		mkdir(build_dir, 0755);
		char *prep_path = strjoin(build_dir, "/", strjoin(base, ".prep.ura", NULL));
		emit_prep_file(ura_scope, prep_path);
		printf("Wrote %s\n", prep_path);
		return NULL;
	}

#if ASM
	if (found_error) return NULL;

	char *build_dir = strjoin(dir, "/build", NULL);
	mkdir(build_dir, 0755);
	char *ll_path = strjoin(build_dir, "/", strjoin(base, ".ll", NULL));

	char *moduleName = resolve_path(path_name);
	init(moduleName);
	free(moduleName);

	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_asm(ura_scope->children[i]);

	finalize(ll_path);
	free(build_dir);
	return found_error ? NULL : ll_path;
#endif
	return NULL;
}

int main(int argc, char **argv) {
	ura_lib = getenv("URA_LIB");
	if (check(!ura_lib, "URA_LIB not set")) return 1;
	argv0 = argv[0];
	if (check(argc < 2,
	          "usage: ura <file.ura> [file2.ura ...] [-O0|-O1|-O2|-O3|-Os|-Oz] [-san] [-o output]"))
		return 1;

	char *output  = NULL;
	bool  no_exec = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-O0") == 0)            op_flags = PASSES_O0;
		else if (strcmp(argv[i], "-O1") == 0)       op_flags = PASSES_O1;
		else if (strcmp(argv[i], "-O2") == 0)       op_flags = PASSES_O2;
		else if (strcmp(argv[i], "-O3") == 0)       op_flags = PASSES_O3;
		else if (strcmp(argv[i], "-Os") == 0)       op_flags = PASSES_Os;
		else if (strcmp(argv[i], "-Oz") == 0)       op_flags = PASSES_Oz;
		else if (strcmp(argv[i], "-san") == 0)      enable_san = true;
		else if (strcmp(argv[i], "-no-exec") == 0)  no_exec = true;
		else if (strcmp(argv[i], "-no-debug") == 0) enable_debug = false;
		else if (strcmp(argv[i], "-prep") == 0)     prep_mode = true;
		else if (strcmp(argv[i], "-o") == 0)        {
			check(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		} else if (argv[i][0] != '-' && strlen(argv[i]) > 4 &&
		           strcmp(argv[i] + strlen(argv[i]) - 4, ".ura") == 0) {
			resize_array(src_files, char *, src_size, src_count);
			src_files[src_count++] = argv[i];
		} else {
			check(argv[i][0] == '-', "unknown flag: %s\n", argv[i]);
			check(argv[i][0] != '-', "unvalid file: %s\n", argv[i]);
		}
	}

	char default_output[URA_MAX_SIZE];
	if (!output && src_count > 0) {
		char *base = basename(src_files[0]);
		snprintf(default_output, sizeof(default_output), "%s", base);
		char *dot = strrchr(default_output, '.');
		if (dot) *dot = '\0';
		output = default_output;
	}
	if (!output) output = "exe.out";

#if ASM
	char final_cmd[URA_MAX_SIZE];
	int  pos = 0;

	pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, "clang");
	if (enable_san)
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos,
		                " -fsanitize=address,undefined -fno-omit-frame-pointer -g");
#endif

	for (int i = 0; i < src_count && !found_error; i++) {
		char *ll = compile(src_files[i]);
		if (prep_mode) continue;
#if ASM
		if (!no_exec && !found_error) {
			if (enable_san) pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
			else {
				char s[URA_MAX_SIZE];
				snprintf(s, sizeof(s), "%s", ll);
				char *dot = strrchr(s, '.');
				if (dot) strcpy(dot, ".s");

				char llc_cmd[URA_MAX_SIZE];
				snprintf(llc_cmd, sizeof(llc_cmd), "llc \"%s\" -o \"%s\"", ll, s);
				if (check(system(llc_cmd) != 0, "llc failed for %s\n", ll)) {
					free(ll);
					break;
				}
				pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", s);
			}
		}
		free(ll);
#endif
		free_local_memory();
	}
#if ASM
	for (int i = 0; i < lcount && !found_error; i++) {
		char env_name[URA_MAX_SIZE];
		snprintf(env_name, sizeof(env_name), "URA_LINK_%s", links[i]);
		char *flags = getenv(env_name);
		if (flags) pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " %s", flags);
	}

	if (!no_exec && !found_error && !prep_mode) {
		pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " -o \"%s\"", output);
		if (!check(system(final_cmd) != 0, "linking failed\n")) {
			char full[URA_MAX_SIZE];
			realpath(output, full);

			if (enable_san) {
				char *asan_file = getenv("ASAN_FILE");
				if (asan_file) {
					setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
					setenv("LSAN_OPTIONS", strjoin("suppressions=", asan_file, NULL), 1);
				} else setenv("ASAN_OPTIONS", "detect_leaks=1", 1);
			}
		}
	}
#endif
	free(src_files);
	free_global_memory();
	return found_error;
}
