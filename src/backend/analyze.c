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

void analyze_block(Node *node) {
	analyze(node->left);
	enter_scope(node);
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
		for (int j = 0; j < branch->children_count; j++)
			analyze(branch->children[j]);
		exit_scope();
	}
	exit_scope();
}

void declare_variable(Token *token) {
	for (int i = 0; i < ura.scope->variables_count; i++)
		if (strcmp(ura.scope->variables[i]->name, token->name) == 0) {
			parse_error(token, "Redeclaration of variable '%s'", token->name);
			return;
		}
	resize_array(ura.scope->variables, Token *);
	ura.scope->variables[ura.scope->variables_count++] = token;
}

Token *find_variable(char *name, bool *captured) {
	bool passed_function = false;
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->variables_count; j++)
			if (scope->variables[j]->name && strcmp(scope->variables[j]->name, name) == 0) {
				if (captured) *captured = passed_function && scope->token->type == FDEC;
				return scope->variables[j];
			}
		if (scope->token->type == FDEC) passed_function = true;
	}
	if (captured) *captured = false;
	return NULL;
}

void declare_function(Node *fn) {
	for (int i = 0; i < ura.scope->functions_count; i++)
		if (strcmp(ura.scope->functions[i]->token->name, fn->token->name) == 0) {
			parse_error(fn->token, "Redeclaration of function '%s'", fn->token->name);
			return;
		}
	resize_array(ura.scope->functions, Node *);
	ura.scope->functions[ura.scope->functions_count++] = fn;
}

Node *find_function(char *name) {
	for (int i = ura.scopes_count; i >= 0; i--) {
		Node *scope = ura.scopes[i];
		if (!scope) continue;
		for (int j = 0; j < scope->functions_count; j++)
			if (scope->functions[j]->token->name && strcmp(scope->functions[j]->token->name, name) == 0)
				return scope->functions[j];
	}
	return NULL;
}

void analyze_binop(Node *node) {
   analyze(node->left);
   analyze(node->right);
}

void analyze_fdec(Node *node) {
   Token *token = node->token;
   enter_scope(node);
   for (int i = 0; i < token->Fn.params_count; i++)
      declare_variable(token->Fn.params[i]);
   for (int i = 0; i < node->children_count; i++)
      if (node->children[i]->token->type == FDEC)
         declare_function(node->children[i]);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   exit_scope();
}

void analyze_id(Node *node) {
   Token *token = node->token;
   if (token->is_dec) { declare_variable(token); return; }
   bool   captured = false;
   Token *decl     = find_variable(token->name, &captured);
   if (decl) {
      if (captured) {
         parse_error(token, "Cannot use '%s' from an enclosing function - pass it as a parameter", token->name);
         return;
      }
      token->ret_type = decl->ret_type;
      if (decl->ret_type == FN_TYPE) token->Fn = decl->Fn;
      if (decl->ret_type == ARRAY_TYPE) token->Array = decl->Array;
      return;
   }
   Node *fn = find_function(token->name);
   if (!fn) { parse_error(token, "Undeclared variable '%s'", token->name); return; }
   token->type             = FN_TYPE;
   token->ret_type         = FN_TYPE;
   token->Fcall.ptr        = fn;
   token->Fn.params        = fn->token->Fn.params;
   token->Fn.params_count  = fn->token->Fn.params_count;
   token->Fn.ret           = new_token(ID, 0);
   token->Fn.ret->ret_type = fn->token->ret_type;
}

void analyze_fcall(Node *node) {
   Token *token = node->token;
   Token *var = find_variable(token->name, NULL);
   if (var && var->ret_type == FN_TYPE) {
      token->Fcall.var = var;
      for (int i = 0; i < node->children_count; i++)
         analyze(node->children[i]);
      token->ret_type = var->Fn.ret->ret_type;
      return;
   }
   Node *fn = find_function(token->name);
   if (!fn) { parse_error(token, "Undeclared function '%s'", token->name); return; }
   token->Fcall.ptr = fn;
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   token->ret_type = fn->token->ret_type;
}

void analyze_for(Node *node) {
   enter_scope(node);
   analyze(node->right);
   Token *var = node->left->token;
   Token *it  = node->right->token;
   if (it->type == RANGE)
      var->ret_type = INT;
   else if (it->ret_type == ARRAY_TYPE) {
      var->Array       = it->Array;
      var->Array.depth = it->Array.depth - 1;
      var->ret_type    = it->Array.depth > 1 ? ARRAY_TYPE : it->Array.sub_type;
   }
   declare_variable(var);
   for (int i = 0; i < node->children_count; i++)
      analyze(node->children[i]);
   exit_scope();
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
      case IF: case ELIF: case ELSE: case WHILE: case LOOP:
         analyze_block(node);
         break;
      case FOR: analyze_for(node); break;
      case MATCH: analyze_match(node); break;
      case BREAK:
         node->left = enclosing_break();
         if (!node->left)
            parse_error(node->token, "'break' outside a loop or match");
         break;
      case CONTINUE:
         node->left = enclosing_continue();
         if (!node->left)
            parse_error(node->token, "'continue' outside a loop");
         break;
      case OUTPUT:
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         break;
      case ARRAY_LIT:
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         break;
      case ACCESS: analyze_binop(node); break;
      case RANGE:  analyze_binop(node); break;
      case DOT:    analyze(node->left); break;
      case TYPEOF: case SIZEOF: analyze(node->left); break;
      case ARRAY:
         for (int i = 0; i < node->children_count; i++)
            analyze(node->children[i]);
         break;
      case CLEAN: analyze(node->left); break;
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
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
