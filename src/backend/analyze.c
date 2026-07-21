#include "header.h"

void enter_scope(Node *node) {
	ura.scopes_count++;
	debug(CYAN("Enter Scope: %k index %d\n"), node->token, ura.scopes_count);
	resize_array(ura.scopes, Node *);
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

Node *enclosing_continue() {
	for (int i = ura.scopes_count; i >= 1; i--) {
		if (!ura.scopes[i]) continue;
		if (includes(ura.scopes[i]->token->type, WHILE, LOOP, FOR, 0)) return ura.scopes[i];
		if (ura.scopes[i]->token->type == FDEC)  return NULL;
	}
	return NULL;
}

Node *enclosing_break() {
	for (int i = ura.scopes_count; i >= 1; i--) {
		if (!ura.scopes[i]) continue;
		if (includes(ura.scopes[i]->token->type, WHILE, LOOP, FOR, MATCH, 0)) return ura.scopes[i];
		if (ura.scopes[i]->token->type == FDEC) return NULL;
	}
	return NULL;
}

void declare_structs(Node *node) {
	for (int i = 0; i < node->children_count; i++)
		if (node->children[i]->token->type == STRUCT_DEF)
			declare_struct(node->children[i]);
}

void analyze_block(Node *node) {
	analyze(node->left);
	enter_scope(node);
	declare_structs(node);
	for (int i = 0; i < node->children_count; i++)
		analyze(node->children[i]);
	exit_scope();
	analyze(node->right);
}

void analyze_match(Node *node) {
	analyze(node->left);
	enter_scope(node);
	for (int i = 0; i < node->children_count; i++) {
		Node *branch = node->children[i];
		if (branch->left)
			for (int j = 0; j < branch->left->children_count; j++)
				analyze(branch->left->children[j]);
		enter_scope(branch);
		declare_structs(branch);
		for (int j = 0; j < branch->children_count; j++)
			analyze(branch->children[j]);
		exit_scope();
	}
	exit_scope();
}

void declare_variable(Token *token) {
	Node *scope     = ura.scope;
	bool  in_struct = scope->token->type == STRUCT_DEF;
	char *err = in_struct ? ERR_STRUCT_DUP_FIELD : ERR_REDECL_VARIABLE;
	for (int i = 0; i < scope->variables_count; i++)
		if (strcmp(scope->variables[i]->name, token->name) == 0) {
			parse_error(token, err, token->name);
			return;
		}
	resize_array(scope->variables, Token *);
	scope->variables[scope->variables_count++] = token;
}

Token *find_variable(char *name, bool *captured) {
	bool passed_function = false;
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->variables_count; j++) {
			char *current = scope->variables[j]->name;
			if (!current || strcmp(current, name) != 0) continue;
			bool from_outer_fn = passed_function && scope->token->type == FDEC;
			if (captured) *captured = from_outer_fn;
			return scope->variables[j];
		}
		if (scope->token->type == FDEC) passed_function = true;
	}
	if (captured) *captured = false;
	return NULL;
}

char *signature_diff(Token *old, Token *new) {
	if (old->Fn.params_count != new->Fn.params_count)
		return format("expected %d parameter(s), found %d",
		              old->Fn.params_count, new->Fn.params_count);
	for (int i = 0; i < old->Fn.params_count; i++) {
		Type a = old->Fn.params[i]->ret_type;
		Type b = new->Fn.params[i]->ret_type;
		if (!same_or_pointer(a, b))
			return format("parameter %d is %s, expected %s", i + 1, type_name(b), type_name(a));
	}
	if (!same_or_pointer(old->ret_type, new->ret_type))
		return format("returns %s, expected %s", type_name(new->ret_type), type_name(old->ret_type));
	return format("one is variadic, the other is not");
}

bool same_signature(Token *a, Token *b) {
	if (!same_or_pointer(a->ret_type, b->ret_type)) return false;
	if (a->is_variadic != b->is_variadic) return false;
	if (a->Fn.params_count != b->Fn.params_count) return false;
	for (int i = 0; i < a->Fn.params_count; i++)
		if (!same_or_pointer(a->Fn.params[i]->ret_type,  b->Fn.params[i]->ret_type))
			return false;
	return true;
}

void declare_function(Node *fn) {
	Token *new = fn->token;
	for (int i = 0; i < ura.scope->functions_count; i++) {
		Token *old = ura.scope->functions[i]->token;
		if (strcmp(old->name, new->name) != 0) continue;
		if (!old->is_proto && !new->is_proto) {
			parse_error(new, ERR_REDECL_FUNCTION, new->name);
			parse_note(old, NOTE_PREV_DECL, new->name);
			return;
		}
		if (!same_signature(old, new)) {
			parse_error(new, ERR_SIG_CONFLICT, new->name, signature_diff(old, new));
			parse_note(old, NOTE_PREV_DECL, new->name);
			return;
		}
		if (old->is_proto && !new->is_proto)
			ura.scope->functions[i] = fn;
		return;
	}
	resize_array(ura.scope->functions, Node *);
	ura.scope->functions[ura.scope->functions_count++] = fn;
}

void declare_struct(Node *node) {
	Token *token = node->token;
	for (int i = 0; i < ura.scope->structs_count; i++)
		if (strcmp(ura.scope->structs[i]->token->name, token->name) == 0) {
			parse_error(token, "Redeclaration of struct '%s'", token->name);
			return;
		}
	resize_array(ura.scope->structs, Node *);
	ura.scope->structs[ura.scope->structs_count++] = node;
}

void declare_enum(Node *node) {
	Token *token = node->token;
	for (int i = 0; i < ura.scope->structs_count; i++)
		if (strcmp(ura.scope->structs[i]->token->name, token->name) == 0) {
			parse_error(token, "Redeclaration of type '%s'", token->name);
			return;
		}
	resize_array(ura.scope->structs, Node *);
	ura.scope->structs[ura.scope->structs_count++] = node;
	for (int i = 0; i < node->children_count; i++)
		declare_variable(node->children[i]->token);
}

Node *find_struct(char *name) {
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->structs_count; j++) {
			char *current = scope->structs[j]->token->name;
			if (current && strcmp(current, name) == 0)
				return scope->structs[j];
		}
	}
	return NULL;
}

Node *find_function(char *name) {
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->functions_count; j++) {
			char *current = scope->functions[j]->token->name;
			if (current && strcmp(current, name) == 0)
				return scope->functions[j];
		}
	}
	return NULL;
}

void analyze_binop(Node *node) {
   analyze(node->left);
   analyze(node->right);
}

void analyze_fdec(Node *node) {
   Token *token = node->token;
   resolve_struct_type(token);
   enter_scope(node);
   for (int i = 0; i < token->Fn.params_count; i++) {
      declare_variable(token->Fn.params[i]);
      resolve_struct_type(token->Fn.params[i]);
   }
   for (int i = 0; i < node->children_count; i++)
      if (node->children[i]->token->type == FDEC)
         declare_function(node->children[i]);
   declare_structs(node);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   exit_scope();
}

void resolve_struct_type(Token *token) {
   bool array = token->ret_type == ARRAY_TYPE;
   Type type  = array ? token->Array.sub_type : token->ret_type;
   if (type != STRUCT_CALL) return;
   Node *def = find_struct(token->Struct.name);
   if (!def) {
      parse_error(token, ERR_UNKNOWN_TYPE, token->Struct.name);
      return;
   }
   if (def->token->type == ENUM_DEF) {
      if (array) token->Array.sub_type = I32;
      else       token->ret_type       = I32;
      return;
   }
   if (array) token->Array.struct_ptr = def;
   else       token->Struct.ptr       = def;
}

bool has_assign(Node *def) {
   char *prefix = format("%s.=", def->token->name);
   size_t n     = strlen(prefix);
   bool  found  = false;
   for (int i = 0; i < def->children_count && !found; i++) {
      Token *child = def->children[i]->token;
      if (child->type != FDEC || !child->name) continue;
      found = strncmp(child->name, prefix, n) == 0;
   }
   free(prefix);
   return found;
}

void analyze_struct(Node *node) {
   if (node->token->has_drop && !has_assign(node))
      parse_warn(node->token, WARN_DROP_NEEDS_ASSIGN, node->token->name);
   enter_scope(node);
   for (int i = 0; i < node->children_count; i++)
      if (node->children[i]->token->type == FDEC)
         declare_function(node->children[i]);
   for (int i = 0; i < node->children_count; i++) {
      Node *field = node->children[i];
      if (field->token->type == STRUCT_DEF) declare_struct(field);
      analyze(field);
   }
   exit_scope();
}

bool rewrite_struct_ctor(Node *node) {
   Node *base  = node;
   int   depth = 0;
   while (base->token->type == ACCESS) { base = base->left; depth++; }
   Token *name = base->token;
   if (name->type != ID || name->is_dec) return false;
   if (find_variable(name->name, NULL)) return false;
   Node *def = find_struct(name->name);
   if (!def) return false;
   Node **dims = allocate(depth, sizeof(Node *));
   Node  *cur  = node;
   for (int i = depth - 1; i >= 0; i--) {
      dims[i] = cur->right;
      cur     = cur->left;
   }
   Token *token           = node->token;
   token->type            = ARRAY;
   token->ret_type        = ARRAY_TYPE;
   token->Struct.name     = name->name;
   token->Array.sub_type  = STRUCT_CALL;
   token->Array.depth     = depth;
   token->Array.struct_ptr = def;
   node->left             = NULL;
   node->right            = NULL;
   node->children         = dims;
   node->children_count   = depth;
   node->children_size    = depth;
   return true;
}

Token *global_decl(Node *child) {
   Token *token = child->token;
   if (token->type == ID && token->is_dec) return token;
   if (token->type == ASSIGN && child->left->token->is_dec)
      return child->left->token;
   return NULL;
}

void analyze_id(Node *node) {
   Token *token = node->token;
   if (token->is_dec) {
      if (!token->is_global) declare_variable(token);
      resolve_struct_type(token);
      return;
   }
   bool   captured = false;
   Token *decl     = find_variable(token->name, &captured);
   if (decl && decl->type == ENUM_CALL) {
      token->type      = I32;
      token->ret_type  = I32;
      token->Int.value = decl->Int.value;
      decl->used++;
      return;
   }
   if (decl) {
      if (captured) {
         parse_error(token, ERR_CAPTURE_NOT_ALLOWED, token->name);
         return;
      }
      decl->used++;
      token->Decl.ptr    = decl;
      token->ret_type    = decl->ret_type;
      token->is_optional = decl->is_optional;
      token->is_ref      = decl->is_ref;
      if (decl->ret_type == FN_TYPE) token->Fn = decl->Fn;
      if (decl->ret_type == ARRAY_TYPE) token->Array = decl->Array;
      if (decl->ret_type == STRUCT_CALL) token->Struct = decl->Struct;
      return;
   }
   Node *fn = find_function(token->name);
   if (!fn) { 
      parse_error(token, ERR_UNDECLARED_VARIABLE, token->name); 
      return; 
   }
   token->type             = FN_TYPE;
   token->ret_type         = FN_TYPE;
   token->Fcall.ptr        = fn;
   token->Fn.params        = fn->token->Fn.params;
   token->Fn.params_count  = fn->token->Fn.params_count;
   token->Fn.ret           = new_token(ID, 0);
   token->Fn.ret->ret_type = fn->token->ret_type;
}

Node *find_method(Node *def, char *name) {
   for (int i = 0; i < def->children_count; i++) {
      Node *child = def->children[i];
      if (child->token->type != FDEC) continue;
      if (strcmp(child->token->name, name) == 0) return child;
   }
   return NULL;
}

void analyze_method_call(Node *node) {
   analyze(node->left);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
}

void analyze_fcall(Node *node) {
   Token *token = node->token;
   if (token->is_method_call) {
      analyze_method_call(node);
      return;
   }
   if (token->is_static_call) {
      for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
      return;
   }
   Token *var = find_variable(token->name, NULL);
   if (var && var->ret_type == FN_TYPE) {
      token->Fcall.var = var;
      for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
      token->ret_type = var->Fn.ret->ret_type;
      return;
   }
   Node *fn = find_function(token->name);
   if (!fn) { 
      parse_error(token, "Undeclared function '%s'", token->name); 
      return; 
   }
   token->Fcall.ptr = fn;
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   token->ret_type = fn->token->ret_type;
   if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
}

void analyze_for(Node *node) {
   enter_scope(node);
   analyze(node->right);
   Token *var = node->left->token;
   Token *it  = node->right->token;
   if (it->type != RANGE && !it->ret_type) type_check(node->right);
   for (int i = 0; i < node->right->children_count; i++)
      analyze(node->right->children[i]);
   if (it->type == RANGE)
      var->ret_type = I32;
   else if (it->ret_type == ARRAY_TYPE) {
      var->Array       = it->Array;
      var->Array.depth = it->Array.depth - 1;
      var->ret_type    = it->Array.depth > 1 ? ARRAY_TYPE : it->Array.sub_type;
      if (var->ret_type == STRUCT_CALL)
         var->Struct.ptr = it->Array.struct_ptr;
   }
   declare_variable(var);
   declare_structs(node);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   exit_scope();
}

void analyze(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:   analyze_fdec(node); break;
      case STRUCT_DEF: analyze_struct(node); break;
      case ENUM_DEF: break;
      case ID:     analyze_id(node); break;
      case I32: case BOOL: case CHARS: case CHAR: case F32: break;
      case NULL_LIT: break;
      case FALLBACK: analyze_binop(node); break;
      case RETURN: analyze(node->left); break;
      case FCALL:  analyze_fcall(node); break;
      case NOT: case BNOT: analyze(node->left); break;
      case AS: analyze(node->left); break;
      case REF: {
         analyze(node->left);
         Type k = node->left->token->type;
         if (!includes(k, ID, ACCESS, DOT, 0))
            parse_error(node->token, ERR_REF_TO_NON_VARIABLE);
         break;
      }
      case IF: case ELIF: case ELSE: case WHILE: case LOOP:
         analyze_block(node);
         break;
      case FOR: analyze_for(node); break;
      case MATCH: analyze_match(node); break;
      case BREAK: {
         node->left = enclosing_break();
         if (!node->left)
         parse_error(node->token, "'break' outside a loop or match");
         break;
      }
      case CONTINUE: {
         node->left = enclosing_continue();
         if (!node->left)
         parse_error(node->token, "'continue' outside a loop");
         break;
      }
      case OUTPUT: {
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case ARRAY_LIT: {
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case ACCESS: {
         if (!rewrite_struct_ctor(node)) { 
            analyze_binop(node); 
            break; 
         }
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case RANGE:  analyze_binop(node); break;
      case DOT:    analyze(node->left); break;
      case TYPEOF: case SIZEOF: analyze(node->left); break;
      case ARRAY: {
         resolve_struct_type(token);
         for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
         break;
      }
      case CLEAN: analyze(node->left); break;
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
      case MOD_ASSIGN: case BAND_ASSIGN: case BOR_ASSIGN: case BXOR_ASSIGN:
      case LSHIFT_ASSIGN: case RSHIFT_ASSIGN:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
         analyze_binop(node);
         break;
      default:
         CHECK(1, "analyze: unhandled node '%s'", to_string(token->type));
         break;
   }
}
