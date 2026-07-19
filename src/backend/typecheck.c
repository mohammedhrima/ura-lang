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

bool is_float(Type type) {
	return includes(type, FLOAT_TYPES, 0);
}

bool is_castable(Type type) {
	return is_float(type) || includes(type, NUMERIC_TYPES, 0);
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
   case DOUBLE: return ura.f64;
   case VOID:  return ura.vd;
   default: TODO(1, "to_llvm_type: unhandled type %t", type); return NULL;
   }
}

TypeRef elem_type(Token *arr, int depth) {
   if (depth > 1) return array_type(arr, depth - 1);
   if (arr->Array.sub_type == STRUCT_CALL)
      return struct_type_of(arr->Array.struct_ptr);
   return to_llvm_type(arr->Array.sub_type);
}

TypeRef array_type(Token *arr, int depth) {
   TypeRef fields[2] = { pointer_to(elem_type(arr, depth)), ura.i64 };
   return LLVMStructTypeInContext(ura.context, fields, 2, 0);
}

TypeRef struct_type_of(Node *def) {
   if (!def) return NULL;
   Token *token = def->token;
   if (token->llvm.struct_type) return token->llvm.struct_type;
   TypeRef type = LLVMStructCreateNamed(ura.context, token->name);
   token->llvm.struct_type = type;
   TypeRef *fields = allocate(def->children_count, sizeof(TypeRef));
   int      n      = 0;
   for (int i = 0; i < def->children_count; i++) {
      Node  *child = def->children[i];
      Token *field = child->token;
      if (field->type == STRUCT_DEF) { struct_type_of(child); continue; }
      if (!is_field(field)) continue;
      TypeRef t = llvm_type_of(field);
      fields[field->Struct.index] = field->is_ref ? pointer_to(t) : t;
      n++;
   }
   LLVMStructSetBody(type, fields, n, 0);
   free(fields);
   return type;
}

TypeRef llvm_type_of(Token *token) {
   if (token->ret_type == STRUCT_CALL)
      return struct_type_of(token->Struct.ptr);
   if (token->ret_type == ARRAY_TYPE)
      return array_type(token, token->Array.depth);
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
   if (is_float(token->ret_type))
      return LLVMConstReal(llvm_type_of(token), 0.0);
   return LLVMConstNull(llvm_type_of(token));
}

void type_check_method_call(Node *node) {
   Token *token = node->token;
   type_check(node->left);
   Token *recv = node->left->token;
   if (recv->ret_type != STRUCT_CALL || !recv->Struct.ptr) {
      parse_error(token, ERR_METHOD_ON_NON_STRUCT, token->name, type_name(recv->ret_type));
      return;
   }
   Node *def       = recv->Struct.ptr;
   char *qualified = format("%s.%s", def->token->name, token->name);
   Node *fn        = find_method(def, qualified);
   free(qualified);
   if (!fn) {
      parse_error(token, ERR_UNKNOWN_METHOD, def->token->name, token->name);
      return;
   }
   if (def->token->has_drop && fn == find_destructor(def)) {
      parse_error(token, ERR_CANNOT_CALL_DROP, def->token->name);
      return;
   }
   token->Fcall.ptr = fn;
   type_check_fcall(node);
   if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
}

void type_check_static_call(Node *node) {
   Token *token = node->token;
   Node  *def   = find_struct(token->Struct.name);
   if (!def) {
      parse_error(token, ERR_UNKNOWN_TYPE, token->Struct.name);
      return;
   }
   char *qualified = format("%s.%s", def->token->name, token->name);
   Node *fn        = find_method(def, qualified);
   free(qualified);
   if (!fn) {
      parse_error(token, ERR_UNKNOWN_METHOD, def->token->name, token->name);
      return;
   }
   if (!fn->token->is_pub) {
      parse_error(token, ERR_METHOD_NEEDS_RECEIVER, def->token->name, token->name);
      return;
   }
   token->Fcall.ptr = fn;
   type_check_fcall(node);
   if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
}

void type_check_fcall(Node *node) {
   Token *token = node->token;
   if (token->is_method_call && !token->Fcall.ptr) {
      type_check_method_call(node);
      return;
   }
   if (token->is_static_call && !token->Fcall.ptr) {
      type_check_static_call(node);
      return;
   }
   bool indirect = token->Fcall.var != NULL;
   if (!indirect && !token->Fcall.ptr) return;
   Token *fn = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   int  self      = token->is_method_call ? 1 : 0;
   int  wanted    = fn->Fn.params_count - self;
   bool bad_count = fn->is_variadic ? node->children_count < wanted
                                    : node->children_count != wanted;
   if (bad_count) {
      parse_error(token, ERR_WRONG_ARG_COUNT, token->name);
      return;
   }
   for (int i = 0; i < wanted; i++) {
      Token *param    = fn->Fn.params[i + self];
      adopt_float(node->children[i], param->ret_type);
      Type arg_type   = node->children[i]->token->ret_type;
      Type param_type = param->ret_type;
      if (arg_type && arg_type != param_type) {
         parse_error(node->children[i]->token, ERR_ARG_TYPE_MISMATCH, i + 1, token->name);
         return;
      }
      bool want_ref = param->is_ref;
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

void type_check_return(Node *node) {
   type_check(node->left);
   if (!ura.fn_ret || !node->left) return;
   Type wanted = ura.fn_ret->ret_type;
   adopt_float(node->left, wanted);
   Type got = node->left->token->ret_type;
   if (got && got != wanted)
      parse_error(node->token, ERR_RETURN_TYPE_MISMATCH, ura.fn_ret->name, type_name(wanted), type_name(got));
}

void type_check_fdec(Node *node) {
   Token *prev = ura.fn_ret;
   ura.fn_ret  = node->token;
   for (int i = 0; i < node->children_count; i++)
      type_check(node->children[i]);
   ura.fn_ret = prev;
}

bool struct_contains(Node *def, Node *target, int depth) {
   if (!def || depth > 64) return false;
   for (int i = 0; i < def->children_count; i++) {
      Token *field = def->children[i]->token;
      if (!is_field(field) || field->is_ref) continue;
      if (field->ret_type != STRUCT_CALL) continue;
      Node *sub = field->Struct.ptr;
      if (!sub) continue;
      if (sub == target) return true;
      if (struct_contains(sub, target, depth + 1)) return true;
   }
   return false;
}

void type_check_struct(Node *node) {
   node->token->ret_type = STRUCT_DEF;
   int n = 0;
   for (int i = 0; i < node->children_count; i++) {
      Node *child = node->children[i];
      type_check(child);
      if (is_field(child->token))
         child->token->Struct.index = n++;
   }
   if (struct_contains(node, node, 0))
      parse_error(node->token, ERR_STRUCT_RECURSIVE, node->token->name);
}

bool is_field(Token *token) {
   return token->name && token->type != STRUCT_DEF && token->type != FDEC;
}

Token *find_field(Node *def, char *name) {
   if (!def) return NULL;
   for (int i = 0; i < def->children_count; i++) {
      Token *field = def->children[i]->token;
      if (!is_field(field)) continue;
      if (strcmp(field->name, name) == 0) return field;
   }
   return NULL;
}

void type_check_dot(Node *node) {
   type_check(node->left);
   Token *token = node->token;
   Token *left  = node->left->token;
   if (left->ret_type == ARRAY_TYPE) {
      if (strcmp(token->name, "len") != 0)
         parse_error(token, ERR_UNKNOWN_MEMBER, token->name);
      else
         token->ret_type = INT;
      return;
   }
   if (left->ret_type != STRUCT_CALL) {
      char *tname = type_name(left->ret_type);
      if (strcmp(token->name, "len") == 0)
         parse_error(token, ERR_LEN_NOT_ARRAY, tname);
      else
         parse_error(token, ERR_TYPE_HAS_NO_FIELDS, token->name, tname);
      return;
   }
   Node  *def   = left->Struct.ptr;
   Token *field = find_field(def, token->name);
   if (!field) {
      if (!def) return;
      parse_error(token, ERR_UNKNOWN_FIELD, def->token->name, token->name);
      return;
   }
   token->ret_type = field->ret_type;
   token->is_ref   = field->is_ref;
   token->Struct   = field->Struct;
   token->Array    = field->Array;
}

char *struct_name_of(Token *token) {
   if (token->ret_type == STRUCT_CALL && token->Struct.name)
      return token->Struct.name;
   return type_name(token->ret_type);
}

Node *find_destructor(Node *def) {
   char *qualified = format("%s.drop", def->token->name);
   Node *fn        = find_method(def, qualified);
   free(qualified);
   return fn;
}

bool find_operator(Node *node) {
   Token *token = node->token;
   Node  *def   = node->left->token->Struct.ptr;
   if (!def) return false;
   char *qualified = format("%s.%s.%s", def->token->name, spell(token->type), struct_name_of(node->right->token));
   Node *fn = find_method(def, qualified);
   free(qualified);
   if (!fn) return false;
   token->Fcall.ptr = fn;
   token->ret_type  = fn->token->ret_type;
   if (token->ret_type == STRUCT_CALL) token->Struct = fn->token->Struct;
   return true;
}

bool is_float_literal(Node *node) {
   if (!node) return false;
   if (node->token->type == FLOAT) return true;
   if (!includes(node->token->type, MATH_TYPE, 0)) return false;
   if (!is_float(node->token->ret_type)) return false;
   return is_float_literal(node->left) && is_float_literal(node->right);
}

void adopt_float(Node *node, Type target) {
   if (!is_float(target) || !is_float_literal(node)) return;
   node->token->ret_type = target;
   adopt_float(node->left, target);
   adopt_float(node->right, target);
}

void type_check_binop(Node *node) {
   Token *token = node->token;
   type_check(node->left);
   type_check(node->right);
   adopt_float(node->left, node->right->token->ret_type);
   adopt_float(node->right, node->left->token->ret_type);
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
   if (lt == STRUCT_CALL) {
      bool assign = token->type == ASSIGN;
      if (!(assign && node->left->token->is_dec) && find_operator(node)) return;
      if (!assign) {
         parse_error(token, ERR_NO_OPERATOR, struct_name_of(node->left->token),
                     spell(token->type), struct_name_of(node->right->token));
         return;
      }
      Node *lhs_def = node->left->token->Struct.ptr;
      Node *rhs_def = node->right->token->Struct.ptr;
      if (rt == STRUCT_CALL && lhs_def && rhs_def && lhs_def != rhs_def) {
         parse_error(token, ERR_ARG_TYPE_MISMATCH_STRUCT, struct_name_of(node->right->token),
                     struct_name_of(node->left->token));
         return;
      }
   }
   if (lt && rt && lt != rt) {
      if (token->type == ASSIGN)
         parse_error(token, ERR_CANNOT_ASSIGN, struct_name_of(node->right->token),
                     struct_name_of(node->left->token));
      else
         parse_error(token, ERR_BINOP_TYPE_MISMATCH, spell(token->type),
                     struct_name_of(node->left->token),
                     struct_name_of(node->right->token));
      return;
   }
   if (is_float(lt) && includes(token->type, BITWISE_TYPE, 0)) {
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
      node->token->ret_type = ARRAY_TYPE;
      node->token->Array    = arr->Array;
      return;
   }
   if (!includes(node->right->token->ret_type, NUMERIC_TYPES, 0))
      parse_error(node->token, ERR_INDEX_NOT_INT, type_name(node->right->token->ret_type));
   node->token->Array       = arr->Array;
   node->token->Array.depth = arr->Array.depth - 1;
   bool elem = node->token->Array.depth == 0;
   node->token->ret_type = elem ? arr->Array.sub_type : ARRAY_TYPE;
   if (elem && arr->Array.sub_type == STRUCT_CALL)
      node->token->Struct.ptr = arr->Array.struct_ptr;
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
   if (iter->type == RANGE && node->right->children_count) {
      Node  *by = node->right->children[0];
      type_check(by);
      Token *bt = by->token;
      if (!includes(bt->ret_type, NUMERIC_TYPES, 0))
         parse_error(bt, ERR_BY_NOT_INT, type_name(bt->ret_type));
      else if (bt->type == INT && bt->Int.value <= 0)
         parse_error(bt, ERR_BY_NOT_POSITIVE);
   }
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
      case RETURN:  type_check_return(node); break;
      case FCALL:   type_check_fcall(node); break;
      case OUTPUT: {
         for (int i = 0; i < node->children_count; i++)
         type_check(node->children[i]);
         token->ret_type = VOID;
         break;
      }
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
         bool bad_src = src && !is_castable(src);
         bool bad_dst = !is_castable(dst);
         if (bad_src || bad_dst)
            parse_error(token, ERR_CANNOT_CAST, type_name(src), type_name(dst));
         token->ret_type = dst;
         break;
      }
      case REF: {
         type_check(node->left);
         token->ret_type = node->left->token->ret_type;
         break;
      }
      case IF: case ELIF: case ELSE: case WHILE: case LOOP: {
         type_check_block(node);
         break;
      }
      case FOR: type_check_for(node); break;
      case MATCH: type_check_match(node); break;
      case ARRAY_LIT: type_check_array_lit(node); break;
      case ACCESS:    type_check_access(node); break;
      case DOT: type_check_dot(node); break;
      case RANGE: {
         type_check(node->left);
         type_check(node->right);
         bool left_num  = includes(node->left->token->ret_type, NUMERIC_TYPES, 0);
         bool right_num = includes(node->right->token->ret_type, NUMERIC_TYPES, 0);
         if (!left_num || !right_num)
         parse_error(node->token, "Range bounds must be integers");
         node->token->ret_type = INT;
         break;
      }
      case ARRAY:     type_check_array_ctor(node); break;
      case TYPEOF: case SIZEOF:
         type_check(node->left);
         node->token->ret_type = token->type == TYPEOF ? CHARS : INT;
         break;
      case CLEAN: {
         type_check(node->left);
         Token *target = node->left->token;
         node->token->ret_type = VOID;
         if (target->ret_type == ARRAY_TYPE) break;
         char *name = struct_name_of(target);
         if (target->ret_type == STRUCT_CALL)
            parse_error(node->token, ERR_CLEAN_ON_STRUCT, name);
         else
            parse_error(node->token, ERR_CLEAN_NEEDS_ARRAY, name);
         break;
      }
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
