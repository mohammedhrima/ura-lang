#include "header.h"

Block here_block() {
   return LLVMGetInsertBlock(ura.builder);
}

Value here_func() {
   return LLVMGetBasicBlockParent(here_block());
}

void llvm_at(Block block) {
   LLVMPositionBuilderAtEnd(ura.builder, block);
}

Block llvm_block(Value fn, char *name) {
   return LLVMAppendBasicBlockInContext(ura.context, fn, name);
}

Value llvm_alloca(TypeRef type, char *name) {
   return LLVMBuildAlloca(ura.builder, type, name);
}

Value llvm_load(TypeRef type, Value ptr, char *name) {
   return LLVMBuildLoad2(ura.builder, type, ptr, name);
}

void llvm_store(Value value, Value ptr) {
   LLVMBuildStore(ura.builder, value, ptr);
}

Value llvm_gep(TypeRef type, Value ptr, Value *idx, int n, char *name) {
   return LLVMBuildGEP2(ura.builder, type, ptr, idx, n, name);
}

Value llvm_icmp(LLVMIntPredicate pred, Value l, Value r, char *name) {
   return LLVMBuildICmp(ura.builder, pred, l, r, name);
}

Value llvm_fcmp(LLVMRealPredicate pred, Value l, Value r, char *name) {
   return LLVMBuildFCmp(ura.builder, pred, l, r, name);
}

void llvm_br(Block dest) {
   LLVMBuildBr(ura.builder, dest);
}

void llvm_cond_br(Value cond, Block yes, Block no) {
   LLVMBuildCondBr(ura.builder, cond, yes, no);
}

Value llvm_call(TypeRef type, Value fn, Value *args, int n, char *name) {
   return LLVMBuildCall2(ura.builder, type, fn, args, n, name);
}

Value llvm_extract(Value agg, unsigned index, char *name) {
   return LLVMBuildExtractValue(ura.builder, agg, index, name);
}

Value llvm_insert(Value agg, Value elem, unsigned index, char *name) {
   return LLVMBuildInsertValue(ura.builder, agg, elem, index, name);
}

Value llvm_string(char *text, char *name) {
   return LLVMBuildGlobalStringPtr(ura.builder, text, name);
}

Value const_int(TypeRef type, long long value) {
   return LLVMConstInt(type, value, 0);
}

Value const_i32(long long value) {
   return LLVMConstInt(ura.i32, value, 0);
}

Value const_i64(long long value) {
   return LLVMConstInt(ura.i64, value, 0);
}

TypeRef pointer_to(TypeRef type) {
   return LLVMPointerType(type, 0);
}

Value llvm_binop(LLVMOpcode op, Value l, Value r, char *name) {
   return LLVMBuildBinOp(ura.builder, op, l, r, name);
}

Value llvm_ret(Value value) {
   return LLVMBuildRet(ura.builder, value);
}

Value llvm_not(Value value) {
   return LLVMBuildNot(ura.builder, value, "not");
}

Value llvm_int_cast(Value value, TypeRef type) {
   return LLVMBuildIntCast2(ura.builder, value, type, 1, "cast");
}

Value llvm_num_cast(Value value, Type src, Type dst) {
   if (src == dst) return value;
   TypeRef to = to_llvm_type(dst);
   if (is_float(src) && is_float(dst))
      return dst == F64 ? LLVMBuildFPExt(ura.builder, value, to, "cast")
                           : LLVMBuildFPTrunc(ura.builder, value, to, "cast");
   if (is_float(src))
      return is_unsigned(dst)
         ? LLVMBuildFPToUI(ura.builder, value, to, "cast")
         : LLVMBuildFPToSI(ura.builder, value, to, "cast");
   if (is_float(dst))
      return is_unsigned(src)
         ? LLVMBuildUIToFP(ura.builder, value, to, "cast")
         : LLVMBuildSIToFP(ura.builder, value, to, "cast");
   return LLVMBuildIntCast2(ura.builder, value, to, !is_unsigned(src), "cast");
}

Value llvm_global(Token *token) {
   TypeRef type = llvm_type_of(token);
   Value   glob = LLVMAddGlobal(ura.module, type, token->name);
   LLVMSetInitializer(glob, LLVMConstNull(type));
   LLVMSetLinkage(glob, LLVMInternalLinkage);
   return token->llvm.elem = glob;
}

void llvm_sanitize(Value fn) {
   unsigned     kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
   AttributeRef attr = LLVMCreateEnumAttribute(ura.context, kind, 0);
   LLVMAddAttributeAtIndex(fn, LLVMAttributeFunctionIndex, attr);
}

MetadataRef llvm_di_subroutine() {
   return LLVMDIBuilderCreateSubroutineType(ura.debug_builder,
                                            ura.debug_file, NULL, 0,
                                            LLVMDIFlagZero);
}

MetadataRef llvm_di_function(char *name, int line, MetadataRef type) {
   size_t len = strlen(name);
   return LLVMDIBuilderCreateFunction(ura.debug_builder,
                                      ura.debug_compile_unit, name, len,
                                      name, len, ura.debug_file, line,
                                      type, 0, 1, line, LLVMDIFlagZero, 0);
}

MetadataRef llvm_di_location(int line, MetadataRef scope) {
   return LLVMDIBuilderCreateDebugLocation(ura.context, line, 0, scope, NULL);
}

MetadataRef llvm_get_location() {
   return LLVMGetCurrentDebugLocation2(ura.builder);
}

void llvm_set_location(MetadataRef loc) {
   LLVMSetCurrentDebugLocation2(ura.builder, loc);
}

void setup_paths(char *path_name) {
   char *slash = strrchr(path_name, '/');
   if (slash) {
      ura.dir = strdup(path_name);
      ura.dir[slash - path_name] = '\0';
      ura.base = strdup(slash + 1);
   } else {
      ura.dir = strdup(".");
      ura.base = strdup(path_name);
   }
   char *dot = strrchr(ura.base, '.');
   if (dot) *dot = '\0';
   ura.build_dir = format("%s/build", ura.dir);
   mkdir(ura.build_dir, 0755);
   ura.ll_path = format("%s/%s.ll", ura.build_dir, ura.base);
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
   ura.f32 = LLVMFloatTypeInContext(ura.context);
   ura.f64 = LLVMDoubleTypeInContext(ura.context);
   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();
   char *triple = LLVMGetDefaultTargetTriple();
   LLVMSetTarget(ura.module, triple);
   Target target;
   if (!LLVMGetTargetFromTriple(triple, &target, NULL)) {
      TargetMachine machine = LLVMCreateTargetMachine(target, triple, "", "", LLVMCodeGenLevelDefault, LLVMRelocDefault, LLVMCodeModelDefault);
      TargetData layout = LLVMCreateTargetDataLayout(machine);
      LLVMSetModuleDataLayout(ura.module, layout);
      LLVMDisposeTargetData(layout);
      LLVMDisposeTargetMachine(machine);
   }
   LLVMDisposeMessage(triple);

   if (!ura.enable_san) return;
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Debug Info Version", 18, LLVMValueAsMetadata(const_i32(3)));
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13, LLVMValueAsMetadata(const_i32(4)));
   ura.debug_builder = LLVMCreateDIBuilder(ura.module);
   char *src   = ura.sources[0]->filename;
   char *slash = strrchr(src, '/');
   char *file  = slash ? slash + 1 : src;
   ura.debug_file = LLVMDIBuilderCreateFile(ura.debug_builder, file, strlen(file),
                                            ura.dir, strlen(ura.dir));
   ura.debug_compile_unit = LLVMDIBuilderCreateCompileUnit(
       ura.debug_builder, LLVMDWARFSourceLanguageC, ura.debug_file, "ura", 3, 0, "", 0, 0, "", 0,
       LLVMDWARFEmissionFull, 0, 0, 0, "", 0, "", 0);
   ura.debug_scope = ura.debug_compile_unit;
}

void finalize_module(char *ll_path) {
   char *error = NULL;
   PassBuilderOptions opts = LLVMCreatePassBuilderOptions();
   if (ura.flags) {
      Error err = LLVMRunPasses(ura.module, ura.flags, NULL, opts);
      if (err) {
         char *msg = LLVMGetErrorMessage(err);
         CHECK(1, "optimizer error: %s", msg);
         LLVMDisposeErrorMessage(msg);
      }
   }
   if (ura.debug_builder) {
      LLVMDIBuilderFinalize(ura.debug_builder);
      LLVMDisposeDIBuilder(ura.debug_builder);
      ura.debug_builder = NULL;
   }
   if (LLVMVerifyModule(ura.module, LLVMReturnStatusAction, &error))
      CHECK(1, "module verification failed:\n%s", error);
   LLVMDisposeMessage(error);
   LLVMDisposePassBuilderOptions(opts);
   LLVMPrintModuleToFile(ura.module, ll_path, NULL);
   if (ura.enable_ll) {
      char *ir = LLVMPrintModuleToString(ura.module);
      printf("%s", ir);
      LLVMDisposeMessage(ir);
   }
}

void debug_enter_function(Token *token) {
   token->llvm.prev_block = here_block();
   token->llvm.prev_scope = ura.debug_scope;
   token->llvm.prev_loc   = ura.debug_builder ? llvm_get_location() : NULL;
   llvm_at(llvm_block(token->llvm.elem, "entry"));
   if (ura.enable_san) llvm_sanitize(token->llvm.elem);
   if (!ura.debug_builder) return;

   MetadataRef di_type = llvm_di_subroutine();
   MetadataRef di_func = llvm_di_function(token->name, token->line, di_type);
   LLVMSetSubprogram(token->llvm.elem, di_func);
   ura.debug_scope = di_func;
   llvm_set_location(llvm_di_location(token->line, di_func));
}

void debug_exit_function(Token *token) {
   if (token->llvm.prev_block) llvm_at(token->llvm.prev_block);
   if (!ura.debug_builder) return;
   ura.debug_scope = token->llvm.prev_scope;
   llvm_set_location(token->llvm.prev_loc);
}

void set_debug_location(Token *token) {
   if (!ura.debug_builder || !ura.debug_scope) return;
   llvm_set_location(llvm_di_location(token->line, ura.debug_scope));
}

Value promote(Type type, Value v) {
   switch (type) {
      case F32:
         return LLVMBuildFPExt(ura.builder, v, ura.f64, "f2d");
      case I8: case I16: case CHAR:
         return LLVMBuildSExt(ura.builder, v, ura.i32, "i2i");
      case BOOL: case U8: case U16:
         return LLVMBuildZExt(ura.builder, v, ura.i32, "u2i");
      default: return v;
   }
}

Value lib_fn(char *name, TypeRef *type) {
   Node *fn = find_function(name);
   if (!fn) {
      parse_error(NULL, ERR_MISSING_LIB_FN, name);
      return NULL;
   }
   emit_signature(fn);
   *type = fn->token->llvm.func_type;
   return fn->token->llvm.elem;
}

void guard(Token *op, Value is_bad, char *what) {
   Value fn   = here_func();
   Block trap = llvm_block(fn, "trap");
   Block cont = llvm_block(fn, "cont");
   llvm_cond_br(is_bad, trap, cont);

   llvm_at(trap);
   char  *text = NULL;
   size_t tlen = 0;
   File   ms   = open_memstream(&text, &tlen);
   fprintf(ms, RED("runtime error: ") "%s\n", what);
   render_caret(ms, op, CARET_ERR);
   fclose(ms);
   if (ura.no_color) decolor(text);
   Value   msg  = llvm_string(text, "trap_msg");
   TypeRef i8p  = pointer_to(ura.i8);

   TypeRef write_ty      = NULL;
   Value   write_fn      = lib_fn("write", &write_ty);
   Value   write_args[3] = { const_i32(2), msg, LLVMConstInt(ura.i64, strlen(text), 0) };
   if (write_fn) llvm_call(write_ty, write_fn, write_args, 3, "");
   free(text);

   TypeRef exit_ty      = NULL;
   Value   exit_fn      = lib_fn("exit", &exit_ty);
   Value   exit_args[1] = { const_i32(1) };
   if (exit_fn) llvm_call(exit_ty, exit_fn, exit_args, 1, "");

   LLVMBuildUnreachable(ura.builder);
   llvm_at(cont);
}

void guard_nonzero(Token *op, Value divisor) {
   TypeRef ty     = LLVMTypeOf(divisor);
   bool    fp     = LLVMGetTypeKind(ty) == FloatType || LLVMGetTypeKind(ty) == DoubleType;
   Value   iszero = fp
      ? llvm_fcmp(LLVMRealOEQ, divisor, LLVMConstReal(ty, 0.0), "iszero")
      : llvm_icmp(LLVMIntEQ,   divisor, LLVMConstInt(ty, 0, 0), "iszero");
   guard(op, iszero, includes(op->type, MOD, MOD_ASSIGN, 0) ? "Modulo by zero" : "Division by zero");
}

void guard_nonnull(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = llvm_icmp(LLVMIntEQ, ptr, null, "isnull");
   guard(op, isnull, "Call to a null function value");
}

void guard_bound(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = llvm_icmp(LLVMIntEQ, ptr, null, "unbound");
   char *msg    = format("reference '%s' used before it was bound - assign '%s = ref <target>' first", op->name, op->name);
   guard(op, isnull, msg);
   free(msg);
}

void guard_index(Token *op, Value idx, Value slice) {
   Value len   = llvm_extract(slice, 1, "arr.len");
   Value idx64 = LLVMBuildIntCast2(ura.builder, idx, ura.i64, 1, "idx");
   Value low   = llvm_icmp(LLVMIntSLT, idx64, const_i64(0), "oob.low");
   Value high  = llvm_icmp(LLVMIntSGE, idx64, len, "oob.high");
   guard(op, LLVMBuildOr(ura.builder, low, high, "oob"), "array index out of bounds");
}

void guard_slice(Token *op, Value start, Value end, Value len) {
   Value lo  = llvm_icmp(LLVMIntSLT, start, const_i64(0), "s.lo");
   Value hi  = llvm_icmp(LLVMIntSGT, end, len, "e.hi");
   Value ord = llvm_icmp(LLVMIntSGT, start, end, "s.gt");
   Value bad = LLVMBuildOr(ura.builder, LLVMBuildOr(ura.builder, lo, hi, "b"), ord, "bad");
   guard(op, bad, "slice range out of bounds");
}

void emit_signature(Node *fn) {
   Token *token = fn->token;
   if (token->llvm.func_type) return;
   int      n      = token->Fn.params_count;
   TypeRef *params = NULL;
   if (n > 0) {
      params = allocate(n, sizeof(TypeRef));
      for (int i = 0; i < n; i++) {
         TypeRef pt = llvm_type_of(token->Fn.params[i]);
         params[i] = token->Fn.params[i]->is_ref ? pointer_to(pt) : pt;
      }
   }
   TypeRef ret;
   if (token->ret_type == STRUCT_CALL)
      ret = struct_type_of(token->Struct.ptr);
   else if (token->ret_type == ARRAY_TYPE)
      ret = array_type(token, token->Array.depth);
   else
      ret = to_llvm_type(token->ret_type);
   if (token->is_ref) ret = pointer_to(ret);
   if (is_main(token)) {
      TypeRef argv = pointer_to(pointer_to(ura.i8));
      TypeRef mp[2] = { ura.i32, argv };
      token->llvm.func_type = LLVMFunctionType(ret, mp, 2, 0);
   } else
      token->llvm.func_type = LLVMFunctionType(ret, params, n, token->is_variadic);
   token->llvm.elem      = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
   free(params);
}

Value field_ptr(Node *node) {
   Token  *token  = node->token;
   Token  *left   = node->left->token;
   Value   base   = struct_arg_ptr(node->left);
   TypeRef sty    = struct_type_of(left->Struct.ptr);
   Value   idx[2] = { const_i32(0), const_i32(token->Struct.index) };
   return llvm_gep(sty, base, idx, 2, token->name);
}

Value access_ptr(Node *node) {
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = llvm_extract(slice, 0, "arr.data");
   code_gen(node->right);
   Value   idx   = node->right->token->llvm.elem;
   if (node->token->is_nullable) guard_index(node->token, idx, slice);
   Token  *arr   = node->left->token;
   TypeRef elem  = elem_type(arr, arr->Array.depth);
   return llvm_gep(elem, data, &idx, 1, "arr.at");
}

void code_gen_slice(Node *node) {
   Token *arr   = node->left->token;
   Node  *range = node->right;
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = llvm_extract(slice, 0, "arr.data");
   code_gen(range->left);
   code_gen(range->right);
   Value   start = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i64, 1, "start");
   Value   end   = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i64, 1, "end");
   if (node->token->is_nullable)
      guard_slice(node->token, start, end, llvm_extract(slice, 1, "arr.len"));
   TypeRef elem  = elem_type(arr, arr->Array.depth);
   Value   ptr   = llvm_gep(elem, data, &start, 1, "slice.data");
   Value   len   = LLVMBuildSub(ura.builder, end, start, "slice.len");
   node->token->llvm.elem = make_slice(arr, arr->Array.depth, ptr, len);
}

void code_gen_dot(Node *node) {
   Token *token = node->token;
   if (node->left->token->ret_type == STRUCT_CALL) {
      Value ptr = field_ptr(node);
      token->llvm.elem = llvm_load(llvm_type_of(token), ptr, token->name);
      return;
   }
   code_gen(node->left);
   token->llvm.elem = llvm_extract(node->left->token->llvm.elem, 1, "len");
}

void code_gen_access(Node *node) {
   if (node->right->token->type == RANGE) { code_gen_slice(node); return; }
   Value   ptr  = access_ptr(node);
   TypeRef elem = llvm_type_of(node->token);
   node->token->llvm.elem = llvm_load(elem, ptr, "idx");
}

void code_gen_array_lit(Node *node) {
   Token  *token = node->token;
   int     n     = node->children_count;
   int     depth = token->Array.depth;
   TypeRef elem  = elem_type(token, depth);
   Value   len   = const_i64(n);
   Value   data  = LLVMBuildArrayAlloca(ura.builder, elem, len, "arr");
   for (int i = 0; i < n; i++) {
      code_gen(node->children[i]);
      Value idx = const_i64(i);
      Value gep = llvm_gep(elem, data, &idx, 1, "arr.init");
      llvm_store(node->children[i]->token->llvm.elem, gep);
   }
   TypeRef slice = array_type(token, depth);
   Value   agg   = LLVMGetUndef(slice);
   agg = llvm_insert(agg, data, 0, "arr.ptr");
   agg = llvm_insert(agg, len,  1, "arr.len");
   token->llvm.elem = agg;
}

Value string_slice(Token *token, char *text) {
   Value str = llvm_string(text, "str");
   Value len = LLVMConstInt(ura.i64, strlen(text), 0);
   return make_slice(token, 1, str, len);
}

Value make_slice(Token *arr, int depth, Value data, Value len) {
   Value agg = LLVMGetUndef(array_type(arr, depth));
   agg = llvm_insert(agg, data, 0, "arr.ptr");
   agg = llvm_insert(agg, len,  1, "arr.len");
   return agg;
}

Value array_calloc(TypeRef elem, Value count, Value esz) {
   TypeRef cty = NULL;
   Value   fn  = lib_fn("calloc", &cty);
   if (!fn) return LLVMConstNull(pointer_to(elem));
   Value   mem = llvm_call(cty, fn, (Value[]){ count, esz }, 2, "heap");
   return LLVMBuildBitCast(ura.builder, mem, pointer_to(elem), "arr");
}

Value build_array(Token *arr, Value *dims, int depth, bool heap) {
   Value   n    = dims[0];
   TypeRef elem = elem_type(arr, depth);
   Value   esz  = LLVMConstInt(ura.i64, LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), elem), 0);
   if (depth == 1) {
      if (heap) return make_slice(arr, 1, array_calloc(elem, n, esz), n);
      Value data  = LLVMBuildArrayAlloca(ura.builder, elem, n, "arr");
      Value bytes = LLVMBuildMul(ura.builder, n, esz, "bytes");
      LLVMBuildMemSet(ura.builder, data, LLVMConstInt(ura.i8, 0, 0), bytes, 0);
      return make_slice(arr, 1, data, n);
   }
   TypeRef inner = elem;
   Value   data  = heap ? array_calloc(inner, n, esz)
                        : LLVMBuildArrayAlloca(ura.builder, inner, n, "arr");
   Value   fn    = here_func();
   Value   slot  = llvm_alloca(ura.i64, "i");
   llvm_store(const_i64(0), slot);
   Block   cond  = llvm_block(fn, "arr.cond");
   Block   body  = llvm_block(fn, "arr.body");
   Block   end   = llvm_block(fn, "arr.end");
   llvm_br(cond);
   llvm_at(cond);
   Value   i     = llvm_load(ura.i64, slot, "i");
   llvm_cond_br(llvm_icmp(LLVMIntSLT, i, n, "more"), body, end);
   llvm_at(body);
   Value   sub_arr = build_array(arr, dims + 1, depth - 1, heap);
   i = llvm_load(ura.i64, slot, "i");
   Value   gep = llvm_gep(inner, data, &i, 1, "arr.slot");
   llvm_store(sub_arr, gep);
   llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "next"), slot);
   llvm_br(cond);
   llvm_at(end);
   return make_slice(arr, depth, data, n);
}

void code_gen_array_ctor(Node *node) {
   Token *token = node->token;
   int    depth = token->Array.depth;
   Value *dims  = allocate(depth, sizeof(Value));
   for (int i = 0; i < depth; i++) {
      code_gen(node->children[i]);
      dims[i] = LLVMBuildIntCast2(ura.builder, node->children[i]->token->llvm.elem, ura.i64, 1, "n");
   }
   token->llvm.elem = build_array(token, dims, depth, token->is_heap);
   free(dims);
}

void free_array(Token *arr, Value slice, int depth) {
   Value data = llvm_extract(slice, 0, "arr.data");
   if (depth > 1) {
      Value   len   = llvm_extract(slice, 1, "arr.len");
      TypeRef inner = array_type(arr, depth - 1);
      Value   fn    = here_func();
      Value   slot  = llvm_alloca(ura.i64, "i");
      llvm_store(const_i64(0), slot);
      Block   cond  = llvm_block(fn, "free.cond");
      Block   body  = llvm_block(fn, "free.body");
      Block   end   = llvm_block(fn, "free.end");
      llvm_br(cond);
      llvm_at(cond);
      Value   i     = llvm_load(ura.i64, slot, "i");
      llvm_cond_br(llvm_icmp(LLVMIntSLT, i, len, "more"), body, end);
      llvm_at(body);
      Value   gep   = llvm_gep(inner, data, &i, 1, "free.slot");
      free_array(arr, llvm_load(inner, gep, "inner"), depth - 1);
      i = llvm_load(ura.i64, slot, "i");
      llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "next"), slot);
      llvm_br(cond);
      llvm_at(end);
   }
   Value   ptr = LLVMBuildBitCast(ura.builder, data, pointer_to(ura.i8), "free.ptr");
   TypeRef fty = NULL;
   Value   ffn = lib_fn("free", &fty);
   if (ffn) llvm_call(fty, ffn, (Value[]){ ptr }, 1, "");
}

void code_gen_typeof(Node *node) {
   Token *arg  = node->left->token;
   char  *name = arg->ret_type == STRUCT_CALL ? struct_name_of(arg)
               : type_name(arg->ret_type);
   node->token->llvm.elem = string_slice(node->token, name);
}

void code_gen_sizeof(Node *node) {
   TypeRef t = llvm_type_of(node->left->token);
   unsigned long long sz = LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), t);
   node->token->llvm.elem = const_i64(sz);
}

void code_gen_clean(Node *node) {
   Token  *arr   = node->left->token;
   Value   slot  = emit_place(node->left);
   TypeRef sty   = array_type(arr, arr->Array.depth);
   Value   slice = llvm_load(sty, slot, "arr");
   free_array(arr, slice, arr->Array.depth);
   llvm_store(LLVMConstNull(sty), slot);
}

void code_gen_literal(Node *node) {
   Token *token = node->token;
   switch (token->type) {
      case I32:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0); break;
      case BOOL:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Bool.value, 0); break;
      case CHARS:
         token->llvm.elem = string_slice(token, token->Chars.value);
         break;
      case NULL_LIT:
         token->llvm.elem = LLVMConstNull(llvm_type_of(token));
         break;
      case CHAR:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Char.value, 0); break;
      case F32: token->llvm.elem = LLVMConstReal(to_llvm_type(token->ret_type), token->Float.value); break;
   }
}

bool needs_drop(Node *def) {
   if (!def) return false;
   if (def->token->has_drop) return true;
   for (int i = 0; i < def->children_count; i++) {
      Token *field = def->children[i]->token;
      if (!is_field(field) || field->is_ref) continue;
      if (field->ret_type != STRUCT_CALL) continue;
      if (needs_drop(field->Struct.ptr)) return true;
   }
   return false;
}

Node *drop_target(Token *var) {
   if (var->is_ref || var->is_param || !var->llvm.elem) return NULL;
   if (var->ret_type != STRUCT_CALL) return NULL;
   Node *def = var->Struct.ptr;
   return needs_drop(def) ? def : NULL;
}

void emit_drop_value(Value ptr, Node *def) {
   if (!def) return;
   if (def->token->has_drop) {
      Node  *fn   = find_destructor(def);
      emit_signature(fn);
      Token *impl = fn->token;
      llvm_call(impl->llvm.func_type, impl->llvm.elem, &ptr, 1, "");
   }
   TypeRef sty = struct_type_of(def);
   for (int i = def->children_count - 1; i >= 0; i--) {
      Token *field = def->children[i]->token;
      if (!is_field(field) || field->is_ref) continue;
      if (field->ret_type != STRUCT_CALL) continue;
      Node *sub = field->Struct.ptr;
      if (!needs_drop(sub)) continue;
      Value idx[2] = { const_i32(0), const_i32(field->Struct.index) };
      Value slot   = llvm_gep(sty, ptr, idx, 2, field->name);
      emit_drop_value(slot, sub);
   }
}

void emit_drops(Node *scope, Token *keep) {
   if (!scope || LLVMGetBasicBlockTerminator(here_block())) return;
   for (int i = scope->variables_count - 1; i >= 0; i--) {
      Token *var = scope->variables[i];
      Node  *def = var == keep ? NULL : drop_target(var);
      if (def) emit_drop_value(var->llvm.elem, def);
   }
}

void emit_unwind(Node *stop, Token *keep) {
   for (int i = ura.scopes_count; i >= 1; i--) {
      Node *scope = ura.scopes[i];
      if (!scope) continue;
      emit_drops(scope, keep);
      bool at_fdec = scope->token->type == FDEC;
      if (stop ? scope == stop : at_fdec && !is_main(scope->token)) return;
   }
}

void scope_out() {
   emit_drops(ura.scope, NULL);
   exit_scope();
}

bool is_main(Token *token) {
   return token->type == FDEC && strcmp(token->name, "main") == 0;
}

void fill_os(Token *os, Value argc, Value argv) {
   TypeRef sty  = struct_type_of(os->Struct.ptr);
   Value   base = os->llvm.elem;
   llvm_store(argc, LLVMBuildStructGEP2(ura.builder, sty, base, 0, "os.argc"));

   TypeRef outer = LLVMStructGetTypeAtIndex(sty, 1);
   TypeRef strt  = LLVMGetElementType(LLVMStructGetTypeAtIndex(outer, 0));
   Value   n64   = LLVMBuildSExt(ura.builder, argc, ura.i64, "argc64");
   TypeRef cty   = NULL;
   Value   cfn   = lib_fn("calloc", &cty);
   Value   raw   = llvm_call(cty, cfn, (Value[]){ n64, const_i64(16) }, 2, "argvbuf");
   Value   buf   = LLVMBuildBitCast(ura.builder, raw, pointer_to(strt), "argv.buf");

   TypeRef lty   = NULL;
   Value   lfn   = lib_fn("strlen", &lty);
   Value   fn    = here_func();
   Block   cond  = llvm_block(fn, "os.cond");
   Block   body  = llvm_block(fn, "os.body");
   Block   end   = llvm_block(fn, "os.end");
   Value   slot  = llvm_alloca(ura.i64, "oi");
   llvm_store(const_i64(0), slot);
   llvm_br(cond);
   llvm_at(cond);
   Value i = llvm_load(ura.i64, slot, "i");
   llvm_cond_br(llvm_icmp(LLVMIntSLT, i, n64, "more"), body, end);
   llvm_at(body);
   TypeRef i8p = pointer_to(ura.i8);
   Value   src = llvm_load(i8p, LLVMBuildGEP2(ura.builder, i8p, argv, &i, 1, "ap"), "arg");
   Value   len = llvm_call(lty, lfn, (Value[]){ src }, 1, "alen");
   Value   sl  = llvm_insert(LLVMGetUndef(strt), src, 0, "a.ptr");
   sl          = llvm_insert(sl, len, 1, "a.len");
   llvm_store(sl, LLVMBuildGEP2(ura.builder, strt, buf, &i, 1, "slot"));
   llvm_store(llvm_binop(LLVMAdd, i, const_i64(1), "inc"), slot);
   llvm_br(cond);
   llvm_at(end);
   Value agg = llvm_insert(LLVMGetUndef(outer), buf, 0, "argv.ptr");
   agg       = llvm_insert(agg, n64, 1, "argv.len");
   llvm_store(agg, LLVMBuildStructGEP2(ura.builder, sty, base, 1, "os.argv"));
}

void init_os() {
   for (int i = 0; i < ura.head->children_count; i++) {
      Token *os = global_decl(ura.head->children[i]);
      if (!os || !os->name || strcmp(os->name, "os") != 0) continue;
      if (os->ret_type != STRUCT_CALL || !os->Struct.ptr) return;
      if (!os->used) return;
      Value fn = here_func();
      fill_os(os, LLVMGetParam(fn, 0), LLVMGetParam(fn, 1));
      return;
   }
}

void init_globals() {
   for (int i = 0; i < ura.head->children_count; i++) {
      Node *child = ura.head->children[i];
      if (child->token->type != ASSIGN || !global_decl(child)) continue;
      code_gen(child);
      drop_temps();
   }
}

void code_gen_fdec(Node *node) {
   Token *token = node->token;
   if (token->is_proto) return;
   Token *prev_ret = ura.fn_ret;
   ura.fn_ret = token;
   emit_signature(node);
   debug_enter_function(token);
   enter_scope(node);
   for (int i = 0; i < token->Fn.params_count; i++) {
      Token  *param = token->Fn.params[i];
      TypeRef pt    = llvm_type_of(param);
      if (param->is_ref) pt = pointer_to(pt);
      param->llvm.elem = llvm_alloca(pt, param->name);
      llvm_store(LLVMGetParam(token->llvm.elem, i), param->llvm.elem);
   }
   if (is_main(token)) { 
      init_os(); 
      init_globals(); 
   }
   for (int i = 0; i < node->children_count; i++) {
      set_debug_location(node->children[i]->token);
      code_gen(node->children[i]);
      drop_temps();
   }
   scope_out();
   if (is_main(token)) emit_drops(ura.head, NULL);
   if (!LLVMGetBasicBlockTerminator(here_block())) {
      if (token->ret_type == VOID) LLVMBuildRetVoid(ura.builder);
      else LLVMBuildRet(ura.builder, default_value(token));
   }
   debug_exit_function(token);
   ura.fn_ret = prev_ret;
}

void code_gen_id(Node *node) {
   Token *token = node->token;
   if (token->is_dec) {
      if (token->is_global) return;
      TypeRef t = llvm_type_of(token);
      if (token->is_ref) t = pointer_to(t);
      token->llvm.elem = llvm_alloca(t, token->name);
      Value init = token->is_ref ? LLVMConstNull(t) : default_value(token);
      llvm_store(init, token->llvm.elem);
      return;
   }
   Token  *decl = token->Decl.ptr;
   TypeRef t    = llvm_type_of(decl);
   if (!decl->is_ref) {
      token->llvm.elem = llvm_load(t, decl->llvm.elem, token->name);
      return;
   }
   Value ptr = llvm_load(pointer_to(t), decl->llvm.elem, "ref");
   if (token->is_nullable) guard_bound(token, ptr);
   token->llvm.elem = llvm_load(t, ptr, token->name);
}

Value decay_ptr(Type from, Type to, Value v) {
   if (from != ARRAY_TYPE || to == ARRAY_TYPE || !is_pointer(to))
      return v;
   return llvm_extract(v, 0, "arr.data");
}

void code_gen_fcall(Node *node) {
   Token *token    = node->token;
   bool   indirect = token->Fcall.var != NULL;
   Token *fn       = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   if (!indirect) emit_signature(token->Fcall.ptr);
   int    self = token->is_method_call ? 1 : 0;
   int    n    = node->children_count + self;
   Value *args = NULL;
   if (n > 0) {
      args = allocate(n, sizeof(Value));
      if (self) args[0] = struct_arg_ptr(node->left);
      for (int i = 0; i < node->children_count; i++) {
         Token *arg = node->children[i]->token;
         code_gen(node->children[i]);
         args[i + self] = arg->llvm.elem;
         bool extra = fn->is_variadic && i + self >= fn->Fn.params_count;
         Type want  = extra ? PTR : fn->Fn.params[i + self]->ret_type;
         args[i + self] = decay_ptr(arg->ret_type, want, args[i + self]);
         if (extra && arg->ret_type != ARRAY_TYPE)
            args[i + self] = promote(arg->ret_type, args[i + self]);
      }
   }
   if (indirect) {
      TypeRef ptr_type = llvm_type_of(fn);
      Value   fn_ptr   = llvm_load(ptr_type, fn->llvm.elem, "fn");
      guard_nonnull(token, fn_ptr);
      char   *name     = fn->Fn.ret->ret_type == VOID ? "" : "call";
      token->llvm.elem = llvm_call(LLVMGetElementType(ptr_type), fn_ptr, args, n, name);
   } else {
      char *name       = fn->ret_type == VOID ? "" : "call";
      token->llvm.elem = llvm_call(fn->llvm.func_type, fn->llvm.elem, args, n, name);
   }
   free(args);
}

void push_temp(Value ptr, Token *of) {
   if (!needs_drop(of->Struct.ptr)) return;
   Token *temp     = new_token(ID, of->indent);
   temp->ret_type  = STRUCT_CALL;
   temp->Struct    = of->Struct;
   temp->llvm.elem = ptr;
   resize_array(ura.temps, Token *);
   ura.temps[ura.temps_count++] = temp;
}

void drop_temps() {
   if (!LLVMGetBasicBlockTerminator(here_block()))
      for (int i = ura.temps_count - 1; i >= 0; i--)
         emit_drop_value(ura.temps[i]->llvm.elem, ura.temps[i]->Struct.ptr);
   ura.temps_count = 0;
}

void code_gen_body(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      code_gen(node->children[i]);
      drop_temps();
      if (LLVMGetBasicBlockTerminator(here_block())) break;
   }
}

void code_gen_loop(Node *node) {
   Value fn   = here_func();
   Block body = llvm_block(fn, "loop.body");
   Block end  = llvm_block(fn, "loop.end");
   node->token->llvm.start = body;
   node->token->llvm.end   = end;
   llvm_br(body);
   llvm_at(body);
   enter_scope(node);
   code_gen_body(node);
   scope_out();
   if (!LLVMGetBasicBlockTerminator(here_block()))
      llvm_br(body);
   llvm_at(end);
}

void code_gen_for_array(Node *node) {
   Token  *var  = node->left->token;
   bool    ref  = node->token->is_ref;
   Token  *arr  = node->right->token;
   code_gen(node->right);
   Value   slice = arr->llvm.elem;
   Value   data  = llvm_extract(slice, 0, "arr.data");
   Value   len   = llvm_extract(slice, 1, "arr.len");
   TypeRef elem  = elem_type(arr, arr->Array.depth);
   Value   fn    = here_func();
   Value   idx   = llvm_alloca(ura.i64, "idx");
   llvm_store(const_i64(0), idx);
   Value   xslot = ref ? NULL : llvm_alloca(elem, var->name);
   if (!ref) var->llvm.elem = xslot;
   Block   cond  = llvm_block(fn, "for.cond");
   Block   body  = llvm_block(fn, "for.body");
   Block   inc   = llvm_block(fn, "for.inc");
   Block   end   = llvm_block(fn, "for.end");
   node->token->llvm.start = inc;
   node->token->llvm.end   = end;
   llvm_br(cond);
   llvm_at(cond);
   Value   i = llvm_load(ura.i64, idx, "i");
   llvm_cond_br(llvm_icmp(LLVMIntSLT, i, len, "more"), body, end);
   llvm_at(body);
   Value   gep = llvm_gep(elem, data, &i, 1, "elem");
   if (ref) var->llvm.elem = gep;
   else     llvm_store(llvm_load(elem, gep, "x"), xslot);
   enter_scope(node);
   code_gen_body(node);
   scope_out();
   if (!LLVMGetBasicBlockTerminator(here_block()))
      llvm_br(inc);
   llvm_at(inc);
   Value   iv = llvm_load(ura.i64, idx, "i");
   llvm_store(LLVMBuildAdd(ura.builder, iv, const_i64(1), "next"), idx);
   llvm_br(cond);
   llvm_at(end);
}

void code_gen_for(Node *node) {
   if (node->right->token->type != RANGE) { code_gen_for_array(node); return; }
   Token *var   = node->left->token;
   Node  *range = node->right;
   code_gen(range->left);
   code_gen(range->right);
   Value a    = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i32, 1, "a");
   Value b    = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i32, 1, "b");
   Value asc  = llvm_icmp(LLVMIntSLT, a, b, "asc");
   Value mag  = const_i32(1);
   if (range->children_count) {
      code_gen(range->children[0]);
      Value raw = range->children[0]->token->llvm.elem;
      mag = LLVMBuildIntCast2(ura.builder, raw, ura.i32, 1, "by");
   }
   Value down = LLVMBuildNeg(ura.builder, mag, "by.neg");
   Value step = LLVMBuildSelect(ura.builder, asc, mag, down, "step");
   Value fn   = here_func();
   Value slot = llvm_alloca(ura.i32, var->name);
   llvm_store(a, slot);
   var->llvm.elem = slot;
   Block cond = llvm_block(fn, "for.cond");
   Block body = llvm_block(fn, "for.body");
   Block inc  = llvm_block(fn, "for.inc");
   Block end  = llvm_block(fn, "for.end");
   node->token->llvm.start = inc;
   node->token->llvm.end   = end;
   llvm_br(cond);
   llvm_at(cond);
   Value i = llvm_load(ura.i32, slot, var->name);
   // '!=' would overshoot and never terminate once the step exceeds 1
   Value more = LLVMBuildSelect(ura.builder, asc,
                                llvm_icmp(LLVMIntSLT, i, b, "lt"),
                                llvm_icmp(LLVMIntSGT, i, b, "gt"), "more");
   llvm_cond_br(more, body, end);
   llvm_at(body);
   enter_scope(node);
   code_gen_body(node);
   scope_out();
   if (!LLVMGetBasicBlockTerminator(here_block()))
      llvm_br(inc);
   llvm_at(inc);
   Value iv = llvm_load(ura.i32, slot, var->name);
   llvm_store(LLVMBuildAdd(ura.builder, iv, step, "next"), slot);
   llvm_br(cond);
   llvm_at(end);
}

void code_gen_while(Node *node) {
   Value fn   = here_func();
   Block cond = llvm_block(fn, "while.cond");
   Block body = llvm_block(fn, "while.body");
   Block end  = llvm_block(fn, "while.end");
   node->token->llvm.start = cond;   
   node->token->llvm.end   = end;    
   llvm_br(cond);
   llvm_at(cond);
   code_gen(node->left);
   llvm_cond_br(node->left->token->llvm.elem, body, end);
   llvm_at(body);
   enter_scope(node);
   code_gen_body(node);
   scope_out();
   if (!LLVMGetBasicBlockTerminator(here_block()))
      llvm_br(cond);
   llvm_at(end);
}

void code_gen_match(Node *node) {
   Value fn   = here_func();
   Block end  = llvm_block(fn, "match.end");
   node->token->llvm.end = end;                       // break target
   code_gen(node->left);                              
   Value subject = node->left->token->llvm.elem;
   bool  fp      = is_float(node->left->token->ret_type);
   enter_scope(node);
   for (int i = 0; i < node->children_count; i++) {
      Node *branch = node->children[i];
      if (branch->token->type == DEFAULT) {
         enter_scope(branch); code_gen_body(branch); scope_out();
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(end);
         break;
      }
      Block body = llvm_block(fn, "case.body");
      Block next = i + 1 < node->children_count ? llvm_block(fn, "case.next") : end;
      Value cond = NULL;
      for (int j = 0; j < branch->left->children_count; j++) {
         code_gen(branch->left->children[j]);
         Value val = branch->left->children[j]->token->llvm.elem;
         Value eq  = fp ? llvm_fcmp(LLVMRealOEQ, subject, val, "feq")
                        : llvm_icmp(LLVMIntEQ, subject, val, "eq");
         cond = cond ? LLVMBuildOr(ura.builder, cond, eq, "case.or") : eq;
      }
      llvm_cond_br(cond, body, next);
      llvm_at(body);
      enter_scope(branch); code_gen_body(branch); scope_out();
      if (!LLVMGetBasicBlockTerminator(here_block()))
         llvm_br(end);
      llvm_at(next);
   }
   scope_out();
   llvm_at(end);
}

void code_gen_if(Node *node) {
   Value fn  = here_func();
   Block end = llvm_block(fn, "endif");
   for (Node *cur = node; cur; cur = cur->right) {
      if (cur->token->type == ELSE) {
         enter_scope(cur);
         code_gen_body(cur);
         scope_out();
         if (!LLVMGetBasicBlockTerminator(here_block()))
            llvm_br(end);
         break;
      }
		// elif
      Block body = llvm_block(fn, "then");
      Block next = cur->right ? llvm_block(fn, "next") : end;
      code_gen(cur->left);
      llvm_cond_br(cur->left->token->llvm.elem, body, next);
      llvm_at(body);
      enter_scope(cur);
      code_gen_body(cur);
      scope_out();
      if (!LLVMGetBasicBlockTerminator(here_block()))
         llvm_br(end);
      llvm_at(next);
   }
   llvm_at(end);
}

Type category(Node *n) {
   Token *t = n->token;
   if (t->ret_type == ARRAY_TYPE)             return CAT_SLICE;
   if (t->type == REF || t->type == NULL_LIT) return CAT_REF;
   if (t->type == FCALL && t->is_ref)         return CAT_REF;
   return CAT_VALUE;
}

Value emit_value(Node *n) {
   code_gen(n);
   return n->token->llvm.elem;
}

Value emit_place(Node *n) {
   Token *t = n->token;
   if (t->type == ACCESS) return access_ptr(n);
   if (t->type == DOT)    return field_ptr(n);
   if (t->is_dec) {
      if (t->is_global) return t->llvm.elem;
      TypeRef ty = llvm_type_of(t);
      if (t->is_ref) ty = pointer_to(ty);
      return t->llvm.elem = llvm_alloca(ty, t->name);
   }
   Token *decl = t->Decl.ptr;
   if (decl->is_ref) {
      TypeRef pty = pointer_to(llvm_type_of(decl));
      Value   ptr = llvm_load(pty, decl->llvm.elem, "ref");
      if (t->is_nullable) guard_bound(t, ptr);
      return ptr;
   }
   return decl->llvm.elem;
}

Value emit_ref(Node *n) {
   Token *t = n->token;
   if (t->type == REF)      return emit_ref(n->left);
   if (t->type == NULL_LIT) return LLVMConstNull(pointer_to(llvm_type_of(t)));
   if (t->type == FCALL)  { code_gen(n); return t->llvm.elem; }
   if (t->type == DOT && t->is_ref) {
      TypeRef pty = pointer_to(llvm_type_of(t));
      return llvm_load(pty, field_ptr(n), "ref");
   }
   bool ref_var = t->type == ID && t->Decl.ptr && t->Decl.ptr->is_ref;
   if (ref_var) {
      TypeRef pty = pointer_to(llvm_type_of(t->Decl.ptr));
      return llvm_load(pty, t->Decl.ptr->llvm.elem, "ref");
   }
   return emit_place(n);
}

Value emit_slot(Node *n) {
   Token *t = n->token;
   if (t->type == DOT) return field_ptr(n);
   Token *decl = t->is_dec ? t : t->Decl.ptr;
   return decl->llvm.elem;
}

void code_gen_assign(Node *node) {
   Token *token = node->token;
   if (token->Fcall.ptr) {
      code_gen_operator(node);
      return;
   }
   if (token->kind == REF_REBIND) {
      Value ptr = emit_ref(node->right);
      llvm_store(ptr, emit_slot(node->left));
      token->llvm.elem = ptr;
      return;
   }
   Value dest   = emit_place(node->left);
   Node *rhs    = node->right;
   bool  as_ref = node->left->token->is_ref && category(rhs) == CAT_REF;
   token->llvm.elem = as_ref ? emit_ref(rhs) : emit_value(rhs);
   llvm_store(token->llvm.elem, dest);
}

void code_gen_operator(Node *node) {
   Token *token = node->token;
   Node  *fn    = token->Fcall.ptr;
   emit_signature(fn);
   Value  self   = struct_arg_ptr(node->left);
   Token *param  = fn->token->Fn.params_count > 1 ? fn->token->Fn.params[1] : NULL;
   bool   by_ref = param && param->is_ref;
   Type   kind     = node->right->token->type;
   bool   spelled  = kind == REF;                 
   bool   has_addr = includes(kind, ID, DOT, ACCESS, 0);
   Value  arg;
   if (spelled) {
      code_gen(node->right);
      Value addr = node->right->token->llvm.elem;
      arg = by_ref ? addr : llvm_load(llvm_type_of(param), addr, "deref");
   } else if (by_ref && has_addr) {
      arg = emit_place(node->right);
   } else {
      code_gen(node->right);
      arg = node->right->token->llvm.elem;
      if (by_ref) {
         Value slot = llvm_alloca(LLVMTypeOf(arg), "op.tmp");
         llvm_store(arg, slot);
         arg = slot;
      }
   }
   Value args[2] = { self, arg };
   char *name = fn->token->ret_type == VOID ? "" : "op";
   token->llvm.elem = llvm_call(fn->token->llvm.func_type, fn->token->llvm.elem, args, 2, name);
}

Value opt_ptr(Token *token, Value v) {
   return token->ret_type == ARRAY_TYPE ? llvm_extract(v, 0, "opt.ptr") : v;
}

void code_gen_fallback(Node *node) {
   code_gen(node->left);
   code_gen(node->right);
   Value left   = node->left->token->llvm.elem;
   Value right  = node->right->token->llvm.elem;
   Value ptr    = opt_ptr(node->left->token, left);
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = llvm_icmp(LLVMIntEQ, ptr, null, "isnull");
   node->token->llvm.elem = LLVMBuildSelect(ura.builder, isnull, right, left, "fallback");
}

void code_gen_binop(Node *node) {
   Token *token = node->token;
   if (token->Fcall.ptr) {
      code_gen_operator(node);
      return;
   }
   int p = token->type == EQUAL ? LLVMIntEQ : LLVMIntNE;
   if (token->kind == CMP_REF) {
      Value l = emit_ref(node->left);
      Value r = emit_ref(node->right);
      token->llvm.elem = llvm_icmp(p, l, r, "refcmp");
      return;
   }
   code_gen(node->left);
   code_gen(node->right);
   Value left  = node->left->token->llvm.elem;
   Value right = node->right->token->llvm.elem;
   Value res   = NULL;
   if (token->kind == CMP_SLICE) {
      Value l = opt_ptr(node->left->token, left);
      Value r = opt_ptr(node->right->token, right);
      token->llvm.elem = llvm_icmp(p, l, r, "nullcmp");
      return;
   }
   Type  lt    = node->left->token->ret_type;
   bool  fp    = is_float(lt);
   bool  un    = is_unsigned(lt);
   if (includes(token->type, DIV, MOD, 0)) guard_nonzero(token, right);
#define ARITH(fop, sop, uop, fname, iname) res = fp \
   ? llvm_binop(fop, left, right, fname) \
   : llvm_binop(un ? uop : sop, left, right, iname)
#define CMP(fpred, spred, upred, fname, iname) res = fp \
   ? llvm_fcmp(fpred, left, right, fname) \
   : llvm_icmp(un ? upred : spred, left, right, iname)
   switch (token->type) {
      case ADD: ARITH(LLVMFAdd, LLVMAdd,  LLVMAdd,  "fadd", "add"); break;
      case SUB: ARITH(LLVMFSub, LLVMSub,  LLVMSub,  "fsub", "sub"); break;
      case MUL: ARITH(LLVMFMul, LLVMMul,  LLVMMul,  "fmul", "mul"); break;
      case DIV: ARITH(LLVMFDiv, LLVMSDiv, LLVMUDiv, "fdiv", "div"); break;
      case MOD: ARITH(LLVMFRem, LLVMSRem, LLVMURem, "frem", "mod"); break;

      case EQUAL:       CMP(LLVMRealOEQ, LLVMIntEQ,  LLVMIntEQ,  "feq", "eq"); break;
      case NOT_EQUAL:   CMP(LLVMRealUNE, LLVMIntNE,  LLVMIntNE,  "fne", "ne"); break;
      case LESS:        CMP(LLVMRealOLT, LLVMIntSLT, LLVMIntULT, "flt", "lt"); break;
      case GREAT:       CMP(LLVMRealOGT, LLVMIntSGT, LLVMIntUGT, "fgt", "gt"); break;
      case LESS_EQUAL:  CMP(LLVMRealOLE, LLVMIntSLE, LLVMIntULE, "fle", "le"); break;
      case GREAT_EQUAL: CMP(LLVMRealOGE, LLVMIntSGE, LLVMIntUGE, "fge", "ge"); break;

      case AND:    res = llvm_binop(LLVMAnd,  left, right, "and");  break;
      case OR:     res = llvm_binop(LLVMOr,   left, right, "or");   break;
      case BAND:   res = llvm_binop(LLVMAnd,  left, right, "band"); break;
      case BOR:    res = llvm_binop(LLVMOr,   left, right, "bor");  break;
      case BXOR:   res = llvm_binop(LLVMXor,  left, right, "bxor"); break;
      case LSHIFT: res = llvm_binop(LLVMShl,  left, right, "shl");  break;
      case RSHIFT: res = llvm_binop(un ? LLVMLShr : LLVMAShr, left, right, "shr"); break;
      default: break;
   }
#undef ARITH
#undef CMP
   token->llvm.elem = res;
}

void code_gen_compound(Node *node) {
   if (node->token->Fcall.ptr) { 
      code_gen_operator(node); 
      return; 
   }
   Value   dest = emit_place(node->left);
   code_gen(node->right);
   Type    op      = node->token->type;
   Value   right   = node->right->token->llvm.elem;
   TypeRef type    = to_llvm_type(node->left->token->ret_type);
   Value   current = llvm_load(type, dest, "cur");
   Value   res     = right;
   Type    lt      = node->left->token->ret_type;
   bool    fp      = is_float(lt);
   bool    un      = is_unsigned(lt);
   bool    divides = includes(op, DIV_ASSIGN, MOD_ASSIGN, 0);
   if (divides) guard_nonzero(node->token, right);
#define ARITH(fop, sop, uop, fname, iname) res = fp \
   ? llvm_binop(fop, current, right, fname) \
   : llvm_binop(un ? uop : sop, current, right, iname)
   switch (op) {
      case ADD_ASSIGN: ARITH(LLVMFAdd, LLVMAdd,  LLVMAdd,  "fadd", "add"); break;
      case SUB_ASSIGN: ARITH(LLVMFSub, LLVMSub,  LLVMSub,  "fsub", "sub"); break;
      case MUL_ASSIGN: ARITH(LLVMFMul, LLVMMul,  LLVMMul,  "fmul", "mul"); break;
      case DIV_ASSIGN: ARITH(LLVMFDiv, LLVMSDiv, LLVMUDiv, "fdiv", "div"); break;
      case MOD_ASSIGN: ARITH(LLVMFRem, LLVMSRem, LLVMURem, "frem", "mod"); break;

      case BAND_ASSIGN:   res = llvm_binop(LLVMAnd, current, right, "band"); break;
      case BOR_ASSIGN:    res = llvm_binop(LLVMOr,  current, right, "bor");  break;
      case BXOR_ASSIGN:   res = llvm_binop(LLVMXor, current, right, "bxor"); break;
      case LSHIFT_ASSIGN: res = llvm_binop(LLVMShl, current, right, "shl");  break;
      case RSHIFT_ASSIGN: res = llvm_binop(un ? LLVMLShr : LLVMAShr, current, right, "shr"); break;
      default: break;
   }
#undef ARITH
   llvm_store(res, dest);
   node->token->llvm.elem = res;
}

Value emit_printf(char *fmt, Value *args, int n) {
   TypeRef printf_ty = NULL;
   Value   printf_fn = lib_fn("printf", &printf_ty);
   if (!printf_fn) return NULL;
   Value  *call      = allocate(n + 1, sizeof(Value));
   call[0]           = llvm_string(fmt, "fmt");
   if (n) memcpy(call + 1, args, n * sizeof(Value));
   Value res = llvm_call(printf_ty, printf_fn, call, n + 1, "");
   free(call);
   return res;
}

Value print_adapt(Type type, Value v, char **spec) {
   switch (type) {
      case I32:   *spec = "%d";   return v;
      case I64:   *spec = "%lld"; return v;
      case U32:   *spec = "%u";   return v;
      case U64:   *spec = "%llu"; return v;
      case PTR:   *spec = "%s";   return v;
      case I8: case I16:
         *spec = "%d";
         return LLVMBuildSExt(ura.builder, v, ura.i32, "s2i");
      case U8: case U16:
         *spec = "%u";
         return LLVMBuildZExt(ura.builder, v, ura.i32, "u2i");
      case CHAR:
         *spec = "%c";
         return LLVMBuildSExt(ura.builder, v, ura.i32, "c2i");
      case F32:
         *spec = "%f";
         return LLVMBuildFPExt(ura.builder, v, ura.f64, "f2d");
      case F64: *spec = "%f"; return v;
      case BOOL: {
         *spec    = "%s";
         Value ts = llvm_string("True", "true_str");
         Value fs = llvm_string("False", "false_str");
         return LLVMBuildSelect(ura.builder, v, ts, fs, "bool_str");
      }
      default: *spec = "?"; return NULL;
   }
}

int type_id(Node *def) {
   static int next = 1;
   Token *token = def->token;
   if (!token->Struct.index) token->Struct.index = next++;
   return token->Struct.index;
}

TypeRef out_frame_type() {
   TypeRef t = LLVMGetTypeByName(ura.module, "__out_frame");
   if (t) return t;
   t = LLVMStructCreateNamed(ura.context, "__out_frame");
   TypeRef body[3] = { pointer_to(ura.i8), ura.i32, pointer_to(t) };
   LLVMStructSetBody(t, body, 3, 0);
   return t;
}

void emit_out_call(Node *def, Value ptr, Value frame) {
   Node *printer = find_printer(def);
   if (printer) {
      emit_printer_call(printer, ptr);
      return;
   }
   Value   fn  = struct_printer(def);
   TypeRef fty = def->token->llvm.func_type;
   llvm_call(fty, fn, (Value[]){ ptr, frame }, 2, "");
}

void emit_out_scalar(Token *info, Value slot) {
   char *spec = NULL;
   Value v    = llvm_load(llvm_type_of(info), slot, "f");
   v          = print_adapt(info->ret_type, v, &spec);
   if (!v) { emit_printf("?", NULL, 0); return; }
   emit_printf(spec, (Value[]){ v }, 1);
}

void emit_out_array(Token *field, Value slot, Value frame, int depth) {
   Value   slice = llvm_load(array_type(field, depth), slot, "arr");
   Value   data  = llvm_extract(slice, 0, "arr.data");
   Value   len   = llvm_extract(slice, 1, "arr.len");
   if (depth == 1 && field->Array.sub_type == CHAR) {
      Value n = LLVMBuildTrunc(ura.builder, len, ura.i32, "len32");
      emit_printf("%.*s", (Value[]){ n, data }, 2);
      return;
   }
   TypeRef ety   = elem_type(field, depth);
   Value   fn    = here_func();
   Value   slot_i = llvm_alloca(ura.i64, "oi");
   llvm_store(const_i64(0), slot_i);
   emit_printf("[", NULL, 0);
   Block cond = llvm_block(fn, "out.arr.cond");
   Block body = llvm_block(fn, "out.arr.body");
   Block sep  = llvm_block(fn, "out.arr.sep");
   Block item = llvm_block(fn, "out.arr.item");
   Block end  = llvm_block(fn, "out.arr.end");
   llvm_br(cond);
   llvm_at(cond);
   Value i = llvm_load(ura.i64, slot_i, "i");
   llvm_cond_br(llvm_icmp(LLVMIntSLT, i, len, "more"), body, end);
   llvm_at(body);
   llvm_cond_br(llvm_icmp(LLVMIntSGT, i, const_i64(0), "notfirst"), sep, item);
   llvm_at(sep);
   emit_printf(", ", NULL, 0);
   llvm_br(item);
   llvm_at(item);
   Value at = llvm_gep(ety, data, &i, 1, "at");
   if (depth > 1)
      emit_out_array(field, at, frame, depth - 1);
   else if (field->Array.sub_type == STRUCT_CALL)
      emit_out_call(field->Array.struct_ptr, at, frame);
   else {
      char *spec = NULL;
      Value v    = print_adapt(field->Array.sub_type,
                               llvm_load(ety, at, "e"), &spec);
      if (v) emit_printf(spec, (Value[]){ v }, 1);
      else   emit_printf("?", NULL, 0);
   }
   llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "n"), slot_i);
   llvm_br(cond);
   llvm_at(end);
   emit_printf("]", NULL, 0);
}

void emit_out_field(Token *field, Value slot, Value frame) {
   if (field->is_ref) {
      TypeRef tty = struct_type_of(field->Struct.ptr);
      Value   ptr = llvm_load(pointer_to(tty), slot, "ref");
      Value   fn  = here_func();
      Block   nb  = llvm_block(fn, "out.null");
      Block   vb  = llvm_block(fn, "out.ref");
      Block   cb  = llvm_block(fn, "out.refend");
      Value   iv  = LLVMBuildPtrToInt(ura.builder, ptr, ura.i64, "p2i");
      llvm_cond_br(llvm_icmp(LLVMIntEQ, iv, const_i64(0), "isnull"), nb, vb);
      llvm_at(nb);
      emit_printf("null", NULL, 0);
      llvm_br(cb);
      llvm_at(vb);
      emit_printf("ref ", NULL, 0);
      emit_out_call(field->Struct.ptr, ptr, frame);
      llvm_br(cb);
      llvm_at(cb);
      return;
   }
   if (field->ret_type == STRUCT_CALL) {
      emit_out_call(field->Struct.ptr, slot, frame);
      return;
   }
   if (field->ret_type == ARRAY_TYPE) {
      emit_out_array(field, slot, frame, field->Array.depth);
      return;
   }
   emit_out_scalar(field, slot);
}

Value struct_printer(Node *def) {
   Token *token = def->token;
   if (token->llvm.elem) return token->llvm.elem;
   TypeRef sty  = struct_type_of(def);
   TypeRef frt  = out_frame_type();
   TypeRef fty  = LLVMFunctionType(ura.vd,
                     (TypeRef[]){ pointer_to(sty), pointer_to(frt) }, 2, 0);
   char   *name = format("__out_%s", token->name);
   Value   fn   = LLVMAddFunction(ura.module, name, fty);
   token->llvm.elem      = fn;
   token->llvm.func_type = fty;
   free(name);

   Block prev   = LLVMGetInsertBlock(ura.builder);
   Block entry  = llvm_block(fn, "entry");
   llvm_at(entry);
   Value self   = LLVMGetParam(fn, 0);
   Value parent = LLVMGetParam(fn, 1);
   Value me     = LLVMBuildBitCast(ura.builder, self, pointer_to(ura.i8), "me");

   Value walk  = llvm_alloca(pointer_to(frt), "walk");
   llvm_store(parent, walk);
   Block sc    = llvm_block(fn, "seen.cond");
   Block sb    = llvm_block(fn, "seen.body");
   Block hit   = llvm_block(fn, "seen.hit");
   Block miss  = llvm_block(fn, "seen.next");
   Block fresh = llvm_block(fn, "seen.fresh");
   llvm_br(sc);
   llvm_at(sc);
   Value node = llvm_load(pointer_to(frt), walk, "q");
   Value done = llvm_icmp(LLVMIntEQ,
                   LLVMBuildPtrToInt(ura.builder, node, ura.i64, "q2i"),
                   const_i64(0), "atroot");
   llvm_cond_br(done, fresh, sb);
   llvm_at(sb);
   Value pidx[2] = { const_i32(0), const_i32(0) };
   Value tidx[2] = { const_i32(0), const_i32(1) };
   Value nidx[2] = { const_i32(0), const_i32(2) };
   Value myid = const_i32(type_id(def));
   Value held = llvm_load(pointer_to(ura.i8),
                   llvm_gep(frt, node, pidx, 2, "q.ptr"), "held");
   Value hid  = llvm_load(ura.i32,
                   llvm_gep(frt, node, tidx, 2, "q.ty"), "heldty");
   Value hit_ptr = llvm_icmp(LLVMIntEQ,
                   LLVMBuildPtrToInt(ura.builder, held, ura.i64, "h2i"),
                   LLVMBuildPtrToInt(ura.builder, me, ura.i64, "m2i"), "sameptr");
   Value hit_ty  = llvm_icmp(LLVMIntEQ, hid, myid, "samety");
   Value same    = llvm_binop(LLVMAnd, hit_ptr, hit_ty, "same");
   llvm_cond_br(same, hit, miss);
   llvm_at(hit);
   emit_printf("[Circular]", NULL, 0);
   LLVMBuildRetVoid(ura.builder);
   llvm_at(miss);
   llvm_store(llvm_load(pointer_to(frt),
                 llvm_gep(frt, node, nidx, 2, "q.prev"), "up"), walk);
   llvm_br(sc);

   llvm_at(fresh);
   Value frame = llvm_alloca(frt, "frame");
   llvm_store(me,     llvm_gep(frt, frame, pidx, 2, "f.ptr"));
   llvm_store(myid,   llvm_gep(frt, frame, tidx, 2, "f.ty"));
   llvm_store(parent, llvm_gep(frt, frame, nidx, 2, "f.prev"));

   char *open = format("%s{", token->name);
   emit_printf(open, NULL, 0);
   free(open);
   int shown = 0;
   for (int i = 0; i < def->children_count; i++) {
      Token *field = def->children[i]->token;
      if (!is_field(field)) continue;
      char *label = format(shown ? ", %s: " : "%s: ", field->name);
      emit_printf(label, NULL, 0);
      free(label);
      shown++;
      Value idx[2] = { const_i32(0), const_i32(field->Struct.index) };
      emit_out_field(field, llvm_gep(sty, self, idx, 2, field->name), frame);
   }
   emit_printf("}", NULL, 0);
   LLVMBuildRetVoid(ura.builder);
   if (prev) llvm_at(prev);
   return fn;
}

Value struct_arg_ptr(Node *arg) {
   Token *token = arg->token;
   if (includes(token->type, ID, ACCESS, DOT, 0) && !token->is_dec) {
      Value slot = emit_place(arg);
      if (token->type != DOT || !token->is_ref) return slot;
      TypeRef sty = pointer_to(struct_type_of(token->Struct.ptr));
      Value   ptr = llvm_load(sty, slot, "ref");
      if (token->is_nullable) guard_bound(token, ptr);
      return ptr;
   }
   code_gen(arg);
   Value tmp = llvm_alloca(struct_type_of(token->Struct.ptr), "out.tmp");
   llvm_store(token->llvm.elem, tmp);
   push_temp(tmp, token);
   return tmp;
}

void emit_printer_call(Node *printer, Value self) {
   Token *fn = printer->token;
   emit_signature(printer);
   Value slice = llvm_call(fn->llvm.func_type, fn->llvm.elem, &self, 1, "out");
   Value len   = llvm_extract(slice, 1, "out.len");
   Value data  = llvm_extract(slice, 0, "out.data");
   Value n     = LLVMBuildTrunc(ura.builder, len, ura.i32, "len32");
   emit_printf("%.*s", (Value[]){ n, data }, 2);
}

void code_gen_output(Node *node) {
   char  *fmt   = allocate(node->children_count * 8 + 16, 1);
   int    fc    = 0;
   Value *args  = allocate(node->children_count * 2 + 1, sizeof(Value));
   int    nargs = 0;
   Value  last  = NULL;

   for (int i = 0; i < node->children_count; i++) {
      Node  *arg   = node->children[i];
      Token *token = arg->token;
      if (token->ret_type == STRUCT_CALL) {
         if (fc) { fmt[fc] = 0; last = emit_printf(fmt, args, nargs); }
         fc    = 0;
         nargs = 0;
         emit_out_call(token->Struct.ptr, struct_arg_ptr(arg),
                       LLVMConstNull(pointer_to(out_frame_type())));
         continue;
      }
      code_gen(arg);
      if (is_string(token)) {
         Value len = llvm_extract(token->llvm.elem, 1, "str.len");
         args[nargs++] = LLVMBuildTrunc(ura.builder, len, ura.i32, "len32");
         args[nargs++] = llvm_extract(token->llvm.elem, 0, "str.data");
         for (char *s = "%.*s"; *s; s++) fmt[fc++] = *s;
         continue;
      }
      if (token->ret_type == ARRAY_TYPE) {
         if (fc) { fmt[fc] = 0; last = emit_printf(fmt, args, nargs); }
         fc    = 0;
         nargs = 0;
         int     depth = token->Array.depth;
         TypeRef at    = array_type(token, depth);
         Value   slot  = LLVMBuildAlloca(ura.builder, at, "arr.tmp");
         llvm_store(token->llvm.elem, slot);
         emit_out_array(token, slot, LLVMConstNull(pointer_to(out_frame_type())), depth);
         continue;
      }
      char *spec = NULL;
      Value v    = print_adapt(token->ret_type, token->llvm.elem, &spec);
      if (!v) { fmt[fc++] = '?'; continue; }
      for (char *s = spec; *s; s++) fmt[fc++] = *s;
      args[nargs++] = v;
   }
   if (fc) { fmt[fc] = 0; last = emit_printf(fmt, args, nargs); }
   node->token->llvm.elem = last;
   free(fmt);
   free(args);
}

void code_gen(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:       code_gen_fdec(node);       break;
      case ID:         code_gen_id(node);         break;
      case FCALL:      code_gen_fcall(node);      break;
      case OUTPUT:     code_gen_output(node);     break;
      case ARRAY_LIT:  code_gen_array_lit(node);  break;
      case ARRAY:      code_gen_array_ctor(node); break;
      case ACCESS:     code_gen_access(node);     break;
      case DOT:        code_gen_dot(node);        break;
      case TYPEOF:     code_gen_typeof(node);     break;
      case SIZEOF:     code_gen_sizeof(node);     break;
      case CLEAN:      code_gen_clean(node);      break;
      case IF:         code_gen_if(node);         break;
      case WHILE:      code_gen_while(node);      break;
      case LOOP:       code_gen_loop(node);       break;
      case FOR:        code_gen_for(node);        break;
      case MATCH:      code_gen_match(node);      break;
      case ASSIGN:     code_gen_assign(node);     break;
      case STRUCT_DEF:
         struct_type_of(node);
         for (int i = 0; i < node->children_count; i++)
            if (node->children[i]->token->type == FDEC)
               code_gen(node->children[i]);
         break;
      case ENUM_DEF: break;

      case REF:      token->llvm.elem = emit_ref(node->left);   break;
      case BREAK: {
         drop_temps();
         emit_unwind(node->left, NULL);
         llvm_br(node->left->token->llvm.end);
         break;
      }
      case CONTINUE: {
         drop_temps();
         emit_unwind(node->left, NULL);
         llvm_br(node->left->token->llvm.start);
         break;
      }
      case I32: case BOOL: case CHARS:
      case CHAR: case F32: case NULL_LIT: {
         code_gen_literal(node);
         break;
      }
      case FN_TYPE: {
         emit_signature(token->Fcall.ptr);
         token->llvm.elem = token->Fcall.ptr->token->llvm.elem;
         break;
      }
      case RETURN: {
         if (!node->left) {
            drop_temps();
            emit_unwind(NULL, NULL);
            token->llvm.elem = LLVMBuildRetVoid(ura.builder);
            break;
         }
         Token *out  = node->left->token;
         bool   ref  = ura.fn_ret && ura.fn_ret->is_ref;
         Value  val  = ref ? emit_ref(node->left) : emit_value(node->left);
         Token *keep = out->type == ID ? find_variable(out->name, NULL) : NULL;
         drop_temps();
         emit_unwind(NULL, keep);
         token->llvm.elem = llvm_ret(val);
         break;
      }
      case NOT: case BNOT: {
         code_gen(node->left);
         token->llvm.elem = llvm_not(node->left->token->llvm.elem);
         break;
      }
      case AS: {
         code_gen(node->left);
         token->llvm.elem = llvm_num_cast(node->left->token->llvm.elem,
                                          node->left->token->ret_type,
                                          token->ret_type);
         break;
      }
      case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN:
      case DIV_ASSIGN: case MOD_ASSIGN: case BAND_ASSIGN:
      case BOR_ASSIGN: case BXOR_ASSIGN: case LSHIFT_ASSIGN:
      case RSHIFT_ASSIGN: {
         code_gen_compound(node);
         break;
      }
      case FALLBACK: {
         code_gen_fallback(node);
         break;
      }
      case ADD: case SUB: case MUL: case DIV: case MOD:
      case EQUAL: case NOT_EQUAL: case LESS: case GREAT:
      case LESS_EQUAL: case GREAT_EQUAL:
      case AND: case OR:
      case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT: {
         code_gen_binop(node);
         break;
      }
      default:
         CHECK(1, ASSERT_CODEGEN_NODE, to_string(token->type));
         break;
   }
}
