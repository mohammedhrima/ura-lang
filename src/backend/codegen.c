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

Value llvm_ret(Value value) {
   return LLVMBuildRet(ura.builder, value);
}

Value llvm_not(Value value) {
   return LLVMBuildNot(ura.builder, value, "not");
}

Value llvm_int_cast(Value value, TypeRef type) {
   return LLVMBuildIntCast2(ura.builder, value, type, 1, "cast");
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
   LLVMInitializeNativeTarget();
   LLVMInitializeNativeAsmPrinter();
   LLVMInitializeNativeAsmParser();
   char *triple = LLVMGetDefaultTargetTriple();
   LLVMSetTarget(ura.module, triple);
   LLVMDisposeMessage(triple);

   if (!ura.enable_san) return;
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Debug Info Version", 18,
                     LLVMValueAsMetadata(const_i32(3)));
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13,
                     LLVMValueAsMetadata(const_i32(4)));
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

Value get_or_declare(char *name, TypeRef fn_type) {
   Value fn = LLVMGetNamedFunction(ura.module, name);
   if (!fn) fn = LLVMAddFunction(ura.module, name, fn_type);
   return fn;
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
   render_caret(ms, op);
   fclose(ms);
   if (ura.no_color) decolor(text);
   Value   msg  = llvm_string(text, "trap_msg");
   TypeRef i8p  = pointer_to(ura.i8);

   TypeRef write_params[3] = { ura.i32, i8p, ura.i64 };
   TypeRef write_ty        = LLVMFunctionType(ura.i64, write_params, 3, 0);
   Value   write_fn        = get_or_declare("write", write_ty);
   Value   write_args[3]   = { const_i32(2), msg, LLVMConstInt(ura.i64, strlen(text), 0) };
   llvm_call(write_ty, write_fn, write_args, 3, "");
   free(text);

   TypeRef exit_ty      = LLVMFunctionType(ura.vd, (TypeRef[]){ ura.i32 }, 1, 0);
   Value   exit_fn      = get_or_declare("exit", exit_ty);
   Value   exit_args[1] = { const_i32(1) };
   llvm_call(exit_ty, exit_fn, exit_args, 1, "");

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
   token->llvm.func_type = LLVMFunctionType(to_llvm_type(token->ret_type), params, n, token->is_variadic);
   token->llvm.elem      = LLVMAddFunction(ura.module, token->name, token->llvm.func_type);
   free(params);
}

Value address_of(Node *node) {
   Token *token = node->token;
   if (token->type == ACCESS) return access_ptr(node);
   if (token->is_dec) {
      TypeRef t = llvm_type_of(token);
      if (token->is_ref) t = pointer_to(t);
      return token->llvm.elem = llvm_alloca(t, token->name);
   }
   Token *decl = find_variable(token->name, NULL);
   if (decl->is_ref) {
      Value ptr = llvm_load(LLVMPointerType(to_llvm_type(decl->ret_type), 0), decl->llvm.elem, "ref");
      if (token->is_nullable) guard_bound(token, ptr);
      return ptr;
   }
   return decl->llvm.elem;
}

Value access_ptr(Node *node) {
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = llvm_extract(slice, 0, "arr.data");
   code_gen(node->right);
   Value   idx   = node->right->token->llvm.elem;
   if (node->token->is_nullable) guard_index(node->token, idx, slice);
   Token  *arr   = node->left->token;
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
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
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   Value   ptr   = llvm_gep(elem, data, &start, 1, "slice.data");
   Value   len   = LLVMBuildSub(ura.builder, end, start, "slice.len");
   node->token->llvm.elem = make_slice(arr->Array.sub_type, arr->Array.depth, ptr, len);
}

void code_gen_dot(Node *node) {
   code_gen(node->left);
   Value len = llvm_extract(node->left->token->llvm.elem, 1, "len");
   node->token->llvm.elem = LLVMBuildIntCast2(ura.builder, len, ura.i32, 1, "len");
}

void code_gen_access(Node *node) {
   if (node->right->token->type == RANGE) { code_gen_slice(node); return; }
   Value   ptr  = access_ptr(node);
   TypeRef elem = node->token->ret_type == ARRAY_TYPE
                  ? array_type(node->token->Array.sub_type, node->token->Array.depth)
                  : to_llvm_type(node->token->ret_type);
   node->token->llvm.elem = llvm_load(elem, ptr, "idx");
}

void code_gen_array_lit(Node *node) {
   Token  *token = node->token;
   int     n     = node->children_count;
   int     depth = token->Array.depth;
   TypeRef elem  = depth > 1 ? array_type(token->Array.sub_type, depth - 1)
                             : to_llvm_type(token->Array.sub_type);
   Value   len   = const_i64(n);
   Value   data  = LLVMBuildArrayAlloca(ura.builder, elem, len, "arr");
   for (int i = 0; i < n; i++) {
      code_gen(node->children[i]);
      Value idx = const_i64(i);
      Value gep = llvm_gep(elem, data, &idx, 1, "arr.init");
      llvm_store(node->children[i]->token->llvm.elem, gep);
   }
   TypeRef slice = array_type(token->Array.sub_type, depth);
   Value   agg   = LLVMGetUndef(slice);
   agg = llvm_insert(agg, data, 0, "arr.ptr");
   agg = llvm_insert(agg, len,  1, "arr.len");
   token->llvm.elem = agg;
}

Value make_slice(Type sub, int depth, Value data, Value len) {
   Value agg = LLVMGetUndef(array_type(sub, depth));
   agg = llvm_insert(agg, data, 0, "arr.ptr");
   agg = llvm_insert(agg, len,  1, "arr.len");
   return agg;
}

Value array_calloc(TypeRef elem, Value count, Value esz) {
   TypeRef i8p = pointer_to(ura.i8);
   TypeRef cty = LLVMFunctionType(i8p, (TypeRef[]){ ura.i64, ura.i64 }, 2, 0);
   Value   mem = llvm_call(cty, get_or_declare("calloc", cty), (Value[]){ count, esz }, 2, "heap");
   return LLVMBuildBitCast(ura.builder, mem, pointer_to(elem), "arr");
}

Value build_array(Type sub, Value *dims, int depth, bool heap) {
   Value   n    = dims[0];
   TypeRef elem = depth == 1 ? to_llvm_type(sub) : array_type(sub, depth - 1);
   Value   esz  = LLVMConstInt(ura.i64, LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), elem), 0);
   if (depth == 1) {
      if (heap) return make_slice(sub, 1, array_calloc(elem, n, esz), n);
      Value data  = LLVMBuildArrayAlloca(ura.builder, elem, n, "arr");
      Value bytes = LLVMBuildMul(ura.builder, n, esz, "bytes");
      LLVMBuildMemSet(ura.builder, data, LLVMConstInt(ura.i8, 0, 0), bytes, 0);
      return make_slice(sub, 1, data, n);
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
   Value   sub_arr = build_array(sub, dims + 1, depth - 1, heap);
   i = llvm_load(ura.i64, slot, "i");
   Value   gep = llvm_gep(inner, data, &i, 1, "arr.slot");
   llvm_store(sub_arr, gep);
   llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "next"), slot);
   llvm_br(cond);
   llvm_at(end);
   return make_slice(sub, depth, data, n);
}

void code_gen_array_ctor(Node *node) {
   Token *token = node->token;
   int    depth = token->Array.depth;
   Value *dims  = allocate(depth, sizeof(Value));
   for (int i = 0; i < depth; i++) {
      code_gen(node->children[i]);
      dims[i] = LLVMBuildIntCast2(ura.builder, node->children[i]->token->llvm.elem, ura.i64, 1, "n");
   }
   token->llvm.elem = build_array(token->Array.sub_type, dims, depth, token->is_heap);
   free(dims);
}

void free_array(Value slice, Type sub, int depth) {
   Value data = llvm_extract(slice, 0, "arr.data");
   if (depth > 1) {
      Value   len   = llvm_extract(slice, 1, "arr.len");
      TypeRef inner = array_type(sub, depth - 1);
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
      free_array(llvm_load(inner, gep, "inner"), sub, depth - 1);
      i = llvm_load(ura.i64, slot, "i");
      llvm_store(LLVMBuildAdd(ura.builder, i, const_i64(1), "next"), slot);
      llvm_br(cond);
      llvm_at(end);
   }
   Value   ptr = LLVMBuildBitCast(ura.builder, data, pointer_to(ura.i8), "free.ptr");
   TypeRef fty = LLVMFunctionType(ura.vd, (TypeRef[]){ pointer_to(ura.i8) }, 1, 0);
   llvm_call(fty, get_or_declare("free", fty), (Value[]){ ptr }, 1, "");
}

void code_gen_typeof(Node *node) {
   char *name = type_name(node->left->token->ret_type);
   node->token->llvm.elem = llvm_string(name, "typeof");
}

void code_gen_sizeof(Node *node) {
   TypeRef t = llvm_type_of(node->left->token);
   unsigned long long sz = LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), t);
   node->token->llvm.elem = const_i32(sz);
}

void code_gen_clean(Node *node) {
   Token  *arr   = node->left->token;
   Value   slot  = address_of(node->left);
   TypeRef sty   = array_type(arr->Array.sub_type, arr->Array.depth);
   Value   slice = llvm_load(sty, slot, "arr");
   free_array(slice, arr->Array.sub_type, arr->Array.depth);
   llvm_store(LLVMConstNull(sty), slot);
}

void code_gen_literal(Node *node) {
   Token *token = node->token;
   switch (token->type) {
      case INT:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0); break;
      case BOOL:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Bool.value, 0); break;
      case CHARS: token->llvm.elem = llvm_string(token->Chars.value, "str"); break;
      case CHAR:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Char.value, 0); break;
      case FLOAT: token->llvm.elem = LLVMConstReal(to_llvm_type(token->ret_type), token->Float.value); break;
   }
}

void code_gen_fdec(Node *node) {
   Token *token = node->token;
   emit_signature(node);
   if (token->is_proto) return;
   debug_enter_function(token);
   enter_scope(node);
   for (int i = 0; i < token->Fn.params_count; i++) {
      Token  *param = token->Fn.params[i];
      TypeRef pt    = llvm_type_of(param);
      if (param->is_ref) pt = pointer_to(pt);
      param->llvm.elem = llvm_alloca(pt, param->name);
      llvm_store(LLVMGetParam(token->llvm.elem, i), param->llvm.elem);
   }
   for (int i = 0; i < node->children_count; i++) {
      set_debug_location(node->children[i]->token);
      code_gen(node->children[i]);
   }
   if (!LLVMGetBasicBlockTerminator(here_block())) {
      if (token->ret_type == VOID) LLVMBuildRetVoid(ura.builder);
      else LLVMBuildRet(ura.builder, default_value(token));
   }
   exit_scope();
   debug_exit_function(token);
}

void code_gen_id(Node *node) {
   Token *token = node->token;
   if (token->is_dec) {
      TypeRef t = llvm_type_of(token);
      if (token->is_ref) t = pointer_to(t);
      token->llvm.elem = llvm_alloca(t, token->name);
      Value init = token->is_ref ? LLVMConstNull(t) : default_value(token);
      llvm_store(init, token->llvm.elem);
      return;
   }
   Token  *decl = find_variable(token->name, NULL);
   TypeRef t    = llvm_type_of(decl);
   if (!decl->is_ref) {
      token->llvm.elem = llvm_load(t, decl->llvm.elem, token->name);
      return;
   }
   Value ptr = llvm_load(pointer_to(t), decl->llvm.elem, "ref");
   if (token->is_nullable) guard_bound(token, ptr);
   token->llvm.elem = llvm_load(t, ptr, token->name);
}

void code_gen_fcall(Node *node) {
   Token *token    = node->token;
   bool   indirect = token->Fcall.var != NULL;
   Token *fn       = indirect ? token->Fcall.var : token->Fcall.ptr->token;
   if (!indirect) emit_signature(token->Fcall.ptr);
   int    n    = node->children_count;
   Value *args = NULL;
   if (n > 0) {
      args = allocate(n, sizeof(Value));
      for (int i = 0; i < n; i++) {
         code_gen(node->children[i]);
         args[i] = node->children[i]->token->llvm.elem;
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

void code_gen_body(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      code_gen(node->children[i]);
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
   exit_scope();
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
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
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
   exit_scope();
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
   Value step = LLVMBuildSelect(ura.builder, asc, const_i32(1), LLVMConstInt(ura.i32, -1, 1), "step");
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
   llvm_cond_br(llvm_icmp(LLVMIntNE, i, b, "more"), body, end);
   llvm_at(body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
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
   exit_scope();
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
   bool  fp      = node->left->token->ret_type == FLOAT;
   enter_scope(node);
   for (int i = 0; i < node->children_count; i++) {
      Node *branch = node->children[i];
      if (branch->token->type == DEFAULT) {
         enter_scope(branch); code_gen_body(branch); exit_scope();
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
      enter_scope(branch); code_gen_body(branch); exit_scope();
      if (!LLVMGetBasicBlockTerminator(here_block()))
         llvm_br(end);
      llvm_at(next);
   }
   exit_scope();
   llvm_at(end);
}

void code_gen_if(Node *node) {
   Value fn  = here_func();
   Block end = llvm_block(fn, "endif");
   for (Node *cur = node; cur; cur = cur->right) {
      if (cur->token->type == ELSE) {
         enter_scope(cur);
         code_gen_body(cur);
         exit_scope();
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
      exit_scope();
      if (!LLVMGetBasicBlockTerminator(here_block()))
         llvm_br(end);
      llvm_at(next);
   }
   llvm_at(end);
}

void code_gen_assign(Node *node) {
   Token *token = node->token;
   Value dest = address_of(node->left);
   code_gen(node->right);
   llvm_store(node->right->token->llvm.elem, dest);
   token->llvm.elem = node->right->token->llvm.elem;
}

void code_gen_binop(Node *node) {
   Token *token = node->token;
   code_gen(node->left);
   code_gen(node->right);
   Value left  = node->left->token->llvm.elem;
   Value right = node->right->token->llvm.elem;
   Value res   = NULL;
   bool  fp    = node->left->token->ret_type == FLOAT;
   if (includes(token->type, DIV, MOD, 0)) guard_nonzero(token, right);
   switch (token->type) {
      case ADD: res = fp ? LLVMBuildFAdd(ura.builder, left, right, "fadd") : LLVMBuildAdd(ura.builder, left, right, "add"); break;
      case SUB: res = fp ? LLVMBuildFSub(ura.builder, left, right, "fsub") : LLVMBuildSub(ura.builder, left, right, "sub"); break;
      case MUL: res = fp ? LLVMBuildFMul(ura.builder, left, right, "fmul") : LLVMBuildMul(ura.builder, left, right, "mul"); break;
      case DIV: res = fp ? LLVMBuildFDiv(ura.builder, left, right, "fdiv") : LLVMBuildSDiv(ura.builder, left, right, "div"); break;
      case MOD: res = fp ? LLVMBuildFRem(ura.builder, left, right, "frem") : LLVMBuildSRem(ura.builder, left, right, "mod"); break;
      case EQUAL:       res = fp ? llvm_fcmp(LLVMRealOEQ, left, right, "feq") : llvm_icmp(LLVMIntEQ,  left, right, "eq"); break;
      case NOT_EQUAL:   res = fp ? llvm_fcmp(LLVMRealUNE, left, right, "fne") : llvm_icmp(LLVMIntNE,  left, right, "ne"); break;
      case LESS:        res = fp ? llvm_fcmp(LLVMRealOLT, left, right, "flt") : llvm_icmp(LLVMIntSLT, left, right, "lt"); break;
      case GREAT:       res = fp ? llvm_fcmp(LLVMRealOGT, left, right, "fgt") : llvm_icmp(LLVMIntSGT, left, right, "gt"); break;
      case LESS_EQUAL:  res = fp ? llvm_fcmp(LLVMRealOLE, left, right, "fle") : llvm_icmp(LLVMIntSLE, left, right, "le"); break;
      case GREAT_EQUAL: res = fp ? llvm_fcmp(LLVMRealOGE, left, right, "fge") : llvm_icmp(LLVMIntSGE, left, right, "ge"); break;
      case AND: res = LLVMBuildAnd(ura.builder, left, right, "and"); break;
      case OR:  res = LLVMBuildOr(ura.builder,  left, right, "or"); break;
      case BAND:   res = LLVMBuildAnd(ura.builder, left, right, "band"); break;
      case BOR:    res = LLVMBuildOr(ura.builder,  left, right, "bor"); break;
      case BXOR:   res = LLVMBuildXor(ura.builder, left, right, "bxor"); break;
      case LSHIFT: res = LLVMBuildShl(ura.builder, left, right, "shl"); break;
      case RSHIFT: res = LLVMBuildAShr(ura.builder, left, right, "shr"); break;
      default: break;
   }
   token->llvm.elem = res;
}

void code_gen_compound(Node *node) {
   Value   dest = address_of(node->left);
   code_gen(node->right);
   Value   r   = node->right->token->llvm.elem;
   TypeRef t   = to_llvm_type(node->left->token->ret_type);
   Value   cur = llvm_load(t, dest, "cur");
   Value   res;
   bool    fp  = node->left->token->ret_type == FLOAT;
   switch (node->token->type) {
      case ADD_ASSIGN: res = fp ? LLVMBuildFAdd(ura.builder, cur, r, "fadd") : LLVMBuildAdd(ura.builder, cur, r, "add"); break;
      case SUB_ASSIGN: res = fp ? LLVMBuildFSub(ura.builder, cur, r, "fsub") : LLVMBuildSub(ura.builder, cur, r, "sub"); break;
      case MUL_ASSIGN: res = fp ? LLVMBuildFMul(ura.builder, cur, r, "fmul") : LLVMBuildMul(ura.builder, cur, r, "mul"); break;
      case DIV_ASSIGN: guard_nonzero(node->token, r); res = fp ? LLVMBuildFDiv(ura.builder, cur, r, "fdiv") : LLVMBuildSDiv(ura.builder, cur, r, "div"); break;
      case MOD_ASSIGN: guard_nonzero(node->token, r); res = fp ? LLVMBuildFRem(ura.builder, cur, r, "frem") : LLVMBuildSRem(ura.builder, cur, r, "mod"); break;
      default: res = r; break;
   }
   llvm_store(res, dest);
   node->token->llvm.elem = res;
}

void code_gen_output(Node *node) {
   char  *fmt   = allocate(node->children_count * 4 + 8, 1);
   int    fc    = 0;
   Value *args  = allocate(node->children_count + 1, sizeof(Value));
   int    nargs = 0;

   for (int i = 0; i < node->children_count; i++) {
      Node *arg = node->children[i];
      code_gen(arg);
      Value v = arg->token->llvm.elem;
      switch (arg->token->ret_type) {
         case INT: fmt[fc++] = '%'; fmt[fc++] = 'd'; args[nargs++] = v; break;
         case SHORT: fmt[fc++] = '%'; fmt[fc++] = 'd'; args[nargs++] = LLVMBuildSExt(ura.builder, v, ura.i32, "s2i"); break;
         case LONG: fmt[fc++] = '%'; fmt[fc++] = 'l'; fmt[fc++] = 'l'; fmt[fc++] = 'd'; args[nargs++] = v; break;
         case CHAR: fmt[fc++] = '%'; fmt[fc++] = 'c'; args[nargs++] = LLVMBuildSExt(ura.builder, v, ura.i32, "c2i"); break;
         case CHARS: fmt[fc++] = '%'; fmt[fc++] = 's'; args[nargs++] = v; break;
         case BOOL: {
            Value ts = llvm_string("True", "true_str");
            Value fs = llvm_string("False", "false_str");
            fmt[fc++] = '%'; fmt[fc++] = 's';
            args[nargs++] = LLVMBuildSelect(ura.builder, v, ts, fs, "bool_str");
            break;
         }
         case FLOAT:
            fmt[fc++] = '%'; fmt[fc++] = 'f';
            args[nargs++] = LLVMBuildFPExt(ura.builder, v, LLVMDoubleTypeInContext(ura.context), "f2d");
            break;
         default: fmt[fc++] = '?'; break;
      }
   }

   TypeRef i8p       = pointer_to(ura.i8);
   TypeRef printf_ty = LLVMFunctionType(ura.i32, (TypeRef[]){ i8p }, 1, 1);
   Value   printf_fn = get_or_declare("printf", printf_ty);
   Value  *call_args = allocate(nargs + 1, sizeof(Value));
   call_args[0]      = llvm_string(fmt, "fmt");
   memcpy(call_args + 1, args, nargs * sizeof(Value));
   node->token->llvm.elem = llvm_call(printf_ty, printf_fn, call_args, nargs + 1, "");
   free(fmt);
   free(args);
   free(call_args);
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
      case STRUCT_DEF:                            break;

      case REF:      token->llvm.elem = address_of(node->left);   break;
      case BREAK:    llvm_br(node->left->token->llvm.end);        break;
      case CONTINUE: llvm_br(node->left->token->llvm.start);      break;

      case INT: case BOOL: case CHARS:
      case CHAR: case FLOAT:
         code_gen_literal(node);
         break;

      case FN_TYPE:
         emit_signature(token->Fcall.ptr);
         token->llvm.elem = token->Fcall.ptr->token->llvm.elem;
         break;

      case RETURN:
         code_gen(node->left);
         token->llvm.elem = llvm_ret(node->left->token->llvm.elem);
         break;

      case NOT: case BNOT:
         code_gen(node->left);
         token->llvm.elem = llvm_not(node->left->token->llvm.elem);
         break;

      case AS:
         code_gen(node->left);
         token->llvm.elem = llvm_int_cast(node->left->token->llvm.elem,
                                          to_llvm_type(token->ret_type));
         break;

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
         CHECK(1, ASSERT_CODEGEN_NODE, to_string(token->type));
         break;
   }
}
