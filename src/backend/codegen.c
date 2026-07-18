#include "header.h"

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
                     LLVMValueAsMetadata(LLVMConstInt(ura.i32, 3, 0)));
   LLVMAddModuleFlag(ura.module, LLVMModuleFlagBehaviorWarning, "Dwarf Version", 13,
                     LLVMValueAsMetadata(LLVMConstInt(ura.i32, 4, 0)));
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
   token->llvm.prev_block = LLVMGetInsertBlock(ura.builder);
   token->llvm.prev_scope = ura.debug_scope;
   token->llvm.prev_loc   = ura.debug_builder ? LLVMGetCurrentDebugLocation2(ura.builder) : NULL;
   Block entry = LLVMAppendBasicBlockInContext(ura.context, token->llvm.elem, "entry");
   LLVMPositionBuilderAtEnd(ura.builder, entry);
   if (ura.enable_san) {
      unsigned kind = LLVMGetEnumAttributeKindForName("sanitize_address", 16);
      LLVMAddAttributeAtIndex(token->llvm.elem, LLVMAttributeFunctionIndex,
                              LLVMCreateEnumAttribute(ura.context, kind, 0));
   }
   if (!ura.debug_builder) return;
   MetadataRef di_type = LLVMDIBuilderCreateSubroutineType(
       ura.debug_builder, ura.debug_file, NULL, 0, LLVMDIFlagZero);
   MetadataRef di_func = LLVMDIBuilderCreateFunction(
       ura.debug_builder, ura.debug_compile_unit, token->name, strlen(token->name),
       token->name, strlen(token->name), ura.debug_file, token->line, di_type, 0, 1,
       token->line, LLVMDIFlagZero, 0);
   LLVMSetSubprogram(token->llvm.elem, di_func);
   ura.debug_scope = di_func;
   MetadataRef loc = LLVMDIBuilderCreateDebugLocation(ura.context, token->line, 0, di_func, NULL);
   LLVMSetCurrentDebugLocation2(ura.builder, loc);
}

void debug_exit_function(Token *token) {
   if (token->llvm.prev_block) LLVMPositionBuilderAtEnd(ura.builder, token->llvm.prev_block);
   if (!ura.debug_builder) return;
   ura.debug_scope = token->llvm.prev_scope;
   LLVMSetCurrentDebugLocation2(ura.builder, token->llvm.prev_loc);
}

void set_debug_location(Token *token) {
   if (!ura.debug_builder || !ura.debug_scope) return;
   MetadataRef loc = LLVMDIBuilderCreateDebugLocation(ura.context, token->line, 0, ura.debug_scope, NULL);
   LLVMSetCurrentDebugLocation2(ura.builder, loc);
}

Value get_or_declare(char *name, TypeRef fn_type) {
   Value fn = LLVMGetNamedFunction(ura.module, name);
   if (!fn) fn = LLVMAddFunction(ura.module, name, fn_type);
   return fn;
}

void guard(Token *op, Value is_bad, char *what) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block trap = LLVMAppendBasicBlockInContext(ura.context, fn, "trap");
   Block cont = LLVMAppendBasicBlockInContext(ura.context, fn, "cont");
   LLVMBuildCondBr(ura.builder, is_bad, trap, cont);

   LLVMPositionBuilderAtEnd(ura.builder, trap);
   char  *text = NULL;
   size_t tlen = 0;
   File   ms   = open_memstream(&text, &tlen);
   fprintf(ms, RED("runtime error: ") "%s\n", what);
   render_caret(ms, op);
   fclose(ms);
   if (ura.no_color) decolor(text);
   Value   msg  = LLVMBuildGlobalStringPtr(ura.builder, text, "trap_msg");
   TypeRef i8p  = LLVMPointerType(ura.i8, 0);

   TypeRef write_params[3] = { ura.i32, i8p, ura.i64 };
   TypeRef write_ty        = LLVMFunctionType(ura.i64, write_params, 3, 0);
   Value   write_fn        = get_or_declare("write", write_ty);
   Value   write_args[3]   = { LLVMConstInt(ura.i32, 2, 0), msg, LLVMConstInt(ura.i64, strlen(text), 0) };
   LLVMBuildCall2(ura.builder, write_ty, write_fn, write_args, 3, "");
   free(text);

   TypeRef exit_ty      = LLVMFunctionType(ura.vd, (TypeRef[]){ ura.i32 }, 1, 0);
   Value   exit_fn      = get_or_declare("exit", exit_ty);
   Value   exit_args[1] = { LLVMConstInt(ura.i32, 1, 0) };
   LLVMBuildCall2(ura.builder, exit_ty, exit_fn, exit_args, 1, "");

   LLVMBuildUnreachable(ura.builder);
   LLVMPositionBuilderAtEnd(ura.builder, cont);
}

void guard_nonzero(Token *op, Value divisor) {
   TypeRef ty     = LLVMTypeOf(divisor);
   bool    fp     = LLVMGetTypeKind(ty) == FloatType || LLVMGetTypeKind(ty) == DoubleType;
   Value   iszero = fp
      ? LLVMBuildFCmp(ura.builder, LLVMRealOEQ, divisor, LLVMConstReal(ty, 0.0), "iszero")
      : LLVMBuildICmp(ura.builder, LLVMIntEQ,   divisor, LLVMConstInt(ty, 0, 0), "iszero");
   guard(op, iszero, includes(op->type, MOD, MOD_ASSIGN, 0) ? "Modulo by zero" : "Division by zero");
}

void guard_nonnull(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = LLVMBuildICmp(ura.builder, LLVMIntEQ, ptr, null, "isnull");
   guard(op, isnull, "Call to a null function value");
}

void guard_bound(Token *op, Value ptr) {
   Value null   = LLVMConstNull(LLVMTypeOf(ptr));
   Value isnull = LLVMBuildICmp(ura.builder, LLVMIntEQ, ptr, null, "unbound");
   char *msg    = format("reference '%s' used before it was bound - assign '%s = ref <target>' first", op->name, op->name);
   guard(op, isnull, msg);
   free(msg);
}

void guard_index(Token *op, Value idx, Value slice) {
   Value len   = LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len");
   Value idx64 = LLVMBuildIntCast2(ura.builder, idx, ura.i64, 1, "idx");
   Value low   = LLVMBuildICmp(ura.builder, LLVMIntSLT, idx64, LLVMConstInt(ura.i64, 0, 0), "oob.low");
   Value high  = LLVMBuildICmp(ura.builder, LLVMIntSGE, idx64, len, "oob.high");
   guard(op, LLVMBuildOr(ura.builder, low, high, "oob"), "array index out of bounds");
}

void guard_slice(Token *op, Value start, Value end, Value len) {
   Value lo  = LLVMBuildICmp(ura.builder, LLVMIntSLT, start, LLVMConstInt(ura.i64, 0, 0), "s.lo");
   Value hi  = LLVMBuildICmp(ura.builder, LLVMIntSGT, end, len, "e.hi");
   Value ord = LLVMBuildICmp(ura.builder, LLVMIntSGT, start, end, "s.gt");
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
         params[i] = token->Fn.params[i]->is_ref ? LLVMPointerType(pt, 0) : pt;
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
      if (token->is_ref) t = LLVMPointerType(t, 0);
      return token->llvm.elem = LLVMBuildAlloca(ura.builder, t, token->name);
   }
   Token *decl = find_variable(token->name, NULL);
   if (decl->is_ref) {
      Value ptr = LLVMBuildLoad2(ura.builder, LLVMPointerType(to_llvm_type(decl->ret_type), 0), decl->llvm.elem, "ref");
      if (token->is_nullable) guard_bound(token, ptr);
      return ptr;
   }
   return decl->llvm.elem;
}

Value access_ptr(Node *node) {
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   code_gen(node->right);
   Value   idx   = node->right->token->llvm.elem;
   if (node->token->is_nullable) guard_index(node->token, idx, slice);
   Token  *arr   = node->left->token;
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   return LLVMBuildGEP2(ura.builder, elem, data, &idx, 1, "arr.at");
}

void code_gen_slice(Node *node) {
   Token *arr   = node->left->token;
   Node  *range = node->right;
   code_gen(node->left);
   Value   slice = node->left->token->llvm.elem;
   Value   data  = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   code_gen(range->left);
   code_gen(range->right);
   Value   start = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i64, 1, "start");
   Value   end   = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i64, 1, "end");
   if (node->token->is_nullable)
      guard_slice(node->token, start, end, LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len"));
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   Value   ptr   = LLVMBuildGEP2(ura.builder, elem, data, &start, 1, "slice.data");
   Value   len   = LLVMBuildSub(ura.builder, end, start, "slice.len");
   node->token->llvm.elem = make_slice(arr->Array.sub_type, arr->Array.depth, ptr, len);
}

void code_gen_dot(Node *node) {
   code_gen(node->left);
   Value len = LLVMBuildExtractValue(ura.builder, node->left->token->llvm.elem, 1, "len");
   node->token->llvm.elem = LLVMBuildIntCast2(ura.builder, len, ura.i32, 1, "len");
}

void code_gen_access(Node *node) {
   if (node->right->token->type == RANGE) { code_gen_slice(node); return; }
   Value   ptr  = access_ptr(node);
   TypeRef elem = node->token->ret_type == ARRAY_TYPE
                  ? array_type(node->token->Array.sub_type, node->token->Array.depth)
                  : to_llvm_type(node->token->ret_type);
   node->token->llvm.elem = LLVMBuildLoad2(ura.builder, elem, ptr, "idx");
}

void code_gen_array_lit(Node *node) {
   Token  *token = node->token;
   int     n     = node->children_count;
   int     depth = token->Array.depth;
   TypeRef elem  = depth > 1 ? array_type(token->Array.sub_type, depth - 1)
                             : to_llvm_type(token->Array.sub_type);
   Value   len   = LLVMConstInt(ura.i64, n, 0);
   Value   data  = LLVMBuildArrayAlloca(ura.builder, elem, len, "arr");
   for (int i = 0; i < n; i++) {
      code_gen(node->children[i]);
      Value idx = LLVMConstInt(ura.i64, i, 0);
      Value gep = LLVMBuildGEP2(ura.builder, elem, data, &idx, 1, "arr.init");
      LLVMBuildStore(ura.builder, node->children[i]->token->llvm.elem, gep);
   }
   TypeRef slice = array_type(token->Array.sub_type, depth);
   Value   agg   = LLVMGetUndef(slice);
   agg = LLVMBuildInsertValue(ura.builder, agg, data, 0, "arr.ptr");
   agg = LLVMBuildInsertValue(ura.builder, agg, len,  1, "arr.len");
   token->llvm.elem = agg;
}

Value make_slice(Type sub, int depth, Value data, Value len) {
   Value agg = LLVMGetUndef(array_type(sub, depth));
   agg = LLVMBuildInsertValue(ura.builder, agg, data, 0, "arr.ptr");
   agg = LLVMBuildInsertValue(ura.builder, agg, len,  1, "arr.len");
   return agg;
}

Value array_calloc(TypeRef elem, Value count, Value esz) {
   TypeRef i8p = LLVMPointerType(ura.i8, 0);
   TypeRef cty = LLVMFunctionType(i8p, (TypeRef[]){ ura.i64, ura.i64 }, 2, 0);
   Value   mem = LLVMBuildCall2(ura.builder, cty, get_or_declare("calloc", cty), (Value[]){ count, esz }, 2, "heap");
   return LLVMBuildBitCast(ura.builder, mem, LLVMPointerType(elem, 0), "arr");
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
   Value   fn    = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Value   slot  = LLVMBuildAlloca(ura.builder, ura.i64, "i");
   LLVMBuildStore(ura.builder, LLVMConstInt(ura.i64, 0, 0), slot);
   Block   cond  = LLVMAppendBasicBlockInContext(ura.context, fn, "arr.cond");
   Block   body  = LLVMAppendBasicBlockInContext(ura.context, fn, "arr.body");
   Block   end   = LLVMAppendBasicBlockInContext(ura.context, fn, "arr.end");
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   Value   i     = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
   LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntSLT, i, n, "more"), body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   Value   sub_arr = build_array(sub, dims + 1, depth - 1, heap);
   i = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
   Value   gep = LLVMBuildGEP2(ura.builder, inner, data, &i, 1, "arr.slot");
   LLVMBuildStore(ura.builder, sub_arr, gep);
   LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, i, LLVMConstInt(ura.i64, 1, 0), "next"), slot);
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
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
   Value data = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   if (depth > 1) {
      Value   len   = LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len");
      TypeRef inner = array_type(sub, depth - 1);
      Value   fn    = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
      Value   slot  = LLVMBuildAlloca(ura.builder, ura.i64, "i");
      LLVMBuildStore(ura.builder, LLVMConstInt(ura.i64, 0, 0), slot);
      Block   cond  = LLVMAppendBasicBlockInContext(ura.context, fn, "free.cond");
      Block   body  = LLVMAppendBasicBlockInContext(ura.context, fn, "free.body");
      Block   end   = LLVMAppendBasicBlockInContext(ura.context, fn, "free.end");
      LLVMBuildBr(ura.builder, cond);
      LLVMPositionBuilderAtEnd(ura.builder, cond);
      Value   i     = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
      LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntSLT, i, len, "more"), body, end);
      LLVMPositionBuilderAtEnd(ura.builder, body);
      Value   gep   = LLVMBuildGEP2(ura.builder, inner, data, &i, 1, "free.slot");
      free_array(LLVMBuildLoad2(ura.builder, inner, gep, "inner"), sub, depth - 1);
      i = LLVMBuildLoad2(ura.builder, ura.i64, slot, "i");
      LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, i, LLVMConstInt(ura.i64, 1, 0), "next"), slot);
      LLVMBuildBr(ura.builder, cond);
      LLVMPositionBuilderAtEnd(ura.builder, end);
   }
   Value   ptr = LLVMBuildBitCast(ura.builder, data, LLVMPointerType(ura.i8, 0), "free.ptr");
   TypeRef fty = LLVMFunctionType(ura.vd, (TypeRef[]){ LLVMPointerType(ura.i8, 0) }, 1, 0);
   LLVMBuildCall2(ura.builder, fty, get_or_declare("free", fty), (Value[]){ ptr }, 1, "");
}

void code_gen_typeof(Node *node) {
   char *name = type_name(node->left->token->ret_type);
   node->token->llvm.elem = LLVMBuildGlobalStringPtr(ura.builder, name, "typeof");
}

void code_gen_sizeof(Node *node) {
   TypeRef t = llvm_type_of(node->left->token);
   unsigned long long sz = LLVMABISizeOfType(LLVMGetModuleDataLayout(ura.module), t);
   node->token->llvm.elem = LLVMConstInt(ura.i32, sz, 0);
}

void code_gen_clean(Node *node) {
   Token  *arr   = node->left->token;
   Value   slot  = address_of(node->left);
   TypeRef sty   = array_type(arr->Array.sub_type, arr->Array.depth);
   Value   slice = LLVMBuildLoad2(ura.builder, sty, slot, "arr");
   free_array(slice, arr->Array.sub_type, arr->Array.depth);
   LLVMBuildStore(ura.builder, LLVMConstNull(sty), slot);
}

void code_gen_literal(Node *node) {
   Token *token = node->token;
   switch (token->type) {
      case INT:   token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Int.value, 0); break;
      case BOOL:  token->llvm.elem = LLVMConstInt(to_llvm_type(token->ret_type), token->Bool.value, 0); break;
      case CHARS: token->llvm.elem = LLVMBuildGlobalStringPtr(ura.builder, token->Chars.value, "str"); break;
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
      if (param->is_ref) pt = LLVMPointerType(pt, 0);
      param->llvm.elem = LLVMBuildAlloca(ura.builder, pt, param->name);
      LLVMBuildStore(ura.builder, LLVMGetParam(token->llvm.elem, i), param->llvm.elem);
   }
   for (int i = 0; i < node->children_count; i++) {
      set_debug_location(node->children[i]->token);
      code_gen(node->children[i]);
   }
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder))) {
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
      if (token->is_ref) t = LLVMPointerType(t, 0);
      token->llvm.elem = LLVMBuildAlloca(ura.builder, t, token->name);
      Value init = token->is_ref ? LLVMConstNull(t) : default_value(token);
      LLVMBuildStore(ura.builder, init, token->llvm.elem);
      return;
   }
   Token  *decl = find_variable(token->name, NULL);
   TypeRef t    = llvm_type_of(decl);
   if (!decl->is_ref) {
      token->llvm.elem = LLVMBuildLoad2(ura.builder, t, decl->llvm.elem, token->name);
      return;
   }
   Value ptr = LLVMBuildLoad2(ura.builder, LLVMPointerType(t, 0), decl->llvm.elem, "ref");
   if (token->is_nullable) guard_bound(token, ptr);
   token->llvm.elem = LLVMBuildLoad2(ura.builder, t, ptr, token->name);
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
      Value   fn_ptr   = LLVMBuildLoad2(ura.builder, ptr_type, fn->llvm.elem, "fn");
      guard_nonnull(token, fn_ptr);
      char   *name     = fn->Fn.ret->ret_type == VOID ? "" : "call";
      token->llvm.elem = LLVMBuildCall2(ura.builder, LLVMGetElementType(ptr_type), fn_ptr, args, n, name);
   } else {
      char *name       = fn->ret_type == VOID ? "" : "call";
      token->llvm.elem = LLVMBuildCall2(ura.builder, fn->llvm.func_type, fn->llvm.elem, args, n, name);
   }
   free(args);
}

void code_gen_body(Node *node) {
   for (int i = 0; i < node->children_count; i++) {
      code_gen(node->children[i]);
      if (LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder))) break;
   }
}

void code_gen_loop(Node *node) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "loop.body");
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "loop.end");
   node->token->llvm.start = body;
   node->token->llvm.end   = end;
   LLVMBuildBr(ura.builder, body);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, body);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_for_array(Node *node) {
   Token  *var  = node->left->token;
   bool    ref  = node->token->is_ref;
   Token  *arr  = node->right->token;
   code_gen(node->right);
   Value   slice = arr->llvm.elem;
   Value   data  = LLVMBuildExtractValue(ura.builder, slice, 0, "arr.data");
   Value   len   = LLVMBuildExtractValue(ura.builder, slice, 1, "arr.len");
   TypeRef elem  = arr->Array.depth > 1 ? array_type(arr->Array.sub_type, arr->Array.depth - 1)
                                        : to_llvm_type(arr->Array.sub_type);
   Value   fn    = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Value   idx   = LLVMBuildAlloca(ura.builder, ura.i64, "idx");
   LLVMBuildStore(ura.builder, LLVMConstInt(ura.i64, 0, 0), idx);
   Value   xslot = ref ? NULL : LLVMBuildAlloca(ura.builder, elem, var->name);
   if (!ref) var->llvm.elem = xslot;
   Block   cond  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.cond");
   Block   body  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.body");
   Block   inc   = LLVMAppendBasicBlockInContext(ura.context, fn, "for.inc");
   Block   end   = LLVMAppendBasicBlockInContext(ura.context, fn, "for.end");
   node->token->llvm.start = inc;
   node->token->llvm.end   = end;
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   Value   i = LLVMBuildLoad2(ura.builder, ura.i64, idx, "i");
   LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntSLT, i, len, "more"), body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   Value   gep = LLVMBuildGEP2(ura.builder, elem, data, &i, 1, "elem");
   if (ref) var->llvm.elem = gep;
   else     LLVMBuildStore(ura.builder, LLVMBuildLoad2(ura.builder, elem, gep, "x"), xslot);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, inc);
   LLVMPositionBuilderAtEnd(ura.builder, inc);
   Value   iv = LLVMBuildLoad2(ura.builder, ura.i64, idx, "i");
   LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, iv, LLVMConstInt(ura.i64, 1, 0), "next"), idx);
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_for(Node *node) {
   if (node->right->token->type != RANGE) { code_gen_for_array(node); return; }
   Token *var   = node->left->token;
   Node  *range = node->right;
   code_gen(range->left);
   code_gen(range->right);
   Value a    = LLVMBuildIntCast2(ura.builder, range->left->token->llvm.elem, ura.i32, 1, "a");
   Value b    = LLVMBuildIntCast2(ura.builder, range->right->token->llvm.elem, ura.i32, 1, "b");
   Value asc  = LLVMBuildICmp(ura.builder, LLVMIntSLT, a, b, "asc");
   Value step = LLVMBuildSelect(ura.builder, asc, LLVMConstInt(ura.i32, 1, 0), LLVMConstInt(ura.i32, -1, 1), "step");
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Value slot = LLVMBuildAlloca(ura.builder, ura.i32, var->name);
   LLVMBuildStore(ura.builder, a, slot);
   var->llvm.elem = slot;
   Block cond = LLVMAppendBasicBlockInContext(ura.context, fn, "for.cond");
   Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "for.body");
   Block inc  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.inc");
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "for.end");
   node->token->llvm.start = inc;
   node->token->llvm.end   = end;
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   Value i = LLVMBuildLoad2(ura.builder, ura.i32, slot, var->name);
   LLVMBuildCondBr(ura.builder, LLVMBuildICmp(ura.builder, LLVMIntNE, i, b, "more"), body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, inc);
   LLVMPositionBuilderAtEnd(ura.builder, inc);
   Value iv = LLVMBuildLoad2(ura.builder, ura.i32, slot, var->name);
   LLVMBuildStore(ura.builder, LLVMBuildAdd(ura.builder, iv, step, "next"), slot);
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_while(Node *node) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block cond = LLVMAppendBasicBlockInContext(ura.context, fn, "while.cond");
   Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "while.body");
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "while.end");
   node->token->llvm.start = cond;   
   node->token->llvm.end   = end;    
   LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, cond);
   code_gen(node->left);
   LLVMBuildCondBr(ura.builder, node->left->token->llvm.elem, body, end);
   LLVMPositionBuilderAtEnd(ura.builder, body);
   enter_scope(node);
   code_gen_body(node);
   exit_scope();
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
      LLVMBuildBr(ura.builder, cond);
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_match(Node *node) {
   Value fn   = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block end  = LLVMAppendBasicBlockInContext(ura.context, fn, "match.end");
   node->token->llvm.end = end;                       // break target
   code_gen(node->left);                              
   Value subject = node->left->token->llvm.elem;
   bool  fp      = node->left->token->ret_type == FLOAT;
   enter_scope(node);
   for (int i = 0; i < node->children_count; i++) {
      Node *branch = node->children[i];
      if (branch->token->type == DEFAULT) {
         enter_scope(branch); code_gen_body(branch); exit_scope();
         if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
            LLVMBuildBr(ura.builder, end);
         break;
      }
      Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "case.body");
      Block next = i + 1 < node->children_count ? LLVMAppendBasicBlockInContext(ura.context, fn, "case.next") : end;
      Value cond = NULL;
      for (int j = 0; j < branch->left->children_count; j++) {
         code_gen(branch->left->children[j]);
         Value val = branch->left->children[j]->token->llvm.elem;
         Value eq  = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOEQ, subject, val, "feq")
                        : LLVMBuildICmp(ura.builder, LLVMIntEQ, subject, val, "eq");
         cond = cond ? LLVMBuildOr(ura.builder, cond, eq, "case.or") : eq;
      }
      LLVMBuildCondBr(ura.builder, cond, body, next);
      LLVMPositionBuilderAtEnd(ura.builder, body);
      enter_scope(branch); code_gen_body(branch); exit_scope();
      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
         LLVMBuildBr(ura.builder, end);
      LLVMPositionBuilderAtEnd(ura.builder, next);
   }
   exit_scope();
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_if(Node *node) {
   Value fn  = LLVMGetBasicBlockParent(LLVMGetInsertBlock(ura.builder));
   Block end = LLVMAppendBasicBlockInContext(ura.context, fn, "endif");
   for (Node *cur = node; cur; cur = cur->right) {
      if (cur->token->type == ELSE) {
         enter_scope(cur);
         code_gen_body(cur);
         exit_scope();
         if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
            LLVMBuildBr(ura.builder, end);
         break;
      }
		// elif
      Block body = LLVMAppendBasicBlockInContext(ura.context, fn, "then");
      Block next = cur->right ? LLVMAppendBasicBlockInContext(ura.context, fn, "next") : end;
      code_gen(cur->left);
      LLVMBuildCondBr(ura.builder, cur->left->token->llvm.elem, body, next);
      LLVMPositionBuilderAtEnd(ura.builder, body);
      enter_scope(cur);
      code_gen_body(cur);
      exit_scope();
      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(ura.builder)))
         LLVMBuildBr(ura.builder, end);
      LLVMPositionBuilderAtEnd(ura.builder, next);
   }
   LLVMPositionBuilderAtEnd(ura.builder, end);
}

void code_gen_assign(Node *node) {
   Token *token = node->token;
   Value dest = address_of(node->left);
   code_gen(node->right);
   LLVMBuildStore(ura.builder, node->right->token->llvm.elem, dest);
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
      case EQUAL:       res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOEQ, left, right, "feq") : LLVMBuildICmp(ura.builder, LLVMIntEQ,  left, right, "eq"); break;
      case NOT_EQUAL:   res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealUNE, left, right, "fne") : LLVMBuildICmp(ura.builder, LLVMIntNE,  left, right, "ne"); break;
      case LESS:        res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOLT, left, right, "flt") : LLVMBuildICmp(ura.builder, LLVMIntSLT, left, right, "lt"); break;
      case GREAT:       res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOGT, left, right, "fgt") : LLVMBuildICmp(ura.builder, LLVMIntSGT, left, right, "gt"); break;
      case LESS_EQUAL:  res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOLE, left, right, "fle") : LLVMBuildICmp(ura.builder, LLVMIntSLE, left, right, "le"); break;
      case GREAT_EQUAL: res = fp ? LLVMBuildFCmp(ura.builder, LLVMRealOGE, left, right, "fge") : LLVMBuildICmp(ura.builder, LLVMIntSGE, left, right, "ge"); break;
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
   Value   cur = LLVMBuildLoad2(ura.builder, t, dest, "cur");
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
   LLVMBuildStore(ura.builder, res, dest);
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
            Value ts = LLVMBuildGlobalStringPtr(ura.builder, "True", "true_str");
            Value fs = LLVMBuildGlobalStringPtr(ura.builder, "False", "false_str");
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

   TypeRef i8p       = LLVMPointerType(ura.i8, 0);
   TypeRef printf_ty = LLVMFunctionType(ura.i32, (TypeRef[]){ i8p }, 1, 1);
   Value   printf_fn = get_or_declare("printf", printf_ty);
   Value  *call_args = allocate(nargs + 1, sizeof(Value));
   call_args[0]      = LLVMBuildGlobalStringPtr(ura.builder, fmt, "fmt");
   memcpy(call_args + 1, args, nargs * sizeof(Value));
   node->token->llvm.elem = LLVMBuildCall2(ura.builder, printf_ty, printf_fn, call_args, nargs + 1, "");
   free(fmt);
   free(args);
   free(call_args);
}

void code_gen(Node *node) {
   if (!node || ura.error_count) return;
   Token *token = node->token;
   switch (token->type) {
      case FDEC:   code_gen_fdec(node); break;
      case STRUCT_DEF: break;
      case INT: case BOOL: case CHARS: case CHAR: case FLOAT: code_gen_literal(node); break;
      case ID:     code_gen_id(node); break;
      case FN_TYPE:
         emit_signature(token->Fcall.ptr);
         token->llvm.elem = token->Fcall.ptr->token->llvm.elem;
         break;
      case RETURN:
         code_gen(node->left);
         token->llvm.elem = LLVMBuildRet(ura.builder, node->left->token->llvm.elem);
         break;
      case FCALL:  code_gen_fcall(node); break;
      case OUTPUT: code_gen_output(node); break;
      case ARRAY_LIT: code_gen_array_lit(node); break;
      case ACCESS:    code_gen_access(node); break;
      case DOT:       code_gen_dot(node); break;
      case ARRAY:     code_gen_array_ctor(node); break;
      case TYPEOF:    code_gen_typeof(node); break;
      case SIZEOF:    code_gen_sizeof(node); break;
      case CLEAN:     code_gen_clean(node); break;
      case NOT: case BNOT:
         code_gen(node->left);
         token->llvm.elem = LLVMBuildNot(ura.builder, node->left->token->llvm.elem, "not");
         break;
      case AS:
         code_gen(node->left);
         token->llvm.elem = LLVMBuildIntCast2(ura.builder, node->left->token->llvm.elem,
                                              to_llvm_type(token->ret_type), 1, "cast");
         break;
      case REF:    token->llvm.elem = address_of(node->left); break;
      case IF:     code_gen_if(node); break;
      case WHILE:  code_gen_while(node); break;
      case LOOP:   code_gen_loop(node); break;
      case FOR:    code_gen_for(node); break;
      case MATCH:  code_gen_match(node); break;
      case BREAK:    LLVMBuildBr(ura.builder, node->left->token->llvm.end);   break;
      case CONTINUE: LLVMBuildBr(ura.builder, node->left->token->llvm.start); break;
      case ASSIGN: code_gen_assign(node); break;
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
         CHECK(1, "code_gen: unhandled node '%s'", to_string(token->type));
         break;
   }
}
