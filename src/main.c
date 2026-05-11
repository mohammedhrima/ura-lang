#include "header.h"

void compile(char *path_name) {
	debug(RED("===========================================\n"));
	debug(RED("START COMPILATION\n"));
	debug(RED("===========================================\n"));

	tokenize(path_name, 0);
	if (error_count > 0) return;

	synth_list_structs();
	if (synth_list_count > 0) {
		Token **user_tokens = tokens;
		int     user_count  = tokens_count;
		tokens              = NULL;
		tokens_count        = 0;
		tokens_size         = 0;
		for (int i = 0; i < synth_list_count; i++)
			tokenize(synth_list_paths[i], 0);
		if (tokens_count > 0 && tokens[tokens_count - 1]->type == END) tokens_count--;
		for (int i = 0; i < user_count; i++) {
			resize_array(tokens, Token *, tokens_size, tokens_count);
			tokens[tokens_count++] = user_tokens[i];
		}
		free(user_tokens);
	}

	for (int i = 0; tokens[i]; i++)
		debug(GREEN("%k\n"), tokens[i]);

#if AST
	ura_scope = new_node(new_token(ID, -TAB));
	setName(ura_scope->token, "ura-scope");
	enter_scope(ura_scope);
	while (!find(END, 0)) {
		Node *child = expr_node();
		// after expr_node, EXPECT_TOKEN/parse_error sites have already
		// called parser_recover(), so found_error is back to false here.
		// We just check whether the node we got is the SYNTAX_ERROR
		// sentinel — if so, skip it and keep parsing the next statement.
		if (!child || child == syntax_error_node) continue;
		Token *t      = child->token;
		Token *anchor = (t->type == ASSIGN && child->left) ? child->left->token : t;
		bool   ok =
		    includes(anchor->type, FDEC, STRUCT_DEF, ENUM_DEF, MODULE, PROTO, 0) || anchor->is_dec;
		if (!ok) {
			parse_error(t, "only declarations are allowed at top level");
			parser_recover(t->space);
			continue;
		}
		add_child(ura_scope, child);
	}

	if (error_count > 0) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");
#endif

#if IR
	debug(GREEN("===========================================\n"));
	debug(GREEN("GENERATE INTERMEDIATE REPRESENTATION\n"));
	debug(GREEN("===========================================\n"));
	// Pre-register every top-level FDEC so forward references work across
	// flat top-level functions (already worked for struct methods via
	// ir_struct_def's pre-pass; same idea, top-level scope).
	for (int i = 0; i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		Node *fn    = NULL;
		if (child->token->type == FDEC)                                  fn = child;
		else if (child->token->type == PROTO && child->left
		         && child->left->token->type == FDEC)                    fn = child->left;
		if (fn && fn->token->name && !find_function(fn->token->name))
			add_function(scope, fn);
	}
	for (int i = 0; i < ura_scope->children_count; i++)
		ir_gen(ura_scope->children[i]);
#endif

	if (error_count > 0) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER IR GENERATION\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < ura_scope->children_count; i++)
		pnode(ura_scope->children[i], "");

	setup_paths(path_name);

	if (enable_prep) {
		char *prep_path = format("%s/%s", build_dir, base);
		char *full_path = format("%s.prep.ura", prep_path);
		free(prep_path);
		emit_prep_file(ura_scope, full_path);
		free(full_path);
	}

#if ASM
	init(path_name);
	// Two passes so calls reference fully-typed callees:
	// 1) Generate STRUCT_DEFs first (so STRUCT_CALL params have llvm.struct_type).
	// 2) Pre-declare every top-level FDEC's signature.
	// 3) Emit the rest of the top-level tree (fn bodies + globals).
	for (int i = 0; error_count == 0 && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type == STRUCT_DEF)
			asm_gen(child);
	}
	for (int i = 0; error_count == 0 && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		Node *fn    = NULL;
		if (child->token->type == FDEC)                                  fn = child;
		else if (child->token->type == PROTO && child->left
		         && child->left->token->type == FDEC)                    fn = child->left;
		if (fn && fn->token->name) {
			bool is_method = strchr(fn->token->name, '.') != NULL;
			bool is_main   = strcmp(fn->token->name, "main") == 0;
			if (fn->token->used > 0 || is_method || is_main)
				resolve_func_type(fn);
		}
	}
	for (int i = 0; error_count == 0 && i < ura_scope->children_count; i++) {
		Node *child = ura_scope->children[i];
		if (child->token->type == STRUCT_DEF) continue;
		asm_gen(child);
	}
	if (error_count == 0) finalize(ll_path);
#endif
}

// ============================================================================
// MAIN
// ============================================================================
int main(int argc, char **argv) {
	ura_lib = getenv("URA_LIB");
	if (CHECK(!ura_lib, "URA_LIB not set")) return 1;
	if (CHECK(argc < 2, "usage: ura <file.ura> [-O0|-O1|-O2|-O3|-Os|-Oz] [-san]"
	                    " [-o output]"))
		return 1;

	char *output  = NULL;
	char *entry   = NULL;
	bool  no_exec = false;
	(void)no_exec;
	for (int i = 1; i < argc && !found_error; i++) {
		char *a = argv[i];
#define MATCH(name, var, val)                                                                      \
	if (strcmp(a, name) == 0) {                                                                     \
		var = val;                                                                                   \
		continue;                                                                                    \
	}
		MATCH("-san", enable_san, true);
		MATCH("-O0", flags, PASSES_O0);
		MATCH("-no-exec", no_exec, true) MATCH("-O1", flags, PASSES_O1);
		MATCH("-no-debug", enable_debug, false);
		MATCH("-O2", flags, PASSES_O2);
		MATCH("-O3", flags, PASSES_O3);
		MATCH("-Os", flags, PASSES_Os);
		MATCH("-Oz", flags, PASSES_Oz);
		MATCH("-prep", enable_prep, true);
#undef MATCH
		if (strcmp(a, "-o") == 0) {
			CHECK(i + 1 >= argc, "-o requires an argument");
			output = argv[++i];
		} else if (a[0] != '-') {
			size_t n = strlen(a);
			CHECK(n <= 4 || strcmp(a + n - 4, ".ura") != 0, "invalid file: %s\n", a);
			CHECK(entry != NULL, "you can compile only one file "
			                     "try importing them in one files");
			entry = a;
		} else {
			CHECK(true, "unknown flag: %s\n", a);
		}
	}
	if (CHECK(!entry, "required .ura files as arguments to compile")) return 1;
	if (found_error)                                                  return 1;

	if (!output) output = "exe.out";

	compile(entry);

#if ASM
	if (ll_path && !no_exec && !found_error) {
		char *san = enable_san ? " -fsanitize=address,undefined -fno-omit-frame-pointer -g" : "";
		char *cmd = format("clang%s \"%s\" -o \"%s\"", san, ll_path, output);
		CHECK(system(cmd) != 0, "linking failed\n");
		free(cmd);
	}
#endif
	free_memory();
	return error_count > 0 ? 1 : 0;
}
