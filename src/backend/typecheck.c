#include "header.h"

void type_check_block(Node *node) {
	type_check(node->left);
	if (node->left && node->left->token->ret_type != BOOL)
		parse_error(node->token, "The '%s' condition must be a bool, got %s",
		            node->token->name, type_name(node->left->token->ret_type));
	for (int i = 0; i < node->children_count; i++)
		type_check(node->children[i]);
	type_check(node->right);
}

void type_check_match(Node *node) {
	type_check(node->left);
	Type subject = node->left->token->ret_type;
	for (int i = 0; i < node->children_count; i++) {
		Node *branch = node->children[i];
		if (branch->left)
			for (int j = 0; j < branch->left->children_count; j++) {
				Node *value = branch->left->children[j];
				type_check(value);
				Type ret_type = value->token->ret_type;
				if (subject && ret_type && ret_type != subject)
					parse_error(value->token, ERR_CASE_TYPE_MISMATCH, type_name(ret_type), type_name(subject));
			}
		for (int j = 0; j < branch->children_count; j++)
			type_check(branch->children[j]);
	}
}

bool is_data_type(Token *token) {
	return token->is_dec && includes(token->type, DATA_TYPES, 0); 
}

TypeRef to_llvm_type(Type type) {
   switch (type) {
   case INT:   return ura.i32;
   case LONG:  return ura.i64;
   case SHORT: return ura.i16;
   case CHAR:  return ura.i8;
   case BOOL:  return ura.i1;
   case CHARS: return LLVMPointerType(ura.i8, 0);
   case FLOAT: return ura.f32;
   case VOID:  return ura.vd;
   default: TODO(1, "to_llvm_type: unhandled type %t", type); return NULL;
   }
}

TypeRef array_type(Type sub, int depth) {
   TypeRef elem      = depth <= 1 ? to_llvm_type(sub) : array_type(sub, depth - 1);
   TypeRef fields[2] = { LLVMPointerType(elem, 0), ura.i64 };
   return LLVMStructTypeInContext(ura.context, fields, 2, 0);
}

TypeRef llvm_type_of(Token *token) {
   if (token->ret_type == ARRAY_TYPE) 
      return array_type(token->Array.sub_type, token->Array.depth);
   if (token->ret_type != FN_TYPE) 
      return to_llvm_type(token->ret_type);
   int      n      = token->Fn.params_count;
   TypeRef *params = n ? allocate(n, sizeof(TypeRef)) : NULL;
   for (int i = 0; i < n; i++) params[i] = llvm_type_of(token->Fn.params[i]);
   TypeRef ft = LLVMFunctionType(llvm_type_of(token->Fn.ret), params, n, 0);
   free(params);
   return LLVMPointerType(ft, 0);
}

Value default_value(Token *token) {
   if (includes(token->ret_type, NUMERIC_TYPES, 0)) 
      return LLVMConstInt(llvm_type_of(token), 0, false);
   if (token->ret_type == FLOAT)                    
      return LLVMConstReal(llvm_type_of(token), 0.0);
   return LLVMConstNull(llvm_type_of(token));
}

void type_check_fcall(Node *node) {
   Token *token = node->token;
   bool indirect = token->Fcall.var != NULL;
   if (!indirect && !token->Fcall.ptr) return;
   Token *fn = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   bool bad_count = fn->is_variadic ? node->children_count < fn->Fn.params_count
                                    : node->children_count != fn->Fn.params_count;
   if (bad_count) {
      parse_error(token, ERR_WRONG_ARG_COUNT, token->name);
      return;
   }
   for (int i = 0; i < fn->Fn.params_count; i++) {
      Type arg_type   = node->children[i]->token->ret_type;
      Type param_type = fn->Fn.params[i]->ret_type;
      if (arg_type && arg_type != param_type) {
         parse_error(node->children[i]->token, ERR_ARG_TYPE_MISMATCH, i + 1, token->name);
         return;
      }
      bool want_ref = fn->Fn.params[i]->is_ref;
      bool got_ref  = node->children[i]->token->type == REF;
      if (want_ref && !got_ref) {
         parse_error(node->children[i]->token, ERR_ARG_NEEDS_REF, i + 1, token->name);
         return;
      }
      if (!want_ref && got_ref) {
         parse_error(node->children[i]->token, ERR_ARG_NO_REF, i + 1, token->name);
         return;
      }
   }
   token->ret_type = indirect ? fn->Fn.ret->ret_type : fn->ret_type;
}

void type_check_fdec(Node *node) {
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
}

void type_check_struct(Node *node) {
   node->token->ret_type = STRUCT_DEF;
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
}

void type_check_binop(Node *node) {
   Token *token = node->token;
   type_check(node->left);
   type_check(node->right);
   Type lt = node->left->token->ret_type;
   Type rt = node->right->token->ret_type;
   if (node->left->token->is_dec && node->left->token->is_ref) {
      if (node->right->token->type != REF)
         parse_error(token, ERR_REF_NEEDS_VARIABLE);
      else if (lt && rt && lt != rt)
         parse_error(token, ERR_REF_TYPE_MISMATCH, type_name(lt), type_name(rt));
      token->ret_type = lt;
      return;
   }
   if (lt && rt && lt != rt) {
      parse_error(token, "Type mismatch between operands");
      return;
   }
   if (lt == FLOAT && includes(token->type, BITWISE_TYPE, 0)) {
      parse_error(token, ERR_BITWISE_NEEDS_INT);
      return;
   }
   if (includes(token->type, LOGIC_TYPE, 0)) {
      if (lt != BOOL || rt != BOOL) {
         parse_error(token, "Operators 'and'/'or' need bool operands");
         return;
      }
      token->ret_type = BOOL;
   } else
      token->ret_type = includes(token->type, COMPARISON_OPS, 0) ? BOOL : lt;
}

void type_check_array_lit(Node *node) {
   if (node->children_count == 0) {
      parse_error(node->token, "Empty array literal has no element type");
      return;
   }
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   Token *first = node->children[0]->token;
   for (int i = 1; i < node->children_count; i++) {
      Token *elem     = node->children[i]->token;
      bool   is_array = first->ret_type == ARRAY_TYPE;
      bool   same_type  = elem->ret_type == first->ret_type;
      bool   same_sub   = elem->Array.sub_type == first->Array.sub_type;
      bool   same_depth = elem->Array.depth == first->Array.depth;
      bool   same_array = !is_array || (same_sub && same_depth);
      if (!same_type || !same_array)
         parse_error(elem, "Array elements must all be the same type");
   }
   node->token->ret_type = ARRAY_TYPE;
   if (first->ret_type == ARRAY_TYPE) {
      node->token->Array.sub_type = first->Array.sub_type;
      node->token->Array.depth    = first->Array.depth + 1;
   } else {
      node->token->Array.sub_type = first->ret_type;
      node->token->Array.depth    = 1;
   }
}

void type_check_access(Node *node) {
   type_check(node->left);
   type_check(node->right);
   Token *arr = node->left->token;
   if (arr->ret_type != ARRAY_TYPE) {
      parse_error(node->token, ERR_NOT_AN_ARRAY, type_name(arr->ret_type));
      return;
   }
   if (node->right->token->type == RANGE) {
      node->token->ret_type       = ARRAY_TYPE;
      node->token->Array.sub_type = arr->Array.sub_type;
      node->token->Array.depth    = arr->Array.depth;
      return;
   }
   if (!includes(node->right->token->ret_type, NUMERIC_TYPES, 0))
      parse_error(node->token, ERR_INDEX_NOT_INT, type_name(node->right->token->ret_type));
   node->token->Array.sub_type = arr->Array.sub_type;
   node->token->Array.depth    = arr->Array.depth - 1;
   node->token->ret_type       = node->token->Array.depth > 0 ? ARRAY_TYPE : arr->Array.sub_type;
}

void type_check_array_ctor(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      type_check(node->children[i]);
      if (!includes(node->children[i]->token->ret_type, NUMERIC_TYPES, 0))
         parse_error(node->children[i]->token, ERR_ARRAY_SIZE_NOT_INT);
   }
}

void type_check_for(Node *node) {
   type_check(node->right);
   Token *iter = node->right->token;
   if (iter->type != RANGE && iter->ret_type != ARRAY_TYPE)
      parse_error(node->token, ERR_FOR_NOT_ITERABLE, node->left->token->name);
   if (node->token->is_ref && iter->type == RANGE)
      parse_error(node->token, ERR_FOR_REF_NEEDS_ARRAY);
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
}

void type_check(Node *node) {
   if (!node) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:   type_check_fdec(node); break;
      case STRUCT_DEF: type_check_struct(node); break;
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
         bool bad_src = src && !includes(src, NUMERIC_TYPES, 0);
         bool bad_dst = !includes(dst, NUMERIC_TYPES, 0);
         if (bad_src || bad_dst)
            parse_error(token, ERR_CANNOT_CAST, type_name(src), type_name(dst));
         token->ret_type = dst;
         break;
      }
      case REF:
         type_check(node->left);
         token->ret_type = node->left->token->ret_type;
         break;
      case IF: case ELIF: case ELSE: case WHILE: case LOOP:
         type_check_block(node);
         break;
      case FOR: type_check_for(node); break;
      case MATCH: type_check_match(node); break;
      case ARRAY_LIT: type_check_array_lit(node); break;
      case ACCESS:    type_check_access(node); break;
      case DOT:
         type_check(node->left);
         if (strcmp(node->token->name, "len") != 0)
            parse_error(node->token, ERR_UNKNOWN_MEMBER, node->token->name);
         else if (node->left->token->ret_type != ARRAY_TYPE)
            parse_error(node->token, ERR_LEN_NOT_ARRAY, type_name(node->left->token->ret_type));
         else
            node->token->ret_type = INT;
         break;
      case RANGE:
         type_check(node->left);
         type_check(node->right);
         bool left_num  = includes(node->left->token->ret_type, NUMERIC_TYPES, 0);
         bool right_num = includes(node->right->token->ret_type, NUMERIC_TYPES, 0);
         if (!left_num || !right_num)
            parse_error(node->token, "Range bounds must be integers");
         node->token->ret_type = INT;
         break;
      case ARRAY:     type_check_array_ctor(node); break;
      case TYPEOF: case SIZEOF:
         type_check(node->left);
         node->token->ret_type = token->type == TYPEOF ? CHARS : INT;
         break;
      case CLEAN:
         type_check(node->left);
         if (node->left->token->ret_type != ARRAY_TYPE)
            parse_error(node->token, "'clean' expects an array");
         node->token->ret_type = VOID;
         break;
      case BREAK: case CONTINUE: break;
      case ASSIGN: case ADD: case SUB: case MUL: case DIV: case MOD:
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
         type_check_binop(node); break;
      default:
         CHECK(1, ASSERT_TYPECHECK_NODE, to_string(token->type));
         break;
   }
}
