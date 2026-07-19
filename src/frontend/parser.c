#include "header.h"

void set_name(Token *token, char *name) {
	if (token->name) free(token->name);
	token->name = name ? strdup(name) : NULL;
}

Token *next() {
	Token *token = peek(0);
	if (ura.exe_pos + 1 < ura.tokens_count) ura.exe_pos++;
	return token;
}

Token *peek(int index) {
	int pos = ura.exe_pos + index;
	if (pos >= ura.tokens_count) pos = ura.tokens_count - 1;
	return ura.tokens[pos];
}

Token *find(Type type, ...) {
	va_list ap;
	va_start(ap, type);
	while (type && ura.tokens[ura.exe_pos]) {
		if (type == ura.tokens[ura.exe_pos]->type) return ura.tokens[ura.exe_pos++];
		type = va_arg(ap, Type);
	}
	return NULL;
}

int get_operation_precedence(Type type)
{
   static const int res[END + 1] = {
      [ASSIGN] = 1,     [ADD_ASSIGN] = 1, [SUB_ASSIGN] = 1,
      [MUL_ASSIGN] = 1, [DIV_ASSIGN] = 1, [MOD_ASSIGN] = 1,
      [OR] = 2,         [AND] = 3,        [RANGE] = 2,
      [BOR] = 4,        [BXOR] = 5,       [BAND] = 6,
      [LESS] = 8,       [EQUAL] = 7,      [NOT_EQUAL] = 7,
      [GREAT] = 8,      [LESS_EQUAL] = 8, [GREAT_EQUAL] = 8,
      [ADD] = 10,       [LSHIFT] = 9,     [RSHIFT] = 9,
      [SUB] = 10,       [MUL] = 11,       [DIV] = 11,
      [AS] = 12,        [MOD] = 11,
   };

   return res[type];
}

bool within(int indent) {
	Token *curr = ura.tokens[ura.exe_pos];
	return !ura.found_error && curr->indent > indent && curr->type != END;
}

void parser_recover(int indent) {
	int start_line = ura.exe_pos < ura.tokens_count ? ura.tokens[ura.exe_pos]->line : -1;
	while (ura.exe_pos < ura.tokens_count && ura.tokens[ura.exe_pos]->type != END) {
		Token *t = ura.tokens[ura.exe_pos];
		if (t->indent <= indent && t->line != start_line) break;
		ura.exe_pos++;
	}
	ura.found_error = false;
}

Node *new_node(Token *token) {
	debug("new node: %k\n", token);
	Node *new  = allocate(1, sizeof(Node));
	new->token = token;
	return new;
}

Node *syntax_error() {
	 return new_node(new_token(SYNTAX_ERROR, -1));
}

void parse_type(Token *target) {
	if (peek(0)->type == FDEC) {
		next();
		target->ret_type = FN_TYPE;
		if (!find(LPAR, 0)) {
			parse_error(peek(0), "Expected '(' in function type");
			return;
		}
		while (!ura.found_error && peek(0)->type != RPAR) {
			Token *param = new_token(ID, 0);
			parse_type(param);
			resize_array(target->Fn.params, Token *);
			target->Fn.params[target->Fn.params_count++] = param;
			while (find(COMA, 0));
		}
		if (!find(RPAR, 0))
			parse_error(peek(0), "Expected ')' in function type");
		target->Fn.ret = new_token(ID, 0);
		parse_type(target->Fn.ret);
	} else if (is_data_type(peek(0)) || peek(0)->type == ID) {
		Token *base = next();
		if (base->type == ID) {
			target->ret_type    = STRUCT_CALL;
			target->Struct.name = base->name;
		} else
			target->ret_type = base->type;
		int depth = 0;
		while (peek(0)->type == LBRA && peek(1)->type == RBRA) {
			find(LBRA, 0);
			find(RBRA, 0);
			depth++;
		}
		if (depth > 0) {
			target->Array.sub_type = target->ret_type;
			target->Array.depth    = depth;
			target->ret_type       = ARRAY_TYPE;
		}
	} else {
		parse_error(peek(0), "Expected a type");
	}
}

void parse_block(Node *node, int indent) {
	while (within(indent)) {
		resize_array(node->children, Node*);
		node->children[node->children_count++] = expr_node(0);
	}
}

Node *match_node(Node *node) {
	Token *token = node->token;
	node->left = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'match' body");
	while (within(token->indent) && includes(peek(0)->type, CASE, DEFAULT, 0)) {
		Token *keyword = next();
		Node  *branch  = new_node(keyword);
		if (keyword->type == CASE) {
			Node *values = new_node(keyword);
			while (!ura.found_error && peek(0)->type != DOTS) {
				resize_array(values->children, Node *);
				values->children[values->children_count++] = expr_node(0);
				if (!find(COMA, 0)) break;
			}
			if (values->children_count == 0)
				parse_error(keyword, "Expected an expression after 'case'");
			branch->left = values;
		}
		if (!find(DOTS, 0))
			parse_error(keyword, ERR_EXPECTED_BODY_COLON, keyword->name);
		parse_block(branch, keyword->indent);
		resize_array(node->children, Node *);
		node->children[node->children_count++] = branch;
		if (keyword->type == DEFAULT) break;
	}
	return node;
}

Node *if_node(Node *node) {
	Token *token = node->token;
	node->left = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'if' body");
	parse_block(node, token->indent);
	Node *tail = node;
	while (includes(peek(0)->type, ELIF, ELSE, 0) && peek(0)->indent == token->indent) {
		Token *keyword = next();
		Node  *branch  = new_node(keyword);
		if (keyword->type == ELIF)
			branch->left = expr_node(0);
		if (!find(DOTS, 0))
			parse_error(keyword, ERR_EXPECTED_BODY_COLON, keyword->name);
		parse_block(branch, keyword->indent);
		tail->right = branch;
		tail        = branch;
		if (keyword->type == ELSE) break;
	}
	return node;
}

Node *while_node(Node *node) {
	Token *token = node->token;
	node->left = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'while' body");
	parse_block(node, token->indent);
	return node;
}

Node *for_node(Node *node) {
	Token *token = node->token;
	bool   ref   = find(REF, 0) != NULL;
	Token *iter  = find(ID, 0);
	if (!iter) {
		parse_error(token, "Expected a loop variable after 'for'");
		return syntax_error();
	}
	iter->is_dec  = true;
	token->is_ref = ref;
	node->left    = new_node(iter);
	if (!find(IN, 0))
		parse_error(token, "Expected 'in' after 'for %s'", iter->name);
	node->right = expr_node(0);
	if (!find(DOTS, 0))
		parse_error(token, "Expected ':' to open the 'for' body");
	parse_block(node, token->indent);
	return node;
}

Node *ref_node(Node *node) {
	Token *token = node->token;
	bool nullable = peek(0)->type == OPTIONAL;
	if (nullable) find(OPTIONAL, 0);
	Token *name = peek(0);
	if (!name || name->type != ID) {
		parse_error(token, "Expected a variable after 'ref'");
		return syntax_error();
	}
	bool fn_type = peek(1)->type == FDEC && peek(2)->type == LPAR;
	bool named   = peek(1)->type == ID && peek(1)->line == name->line;
	if (is_data_type(peek(1)) || named || fn_type) {
		find(ID, 0);
		name->is_dec      = true;
		name->is_ref      = true;
		name->is_nullable = nullable;
		parse_type(name);
		if (!nullable && peek(0)->type != ASSIGN) {
			parse_error(name, ERR_REF_MUST_BE_BOUND);
			return syntax_error();
		}
		node->token = name;
		return node;
	}
	node->left = prime_node();
	return node;
}

Node *id_node(Node *node) {
	Token *token = node->token;
	if (peek(0)->type == LPAR) {
		if (strcmp(token->name, "main") == 0)
			return fdec_node(node);
		if (strcmp(token->name, "output") == 0)
			return output_node(node);
		return fcall_node(node);
	}
	if (peek(0)->type == LBRA || peek(0)->type == DOT)
		return access_node(node);
	bool named   = peek(0)->type == ID && peek(0)->line == token->line;
	bool fn_type = peek(0)->type == FDEC && peek(1)->type == LPAR;
	if (is_data_type(peek(0)) || named || fn_type) {
		token->is_dec = true;
		parse_type(token);
	}
	if (!token->is_dec && peek(0)->type == OPTIONAL) {
		find(OPTIONAL, 0);
		token->is_nullable = true;
	}
	return node;
}

Node *fdec_node(Node *node) {
	node->token->type = FDEC;
	Node *owner  = ura.scope;
	bool  method = owner && owner->token->type == STRUCT_DEF;
	enter_scope(node);
	if (!find(LPAR, 0))
		parse_error(node->token, ERR_FN_EXPECTED_LPAREN, node->token->name);
	if (method && !node->token->is_pub) {
		Token *self = new_token(ID, 0);
		set_name(self, "self");
		self->ret_type    = STRUCT_CALL;
		self->Struct.name = owner->token->name;
		self->Struct.ptr  = owner;
		self->is_dec      = true;
		self->is_ref      = true;
		self->is_param    = true;
		resize_array(node->token->Fn.params, Token *);
		node->token->Fn.params[node->token->Fn.params_count++] = self;
	}
	while (!ura.found_error && peek(0)->type != RPAR) {
		if (find(VARIADIC, 0)) {
			node->token->is_variadic = true;
			break;
		}
		bool   is_ref = find(REF, 0) != NULL;
		Token *param  = find(ID, 0);
		if (!param) {
			parse_error(node->token, ERR_FN_EXPECTED_PARAM_NAME, node->token->name);
			break;
		}
		parse_type(param);
		param->is_param = true;
		param->is_dec   = true;
		param->is_ref   = is_ref;
		resize_array(node->token->Fn.params, Token *);
		node->token->Fn.params[node->token->Fn.params_count++] = param;
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, ERR_FN_EXPECTED_RPAREN, node->token->name);

	if(strcmp(node->token->name, "main") == 0) {
		node->token->ret_type = INT;
	}
	else if(is_data_type(peek(0))) {
		node->token->ret_type = next()->type;
	}
	else if(peek(0)->type == ID) {
		node->token->ret_type    = STRUCT_CALL;
		node->token->Struct.name = next()->name;
	}
	else
		parse_error(node->token, ERR_FN_EXPECTED_RET_TYPE, node->token->name);
	if (!node->token->is_proto) {
		if (!find(DOTS, 0))
			parse_error(node->token, ERR_FN_EXPECTED_COLON, node->token->name);

		parse_block(node, node->token->indent);
	}

	exit_scope();
	return node;
}

Node *struct_node(Node *node) {
	node->token->type = STRUCT_DEF;
	enter_scope(node);
	if (!find(DOTS, 0))
		parse_error(node->token, ERR_STRUCT_EXPECTED_COLON, node->token->name);
	parse_block(node, node->token->indent);
	if (!node->children_count)
		parse_error(node->token, ERR_STRUCT_EMPTY, node->token->name);
	for (int i = 0; i < node->children_count; i++) {
		Token *child = node->children[i]->token;
		if (child->type != FDEC) continue;
		char *qualified = format("%s.%s", node->token->name, child->name);
		set_name(child, qualified);
		free(qualified);
	}
	exit_scope();
	return node;
}

Node *fcall_node(Node *node) {
	node->token->type = FCALL;
	if (!find(LPAR, 0))
		parse_error(node->token, "Expected '(' after %s", node->token->name);
	while (!ura.found_error && peek(0)->type != RPAR) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, ERR_CALL_EXPECTED_RPAREN, node->token->name);
	return node;
}

Node *output_node(Node *node) {
	node->token->type     = OUTPUT;
	node->token->ret_type = VOID;
	if (!find(LPAR, 0))
		parse_error(node->token, "Expected '(' after output");
	while (!ura.found_error && peek(0)->type != RPAR) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RPAR, 0))
		parse_error(node->token, "Expected ')' after output arguments");
	return node;
}

Node *access_node(Node *node) {
	while (peek(0)->type == LBRA || peek(0)->type == DOT) {
		if (peek(0)->type == DOT) {
			Token *dot    = next();
			Token *member = find(ID, 0);
			if (!member) {
				parse_error(dot, "Expected a member name after '.'");
				return syntax_error();
			}
			Node *dnode = new_node(dot);
			dnode->left = node;
			set_name(dnode->token, member->name);
			if (peek(0)->type == LPAR) {
				dnode = fcall_node(dnode);
				dnode->token->is_method_call = true;
			}
			node = dnode;
			continue;
		}
		Token *bracket = next();
		bracket->type  = ACCESS;
		Node  *access  = new_node(bracket);
		access->left   = node;
		access->right  = expr_node(0);
		if (!find(RBRA, 0))
			parse_error(bracket, "Expected ']' after array index");
		if (find(OPTIONAL, 0))
			bracket->is_nullable = true;
		node = access;
	}
	return node;
}

Node *array_lit_node(Node *node) {
	node->token->type = ARRAY_LIT;
	while (!ura.found_error && peek(0)->type != RBRA) {
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		while (find(COMA, 0));
	}
	if (!find(RBRA, 0))
		parse_error(node->token, "Expected ']' to close array literal");
	return node;
}

Node *array_ctor_node(Node *node) {
	Token *token   = node->token;
	bool   is_name = token->type == ID;
	Type   sub     = is_name ? STRUCT_CALL : token->type;
	if (is_name) token->Struct.name = token->name;
	token->type           = ARRAY;
	token->is_dec         = false;
	token->ret_type       = ARRAY_TYPE;
	token->Array.sub_type = sub;
	int depth = 0;
	while (peek(0)->type == LBRA) {
		find(LBRA, 0);
		resize_array(node->children, Node *);
		node->children[node->children_count++] = expr_node(0);
		if (!find(RBRA, 0))
			parse_error(node->token, "Expected ']' after array size");
		depth++;
	}
	node->token->Array.depth = depth;
	return node;
}

Node *prime_node() {
   Token *token = next();
   switch(token->type)
   {
   case INT: case BOOL: case CHARS: case CHAR: case FLOAT:
   case LONG: case SHORT:
      if (token->is_dec && peek(0)->type == LBRA)
         return array_ctor_node(new_node(token));
      return new_node(token);
   case LBRA: return array_lit_node(new_node(token));
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
   case STRUCT_DEF: {
      Node *node = new_node(token);
      Token *sname = find(ID, 0);
      if (!sname) {
         parse_error(token, "Expected a struct name after 'struct'");
         return syntax_error();
      }
      set_name(node->token, sname->name);
      return struct_node(node);
   }
   case ID: return id_node(new_node(token));
   case NEW: {
      Token *type = next();
      bool   ok   = is_data_type(type) || type->type == ID;
      if (!ok || peek(0)->type != LBRA) {
         parse_error(token, ERR_NEW_EXPECTED_ARRAY_TYPE);
         return syntax_error();
      }
      Node *arr = array_ctor_node(new_node(type));
      arr->token->is_heap = true;
      return arr;
   }
   case CLEAN: {
      Node *clean = new_node(token);
      clean->left = expr_node(0);
      return clean;
   }
   case TYPEOF: case SIZEOF: {
      char *kw   = token->type == TYPEOF ? "typeof" : "sizeof";
      Node *node = new_node(token);
      if (!find(LPAR, 0))
         parse_error(token, "Expected '(' after '%s'", kw);
      node->left = expr_node(0);
      if (!find(RPAR, 0))
         parse_error(token, "Expected ')' to close '%s'", kw);
      return node;
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
   case REF: return ref_node(new_node(token));
   case IF: return if_node(new_node(token));
   case ELIF: case ELSE:
      parse_error(token, "'%s' without a matching 'if'", token->name);
      return syntax_error();
   case MATCH: return match_node(new_node(token));
   case CASE: case DEFAULT:
      parse_error(token, "'%s' without a matching 'match'", token->name);
      return syntax_error();
   case WHILE: return while_node(new_node(token));
   case LOOP: {
      Node *node = new_node(token);
      if (!find(DOTS, 0))
         parse_error(token, "Expected ':' to open the 'loop' body");
      parse_block(node, token->indent);
      return node;
   }
   case FOR: return for_node(new_node(token));
   case BREAK: case CONTINUE:
      return new_node(token);
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
