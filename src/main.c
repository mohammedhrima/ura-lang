#include "header.h"

UraGlobal ura;

void parse_arguments(int argc, char **argv)
{
   // ura.lib = getenv("URA_LIB");
   // if (CHECK(!ura.lib, "URA_LIB not set")) return;
   ura.output = "exe.out";
   
   for (int i = 1; i < argc && !ura.error_count; i++) {
      char *arg = argv[i];
#define MATCH(name, field, val) if (strcmp(arg, name) == 0) { field = val; continue; }
      MATCH("-debug", ura.enable_debug, true);
      MATCH("-exec",  ura.enable_exec,  true);
      MATCH("-san",   ura.enable_san,   true);
      MATCH("-O0", ura.flags, PASSES_O0);   
      MATCH("-O1", ura.flags, PASSES_O1);
      MATCH("-O2", ura.flags, PASSES_O2);   
      MATCH("-O3", ura.flags, PASSES_O3);
      MATCH("-Os", ura.flags, PASSES_Os);   
      MATCH("-Oz", ura.flags, PASSES_Oz);
#undef MATCH
      if (strcmp(arg, "-o") == 0) {
         if (i + 1 >= argc) { parse_error(NULL, "Missing argument for '-o'"); return; }
         ura.output = argv[++i];
      } else if (arg[0] == '-') {
         parse_error(NULL, "Unknown flag '%s'", arg);
      } else {
         size_t n = strlen(arg);
         if (n <= 4 || strcmp(arg + n - 4, ".ura") != 0) parse_error(NULL, "Invalid file '%s'", arg);
         else new_source(arg);
      }
   }
   if (ura.error_count) return;
   if (!ura.sources) parse_error(NULL, "No input file (usage: ura <file.ura> [-o out] [-O0..-Oz] [-san] [-debug])");
}

void tokenize(int default_indent) {
   if (ura.error_count || !ura.sources) return;
   Source *src = ura.sources[ura.sources_pos - 1];
   char *content = src->content;
   int line = 1;
   int indent = default_indent;
   
   int s, i = 0;
   while (content && content[i] && !ura.error_count) {
      s = i;
      char c = content[i];
      if (lex_spaces(content, &i, &line, &indent, default_indent)) continue;
      if (lex_multi_comment(content, &i, &line, indent, default_indent)) continue;
      if (lex_comment(content, &i, line, indent, default_indent)) continue;
      if (lex_chars(content, &i, line, indent, default_indent)) continue;
      if (lex_char(content, &i, line, indent, default_indent)) continue;
      if (lex_number(content, &i, line, indent, default_indent)) continue;
      if (lex_identifier(content, &i, line, indent, default_indent)) continue;
      if (lex_symbol(content, &i, line, &indent, default_indent)) continue;
      tokenize_error(line, i, i + 1, "Unexpected character '%c'", c);
   }
   if (!ura.calling_use)
   {
      parse_token(line, s, i, END, -1);
      for(int i = 0; i < ura.tokens_count; i++)
         debug("token %k\n", ura.tokens[i]);
   }
   exit_source();
}

Node *prime_node() {
   Token *token = next();
   switch(token->type)
   {
   case INT: case BOOL: case CHARS: case CHAR: case FLOAT: return new_node(token);
   case LPAR: {
      Node *node = expr_node(0);
      if (!find(RPAR, 0))
         parse_error(token, "Expected ')' after expression");
      return node;
   }
   case PROTO:
      if (peek(0)->type != FDEC) {
         parse_error(token, "Expected 'fn' after 'proto'");
         return syntax_error();
      }
      peek(0)->is_proto = true;
      return prime_node();
   case FDEC: {
      Node *node = new_node(token);
      Token *fname = find(ID, 0);
      if (!fname) {
         parse_error(token, "Expected a function name after 'fn'");
         return syntax_error();
      }
      set_name(node->token, fname->name);
      return fdec_node(node);
   }
   case ID: {
      if (peek(0)->type == LPAR)
      {
         if (strcmp(token->name, "main") == 0) // main func
            return fdec_node(new_node(token));
         if (strcmp(token->name, "output") == 0)
            return output_node(new_node(token));
         return fcall_node(new_node(token));
      }
      if (is_data_type(peek(0)) || (peek(0)->type == FDEC && peek(1)->type == LPAR))
      {
         token->is_dec = true;
         parse_type(token);
      }
      if (!token->is_dec && peek(0)->type == OPTIONAL) {
         find(OPTIONAL, 0);
         token->is_nullable = true;
      }
      return new_node(token);
   }
   case RETURN: {
      Node *node = new_node(token);
      node->left = expr_node(0);
      return node;
   }
   case NOT: case BNOT: {           
      Node *node = new_node(token);
      node->left = prime_node();
      return node;
   }
   case SUB: {                     
      Node *node  = new_node(token);
      node->left  = new_node(new_token(INT, token->indent));
      node->right = prime_node();
      return node;
   }
   case REF: {
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
         return new_node(name);
      }
      Node *node  = new_node(token);
      node->left  = prime_node();
      return node;
   }
   default:
      break;
   }
   parse_error(token, "Unexpected token");
   return syntax_error();
}

Node *expr_node(int min_op) {
   Node *left = prime_node();

   while (true)
   {
      int op = get_operation_precedence(ura.tokens[ura.exe_pos]->type);
      if(op <= min_op) break;
      Node *node = new_node(next());
      node->left = left;
      if (node->token->type == AS) {
         Token *type_tok = new_token(ID, 0);
         parse_type(type_tok);
         node->right = new_node(type_tok);
      } else
         node->right = expr_node(op);
      left = node;
   }
   return left;
}

void generate_ast() {
   if(ura.error_count) return;
   Node *head = new_node(new_token(ID, -TAB));
   ura.head = head;
   enter_scope(head);
   while (!find(END, 0)) {
      int before = ura.error_count;
      Node *child = expr_node(0);
      if (ura.error_count > before) {
         parser_recover(0);
         if (ura.error_count > ura.max_errors) break;
         continue;
      }
      resize_array(head->children, Node *);
      head->children[head->children_count++] = child;
      // TODO: only function declarations and
      // struct declaration are allowed here
   }
   if (ura.error_count > 0) return;
	debug(GREEN("===========================================\n"));
	debug(GREEN("AFTER PARSING\n"));
	debug(GREEN("===========================================\n"));
	for (int i = 0; i < head->children_count; i++)
		pnode(head->children[i], "");
}

void analyze(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:   analyze_fdec(node); break;
      case ID:     analyze_id(node); break;
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT: break;
      case RETURN: analyze(node->left); break;
      case FCALL:  analyze_fcall(node); break;
      case NOT: case BNOT: analyze(node->left); break;
      case AS: analyze(node->left); break;
      case REF:
         analyze(node->left);
         if (node->left->token->type != ID)
            parse_error(node->token, "Cannot take a reference to a non-variable");
         break;
      case OUTPUT:
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         break;
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
         analyze(node->left);
         analyze(node->right);
         break;
      default:
         CHECK(1, "analyze: unhandled node '%s'", to_string(token->type));
         break;
   }
}

void type_check(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         break;
      case INT:     token->ret_type = INT; break;
      case BOOL:    token->ret_type = BOOL; break;
      case CHARS:   token->ret_type = CHARS; break;
      case CHAR:    token->ret_type = CHAR; break;
      case FLOAT:   token->ret_type = FLOAT; break;
      case ID:      break;
      case FN_TYPE: break;
      case RETURN:  type_check(node->left); break;
      case FCALL:   type_check_fcall(node); break;
      case OUTPUT:
         for (int i = 0; i < node->children_count; i++)
            type_check(node->children[i]);
         token->ret_type = VOID;
         break;
      case NOT: case BNOT:
         type_check(node->left);
         if (token->type == NOT && node->left->token->ret_type != BOOL)
            parse_error(token, "Operator 'not' needs a bool operand");
         token->ret_type = (token->type == NOT) ? BOOL : node->left->token->ret_type;
         break;
      case AS: {
         type_check(node->left);
         Type src = node->left->token->ret_type;
         Type dst = node->right->token->ret_type;
         if ((src && !includes(src, NUMERIC_TYPES, 0)) || !includes(dst, NUMERIC_TYPES, 0))
            parse_error(token, "Cannot cast %s to %s", to_string(src), to_string(dst));
         token->ret_type = dst;
         break;
      }
      case REF:
         type_check(node->left);
         token->ret_type = node->left->token->ret_type;
         break;
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
         type_check_binop(node); break;
      default:
         CHECK(1, "type_check: unhandled node '%s'", to_string(token->type));
         break;
   }
}

void code_gen(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:   code_gen_fdec(node); break;
      case INT:    token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0); break;
      case BOOL:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Bool.value, 0); break;
      case CHARS:  token->llvm.elem = LLVMBuildGlobalStringPtr(ura.builder, token->Chars.value, "str"); break;
      case CHAR:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Char.value, 0); break;
      case FLOAT:  token->llvm.elem = LLVMConstReal(to_llvm_type(token->ret_type), token->Float.value); break;
      case ID:     code_gen_id(node); break;
      case FN_TYPE:
         emit_signature(token->Fcall.ptr);
         token->llvm.elem = token->Fcall.ptr->token->llvm.elem;
         break;
      case RETURN:
         code_gen(node->left);
         token->llvm.elem = LLVMBuildRet(ura.builder, node->left->token->llvm.elem);
         break;
      case FCALL:  code_gen_fcall(node); break;
      case OUTPUT: code_gen_output(node); break;
      case NOT: case BNOT:
         code_gen(node->left);
         token->llvm.elem = LLVMBuildNot(ura.builder, node->left->token->llvm.elem, "not");
         break;
      case AS:
         code_gen(node->left);
         token->llvm.elem = LLVMBuildIntCast2(ura.builder, node->left->token->llvm.elem,
                                              to_llvm_type(token->ret_type), 1, "cast");
         break;
      case REF:    token->llvm.elem = address_of(node->left); break;
      case ASSIGN: code_gen_assign(node); break;
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: 
      case DIV_ASSIGN: case MOD_ASSIGN:
         code_gen_compound(node); 
         break;
      case ADD: case SUB: case MUL: case DIV: case MOD:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT:
      case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
         code_gen_binop(node);
         break;
      default:
         CHECK(1, "code_gen: unhandled node '%s'", to_string(token->type));
         break;
   }
}

void generate_ir() {
   if(ura.error_count || !ura.head) return;
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == FDEC)
         declare_function(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      analyze(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      type_check(ura.head->children[i]);
}

void generate_asm() {
   if(ura.error_count || !ura.head) return;
   setup_paths(ura.sources[0]->filename);
   init_module(ura.output);
   for (int i = 0; i < ura.head->children_count; i++)
      code_gen(ura.head->children[i]);
   finalize_module(ura.ll_path);
   if (ura.error_count) return;
   if (ura.enable_exec)
      fprintf(stderr, CYAN("%-9s") " " BLUE("%s (%s)") "\n",
              "Compiling", ura.base, ura.sources[0]->filename);
   char *cc  = ura.enable_san ? "/usr/bin/clang" : "clang";
   char *san = ura.enable_san ? " -fsanitize=address,undefined -fno-omit-frame-pointer -g" : "";
   char *cmd = format("%s%s %s -o %s 2>/dev/null", cc, san, ura.ll_path, ura.output);
   system(cmd);
   free(cmd);
   if (!ura.enable_exec) return;

   char *opt = ura.flags ? "optimized" : "unoptimized";
   char *tag = ura.enable_san ? " + sanitized" : "";
   fprintf(stderr, CYAN("%-9s") " \033[2m[%s%s]\033[0m in %.2fs\n",
           "Finished", opt, tag, clock_now() - ura.t_start);

   char  *run = strchr(ura.output, '/') ? ura.output : format("./%s", ura.output);
   fprintf(stderr, CYAN("%-9s") " " BLUE("%s") "\n", "Running", run);
   double e0  = clock_now();
   pid_t  pid = fork();
   if (pid == 0) {
      execl(run, run, NULL);
      _exit(127);
   }
   if (pid < 0) return;
   int status = 0;
   waitpid(pid, &status, 0);
   double exec = clock_now() - e0;
   if (WIFSIGNALED(status))
      fprintf(stderr, RED("%-9s") " %s (signal %d) in %.2fs\n",
              "Crashed", signal_name(WTERMSIG(status)), WTERMSIG(status), exec);
   else
      fprintf(stderr, CYAN("%-9s") " with code %d in %.2fs\n",
              "Exited", WEXITSTATUS(status), exec);
}

int main(int argc, char**argv) {
   ura.t_start = clock_now();
   ura.max_errors = 20;
   ura.enable_debug = false;
   parse_arguments(argc, argv);
   tokenize(0);
   generate_ast();
   generate_ir();
   generate_asm();
   free_memory();
   return ura.error_count != 0;
}
