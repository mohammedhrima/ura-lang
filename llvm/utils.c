#include "header.h"

char *importedFiles[100];
int importedFileCount;
int block_counter;
Token *tokens[1000];
int tk_pos;
int exe_pos;
char *input;
Context context;
Module module;
Builder builder;
TypeRef vd, f32, i1, i8, i16, i32, i64, p8, p32;

Node *Gscoop[100];
int scoop_pos = -1;
Node *scoop;
bool enable_bounds_check = false;

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
            {
               if (new->type == CHARS) new->is_ref = true;
               new->is_dec = true;
            }
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
         char str[255];
         memset(str, 0, sizeof(str));
         str['n'] = '\n';
         str['t'] = '\t';
         str['\''] = '\'';
         str['0'] = '\0';
         str['\\'] = '\\';
         str['\"'] = '\"';
         str['r'] = '\r';

         c = str[(unsigned char)input[s + 1]];
         if (c == 0 && input[s + 1] != '0')
            check(1, "Unknown escape sequence '\\%c'\n", input[s + 1]);

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
   int line = 1, pos = 0, space = 0;
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

ExcepCTX *exceps;
int exceps_len;
int exceps_pos = -1;

void push_exception_context(Block lpad, Block catch, Block end, Value storage,
                            Type type) {
   if (exceps_len == 0)
   {
      exceps = calloc(100, sizeof( ExcepCTX));
      exceps_len = 100;
   }
   else if (exceps_pos + 1 == exceps_len)
   {
      ExcepCTX *tmp = calloc(exceps_len *= 2, sizeof( ExcepCTX));
      memcpy(tmp, exceps, exceps_pos * sizeof( ExcepCTX));
      free(exceps);
      exceps = tmp;
   }
   exceps_pos++;
   exceps[exceps_pos].lpad = lpad;
   exceps[exceps_pos].catch = catch;
   exceps[exceps_pos].end = end;
   exceps[exceps_pos].storage = storage;
   exceps[exceps_pos].type = type;
   exceps[exceps_pos].in_catch = false;
}

void pop_exception_context() {
   exceps_pos--;
}

ExcepCTX* get_current_exception_context() {
   if (exceps_pos < 0) return NULL;
   return &exceps[exceps_pos];
}

Value get_type_info_for_type(Type type) {
   const char *typeinfo_name;
   switch (type) {
   case INT: typeinfo_name = "_ZTIi"; break;
   case LONG: typeinfo_name = "_ZTIl"; break;
   case CHAR: typeinfo_name = "_ZTIc"; break;
   default: typeinfo_name = "_ZTIi"; break;
   }

   Value type_info = LLVMGetNamedGlobal(module, typeinfo_name);
   if (!type_info) {
      type_info = LLVMAddGlobal(module, p8, typeinfo_name);
      LLVMSetLinkage(type_info, LLVMExternalLinkage);
   }
   return type_info;
}

Value get_exception() {
   Value fn = LLVMGetNamedFunction(module, "__cxa_allocate_exception");
   if (!fn) {
      TypeRef fn_type = LLVMFunctionType(p8, (TypeRef[]) {i64}, 1, false);
      fn = LLVMAddFunction(module, "__cxa_allocate_exception", fn_type);
   }
   return fn;
}

Value get_throw() {
   Value fn = LLVMGetNamedFunction(module, "__cxa_throw");
   if (!fn) {
      TypeRef fn_type = LLVMFunctionType(vd, (TypeRef[]) {p8, p8, p8}, 3, false);
      fn = LLVMAddFunction(module, "__cxa_throw", fn_type);
      LLVMSetFunctionCallConv(fn, LLVMCCallConv);
      LLVMAddAttributeAtIndex(fn, LLVMAttributeFunctionIndex,
                              LLVMCreateEnumAttribute(context, LLVMGetEnumAttributeKindForName("noreturn", 8), 0));
   }
   return fn;
}

Value get_begin_catch() {
   Value fn = LLVMGetNamedFunction(module, "__cxa_begin_catch");
   if (!fn) {
      TypeRef fn_type = LLVMFunctionType(p8, (TypeRef[]) {p8}, 1, false);
      fn = LLVMAddFunction(module, "__cxa_begin_catch", fn_type);
   }
   return fn;
}

Value get_end_catch() {
   Value fn = LLVMGetNamedFunction(module, "__cxa_end_catch");
   if (!fn) {
      TypeRef fn_type = LLVMFunctionType(vd, NULL, 0, false);
      fn = LLVMAddFunction(module, "__cxa_end_catch", fn_type);
   }
   return fn;
}

Value get_personality() {
   Value personality = LLVMGetNamedFunction(module, "__gxx_personality_v0");
   if (!personality) {
      TypeRef personality_type = LLVMFunctionType(i32, NULL, 0, true);
      personality = LLVMAddFunction(module, "__gxx_personality_v0", personality_type);
   }
   return personality;
}

void finalize()
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, RED"Module verification failed:\n%s\n"RESET, error);
      LLVMDisposeMessage(error);
   }
   LLVMPrintModuleToFile(module, "build/out.ll", NULL);

   LLVMDisposeBuilder(builder);
   LLVMDisposModule(module);
   LLVMContextDispose(context);
}

bool includes(Type to_find, ...)
{
   va_list ap; Type current; va_start(ap, to_find);
   while ((current = va_arg(ap, Type)) != 0)
      if (current == to_find) return true;
   return false;
}

char *substr(char *input, int s, int e)
{
   char *res = calloc(e - s + 1, sizeof(char));
   strncpy(res, input + s, e - s);
   return res;
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

Token *copy_token(Token *token)
{
   if (token == NULL) return NULL;
   Token *new = calloc(1, sizeof(Token));
   memcpy(new, token, sizeof(Token));
   if (token->name) new->name = strdup(token->name);
   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   tokens[tk_pos++] = new;
   return new;
}

char* open_file(char *filename)
{
   char dir[100000] = {};
   getwd(dir);
   filename = strjoin(dir, "/", filename, NULL);
   File file = fopen(filename, "r");
   if (!file)
   {
      printf("Error: openning file [%s]", filename);
      free(filename);
      exit(1);
   }
   fseek(file, 0, SEEK_END);
   int size = ftell(file);
   fseek(file, 0, SEEK_SET);
   char *input = calloc((size + 1), sizeof(char));
   if (input) fread(input, size, sizeof(char), file);
   fclose(file);
   free(filename);
   return input;
}

char *strjoin(char *str, ...)
{
   if (!str) return NULL;

   va_list ap;
   size_t len = 0;

   va_start(ap, str);
   char *ptr = str;
   while (ptr)
   {
      len += strlen(ptr);
      ptr = va_arg(ap, char *);
   }
   va_end(ap);

   char *res = calloc(len + 1, 1);
   if (!res) return NULL;

   char *dst = res;
   va_start(ap, str);
   ptr = str;
   while (ptr)
   {
      size_t l = strlen(ptr);
      memcpy(dst, ptr, l);
      dst += l;
      ptr = va_arg(ap, char *);
   }
   va_end(ap);

   return res;
}

void ptoken(Token *token)
{
   if (TESTING) return;
   if (!token)
   {
      printf("token NULL \n");
      return;
   }
   printf("token %s ", to_string(token->type));
   if (token->name) printf("[%s] ", token->name);

   if (token->is_dec) printf("is dec ");
   else
      switch (token->type)
      {
      case INT: printf("%ld ", token->Int.value); break;
      case CHAR: printf("[%c] ", token->Char.value); break;
      case CHARS: printf("[%s] ", token->Chars.value); break;
      default: break;
      }
   if (token->rTypeRef) printf("ret [%s] ", to_string(token->rTypeRef));
   if (token->is_ref) printf("is_ref");
   printf(" space [%d]\n", token->space);
}

void pnode(Node *node, char *side, int space)
{
   if (!node || TESTING) return;
#if !TESTING
   int i = 0;
   while (i < space) i += printf(" ");
#endif
   if (side) printf("%s ", side);
   printf("node "); ptoken(node->token);
   pnode(node->left, "L:", space + TAB);
   pnode(node->right, "R:", space + TAB);
   switch (node->token->type)
   {
   case FDEC:
   {
      for (int i = 0; i < node->cpos; i++)
         pnode(node->children[i], NULL, space + TAB);
      break;
   }
   default:
      break;
   }
}

char *to_string(Type type)
{
   char* res[END + 1] = {
      [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
      [INT] = "INT", [BOOL] = "BOOL", [NEWLINE] = "NEWLINE", [FDEC] = "FDEC",
      [FCALL] = "FCALL", [END] = "END", [LPAR] = "LPAR", [RPAR] = "RPAR",
      [IF] = "IF", [ELIF] = "ELIF", [ELSE] = "ELSE", [WHILE] = "WHILE",
      [RETURN] = "RETURN", [END_BLOCK] = "END_BLOCK", [ADD] = "ADD",
      [SUB] = "SUB", [MUL] = "MUL", [DIV] = "DIV", [ASSIGN] = "ASSIGN",
      [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
      [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
      [MOD_ASSIGN] = "MOD_ASSIGN", [ACCESS] = "ACCESS",
      [MOD] = "MOD", [COMA] = "COMA", [REF] = "REF",
      [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT_EQUAL", [LESS] = "LESS",
      [MORE] = "MORE", [LESS_EQUAL] = "LESS_EQUAL",
      [MORE_EQUAL] = "MORE_EQUAL", [AND] = "AND", [OR] = "OR",
      [DOTS] = "DOTS", [COLON] = "COLON", [COMMA] = "COMMA",
      [LBRACKET] = "LBRACKET", [RBRACKET] = "RBRACKET",
      [PROTO] = "PROTO", [VARIADIC] = "VARIADIC",
      [VA_LIST] = "VA_LIST", [AS] = "AS", [STACK] = "STACK",
      [TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW",
      [USE] = "USE", [LBRA] = "LBRA", [RBRA] = "RBRA",
      [DOT] = "DOT",
   };
   if (!res[type])
   {
      printf("%s:%d handle this case %d\n", __FILE__, __LINE__, type);
      exit(1);
   }
   return res[type];
}

void create_bounds_check_function() {
   Value existing = LLVMGetNamedFunction(module, "__bounds_check");
   if (existing) return;

   // void __bounds_check(i32 index, i32 size, i32 line, ptr filename)
   TypeRef param_types[] = {i32, i32, i32, p8};
   TypeRef func_type = LLVMFunctionType(vd, param_types, 4, false);
   Value func = LLVMAddFunction(module, "__bounds_check", func_type);

   Block entry = LLVMAppendBasicBlockInContext(context, func, "entry");
   Block error_block = LLVMAppendBasicBlockInContext(context, func, "error");
   Block ok_block = LLVMAppendBasicBlockInContext(context, func, "ok");

   LLVMPositionBuilderAtEnd(builder, entry);

   Value index_param = LLVMGetParam(func, 0);
   Value size_param = LLVMGetParam(func, 1);
   Value line_param = LLVMGetParam(func, 2);
   Value filename_param = LLVMGetParam(func, 3);

   // Check: index >= 0 && index < size
   Value cmp_negative = LLVMBuildICmp(builder, LLVMIntSLT, index_param,
                                       LLVMConstInt(i32, 0, 0), "is_negative");
   Value cmp_overflow = LLVMBuildICmp(builder, LLVMIntSGE, index_param,
                                       size_param, "is_overflow");
   Value is_bad = LLVMBuildOr(builder, cmp_negative, cmp_overflow, "is_bad");

   LLVMBuildCondBr(builder, is_bad, error_block, ok_block);

   // Error block
   LLVMPositionBuilderAtEnd(builder, error_block);

   // Get printf function (simpler than fprintf with stderr)
   Value printf_func = LLVMGetNamedFunction(module, "printf");
   if (!printf_func) {
      TypeRef printf_type = LLVMFunctionType(i32, (TypeRef[]) {p8}, 1, true);
      printf_func = LLVMAddFunction(module, "printf", printf_type);
   }

   // Error messages
   Value fmt_header = LLVMBuildGlobalStringPtr(builder,
                       "\n\033[1m\033[31mruntime error:\033[0m array index out of bounds\n", "fmt_header");
   Value fmt_location = LLVMBuildGlobalStringPtr(builder,
                         "\033[1m%s:%d:\033[0m ", "fmt_location");
   Value fmt_error = LLVMBuildGlobalStringPtr(builder,
                      "\033[1m\033[31merror:\033[0m index \033[1m%d\033[0m is out of bounds for array of size \033[1m%d\033[0m\n\n",
                      "fmt_error");

   // Print error using printf
   LLVMBuildCall2(builder, LLVMGlobalGetValuTypeRef(printf_func), printf_func,
   (Value[]) {fmt_header}, 1, "");
   LLVMBuildCall2(builder, LLVMGlobalGetValuTypeRef(printf_func), printf_func,
   (Value[]) {fmt_location, filename_param, line_param}, 3, "");
   LLVMBuildCall2(builder, LLVMGlobalGetValuTypeRef(printf_func), printf_func,
   (Value[]) {fmt_error, index_param, size_param}, 3, "");

   // Abort
   Value abort_func = LLVMGetNamedFunction(module, "abort");
   if (!abort_func) {
      TypeRef abort_type = LLVMFunctionType(vd, NULL, 0, false);
      abort_func = LLVMAddFunction(module, "abort", abort_type);
      LLVMAddAttributeAtIndex(abort_func, LLVMAttributeFunctionIndex,
                              LLVMCreateEnumAttribute(context, LLVMGetEnumAttributeKindForName("noreturn", 8), 0));
   }
   LLVMBuildCall2(builder, LLVMGlobalGetValuTypeRef(abort_func), abort_func, NULL, 0, "");
   LLVMBuildUnreachable(builder);

   // OK block
   LLVMPositionBuilderAtEnd(builder, ok_block);
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
   LLVMSetTarget(module, LLVMGetDefaultTargetTriple());
}
