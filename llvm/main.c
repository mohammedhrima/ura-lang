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
      struct { char *value; Type type; } special[] = {
         {"int", INT}, {"chars", CHARS}, {"char", CHAR}, {"bool", BOOL},
         {"void", VOID}, {"va_list", VA_LIST}, {"def", FDEC}, {"if", IF},
         {"while", WHILE}, {"return", RETURN}, {"end", END_BLOCK},
         {"else", ELSE}, {"protoFunc", PROTO}, {"ref", REF},
         {"as", AS}, {"use", USE}, {"stack", STACK}, {"try", TRY},
         {"catch", CATCH}, {"throw", THROW}, {NULL, 0}
      };
      new->name = substr(input, s, e);

      for (int i = 0; special[i].value; i++)
      {
         if (strcmp(new->name, special[i].value) == 0)
         {
            new->type = special[i].type;
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

void skip(char l, char r)
{
   if (l == r) return;
   printf("%s:%d expected [%c] found [%c]\n", __FILE__, __LINE__, l, r);
   seg();
}

void tokenize()
{
   int line = 0, pos = 0, space = 0;
   // bool skiping = false;
   for (int i = 0; input[i];)
   {
      if (isspace(input[i]))
      {
         if (input[i] == '\n')
         {
            line++;
            pos = 0;
         }
         else
         {
            pos++;
         }
         i++;
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
         skip(input[i], '"');
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
         skip(input[i], '\'');
         i++;
      }
      else if (isalpha(input[i]) || input[i] == '_')
      {
         int s = i;
         while (isalnum(input[i]) || input[i] == '_') i++;
         char *tmp = substr(input, s, i);
         // printf("[%s]\n", tmp);
         free(tmp);
         // exit(1);
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
            {"/=", DIV_ASSIGN},
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
      // printf("<%d>\n", i);
   }
   new_token(END, 0, 0, 0, 0, 0);
}

Node *assign();
Node *add_sub();
Node *mul_div();
Node *prime();

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

Node *expr()
{
   return assign();
}

Node *assign()
{
   Node *left = logic();
   Token *token;
   while ((token = find(ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, 0)))
   {
      Node *node = new_node(token);
      if (token->type == ASSIGN)
      {
         node->left = left;
         node->right = logic();
      }
      else
      {
         node->left = left;
         Type type = (Type)0;
         switch (token->type)
         {
         case ADD_ASSIGN: type = ADD; break;
         case SUB_ASSIGN: type = SUB; break;
         case MUL_ASSIGN: type = MUL; break;
         case DIV_ASSIGN: type = DIV; break;
         case MOD_ASSIGN: type = MOD; break;
         default: break;
         }
         node->right = new_node(new_token(type, 0, 0, 0, 0,
                                          node->token->space));
         node->right->left = new_node(left->token);
         node->right->right = logic();

         node->token->type = ASSIGN;
      }
      left = node;
   }
   return left;
}

AST_NODE(logic, equality, AND, OR, 0);
AST_NODE(equality, comparison, EQUAL, NOT_EQUAL, 0);
AST_NODE(comparison, add_sub, LESS, MORE, LESS_EQUAL, MORE_EQUAL, 0);
AST_NODE(add_sub, mul_div, ADD, SUB, 0);
AST_NODE(mul_div, prime, MUL, DIV, MOD, 0);

Node *prime()
{
   Token *token;
   Node *node = NULL;
   if ((token = find(INT, CHARS, CHAR, ID, 0)))
   {
      if (token->is_dec)
      {
         Token *fname = find(ID, 0);
         check(!fname, "");
         token->name = fname->name;
         fname->name = NULL;
         return new_node(token);
      }
      else if (find(LPAR, 0))
      {
         token->type = FCALL;
         Token *arg = NULL;
         int len = 0;
         int p = 0;
         do {
            arg = find(RPAR, END, 0);
            if (arg) break;

            arg = prime()->token;
            printf("arg %d ", p); ptoken(arg);
            if (len == 0)
            {
               len = 10;
               token->Fcall.args = calloc(len, sizeof(Token*));
            }
            else if (p + 1 == len)
            {
               Token **tmp = calloc(len *= 2, sizeof(Token*));
               memcpy(tmp, token->Fcall.args, p *  sizeof(Token*));
               free(token->Fcall.args);
               token->Fcall.args = tmp;
            }
            token->Fcall.args[p++] = arg;
            arg = find(RPAR, END, 0);
            if (arg) break;
            check(!find(COMA, 0), "");
         } while (true);
         token->Fcall.argslen = p;
         return new_node(token);
      }
      else if (find(LBRA, 0))
      {
         node = new_node(tokens[exe_pos - 1]);
         node->token->type = ACCESS;
         node->left = new_node(token);
         node->right = prime();
         check(!find(RBRA, 0), "");
         return node;
      }
      return new_node(token);
   }
   if ((token = find(FDEC, PROTO, 0)))
   {
      node = new_node(token);
      Token *ret = find(INT, CHARS, CHAR, 0);
      check(!ret, "");
      node->token->Fdec.retType = ret->type;
      Token *fname = find(ID, 0);
      check(!fname, "");
      node->token->name = fname->name;
      fname->name = NULL;
      check(!find(LPAR, 0), "");
      Token *arg = NULL;
      int len = 0;
      int p = 0;
      do {
         arg = find(RPAR, END, 0);
         if (arg) break;

         arg = prime()->token;
         if (len == 0)
         {
            len = 10;
            node->token->Fdec.args = calloc(len, sizeof(Token*));
         }
         else if (p + 1 == len)
         {
            Token **tmp = calloc(len *= 2, sizeof(Token*));
            memcpy(tmp, node->token->Fdec.args, p *  sizeof(Token*));
            free(node->token->Fdec.args);
            node->token->Fdec.args = tmp;
         }
         node->token->Fdec.args[p++] = arg;
         arg = find(RPAR, END, 0);
         if (arg) break;
         check(!find(COMA, 0), "");
      } while (true);
      node->token->Fdec.argslen = p;
      check(arg->type != RPAR, "");
      if (node->token->type != PROTO)
      {
         check(!find(DOTS, 0), "");
         while (!(token = find(END_BLOCK, END, 0)))
            add_child_node(node, expr());
         check(token->type != END_BLOCK, "");
      }
      return node;
   }
   if ((token = find(WHILE, 0)))
   {
      node = new_node(token);
      node->left = expr();
      check(!find(DOTS, 0), "");
      while (!(token = find(END_BLOCK, END, 0)))
         add_child_node(node, expr());
      check(token->type != END_BLOCK, "");
      return node;
   }
   if ((token = find(RETURN, 0)))
   {
      node = new_node(token);
      node->left = expr();
      return node;
   }

   check(1, "handle this case [%s]", to_string(tokens[exe_pos]->type));
   return NULL;
}

void _fcall(Token *token)
{
   LLVMValueRef func = LLVMGetNamedFunction(module, token->name);
   if (!func) {
      check(1, "function %s not found", token->name);
   }

   int argCount = token->Fcall.argslen;
   LLVMValueRef *args = calloc(argCount, sizeof(LLVMValueRef));

   for (int i = 0; i < argCount; i++) {
      args[i] = token->Fcall.args[i]->llvm.elem;
   }

   LLVMTypeRef funcType = LLVMGetElementType(LLVMTypeOf(func));
   if (!LLVMIsAFunction(func)) {
      check(1, "%s is not a function", token->name);
   }

   funcType = LLVMGlobalGetValueType(func);
   LLVMTypeRef retType = LLVMGetReturnType(funcType);
   const char *callName =
      (LLVMGetTypeKind(retType) == LLVMVoidTypeKind) ? "" : token->name;
   token->llvm.elem =  LLVMBuildCall2(
                          builder,
                          funcType,
                          func,
                          args,
                          argCount,
                          callName
                       );
}


void _fdec(Token *token)
{
   printf("declare function %s\n", token->name);
   // struct { Type retType; Token **args; int argsCount; } Fdec;
   token->llvm.retType = get_llvm_type(token);
   token->llvm.paramCount = token->Fdec.argslen;
   token->llvm.paramTypes = calloc(token->Fdec.argslen + 1, sizeof(TypeRef));
   for (int i = 0; i < token->Fdec.argslen; i++)
      token->llvm.paramTypes[i] = get_llvm_type(token->Fdec.args[i]);
   token->llvm.is_variadic = false;

   token->llvm.funcType = LLVMFunctionType(
                             token->llvm.retType,
                             token->llvm.paramTypes,
                             token->llvm.paramCount,
                             token->llvm.is_variadic
                          );

   ValueRef existingFunc = LLVMGetNamedFunction(module, token->name);
   if (existingFunc) {
      token->llvm.elem = existingFunc;
   } else {
      token->llvm.elem = LLVMAddFunction(module, token->name, token->llvm.funcType);
   }
}

void _entry(Token *token)
{
   BasicBlockRef entry = LLVMAppendBasicBlockInContext(context, token->llvm.elem, "entry");
   LLVMPositionBuilderAtEnd(builder, entry);
}

void _return(Token *token)
{
   ValueRef value = token->llvm.elem;
   if (value) LLVMBuildRet(builder, value);
   else LLVMBuildRetVoid(builder);
}

void _alloca(Token *token)
{
   TypeRef type = get_llvm_type(token);
   token->llvm.elem = LLVMBuildAlloca(builder, type, token->name);
}

void _int(Token *token)
{
   TypeRef type = get_llvm_type(token); long long value;
   value = (long long)token->Int.value;
   token->llvm.elem = LLVMConstInt(type, value, 0);
}

void _char(Token *token)
{
   TypeRef type = get_llvm_type(token); int value;
   value = (int)token->Char.value;
   token->llvm.elem = LLVMConstInt(type, value, 0);
}

void _chars(Token *token)
{
   static int index = 0;
   char name[20];
   snprintf(name, sizeof(name), "STR%d", index++);
   token->llvm.elem = LLVMBuildGlobalStringPtr(builder, token->Chars.value, name);
}

void _assign(Token *token, Token* variable, Token* value)
{
   LLVMBuildStore(builder, value->llvm.elem, variable->llvm.elem);
   token->llvm.elem = variable->llvm.elem;
}

void _load(Token *to, Token *from)
{
   TypeRef type = get_llvm_type(from);
   char *name = to->name;
   ValueRef source = from->llvm.elem;
   to->llvm.elem = LLVMBuildLoad2(builder, type, source, name);
}

void _op(Token *token, Token *left, Token *right)
{
   ValueRef l = left->llvm.elem;
   ValueRef r = right->llvm.elem;
   ValueRef elem = NULL;
   switch (token->type)
   {
   case ADD: elem = LLVMBuildAdd(builder, l, r, "add"); break;
   case SUB: elem = LLVMBuildSub(builder, l, r, "sub"); break;
   case EQUAL: elem = LLVMBuildICmp(builder, LLVMIntEQ, l, r, "equal"); break;
   case NOT_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntNE, l, r, "not_equal"); break;
   case LESS: elem = LLVMBuildICmp(builder, LLVMIntSLT, l, r, "less"); break;
   case MORE: elem = LLVMBuildICmp(builder, LLVMIntSGT, l, r, "more"); break;
   case LESS_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntSLE, l, r, "less_equal"); break;
   case MORE_EQUAL: elem = LLVMBuildICmp(builder, LLVMIntSGE, l, r, "more_equal"); break;
   case MUL: elem = LLVMBuildMul(builder, l, r, "mul"); break;
   case DIV: elem = LLVMBuildSDiv(builder, l, r, "div"); break;
   case MOD: elem = LLVMBuildSRem(builder, l, r, "mod"); break;
   case AND: elem = LLVMBuildAnd(builder, l, r, "and"); break;
   case OR: elem = LLVMBuildOr(builder, l, r, "or"); break;
   default:
      check(stderr, "Unknown operator: %s\n", to_string(token->type));
      break;
   }
   token->llvm.elem = elem;
}

void _branch(BasicBlockRef bloc)
{
   LLVMBuildBr(builder, bloc);
}

void _position_at(BasicBlockRef bloc)
{
   LLVMPositionBuilderAtEnd(builder, bloc);
}

void _condition(ValueRef cond, BasicBlockRef isTrue, BasicBlockRef isFalse)
{
   LLVMBuildCondBr(builder, cond, isTrue, isFalse);
}

BasicBlockRef _append_block(char *name)
{
   char block_name[256];
   snprintf(block_name, sizeof(block_name), "%s.%d", name, block_counter++);
   return LLVMAppendBasicBlockInContext(context,
                                        LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder)),
                                        block_name);
}

void _access(Token *curr, Token *left, Token *right)
{
   ValueRef leftValue = left->llvm.elem;
   ValueRef rightRef = right->llvm.elem;

   ValueRef indices[] = { rightRef };
   ValueRef gep = LLVMBuildGEP2(builder, get_llvm_type(curr), leftValue, indices, 1, "ACCESS");
   curr->llvm.elem = LLVMBuildLoad2(builder, get_llvm_type(curr), gep, "");
}

Node *scoop[100];
int scoop_pos = -1;
Node *curr_scoop;
void enter_scoop(Node *node)
{
   scoop_pos++;
   scoop[scoop_pos] = node;
   curr_scoop = scoop[scoop_pos];
}

void exit_scoop() { scoop_pos--; }

void add_variable(Node *node)
{
   if (curr_scoop->vlen == 0)
   {
      curr_scoop->vlen = 100;
      curr_scoop->variables = calloc(100, sizeof(Node *));
   }
   else if (curr_scoop->vpos + 1 == curr_scoop->vlen)
   {
      Node **tmp = calloc((curr_scoop->vlen *= 2), sizeof(Node*));
      memcpy(tmp, curr_scoop->variables, curr_scoop->vpos * sizeof(Node*));
      free(curr_scoop->variables);
      curr_scoop->variables = tmp;
   }
   curr_scoop->variables[curr_scoop->vpos++] = node;
}

Token *get_variable(char *to_find)
{
   for (int j = scoop_pos; j >= 0; j--)
   {
      Node *curr = scoop[j];
      for (int i = 0; i < curr->vpos; i++)
      {
         Token *token = curr->variables[i]->token;
         char *name = token->name;
         if (strcmp(name, to_find) == 0) return token;
      }
   }
   seg();
   check(1, "variable [%s] not found", to_find);
   return NULL;
}

void load_if_neccessary(Node *node)
{
   Token *token = node->token;
   if ((token->name && token->type != FCALL) || includes(token->type, MATH_OP, 0))
   {
      Token *new = copy_token(token);
      _load(new, token);
      token = new;
   }
   node->token = token;
}

// void safe_access_(Token *curr, Token *left, Token *right)
// {
//    ValueRef leftRef = llvm_get_ref(left);
//    ValueRef rightRef;

//    if (right->name && right->llvm.is_set) {
//       TypeRef rightType = get_llvm_type(right);
//       rightRef = LLVMBuildLoad2(builder, rightType, right->llvm.elem, "idx");
//    } else {
//       rightRef = llvm_get_ref(right);
//    }

//    ValueRef indices[] = { rightRef };
//    TypeRef indexType = get_llvm_type(curr);

//    curr->llvm.elem = LLVMBuildGEP2(builder, indexType, leftRef, indices, 1, "access");
// }

void generate_ir(Node *node)
{
   printf("%s: %s\n", __func__, to_string(node->token->type));
   switch (node->token->type)
   {
   case ID:
   {
      node->token = get_variable(node->token->name);
      break;
   }
   case INT: case CHARS: case CHAR:
   {
      if (node->token->is_dec)
      {
         _alloca(node->token);
         add_variable(node);
         node->token->is_dec = false;
         return;
      }
      else if (node->token->name)
      {

         return;
      }
      void (*funcs[END])(Token *) = {[INT] = &_int, [CHAR] = &_char, [CHARS] = &_chars};
      void (*func)(Token *) = funcs[node->token->type];
      check(!func, "handle this case [%s]\n", to_string(node->token->type));
      func(node->token);
      break;
   }
   case ADD: case SUB: case MUL: case DIV: case LESS:
   case MORE: case EQUAL: case LESS_EQUAL: case MORE_EQUAL:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);
      generate_ir(node->right);
      load_if_neccessary(node->right);
      _op(node->token, node->left->token, node->right->token);
      node->token->type = node->left->token->type;
      break;
   }
   case ASSIGN:
   {
      generate_ir(node->left);
      // load_if_neccessary(node->left->token);
      generate_ir(node->right);
      load_if_neccessary(node->right);
      _assign(node->token, node->left->token, node->right->token);
      break;
   }
   case FDEC: case PROTO:
   {
      enter_scoop(node);
      _fdec(node->token);
      if (node->token->type == FDEC)
      {
         _entry(node->token);
         for (int i = 0; i < node->token->Fdec.argslen; i++)
         {
            LLVMValueRef param = LLVMGetParam(node->token->llvm.elem, i);
            LLVMValueRef alloca = LLVMBuildAlloca(builder,
                                                  get_llvm_type(node->token->Fdec.args[i]), "param");
            LLVMBuildStore(builder, param, alloca);
            node->token->Fdec.args[i]->llvm.elem = alloca;
            add_variable(new_node(node->token->Fdec.args[i]));
         }
         for (int i = 0; i < node->cpos; i++) generate_ir(node->children[i]);
      }
      exit_scoop();
      break;
   }
   case FCALL:
   {
      for (int i = 0; i < node->token->Fcall.argslen; i++)
      {
         Node *nodeArg = new_node(node->token->Fcall.args[i]);
         generate_ir(nodeArg);
         load_if_neccessary(nodeArg);
         node->token->Fcall.args[i] = nodeArg->token;
         free(nodeArg);
      }
      _fcall(node->token);
      break;
   }
   case WHILE:
   {
      enter_scoop(node);

      node->token->llvm.cond = _append_block("while.cond");
      node->token->llvm.body = _append_block("while.body");
      node->token->llvm.end = _append_block("while.end");

      _branch(node->token->llvm.cond);
      _position_at(node->token->llvm.cond);

      generate_ir(node->left);

      _condition(node->left->token->llvm.elem, node->token->llvm.body, node->token->llvm.end);

      _position_at(node->token->llvm.body);
      for (int i = 0; i < node->cpos; i++) generate_ir(node->children[i]);
      _branch(node->token->llvm.cond);

      _position_at(node->token->llvm.end);
      exit_scoop();
      break;
   }
   case RETURN:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);
      _return(node->left->token);
      break;
   }
   case ACCESS:
   {
      generate_ir(node->left);
      load_if_neccessary(node->left);
      generate_ir(node->right);
      load_if_neccessary(node->right);
      _access(node->token, node->left->token, node->right->token);
      break;
   }
   default:
      check(1, "handle this case [%s]\n", to_string(node->token->type));
      break;
   }
}

int main(int argc, char **argv)
{
   (void)argc;
   init("module");

   input = open_file(argv[1]);
   tokenize();
   Node *buff[10000] = {};
   int p = 0;
   while (tokens[exe_pos]->type != END) buff[p++] = expr();
   for (int i = 0; buff[i]; i++) generate_ir(buff[i]);
   finalize();
   free_tokens();
   return 0;
}

// UTILS
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
   // init_varargs_intrinsics();
   // init_bounds_check_function();  // Add this
}

char *to_string(Type type)
{
   char* res[END + 1] = {
      [ID] = "ID", [CHAR] = "CHAR", [CHARS] = "CHARS", [VOID] = "VOID",
      [INT] = "INT", [BOOL] = "BOOL", [NEWLINE] = "NEWLINE", [FDEC] = "FDEC",
      [FCALL] = "FCALL", [END] = "END", [LPAR] = "LPAR", [RPAR] = "RPAR",
      [IF] = "IF", [WHILE] = "WHILE", [RETURN] = "RETURN",
      [END_BLOCK] = "END_BLOCK", [ELSE] = "ELSE", [ADD] = "ADD", [SUB] = "SUB",
      [MUL] = "MUL", [DIV] = "DIV", [ASSIGN] = "ASSIGN",
      [ADD_ASSIGN] = "ADD_ASSIGN", [SUB_ASSIGN] = "SUB_ASSIGN",
      [MUL_ASSIGN] = "MUL_ASSIGN", [DIV_ASSIGN] = "DIV_ASSIGN",
      [MOD_ASSIGN] = "MOD_ASSIGN", [ACCESS] = "ACCESS",
      [MOD] = "MOD", [COMA] = "COMA",
      [EQUAL] = "EQUAL", [NOT_EQUAL] = "NOT_EQUAL", [LESS] = "LESS", [MORE] = "MORE",
      [LESS_EQUAL] = "LESS_EQUAL", [MORE_EQUAL] = "MORE_EQUAL",
      [AND] = "AND", [OR] = "OR", [DOTS] = "DOTS",
      [COLON] = "COLON", [COMMA] = "COMMA",
      [LBRACKET] = "LBRACKET", [RBRACKET] = "RBRACKET",
      [PROTO] = "PROTO", [ELLIPSIS] = "ELLIPSIS", [REF] = "REF",
      [VA_LIST] = "VA_LIST", [AS] = "AS", [STACK] = "STACK",
      [TRY] = "TRY", [CATCH] = "CATCH", [THROW] = "THROW",
      [USE] = "USE", [LBRA] = "LBRA", [RBRA] = "RBRA",
   };
   if (!res[type])
   {
      printf("%s:%d handle this case %d\n", __FILE__, __LINE__, type);
      exit(1);
   }
   return res[type];
}

void ptoken(Token *token)
{
   printf("token %s ", to_string(token->type));
   if (token->name) printf("[%s] ", token->name);

   if (token->is_dec) printf("is dec");
   else
      switch (token->type)
      {
      case INT: printf("%ld", token->Int.value); break;
      case CHAR: printf("[%c]", token->Char.value); break;
      case CHARS: printf("[%s]", token->Chars.value); break;
      default: break;
      }
   printf("\n");
}

void pnode(Node *node, char *side, int space)
{
   if (!node) return;
   int i = 0;
   while (i < space) i += printf(" ");
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

TypeRef get_llvm_type(Token *token)
{
   Type type = token->type;
   if (includes(type, FDEC, PROTO, 0)) type = token->Fdec.retType;
   TypeRef res[END] = {[INT] = i32, [CHAR] = i8, [CHARS] = p8,
                       [BOOL] = i1, [VOID] = vd, [VA_LIST] = p8,
                       [ACCESS] = i8, [FCALL] = i32, // TODO: to be fixed later
                      };
   check(!res[type], "handle this case [%s]\n", to_string(type));
   return res[type];
}

void finalize()
{
   char *error = NULL;
   if (LLVMVerifyModule(module, LLVMReturnStatusAction, &error)) {
      fprintf(stderr, RED"Module verification failed:\n%s\n"RESET, error);
      LLVMDisposeMessage(error);
   }
   LLVMPrintModuleToFile(module, "out.ll", NULL);

   LLVMDisposeBuilder(builder);
   LLVMDisposeModule(module);
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
   // TODO: can't use setName here to investigate later why ?
   if (token->name)
      new->name = strdup(token->name);

   if (token->Chars.value) new->Chars.value = strdup(token->Chars.value);
   tokens[tk_pos++] = new;
   return new;
}