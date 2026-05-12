#include "header.h"

const Keyword keywords[] = {
    {"int", INT, 1, 1},           {"bool", BOOL, 1, 1},
    {"chars", CHARS, 1, 1},       {"char", CHAR, 1, 1},
    {"float", FLOAT, 1, 1},       {"void", VOID, 1, 1},
    {"long", LONG, 1, 1},         {"pointer", CHARS, 1, 1},
    {"short", SHORT, 1, 1},       {"if", IF, 0, 0},
    {"elif", ELIF, 0, 0},         {"else", ELSE, 0, 0},
    {"while", WHILE, 0, 0},       {"for", FOR, 0, 0},
    {"to", TO, 0, 0},             {"step", STEP, 0, 0},
    {"in", IN, 0, 0},             {"fn", FDEC, 0, 0},
    {"return", RETURN, 0, 0},     {"break", BREAK, 0, 0},
    {"continue", CONTINUE, 0, 0}, {"ref", REF, 0, 0},
    {"struct", STRUCT_DEF, 0, 0}, {"enum", ENUM_DEF, 0, 0},
    {"proto", PROTO, 0, 0},       {"mod", MODULE, 0, 0},
    {"operator", OPERATOR, 0, 0}, {"as", AS, 0, 0},
    {"pub", PUB, 0, 0},           {"delete", DELETE, 0, 0},
    {"and", AND, 0, 1},           {"or", OR, 0, 1},
    {"is", EQUAL, 0, 1},          {"not", NOT, 0, 1},
    {"typeof", TYPEOF, 0, 1},     {"sizeof", SIZEOF, 0, 1},
    {"stack", STACK, 0, 1},       {"heap", HEAP, 0, 1},
    {"array", ARRAY_TYPE, 0, 1},  {"List", LIST_TYPE, 0, 1},
    {"null", NULLABLE, 0, 1},
};

const Keyword specials[] = {
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

// TODO: use default_space in int s, int e
Token *parse_token(Source *src, int line, int s, int e, Type type, int space) {
	Token *new       = new_token(type, space);
	char *input      = src->content;
	new->line        = line;
	new->source      = src;
	new->start_index = s;
	new->end_index   = e;

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
			if (buf[0] == '\\' && input[s + 1] != '\\')
				tokenize_error(src, line, s, e, "unknown escape character: \\%c", input[s + 1]);
		}
		new->Char.value = buf[0];
		break;
	}
	default: break;
	}
	return new;
}

// Conditional-compile frame stack. `@if linux:` pushes; `@elif`/`@else` updates
// the top frame; a non-directive line at indent <= the frame's @if-indent pops.
// `active` means tokens inside this frame are emitted; `any_matched` records
// whether any branch in the chain has already been chosen (so later branches
// don't double-fire even if their condition is true).
typedef struct CondFrame {
	int  indent;
	bool active;
	bool any_matched;
} CondFrame;

#define MAX_COND_DEPTH 32
static CondFrame cond_stack[MAX_COND_DEPTH];
static int       cond_top = -1;

static bool cond_emitting(void) {
	for (int j = 0; j <= cond_top; j++)
		if (!cond_stack[j].active) return false;
	return true;
}
static bool cond_outer_active(void) {
	for (int j = 0; j < cond_top; j++)
		if (!cond_stack[j].active) return false;
	return true;
}

void tokenize(char *file_name, int default_space) {
	debug(GREEN("TOKENIZE: [%s]\n"), file_name);
	Source *src = new_source(file_name);
	if (found_error) return;
	char      *input = src->content;

	int        space    = default_space;
	int        line     = 1;
	bool       new_line = true;
	static int calling_use;

	int        i = 0;
	int        s = i;
	for (; input[i] && !found_error;) {
		s      = i;
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
			if (strncmp(input + i, "*/", 2) != 0) {
				tokenize_error(src, line, s, s + 2, "unterminated block comment, expected '*/'");
				break;
			}
			i += 2;
			continue;
		}
		if (strncmp(input + i, "//", 2) == 0) {
			while (input[i] && input[i] != '\n')
				i++;
			continue;
		}

		// === Conditional compilation: `@if name:` / `@elif name:` / `@else:`
		// Runs once per line at the first non-whitespace char. `space` is the
		// current line's indent. Three jobs:
		//   1. Pop any frames we've dedented out of (unless the current line is
		//      `@elif`/`@else` matching the top frame's indent — that's a
		//      continuation, not an exit).
		//   2. Handle a directive line: push (@if) / update (@elif, @else) the
		//      top frame, then consume the rest of the line.
		//   3. If we're inside an inactive branch, skip the line.
		if (new_line) {
			bool starts_elif_or_else =
			    input[i] == '@'
			    && (strncmp(input + i, "@elif", 5) == 0 || strncmp(input + i, "@else", 5) == 0);
			while (cond_top >= 0 && space <= cond_stack[cond_top].indent
			       && !(starts_elif_or_else && space == cond_stack[cond_top].indent))
				cond_top--;

			if (input[i] == '@' && strncmp(input + i, "@if", 3) == 0
			    && (input[i + 3] == ' ' || input[i + 3] == '\t')) {
				i += 3;
				while (input[i] == ' ' || input[i] == '\t') i++;
				int ns = i;
				while (isalnum(input[i]) || input[i] == '_') i++;
				char *name = strndup(input + ns, i - ns);
				while (input[i] == ' ' || input[i] == '\t') i++;
				if (input[i] != ':') {
					tokenize_error(src, line, ns, i, "expected ':' after @if name");
					free(name);
					break;
				}
				i++;
				while (input[i] && input[i] != '\n') i++;
				if (cond_top + 1 >= MAX_COND_DEPTH) {
					tokenize_error(src, line, ns, i, "too many nested @if directives");
					free(name);
					break;
				}
				bool match = (ura_target_os && strcmp(name, ura_target_os) == 0);
				free(name);
				cond_top++;
				cond_stack[cond_top].indent      = space;
				cond_stack[cond_top].active      = match && cond_outer_active();
				cond_stack[cond_top].any_matched = match;
				continue;
			}
			if (input[i] == '@' && strncmp(input + i, "@elif", 5) == 0
			    && (input[i + 5] == ' ' || input[i + 5] == '\t')) {
				if (cond_top < 0 || space != cond_stack[cond_top].indent) {
					tokenize_error(src, line, i, i + 5, "@elif without matching @if at same indent");
					break;
				}
				i += 5;
				while (input[i] == ' ' || input[i] == '\t') i++;
				int ns = i;
				while (isalnum(input[i]) || input[i] == '_') i++;
				char *name = strndup(input + ns, i - ns);
				while (input[i] == ' ' || input[i] == '\t') i++;
				if (input[i] != ':') {
					tokenize_error(src, line, ns, i, "expected ':' after @elif name");
					free(name);
					break;
				}
				i++;
				while (input[i] && input[i] != '\n') i++;
				bool match = (ura_target_os && strcmp(name, ura_target_os) == 0);
				free(name);
				if (cond_stack[cond_top].any_matched) {
					cond_stack[cond_top].active = false;
				} else {
					cond_stack[cond_top].active      = match && cond_outer_active();
					cond_stack[cond_top].any_matched = match;
				}
				continue;
			}
			if (input[i] == '@' && strncmp(input + i, "@else", 5) == 0
			    && (input[i + 5] == ':' || input[i + 5] == ' ' || input[i + 5] == '\t')) {
				if (cond_top < 0 || space != cond_stack[cond_top].indent) {
					tokenize_error(src, line, i, i + 5, "@else without matching @if at same indent");
					break;
				}
				while (input[i] && input[i] != ':' && input[i] != '\n') i++;
				if (input[i] == ':') i++;
				while (input[i] && input[i] != '\n') i++;
				if (cond_stack[cond_top].any_matched) cond_stack[cond_top].active = false;
				else {
					cond_stack[cond_top].active      = cond_outer_active();
					cond_stack[cond_top].any_matched = true;
				}
				continue;
			}

			if (!cond_emitting()) {
				while (input[i] && input[i] != '\n') i++;
				continue;
			}
		}

		new_line = false;
		if (c == '\"') {
			i++;
			while (input[i] && input[i] != '\"') {
				if (input[i] == '\\' && input[i + 1]) i++;
				i++;
			}
			if (input[i] != '\"') {
				tokenize_error(src, line, s, s + 1, "unterminated string literal, expected '\"'");
				break;
			}
			i++;
			parse_token(src, line, s + 1, i - 1, CHARS, space);
			continue;
		}
		if (c == '\'') {
			i++;
			if (input[i] == '\\' && input[i + 1]) i++;
			if (input[i] && input[i] != '\'')     i++;
			if (input[i] != '\'')                 {
				tokenize_error(src, line, s, s + 1, "unterminated character literal, expected \"'\"");
				break;
			}
			i++;
			parse_token(src, line, s + 1, i - 1, CHAR, space);
			continue;
		}
		if (isdigit(c)) {
			while (isdigit(input[i]))
				i++;
			if (input[i] == '.' && isdigit(input[i + 1])) {
				i++;
				while (isdigit(input[i]))
					i++;
				parse_token(src, line, s, i, FLOAT, space);
			} else parse_token(src, line, s, i, INT, space);
			continue;
		}
		if (isalpha(c) || strchr("@$_", c)) {
			while (input[i] && (isalnum(input[i]) || strchr("@$_", input[i])))
				i++;
			if (i - s == 3 && strncmp(input + s, "use", 3) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, i, i + 1, "expected '\"' after `use`");
					break;
				}
				i++;
				int s = i;
				while (input[i] && input[i] != '\"' && input[i] != '\n')
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, s - 1, s,
					               "unterminated `use` path, expected closing '\"'");
					break;
				}
				i++;
				char *use = strndup(input + s, i - s - 1);
				char *tmp = format("%s.ura", use);
				free(use);
				use = tmp;
				if (use[0] == '@') {
					char *rest = use + 1;
					if (*rest == '/') rest++;
					tmp = format("%s/%s", ura_lib, rest);
					free(use);
					use = tmp;
				} else if (use[0] != '/') {
					char *dir_copy = strdup(file_name);
					char *dir_name = dirname(dir_copy);
					tmp            = format("%s/%s", dir_name, use);
					free(dir_copy);
					free(use);
					use = tmp;
				}
				calling_use++;
				tokenize(use, space);
				calling_use--;
			} else if (i - s == 4 && strncmp(input + s, "link", 4) == 0 && isspace(input[i])) {
				while (isspace(input[i]))
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, i, i + 1, "expected '\"' after `link`");
					break;
				}
				i++;
				int link_s = i;
				while (input[i] && input[i] != '\"' && input[i] != '\n')
					i++;
				if (input[i] != '\"') {
					tokenize_error(src, line, link_s - 1, link_s,
					               "unterminated `link` path, expected closing '\"'");
					break;
				}
				i++;
			} else parse_token(src, line, s, i, ID, space);
			continue;
		}

		bool found = false;
		for (size_t j = 0; j < sizeof(specials) / sizeof(*specials); j++) {
			size_t len = strlen(specials[j].name);
			if (strncmp(specials[j].name, input + i, len) == 0) {
				i += len;
				parse_token(src, line, s, i, specials[j].type, space);
				if (includes(specials[j].type, DOTS, 0)) space += TAB;
				found = true;
				break;
			}
		}
		if (found) continue;
		tokenize_error(src, line, i, i + 1, "unexpected character '%c'", c);
		break;
	}
	if (!calling_use) parse_token(src, line, s, i, END, -1);
}

Token *new_token(Type type, int space) {
	Token *token = allocate(1, sizeof(Token));
	token->type  = type;
	token->space = ((space + TAB / 2) / TAB) * TAB;
	add_token(token);
	return token;
}

Token *copy_token(Token *token) {
	if (token == NULL) return NULL;
	Token *new = allocate(1, sizeof(Token));
	memcpy(new, token, sizeof(Token));
	new->name = NULL;
	if (token->name)        setName(new, token->name);
	if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
	new->llvm.dims       = NULL;
	new->llvm.dims_count = 0;
	new->llvm.dims_size  = 0;
	for (int i = 0; i < token->llvm.dims_count; i++) {
		resize_array(new->llvm.dims, Value, new->llvm.dims_size, new->llvm.dims_count);
		new->llvm.dims[new->llvm.dims_count++] = token->llvm.dims[i];
	}
	add_token(new);
	return new;
}

void add_token(Token *token) {
	resize_array(tokens, Token *, tokens_size, tokens_count);
	tokens[tokens_count++] = token;
}

void setName(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

Node *add_child(Node *node, Node *child) {
	// Skip the SYNTAX_ERROR sentinel — it's a single static node returned
	// by every failed parse, so adding it as a child more than once would
	// UAF on free_memory (same pointer freed twice).
	if (child == syntax_error_node) return child;
	if (child) {
		resize_array(node->children, Node *, node->children_size, node->children_count);
		child->token->space                    = node->token->space + TAB;
		node->children[node->children_count++] = child;
	}
	return child;
}

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

Node *syntax_error_node;

Node *syntax_error() {
	// Note: do NOT set found_error here. parse_error() already records the
	// error in error_count and sets found_error; the caller has typically
	// already invoked parser_recover() to clear that flag and skip past
	// the bad tokens. Setting found_error again here would re-block
	// find()/within() and prevent further error reporting.
	if (syntax_error_node == NULL)
		syntax_error_node = new_node(new_token(SYNTAX_ERROR, -1));
	if (exe_count >= 0 && exe_count < tokens_count && tokens[exe_count] && tokens[exe_count]->source)
		syntax_error_node->token->source = tokens[exe_count]->source;
	return syntax_error_node;
}

// Advance to a sync point and clear the scoped-error flag.
// A sync point is the next token that is BOTH at-or-shallower than
// `indent` AND on a different line from where we currently are. This
// guarantees we always cross at least one newline and skip the broken
// remainder of the current statement (otherwise we'd loop forever
// when the error fires mid-line on a token at the target indent —
// e.g. `operator () Vec:` where `(` shares the indent with `operator`).
void parser_recover(int indent) {
	int start_line = (exe_count < tokens_count) ? tokens[exe_count]->line : -1;
	while (exe_count < tokens_count && tokens[exe_count]->type != END) {
		Token *t = tokens[exe_count];
		if (t->space <= indent && t->line != start_line) break;
		exe_count++;
	}
	found_error = false;
}

Token *find(Type type, ...) {
	if (found_error) return NULL;
	// Past-the-end: treat as if we hit END so callers don't deref a NULL slot
	// and the top-level `while (!find(END, 0))` loop terminates.
	if (exe_count >= tokens_count) {
		va_list ap;
		va_start(ap, type);
		bool wants_end = (type == END);
		while (!wants_end && type) {
			type      = va_arg(ap, Type);
			wants_end = (type == END);
		}
		if (wants_end && tokens_count > 0) return tokens[tokens_count - 1];
		return NULL;
	}
	va_list ap;
	va_start(ap, type);
	while (type) {
		if (type == tokens[exe_count]->type) return tokens[exe_count++];
		type = va_arg(ap, Type);
	}
	return NULL;
}

bool within(int space) {
	Token *curr = tokens[exe_count];
	return !found_error && curr->space > space && curr->type != END;
}

bool is_data_type(Token *token) { return includes(token->type, DATA_TYPES, 0); }

bool is_type_starter(Token *token) {
	return is_data_type(token) || token->type == FDEC;
}

Token *parse_type_in_signature(int anchor_space);

Token *parse_fn_type(int anchor_space) {
	Token *fn_kw = find(FDEC, 0);
	if (!fn_kw) return NULL;

	Token *ftype       = new_token(FN_TYPE, fn_kw->space);
	ftype->source      = fn_kw->source;
	ftype->line        = fn_kw->line;
	ftype->start_index = fn_kw->start_index;
	ftype->end_index   = fn_kw->end_index;

	if (!find(LPAR, 0)) {
		parse_error(fn_kw, "expected '(' after `fn` in type");
		parser_recover(anchor_space);
		return NULL;
	}

	// Parse comma-separated parameter types.
	while (tokens[exe_count]->type != RPAR && tokens[exe_count]->type != END) {
		Token *param_t = parse_type_in_signature(anchor_space);
		if (!param_t) return NULL;
		resize_array(ftype->Fn.params, Token *, ftype->Fn.params_size, ftype->Fn.params_count);
		ftype->Fn.params[ftype->Fn.params_count++] = param_t;
		if (tokens[exe_count]->type == COMA) exe_count++;
		else if (tokens[exe_count]->type != RPAR) {
			parse_error(fn_kw, "expected ',' or ')' in fn-type parameter list");
			parser_recover(anchor_space);
			return NULL;
		}
	}
	if (!find(RPAR, 0)) {
		parse_error(fn_kw, "expected ')' to close fn-type parameter list");
		parser_recover(anchor_space);
		return NULL;
	}

	ftype->Fn.ret = parse_type_in_signature(anchor_space);
	if (!ftype->Fn.ret) return NULL;
	return ftype;
}

Token *parse_type_in_signature(int anchor_space) {
	if (tokens[exe_count]->type == FDEC) return parse_fn_type(anchor_space);
	Token *t = find(DATA_TYPES, ID, 0);
	if (!t) {
		parse_error(tokens[exe_count], "expected type");
		parser_recover(anchor_space);
		return NULL;
	}
	if (includes(t->type, ARRAY_TYPE, LIST_TYPE, 0)) array_list_type_setup(t);
	return t;
}

void enter_scope(Node *node) {
	scopes_count++;
	debug(CYAN("Enter Scope: %k index %d\n"), node->token, scopes_count);
	resize_array(scopes, Node *, scopes_size, scopes_count);
	scopes[scopes_count] = node;
	scope                = scopes[scopes_count];
}

void exit_scope() {
	if (CHECK(scopes_count < 0, "No active scope to exit")) return;
	debug(CYAN("Exit Scope: %k index %d\n"), scopes[scopes_count]->token, scopes_count);
	scopes[scopes_count] = NULL;
	scopes_count--;
	scope = scopes[scopes_count];
}

// ============================================================================
// AST BUILDERS
// ============================================================================
Node *expr_node() { return assign_node(); }

// Hand-rolled equivalent of AST_NODE for assign_node, with one extra step:
// when the LHS is a primitive/struct declaration (is_dec on the operand), we
// stamp `is_dec` on the ASSIGN token itself so the -prep emitter can later
// distinguish a typed declaration (`x int = 1`) from a re-assignment (`x = 1`).
// ir_id replaces the LHS node->token with the canonical variable Token, which
// preserves is_dec on the operand — so we can't tell decl vs use from the
// operand alone after IR.
Node *assign_node() {
	Node  *left = or_node();
	Token *token;
	while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
	                     DIV_ASSIGN, MOD_ASSIGN, 0))) {
		Node *node  = new_node(token);
		node->left  = left;
		node->right = or_node();
		if (token->type == ASSIGN && left && left->token && left->token->is_dec)
			token->is_dec = true;
		left = node;
	}
	return left;
}
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

Node *as_node() {
	Node  *left = unary_node();
	Token *token;
	while ((token = find(AS, 0))) {
		Node *node  = new_node(token);
		node->left  = left;
		node->right = unary_node();
		Token *to   = node->right->token;
		// TODO: casting to struct type
		if (!to->is_dec) {
			parse_error(token, "expected data type after 'as'");
			parser_recover(token->space);
			return syntax_error();
		}
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
		node->right->token->source    = token->source;
		node->right->token->line      = token->line;
		node->right->token->Int.value = token->type == SUB ? -1 : 1;
		node->token->type             = MUL;
		return node;
	}
	return access_node();
}

Node *access_node() // . [] ::
{
	Node  *left = keyword_node();
	Token *token;
	while ((token = find(DOT, LBRA, DOUBLE_DOTS, 0))) {
		Node *node = new_node(token);
		switch (token->type) {
		case DOT: {
			node->left  = left;
			node->right = keyword_node();

			if (!node->right) {
				parse_error(token, "expected attribute or method name after '.'");
				parser_recover(token->space);
				return syntax_error();
			}
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
			if (!node->right) {
				parse_error(token, "expected expression between '[' and ']'");
				parser_recover(token->space);
				return syntax_error();
			}
			EXPECT_TOKEN(token, RBRA, "expected ']' to close index access");
			break;
		}
		case DOUBLE_DOTS: {
			Node *call = keyword_node();
			if (!call || call->token->type != FCALL) {
				parse_error(token, "expected function call after '::'");
				parser_recover(token->space);
				return syntax_error();
			}
			char *name = format("%s.%s", left->token->name, call->token->name);
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

Node *keyword_node() {
	Token *token = NULL;
	Node  *node;

	if ((token = find(PUB, 0))) {
		parse_error(token, "'pub' is only valid inside struct definitions");
		parser_recover(token->space);
		return syntax_error();
	}

	if ((token = find(MODULE, 0)))                    return module_node(token);
	if ((token = find(STRUCT_DEF, 0)))                return struct_def_node(token);
	if ((token = find(ENUM_DEF, 0)))                  return enum_def_node(token);
	if ((token = find(IF, 0)))                        return if_node(token);
	if ((token = find(WHILE, 0)))                     return while_node(token);
	if ((token = find(FOR, 0)))                       return for_node(token);
	if ((token = find(FDEC, 0)))                      return fdec_node(token);
	if ((token = find(RETURN, 0)))                    return return_node(token);
	if ((token = find(BREAK, CONTINUE, NULLABLE, 0))) return new_node(token);
	if ((token = find(STACK, HEAP, 0)))               return stack_heap_node(token);
	if ((token = find(TYPEOF, SIZEOF, 0)))            return typeof_sizeof_node(token);
	if ((token = find(LBRA, 0)))                      return array_lit_node(token);

	if ((token = find(REF, 0))) {
		// Leading-ref declaration: `ref name type [= init]` for primitives,
		// `ref name Struct = init` for structs, `ref name fn(...) ret` for fn-types.
		// Anything else after REF is the unary ref-of expression form (`p = ref x`).
		bool is_decl = false;
		if (exe_count + 1 < tokens_count
		    && tokens[exe_count]->type == ID
		    && tokens[exe_count]->line == token->line
		    && (is_data_type(tokens[exe_count + 1])
		        || tokens[exe_count + 1]->type == FDEC
		        || (tokens[exe_count + 1]->type == ID
		            && get_struct(tokens[exe_count + 1]->name)))) {
			is_decl = true;
		}
		if (is_decl) {
			Node *decl = prime_node();
			if (decl && decl != syntax_error_node && decl->token) {
				decl->token->is_ref = true;
				if (decl->token->type == STRUCT_CALL) {
					EXPECT_TOKEN(token, ASSIGN,
					    "'%s': ref must be initialized at declaration",
					    decl->token->name);
					exe_count--;
				}
			}
			return decl;
		}
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
	if ((token = find(LPAR, 0))) {
		node = (tokens[exe_count]->type != RPAR) ? expr_node() : NULL;
		EXPECT_TOKEN(token, RPAR, "expected ')' to close parenthesized expression");
		return node;
	}
	if ((token = find(PROTO, 0))) {
		if (includes(tokens[exe_count]->type, FDEC, STRUCT_DEF, 0))
			tokens[exe_count]->is_proto = true;
		else {
			parse_error(token, "expected `fn` or `struct` after `proto`");
			parser_recover(token->space);
			return syntax_error();
		}
		return expr_node();
	}
	return prime_node();
}

Node *prime_node() {
	Token *token = find(ID, DATA_TYPES, 0);
	if (!token) {
		Token *cur = tokens[exe_count];
		parse_error(cur, "unexpected token in expression");
		parser_recover(cur->space);
		return syntax_error();
	}

	if (includes(token->type, DATA_TYPES) && !token->name && !token->is_dec) return new_node(token);
	if (token->is_dec)                                                       return new_node(token);

	if (token->type == ID && is_type_starter(tokens[exe_count]) &&
	    tokens[exe_count]->line == token->line) {
		Token *data_type;
		if (tokens[exe_count]->type == FDEC) {
			data_type = parse_fn_type(token->space);
			if (!data_type) return syntax_error();
		} else {
			data_type = tokens[exe_count++];
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0))
				array_list_type_setup(data_type);
		}
		if (tokens[exe_count]->type == REF) {
			parse_error(tokens[exe_count],
			    "trailing `ref` is no longer supported; write `ref %s %s` instead",
			    token->name,
			    data_type->type == STRUCT_CALL && data_type->Struct.ptr
			        ? data_type->Struct.ptr->token->name
			        : to_string(data_type->type));
			parser_recover(token->space);
			return syntax_error();
		}
		setName(data_type, token->name);
		data_type->is_dec = true;
		if (tokens[exe_count]->type == COMA && exe_count + 2 < tokens_count &&
		    tokens[exe_count + 1]->type == ID && is_data_type(tokens[exe_count + 2]))
			return tuple_unpack_node(data_type);
		return new_node(data_type);
	}

	Node *st_dec = NULL;
	if (token->type == ID && tokens[exe_count]->type == ID &&
	    (st_dec = get_struct(tokens[exe_count]->name))) {
		find(ID, 0);
		if (tokens[exe_count]->type == REF) {
			parse_error(tokens[exe_count],
			    "trailing `ref` is no longer supported; write `ref %s %s` instead",
			    token->name, st_dec->token->name);
			parser_recover(token->space);
			return syntax_error();
		}
		token->type       = STRUCT_CALL;
		token->is_dec     = true;
		token->Struct.ptr = st_dec;
		return new_node(token);
	}

	if (token->type == ID && find(LPAR, 0)) {
		if (strcmp(token->name, "main") == 0)        return main_node(token);
		else if (strcmp(token->name, "output") == 0) return output_node(token);
		return fcall_node(token);
	}

	if (token->type == ID) return new_node(token);
	// CHECK(1, "Unexpected token %k line %d", tokens[exe_count], tokens[exe_count]->line);
	return syntax_error();
}

void fdec_tuple_elem(Node *node) {
	Token *token2 = NULL;
	if (includes(tokens[exe_count]->type, ARRAY_TYPE, LIST_TYPE, 0)) {
		token2         = tokens[exe_count++];
		Type container = token2->type;
		EXPECT_TOKEN_VOID(token2, LBRA, "expected '[' after %s in tuple type",
		                  container == LIST_TYPE ? "list" : "array");
		Token *et = find(DATA_TYPES, ID, 0);
		if (!et) {
			parse_error(token2, "expected element type in tuple %s",
			            container == LIST_TYPE ? "list" : "array");
			syntax_error();
			return;
		}
		EXPECT_TOKEN_VOID(token2, RBRA, "expected ']' to close tuple %s type",
		                  container == LIST_TYPE ? "list" : "array");
		if (container == LIST_TYPE) {
			const char *ename = et->type == ID ? et->name : type_to_ura_name(et->type);
			char        sname[LIST_NAME_MAX];
			snprintf(sname, sizeof(sname), LIST_STRUCT_PREFIX "%s", ename);
			Node *list_st = get_struct(sname);
			if (!list_st) {
				parse_error(token2, "list struct '%s' not found", sname);
				syntax_error();
				return;
			}
			token2->type       = STRUCT_CALL;
			token2->Struct.ptr = list_st;
		} else {
			token2->Array.sub_type = et->type;
			token2->ret_type       = container;
			if (et->type == ID) {
				Node *sd = get_struct(et->name);
				if (sd) {
					token2->Array.sub_type   = STRUCT_CALL;
					token2->Array.struct_ptr = sd;
				}
			}
		}
	} else if (tokens[exe_count]->type == ID) {
		token2   = tokens[exe_count++];
		Node *st = get_struct(token2->name);
		if (!st) {
			parse_error(token2, "unknown type '%s' in tuple", token2->name);
			syntax_error();
			return;
		}
		token2->type       = STRUCT_CALL;
		token2->Struct.ptr = st;
	} else if (is_data_type(tokens[exe_count])) {
		token2 = tokens[exe_count++];
	} else {
		parse_error(tokens[exe_count], "unexpected token in tuple return type");
		syntax_error();
		return;
	}
	resize_array(node->token->Tuple.types, Token *, node->token->Tuple.types_size,
	             node->token->Tuple.types_count);
	node->token->Tuple.types[node->token->Tuple.types_count++] = token2;
}

void fdec_tuple_ret(Node *node) {
	exe_count++;
	node->token->ret_type = TUPLE;
	while (!found_error && tokens[exe_count]->type != RPAR) {
		fdec_tuple_elem(node);
		if (found_error)                     return;
		if (tokens[exe_count]->type == COMA) exe_count++;
	}
	EXPECT_TOKEN_VOID(node->token, RPAR, "expected ')' to close tuple return type list");
}

void fdec_single_ret(Node *node) {
	bool is_ref_ret = false;
	if (tokens[exe_count]->type == REF) {
		is_ref_ret = true;
		exe_count++;
	}
	if (!is_data_type(tokens[exe_count]) && tokens[exe_count]->type != ID) {
		parse_error(node->token, "expected return type after function declaration");
		syntax_error();
		return;
	}
	Token *ret_tok = tokens[exe_count++];
	if (ret_tok->type == ID) {
		Node *st = get_struct(ret_tok->name);
		if (!st) {
			parse_error(ret_tok, "unknown return type '%s'", ret_tok->name);
			syntax_error();
			return;
		}
		node->token->ret_type   = STRUCT_CALL;
		node->token->Struct.ptr = st;
	} else node->token->ret_type = ret_tok->type;
	node->token->is_ref = is_ref_ret;
}

void fdec_body(Node *node) {
	Token *next = find(DOTS, 0);
	if (!found_error && !next) {
		parse_error(node->token, "expected ':' after function declaration");
		syntax_error();
		return;
	}
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
		if (node->token->ret_type != VOID) {
			if (!child || child->token->type != RETURN) {
				parse_error(node->token, "expected return statement in function '%s'",
				            node->token->name);
				syntax_error();
				return;
			}
		} else {
			Node *ret = new_node(new_token(RETURN, node->token->space + TAB));
			ret->left = new_node(new_token(VOID, node->token->space + TAB));
			add_child(node, ret);
		}
	}
}

Node *fdec_node(Token *token) {
	Node *node = new_node(token);
	if (!node->token->name || strcmp(node->token->name, "fn") == 0) {
		Token *fname = find(ID, 0);
		if (!fname) {
			parse_error(token, "expected identifier after `fn` keyword");
			parser_recover(token->space);
			return syntax_error();
		}
		setName(node->token, fname->name);
	}

	Node *struct_owner = NULL;
	Node *parent       = scopes[scopes_count];
	if (parent && parent->token->type == STRUCT_DEF) struct_owner = parent;

	enter_scope(node);
	EXPECT_TOKEN(token, LPAR, "expected '(' after function declaration");
	params_node(node);
	if (found_error) return syntax_error();

	if (struct_owner && !node->token->is_pub) {
		Token *self_tok = new_token(STRUCT_CALL, node->token->space);
		setName(self_tok, "self");
		self_tok->Struct.ptr = struct_owner;
		self_tok->is_dec     = true;
		self_tok->is_ref     = true;
		add_child(node->left, new_node(self_tok));
	}

	if (tokens[exe_count]->type == LPAR) fdec_tuple_ret(node);
	else fdec_single_ret(node);
	if (found_error) return syntax_error();

	if (tokens[exe_count]->type == REF) {
		parse_error(tokens[exe_count],
		    "trailing `ref` on return type is no longer supported; write `ref <type>` before the type");
		syntax_error();
		return node;
	}
	if (!node->token->is_proto) fdec_body(node);
	if (found_error)            return syntax_error();
	exit_scope();
	return node;
}

Node *return_node(Token *token) {
	Node *node = new_node(token);
	for (int i = scopes_count; i >= 0; i--) {
		Node *curr = scopes[i];
		if (curr->token->type == FDEC) {
			if (curr->token->ret_type == VOID) {
				node->left              = copy_node(node);
				node->left->token->type = VOID;
			} else if (curr->token->ret_type == TUPLE) {
				add_child(node, expr_node());
				while (tokens[exe_count]->type == COMA) {
					exe_count++;
					add_child(node, expr_node());
				}
			} else node->left = expr_node();
			break;
		}
	}
	return node;
}

Node *stack_heap_node(Token *token) {
	Node *node = new_node(token);
	EXPECT_TOKEN(token, LBRA, "expected '[' after %s", to_string(token->type));
	int depth = 1;
	while (find(LBRA, 0))
		depth++;
	Token *elem_type = find(DATA_TYPES, ID, 0);
	if (!elem_type) {
		parse_error(token, "expected element type after %s[", to_string(token->type));
		parser_recover(token->space);
		return syntax_error();
	}
	for (int i = 0; i < depth; i++)
		EXPECT_TOKEN(token, RBRA, "expected ']' in %s type", to_string(token->type));
	EXPECT_TOKEN(token, LPAR, "expected '(' after %s[type]", to_string(token->type));
	node->token->ret_type       = ARRAY;
	node->token->Array.sub_type = elem_type->type;
	node->token->Array.depth    = depth;
	if (elem_type->type == ID) {
		Node *sd = get_struct(elem_type->name);
		if (sd) {
			node->token->Array.sub_type   = STRUCT_CALL;
			node->token->Array.struct_ptr = sd;
		}
		// TODO: check if struct doesn not exists
	}
	for (int i = 0; i < depth; i++) {
		add_child(node, expr_node());
		if (i < depth - 1) EXPECT_TOKEN(token, COMA, "expected ',' between dimensions");
	}
	EXPECT_TOKEN(token, RPAR, "expected ')' after %s size", to_string(token->type));
	return node;
}

Node *typeof_sizeof_node(Token *token) {
	Type  ret  = token->type == TYPEOF ? CHARS : INT;
	Node *node = new_node(token);
	EXPECT_TOKEN(token, LPAR, "expected '(' after %s", to_string(token->type));
	node->left = prime_node();
	EXPECT_TOKEN(token, RPAR, "expected ')' to close %s", to_string(token->type));
	node->token->ret_type = ret;
	return node;
}

Node *if_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);

	node->left = expr_node();
	EXPECT_TOKEN(token, DOTS, "expected ':' after `if` condition");

	while (within(node->token->space))
		add_child(node, expr_node());

	Node *curr = node;
	while (includes(tokens[exe_count]->type, ELSE, ELIF, 0) && within(node->token->space - TAB)) {
		Token *tok = find(ELSE, ELIF, 0);

		curr->right = new_node(tok);
		curr        = curr->right;

		if (tok->type == ELIF) {
			enter_scope(curr);
			curr->left = expr_node();
			EXPECT_TOKEN(tok, DOTS, "expected ':' after `elif` condition");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
		} else if (tok->type == ELSE) {
			enter_scope(curr);
			EXPECT_TOKEN(tok, DOTS, "expected ':' after `else`");
			while (within(tok->space))
				add_child(curr, expr_node());
			exit_scope();
			break;
		}
	}
	exit_scope();
	return node;
}

Node *struct_def_node(Token *token) {
	Node  *node = new_node(token);
	Token *st_name;
	if (!(st_name = find(ID, 0))) {
		parse_error(token, "expected identifier after `struct`");
		parser_recover(token->space);
		return syntax_error();
	}
	EXPECT_TOKEN(token, DOTS, "expected ':' after struct name");

	setName(node->token, st_name->name);
	node->token->type       = STRUCT_DEF;
	node->token->Struct.ptr = node;
	new_struct(node);
	enter_scope(node);

	while (within(node->token->space)) {
		Token *pub_tok = find(PUB, 0);
		if (pub_tok) {
			pub_node(node, pub_tok);
			if (found_error) {
				exit_scope();
				return syntax_error();
			}
			continue;
		}
		Token *op_kw;
		if ((op_kw = find(OPERATOR, 0))) {
			operator_node(node, op_kw);
			if (found_error) {
				exit_scope();
				return syntax_error();
			}
			continue;
		}
		Node *child = keyword_node();
		if (found_error) {
			exit_scope();
			return syntax_error();
		}
		if (child->token->type == FDEC) {
			resize_array(child->functions, Node *, child->functions_size, child->functions_count);
			char *qualified = format("%s.%s", node->token->name, child->token->name);
			setName(child->token, qualified);
			free(qualified);
			new_function(child);
		} else {
			if (!child->token->is_dec) {
				parse_error(child->token, "invalid struct attribute");
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

Node *enum_def_node(Token *token) {
	Token *ename;
	if (!(ename = find(ID, 0))) {
		parse_error(token, "expected identifier after `enum`");
		parser_recover(token->space);
		return syntax_error();
	}
	if (!find(DOTS, 0)) {
		parse_error(ename, "expected ':' after enum name");
		parser_recover(token->space);
		return syntax_error();
	}

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

	Token *next = tokens[exe_count];
	if (next->type == ID && within(next->space)) {
		parse_error(next, "enum '%s': expected ',' before '%s'", token->name, next->name);
		syntax_error();
	}

	return node;
}

void array_list_type_setup(Token *data_type) {
	Type container = data_type->type;
	EXPECT_TOKEN_VOID(data_type, LBRA, "expected '[' after %s",
	                  container == LIST_TYPE ? "list" : "array");
	int depth = 1;
	while (find(LBRA, 0))
		depth++;
	Token *elem_type = find(DATA_TYPES, ID, 0);
	if (!elem_type) {
		parse_error(data_type, "expected element type in %s type",
		            container == LIST_TYPE ? "list" : "array");
		syntax_error();
		return;
	}
	for (int i = 0; i < depth; i++)
		EXPECT_TOKEN_VOID(data_type, RBRA, "expected ']' to close %s type",
		                  container == LIST_TYPE ? "list" : "array");
	if (container == LIST_TYPE) {
		const char *ename =
		    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
		char sname[LIST_NAME_MAX];
		snprintf(sname, sizeof(sname), LIST_STRUCT_PREFIX "%s", ename);
		for (int d = 1; d < depth; d++) {
			char tn[LIST_NAME_MAX];
			snprintf(tn, sizeof(tn), LIST_STRUCT_PREFIX "%s", sname);
			snprintf(sname, sizeof(sname), "%s", tn);
		}
		Node *list_st = get_struct(sname);
		if (!list_st) {
			parse_error(elem_type, "unknown type '%s' in list element type", ename);
			syntax_error();
			return;
		}
		data_type->type       = STRUCT_CALL;
		data_type->Struct.ptr = list_st;
	} else {
		data_type->Array.sub_type = elem_type->type;
		data_type->Array.depth    = depth;
		data_type->ret_type       = container;
		if (elem_type->type == ID) {
			Node *sd = get_struct(elem_type->name);
			if (!sd) {
				parse_error(elem_type, "unknown type '%s' in array element type", elem_type->name);
				syntax_error();
				return;
			}
			data_type->Array.sub_type   = STRUCT_CALL;
			data_type->Array.struct_ptr = sd;
		}
	}
}

#define MAX_LIST_SYNTHS 64

char       *synth_list_paths[MAX_LIST_SYNTHS];
int         synth_list_count;

static void list_struct_names(const char *base, int depth, char *elem, char *sname) {
	snprintf(elem, LIST_NAME_MAX, "%s", base);
	snprintf(sname, LIST_NAME_MAX, LIST_STRUCT_PREFIX "%s", base);
	for (int d = 1; d < depth; d++) {
		snprintf(elem, LIST_NAME_MAX, "%s", sname);
		char tmp[LIST_NAME_MAX];
		snprintf(tmp, sizeof tmp, LIST_STRUCT_PREFIX "%s", sname);
		snprintf(sname, LIST_NAME_MAX, "%s", tmp);
	}
}

void synth_list_structs() {
	struct {
		const char *base;
		int         max_depth;
	} specs[MAX_LIST_SYNTHS];
	int n = 0;
	for (int i = 0; i < tokens_count; i++) {
		if (tokens[i]->type != LIST_TYPE) continue;
		int j = i + 1, depth = 0;
		while (j < tokens_count && tokens[j]->type == LBRA) {
			depth++;
			j++;
		}
		if (j >= tokens_count) continue;
		const char *name = tokens[j]->name ? tokens[j]->name : type_to_ura_name(tokens[j]->type);
		if (!name) continue;
		int idx = -1;
		for (int k = 0; k < n; k++)
			if (strcmp(specs[k].base, name) == 0) {
				idx = k;
				break;
			}
		if (idx == -1) {
			if (n < MAX_LIST_SYNTHS) {
				specs[n].base      = name;
				specs[n].max_depth = depth;
				n++;
			}
		} else if (depth > specs[idx].max_depth) specs[idx].max_depth = depth;
	}

	synth_list_count = 0;
	for (int b = 0; b < n; b++) {
		for (int d = 1; d <= specs[b].max_depth; d++) {
			char elem[LIST_NAME_MAX], sname[LIST_NAME_MAX];
			list_struct_names(specs[b].base, d, elem, sname);
			char *src  = generate_list_source(elem, sname);
			char *path = format("/tmp/__ura_%d_%s.ura", (int)getpid(), sname);
			File  f    = fopen(path, "w");
			if (!f) {
				free(path);
				free(src);
				continue;
			}
			if (synth_list_count == 0) fputs("use \"@/memory\"\n\n", f);
			fputs(src, f);
			fclose(f);
			free(src);
			synth_list_paths[synth_list_count++] = path;
		}
	}
}

void params_node(Node *node) {
	node->left = new_node(new_token(ARGS, node->token->space));

	Token *last;
	while (!found_error && !(last = find(RPAR, END, 0))) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			last                     = find(RPAR, 0);
			if (!last) {
				parse_error(node->token, "expected ')' after `...` in variadic function");
				syntax_error();
				return;
			}
			break;
		}

		bool is_ref = false;
		if (tokens[exe_count]->type == REF) {
			is_ref = true;
			exe_count++;
		}

		Token *name = find(ID, 0);
		if (!name) {
			parse_error(node->token, "expected parameter name in function '%s'", node->token->name);
			syntax_error();
			return;
		}

		Token *data_type;
		if (tokens[exe_count]->type == FDEC) {
			data_type = parse_fn_type(name->space);
			if (!data_type) break;
		} else {
			data_type = find(DATA_TYPES, ID, 0);
			if (!data_type) {
				parse_error(name, "expected type for parameter '%s'", name->name);
				syntax_error();
				break;
			}
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				array_list_type_setup(data_type);
				if (found_error) break;
			}
		}
		if (tokens[exe_count]->type == REF) {
			parse_error(tokens[exe_count],
			    "trailing `ref` is no longer supported; write `ref %s <type>` before the name",
			    name->name);
			syntax_error();
			break;
		}
		if (data_type->type == ID) {
			Node *to_find = get_struct(data_type->name);
			if (to_find) data_type->type = STRUCT_CALL;
			else {
				parse_error(data_type, "unknown type '%s'", data_type->name);
				syntax_error();
				break;
			}
		}

		Node *curr;
		if (data_type->type == STRUCT_CALL) {
			Node *st = get_struct(data_type->name);
			if (!st) {
				parse_error(data_type, "unknown struct type '%s'", data_type->name);
				syntax_error();
				break;
			}
			data_type->Struct.ptr = st;
			curr                  = new_node(data_type);
			data_type->is_ref     = is_ref;
			setName(data_type, name->name);
		} else {
			curr         = new_node(name);
			name->is_ref = is_ref;
			name->type   = data_type->type;
			if (includes(data_type->type, ARRAY_TYPE, LIST_TYPE, 0)) {
				name->ret_type         = data_type->type;
				name->Array.sub_type   = data_type->Array.sub_type;
				name->Array.depth      = data_type->Array.depth;
				name->Array.struct_ptr = data_type->Array.struct_ptr;
			} else if (data_type->type == FN_TYPE) {
				name->ret_type     = FN_TYPE;
				name->Fn.params    = data_type->Fn.params;
				name->Fn.params_count = data_type->Fn.params_count;
				name->Fn.params_size  = data_type->Fn.params_size;
				name->Fn.ret       = data_type->Fn.ret;
			}
		}
		curr->token->is_dec   = true;
		curr->token->is_param = true;
		add_child(node->left, curr);

		if (tokens[exe_count]->type != RPAR)
			EXPECT_TOKEN_VOID(name, COMA, "expected ',' between function parameters");
	}
	if (!found_error && last && last->type != RPAR) {
		parse_error(node->token, "expected ')' to close function parameter list");
		syntax_error();
	}
}

Node *while_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);
	node->left = expr_node();
	EXPECT_TOKEN(token, DOTS, "expected ':' after `while` condition");
	while (within(node->token->space))
		add_child(node, expr_node());
	exit_scope();
	return node;
}

Node *for_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);

	Token *iter = find(ID, 0);
	if (!iter) {
		parse_error(token, "expected loop variable name after `for`");
		parser_recover(token->space);
		return syntax_error();
	}
	iter->type   = INT;
	iter->is_dec = true;
	node->left   = new_node(iter);

	Token *from_kw = find(ID, 0);
	if (!from_kw || !from_kw->name || strcmp(from_kw->name, "from") != 0) {
		parse_error(token, "expected `from` after `for %s`", iter->name);
		parser_recover(token->space);
		return syntax_error();
	}

	Node *from_expr = expr_node();
	EXPECT_TOKEN(token, TO, "expected `to` after `for %s from <expr>`", iter->name);
	Node *to_expr   = expr_node();
	Node *step_expr = NULL;
	if (find(STEP, 0)) step_expr = expr_node();
	if (!step_expr) {
		Token *one     = new_token(INT, token->space);
		one->Int.value = 1;
		step_expr      = new_node(one);
	}
	EXPECT_TOKEN(token, DOTS, "expected ':' after `for ... to <expr>`");

	add_child(node, from_expr);
	add_child(node, to_expr);
	add_child(node, step_expr);

	while (within(node->token->space))
		add_child(node, expr_node());
	exit_scope();
	return node;
}

Node *module_node(Token *token) {
	Token *id = find(ID, 0);
	if (!id) {
		parse_error(token, "expected module identifier after `mod`");
		parser_recover(token->space);
		return syntax_error();
	}
	EXPECT_TOKEN(token, DOTS, "expected ':' after `mod` name");
	setName(token, id->name);
	Node *node = new_node(token);
	enter_scope(node);
	while (within(token->space)) {
		Node  *curr   = expr_node();
		Token *ctoken = curr->token;
		// `assign_node` stamps is_dec onto the ASSIGN itself when the LHS is
		// a typed declaration — so we must look through ASSIGN to find the
		// real anchor (mirrors the top-level guard in main.c).
		Token *anchor =
		    (ctoken->type == ASSIGN && curr->left) ? curr->left->token : ctoken;
		if (!includes(anchor->type, STRUCT_DEF, FDEC, 0)) {
			parse_error(ctoken, "only function/struct declarations are allowed inside `mod`");
			syntax_error();
			break;
		}
		if (ctoken->type == FDEC) add_function(node, curr);
		else add_child(node, curr);
	}
	exit_scope();
	return node;
}

Node *array_lit_node(Token *token) {
	token->type = ARRAY_LIT;
	Node *node  = new_node(token);
	if (tokens[exe_count]->type != RBRA) {
		add_child(node, expr_node());
		while (find(COMA, 0))
			add_child(node, expr_node());
	}
	EXPECT_TOKEN(token, RBRA, "expected ']' to close array literal");
	return node;
}

Node *tuple_unpack_node(Token *first_decl) {
	Token *tu_tok = new_token(TUPLE_UNPACK, first_decl->space);
	Node  *tu     = new_node(tu_tok);
	add_child(tu, new_node(first_decl));
	while (tokens[exe_count]->type == COMA) {
		exe_count++;
		Token *nname = find(ID, 0);
		Token *ntype = find(DATA_TYPES, 0);
		if (includes(ntype->type, ARRAY_TYPE, LIST_TYPE, 0)) {
			Type container = ntype->type;
			EXPECT_TOKEN(ntype, LBRA, "expected '[' after %s",
			             container == LIST_TYPE ? "list" : "array");
			int depth = 1;
			while (find(LBRA, 0))
				depth++;
			Token *elem_type = find(DATA_TYPES, ID, 0);
			if (!elem_type) {
				parse_error(ntype, "expected element type in %s type",
				            container == LIST_TYPE ? "list" : "array");
				return syntax_error();
			}
			for (int i = 0; i < depth; i++)
				EXPECT_TOKEN(ntype, RBRA, "expected ']' to close %s type",
				             container == LIST_TYPE ? "list" : "array");
			if (container == LIST_TYPE) {
				const char *ename =
				    elem_type->type == ID ? elem_type->name : type_to_ura_name(elem_type->type);
				char sname[LIST_NAME_MAX];
				snprintf(sname, sizeof(sname), LIST_STRUCT_PREFIX "%s", ename);
				for (int d = 1; d < depth; d++) {
					char tn[LIST_NAME_MAX];
					snprintf(tn, sizeof(tn), LIST_STRUCT_PREFIX "%s", sname);
					snprintf(sname, sizeof(sname), "%s", tn);
				}
				Node *list_st = get_struct(sname);
				if (!list_st) {
					parse_error(ntype, "list struct '%s' not found", sname);
					parser_recover(ntype->space);
					return syntax_error();
				}
				ntype->type       = STRUCT_CALL;
				ntype->Struct.ptr = list_st;
			} else {
				ntype->Array.sub_type = elem_type->type;
				ntype->Array.depth    = depth;
				ntype->ret_type       = container;
				if (elem_type->type == ID) {
					Node *sd = get_struct(elem_type->name);
					if (sd) {
						ntype->Array.sub_type   = STRUCT_CALL;
						ntype->Array.struct_ptr = sd;
					}
				}
			}
		}
		setName(ntype, nname->name);
		ntype->is_dec = true;
		add_child(tu, new_node(ntype));
	}
	EXPECT_TOKEN(first_decl, ASSIGN, "expected '=' in tuple unpack");
	tu->left = expr_node();
	return tu;
}

Node *pub_node(Node *struct_node, Token *pub_tok) {
	Token *fdec_tok = find(FDEC, 0);
	if (!fdec_tok) {
		parse_error(pub_tok, "struct '%s': expected `fn` after `pub`", struct_node->token->name);
		parser_recover(pub_tok->space);
		return syntax_error();
	}
	fdec_tok->is_pub = true;
	Node *child      = fdec_node(fdec_tok);
	if (!child || child->token->type != FDEC) {
		parse_error(pub_tok, "struct '%s': expected `fn` after `pub`", struct_node->token->name);
		parser_recover(pub_tok->space);
		return syntax_error();
	}
	child->token->is_pub = true;
	char *qualified      = format("%s.%s", struct_node->token->name, child->token->name);
	setName(child->token, qualified);
	free(qualified);
	new_function(child);
	return child;
}

Node *delete_node(Node *struct_node, Token *op_kw) {
	Token *fdec_tok = new_token(FDEC, op_kw->space);
	setName(fdec_tok, "delete");
	Node *child = fdec_node(fdec_tok);
	if (child->token->ret_type != VOID) {
		parse_error(op_kw, "struct '%s': operator delete must return void", struct_node->token->name);
		parser_recover(op_kw->space);
		return syntax_error();
	}
	if (child->left->children_count != 1) {
		parse_error(op_kw, "struct '%s': operator delete takes no parameters",
		            struct_node->token->name);
		parser_recover(op_kw->space);
		return syntax_error();
	}
	char *qualified = format("%s.delete", struct_node->token->name);
	setName(child->token, qualified);
	free(qualified);
	struct_node->token->has_clean = true;
	new_function(child);
	return child;
}

Node *operator_node(Node *struct_node, Token *op_kw) {
	Token *op_tok = find(ASSIGN, ADD, SUB, MUL, DIV, MOD, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN,
	                     DIV_ASSIGN, MOD_ASSIGN, EQUAL, NOT_EQUAL, LESS, GREAT, LESS_EQUAL,
	                     GREAT_EQUAL, AND, OR, BAND, BOR, BXOR, LSHIFT, RSHIFT, DELETE, 0);
	if (!op_tok && tokens[exe_count]->type == ID && strcmp(tokens[exe_count]->name, "output") == 0) {
		op_tok       = tokens[exe_count++];
		op_tok->type = OUTPUT;
	}
	if (!op_tok) {
		parse_error(op_kw, "struct '%s': expected operator symbol after `operator`",
		            struct_node->token->name);
		parser_recover(op_kw->space);
		return syntax_error();
	}

	if (op_tok->type == DELETE) return delete_node(struct_node, op_kw);

	if (op_tok->type == OUTPUT) {
		Token *fdec_tok = new_token(FDEC, op_kw->space);
		setName(fdec_tok, "output");
		Node *child     = fdec_node(fdec_tok);
		char *qualified = format("%s" OP_PREFIX "output", struct_node->token->name);
		setName(child->token, qualified);
		free(qualified);
		new_function(child);
		return child;
	}

	char  *op_base  = format("operator.%s", to_string(op_tok->type));
	Token *fdec_tok = new_token(FDEC, op_kw->space);
	setName(fdec_tok, op_base);
	free(op_base);
	Node *child = fdec_node(fdec_tok);
	char *op_named;
	if (child->left && child->left->children_count >= 2) {
		Token *p      = child->left->children[0]->token;
		char  *suffix = (p->type == STRUCT_CALL && p->Struct.ptr) ? p->Struct.ptr->token->name
		                                                          : to_string(p->type);
		op_named      = format("%s.%s", child->token->name, suffix);
	} else {
		op_named = format("%s", child->token->name);
	}
	char *qualified = format("%s.%s", struct_node->token->name, op_named);
	free(op_named);
	setName(child->token, qualified);
	free(qualified);
	new_function(child);
	return child;
}

Node *main_node(Token *token) {
	Node *node = new_node(token);
	enter_scope(node);
	node->token->type     = FDEC;
	node->token->ret_type = INT;

	params_node(node);
	if (found_error) return syntax_error();

	EXPECT_TOKEN(token, DOTS, "expected ':' after `main()` declaration");

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
}

Node *output_node(Token *token) {
	Node *node        = new_node(token);
	node->token->type = OUTPUT;
	node->left        = new_node(new_token(ARGS, token->space));
	Token *end        = NULL;
	while (!found_error && !(end = find(RPAR, END, 0))) {
		add_child(node->left, expr_node());
		find(COMA, 0);
	}
	if (!found_error && end->type != RPAR) {
		parse_error(token, "expected ')' to close `output(...)` call");
		parser_recover(token->space);
		return syntax_error();
	}
	node->token->ret_type = VOID;
	return node;
}

Node *fcall_node(Token *token) {
	Node *node        = new_node(token);
	node->token->type = FCALL;
	node->left        = new_node(new_token(ARGS, node->token->space));
	Token *end        = NULL;
	while (!found_error && !(end = find(RPAR, END, 0))) {
		add_child(node->left, expr_node());
		find(COMA, 0);
	}
	if (!found_error && end->type != RPAR) {
		parse_error(token, "expected ')' to close function call");
		syntax_error();
	}
	return node;
}
