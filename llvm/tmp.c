#include "header.h"

ContextRef context;
ModuleRef module;
BuilderRef builder;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;

ValueRef boundsCheckFunc = NULL;
ValueRef nullCheckFunc = NULL;
ValueRef vaStartFunc = NULL;
ValueRef vaEndFunc = NULL;
char *importedFiles[100];
int importedFileCount = 0;
int block_counter = 0;

void init_varargs_intrinsics()
{
   TypeRef vaStartParams[] = {p8};
   TypeRef vaStartType = LLVMFunctionType(vd, vaStartParams, 1, 0);
   vaStartFunc = LLVMAddFunction(module, "llvm.va_start.p0", vaStartType);

   TypeRef vaEndParams[] = {p8};
   TypeRef vaEndType = LLVMFunctionType(vd, vaEndParams, 1, 0);
   vaEndFunc = LLVMAddFunction(module, "llvm.va_end.p0", vaEndType);
}

void init_bounds_check_function()
{
   // Create function: void bounds_check(i32 index, i32 size, i32 line, i32 column, ptr varname)
   TypeRef paramTypes[] = {i32, i32, i32, i32, p8};
   TypeRef boundsCheckType = LLVMFunctionType(vd, paramTypes, 5, 0);
   boundsCheckFunc = LLVMAddFunction(module, "bounds_check", boundsCheckType);

   // Create the function body
   BasicBlockRef entry = LLVMAppendBasicBlockInContext(context, boundsCheckFunc, "entry");
   BasicBlockRef errorBlock = LLVMAppendBasicBlockInContext(context, boundsCheckFunc, "error");
   BasicBlockRef validBlock = LLVMAppendBasicBlockInContext(context, boundsCheckFunc, "valid");

   LLVMPositionBuilderAtEnd(builder, entry);

   ValueRef index = LLVMGetParam(boundsCheckFunc, 0);
   ValueRef size = LLVMGetParam(boundsCheckFunc, 1);
   ValueRef line = LLVMGetParam(boundsCheckFunc, 2);
   // column parameter (index 3) - reserved for future use
   ValueRef varname = LLVMGetParam(boundsCheckFunc, 4);

   // Check if index < 0
   ValueRef zero = create_int(i32, 0);
   ValueRef isNegative = LLVMBuildICmp(builder, LLVMIntSLT, index, zero, "is_negative");

   // Check if index >= size
   ValueRef isOutOfBounds = LLVMBuildICmp(builder, LLVMIntSGE, index, size, "is_out_of_bounds");

   // Combine conditions: isNegative || isOutOfBounds
   ValueRef isInvalid = LLVMBuildOr(builder, isNegative, isOutOfBounds, "is_invalid");

   LLVMBuildCondBr(builder, isInvalid, errorBlock, validBlock);

   // Error block
   LLVMPositionBuilderAtEnd(builder, errorBlock);

   // Get or declare printf
   ValueRef printfFunc = LLVMGetNamedFunction(module, "printf");
   if (!printfFunc) {
      TypeRef printfParamTypes[] = {p8};
      TypeRef printfType = LLVMFunctionType(i32, printfParamTypes, 1, 1);
      printfFunc = LLVMAddFunction(module, "printf", printfType);
   }
   TypeRef printfType = LLVMGlobalGetValueType(printfFunc);

   // Print segmentation fault header
   ValueRef headerFmt = LLVMBuildGlobalStringPtr(builder,
                        "\nline %d: segmentation fault (array bounds)\n", "err_header");
   ValueRef headerArgs[] = {headerFmt, line};
   LLVMBuildCall2(builder, printfType, printfFunc, headerArgs, 2, "");

   // Print the error message with variable and index
   ValueRef errorFmt = LLVMBuildGlobalStringPtr(builder,
                       "error in accessing %s[%d]\n", "err_msg");
   ValueRef errorArgs[] = {errorFmt, varname, index};
   LLVMBuildCall2(builder, printfType, printfFunc, errorArgs, 3, "");

   // Print pointer indicator (^^^)
   ValueRef pointerFmt = LLVMBuildGlobalStringPtr(builder,
                         "                   ^^^^\n", "err_pointer");
   LLVMBuildCall2(builder, printfType, printfFunc, &pointerFmt, 1, "");

   // Get or declare exit
   ValueRef exitFunc = LLVMGetNamedFunction(module, "exit");
   if (!exitFunc) {
      TypeRef exitParamTypes[] = {i32};
      TypeRef exitType = LLVMFunctionType(vd, exitParamTypes, 1, 0);
      exitFunc = LLVMAddFunction(module, "exit", exitType);
   }
   TypeRef exitType = LLVMGlobalGetValueType(exitFunc);

   ValueRef exitCode = create_int(i32, 1);
   ValueRef exitArgs[] = {exitCode};
   LLVMBuildCall2(builder, exitType, exitFunc, exitArgs, 1, "");
   LLVMBuildUnreachable(builder);

   // Valid block
   LLVMPositionBuilderAtEnd(builder, validBlock);
   LLVMBuildRetVoid(builder);
}

void init(char *name)
{
   context = LLVMContextCreate();
   module = LLVMModuleCreateWithNameInContext(name, context);
   builder = LLVMCreateBuilderInContext(context);

   vd = LLVMVoidTypeInContext(context);
   f32 = LLVMFloatTypeInContext(context);
   i1 = LLVMInt1TypeInContext(context);
   i8 = LLVMInt8TypeInContext(context);
   i16 = LLVMInt16TypeInContext(context);
   i32 = LLVMInt32TypeInContext(context);
   i64 = LLVMInt64TypeInContext(context);
   p8 = LLVMPointerType(i8, 0);
   p32 = LLVMPointerType(i32, 0);

   LLVMInitializeAllTargetInfos();
   LLVMInitializeAllTargets();
   LLVMInitializeAllTargetMCs();
   LLVMInitializeAllAsmParsers();
   LLVMInitializeAllAsmPrinters();
   init_varargs_intrinsics();
   init_bounds_check_function();  // Add this
}

BasicBlockRef create_basic_block(char *name)
{
   char block_name[256];
   snprintf(block_name, sizeof(block_name), "%s.%d", name, block_counter++);
   return LLVMAppendBasicBlockInContext(context, LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder)),
                                        block_name);
}

void branch(BasicBlockRef bloc)
{
   LLVMBuildBr(builder, bloc);
}

void open_block(BasicBlockRef bloc)
{
   LLVMPositionBuilderAtEnd(builder, bloc);
}

void create_function(Token *token)
{
   token->llvm.funcType = LLVMFunctionType(
                             token->llvm.retType,
                             token->llvm.paramTypes,
                             token->llvm.paramCount,
                             token->llvm.is_variadic
                          );

   // Check if function already exists (e.g., printf declared in init_bounds_check_function)
   ValueRef existingFunc = LLVMGetNamedFunction(module, token->name);
   if (existingFunc) {
      token->llvm.elem = existingFunc;
   } else {
      token->llvm.elem = LLVMAddFunction(module, token->name, token->llvm.funcType);
   }
}

ValueRef call_function(Token *token, ValueRef *args, int argCount)
{
   // LLVMTypeRef retType = LLVMGetReturnType(token->llvm.funcType);

   // if (LLVMGetTypeKind(token->llvm.funcType) == LLVMVoidTypeKind) {
   //    return LLVMBuildCall2(builder, token->llvm.funcType, token->llvm.elem, args, argCount, "");
   // }
   char *name = get_named_function(token->name);
   return LLVMBuildCall2(builder, token->llvm.funcType, token->llvm.elem, args, argCount, token->name);
}

ValueRef create_string(char *value)
{
   static int index = 0;
   char name[20];
   snprintf(name, sizeof(name), "STR%d", index++);
   return LLVMBuildGlobalStringPtr(builder, value, name);
}

ValueRef create_int(TypeRef type, long long value)
{
   return LLVMConstInt(type, value, 0);
}

ValueRef create_float(float value)
{
   return LLVMConstReal(f32, value);
}

void create_condition(ValueRef cond, BasicBlockRef isTrue, BasicBlockRef isFalse)
{
   LLVMBuildCondBr(builder, cond, isTrue, isFalse);
}

ValueRef load_variable(TypeRef type, char *name, ValueRef source)
{
   return LLVMBuildLoad2(builder, type, source, name);
}

ValueRef allocate_variable_token(Token *token, Type varType)
{
   TypeRef type = get_llvm_type(varType);
   return LLVMBuildAlloca(builder, type, token->name);
}

ValueRef assign(ValueRef variable, ValueRef value)
{
   LLVMBuildStore(builder, value, variable);
   return variable;
}

ValueRef operation(ValueRef left, Type op, ValueRef right)
{
   switch (op)
   {
   case ADD: return LLVMBuildAdd(builder, left, right, "add");
   case SUB: return LLVMBuildSub(builder, left, right, "sub");
   case EQUAL: return LLVMBuildICmp(builder, LLVMIntEQ, left, right, "eq");
   case NOT_EQUAL: return LLVMBuildICmp(builder, LLVMIntNE, left, right, "ne");
   case LESS: return LLVMBuildICmp(builder, LLVMIntSLT, left, right, "lt");
   case MORE: return LLVMBuildICmp(builder, LLVMIntSGT, left, right, "gt");
   case LESS_EQUAL: return LLVMBuildICmp(builder, LLVMIntSLE, left, right, "le");
   case MORE_EQUAL: return LLVMBuildICmp(builder, LLVMIntSGE, left, right, "ge");
   case MUL: return LLVMBuildMul(builder, left, right, "mul");
   case DIV: return LLVMBuildSDiv(builder, left, right, "div");
   case MOD: return LLVMBuildSRem(builder, left, right, "mod");
   case AND: return LLVMBuildAnd(builder, left, right, "and");
   case OR: return LLVMBuildOr(builder, left, right, "or");
   default: break;
   }

   fprintf(stderr, "Unknown operator: %s\n", to_string(op));
   return NULL;
}

ValueRef index_pointer(ValueRef ptr, ValueRef index)
{
   ValueRef indices[] = {index};
   ValueRef gep = LLVMBuildGEP2(builder, i8, ptr, indices, 1, "idx");
   return LLVMBuildLoad2(builder, i8, gep, "load");
}

ValueRef build_va_start(ValueRef va_list_ptr, ValueRef last_param_ptr)
{
   ValueRef va_list_i8 = LLVMBuildBitCast(builder, va_list_ptr, p8, "va_list_i8");
   ValueRef args[] = {va_list_i8};
   return LLVMBuildCall2(builder, LLVMGlobalGetValueType(vaStartFunc),
                         vaStartFunc, args, 1, "");
}

ValueRef build_va_arg(ValueRef va_list_ptr, TypeRef type)
{
   ValueRef va_list_i8 = LLVMBuildBitCast(builder, va_list_ptr, p8, "va_list_i8");
   return LLVMBuildVAArg(builder, va_list_i8, type, "va_arg");
}

ValueRef build_va_end(ValueRef va_list_ptr)
{
   ValueRef va_list_i8 = LLVMBuildBitCast(builder, va_list_ptr, p8, "va_list_i8");
   ValueRef args[] = {va_list_i8};
   return LLVMBuildCall2(builder, LLVMGlobalGetValueType(vaEndFunc),
                         vaEndFunc, args, 1, "");
}

ValueRef allocate_stack_bytes(ValueRef size)
{
   // Allocate array of i8 (bytes) on the stack
   return LLVMBuildArrayAlloca(builder, i8, size, "stack_alloc");
}

ValueRef cast_value(ValueRef value, TypeRef fromType, TypeRef toType)
{
   // If types are the same, no cast needed
   if (fromType == toType) return value;

   // Pointer to pointer cast (for stack() as chars)
   if (LLVMGetTypeKind(fromType) == LLVMPointerTypeKind &&
         LLVMGetTypeKind(toType) == LLVMPointerTypeKind) {
      return value;  // In LLVM with opaque pointers, all pointers are the same
   }

   // Int to Char (truncate)
   if (fromType == i32 && toType == i8) {
      return LLVMBuildTrunc(builder, value, i8, "i32_to_i8");
   }

   // Char to Int (sign extend)
   if (fromType == i8 && toType == i32) {
      return LLVMBuildSExt(builder, value, i32, "i8_to_i32");
   }

   // Bool to Int (zero extend)
   if (fromType == i1 && toType == i32) {
      return LLVMBuildZExt(builder, value, i32, "i1_to_i32");
   }

   // Int to Bool (compare with 0)
   if (fromType == i32 && toType == i1) {
      ValueRef zero = create_int(i32, 0);
      return LLVMBuildICmp(builder, LLVMIntNE, value, zero, "i32_to_i1");
   }

   fprintf(stderr, "Unsupported cast from type kind %d to %d\n",
           LLVMGetTypeKind(fromType), LLVMGetTypeKind(toType));
   exit(1);
}

void ret(ValueRef value)
{
   if (value) LLVMBuildRet(builder, value);
   else LLVMBuildRetVoid(builder);
}

int verify_module()
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, "Module verification failed:\n%s\n", error);
      LLVMDisposeMessage(error);
      return 0;
   }
   return 1;
}

void finalize()
{
   if (verify_module()) {
      LLVMPrintModuleToFile(module, "out.ll", NULL);
      printf("LLVM IR written to out.ll\n");
   }

   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
   LLVMContextDispose(context);
}

// PARSING
Token *tokens[1000];
int tk_pos = 0;
int exe_pos = 0;
char *input = NULL;

Token *vars[100];
int var_count = 0;

Token *global_vars[100];
int global_var_count = 0;

typedef struct ExceptionContext {
   BasicBlockRef catchBlock;
   BasicBlockRef endBlock;
   ValueRef exceptionValue;
   Token *exceptionVar;
} ExceptionContext;

ExceptionContext exceptionStack[10];
int exceptionStackPos = 0;

ValueRef currentException = NULL;

void push_exception_context(BasicBlockRef catchBlock, BasicBlockRef endBlock,
                            ValueRef exceptionValue, Token *exceptionVar)
{
   exceptionStack[exceptionStackPos].catchBlock = catchBlock;
   exceptionStack[exceptionStackPos].endBlock = endBlock;
   exceptionStack[exceptionStackPos].exceptionValue = exceptionValue;
   exceptionStack[exceptionStackPos].exceptionVar = exceptionVar;
   exceptionStackPos++;
}

ExceptionContext* get_current_exception_context()
{
   if (exceptionStackPos == 0) return NULL;
   return &exceptionStack[exceptionStackPos - 1];
}

void pop_exception_context()
{
   if (exceptionStackPos > 0) exceptionStackPos--;
}

ValueRef throw_exception(ValueRef value)
{
   ExceptionContext *ctx = get_current_exception_context();
   if (!ctx) {
      fprintf(stderr, "Error: throw outside of try-catch block\n");
      exit(1);
   }

   // Store the exception value
   LLVMBuildStore(builder, value, ctx->exceptionValue);

   // Jump to catch block
   LLVMBuildBr(builder, ctx->catchBlock);

   return value;
}

Token* find_variable(char *name) {
   for (int i = var_count - 1; i >= 0; i--) {
      if (strcmp(vars[i]->name, name) == 0) return vars[i];
   }
   for (int i = global_var_count - 1; i >= 0; i--) {
      if (strcmp(global_vars[i]->name, name) == 0) return global_vars[i];
   }
   return NULL;
}

void add_global_variable(Token *token) {
   global_vars[global_var_count++] = token;
}

void add_variable(Token *token) {
   vars[var_count++] = token;
}

void ptoken(Token *token)
{
   printf("token %s ", to_string(token->type));
   if (!token->is_dec)
      switch (token->type)
      {
      case INT: printf("value [%ld]", token->Int.value); break;
      case ID: printf("name [%s]", token->name); break;
      case CHAR: printf("value [%c]", token->Char.value); break;
      case CHARS: printf("value [%s]", token->Chars.value); break;
      default: break;
      }
   printf("\n");
}

bool is_file_imported(char *filename)
{
   for (int i = 0; i < importedFileCount; i++) {
      if (strcmp(importedFiles[i], filename) == 0) {
         return true;
      }
   }
   return false;
}

void mark_file_imported(char *filename)
{
   importedFiles[importedFileCount] = strdup(filename);
   importedFileCount++;
}

void import_module(char *filename)
{
   if (is_file_imported(filename)) {
      printf("Module '%s' already imported, skipping\n", filename);
      return;
   }

   printf("Importing module: %s\n", filename);
   mark_file_imported(filename);

   // Save current parsing state
   char *saved_input = input;
   int saved_tk_pos = tk_pos;

   // Read and tokenize the module file
   input = open_file(filename);
   tokenize();

   // Restore input (but keep tokens)
   free(input);
   input = saved_input;
   tk_pos = saved_tk_pos;
}

Token *new_token(Type type, int line, int pos, int s, int e)
{
   Token *new = calloc(1, sizeof(Token));
   new->type = type;
   new->line = line;
   new->pos = pos;
   switch (type)
   {
   case ID:
   {
      if (e <= s) break;
      struct {char *value; Type type;} special[] = {
         {"int", INT}, {"chars", CHARS}, {"char", CHAR}, {"bool", BOOL},
         {"void", VOID},
         {"va_list", VA_LIST},
         {"def", FDEC}, {"if", IF}, {"while", WHILE},
         {"return", RETURN}, {"end", END_BLOCK}, {"else", ELSE},
         {"protoFunc", PROTO}, {"ref", REF}, {"as", AS}, {"use", USE},
         {"stack", STACK}, {"try", TRY}, {"catch", CATCH}, {"throw", THROW},
         {NULL, 0},
      };
      for (int i = 0; special[i].value; i++)
      {
         if (strncmp(input + s, special[i].value, strlen(special[i].value)) == 0 &&
               (e - s) == strlen(special[i].value))
         {
            new->type = special[i].type;
            if (includes(new->type, INT, CHARS, CHAR, BOOL, VOID, VA_LIST, 0))
               new->is_dec = true;
            break;
         }
      }
      if (new->type != ID) break;
      new->name = substr(input, s, e);
      break;
   }
   case CHAR:
   {
      if (e <= s) break;
      new->Char.value = input[s];
      break;
   }
   case CHARS:
   {
      if (e <= s) break;
      new->Chars.value = substr(input, s, e);
      break;
   }
   case INT:
   {
      if (e <= s) break;
      new->Int.value = atol(input + s);
      break;
   }
   default: break;
   }
   ptoken(new);
   return (tokens[tk_pos++] = new);
}

void tokenize()
{
   int line = 0;
   int pos = 0;

   for (int i = 0; input[i] != '\0'; i++)
   {
      if (input[i] == ' ' || input[i] == '\t')
      {
         pos++;
         continue;
      }

      if (input[i] == '\n')
      {
         new_token(NEWLINE, line, pos, 0, 0);
         pos = 0;
         line++;
         continue;
      }

      if (input[i] == '"')
      {
         int s = i + 1;
         i++;
         while (input[i] != '"' && input[i] != '\0') i++;
         new_token(CHARS, line, pos, s, i);
         pos += (i - s + 2);
         continue;
      }

      if (input[i] == '\'')
      {
         int s = i + 1;
         i++;
         if (input[i] != '\'' && input[i] != '\0') {
            new_token(CHAR, line, pos, s, s + 1);
            i++;
            if (input[i] == '\'') i++;
         }
         pos += 3;
         i--;
         continue;
      }

      if (isalpha(input[i]) || input[i] == '_')
      {
         int s = i;
         while (isalnum(input[i]) || input[i] == '_') i++;
         new_token(ID, line, pos, s, i);
         pos += (i - s);
         i--;
         continue;
      }

      if (isdigit(input[i]))
      {
         int s = i;
         while (isdigit(input[i])) i++;
         new_token(INT, line, pos, s, i);
         pos += (i - s);
         i--;
         continue;
      }

      bool found = false;
      struct {char *value; Type type;} symbols[] = {
         {"...", ELLIPSIS},
         {"&&", AND}, {"||", OR},
         {"==", EQUAL}, {"!=", NOT_EQUAL}, {"<=", LESS_EQUAL}, {">=", MORE_EQUAL},
         {"<", LESS}, {">", MORE},
         {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD},
         {"=", ASSIGN}, {"(", LPAR}, {")", RPAR},
         {":", COLON}, {",", COMMA}, {"[", LBRACKET}, {"]", RBRACKET},
         {NULL, (Type)0},
      };

      for (int j = 0; symbols[j].value; j++)
      {
         char *value = symbols[j].value;
         int len = strlen(value);
         if (strncmp(value, input + i, len) == 0)
         {
            found = true;
            new_token(symbols[j].type, line, pos, 0, 0);
            pos += len;
            i += len - 1;
            break;
         }
      }
      if (found) continue;

      printf("Unknown character [%c] at line %d, pos %d\n", input[i], line, pos);
      exit(1);
   }
   new_token(END, line + 1, 0, 0, 0);
}

void free_tokens()
{
   for (int i = 0; tokens[i]; i++)
   {
      if (tokens[i]->Chars.value) free(tokens[i]->Chars.value);
      if (tokens[i]->name) free(tokens[i]->name);
      free(tokens[i]);
   }
}

Token *find(Type type, ...)
{
   va_list ap;
   va_start(ap, type);
   while (type)
   {
      if (type == tokens[exe_pos]->type)
         return tokens[exe_pos];
      type = va_arg(ap, Type);
   }
   return NULL;
}



Token* parse_expression();
void parse_statement();

Token* parse_primary()
{
   Token *token = tokens[exe_pos];

   if (token->type == STACK)
   {
      exe_pos++;
      if (tokens[exe_pos]->type != LPAR) {
         printf("Error: Expected '(' after stack\n");
         exit(1);
      }
      exe_pos++;

      Token *sizeExpr = parse_expression();

      if (tokens[exe_pos]->type != RPAR) {
         printf("Error: Expected ')' after stack size\n");
         exit(1);
      }
      exe_pos++;

      Token *result = calloc(1, sizeof(Token));
      result->type = CHARS;  // Returns a pointer (chars)
      result->llvm.elem = allocate_stack_bytes(sizeExpr->llvm.elem);
      result->llvm.type = p8;
      result->llvm.arraySize = sizeExpr->llvm.elem;
      return result;
   }

   if (token->type == INT && !token->is_dec)
   {
      exe_pos++;
      token->llvm.elem = create_int(i32, token->Int.value);
      return token;
   }

   if (token->type == CHARS && !token->is_dec)
   {
      exe_pos++;
      token->llvm.elem = create_string(token->Chars.value);
      token->llvm.type = p8;
      return token;
   }

   if (token->type == CHAR && !token->is_dec)
   {
      exe_pos++;
      token->llvm.elem = create_int(i8, token->Char.value);
      token->llvm.type = i8;
      return token;
   }

   if (token->type == ID)
   {
      if (strcmp(token->name, "va_start") == 0)
      {
         exe_pos++;
         if (tokens[exe_pos]->type != LPAR) {
            printf("Error: Expected '(' after va_start\n");
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != ID) {
            printf("Error: Expected va_list variable\n");
            exit(1);
         }

         Token *va_list_var = find_variable(tokens[exe_pos]->name);
         if (!va_list_var) {
            printf("Error: Undefined variable '%s'\n", tokens[exe_pos]->name);
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != COMMA) {
            printf("Error: Expected ',' in va_start\n");
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != ID) {
            printf("Error: Expected parameter name\n");
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != RPAR) {
            printf("Error: Expected ')' after va_start\n");
            exit(1);
         }
         exe_pos++;

         Token *result = calloc(1, sizeof(Token));
         result->llvm.elem = build_va_start(va_list_var->llvm.elem, NULL);
         return result;
      }

      if (strcmp(token->name, "va_arg") == 0)
      {
         exe_pos++;
         if (tokens[exe_pos]->type != LPAR) {
            printf("Error: Expected '(' after va_arg\n");
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != ID) {
            printf("Error: Expected va_list variable\n");
            exit(1);
         }

         Token *va_list_var = find_variable(tokens[exe_pos]->name);
         if (!va_list_var) {
            printf("Error: Undefined variable '%s'\n", tokens[exe_pos]->name);
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != COMMA) {
            printf("Error: Expected ',' in va_arg\n");
            exit(1);
         }
         exe_pos++;

         if (!tokens[exe_pos]->is_dec) {
            printf("Error: Expected type in va_arg\n");
            exit(1);
         }

         Type argType = tokens[exe_pos]->type;
         TypeRef llvmType = get_llvm_type(argType);
         exe_pos++;

         if (tokens[exe_pos]->type != RPAR) {
            printf("Error: Expected ')' after va_arg\n");
            exit(1);
         }
         exe_pos++;

         Token *result = calloc(1, sizeof(Token));
         result->type = argType;
         result->llvm.elem = build_va_arg(va_list_var->llvm.elem, llvmType);
         return result;
      }

      if (strcmp(token->name, "va_end") == 0)
      {
         exe_pos++;
         if (tokens[exe_pos]->type != LPAR) {
            printf("Error: Expected '(' after va_end\n");
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != ID) {
            printf("Error: Expected va_list variable\n");
            exit(1);
         }

         Token *va_list_var = find_variable(tokens[exe_pos]->name);
         if (!va_list_var) {
            printf("Error: Undefined variable '%s'\n", tokens[exe_pos]->name);
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != RPAR) {
            printf("Error: Expected ')' after va_end\n");
            exit(1);
         }
         exe_pos++;

         Token *result = calloc(1, sizeof(Token));
         result->llvm.elem = build_va_end(va_list_var->llvm.elem);
         return result;
      }

      Token *var = find_variable(token->name);
      if (!var) {
         printf("Error: Undefined variable '%s'\n", token->name);
         exit(1);
      }
      exe_pos++;
      if (tokens[exe_pos]->type == LBRACKET)
      {
         exe_pos++;
         Token *indexExpr = parse_expression();

         if (tokens[exe_pos]->type != RBRACKET) {
            printf("Error: Expected ']'\n");
            exit(1);
         }
         exe_pos++;

         // Perform bounds check if size is known
         if (var->llvm.arraySize) {
#if 0
            ValueRef lineNum = create_int(i32, var->line);
            ValueRef colNum = create_int(i32, var->pos);
            ValueRef varName = create_string(var->name);

            ValueRef checkArgs[] = {
               indexExpr->llvm.elem,  // index
               var->llvm.arraySize,   // size
               lineNum,               // line number
               colNum,                // column number
               varName                // variable name
            };

            TypeRef boundsCheckType = LLVMGlobalGetValueType(boundsCheckFunc);
            LLVMBuildCall2(builder, boundsCheckType, boundsCheckFunc, checkArgs, 5, "");
#else
            call_bounds_check(var, indexExpr);
#endif
         }

         // Read from array
         ValueRef ptr = load_variable(get_llvm_type(var->type), var->name, var->llvm.elem);

         Token *result = calloc(1, sizeof(Token));
         result->type = CHAR;
         result->llvm.elem = index_pointer(ptr, indexExpr->llvm.elem);
         return result;  // Returns Token - correct for parse_primary
      }

      if (tokens[exe_pos]->type == LPAR)
      {
         exe_pos++;

         ValueRef args[10];
         int argCount = 0;

         while (tokens[exe_pos]->type != RPAR && tokens[exe_pos]->type != END)
         {
            Token *arg = parse_expression();
            args[argCount++] = arg->llvm.elem;

            if (tokens[exe_pos]->type == COMMA) {
               exe_pos++;
            }
         }

         if (tokens[exe_pos]->type != RPAR) {
            printf("Error: Expected ')' in function call\n");
            exit(1);
         }
         exe_pos++;

         Token *result = calloc(1, sizeof(Token));
         result->llvm.elem = call_function(var, args, argCount);
         return result;
      }

      Token *result = calloc(1, sizeof(Token));
      result->type = var->type;
      result->llvm.elem = load_variable(get_llvm_type(var->type), var->name, var->llvm.elem);
      return result;
   }

   if (token->type == LPAR)
   {
      exe_pos++;
      Token *expr = parse_expression();
      if (tokens[exe_pos]->type != RPAR) {
         printf("Error: Expected ')'\n");
         exit(1);
      }
      exe_pos++;
      return expr;
   }

   printf("Error: Unexpected token in expression: %s\n", to_string(token->type));
   exit(1);
}

Token* parse_cast()
{
   Token *token = tokens[exe_pos];

   // Handle unary minus
   if (token->type == SUB)
   {
      exe_pos++;
      Token *operand = parse_primary();

      Token *result = calloc(1, sizeof(Token));
      result->type = operand->type;

      // Negate the value: 0 - operand
      ValueRef zero;
      if (operand->type == INT) {
         zero = create_int(i32, 0);
      } else if (operand->type == CHAR) {
         zero = create_int(i8, 0);
      } else {
         zero = create_int(i32, 0);
      }

      result->llvm.elem = LLVMBuildSub(builder, zero, operand->llvm.elem, "neg");
      return result;
   }

   if (token->type == ADD)
   {
      exe_pos++;
      return parse_primary();
   }

   Token *left = parse_primary();

   if (tokens[exe_pos]->type == AS)
   {
      exe_pos++;

      if (!tokens[exe_pos]->is_dec) {
         printf("Error: Expected type after 'as'\n");
         exit(1);
      }

      Type targetType = tokens[exe_pos]->type;
      TypeRef targetLLVMType = get_llvm_type(targetType);
      exe_pos++;

      Token *result = calloc(1, sizeof(Token));
      result->type = targetType;
      result->llvm.type = targetLLVMType;

      // Determine source type
      TypeRef sourceType;
      ValueRef sourceValue = left->llvm.elem;

      if (left->llvm.type) {
         sourceType = left->llvm.type;
      } else {
         sourceType = get_llvm_type(left->type);
      }

      result->llvm.elem = cast_value(sourceValue, sourceType, targetLLVMType);

      // CRITICAL: Preserve arraySize through cast
      if (left->llvm.arraySize) {
         result->llvm.arraySize = left->llvm.arraySize;
      }

      return result;
   }

   return left;
}

Token* parse_term()
{
   Token *left = parse_cast();;

   while (find(MUL, DIV, MOD, 0))
   {
      Type op = tokens[exe_pos]->type;
      exe_pos++;
      Token *right = parse_cast();;

      Token *result = calloc(1, sizeof(Token));
      result->type = INT;
      result->llvm.elem = operation(left->llvm.elem, op, right->llvm.elem);

      left = result;
   }

   return left;
}

Token* parse_arithmetic()
{
   Token *left = parse_term();

   while (find(ADD, SUB, 0))
   {
      Type op = tokens[exe_pos]->type;
      exe_pos++;
      Token *right = parse_term();

      Token *result = calloc(1, sizeof(Token));
      result->type = INT;
      result->llvm.elem = operation(left->llvm.elem, op, right->llvm.elem);
      left = result;
   }

   return left;
}

Token* parse_comparison()
{
   Token *left = parse_arithmetic();

   if (find(EQUAL, NOT_EQUAL, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0))
   {
      Type op = tokens[exe_pos]->type;
      exe_pos++;
      Token *right = parse_arithmetic();

      Token *result = calloc(1, sizeof(Token));
      result->type = BOOL;
      result->llvm.elem = operation(left->llvm.elem, op, right->llvm.elem);
      return result;
   }
   return left;
}

Token* parse_logical()
{
   Token *left = parse_comparison();

   while (find(AND, OR, 0))
   {
      Type op = tokens[exe_pos]->type;
      exe_pos++;
      Token *right = parse_comparison();

      Token *result = calloc(1, sizeof(Token));
      result->type = BOOL;
      result->llvm.elem = operation(left->llvm.elem, op, right->llvm.elem);
      left = result;
   }

   return left;
}

Token* parse_expression()
{
   return parse_logical();
}

void skip_newlines()
{
   while (tokens[exe_pos]->type == NEWLINE) exe_pos++;
}

void parse_block()
{
   skip_newlines();

   while (!find(END_BLOCK, END, CATCH, 0))
   {
      parse_statement();
      skip_newlines();
   }
}

void parse_statement()
{
   Token *token = tokens[exe_pos];

   if (token->type == REF)
   {
      exe_pos++;

      if (!tokens[exe_pos]->is_dec) {
         printf("Error: Expected type after 'ref'\n");
         exit(1);
      }

      Type varType = tokens[exe_pos]->type;
      exe_pos++;

      if (tokens[exe_pos]->type != ID) {
         printf("Error: Expected variable name\n");
         exit(1);
      }

      Token *refToken = tokens[exe_pos];
      refToken->type = varType;
      refToken->is_ref = true;
      exe_pos++;

      if (tokens[exe_pos]->type != ASSIGN) {
         printf("Error: References must be initialized\n");
         exit(1);
      }
      exe_pos++;

      if (tokens[exe_pos]->type != ID) {
         printf("Error: Expected variable name after '='\n");
         exit(1);
      }

      Token *sourceVar = find_variable(tokens[exe_pos]->name);
      if (!sourceVar) {
         printf("Error: Undefined variable '%s'\n", tokens[exe_pos]->name);
         exit(1);
      }
      exe_pos++;

      refToken->llvm.elem = sourceVar->llvm.elem;
      add_variable(refToken);

      skip_newlines();
      return;
   }

   if (token->is_dec)
   {
      Type varType = token->type;
      exe_pos++;

      if (tokens[exe_pos]->type != ID) {
         printf("Error: Expected variable name\n");
         exit(1);
      }

      Token *varToken = tokens[exe_pos];
      varToken->type = varType;
      varToken->is_ref = false;
      exe_pos++;

      varToken->llvm.elem = allocate_variable_token(varToken, varType);
      add_variable(varToken);

      if (tokens[exe_pos]->type == ASSIGN)
      {
         exe_pos++;
         Token *value = parse_expression();
         assign(varToken->llvm.elem, value->llvm.elem);

         if (value->llvm.arraySize) {
            varToken->llvm.arraySize = value->llvm.arraySize;
         }
      }

      skip_newlines();
      return;
   }

   if (token->type == ID)
   {
      if (strcmp(token->name, "va_start") == 0 ||
            strcmp(token->name, "va_arg") == 0 ||
            strcmp(token->name, "va_end") == 0)
      {
         parse_expression();
         skip_newlines();
         return;
      }

      Token *var = find_variable(token->name);
      if (var && tokens[exe_pos + 1]->type == LPAR)
      {
         parse_expression();
         skip_newlines();
         return;
      }

      if (!var) {
         printf("Error: Undefined variable '%s'\n", token->name);
         exit(1);
      }

      exe_pos++;

      // Handle array index assignment: a[index] = value
      // Handle array index assignment: a[index] = value
      if (tokens[exe_pos]->type == LBRACKET)
      {
         exe_pos++;
         Token *indexExpr = parse_expression();

         if (tokens[exe_pos]->type != RBRACKET) {
            printf("Error: Expected ']'\n");
            exit(1);
         }
         exe_pos++;

         if (tokens[exe_pos]->type != ASSIGN) {
            printf("Error: Expected '=' after array index\n");
            exit(1);
         }
         exe_pos++;

         Token *value = parse_expression();

         // Perform bounds check if size is known
         if (var->llvm.arraySize) {
#if 0
            ValueRef lineNum = create_int(i32, var->line);
            ValueRef colNum = create_int(i32, var->pos);
            ValueRef varName = create_string(var->name);

            ValueRef checkArgs[] = {
               indexExpr->llvm.elem,  // index
               var->llvm.arraySize,   // size
               lineNum,               // line number
               colNum,                // column number
               varName                // variable name
            };

            TypeRef boundsCheckType = LLVMGlobalGetValueType(boundsCheckFunc);
            LLVMBuildCall2(builder, boundsCheckType, boundsCheckFunc, checkArgs, 5, "");
#else
            call_bounds_check(var, indexExpr);
#endif
         }

         // Perform the assignment (WRITE to array)
         ValueRef ptr = load_variable(get_llvm_type(var->type), var->name, var->llvm.elem);
         ValueRef indices[] = {indexExpr->llvm.elem};
         ValueRef gep = LLVMBuildGEP2(builder, i8, ptr, indices, 1, "idx");
         LLVMBuildStore(builder, value->llvm.elem, gep);

         skip_newlines();
         return;  // CRITICAL: Return here after assignment
      }
      // Regular assignment: a = value
      if (tokens[exe_pos]->type != ASSIGN) {
         printf("Error: Expected '=' after variable name\n");
         exit(1);
      }

      exe_pos++;
      Token *value = parse_expression();
      assign(var->llvm.elem, value->llvm.elem);

      // Preserve array size information
      if (value->llvm.arraySize) {
         var->llvm.arraySize = value->llvm.arraySize;
      }

      skip_newlines();
      return;
   }

   if (token->type == IF)
   {
      exe_pos++;
      Token *cond = parse_expression();

      if (tokens[exe_pos]->type != COLON) {
         printf("Error: Expected ':' after if condition\n");
         exit(1);
      }
      exe_pos++;

      BasicBlockRef thenBlock = create_basic_block("if.then");
      BasicBlockRef elseBlock = create_basic_block("if.else");
      BasicBlockRef mergeBlock = create_basic_block("if.merge");

      create_condition(cond->llvm.elem, thenBlock, elseBlock);

      open_block(thenBlock);
      parse_block();

      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) {
         branch(mergeBlock);
      }

      if (tokens[exe_pos]->type == END_BLOCK) exe_pos++;

      skip_newlines();
      bool hasElse = false;
      if (tokens[exe_pos]->type == ELSE) {
         hasElse = true;
         exe_pos++;
         if (tokens[exe_pos]->type != COLON) {
            printf("Error: Expected ':' after else\n");
            exit(1);
         }
         exe_pos++;
      }

      open_block(elseBlock);
      if (hasElse) {
         parse_block();
         if (tokens[exe_pos]->type == END_BLOCK) exe_pos++;
      }

      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) {
         branch(mergeBlock);
      }

      open_block(mergeBlock);
      skip_newlines();
      return;
   }

   if (token->type == WHILE)
   {
      exe_pos++;

      BasicBlockRef condBlock = create_basic_block("while.cond");
      BasicBlockRef bodyBlock = create_basic_block("while.body");
      BasicBlockRef endBlock = create_basic_block("while.end");

      branch(condBlock);
      open_block(condBlock);

      Token *cond = parse_expression();

      if (tokens[exe_pos]->type != COLON) {
         printf("Error: Expected ':' after while condition\n");
         exit(1);
      }
      exe_pos++;

      create_condition(cond->llvm.elem, bodyBlock, endBlock);

      open_block(bodyBlock);
      parse_block();
      branch(condBlock);

      if (tokens[exe_pos]->type == END_BLOCK) exe_pos++;

      open_block(endBlock);
      skip_newlines();
      return;
   }
   if (token->type == TRY)
   {
      exe_pos++;
      if (tokens[exe_pos]->type != COLON) {
         printf("Error: Expected ':' after try\n");
         exit(1);
      }
      exe_pos++;

      // Create blocks
      BasicBlockRef tryBlock = create_basic_block("try");
      BasicBlockRef catchBlock = create_basic_block("catch");
      BasicBlockRef endBlock = create_basic_block("try.end");

      // Create exception value storage
      ValueRef exceptionValue = LLVMBuildAlloca(builder, i32, "exception.value");

      // Jump to try block
      branch(tryBlock);
      open_block(tryBlock);

      // Parse try block with exception context
      // We'll set up the exception var later when we parse catch
      push_exception_context(catchBlock, endBlock, exceptionValue, NULL);

      parse_block();

      // If we reach here without throwing, jump to end
      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) {
         branch(endBlock);
      }

      if (tokens[exe_pos]->type == END_BLOCK) exe_pos++;
      skip_newlines();

      // Parse catch block
      if (tokens[exe_pos]->type != CATCH) {
         printf("Error: Expected 'catch' after try block\n");
         exit(1);
      }
      exe_pos++;

      // Parse exception variable declaration
      if (!tokens[exe_pos]->is_dec) {
         printf("Error: Expected type in catch\n");
         exit(1);
      }

      Type exceptionType = tokens[exe_pos]->type;
      exe_pos++;

      if (tokens[exe_pos]->type != ID) {
         printf("Error: Expected exception variable name\n");
         exit(1);
      }

      Token *exceptionVar = tokens[exe_pos];
      exceptionVar->type = exceptionType;
      exe_pos++;

      if (tokens[exe_pos]->type != COLON) {
         printf("Error: Expected ':' after catch declaration\n");
         exit(1);
      }
      exe_pos++;

      // Set up catch block
      open_block(catchBlock);

      // Load the exception value into the exception variable
      exceptionVar->llvm.elem = LLVMBuildAlloca(builder, get_llvm_type(exceptionType),
                                exceptionVar->name);
      ValueRef thrownValue = load_variable(i32, "thrown", exceptionValue);
      LLVMBuildStore(builder, thrownValue, exceptionVar->llvm.elem);

      add_variable(exceptionVar);

      // Parse catch block body
      parse_block();

      if (!LLVMGetBasicBlockTerminator(LLVMGetInsertBlock(builder))) {
         branch(endBlock);
      }

      if (tokens[exe_pos]->type == END_BLOCK) exe_pos++;

      // Pop exception context
      pop_exception_context();

      // Continue with end block
      open_block(endBlock);
      skip_newlines();
      return;
   }
   if (token->type == THROW)
   {
      exe_pos++;
      Token *value = parse_expression();
      throw_exception(value->llvm.elem);
      skip_newlines();
      return;
   }

   if (token->type == RETURN)
   {
      exe_pos++;
      if (tokens[exe_pos]->type == NEWLINE || tokens[exe_pos]->type == END_BLOCK) {
         ret(NULL);
      } else {
         Token *value = parse_expression();
         ret(value->llvm.elem);
      }
      skip_newlines();
      return;
   }

   if (token->type == NEWLINE) {
      exe_pos++;
      return;
   }

   printf("Error: Unexpected statement token: %s\n", to_string(token->type));
   exit(1);
}

void compile()
{
   init("module");

   while (tokens[exe_pos]->type != END)
   {
      skip_newlines();
      if (tokens[exe_pos]->type == END) break;

      if (tokens[exe_pos]->type == USE)
      {
         exe_pos++;

         if (tokens[exe_pos]->type != CHARS) {
            printf("Error: Expected filename string after 'use'\n");
            exit(1);
         }

         char *filename = tokens[exe_pos]->Chars.value;
         exe_pos++;

         import_module(filename);

         skip_newlines();
         continue;
      }

      if (tokens[exe_pos]->type == PROTO)
      {
         exe_pos++;

         if (!tokens[exe_pos]->is_dec) {
            printf("Error: Expected return type after 'protoFunc'"
                   " found [%s] [%s]\n", to_string(tokens[exe_pos]->type),
                   tokens[exe_pos]->name
                  );
            exit(1);
         }

         Type retType = tokens[exe_pos]->type;
         exe_pos++;

         if (tokens[exe_pos]->type != ID) {
            printf("Error: Expected function name\n");
            exit(1);
         }

         Token *funcToken = tokens[exe_pos];
         exe_pos++;

         TypeRef paramTypes[10];
         int paramCount = 0;
         bool isVariadic = false;

         if (tokens[exe_pos]->type == LPAR) {
            exe_pos++;

            while (tokens[exe_pos]->type != RPAR && tokens[exe_pos]->type != END)
            {
               if (tokens[exe_pos]->type == ELLIPSIS) {
                  isVariadic = true;
                  exe_pos++;
                  break;
               }

               if (tokens[exe_pos]->is_dec) {
                  paramTypes[paramCount++] = get_llvm_type(tokens[exe_pos]->type);
                  exe_pos++;

                  if (tokens[exe_pos]->type == ID) {
                     exe_pos++;
                  }

                  if (tokens[exe_pos]->type == COMMA) {
                     exe_pos++;
                  }
               } else {
                  exe_pos++;
               }
            }

            if (tokens[exe_pos]->type == RPAR) exe_pos++;
         }

         funcToken->llvm.retType = get_llvm_type(retType);
         funcToken->llvm.paramTypes = malloc(paramCount * sizeof(TypeRef));
         memcpy(funcToken->llvm.paramTypes, paramTypes, paramCount * sizeof(TypeRef));
         funcToken->llvm.paramCount = paramCount;
         funcToken->llvm.is_variadic = isVariadic;

         create_function(funcToken);
         add_global_variable(funcToken);

         printf("Declared function prototype: %s (variadic=%d)\n", funcToken->name, isVariadic);
         skip_newlines();
         continue;
      }

      if (tokens[exe_pos]->type == FDEC)
      {
         exe_pos++;

         if (!tokens[exe_pos]->is_dec) {
            printf("Error: Expected return type after 'def'\n");
            exit(1);
         }

         Type retType = tokens[exe_pos]->type;
         exe_pos++;

         if (tokens[exe_pos]->type != ID) {
            printf("Error: Expected function name\n");
            exit(1);
         }

         Token *funcToken = tokens[exe_pos];
         exe_pos++;

         TypeRef paramTypes[10];
         Token *paramTokens[10];
         int paramCount = 0;
         bool isVariadic = false;

         if (tokens[exe_pos]->type == LPAR) {
            exe_pos++;

            while (tokens[exe_pos]->type != RPAR && tokens[exe_pos]->type != END)
            {
               if (tokens[exe_pos]->type == ELLIPSIS) {
                  isVariadic = true;
                  exe_pos++;
                  break;
               }

               if (tokens[exe_pos]->is_dec) {
                  Type paramType = tokens[exe_pos]->type;
                  paramTypes[paramCount] = get_llvm_type(paramType);
                  exe_pos++;

                  if (tokens[exe_pos]->type == ID) {
                     Token *paramToken = tokens[exe_pos];
                     paramToken->type = paramType;
                     paramTokens[paramCount] = paramToken;
                     paramCount++;
                     exe_pos++;
                  }

                  if (tokens[exe_pos]->type == COMMA) {
                     exe_pos++;
                  }
               } else {
                  exe_pos++;
               }
            }

            if (tokens[exe_pos]->type == RPAR) exe_pos++;
         }

         if (tokens[exe_pos]->type != COLON) {
            printf("Error: Expected ':' after function declaration\n");
            exit(1);
         }
         exe_pos++;

         funcToken->llvm.retType = get_llvm_type(retType);
         funcToken->llvm.paramTypes = malloc(paramCount * sizeof(TypeRef));
         memcpy(funcToken->llvm.paramTypes, paramTypes, paramCount * sizeof(TypeRef));
         funcToken->llvm.paramCount = paramCount;
         funcToken->llvm.is_variadic = isVariadic;

         create_function(funcToken);
         add_global_variable(funcToken);

         BasicBlockRef entry = LLVMAppendBasicBlockInContext(context, funcToken->llvm.elem, "entry");
         LLVMPositionBuilderAtEnd(builder, entry);

         var_count = 0;

         for (int i = 0; i < paramCount; i++) {
            Token *param = paramTokens[i];
            ValueRef paramValue = LLVMGetParam(funcToken->llvm.elem, i);

            param->llvm.elem = LLVMBuildAlloca(builder, paramTypes[i], param->name);
            LLVMBuildStore(builder, paramValue, param->llvm.elem);

            add_variable(param);
         }

         parse_block();

         if (tokens[exe_pos]->type == END_BLOCK) exe_pos++;

         printf("Compiled function: %s (variadic=%d)\n", funcToken->name, isVariadic);

         skip_newlines();
      }
      else
      {
         printf("Error: Expected function definition or prototype, got %s\n",
                to_string(tokens[exe_pos]->type));
         exit(1);
      }
   }

   finalize();
}

int main(int argc, char **argv)
{
   (void)argc;
   input = open_file(argv[1]);
   tokenize();
   compile();
   free_tokens();
   return 0;
}