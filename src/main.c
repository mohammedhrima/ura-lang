#include "utils.c"

// TOKENIZE
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

		const Keyword keywords[] = {
		    {"int", INT, 1, 1}, {"bool", BOOL, 1, 1}, {"chars", CHARS, 1, 1}, {"char", CHAR, 1, 1},
		    {"float", FLOAT, 1, 1}, {"void", VOID, 1, 1}, {"long", LONG, 1, 1}, {"pointer", CHARS, 1, 1},
		    {"short", SHORT, 1, 1},

		    {"if", IF, 0, 0}, {"elif", ELIF, 0, 0}, {"else", ELSE, 0, 0}, {"while", WHILE, 0, 0},
		    {"fn", FDEC, 0, 0}, {"return", RETURN, 0, 0}, {"break", BREAK, 0, 0},
		    {"continue", CONTINUE, 0, 0}, {"ref", REF, 0, 0}, {"struct", STRUCT_DEF, 0, 0},
		    {"enum", ENUM_DEF, 0, 0}, {"proto", PROTO, 0, 0}, {"mod", MODULE, 0, 0},
		    {"operator", OPERATOR, 0, 0}, {"as", AS, 0, 0}, {"pub", PUB, 0, 0}, {"delete", DELETE, 0, 0},

		    {"and", AND, 0, 1}, {"or", OR, 0, 1}, {"is", EQUAL, 0, 1}, {"not", NOT, 0, 1},
		    {"typeof", TYPEOF, 0, 1}, {"sizeof", SIZEOF, 0, 1}, {"stack", STACK, 0, 1},
		    {"heap", HEAP, 0, 1}, {"array", ARRAY_TYPE, 0, 1}, {"List", LIST_TYPE, 0, 1},
		    {"null", NULLABLE, 0, 1},
		};

		if (strcmp(new->name, "True") == 0 || strcmp(new->name, "False") == 0) {
			new->type       = BOOL;
			new->Bool.value = (new->name[0] == 'T');
			setName(new, NULL);
			break;
		}

		for (size_t i = 0; i < sizeof(keywords) / sizeof(*keywords); i++) {
			if (strcmp(new->name, keywords[i].name) != 0) continue;
			new->type   = keywords[i].type;
			new->is_dec = keywords[i].is_dec;
			if (keywords[i].is_dec || keywords[i].clear_name) setName(new, NULL);
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
			CHECK(buf[0] == '\\' && input[s + 1] != '\\', "unknown escape character: \\%c",
			      input[s + 1]);
		}
		new->Char.value = buf[0];
		break;
	}

	default: break;
	}
	return new;
}

void tokenize(char *file_name, int default_space) {
	debug(GREEN("TOKENIZE: [%s]\n"), file_name);
	char *input = open_file(file_name);
	if (found_error) return;
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

	int        space    = default_space;
	int        line     = 1;
	bool       new_line = true;
	static int calling_use;

	resize_array(files, char *, files_size, files_count);
	files[files_count++] = file_name;

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
			CHECK(input[i + 1] && strncmp(input + i, "*/", 2), "expected '*/'");
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
			CHECK(input[i++] != '\"', "expected '\"'");
			parse_token(file_name, line, input, s + 1, i - 1, CHARS, space);
			continue;
		}

		if (c == '\'') {
			i++;
			if (input[i] == '\\')             i++;
			if (input[i] && input[i] != '\'') i++;
			CHECK(input[i++] != '\'', "expected '\''");
			parse_token(file_name, line, input, s + 1, i - 1, CHAR, space);
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

		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				CHECK(input[i++] != '\"', "expected \" after use");
				int s = i;
				while (input[i] && input[i] != '\"')
					i++;
				CHECK(input[i++] != '\"', "expected \" after use <filename>");

				char *use = strndup(input + s, i - s - 1);
				char *tmp = strjoin(use, ".ura", NULL);
				free(use);
				use = tmp;
				if (use[0] == '@') {
					TODO(1, "handle this case");

					// tmp = strjoin(ura_lib, "/", use + 1);
					// free(use);
					// use = tmp;
				} else if (use[0] != '/') {
					char *dir_copy = strdup(file_name);
					char *dir_name = dirname(dir_copy);
					tmp            = strjoin(dir_name, "/", use);
					free(dir_copy);
					free(use);
					use = tmp;
				}
				calling_use++;
				tokenize(use, space);
				calling_use--;
			} else if (i - s == 4 && strncmp(input + s, "link", 4) == 0 && isspace(input[i])) {
				TODO(1, "handle this case");
				// while (isspace(input[i]))
				// 	i++;
				// CHECK(input[i++] != '\"', "expected \" after link");
				// int s = i;
				// while (input[i] && input[i] != '\"')
				// 	i++;
				// CHECK(input[i++] != '\"', "expected \" after link <filename>");
				// char *raw = strndup(input + s, i - s - 1);
				// // strip "@/" prefix: link "@/raylib" → "raylib" → env URA_LINK_raylib
				// if (raw[0] == '@' && raw[1] == '/') {
				// 	char *stripped = strdup(raw + 2);
				// 	free(raw);
				// 	raw = stripped;
				// }
				// for (int i = 0; i < lcount; i++)
				// 	if (strcmp(links[i], raw) == 0) goto _continue;
				// resize_array(links, char *, lsize, lcount);
				// links[lcount++] = raw;
			} else parse_token(file_name, line, input, s, i, ID, space);
			// _continue:
			continue;
		}

		bool found = false;
		for (size_t j = 0; j < sizeof(specials) / sizeof(*specials); j++) {
			size_t len = strlen(specials[j].name);
			if (strncmp(specials[j].name, input + i, len) == 0) {
				parse_token(file_name, line, NULL, 0, 0, specials[j].type, space);
				i += len;
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}

		if (found) continue;
		CHECK(1, "Syntax error <%c>", c);
	}
	if (!calling_use) new_token(END, -1);
	free(input);
}

// ABSTRACT SYNTAX TREE
Node *expr_node() { return assign_node(); }
AST_NODE(assign_node, or_node, ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN)
AST_NODE(or_node, and_node, OR)
AST_NODE(and_node, bitor_node, AND)
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
		if (CHECK(to == NULL || !to->is_dec, "expected data type after as")) return syntax_error();
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

Node *access_node() // . [] ::
{
	Node  *left = prime_node();
	Token *token;
	while ((token = find(DOT, LBRA, DOUBLE_DOTS, 0))) {
		Node *node = new_node(token);
		switch (token->type) {
		case DOT: {
			node->left  = left;
			node->right = prime_node();

			CHECK(!node->right, "expected (attribute/method call) after .");
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
			CHECK(!node->right, "expected something between []");
			EXPECT_TOKEN(RBRA, "expected right bracket");
			break;
		}
		case DOUBLE_DOTS: {
			Node *call = prime_node();
			if (!call || call->token->type != FCALL) {
				CHECK(1, "expected function call after '::'");
				return syntax_error();
			}
			char *name = strjoin(left->token->name, ".", call->token->name);
			setName(call->token, name);
			free(name);
			call->token->is_static_call = true;
			node                        = call;
			break;
		}
		default: {
			CHECK(1, "Unexpected %k line %d", token, token->line);
			return syntax_error();
		}
		}
		left = node;
	}
	return left;
}

Node *prime_node() {
	Token *token = find(ID, DATA_TYPES, 0);
	if (!token) TODO(1, "handle this case");

	// Literal value: "hello", 1, 'c'
	if (includes(token->type, DATA_TYPES) && !token->name && !token->is_dec) return new_node(token);

	// variable declaration
	if (token->type == ID && is_data_type(tokens[exe_count])) {
		Token *data_type = tokens[exe_count++];
		bool   is_ref    = find(REF, 0) != NULL;

		if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) TODO(1, "handle this case");
		setName(data_type, token->name);
		token         = data_type;
		token->is_dec = true;
		token->is_ref = is_ref;
		return new_node(token);
	}
	// function call, or main function
	if (token->type == ID && find(LPAR, 0)) {
		Node *node = new_node(token);
		if (strcmp(token->name, "main") == 0) {
			EXPECT_TOKEN(RPAR, "expected ) after main declaration");
			EXPECT_TOKEN(DOTS, "expected : after main() declaration");

			enter_scope(node);
			node->token->type     = FDEC;
			node->token->ret_type = INT;
			node->left            = new_node(new_token(ARGS, node->token->space));

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
			node->token->type = OUTPUT;
			node->left        = new_node(new_token(ARGS, token->space));
			Token *end        = NULL;

			while (!found_error && !(end = find(RPAR, END, 0))) {
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			if (CHECK(!found_error && end->type != RPAR, "output: expected )")) return syntax_error();
			node->token->ret_type = VOID;
			return node;
		} else {
			node->token->type = FCALL;
			node->left        = new_node(new_token(ARGS, node->token->space));
			Token *end        = NULL;

			while (!found_error && !(end = find(RPAR, END, 0))) {
				Node *curr = expr_node();
				add_child(node->left, curr);
				find(COMA, 0);
			}
			CHECK(!found_error && end->type != RPAR, "expected ) after function call");
		}
		return node;
	}

	if (token->type == ID) return new_node(token);

	CHECK(1, "Unexpected token %k line %d", tokens[exe_count], tokens[exe_count]->line);
	return syntax_error();
}

// INTERMEDIATE REPRESENTATION
void gen_ir(Node *node) {
	if (found_error) return;
	Node *left  = node->left;
	Node *right = node->right;
	switch (node->token->type) {
	case ID: {
		Token *find = get_variable(node->token->name);
		if (find) node->token = find;
		break;
	}
	case INT:   case BOOL: case CHAR: case ARRAY_TYPE: case FLOAT: case LONG:
	case CHARS: case PTR: case VOID: case SHORT: {
		if (node->token->is_dec) new_variable(node->token);
		else node->token->used++;
		break;
	}
	case ASSIGN: {
		gen_ir(left);
		gen_ir(right);
		if (!compatible(left->token, right->token)) {
			CHECK(1, "type mismatch: cannot assign %k to %k — use '%k as %s'", right->token,
			      left->token, right->token, to_string(left->token->type));
			break;
		}

		Type lt = left->token->ret_type ? left->token->ret_type : left->token->type;
		if (lt == STRUCT_CALL) {
			TODO(1, "handle this case");
#if 0
			Type  rt           = right->token->ret_type ? right->token->ret_type : right->token->type;
			char *param_suffix = (rt == STRUCT_CALL && right->token->Struct.ptr)
			                         ? right->token->Struct.ptr->token->name
			                         : to_string(rt);
			char *op_base = strjoin(left->token->Struct.ptr->token->name, ".operator.", to_string(ASSIGN));
			char *op_name = strjoin(op_base, ".", param_suffix);
			free(op_base);
			Node *func = find_function(op_name);
			free(op_name);
			if (func) {
				node->token->Fcall.ptr = func;
				func->token->used++;
				break;
			}
#endif
		}
		node->token->ret_type = left->token->ret_type;
		break;
	}
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
	case RETURN: {
		if (node->token->type == TUPLE) {
			for (int i = 0; i < node->children_count; i++)
				gen_ir(node->children[i]);
			node->token->ret_type = TUPLE;
			break;
		}
		gen_ir(node->left);
		node->token->ret_type = node->left->token->type;
		if (node->left->token->type == STRUCT_CALL)
			node->token->Struct.ptr = node->left->token->Struct.ptr;
		node->left->token->used++;
		break;
	}
	default: {
		TODO(1, "handle this case %s", to_string(node->token->type));
		break;
	}
	}
}

// ASSEMBLY GENERATION
void gen_asm(Node *node) {
	set_debug_location(node->token);
	Node *left  = node->left;
	Node *right = node->right;

	if (CHECK(node->token->llvm.is_set, "already set")) return;
	switch (node->token->type) {
	case INT: case LONG: case SHORT: case CHARS:
	case CHAR: case BOOL: case ARRAY_TYPE: case FLOAT: {
		Token *token = node->token;
		if (!token->is_dec && !token->name) _const_value(token);
		if (!token->is_dec) return;
		if (token->is_global) {
			TypeRef type     = get_llvm_type(token);
			token->llvm.elem = LLVMAddGlobal(module, type, token->name);
			LLVMSetInitializer(token->llvm.elem, _get_default_value(token));
		} else {
			_alloca(token);
			LLVMBuildStore(builder, _get_default_value(token), token->llvm.elem);
		}
		token->is_dec = false;
		break;
	}
	case ASSIGN: {
		gen_asm(left);
		gen_asm(right);

		Value val = read_value(right->token);

		write_value(left->token, val);
		node->token->llvm.elem = val;
		break;
	}
	case FDEC: {
		resolve_func_type(node);
		enter_scope(node);
		emit_func_body(node);
		exit_scope();
		debug_scope = debug_compile_unit;
		break;
	}
	case RETURN: {
		Node *fdec = find_enclosing(FDEC);
		if(CHECK(!fdec, "can not return outside function"))
			break;
		
		gen_asm(node->left);
		ensure_loaded(node->left);
		LLVMBuildRet(builder, node->left->token->llvm.elem);
		
		break;
	}
	default: {
		TODO(1, "handle this case %k", node->token);
		break;
	}
	}
}

// PROGRAM STARTING POINT
void compile(char *path_name) {
	debug(RED("===========================================\n"));
	debug(RED("START COMPILATION\n"));
	debug(RED("===========================================\n"));

	tokenize(path_name, 0);
	for (int i = 0; !found_error && tokens[i]; i++)
		debug(GREEN("%k\n"), tokens[i]);
	if (found_error) return;

#if AST
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
	if (found_error) return;
#endif

#if IR
	debug(GREEN("===========================================\n"));
	debug(GREEN("GENERATE INTERMEDIATE REPRESENTATION\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_ir(ura_scope->children[i]);
#endif

	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER IR GENERATION\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");
	if (found_error) return;

	setup_paths(path_name);

#if ASM
	init(path_name);
	for (int i = 0; !found_error && i < ura_scope->children_count; i++)
		gen_asm(ura_scope->children[i]);
	finalize(ll_path);
#endif
}

int main(int argc, char **argv) {
	if (CHECK(argc < 2, "usage: ura <file.ura> [-O0|-O1|-O2|-O3|-Os|-Oz] [-san]"
	                    " [-o output]"))
		return 1;

	char *output  = NULL;
	bool  no_exec = false;
	for (int i = 1; i < argc && !found_error; i++) {
		char *a = argv[i];
#define MATCH(name, var, val)                                                                      \
	if (strcmp(a, name) == 0) {                                                                     \
		var = val;                                                                                   \
		continue;                                                                                    \
	}
		MATCH("-san", enable_san, true); MATCH("-O0", flags, PASSES_O0); 
		MATCH("-no-exec", no_exec, true) MATCH("-O1", flags, PASSES_O1);
		MATCH("-no-debug", enable_debug, false);
		MATCH("-O2", flags, PASSES_O2); MATCH("-O3", flags, PASSES_O3);
		MATCH("-Os", flags, PASSES_Os); MATCH("-Oz", flags, PASSES_Oz);
		MATCH("-prep", enable_prep, true);
#undef MATCH
		if (strcmp(a, "-o") == 0) {
			CHECK(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		} else if (a[0] != '-') {
			size_t n = strlen(a);
			CHECK(n <= 4 || strcmp(a + n - 4, ".ura") != 0, "invalid file: %s\n", a);
			CHECK(files_count > 0, "you can compile only one file"
			                       "try importing them in one files");
			resize_array(files, char *, files_size, files_count);
			files[files_count++] = a;
		} else {
			CHECK(true, "unknown flag: %s\n", a);
		}
	}
	if (CHECK(files_count == 0, "required .ura files as arguments to compile")) return 1;
	if (found_error)                                                            return 1;

	if (!output) output = "exe.out";

	compile(files[0]);

#if ASM
	if (ll_path && !no_exec && !found_error) {
		char *san = enable_san ? " -fsanitize=address,undefined -fno-omit-frame-pointer -g" : "";
		char  cmd[URA_MAX_SIZE];
		snprintf(cmd, sizeof cmd, "clang%s \"%s\" -o \"%s\"", san, ll_path, output);
		CHECK(system(cmd) != 0, "linking failed\n");
	}
#endif
	free_memory();
	return found_error;
}