#include "header.h"

// Global variable definitions
bool             found_error;

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
char            *argv0;
char            *ura_lib;
int              calling_use;

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
	case '\\': buf[(*j)++] = '\\'; return s + 1;  // backslash
	case '"':  buf[(*j)++] = '"'; return s + 1;   // double quote
	case '\'': buf[(*j)++] = '\''; return s + 1;  // single quote
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
		                    {"as", AS},         {0, 0}};
		if (match_table(new, new->name, keywords, false)) goto id_done;

		Types keywords2[] = {{"and", AND},          {"or", OR},         {"is", EQUAL},
		                     {"not", NOT},          {"typeof", TYPEOF}, {"sizeof", SIZEOF},
		                     {"output", OUTPUT},    {"stack", STACK},   {"heap", HEAP},
		                     {"array", ARRAY_TYPE}, {"null", NULLABLE}, {0, 0}};
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
void tokenize(char *file_name) {
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
	    {"...", VARIADIC},  {".", DOT},         {":", DOTS},         {"+=", ADD_ASSIGN},
	    {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN}, {"/=", DIV_ASSIGN},  {"%=", MOD_ASSIGN},
	    {"!=", NOT_EQUAL},  {"!", NOT},         {"==", EQUAL},       {"<<", LSHIFT},
	    {">>", RSHIFT},     {"<=", LESS_EQUAL}, {">=", GREAT_EQUAL}, {"<", LESS},
	    {">", GREAT},       {"=", ASSIGN},      {"+", ADD},          {"-", SUB},
	    {"*", MUL},         {"/", DIV},         {"%", MOD},          {"(", LPAR},
	    {")", RPAR},        {"[", LBRA},        {"]", RBRA},         {",", COMA},
	    {"&&", AND},        {"||", OR},         {"&", BAND},         {"|", BOR},
	    {"^", BXOR},        {"~", BNOT},        {0, (Type)0},
	};

	int  space    = 0;
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
				space    = 0;
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
				tokenize(use);
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
		if (check(to == NULL || !to->is_dec, "expected data type after to")) return syntax_error();
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
	while ((token = find(DOT, LBRA, 0))) {
		Node *node = new_node(token);
		if (token->type == DOT) {
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
		} else // LBRA
		{
			node->token->type = ACCESS;
			node->left        = left;
			node->right       = expr_node();
			check(!node->right, "expected something between []");
			expect_token(RBRA, "expected right bracket");
		}
		left = node;
	}
	return left;
}

// ----------------------------------------------------------------------------
// prime_node sub-parsers (token already consumed by caller)
// ----------------------------------------------------------------------------

// [INT/CHARS/…] varname               variable declaration
// [INT] [42]                           literal value
// [STRUCT_CALL] varname → StructType  struct variable declaration
// [TUPLE_UNPACK]                       a int, b chars = fn()
// ├──[INT] a
// ├──[CHARS] b
// └──left: [FCALL] fn ret [TUPLE]
// [FCALL] name ret [TYPE]             function call
// └──[ARGS]
//    ├──arg1
//    └──arg2
static Node *parse_var_dec(Token *token) {
	// Literal value: "hello", 1, 'c'
	if (token->type != ID && !token->is_dec && !token->name) return new_node(token);
	// int, char, chars, etc. declaration keyword
	if (token->is_dec) return new_node(token);

	// variable declaration: name type [ref] [= ...]
	if (token->type == ID && is_data_type(tokens[ecount])) {
		Token *tmp    = find(DATA_TYPES, 0); // skip data type
		bool   is_ref = find(REF, 0) != NULL;
		if (tmp->type == ARRAY_TYPE) {
			expect_token(LBRA, "expected [ after array");
			int depth = 1;
			while (find(LBRA, 0))
				depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			check(!elem_type, "expected element type in array type");
			for (int i = 0; i < depth; i++)
				expect_token(RBRA, "expected ] in array type");
			tmp->Array.elem_type = elem_type->type;
			tmp->Array.depth     = depth;
			tmp->retType         = ARRAY_TYPE;
			if (elem_type->type == ID) {
				Node *sd = get_struct(elem_type->name);
				if (sd) {
					tmp->Array.elem_type  = STRUCT_CALL;
					tmp->Array.struct_ptr = sd;
				}
			}
		}
		setName(tmp, token->name);
		tmp->is_dec = true;
		tmp->is_ref = is_ref;
		if (is_ref) {
			expect_token(ASSIGN, "'%s': ref must be initialized at declaration, e.g: x int ref = a",
			             token->name);
			ecount--;
		}
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
				if (ntype->type == ARRAY_TYPE) {
					expect_token(LBRA, "expected [ after array");
					int depth = 1;
					while (find(LBRA, 0))
						depth++;
					Token *elem_type = find(DATA_TYPES, ID, 0);
					check(!elem_type, "expected element type in array type");
					for (int i = 0; i < depth; i++)
						expect_token(RBRA, "expected ] in array type");
					ntype->Array.elem_type = elem_type->type;
					ntype->Array.depth     = depth;
					ntype->retType         = ARRAY_TYPE;
					if (elem_type->type == ID) {
						Node *sd = get_struct(elem_type->name);
						if (sd) {
							ntype->Array.elem_type  = STRUCT_CALL;
							ntype->Array.struct_ptr = sd;
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
			// Function main:
			//    + children: code bloc
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
		} else {
			// Function call:
			//    + left:
			//       + children: parameters
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
	return new_node(token);
}

// [STRUCT_DEF] Name
// ├──[FIELD_TYPE] field1
// ├──[FIELD_TYPE] field2
// └──methods[]: [FDEC] Name.method …
static Node *parse_struct_def(Token *token) {
	// Struct def layout:
	//    + children: attributes
	//    + methods[]: method FDEC nodes
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
		Node *child = prime_node();
		if (!child)                     return syntax_error();
		if (child->token->type == FDEC) {
			resize_array(child->functions, Node *, child->functions_size, child->functions_count);
			if (strcmp(child->token->name, "init") == 0) {
				if (check(child->token->retType != VOID, "struct '%s': init() must return void",
				          node->token->name))
					return syntax_error();
				if (check(child->left->children_count != 1,
				          "struct '%s': init() must take no parameters", node->token->name))
					return syntax_error();
				node->token->has_init = true;
			} else if (strcmp(child->token->name, "clean") == 0) {
				if (check(child->token->retType != VOID, "struct '%s': clean() must return void",
				          node->token->name))
					return syntax_error();
				if (check(child->left->children_count != 1,
				          "struct '%s': clean() must take no parameters", node->token->name))
					return syntax_error();
				node->token->has_clean = true;
			}
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

// [ENUM_DEF] Name
// ├──[INT] VariantA  (value=0, is_global)
// ├──[INT] VariantB  (value=1, is_global)
// └──...
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

// [IF]
// ├──left:     condition
// ├──children: body stmts
// └──right: [ELIF]
//           ├──left:     condition
//           ├──children: body stmts
//           └──right: [ELSE]
//                     └──children: body stmts
static Node *parse_if(Token *token) {
	// if/elif/else layout:
	//    + left    : condition
	//    + children: code bloc
	//    + right   : elif/else node (same layout)
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

// [FDEC] name ret [TYPE]
// ├──[ARGS]
// │  ├──[TYPE] param1
// │  └──[STRUCT_CALL] param2    (struct param)
// └──children: body stmts
//
// [FDEC] name ret [TUPLE]       (tuple return)
// ├──[ARGS]
// │  └──…
// └──children: body stmts
static Node *parse_fdec(Token *token) {
	// Function declaration layout:
	//    + left:
	//       + children: arguments
	//    + children: code block
	Node  *node  = new_node(token);
	Token *fname = find(ID, 0);
	if (check(!fname, "expected identifier after fn declaration")) return syntax_error();

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
			if (check(!name, "expected identifier in function argument %s", fname->name))
				return syntax_error();

			Token *data_type = find(DATA_TYPES, ID, 0);
			bool   is_ref    = find(REF, 0) != NULL;
			if (check(!data_type, "expected data type in function argument")) break;
			if (data_type->type == ID)                                        {
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
			}
			curr->token->is_dec   = true;
			curr->token->is_param = true;
			add_child(node->left, curr);
		}
		if (tokens[ecount]->type != RPAR) expect_token(COMA, "expected coma");
	}
	check(!found_error && last->type != RPAR, "expected ) after function declaration");

	// If inside a struct, prepend implicit 'self' parameter
	if (struct_owner) {
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
			if (tokens[ecount]->type == ARRAY_TYPE) {
				Token *t = tokens[ecount++];
				expect_token(LBRA, "expected [ in tuple array type");
				Token *et = find(DATA_TYPES, ID, 0);
				if (check(!et, "expected element type in tuple array")) return syntax_error();
				expect_token(RBRA, "expected ] in tuple array type");
				t->Array.elem_type = et->type;
				t->retType         = ARRAY_TYPE;
				if (et->type == ID) {
					Node *sd = get_struct(et->name);
					if (sd) {
						t->Array.elem_type  = STRUCT_CALL;
						t->Array.struct_ptr = sd;
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
	setName(node->token, fname->name);

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

// [RETURN] ret [TYPE]        (single value)
// └──left: expr
//
// [RETURN] ret [TUPLE]       (multiple values)
// ├──expr1
// └──expr2
//
// [RETURN] ret [VOID]        (void function — left = [VOID] copy)
// └──left: [VOID]
static Node *parse_return(Token *token) {
	// TODO: check if return type is compatible with function in current scope (done in gen_ir)
	Node *node = new_node(token);
	for (int i = sccount; i >= 0; i--) {
		Node *curr = level_scope[i];
		if (curr->token->type == FDEC)     {
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

	if ((token = find(ID, DATA_TYPES, 0))) return parse_var_dec(token);
	if ((token = find(STRUCT_DEF, 0)))     return parse_struct_def(token);
	if ((token = find(ENUM_DEF, 0)))       return parse_enum_def(token);
	if ((token = find(IF, 0)))             return parse_if(token);

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
	if ((token = find(OUTPUT, 0))) {
		node = new_node(token);
		expect_token(LPAR, "output: expected (");
		node->left     = new_node(new_token(ARGS, token->space));
		Token *end_tok = NULL;
		while (!found_error && !(end_tok = find(RPAR, END, 0))) {
			Node *curr = expr_node();
			add_child(node->left, curr);
			find(COMA, 0);
		}
		if (check(!found_error && end_tok->type != RPAR, "output: expected )")) return syntax_error();
		node->token->retType = VOID;
		return node;
	}
	if ((token = find(PROTO, 0)))                             {
		if (includes(tokens[ecount]->type, FDEC, STRUCT_DEF, 0)) tokens[ecount]->is_proto = true;
		else check(1, "expected <fn> or <struct> after proto");
		return expr_node();
	}

	if ((token = find(FDEC, 0)))   return parse_fdec(token);
	if ((token = find(RETURN, 0))) return parse_return(token);

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
	if ((token = find(LPAR, 0)))       {
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
	// TODO: add disable default if next is assignment
#if 0
	case INT: case BOOL: case CHAR: case FLOAT: case LONG: case CHARS:
	case PTR: case STRUCT_CALL:
	{
		if (node->token->is_dec && node->token->used == 0)
		{
			debug(RED("remove unused var %k\n"), node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
	case ADD:        case SUB: case MUL: case DIV: case MOD: case AND: case OR:
	case NOT:        case EQUAL: case NOT_EQUAL: case LESS: case GREAT:
	case LESS_EQUAL: case GREAT_EQUAL:
	{
		if (node->token->used == 0)
		{
			debug(RED("remove unused op %k\n"), node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
	case ASSIGN:
	{
		bool cond = node->left == NULL || (node->left
		                                   && node->left->token
		                                   && node->left->token->used == 0
		                                   && is_data_type(node->left->token));
		if (cond)
		{
			debug(RED("remove unused assign %k\n"), node->token);
			unuse(node);
			free_node(node);
			*changed = true;
			return NULL;
		}
		break;
	}
#endif
	default: break;
	}

	return node;
}

// ----------------------------------------------------------------------------
// gen_ir helpers — extracted from large switch cases
// ----------------------------------------------------------------------------

// [FCALL] obj.method ret [TYPE]   (is_method_call = true)
// └──[ARGS]
//    ├──arg1
//    ├──arg2
//    └──[STRUCT_CALL] obj         ← last child = receiver
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
	if (!func) return;
	func->token->used++;
	node->token->Fcall.ptr = func;
	func                   = copy_node(func);

	// first arg already gen_ir'd (obj), do rest
	obj_node->token->used++;
	for (int i = 0; !found_error && i < node->left->children_count - 1; i++) {
		Node *carg = node->left->children[i];
		gen_ir(carg);
		if (check(carg->token->type == ID, "Indeclared variable %s", carg->token->name)) break;
		carg->token->used++;
	}
	free_node(func);
	set_ret_type(node);
}

// [FCALL] name ret [TYPE]
// └──[ARGS]
//    ├──arg1
//    └──arg2
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
				if (check(!src->name || src->type == FCALL,
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

// [DOT] ret [FIELD_TYPE]
// ├──left:  [STRUCT_CALL] obj
// └──right: [ID] field   → resolved to field's type + index
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
				node->right->token->Array.elem_type = src_child->token->Array.elem_type;
				node->right->token->Array.depth     = src_child->token->Array.depth;
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
		node->token->Array.elem_type = node->right->token->Array.elem_type;
		node->token->Array.depth     = node->right->token->Array.depth;
	}
}

// ----------------------------------------------------------------------------
// Semantic analysis / name resolution pass
// ----------------------------------------------------------------------------

void gen_ir(Node *node) {
	if (found_error) return;
	Node *left  = node->left;
	Node *right = node->right;
	switch (node->token->type) {
	// [ID] varname   → resolved to declared token (type, llvm.elem, etc.)
	case ID: {
		Token *find = get_variable(node->token->name);
		if (find) node->token = find;
		break;
	}
	// [STRUCT_CALL] varname → StructType   (is_dec = declaration | usage)
	case STRUCT_CALL: {
		Node *src               = get_struct(node->token->Struct.ptr->token->name);
		node->token->Struct.ptr = src;
		if (node->token->is_dec) {
			if (sccount == 1)       node->token->is_global = true;
			else src->token->used++; // local declaration → always emit struct def
			new_variable(node->token);
			if (src->token->has_init) {
				// TODO: to be checked later
				char *qname   = strjoin(src->token->name, ".init", NULL);
				Node *init_fn = get_function(qname);
				free(qname);
				if (init_fn) init_fn->token->used++;
			}
		}
		break;
	}
	// [INT/BOOL/CHAR/CHARS/FLOAT/LONG/VOID] name   (is_dec → register var)
	// [INT] [42]                                     (literal → mark used)
	case NULLABLE: {
		node->token->retType = CHARS; // null is a null chars (i8*) pointer
		node->token->used++;
		break;
	}
	case INT:   case BOOL: case CHAR: case ARRAY_TYPE: case FLOAT: case LONG:
	case CHARS: case PTR: case VOID: case SHORT: {
		if (node->token->is_dec) {
			if (sccount == 1)       node->token->is_global = true;
			new_variable(node->token);
		} else node->token->used++;
		break;
	}
	// [TUPLE_UNPACK]
	// ├──[TYPE] a
	// ├──[TYPE] b
	// └──left: [FCALL] fn ret [TUPLE]
	case TUPLE_UNPACK: {
		// Register each LHS variable, then process RHS
		for (int i = 0; i < node->children_count; i++) {
			Node *lhs = node->children[i];
			if (lhs->token->is_dec) {
				if (sccount == 1)      lhs->token->is_global = true;
				new_variable(lhs->token);
			}
		}
		gen_ir(node->left); // RHS function call
		break;
	}
	// [ASSIGN] ret [LEFT_TYPE]
	// ├──left:  lvalue
	// └──right: rvalue
	case ASSIGN: {
		// TODO: check compatibility
		// TODO: add all ir_bound checks in &ll places
		gen_ir(left);
		gen_ir(right);
		if (!compatible(left->token, right->token)) {
			check(1, "type mismatch: cannot assign %k to %k — use '%k as %s'",
			      right->token, left->token, right->token, to_string(left->token->type));
			break;
		}
		bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound) &&
		                (!right->token->name || right->token->type == FCALL);
		if (check(error_op, "'%s': ref must be bound to a named variable", left->token->name)) break;
		node->token->retType = left->token->retType;
		if (left->token->is_ref) left->token->ir_bound = true;
		break;
	}
	// [+=/-=/etc.] ret [LEFT_TYPE]
	// ├──left:  lvalue
	// └──right: rvalue
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN: {
		gen_ir(left);
		bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound);
		if (check(error_op, "'%s': ref must be bound to a named variable", left->token->name)) break;
		gen_ir(right);
		node->token->retType = left->token->retType;
		node->token->used++;
		node->left->token->used++;
		node->right->token->used++;
		break;
	}
	// [NOT] ret [BOOL]        [BNOT] ret [LEFT_TYPE]
	// └──left: expr           └──left: expr
	case NOT: case BNOT: {
		// TODO: left must be boolean
		gen_ir(left);
		node->token->used++;
		node->left->token->used++;
		if (node->token->type == NOT) node->token->retType = BOOL;
		else node->token->retType = left->token->retType ? left->token->retType : left->token->type;
		break;
	}
	// [ADD/SUB/MUL/DIV/MOD/AND/OR/BAND/BOR/BXOR/LSHIFT/RSHIFT/…] ret [TYPE]
	// ├──left:  expr
	// └──right: expr
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
		break;
	}
	// [WHILE]
	// ├──left:     condition
	// └──children: body stmts
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
	// [IF]
	// ├──left:     condition
	// ├──children: body stmts
	// └──right: [ELIF]
	//           ├──left:     condition
	//           ├──children: body stmts
	//           └──right: [ELSE]
	//                     └──children: body stmts
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
	// [FDEC] name ret [TYPE]
	// ├──[ARGS]
	// │  ├──[TYPE] param1
	// │  └──[TYPE] param2
	// └──children: body stmts
	case FDEC: {
		new_function(node);
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
	// [FCALL] name ret [TYPE]   →   ir_regular_call
	// [FCALL] obj.method        →   ir_method_call  (is_method_call=true)
	case FCALL: {
		if (node->token->is_method_call && node->left) ir_method_call(node);
		else ir_regular_call(node);
		break;
	}
	// [AS] ret [TARGET_TYPE]
	// ├──left:  expr
	// └──right: [TARGET_TYPE]
	case AS: {
		gen_ir(node->left);
		node->token->retType = node->right->token->type;
		break;
	}
	// [DOT] ret [FIELD_TYPE]     →   ir_dot
	// ├──left:  [STRUCT_CALL] obj
	// └──right: [ID] field
	case DOT: {
		ir_dot(node);
		break;
	}
	// [ACCESS] ret [ELEM_TYPE]
	// ├──left:  array / chars
	// └──right: index expr
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
		default: check(1, "handle this case %s", to_string(node->left->token->type)); break;
		}
		node->token->retType = retType;
		break;
	}
	// [RETURN] ret [TYPE]        [RETURN] ret [TUPLE]
	// └──left: expr              ├──expr1
	//                            └──expr2
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
	// [OUTPUT]
	// └──[ARGS]
	//    ├──fmt expr
	//    └──value exprs...
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
	// [TYPEOF] ret [CHARS]       [SIZEOF] ret [INT]
	// └──left: expr              └──left: expr
	case TYPEOF: case SIZEOF: {
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
	// [STACK] ret [ARRAY] elem_type    [HEAP] ret [ARRAY] elem_type
	// └──left: size expr               └──left: count expr
	case STACK: case HEAP: {
		for (int i = 0; i < node->children_count; i++) {
			gen_ir(node->children[i]);
			node->children[i]->token->used++;
		}
		node->token->used++;
		break;
	}
	// [STRUCT_DEF] Name
	// ├──[FIELD_TYPE] field1
	// ├──[FIELD_TYPE] field2
	// └──methods[]: [FDEC] ...
	case STRUCT_DEF: {
		for (int i = 0; node && i < node->functions_count; i++)
			gen_ir(node->functions[i]);
		break;
	}
	// [ENUM_DEF] Name
	// ├──[INT] VariantA  (ura_scope constant)
	// └──...
	case ENUM_DEF: {
		for (int i = 0; i < node->children_count; i++)
			node->children[i]->token->used++;
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
	debug(CYAN("get variable [%s] from scope %k\n"), name, scope->token);
	for (int j = sccount; j > 0; j--) {
		Node *scope = level_scope[j];
		for (int i = 0; i < scope->variables_count; i++)
			if (strcmp(scope->variables[i]->name, name) == 0) return scope->variables[i];
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

Node *get_function(char *name) {
	for (int j = sccount; j > 0; j--) {
		Node *scope = level_scope[j];
		for (int i = 0; i < scope->functions_count; i++)
			if (strcmp(scope->functions[i]->token->name, name) == 0) return scope->functions[i];
	}
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
	if (rt == CHARS && includes(rt, ARRAY_TYPE, ARRAY, 0) && left->Array.elem_type == CHAR)
		return true;

	if (lt == ARRAY_TYPE && rt == ARRAY) return true;
	if (lt == ARRAY && rt == ARRAY_TYPE) return true;

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

void emit_scope_clean(Node *scope, int from) {
	for (int i = from; i < scope->variables_count; i++) {
		Token *var = scope->variables[i];
		if (var->type != STRUCT_CALL || !var->llvm.elem) continue;
		if (var->is_ref)                                 continue; // refs are borrowed, not owned — caller cleans up
		Node *sd       = var->Struct.ptr;
		char *qname    = strjoin(sd->token->name, ".clean", NULL);
		Value clean_fn = LLVMGetNamedFunction(module, qname);
		free(qname);
		if (!clean_fn) continue;
		Value args[] = {var->llvm.elem};
		LLVMBuildCall2(builder, LLVMGlobalGetValueType(clean_fn), clean_fn, args, 1, "");
	}
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
		fmt[(*functions_count)++] = '{';
		fmt[(*functions_count)++] = ' ';
		Node *sd                  = tok->Struct.ptr;
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
		if (node->token->is_dec)      {
			if (node->token->is_global)  {
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

				Node *src     = node->token->Struct.ptr;
				char *qname   = strjoin(src->token->name, ".init", NULL);
				Value init_fn = LLVMGetNamedFunction(module, qname);
				free(qname);
				if (init_fn) {
					Value args[] = {node->token->llvm.elem};
					LLVMBuildCall2(builder, LLVMGlobalGetValueType(init_fn), init_fn, args, 1, "");
				}
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
		if (node->token->is_dec)      {
			if (node->token->is_global)  {
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
		if (left->token->is_global && left->token->is_dec) {
			gen_asm(left);
			gen_asm(right);
			if (left->token->llvm.elem && right->token->llvm.elem &&
			    LLVMIsConstant(right->token->llvm.elem))
				LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
			return;
		}
		if (left->token->is_ref && !left->token->asm_bound && !left->token->is_param) {
			if (!left->token->llvm.elem)                                                 _alloca(left->token);
			gen_asm(right);
			LLVMBuildStore(builder, right->token->llvm.elem, left->token->llvm.elem);
			left->token->asm_bound = true;
			left->token->is_dec    = false;
			break;
		}

		gen_asm(left);
		gen_asm(right);

		// propagate multi-dim info from stack/heap allocation to the variable
		if (includes(right->token->type, STACK, HEAP, 0) && right->token->llvm.dim_count > 1) {
			left->token->llvm.dim_count = right->token->llvm.dim_count;
			for (int i = 0; i < right->token->llvm.dim_count; i++)
				left->token->llvm.dim_sizes[i] = right->token->llvm.dim_sizes[i];
		}

		TypeRef type = get_llvm_type(right->token);

		Value   val;
		if (right->token->is_ref) val = load_value(right->token);
		else {
			load_if_necessary(right);
			val = right->token->llvm.elem;
		}

		if (left->token->is_ref) {
			Value ptr =
			    LLVMBuildLoad2(builder, LLVMPointerType(type, 0), left->token->llvm.elem, "ref_ptr");
			LLVMBuildStore(builder, val, ptr);
		} else LLVMBuildStore(builder, val, left->token->llvm.elem);

		break;
	}
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN: {
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
		gen_asm(left);
		gen_asm(right);

		load_if_necessary(left);
		load_if_necessary(right);

		Value lref = left->token->llvm.elem;
		Value rref = right->token->llvm.elem;

		if (left->token->is_ref && !left->token->llvm.is_loaded)   lref = load_value(left->token);
		if (right->token->is_ref && !right->token->llvm.is_loaded) rref = load_value(right->token);

		node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
		if (includes(node->token->type, COMPARISON_OPS, 0)) node->token->retType = BOOL;
		else node->token->retType = left->token->retType ? left->token->retType : left->token->type;
		break;
	}
	case STACK: {
		Token   tmp    = {.type = node->token->Array.elem_type};
		TypeRef elem_t = get_llvm_type(&tmp);
		int     depth  = node->token->Array.depth;

		// evaluate each dimension, compute total element count, store dim_sizes
		Value total = LLVMConstInt(i32, 1, 0);
		for (int i = 0; i < depth; i++) {
			gen_asm(node->children[i]);
			load_if_necessary(node->children[i]);
			Value dv                       = node->children[i]->token->llvm.elem;
			node->token->llvm.dim_sizes[i] = dv;
			total                          = LLVMBuildMul(builder, total, dv, "dim");
		}
		node->token->llvm.dim_count = depth;

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
			Value dv                       = node->children[i]->token->llvm.elem;
			node->token->llvm.dim_sizes[i] = dv;
			total                          = LLVMBuildMul(builder, total, dv, "dim");
		}
		node->token->llvm.dim_count = depth;

		TargetData td        = LLVMGetModuleDataLayout(module);
		size_t     elem_size = LLVMABISizeOfType(td, elem_t);
		Value      total_bytes =
		    LLVMBuildMul(builder, total, LLVMConstInt(i32, (unsigned)elem_size, 0), "bytes");

		node->token->llvm.elem       = allocate_heap(total_bytes, elem_t, "heap");
		node->token->llvm.is_set     = true;
		node->token->llvm.array_size = total;
		break;
	}
	case FCALL: {
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
					// non-ref → non-ref: normal load
					load_if_necessary(argNodes[i]);
					args[i] = argNodes[i]->token->llvm.elem;

					// ARM64 ABI: proto calls pass small structs as i64 via memcpy
					bool is_proto_call = node->token->Fcall.ptr->token->is_proto;
					bool param_is_struct =
					    (i < dec_args->children_count) &&
					    includes(dec_args->children[i]->token->type, STRUCT_CALL, STRUCT_DEF, 0);
					if (is_proto_call && param_is_struct) {
						TypeRef st_type = get_llvm_type(dec_args->children[i]->token);
						Value   st_ptr  = LLVMBuildAlloca(builder, st_type, "st_slot");
						LLVMBuildStore(builder, argNodes[i]->token->llvm.elem, st_ptr);
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
		break;
	}
	case FDEC: {
		// debug("================================\n");
		// pnode(node, "");
		// debug("\n================================\n");
		enter_scope(node);
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
		int      param_count = node->left->children_count;
		int      fixed_count = param_count;
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

		char   *fname = node->token->name;
		TypeRef funcType;
		if (strcmp(fname, "main") == 0) {
			// Only use (i32, ptr) signature if os ura_scope is actually accessed
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
				TypeRef argv_type  = LLVMPointerType(LLVMPointerType(i8, 0), 0); // char**
				TypeRef main_pts[] = {i32, argv_type};
				funcType           = LLVMFunctionType(retType, main_pts, 2, false);
			} else funcType = LLVMFunctionType(retType, NULL, 0, false);
		} else funcType = LLVMFunctionType(retType, paramTypes, _count, node->token->is_variadic);
		Value existingFunc = LLVMGetNamedFunction(module, fname);
		if (existingFunc) node->token->llvm.elem = existingFunc;
		else node->token->llvm.elem = _add_function(fname, funcType);
		node->token->llvm.funcType = funcType;

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

			// Auto-insert nested struct field init calls before user body
			{
				int  functions_size = strlen(fname);
				bool is_init_fn =
				    functions_size > 5 && strcmp(fname + functions_size - 5, ".init") == 0;
				if (is_init_fn && fixed_count >= 1) {
					Token *self_tok = node->left->children[0]->token;
					if (self_tok->type == STRUCT_CALL && self_tok->is_ref) {
						Node   *sd       = self_tok->Struct.ptr;
						TypeRef st_type  = sd->token->llvm.stType;
						TypeRef ptr_type = LLVMPointerType(st_type, 0);
						Value   self_ptr = LLVMBuildLoad2(builder, ptr_type, self_tok->llvm.elem, "self");
						for (int i = 0; i < sd->children_count; i++) {
							Token *field = sd->children[i]->token;
							if (field->type != STRUCT_CALL) continue;
							Node *fsd   = field->Struct.ptr;
							char *qname = strjoin(fsd->token->name, ".init", NULL);
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

			// Inject ura_scope struct init calls at the start of main
			if (strcmp(fname, "main") == 0) {
				for (int i = 0; i < ura_scope->children_count; i++) {
					Node *child = ura_scope->children[i];
					if (child->token->type != STRUCT_CALL || !child->token->is_global) continue;
					if (child->token->used == 0)                                       continue;
					char *qname  = strjoin(child->token->Struct.ptr->token->name, ".init", NULL);
					Value init_f = LLVMGetNamedFunction(module, qname);
					free(qname);
					if (!init_f) continue;
					Value args[] = {child->token->llvm.elem};
					LLVMBuildCall2(builder, LLVMGlobalGetValueType(init_f), init_f, args, 1, "");
				}
				// Populate os.argc and os.argv from main params (only if os is used)
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

				if (curr->right)                       {
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
		emit_scope_clean(scope, 0);

		// Auto-insert nested field clean calls for .clean methods before return
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
				bool  is_clean_f = fn_len > 6 && strcmp(fn_name + fn_len - 6, ".clean") == 0;
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
							char *qname = strjoin(fsd->token->name, ".clean", NULL);
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
					char *qname   = strjoin(child->token->Struct.ptr->token->name, ".clean", NULL);
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

		Value result;

		bool srcIsFloat = (sourceKind == FloatType || sourceKind == DoubleType);
		bool tgtIsFloat = (targetKind == FloatType || targetKind == DoubleType);

		// Same type — no-op
		if (sourceType == targetType)
			result = source;
		// Integer → Integer (trunc / sign-extend / same)
		else if (sourceKind == IntegerType && targetKind == IntegerType) {
			unsigned srcBits = LLVMGetIntTypeWidth(sourceType);
			unsigned tgtBits = LLVMGetIntTypeWidth(targetType);
			if      (srcBits > tgtBits) result = LLVMBuildTrunc(builder, source, targetType, "as");
			else if (srcBits < tgtBits) result = LLVMBuildSExt(builder, source, targetType, "as");
			else                        result = source;
		}
		// Integer → Float / Double  (signed)
		else if (sourceKind == IntegerType && tgtIsFloat)
			result = LLVMBuildSIToFP(builder, source, targetType, "as");
		// Float / Double → Integer  (signed, truncating toward zero)
		else if (srcIsFloat && targetKind == IntegerType)
			result = LLVMBuildFPToSI(builder, source, targetType, "as");
		// Float ↔ Double
		else if (srcIsFloat && tgtIsFloat) {
			if   (sourceKind == FloatType) result = LLVMBuildFPExt(builder, source, targetType, "as");
			else                           result = LLVMBuildFPTrunc(builder, source, targetType, "as");
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
	case STRUCT_DEF: {
		if (node->token->used == 0) return;
		// Ensure nested struct field types are also emitted
		for (int i = 0; i < node->children_count; i++) {
			Token *ft = node->children[i]->token;
			if (ft->type == STRUCT_CALL && ft->Struct.ptr && ft->Struct.ptr->token->used == 0) {
				ft->Struct.ptr->token->used++;
				gen_asm(ft->Struct.ptr);
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

		if (!node->token->has_init) {
			char *fname = strjoin(node->token->name, ".init", NULL);
			Value fn    = _add_function(fname, lc_fn_type);
			free(fname);
			Block entry = LLVMAppendBasicBlockInContext(context, fn, "entry");
			LLVMPositionBuilderAtEnd(builder, entry);
			LLVMSetCurrentDebugLocation2(builder, NULL);
			Value self = LLVMGetParam(fn, 0);
			LLVMBuildStore(builder, LLVMConstNull(st_type), self);
			for (int i = 0; i < node->children_count; i++) {
				Token *field = node->children[i]->token;
				if (field->type != STRUCT_CALL) continue;
				Node *fsd   = field->Struct.ptr;
				char *qname = strjoin(fsd->token->name, ".init", NULL);
				Value ffn   = LLVMGetNamedFunction(module, qname);
				free(qname);
				if (!ffn) continue;
				Value indices[] = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, i, 0)};
				Value field_ptr = LLVMBuildGEP2(builder, st_type, self, indices, 2, field->name);
				Value args[]    = {field_ptr};
				LLVMBuildCall2(builder, LLVMGlobalGetValueType(ffn), ffn, args, 1, "");
			}
			LLVMBuildRetVoid(builder);
			node->token->has_init = true;
		}

		if (!node->token->has_clean) {
			char *fname = strjoin(node->token->name, ".clean", NULL);
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
				char *qname = strjoin(fsd->token->name, ".clean", NULL);
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
			    node->left->token->type != STACK) {
				leftValue = LLVMBuildLoad2(builder, p8, leftValue, "ptr_load");
			}
		}

		// Load the index (handles refs)
		load_if_necessary(node->right);
		Value rightRef = node->right->token->llvm.elem;

		// multi-dim flat array: compute stride, emit GEP to sub-array, propagate dims
		int left_depth = node->left->token->llvm.dim_count;
		if (left_depth > 1) {
			Value stride = LLVMConstInt(i32, 1, 0);
			for (int d = 1; d < left_depth; d++)
				stride = LLVMBuildMul(builder, stride, node->left->token->llvm.dim_sizes[d], "stride");
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
			node->token->llvm.dim_count   = left_depth - 1;
			for (int d = 1; d < left_depth; d++)
				node->token->llvm.dim_sizes[d - 1] = node->left->token->llvm.dim_sizes[d];
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
	tokenize(file_name);

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

#if ASM
	if (found_error) return NULL;

	// build/  directory next to the source file
	char *dir  = dirname(strdup(path_name));
	char *base = basename(strdup(path_name));
	// strip .ura extension
	char *dot = strrchr(base, '.');
	if (dot) *dot = '\0';

	// mkdir build/
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

	char *output       = NULL;
	bool  testing_mode = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-O0") == 0)           op_flags = PASSES_O0;
		else if (strcmp(argv[i], "-O1") == 0)      op_flags = PASSES_O1;
		else if (strcmp(argv[i], "-O2") == 0)      op_flags = PASSES_O2;
		else if (strcmp(argv[i], "-O3") == 0)      op_flags = PASSES_O3;
		else if (strcmp(argv[i], "-Os") == 0)      op_flags = PASSES_Os;
		else if (strcmp(argv[i], "-Oz") == 0)      op_flags = PASSES_Oz;
		else if (strcmp(argv[i], "-san") == 0)     enable_san = true;
		else if (strcmp(argv[i], "-no-exec") == 0) testing_mode = true;
		else if (strcmp(argv[i], "-testing") == 0) testing_mode = true;
		else if (strcmp(argv[i], "-o") == 0)       {
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
#if ASM
		if (!testing_mode) {
			if (enable_san)   pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " \"%s\"", ll);
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
	for (int i = 0; i < lcount; i++) {
		char env_name[URA_MAX_SIZE];
		snprintf(env_name, sizeof(env_name), "URA_LINK_%s", links[i]);
		char *flags = getenv(env_name);
		if (flags) pos += snprintf(final_cmd + pos, sizeof(final_cmd) - pos, " %s", flags);
	}

	if (!testing_mode && !found_error) {
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
