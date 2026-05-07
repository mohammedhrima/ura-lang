#include "header.h"

// ============================================================================
// INTERMEDIATE REPRESENTATION
// ============================================================================

void ir_id(Node *node) {
	Token *find = get_variable(node->token->name);
	if (find) node->token = find;
}

void ir_struct_call(Node *node) {
	Node *src               = get_struct(node->token->Struct.ptr->token->name);
	node->token->Struct.ptr = src;
	if (node->token->is_dec) {
		if (scopes_count == 1) node->token->is_global = true;
		else src->token->used++;
		new_variable(node->token);
	}
}

void ir_nullable(Node *node) {
	node->token->ret_type = CHARS;
	node->token->used++;
}

void ir_primitive(Node *node) {
	if (node->token->is_dec) {
		if (scopes_count == 1) node->token->is_global = true;
		new_variable(node->token);
	} else node->token->used++;
}

void ir_assign(Node *node) {
	gen_ir(node->left);
	if (found_error) return;
	gen_ir(node->right);
	if (found_error) return;
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (lt == STRUCT_CALL) {
		Node *func = find_op_overload(node->left->token, node->right->token, ASSIGN);
		if (func) {
			node->token->Fcall.ptr = func;
			func->token->used++;
			return;
		}
	}
	if (!compatible(node->left->token, node->right->token)) {
		Type rt =
		    node->right->token->ret_type ? node->right->token->ret_type : node->right->token->type;
		parse_error(node->token, "type mismatch: cannot assign %s to %s", to_string(rt),
		            to_string(lt));
		return;
	}
	node->token->ret_type = node->left->token->ret_type;
}

void ir_compound_assign(Node *node) {
	gen_ir(node->left);
	gen_ir(node->right);
	node->token->ret_type = node->left->token->ret_type;
	node->token->used++;
	node->left->token->used++;
	node->right->token->used++;
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (lt == STRUCT_CALL) {
		Node *func = find_op_overload(node->left->token, node->right->token, node->token->type);
		if (func) {
			node->token->Fcall.ptr = func;
			func->token->used++;
			node->token->ret_type = func->token->ret_type;
		}
	}
}

void ir_unary_op(Node *node) {
	gen_ir(node->left);
	node->token->used++;
	node->left->token->used++;
	if (node->token->type == NOT) node->token->ret_type = BOOL;
	else
		node->token->ret_type =
		    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
}

void ir_binary_op(Node *node) {
	gen_ir(node->left);
	gen_ir(node->right);
	CHECK(!node->left || !node->left->token, "left is NULL");
	CHECK(!node->right || !node->right->token, "right is NULL");
	if (includes(node->token->type, ADD, SUB, MUL, DIV, MOD, 0))
		node->token->ret_type = node->left->token->ret_type;
	else if (includes(node->token->type, COMPARISON_OPS, AND, OR, 0)) node->token->ret_type = BOOL;
	node->token->used++;
	node->left->token->used++;
	node->right->token->used++;
	set_ret_type(node);
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	if (lt == STRUCT_CALL) {
		Node *func = find_op_overload(node->left->token, node->right->token, node->token->type);
		if (!func) {
			parse_error(
			    node->token, "no `operator %s` overload for struct '%s'", to_string(node->token->type),
			    node->left->token->Struct.ptr ? node->left->token->Struct.ptr->token->name : "?");
			return;
		}
		node->token->Fcall.ptr = func;
		func->token->used++;
		if (func->token->ret_type == STRUCT_CALL) {
			node->token->ret_type   = STRUCT_CALL;
			node->token->Struct.ptr = func->token->Struct.ptr;
		} else node->token->ret_type = func->token->ret_type;
	}
}

void ir_as(Node *node) {
	gen_ir(node->left);
	if (found_error) return;
	Type src = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	Type dst = node->right->token->type;
	if (src == STRUCT_CALL || dst == STRUCT_CALL) {
		parse_error(node->token, "cannot cast %s to %s", to_string(src), to_string(dst));
		return;
	}
	node->token->ret_type = dst;
}

void ir_ref(Node *node) {
	gen_ir(node->left);
	node->token->is_ref = true;
	Type lt = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	node->token->ret_type = lt;
}

void ir_dot(Node *node) {
	gen_ir(node->left);
	if (found_error) return;
	node->left->token->used++;
	if (node->left->token->Struct.ptr) node->left->token->Struct.ptr->token->used++;

	Node *src = NULL;
	if (node->left->token->type == STRUCT_CALL)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else if (node->left->token->ret_type == STRUCT_CALL && node->left->token->Struct.ptr)
		src = get_struct(node->left->token->Struct.ptr->token->name);
	else {
		parse_error(node->left->token, "cannot access field of non-struct value (%s)",
		            to_string(node->left->token->type));
		return;
	}

	Type ret_type = 0;
	bool found    = false;
	for (int i = 0; src && i < src->children_count; i++) {
		Node *src_child = src->children[i];
		if (strcmp(src_child->token->name, node->right->token->name) != 0) continue;
		found                            = true;
		ret_type                         = src_child->token->type;
		node->right->token->Struct.index = i;
		node->right->token->type         = src_child->token->type;
		node->right->token->ret_type     = src_child->token->ret_type;
		if (src_child->token->type == STRUCT_CALL)
			node->right->token->Struct.ptr = src_child->token->Struct.ptr;
		else if (src_child->token->type == ARRAY_TYPE) {
			node->right->token->Array.sub_type   = src_child->token->Array.sub_type;
			node->right->token->Array.depth      = src_child->token->Array.depth;
			node->right->token->Array.struct_ptr = src_child->token->Array.struct_ptr;
		}
		break;
	}
	if (!found) {
		parse_error(node->right->token, "'%s' has no field '%s'",
		            src ? src->token->name : "(unknown)",
		            node->right->token->name ? node->right->token->name : "?");
		return;
	}
	node->token->ret_type = ret_type;
	if (ret_type == STRUCT_CALL)     node->token->Struct.ptr = node->right->token->Struct.ptr;
	else if (ret_type == ARRAY_TYPE) {
		node->token->Array.sub_type   = node->right->token->Array.sub_type;
		node->token->Array.depth      = node->right->token->Array.depth;
		node->token->Array.struct_ptr = node->right->token->Array.struct_ptr;
	}
}

void ir_access(Node *node) {
	gen_ir(node->left);
	gen_ir(node->right);
	if (found_error) return;
	node->right->token->used++;
	Type type  = node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
	int  depth = node->left->token->Array.depth;
	Type ret_type = 0;
	switch (type) {
	case CHARS: ret_type = CHAR; break;
	case STACK: case HEAP: case ARRAY_TYPE: case ARRAY:
		if (depth > 1) {
			ret_type                      = ARRAY;
			node->token->Array.sub_type   = node->left->token->Array.sub_type;
			node->token->Array.depth      = depth - 1;
			node->token->Array.struct_ptr = node->left->token->Array.struct_ptr;
		} else {
			ret_type = node->left->token->Array.sub_type;
			if (ret_type == STRUCT_CALL) node->token->Struct.ptr = node->left->token->Array.struct_ptr;
		}
		break;
	case STRUCT_CALL: {
		Node *st = node->left->token->Struct.ptr;
		if (st && strncmp(st->token->name, LIST_STRUCT_PREFIX, sizeof(LIST_STRUCT_PREFIX) - 1) == 0) {
			ir_access_list_struct(node, st);
			return;
		}
		CHECK(1, "struct '%s' does not support [] access", st ? st->token->name : "(null)");
		break;
	}
	default: CHECK(1, "handle this case %s", to_string(node->left->token->type)); break;
	}
	node->token->ret_type = ret_type;
}

void ir_fcall(Node *node) {
	if (try_module_call(node))                          return;
	if (node->token->is_static_call)                    ir_static_call(node);
	else if (node->token->is_method_call && node->left) ir_method_call(node);
	else ir_regular_call(node);
}

void ir_output(Node *node) {
	for (int i = 0; i < node->left->children_count; i++) {
		gen_ir(node->left->children[i]);
		node->left->children[i]->token->used++;
	}
	node->token->ret_type = VOID;
}

void ir_typeof_sizeof(Node *node) {
	if (node->left && node->left->token->type == ID && node->left->token->name) {
		Node *st = get_struct(node->left->token->name);
		if (st) {
			node->left->token->type       = STRUCT_CALL;
			node->left->token->Struct.ptr = st;
		}
	}
	gen_ir(node->left);
	Token *type_tok = node->left->token;
	Type   type     = type_tok->type ? type_tok->type : type_tok->ret_type;
	if (node->token->type == TYPEOF) {
		node->token->ret_type    = CHARS;
		node->token->Chars.value = strdup(to_string(type));
	} else {
		node->token->ret_type  = INT;
		node->token->Int.value = 0;
	}
	node->token->used++;
}

void ir_stack_heap(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		gen_ir(node->children[i]);
		node->children[i]->token->used++;
	}
	node->token->used++;
}

void ir_tuple_unpack(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Node *lhs = node->children[i];
		if (lhs->token->is_dec) {
			if (scopes_count == 1) lhs->token->is_global = true;
			new_variable(lhs->token);
		}
	}
	gen_ir(node->left);
	if (found_error) return;

	Token *rhs  = node->left->token;
	Node  *func = rhs->Fcall.ptr;
	if (!func || func->token->ret_type != TUPLE) {
		parse_error(node->token, "tuple unpack requires a tuple-returning function call");
		return;
	}
	int got = node->children_count;
	int exp = func->token->Tuple.types_count;
	if (got != exp) {
		parse_error(node->token, "tuple unpack expects %d values, got %d names", exp, got);
		return;
	}
	for (int i = 0; i < got; i++) {
		Token *lhs_tok = node->children[i]->token;
		Token *rhs_tok = func->token->Tuple.types[i];
		Type   lt      = lhs_tok->ret_type ? lhs_tok->ret_type : lhs_tok->type;
		Type   rt      = rhs_tok->ret_type ? rhs_tok->ret_type : rhs_tok->type;
		if (lt != rt) {
			parse_error(lhs_tok, "tuple unpack element %d: expected %s, got %s", i + 1, to_string(rt),
			            to_string(lt));
			return;
		}
	}
}

void ir_return(Node *node) {
	if (node->children_count > 0) {
		for (int i = 0; i < node->children_count; i++)
			gen_ir(node->children[i]);
		node->token->ret_type = TUPLE;
		return;
	}
	if (!node->left) return;
	gen_ir(node->left);
	if (found_error) return;
	node->token->ret_type = node->left->token->type;
	if (node->left->token->type == STRUCT_CALL)
		node->token->Struct.ptr = node->left->token->Struct.ptr;
	node->left->token->used++;

	Node *fdec = NULL;
	for (int j = scopes_count; j > 0; j--) {
		if (scopes[j]->token->type == FDEC) {
			fdec = scopes[j];
			break;
		}
	}
	if (fdec && !compatible(fdec->token, node->left->token)) {
		Type got =
		    node->left->token->ret_type ? node->left->token->ret_type : node->left->token->type;
		parse_error(node->left->token, "return type mismatch in '%s': expected %s, got %s",
		            fdec->token->name, to_string(fdec->token->ret_type), to_string(got));
	}
}

void ir_if(Node *node) {
	enter_scope(node);
	Node *curr = node;
	while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0)) {
		if (includes(curr->token->type, IF, ELIF, 0)) {
			CHECK(curr->left == NULL, "error");
			gen_ir(curr->left);
			curr->left->token->used++;
		}
		for (int i = 0; i < curr->children_count; i++)
			gen_ir(curr->children[i]);
		curr = curr->right;
	}
	exit_scope();
	node->token->used++;
}

void ir_while(Node *node) {
	enter_scope(node);
	gen_ir(node->left);
	node->left->token->used++;
	for (int i = 0; i < node->children_count; i++)
		gen_ir(node->children[i]);
	exit_scope();
	node->token->used++;
}

void ir_fdec(Node *node) {
	Node *existing = find_function(node->token->name);
	if (existing && existing != node) {
		parse_error(node->token, "redefinition of function '%s'", node->token->name);
		return;
	}
	if (!existing) new_function(node);
	enter_scope(node);
	Node **params = (node->left ? node->left->children : NULL);
	for (int i = 0; params && i < node->left->children_count && !found_error; i++) {
		gen_ir(params[i]);
		params[i]->token->used++;
	}
	for (int i = 0; !node->token->is_proto && i < node->children_count; i++)
		gen_ir(node->children[i]);
	exit_scope();
}

void ir_struct_def(Node *node) {
	for (int i = 0; node && i < node->functions_count; i++) {
		Node *fn = node->functions[i];
		if (!find_function(fn->token->name)) add_function(scope, fn);
	}
	for (int i = 0; node && i < node->functions_count; i++)
		gen_ir(node->functions[i]);
}

void ir_enum_def(Node *node) {
	for (int i = 0; i < node->children_count; i++)
		node->children[i]->token->used++;
}

void ir_module(Node *node) {
	char *mname        = node->token->name;
	char *saved        = current_gen_module;
	current_gen_module = mname;
	for (int i = 0; i < node->functions_count; i++) {
		Node *fn    = node->functions[i];
		char *qname = format("%s.%s", mname, fn->token->name);
		setName(fn->token, qname);
		free(qname);
	}
	for (int i = 0; i < node->functions_count; i++)
		gen_ir(node->functions[i]);
	current_gen_module = saved;
}

void ir_array_lit(Node *node) {
	for (int i = 0; i < node->children_count; i++)
		gen_ir(node->children[i]);
	if (node->children_count == 0) return;
	Token *first             = node->children[0]->token;
	Type   ft                = first->ret_type ? first->ret_type : first->type;
	node->token->ret_type    = ARRAY;
	node->token->Array.depth = 1;
	if (includes(ft, ARRAY_LIT, ARRAY, ARRAY_TYPE, 0)) {
		node->token->Array.sub_type   = first->Array.sub_type;
		node->token->Array.struct_ptr = first->Array.struct_ptr;
		node->token->Array.depth      = first->Array.depth + 1;
	} else {
		node->token->Array.sub_type = ft;
		if (ft == STRUCT_CALL) node->token->Array.struct_ptr = first->Struct.ptr;
	}
}

void gen_ir(Node *node) {
	if (found_error) return;
	switch (node->token->type) {
	case ID:           ir_id(node); break;
	case STRUCT_CALL:  ir_struct_call(node); break;
	case NULLABLE:     ir_nullable(node); break;
	case INT:          case BOOL: case CHAR: case ARRAY_TYPE: case FLOAT: case LONG:
	case CHARS:        case PTR: case VOID:
	case SHORT:        ir_primitive(node); break;
	case TUPLE_UNPACK: ir_tuple_unpack(node); break;
	case ASSIGN:       ir_assign(node); break;
	case ADD_ASSIGN:   case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN:   ir_compound_assign(node); break;
	case NOT:
	case BNOT:      ir_unary_op(node); break;
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:    ir_binary_op(node); break;
	case AS:        ir_as(node); break;
	case REF:       ir_ref(node); break;
	case DOT:       ir_dot(node); break;
	case ACCESS:    ir_access(node); break;
	case FCALL:     ir_fcall(node); break;
	case OUTPUT:    ir_output(node); break;
	case TYPEOF:
	case SIZEOF: ir_typeof_sizeof(node); break;
	case STACK:
	case HEAP:       ir_stack_heap(node); break;
	case RETURN:     ir_return(node); break;
	case IF:         ir_if(node); break;
	case WHILE:      ir_while(node); break;
	case FDEC:       ir_fdec(node); break;
	case STRUCT_DEF: ir_struct_def(node); break;
	case ENUM_DEF:   ir_enum_def(node); break;
	case MODULE:     ir_module(node); break;
	case ARRAY_LIT:  ir_array_lit(node); break;
	case BREAK:
	case CONTINUE: break;
	default:       TODO(1, "handle this case %s", to_string(node->token->type)); break;
	}
}

// ============================================================================
// ASSEMBLY GENERATION
// ============================================================================

void asm_struct_call(Node *node) { gen_struct_declaration(node->token); }

void asm_nullable(Node *node) { node->token->llvm.elem = LLVMConstNull(p8); }

void asm_primitive(Node *node) { gen_primitive_declaration(node->token); }

void asm_assign(Node *node) {
	Node *left  = node->left;
	Node *right = node->right;
	if (node->token->Fcall.ptr) {
		Value result = gen_operator_call(node, left, right, false);
		if (result) node->token->llvm.elem = result;
		return;
	}
	if (left->token->is_global && left->token->is_dec) {
		gen_asm(left);
		gen_asm(right);
		if (left->token->llvm.elem && right->token->llvm.elem &&
		    LLVMIsConstant(right->token->llvm.elem))
			LLVMSetInitializer(left->token->llvm.elem, right->token->llvm.elem);
		return;
	}
	if (left->token->is_ref && right->token->type == REF) {
		if (!left->token->llvm.elem) _alloca(left->token);
		gen_asm(right);
		LLVMBuildStore(builder, right->token->llvm.elem, left->token->llvm.elem);
		left->token->is_dec = false;
		return;
	}
	gen_asm(left);
	gen_asm(right);
	if (includes(right->token->type, STACK, HEAP, 0) && right->token->llvm.dims_count > 1)
		propagate_dims(left->token, right->token, left);
	Value val;
	if (right->token->is_ref) val = read_value(right->token);
	else {
		ensure_loaded(right);
		val = right->token->llvm.elem;
	}
	val = asm_assign_cast(left, right, val);
	write_value(left->token, val);
	node->token->llvm.elem = val;
}

void asm_compound_assign(Node *node) {
	Node *left  = node->left;
	Node *right = node->right;
	if (node->token->Fcall.ptr) {
		Value result = gen_operator_call(node, left, right, false);
		if (result) node->token->llvm.elem = result;
		return;
	}
	gen_asm(left);
	gen_asm(right);
	Value current_val = read_value(left->token);
	Value right_val;
	if (right->token->is_ref) right_val = read_value(right->token);
	else {
		ensure_loaded(right);
		right_val = right->token->llvm.elem;
	}
	Value result = build_binary_op(assign_base_op(node->token->type), current_val, right_val);
	write_value(left->token, result);
	node->token->llvm.elem = result;
}

void asm_unary_op(Node *node) {
	Node *left = node->left;
	gen_asm(left);
	ensure_loaded(left);
	if (left->token->is_ref) {
		left->token->llvm.elem      = read_value(left->token);
		left->token->llvm.is_loaded = true;
	}
	node->token->llvm.elem =
	    LLVMBuildNot(builder, left->token->llvm.elem, to_string(node->token->type));
}

void asm_binary_op(Node *node) {
	Node *left  = node->left;
	Node *right = node->right;
	if (node->token->Fcall.ptr) {
		Value result = gen_operator_call(node, left, right, true);
		if (result) {
			node->token->llvm.elem = result;
			if (node->token->Fcall.ptr->token->ret_type != STRUCT_CALL &&
			    node->token->Fcall.ptr->token->ret_type != VOID)
				node->token->llvm.is_loaded = true;
		}
		return;
	}
	gen_asm(left);
	gen_asm(right);
	ensure_loaded(left);
	ensure_loaded(right);
	Value lref = left->token->llvm.elem;
	Value rref = right->token->llvm.elem;
	if (left->token->is_ref && !left->token->llvm.is_loaded)                                  lref = read_value(left->token);
	if (right->token->is_ref && !right->token->llvm.is_loaded)                                rref = read_value(right->token);
	if (node->token->type == ADD && LLVMGetTypeKind(LLVMTypeOf(lref)) == LLVMPointerTypeKind) {
		Value idx              = LLVMGetTypeKind(LLVMTypeOf(rref)) == LLVMIntegerTypeKind &&
                          LLVMGetIntTypeWidth(LLVMTypeOf(rref)) < 64
		                             ? LLVMBuildSExt(builder, rref, i64, "idx")
		                             : rref;
		node->token->llvm.elem = LLVMBuildGEP2(builder, i8, lref, &idx, 1, "ADD");
	} else node->token->llvm.elem = build_binary_op(node->token->type, lref, rref);
	if (includes(node->token->type, COMPARISON_OPS, 0)) node->token->ret_type = BOOL;
	else node->token->ret_type = left->token->ret_type ? left->token->ret_type : left->token->type;
}

void asm_as(Node *node) {
	Node *left = node->left;
	gen_asm(left);
	ensure_loaded(left);
	Value    source = left->token->llvm.elem;
	TypeRef  stype  = LLVMTypeOf(source);
	TypeRef  ttype  = get_llvm_type(node->right->token);
	TypeKind sk     = LLVMGetTypeKind(stype);
	TypeKind tk     = LLVMGetTypeKind(ttype);
	bool     sf     = (sk == FloatType || sk == DoubleType);
	bool     tf     = (tk == FloatType || tk == DoubleType);
	Value    result;
	if (stype == ttype) result = source;
	else if (sk == IntegerType && tk == IntegerType)
		result = asm_as_int_to_int(source, stype, ttype);
	else if (sk == IntegerType && tf) result = LLVMBuildSIToFP(builder, source, ttype, "as");
	else if (sf && tk == IntegerType) result = LLVMBuildFPToSI(builder, source, ttype, "as");
	else if (sf && tf)                {
		if (sk == FloatType) result = LLVMBuildFPExt(builder, source, ttype, "as");
		else result = LLVMBuildFPTrunc(builder, source, ttype, "as");
	} else if (sk == IntegerType && tk == PointerType)
		result = LLVMBuildIntToPtr(builder, source, ttype, "as");
	else if (sk == PointerType && tk == IntegerType)
		result = LLVMBuildPtrToInt(builder, source, ttype, "as");
	else if (sk == PointerType && tk == PointerType)
		result = LLVMBuildBitCast(builder, source, ttype, "as");
	else {
		CHECK(1, "unsupported cast from %d to %d", sk, tk);
		result = source;
	}
	node->token->llvm.elem      = result;
	node->token->llvm.is_loaded = true;
}

void asm_ref(Node *node) {
	gen_asm(node->left);
	node->token->llvm.elem = node->left->token->llvm.elem;
}

void asm_stack(Node *node) {
	Token   tmp                  = {.type = node->token->Array.sub_type};
	TypeRef elem_t               = get_llvm_type(&tmp);
	Value   total                = asm_collect_dims(node);
	Value   bytes                = asm_total_bytes(total, elem_t);
	node->token->llvm.elem       = allocate_stack(bytes, elem_t, "stack");
	node->token->llvm.is_set     = true;
	node->token->llvm.array_size = total;
}

void asm_heap(Node *node) {
	Token   tmp                  = {.type = node->token->Array.sub_type};
	TypeRef elem_t               = get_llvm_type(&tmp);
	Value   total                = asm_collect_dims(node);
	Value   bytes                = asm_total_bytes(total, elem_t);
	node->token->llvm.elem       = allocate_heap(bytes, elem_t, "heap");
	node->token->llvm.is_set     = true;
	node->token->llvm.array_size = total;
}

void asm_array_lit(Node *node) {
	int     count    = node->children_count;
	TypeRef elem_t   = asm_array_lit_elem_type(node);
	Value   cnt      = LLVMConstInt(i32, count, 0);
	Value   raw_data = allocate_heap(cnt, elem_t, "array_lit");
	Value   data     = LLVMBuildBitCast(builder, raw_data, LLVMPointerType(elem_t, 0), "lit_data");
	for (int i = 0; i < count; i++) {
		gen_asm(node->children[i]);
		ensure_loaded(node->children[i]);
		Value idx       = LLVMConstInt(i32, i, 0);
		Value indices[] = {idx};
		Value gep       = LLVMBuildGEP2(builder, elem_t, data, indices, 1, "lit_elem");
		LLVMBuildStore(builder, node->children[i]->token->llvm.elem, gep);
	}
	node->token->llvm.elem       = data;
	node->token->llvm.is_set     = true;
	node->token->llvm.array_size = cnt;
}

void asm_dot(Node *node) {
	gen_asm(node->left);
	Token *struct_tok  = node->left->token;
	int    field_index = node->right->token->Struct.index;
	if (struct_tok->is_ref) {
		TypeRef struct_type = get_llvm_type(struct_tok);
		Value   struct_ptr  = LLVMBuildLoad2(builder, LLVMPointerType(struct_type, 0),
		                                     struct_tok->llvm.elem, struct_tok->name);
		Value   indices[]   = {LLVMConstInt(i32, 0, 0), LLVMConstInt(i32, field_index, 0)};
		node->token->llvm.elem =
		    LLVMBuildGEP2(builder, struct_type, struct_ptr, indices, 2, node->right->token->name);
	} else {
		node->token->llvm.elem = struct_field_ptr(struct_tok, field_index, node->right->token->name);
	}
	asm_dot_propagate_field_dims(struct_tok, field_index, node->token);
}

void asm_access(Node *node) {
	gen_asm(node->left);
	if (node->left->token->type == STRUCT_CALL) {
		asm_access_struct_field(node);
		return;
	}
	gen_asm(node->right);
	Value left_value = asm_access_left_value(node);
	ensure_loaded(node->right);
	Value right_ref = node->right->token->llvm.elem;
	if (asm_access_multidim(node, left_value, right_ref)) return;
	TypeRef element_type   = asm_access_element_type(node);
	Value   indices[]      = {right_ref};
	node->token->llvm.elem = LLVMBuildGEP2(builder, element_type, left_value, indices, 1, "ACCESS");
}

void asm_fcall(Node *node) {
	if (node->token->is_static_call) asm_fcall_static(node);
	else asm_fcall_instance(node);
}

void asm_typeof_sizeof(Node *node) {
	Token *type_tok = node->left->token;
	if (node->token->type == TYPEOF) {
		node->token->llvm.elem =
		    LLVMBuildGlobalStringPtr(builder, node->token->Chars.value, "typeof");
		node->token->llvm.is_loaded = true;
		return;
	}
	TypeRef    type             = get_llvm_type(type_tok);
	TargetData td               = LLVMGetModuleDataLayout(module);
	size_t     size             = LLVMABISizeOfType(td, type);
	node->token->Int.value      = (long long)size;
	node->token->llvm.elem      = LLVMConstInt(i32, size, 0);
	node->token->llvm.is_loaded = true;
}

void asm_output(Node *node) {
	int    argc  = node->left->children_count;
	Node **argv  = node->left->children;
	int    cap   = output_format_capacity(argc, argv);
	char  *fmt   = allocate(cap, 1);
	Value *args  = allocate(256, sizeof(Value));
	int    fc    = 0;
	int    nargs = 0;
	for (int i = 0; i < argc; i++) {
		gen_asm(argv[i]);
		append_output_arg(argv[i]->token, fmt, &fc, args, &nargs);
	}
	fmt[fc]         = '\0';
	Value printf_fn = LLVMGetNamedFunction(module, "printf");
	if (!printf_fn) {
		TypeRef params[]  = {p8, i32};
		TypeRef printf_ft = LLVMFunctionType(i32, params, 2, 1);
		printf_fn         = _add_function("printf", printf_ft);
	}
	TypeRef printf_ft = LLVMGlobalGetValueType(printf_fn);
	Value  *call_args = allocate(nargs + 3, sizeof(Value));
	call_args[0]      = LLVMBuildGlobalStringPtr(builder, fmt, "output_fmt");
	call_args[1]      = LLVMConstInt(i32, nargs, 0);
	memcpy(call_args + 2, args, nargs * sizeof(Value));
	LLVMBuildCall2(builder, printf_ft, printf_fn, call_args, nargs + 2, "");
	free(fmt);
	free(args);
	free(call_args);
}

void asm_tuple_unpack(Node *node) {
	gen_asm(node->left);
	Value tuple_val = node->left->token->llvm.elem;
	for (int i = 0; i < node->children_count; i++) {
		Node *lhs = node->children[i];
		gen_asm(lhs);
		Value field = LLVMBuildExtractValue(builder, tuple_val, i, lhs->token->name);
		LLVMBuildStore(builder, field, lhs->token->llvm.elem);
	}
}

void asm_while(Node *node) {
	enter_scope(node);
	Block start             = _append_block("while.start");
	Block then              = _append_block("while.then");
	Block end               = _append_block("while.end");
	node->token->llvm.start = start;
	node->token->llvm.then  = then;
	node->token->llvm.end   = end;
	_branch(start);
	LLVMPositionBuilderAtEnd(builder, start);
	gen_asm(node->left);
	ensure_loaded(node->left);
	LLVMBuildCondBr(builder, node->left->token->llvm.elem, then, end);
	LLVMPositionBuilderAtEnd(builder, then);
	for (int i = 0; i < node->children_count; i++) {
		if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) break;
		gen_asm(node->children[i]);
	}
	_branch(start);
	LLVMPositionBuilderAtEnd(builder, end);
	exit_scope();
}

void asm_if(Node *node) {
	enter_scope(node);
	Block if_start          = _append_block("if.start");
	Block end               = _append_block("if.end");
	node->token->llvm.start = if_start;
	node->token->llvm.end   = end;
	_branch(if_start);
	gen_if_chain(node, if_start, end);
	LLVMPositionBuilderAtEnd(builder, end);
	exit_scope();
}

void asm_break(void) {
	for (int i = scopes_count; i > 0; i--)
		if (scopes[i]->token->type == WHILE) {
			_branch(scopes[i]->token->llvm.end);
			return;
		}
	CHECK(1, "break outside loop");
}

void asm_continue(void) {
	for (int i = scopes_count; i > 0; i--)
		if (scopes[i]->token->type == WHILE) {
			_branch(scopes[i]->token->llvm.start);
			return;
		}
	CHECK(1, "continue outside loop");
}

void asm_return(Node *node) {
	Token *ret_skip =
	    (node->left && node->left->token->type == STRUCT_CALL && !node->left->token->is_ref)
	        ? node->left->token
	        : NULL;
	emit_scope_clean(scope, 0, ret_skip);
	Node *fdec = find_enclosing(FDEC);
	if (CHECK(!fdec, "can not return outside function")) return;
	char *fn_name = fdec->token->name;
	int   fn_len  = strlen(fn_name);
	if (fn_len > 7 && strcmp(fn_name + fn_len - 7, ".delete") == 0) asm_return_delete_chain(fdec);
	if (strcmp(fdec->token->name, "main") == 0)                     asm_return_main_globals();
	if (fdec->token->ret_type == TUPLE && node->children_count > 0) {
		asm_return_tuple(node, fdec);
		return;
	}
	asm_return_value(node, fdec);
}

void asm_fdec(Node *node) {
	Node *registered = find_function(node->token->name);
	if (registered && registered != node) return;
	char *name      = node->token->name;
	bool  is_method = name && strchr(name, '.');
	bool  is_main   = name && strcmp(name, "main") == 0;
	if (node->token->used == 0 && !is_method && !is_main) return;
	resolve_func_type(node);
	enter_scope(node);
	if (!node->token->is_proto) emit_func_body(node);
	exit_scope();
	debug_scope = debug_compile_unit;
}

void asm_enum_def(Node *node) {
	for (int i = 0; i < node->children_count; i++) {
		Token *var = node->children[i]->token;
		Value  g   = LLVMAddGlobal(module, i32, var->name);
		LLVMSetInitializer(g, LLVMConstInt(i32, (unsigned)var->Int.value, 0));
		LLVMSetGlobalConstant(g, 1);
		var->llvm.elem = g;
		var->is_dec    = false;
	}
}

void asm_module(Node *node) {
	for (int i = 0; i < node->functions_count; i++)
		gen_asm(node->functions[i]);
}

void asm_struct_def(Node *node) {
	if (node->token->used == 0) return;
	gen_struct_emit_nested(node);
	gen_struct_build_type(node);
	gen_struct_emit_delete(node);
	gen_struct_predeclare_methods(node);
	for (int i = 0; i < node->functions_count; i++)
		gen_asm(node->functions[i]);
}

void gen_asm(Node *node) {
	set_debug_location(node->token);
	if (CHECK(node->token->llvm.is_set, "already set")) return;
	switch (node->token->type) {
	case STRUCT_CALL: asm_struct_call(node); break;
	case NULLABLE:    asm_nullable(node); break;
	case INT:         case LONG: case SHORT: case CHARS: case CHAR: case BOOL:
	case ARRAY_TYPE:
	case FLOAT:      asm_primitive(node); break;
	case ASSIGN:     asm_assign(node); break;
	case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
	case MOD_ASSIGN: asm_compound_assign(node); break;
	case NOT:
	case BNOT:      asm_unary_op(node); break;
	case ADD:       case SUB: case MUL: case DIV: case MOD: case EQUAL:
	case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL: case GREAT_EQUAL:
	case AND:       case OR: case BAND: case BOR: case BXOR: case LSHIFT:
	case RSHIFT:    asm_binary_op(node); break;
	case AS:        asm_as(node); break;
	case REF:       asm_ref(node); break;
	case STACK:     asm_stack(node); break;
	case HEAP:      asm_heap(node); break;
	case ARRAY_LIT: asm_array_lit(node); break;
	case DOT:       asm_dot(node); break;
	case ACCESS:    asm_access(node); break;
	case FCALL:     asm_fcall(node); break;
	case OUTPUT:    asm_output(node); break;
	case TYPEOF:
	case SIZEOF:       asm_typeof_sizeof(node); break;
	case TUPLE_UNPACK: asm_tuple_unpack(node); break;
	case RETURN:       asm_return(node); break;
	case IF:           asm_if(node); break;
	case WHILE:        asm_while(node); break;
	case BREAK:        asm_break(); break;
	case CONTINUE:     asm_continue(); break;
	case FDEC:         asm_fdec(node); break;
	case STRUCT_DEF:   asm_struct_def(node); break;
	case ENUM_DEF:     asm_enum_def(node); break;
	case MODULE:       asm_module(node); break;
	default:           TODO(1, "handle this case %k", node->token); break;
	}
}
