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
      // MATCH("-exec",  ura.enable_exec,  true);
      MATCH("-san",   ura.enable_san,   true);
      MATCH("-O0", ura.flags, PASSES_O0);   
      MATCH("-O1", ura.flags, PASSES_O1);
      MATCH("-O2", ura.flags, PASSES_O2);   
      MATCH("-O3", ura.flags, PASSES_O3);
      MATCH("-Os", ura.flags, PASSES_Os);   
      MATCH("-Oz", ura.flags, PASSES_Oz);
#undef MATCH
      if (strcmp(arg, "-o") == 0) {
         if (i + 1 >= argc) { parse_error(NULL, "'-o' requires an argument"); return; }
         ura.output = argv[++i];
      } else if (arg[0] == '-') {
         parse_error(NULL, "unknown flag '%s'", arg);
      } else {
         size_t n = strlen(arg);
         if (n <= 4 || strcmp(arg + n - 4, ".ura") != 0) parse_error(NULL, "invalid file '%s'", arg);
         else new_source(arg);
      }
   }
   if (ura.error_count) return;
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

Node *prime_node() {
   Token *token = next();
   switch(token->type)
   {
   case INT: return new_node(token);
   case FDEC: {
      Node *node = new_node(token);
      Token *fname = find(ID, 0);
      if (!fname) {
         parse_error(token, "expected a function name after 'fn'");
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
         return fcall_node(new_node(token));
      }
      if (is_data_type(peek(0)) || (peek(0)->type == FDEC && peek(1)->type == LPAR))
      {
         token->is_dec = true;
         parse_type(token);
      }
      return new_node(token);
   }
   case RETURN: {
      Node *node = new_node(token);
      node->left = expr_node(0);
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

   while (true)
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

void resolve(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         enter_scope(node);
         for (int i = 0; i < token->Fn.params_count; i++)
            declare_variable(token->Fn.params[i]);
         for (int i = 0; i < node->children_count; i++)
            if (node->children[i]->token->type == FDEC)
               declare_function(node->children[i]);
         for (int i = 0; i < node->children_count; i++)
            resolve(node->children[i]);
         exit_scope();
         break;
      }
      case ID: {
         if (token->is_dec) { declare_variable(token); break; }
         bool   captured = false;
         Token *decl     = find_variable(token->name, &captured);
         if (decl) {
            if (captured) {
               parse_error(token, "cannot use '%s' from an enclosing function - pass it as a parameter", token->name);
               return;
            }
            token->ret_type = decl->ret_type;
            if (decl->ret_type == FN_TYPE) token->Fn = decl->Fn;
            break;
         }
         Node *fn = find_function(token->name);
         if (!fn) { parse_error(token, "undeclared variable '%s'", token->name); return; }
         token->type             = FN_TYPE;
         token->ret_type         = FN_TYPE;
         token->Fcall.ptr        = fn;
         token->Fn.params        = fn->token->Fn.params;
         token->Fn.params_count  = fn->token->Fn.params_count;
         token->Fn.ret           = new_token(ID, 0);
         token->Fn.ret->ret_type = fn->token->ret_type;
         break;
      }
      case INT: break;
      case RETURN: {
         resolve(node->left);
         break;
      }
      case FCALL: {
         Token *var = find_variable(token->name, NULL);
         if (var && var->ret_type == FN_TYPE) {
            token->Fcall.var = var;
            for (int i = 0; i < node->children_count; i++)
               resolve(node->children[i]);
            token->ret_type = var->Fn.ret->ret_type;
            break;
         }
         Node *fn = find_function(token->name);
         if (!fn) { parse_error(token, "undeclared function '%s'", token->name); return; }
         token->Fcall.ptr = fn;
         for (int i = 0; i < node->children_count; i++)
            resolve(node->children[i]);
         token->ret_type = fn->token->ret_type;
         break;
      }
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD: {
         resolve(node->left);
         resolve(node->right);
         break;
      }
      default:
         CHECK(1, "resolve: unhandled node '%s'", to_string(token->type));
         break;
   }
}

void typecheck(Node *node) {
   if (!node) return;
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
      case FN_TYPE: break;
      case RETURN: {
         typecheck(node->left);
         break;
      }
      case FCALL: {
         bool indirect = token->Fcall.var != NULL;
         if (!indirect && !token->Fcall.ptr) return;
         Token *fn = indirect ? token->Fcall.var : token->Fcall.ptr->token;
         for (int i = 0; i < node->children_count; i++)
            typecheck(node->children[i]);
         if (node->children_count != fn->Fn.params_count) {
            parse_error(token, "wrong number of arguments to '%s'", token->name);
            return;
         }
         for (int i = 0; i < node->children_count; i++) {
            if (node->children[i]->token->ret_type &&
                node->children[i]->token->ret_type != fn->Fn.params[i]->ret_type) {
               parse_error(node->children[i]->token, "argument %d type mismatch in call to '%s'", i + 1, token->name);
               return;
            }
            if (fn->Fn.params[i]->is_ref && node->children[i]->token->type != ID) {
               parse_error(node->children[i]->token, "argument %d to '%s' must be a variable (ref parameter)", i + 1, token->name);
               return;
            }
         }
         token->ret_type = indirect ? fn->Fn.ret->ret_type : fn->ret_type;
         break;
      }
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD: {
         typecheck(node->left);
         typecheck(node->right);
         Type lt = node->left->token->ret_type;
         Type rt = node->right->token->ret_type;
         if (lt && rt && lt != rt) {
            parse_error(token, "type mismatch between operands");
            return;
         }
         token->ret_type = lt;
         break;
      }
      default:
         CHECK(1, "typecheck: unhandled node '%s'", to_string(token->type));
         break;
   }
}

void codegen_fn_signature(Node *fn) {
   Token *token = fn->token;
   if (token->llvm.func_type) return;
   int      n      = token->Fn.params_count;
   TypeRef *params = NULL;
   if (n > 0) {
      params = allocate(n, sizeof(TypeRef));
      for (int i = 0; i < n; i++) {
         TypeRef pt = llvm_type_of(token->Fn.params[i]);
         params[i] = token->Fn.params[i]->is_ref ? LLVMPointerType(pt, 0) : pt;
      }
   }
   token->llvm.func_type = LLVMFunctionType(to_llvm_type(token->ret_type), params, n, 0);
   token->llvm.elem      = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
   free(params);
}

Value address_of(Node *node) {
   Token *token = node->token;
   if (token->is_dec)
      return token->llvm.elem = LLVMBuildAlloca(ura.builder, llvm_type_of(token), token->name);
   Token *decl = find_variable(token->name, NULL);
   if (decl->is_ref)
      return LLVMBuildLoad2(ura.builder, LLVMPointerType(to_llvm_type(decl->ret_type), 0), decl->llvm.elem, "ref");
   return decl->llvm.elem;
}

void codegen(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC: {
         codegen_fn_signature(node);
        
         debug_enter_function(token);
         enter_scope(node);
         for (int i = 0; i < token->Fn.params_count; i++) {
            Token  *param = token->Fn.params[i];
            TypeRef pt    = llvm_type_of(param);
            if (param->is_ref) pt = LLVMPointerType(pt, 0);
            param->llvm.elem = LLVMBuildAlloca(ura.builder, pt, param->name);
            LLVMBuildStore(ura.builder, LLVMGetParam(token->llvm.elem, i), param->llvm.elem);
         }
         for (int i = 0; i < node->children_count; i++) {
            set_debug_location(node->children[i]->token);
            codegen(node->children[i]);
         }
         if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder))) {
            if (token->ret_type == VOID) LLVMBuildRetVoid(ura.builder);
            else LLVMBuildRet(ura.builder, default_value(token));
         }
         exit_scope();

         debug_exit_function(token);
         break;
      }
      case INT: {
         token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0);
         break;
      }
      case ID: {
         if (token->is_dec) {
            TypeRef t        = llvm_type_of(token);
            token->llvm.elem = LLVMBuildAlloca(ura.builder, t, token->name);
            LLVMBuildStore(ura.builder, default_value(token), token->llvm.elem);
         }
         else {
            Token  *decl = find_variable(token->name, NULL);
            TypeRef t    = llvm_type_of(decl);
            if (decl->is_ref) {
               Value ptr = LLVMBuildLoad2(ura.builder, LLVMPointerType(t, 0), decl->llvm.elem, "ref");
               token->llvm.elem = LLVMBuildLoad2(ura.builder, t, ptr, token->name);
            } else
               token->llvm.elem = LLVMBuildLoad2(ura.builder, t, decl->llvm.elem, token->name);
         }
         break;
      }
      case FN_TYPE: {
         codegen_fn_signature(token->Fcall.ptr);
         token->llvm.elem = token->Fcall.ptr->token->llvm.elem;
         break;
      }
      case RETURN: {
         codegen(node->left);
         token->llvm.elem = LLVMBuildRet(ura.builder, node->left->token->llvm.elem);
         break;
      }
      case FCALL: {
         bool   indirect = token->Fcall.var != NULL;
         Token *fn       = indirect ? token->Fcall.var : token->Fcall.ptr->token;
         if (!indirect) codegen_fn_signature(token->Fcall.ptr);
         int    n    = node->children_count;
         Value *args = NULL;
         if (n > 0) {
            args = allocate(n, sizeof(Value));
            for (int i = 0; i < n; i++) {
               if (fn->Fn.params[i]->is_ref)
                  args[i] = address_of(node->children[i]);
               else {
                  codegen(node->children[i]);
                  args[i] = node->children[i]->token->llvm.elem;
               }
            }
         }
         if (indirect) {
            TypeRef ptr_type = llvm_type_of(fn);
            Value   fn_ptr   = LLVMBuildLoad2(ura.builder, ptr_type, fn->llvm.elem, "fn");
            char   *name     = fn->Fn.ret->ret_type == VOID ? "" : "call";
            token->llvm.elem = LLVMBuildCall2(ura.builder, LLVMGetElementType(ptr_type), fn_ptr, args, n, name);
         } else {
            char *name       = fn->ret_type == VOID ? "" : "call";
            token->llvm.elem = LLVMBuildCall2(ura.builder, fn->llvm.func_type, fn->llvm.elem, args, n, name);
         }
         free(args);
         break;
      }
      case ASSIGN: {
         Value dest = address_of(node->left);
         codegen(node->right);
         LLVMBuildStore(ura.builder, node->right->token->llvm.elem, dest);
         token->llvm.elem = node->right->token->llvm.elem;
         break;
      }
      case ADD: case SUB: case MUL: case DIV: case MOD: {
         codegen(node->left);
         codegen(node->right);
         Value left  = node->left->token->llvm.elem;
         Value right = node->right->token->llvm.elem;
         if (token->type == DIV || token->type == MOD) guard_nonzero(token, right);
         switch (token->type) {
            case ADD: token->llvm.elem = LLVMBuildAdd(ura.builder, left, right, "add");  break;
            case SUB: token->llvm.elem = LLVMBuildSub(ura.builder, left, right, "sub");  break;
            case MUL: token->llvm.elem = LLVMBuildMul(ura.builder, left, right, "mul");  break;
            case DIV: token->llvm.elem = LLVMBuildSDiv(ura.builder, left, right, "div"); break;
            case MOD: token->llvm.elem = LLVMBuildSRem(ura.builder, left, right, "mod"); break;
            default: break;
         }
         break;
      }
      default: {
         CHECK(1, "codegen: unhandled node '%s'", to_string(token->type));
         break;
      }
   }
}

void generate_ir() {
   if(ura.error_count || !ura.head) return;
   for (int i = 0; i < ura.head->children_count; i++)
      if (ura.head->children[i]->token->type == FDEC)
         declare_function(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      resolve(ura.head->children[i]);
   for (int i = 0; i < ura.head->children_count; i++)
      typecheck(ura.head->children[i]);
}

void generate_asm() {
   if(ura.error_count || !ura.head) return;
   setup_paths(ura.sources[0]->filename);
   init_module(ura.output);
   for (int i = 0; i < ura.head->children_count; i++)
      codegen(ura.head->children[i]);
   finalize_module(ura.ll_path);
   if (ura.error_count) return;
   char *cc  = ura.enable_san ? "/usr/bin/clang" : "clang";
   char *san = ura.enable_san ? " -fsanitize=address,undefined -fno-omit-frame-pointer -g" : "";
   char *cmd = format("%s%s %s -o %s 2>/dev/null", cc, san, ura.ll_path, ura.output);
   system(cmd);
   free(cmd);
   debug(GREEN("compiled -> %s\n"), ura.output);
}

int main(int argc, char**argv) {
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
