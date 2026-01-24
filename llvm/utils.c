#include "header.h"

char *importedFiles[100];
int importedFileCount;
int block_counter;
Token *tokens[1000];
int tk_pos;
int exe_pos;
char *input;
ContextRef context;
ModuleRef module;
BuilderRef builder;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;

bool check_error(char *filename, const char *funcname, int line, bool cond, char *fmt, ...)
{
   if (!cond) return cond;
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, "error:%s:%s:%d ", filename, funcname, line);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   exit(1);
   return cond;
}
#define check(cond, fmt, ...) check_error(__FILE__, __func__, __LINE__, cond, fmt, ##__VA_ARGS__)

Token *new_token(Type type, int line, int pos, int s, int e, int space)
{
   Token *new = calloc(1, sizeof(Token));
   new->type = type;
   new->line = line;
   new->pos = pos;
   new->space = space;
   switch (type)
   {
   case ID:
   {
      if (e <= s) break;
      struct { char *value; Type type; } specials[] = {
         {"int", INT}, {"chars", CHARS}, {"char", CHAR}, {"bool", BOOL},
         {"void", VOID}, {"va_list", VA_LIST}, {"def", FDEC}, {"if", IF},
         {"while", WHILE}, {"return", RETURN}, {"end", END_BLOCK}, {"elif", ELIF},
         {"else", ELSE}, {"protoFunc", PROTO}, {"ref", REF},
         {"as", AS}, {"use", USE}, {"stack", STACK}, {"try", TRY},
         {"catch", CATCH}, {"throw", THROW}, {NULL, 0}
      };
      new->name = substr(input, s, e);

      for (int i = 0; specials[i].value; i++)
      {
         if (strcmp(new->name, specials[i].value) == 0)
         {
            new->type = specials[i].type;
            if (includes(new->type, INT, CHARS, CHAR, BOOL, VOID, VA_LIST, 0))
               new->is_dec = true;
            free(new->name);
            new->name = NULL;
            break;
         }
      }
      break;
   }
   case CHAR:
   {
      if (e <= s) break;
      char c = 0;
      if (input[s] == '\\')
      {
         char str[255] = {['n'] = '\n', ['t'] = '\t', ['\''] = '\'', ['0'] = '\0'};
         c = str[(unsigned char)input[s + 1]];
         check(!c, "handle this case [%c]\n", input[s + 1]);
      }
      else c = input[s];
      new->Char.value = c;
      break;
   }
   case CHARS: if (s < e) new->Chars.value = substr(input, s, e); break;
   case INT: if (s < e) new->Int.value = atol(input + s); break;
   default: break;
   }
   printf("new "); ptoken(new);
   return (tokens[tk_pos++] = new);
}

void tokenize()
{
   int line = 0, pos = 0, space = 0;
   for (int i = 0; input[i];)
   {
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            line++;
            pos = 0;
            space = 0;
            i++;
         }
         else if (input[i] == ' ')
         {
            if (space == 0)
            {
               while (input[i] == ' ')
               {
                  space++;
                  pos++;
                  i++;
               }
            }
            else
            {
               pos++;
               i++;
            }
         }
         else
         {
            pos++;
            i++;
         }
      }
      else if (strncmp(input + i, "//", 2) == 0)
         while (input[i] && input[i] != '\n') i++;
      else if (input[i] == '"')
      {
         int s = ++i;
         while (input[i] != '"' && input[i])
         {
            if (input[i] == '\\') i += 2;
            else i++;
         }
         new_token(CHARS, line, pos, s, i, space);
         if (input[i] != '\"')
         {
            printf("%s:%d expected [\"] found [%c]\n", __FILE__, __LINE__, input[i]);
            seg();
         }
         i++;
      }
      else if (input[i] == '\'')
      {
         int s = ++i;
         while (input[i] != '\'' && input[i])
         {
            if (input[i] == '\\') i += 2;
            else i++;
         }
         new_token(CHAR, line, pos, s, i, space);
         if (input[i] != '\'')
         {
            printf("%s:%d expected ['] found [%c]\n", __FILE__, __LINE__, input[i]);
            seg();
         }
         i++;
      }
      else if (isalpha(input[i]) || input[i] == '_')
      {
         int s = i;
         while (isalnum(input[i]) || input[i] == '_') i++;
         char *tmp = substr(input, s, i);
         free(tmp);
         new_token(ID, line, pos, s, i, space);
         pos += (i - s);
      }
      else if (isdigit(input[i]))
      {
         int s = i;
         while (input[i] && isdigit(input[i])) i++;
         new_token(INT, line, pos, s, i, space);
      }
      else
      {
         struct { char *value; Type type; } specials[] = {
            {"!=", NOT_EQUAL},  {"==", EQUAL}, {"<=", LESS_EQUAL},
            {">=", MORE_EQUAL}, {"<", LESS}, {">", MORE}, {"=", ASSIGN},
            {"+=", ADD_ASSIGN}, {"-=", SUB_ASSIGN}, {"*=", MUL_ASSIGN},
            {"/=", DIV_ASSIGN}, {"...", VARIADIC}, {".", DOT},
            {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
            {"(", LPAR}, {")", RPAR}, {"[", LBRA}, {"]", RBRA}, {":", DOTS},
            {"&&", AND}, {"||", OR}, {",", COMA},
            {0, (Type)0}
         };
         bool found = false;
         for (int j = 0; specials[j].value; j++)
         {
            size_t len = strlen(specials[j].value);
            if (strncmp(specials[j].value, input + i, len) == 0)
            {
               new_token(specials[j].type, line, pos, 0, 0, space);
               i += len;
               found = true;
               break;
            }
         }
         if (!found)
         {
            check(1, "Syntax error <%c>\n", input[i]);
            exit(1);
         }
      }
   }
   new_token(END, 0, 0, 0, 0, 0);
}

Token *find(Type type, ...)
{
   va_list ap;
   va_start(ap, type);
   while (type)
   {
      if (type == tokens[exe_pos]->type)
         return tokens[exe_pos++];
      type = va_arg(ap, Type);
   }
   return NULL;
}

Node *new_node(Token *token)
{
   Node *new = calloc(1, sizeof(Node));
   new->token = token;
   printf("new node: "); ptoken(token);
   return new;
}

void add_child_node(Node *parent, Node *child)
{
   if (parent->clen == 0)
   {
      parent->clen = 100;
      parent->children = calloc(100, sizeof(Node*));
   }
   else if (parent->cpos + 1 == parent->clen)
   {
      Node **tmp = calloc((parent->clen *= 2), sizeof(Node*));
      memcpy(tmp, parent->children, parent->cpos * sizeof(Node*));
      free(parent->children);
      parent->children = tmp;
   }
   parent->children[parent->cpos++] = child;
   child->token->space = parent->token->space + TAB;
}


// Global to track current function's landing pad
typedef struct {
   BasicBlockRef landing_pad;
   BasicBlockRef catch_block;
   BasicBlockRef end_block;
   LLVMValueRef error_storage;
   Type error_type;
   bool in_catch_block;
} ExceptionContext;

ExceptionContext exception_stack[100];
int exception_stack_pos = -1;

void push_exception_context(BasicBlockRef landing_pad, BasicBlockRef catch_block,
                            BasicBlockRef end_block, LLVMValueRef error_storage, Type error_type) {
   exception_stack_pos++;
   exception_stack[exception_stack_pos].landing_pad = landing_pad;
   exception_stack[exception_stack_pos].catch_block = catch_block;
   exception_stack[exception_stack_pos].end_block = end_block;
   exception_stack[exception_stack_pos].error_storage = error_storage;
   exception_stack[exception_stack_pos].error_type = error_type;
   exception_stack[exception_stack_pos].in_catch_block = false;
}

void pop_exception_context() {
   exception_stack_pos--;
}

ExceptionContext* get_current_exception_context() {
   if (exception_stack_pos < 0) return NULL;
   return &exception_stack[exception_stack_pos];
}

// Helper to get or create exception type info
LLVMValueRef get_type_info_for_type(Type type) {
   const char *typeinfo_name;
   switch (type) {
   case INT: typeinfo_name = "_ZTIi"; break;  // typeinfo for int
   case LONG: typeinfo_name = "_ZTIl"; break; // typeinfo for long
   case CHAR: typeinfo_name = "_ZTIc"; break; // typeinfo for char
   default: typeinfo_name = "_ZTIi"; break;   // default to int
   }

   LLVMValueRef type_info = LLVMGetNamedGlobal(module, typeinfo_name);
   if (!type_info) {
      type_info = LLVMAddGlobal(module, p8, typeinfo_name);
      LLVMSetLinkage(type_info, LLVMExternalLinkage);
   }
   return type_info;
}

// Declare __cxa_* functions
LLVMValueRef get_or_declare_cxa_allocate_exception() {
   LLVMValueRef fn = LLVMGetNamedFunction(module, "__cxa_allocate_exception");
   if (!fn) {
      LLVMTypeRef fn_type = LLVMFunctionType(p8, (LLVMTypeRef[]) {i64}, 1, false);
      fn = LLVMAddFunction(module, "__cxa_allocate_exception", fn_type);
   }
   return fn;
}

LLVMValueRef get_or_declare_cxa_throw() {
   LLVMValueRef fn = LLVMGetNamedFunction(module, "__cxa_throw");
   if (!fn) {
      LLVMTypeRef fn_type = LLVMFunctionType(vd, (LLVMTypeRef[]) {p8, p8, p8}, 3, false);
      fn = LLVMAddFunction(module, "__cxa_throw", fn_type);
      LLVMSetFunctionCallConv(fn, LLVMCCallConv);
      LLVMAddAttributeAtIndex(fn, LLVMAttributeFunctionIndex,
                              LLVMCreateEnumAttribute(context, LLVMGetEnumAttributeKindForName("noreturn", 8), 0));
   }
   return fn;
}

LLVMValueRef get_or_declare_cxa_begin_catch() {
   LLVMValueRef fn = LLVMGetNamedFunction(module, "__cxa_begin_catch");
   if (!fn) {
      LLVMTypeRef fn_type = LLVMFunctionType(p8, (LLVMTypeRef[]) {p8}, 1, false);
      fn = LLVMAddFunction(module, "__cxa_begin_catch", fn_type);
   }
   return fn;
}

LLVMValueRef get_or_declare_cxa_end_catch() {
   LLVMValueRef fn = LLVMGetNamedFunction(module, "__cxa_end_catch");
   if (!fn) {
      LLVMTypeRef fn_type = LLVMFunctionType(vd, NULL, 0, false);
      fn = LLVMAddFunction(module, "__cxa_end_catch", fn_type);
   }
   return fn;
}

LLVMValueRef get_or_declare_personality() {
   LLVMValueRef personality = LLVMGetNamedFunction(module, "__gxx_personality_v0");
   if (!personality) {
      LLVMTypeRef personality_type = LLVMFunctionType(i32, NULL, 0, true);
      personality = LLVMAddFunction(module, "__gxx_personality_v0", personality_type);
   }
   return personality;
}

// Modified function call to use invoke instead of call inside try blocks
void _fcall_invoke(Token *token) {
   Token *funcToken = get_function(token->name);
   LLVMValueRef func = funcToken->llvm.elem;

   int arg_count = token->Fcall.args_len;
   LLVMValueRef *args = NULL;

   if (funcToken->Fdec.is_variadic) {
      int fixed_params = funcToken->Fdec.args_len - 1;
      int variadic_count = arg_count - fixed_params;
      args = calloc(arg_count + 1, sizeof(LLVMValueRef));
      for (int i = 0; i < fixed_params; i++)
         args[i] = token->Fcall.args[i]->llvm.elem;
      args[fixed_params] = LLVMConstInt(i32, variadic_count, 0);
      for (int i = fixed_params; i < arg_count; i++)
         args[i + 1] = token->Fcall.args[i]->llvm.elem;
      arg_count++;
   } else {
      args = calloc(arg_count, sizeof(LLVMValueRef));
      for (int i = 0; i < arg_count; i++)
         args[i] = token->Fcall.args[i]->llvm.elem;
   }

   LLVMTypeRef funcType = LLVMGlobalGetValueType(func);
   LLVMTypeRef retType = LLVMGetReturnType(funcType);
   char *callName = (LLVMGetTypeKind(retType) == LLVMVoidTypeKind) ? "" : token->name;

   // Check if we're inside a try block
   ExceptionContext *ctx = get_current_exception_context();
   if (ctx && ctx->landing_pad) {
      // Use invoke instead of call
      BasicBlockRef normal_dest = _append_block("invoke.cont");
      token->llvm.elem = LLVMBuildInvoke2(builder, funcType, func, args, arg_count,
                                          normal_dest, ctx->landing_pad, callName);
      LLVMPositionBuilderAtEnd(builder, normal_dest);
   } else {
      // Regular call
      token->llvm.elem = LLVMBuildCall2(builder, funcType, func, args, arg_count, callName);
   }

   free(args);
}

// Now update the generate_ir function for TRY and THROW cases:

void generate_ir_try_catch(Node *node) {
   enter_scoop(node);

   // Get current function from the basic block we're in
   BasicBlockRef current_block = LLVMGetInsertBlock(builder);
   if (!current_block) {
      check(1, "try-catch must be inside a function");
      return;
   }
   LLVMValueRef current_func = LLVMGetBasicBlockParent(current_block);
   if (!current_func) {
      check(1, "try-catch must be inside a function");
      return;
   }

   // Set personality function
   LLVMValueRef personality = get_or_declare_personality();
   LLVMSetPersonalityFn(current_func, personality);

   // Create basic blocks
   BasicBlockRef try_block = _append_block("try");
   BasicBlockRef landing_pad_block = _append_block("lpad");
   BasicBlockRef catch_block = _append_block("catch");
   BasicBlockRef end_block = _append_block("try.end");

   // Get catch type
   Type catch_type = node->right ? node->right->token->Catch.error_type : INT;
   TypeRef catch_llvm_type = get_llvm_type(node->right->token);

   // Allocate storage for caught exception
   BasicBlockRef entry = LLVMGetEntryBasicBlock(current_func);
   BasicBlockRef saved_pos = LLVMGetInsertBlock(builder);
   LLVMPositionBuilderAtEnd(builder, entry);
   LLVMValueRef error_storage = LLVMBuildAlloca(builder, catch_llvm_type, "exception.storage");
   LLVMPositionBuilderAtEnd(builder, saved_pos);

   // Push exception context
   push_exception_context(landing_pad_block, catch_block, end_block, error_storage, catch_type);

   // Jump to try block
   _branch(try_block);
   LLVMPositionBuilderAtEnd(builder, try_block);

   // Generate try block body - all function calls will use invoke
   for (int i = 0; i < node->cpos; i++) {
      generate_ir(node->children[i]);
   }

   // If no exception, jump to end
   _branch(end_block);

   // Generate landing pad
   LLVMPositionBuilderAtEnd(builder, landing_pad_block);

   // Create landing pad instruction
   LLVMTypeRef landing_pad_type = LLVMStructTypeInContext(context,
   (LLVMTypeRef[]) {p8, i32}, 2, false);
   LLVMValueRef landing_pad = LLVMBuildLandingPad(builder, landing_pad_type,
                              personality, 1, "lpad");

   // Add catch clause for our type
   LLVMValueRef type_info = get_type_info_for_type(catch_type);
   LLVMAddClause(landing_pad, type_info);

   // Extract exception pointer
   LLVMValueRef exception_ptr = LLVMBuildExtractValue(builder, landing_pad, 0, "exc.ptr");

   // Call __cxa_begin_catch
   LLVMValueRef begin_catch = get_or_declare_cxa_begin_catch();
   LLVMValueRef caught_ptr = LLVMBuildCall2(builder,
                             LLVMGlobalGetValueType(begin_catch), begin_catch,
                             &exception_ptr, 1, "caught");

   // Cast and load the exception value
   LLVMValueRef typed_ptr = LLVMBuildBitCast(builder, caught_ptr,
                            LLVMPointerType(catch_llvm_type, 0), "typed.ptr");
   LLVMValueRef exception_value = LLVMBuildLoad2(builder, catch_llvm_type,
                                  typed_ptr, "exception.value");

   // Store in our error_storage
   LLVMBuildStore(builder, exception_value, error_storage);

   // Jump to catch block
   LLVMBuildBr(builder, catch_block);

// Generate catch block
   LLVMPositionBuilderAtEnd(builder, catch_block);
   ExceptionContext *ctx_for_catch = get_current_exception_context();
   if (ctx_for_catch) {
      ctx_for_catch->in_catch_block = true;
   }

   if (node->right) {
      // Create catch variable pointing to error_storage
      Token *catch_param = calloc(1, sizeof(Token));
      catch_param->type = catch_type;
      catch_param->name = node->right->token->Catch.error_name;
      catch_param->llvm.elem = error_storage;
      add_variable(new_node(catch_param));

      // Generate catch block body
      for (int i = 0; i < node->right->cpos; i++) {
         generate_ir(node->right->children[i]);
      }
   }

   // Mark that we're no longer in the catch block
   if (ctx_for_catch) {
      ctx_for_catch->in_catch_block = false;
   }

   // Only add end_catch and branch if there's no terminator
   // (return statements in catch block will have already added end_catch)
   if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) {
      LLVMValueRef end_catch = get_or_declare_cxa_end_catch();
      LLVMBuildCall2(builder, LLVMGlobalGetValueType(end_catch), end_catch, NULL, 0, "");
      LLVMBuildBr(builder, end_block);
   }

   // Position at end block
   LLVMPositionBuilderAtEnd(builder, end_block);

   // Pop exception context
   pop_exception_context();
   exit_scoop();
}

void generate_ir_throw(Node *node) {
   // Evaluate the expression to throw
   generate_ir(node->left);
   load_if_neccessary(node->left);

   Token *throw_token = node->left->token;
   Type throw_type = throw_token->type;
   TypeRef throw_llvm_type = get_llvm_type(throw_token);

   // Allocate exception object
   LLVMValueRef alloc_exception = get_or_declare_cxa_allocate_exception();
   size_t exception_size = LLVMABISizeOfType(LLVMGetModuleDataLayout(module), throw_llvm_type);
   LLVMValueRef size_val = LLVMConstInt(i64, exception_size, 0);
   LLVMValueRef exception_mem = LLVMBuildCall2(builder,
                                LLVMGlobalGetValueType(alloc_exception), alloc_exception,
                                &size_val, 1, "exception");

   // Cast to appropriate pointer type and store value
   LLVMValueRef typed_exception = LLVMBuildBitCast(builder, exception_mem,
                                  LLVMPointerType(throw_llvm_type, 0), "typed.exception");
   LLVMBuildStore(builder, throw_token->llvm.elem, typed_exception);

   // Get type info
   LLVMValueRef type_info = get_type_info_for_type(throw_type);

   // Call __cxa_throw (destructor is NULL for POD types)
   LLVMValueRef cxa_throw = get_or_declare_cxa_throw();
   LLVMValueRef null_dtor = LLVMConstNull(p8);
   LLVMValueRef throw_args[] = {exception_mem, type_info, null_dtor};
   LLVMBuildCall2(builder, LLVMGlobalGetValueType(cxa_throw), cxa_throw,
                  throw_args, 3, "");

   // Add unreachable after throw
   LLVMBuildUnreachable(builder);

   // Create new block for any potential code after throw (unreachable)
   BasicBlockRef after_throw = _append_block("after.throw");
   LLVMPositionBuilderAtEnd(builder, after_throw);
}