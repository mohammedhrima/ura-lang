#include "header.h"

UraGlobal ura;

void parse_arguments(int argc, char **argv)
{
   // ura.lib = getenv("URA_LIB");
   // if (CHECK(!ura.lib, "URA_LIB not set")) return;
   ura.output = "exe.out";
   
   // TODO: compile only 1 file, if alot import them with each others
   for(int i = 1; i < argc && !ura.error_count; i++) {
      char *arg = argv[i];
      size_t n = strlen(arg);
      CHECK(n <= 4 || strcmp(arg + n - 4, ".ura") != 0, "invalid file: %s\n", arg);
      new_source(arg);
   }
   if(ura.error_count) return;
   // TODO: error if there is no input
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
      if (lex_multi_coment(content, &i, &line, indent, default_indent)) continue;
      if (lex_coment(content, &i, line, indent, default_indent)) continue;
      if (lex_chars(content, &i, line, indent, default_indent)) continue;
      if (lex_char(content, &i, line, indent, default_indent)) continue;
      if (lex_number(content, &i, line, indent, default_indent)) continue;
      if (lex_identifier(content, &i, line, indent, default_indent)) continue;
      if (lex_symbol(content, &i, line, &indent, default_indent)) continue;
      tokenize_error(line, i, i + 1, "unexpected character '%c'", c);
   }
   if (!ura.calling_use)
   {
      parse_token(line, s, i, END, -1);
      for(int i = 0; i < ura.tokens_count; i++)
         debug("token %k\n", ura.tokens[i]);
   }
   exit_source();
}

int MAX_OP = 1000;
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

Token *next() { return ura.tokens[ura.exe_pos++]; }
Token *peek(int index) { return ura.tokens[ura.exe_pos + index];}

Node *prime_node() {
   Token *token = next();
   switch(token->type)
   {
   case INT: return new_node(token);
   case FDEC: {
      Node *node = new_node(token);
   	Token *fname = find(ID, 0);
	   // TODO: check if name does not exists
	   set_name(node->token, fname->name);
      return fdec_node(node);
   }
   case ID: {
      if (peek(0)->type != LPAR)
      {
         if (includes(peek(0)->type, INT, 0))
         {
            token->ret_type = peek(0)->type; // declared type
            token->is_dec = true;
            next(); // skip data type
         }
         return new_node(token);
      }
      if (strcmp(token->name, "main") == 0) // main func
         return fdec_node(new_node(token));
      // fcall
      break;
   }
   case RETURN: {
      Node *node = new_node(token);
      node->left = expr_node(MAX_OP);
      return node;
   }
   default:
      break;
   }
   debug(RED("Unexpected token %k\n"), token);
   return syntax_error();
}

Node *expr_node(int min_op) {
   Node *left = prime_node();

   while(true)
   {
      int op = get_operation_precedence(ura.tokens[ura.exe_pos]->type);
      if(op <= min_op) break;
      Node *node = new_node(next());
      node->left = left;
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
      resize_array(head->children, Node *, head->children_size, head->children_count);
      head->children[head->children_count++] = expr_node(0);
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

void resolve(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         enter_scope(node);
         for (int i = 0; i < node->children_count; i++)
            resolve(node->children[i]);
         exit_scope();
         break;
      }
      case ID: {
         if (token->is_dec) declare_variable(token);
         else {
            Token *decl = find_variable(token->name);
            if (CHECK(!decl, "undeclared variable '%s'", token->name)) return;
            token->ret_type = decl->ret_type;
         }
         break;
      }
      case INT: break;
      case RETURN: {
         resolve(node->left);
         break;
      }
      default: {
         resolve(node->left);
         resolve(node->right);
         break;
      }
   }
}

void typecheck(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         for (int i = 0; i < node->children_count; i++)
            typecheck(node->children[i]);
         break;
      }
      case INT: {
         token->ret_type = INT;
         break;
      }
      case ID: break;
      case RETURN: {
         typecheck(node->left);
         break;
      }
      default: {
         typecheck(node->left);
         typecheck(node->right);
         if (CHECK(
            node->left->token->ret_type != node->right->token->ret_type, 
            "type mismatch in assignment"))
            return;
         token->ret_type = node->left->token->ret_type;
         break;
      }
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

void codegen(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         TypeRef fn_type  = LLVMFunctionType(ura.i32, NULL, 0, 0);
         token->llvm.elem = LLVMAddFunction(ura.module, token->name, fn_type);
         Block entry      = LLVMAppendBasicBlockInContext(ura.context, token->llvm.elem, "entry");
         LLVMPositionBuilderAtEnd(ura.builder, entry);
         enter_scope(node);
         for (int i = 0; i < node->children_count; i++)
            codegen(node->children[i]);
         exit_scope();
         break;
      }
      case INT: {
         token->llvm.elem = LLVMConstInt(ura.i32, token->Int.value, 0);
         break;
      }
      case ID: {
         if (token->is_dec)
            token->llvm.elem = LLVMBuildAlloca(ura.builder, ura.i32, token->name);
         else {
            Token *decl      = find_variable(token->name);
            token->llvm.elem = LLVMBuildLoad2(ura.builder, ura.i32, decl->llvm.elem, token->name);
         }
         break;
      }
      case RETURN: {
         codegen(node->left);
         token->llvm.elem = LLVMBuildRet(ura.builder, node->left->token->llvm.elem);
         break;
      }
      default: {
         codegen(node->left);
         codegen(node->right);
         LLVMBuildStore(ura.builder, node->right->token->llvm.elem, node->left->token->llvm.elem);
         token->llvm.elem = node->right->token->llvm.elem;
         break;
      }
   }
}

void finalize_module(char *ll_path) {
   char *error = NULL;
   if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
      CHECK(1, "module verification failed:\n%s", error);
   LLVMDisposeMessage(error);
   LLVMPrintModuleToFile(ura.module, ll_path, NULL);
}

void generate_ir(){
   if(ura.error_count || !ura.head) return;
   for (int i = 0; i < ura.head->children_count; i++)
      resolve(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      typecheck(ura.head->children[i]);
}

void generate_asm(){
   if(ura.error_count || !ura.head) return;
   init_module(ura.output);
   for (int i = 0; i < ura.head->children_count; i++)
      codegen(ura.head->children[i]);
   mkdir("build", 0755);
   finalize_module("build/out.ll");
   if (ura.error_count) return;
   char *cmd = format("clang build/out.ll -o %s 2>/dev/null", ura.output);
   system(cmd);
   free(cmd);
   debug(GREEN("compiled -> %s\n"), ura.output);
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

void free_token(Token *token) {
   if (!token) return;
   free(token->name);
   free(token->Chars.value);
   free(token->llvm.dims);
   free(token);
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

int main(int argc, char**argv) {
   ura.max_errors = 20;
   ura.enable_debug = true;
   parse_arguments(argc, argv);
   tokenize(0);
   generate_ast();
   generate_ir();
   generate_asm();
   free_memory();
   return ura.error_count != 0;
}
