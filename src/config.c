#include "./include/header.h"

void create_builtin(char *name, Type *params, Type retType)
{
   if (found_error) return;
   Node *node = new_node(new_token(name, 0, strlen(name), FDEC, 0));
   node->token->retType = retType;
   switch (retType)
   {
   case INT:   setReg(node->token, "eax"); break;
   case CHAR:  setReg(node->token, "al"); break;
   case CHARS: setReg(node->token, "rax"); break;
   case VOID:  setReg(node->token, "rax"); break;
   case PTR:   setReg(node->token, "rax"); break;
   case LONG:  setReg(node->token, "rax"); break;
   default: check(1, "handle this case %s", to_string(retType)) ;
   }

   node->left = new_node(new_token(0, 0, 0, CHILDREN, node->token->space));
   int i = 0;
   int j = 0;
   while (params[i] && !found_error)
   {
      Node *child = new_node(new_token(NULL, 0, 0, params[i], node->token->space));
      if (eregs[j])
      {
         if (child->token->is_ref) setReg(child->token, rregs[j]);
         else
         {
            // TODO: add other data type and math operations
            switch (child->token->type)
            {
            case CHARS: setReg(child->token, rregs[j]); break;
            case INT:   setReg(child->token, eregs[j]); break;
            case CHAR:  setReg(child->token, eregs[j]); break;
            case FLOAT: setReg(child->token, rregs[j]); break; // TODO: to be checked
            case BOOL:  setReg(child->token, eregs[j]); break;
            case LONG:  setReg(child->token, rregs[j]); break;
            case PTR:   setReg(child->token, rregs[j]); break;
            default: todo(1, "set reg for %s", to_string(child->token->type));
            };
         }
         j++;
      }
      else
      {
         // TODO:
         todo(1, "implement assigning function argument using PTR");
      }
      add_child(node->left, child);
      i++;
   }
   new_function(node);
}

void add_builtins()
{
   struct { char *name; Type *attrs; Type ret;} builtins[] = {
      //----------------------
      // Memory Management
      //----------------------
      {"malloc", (Type[]){LONG, 0}, PTR},
      {"calloc", (Type[]){LONG, LONG, 0}, PTR},
      {"realloc", (Type[]){PTR, LONG, 0}, PTR},
      {"free", (Type[]){PTR, 0}, VOID},

      //----------------------
      // String Operations
      //----------------------
      {"strlen", (Type[]){CHARS, 0}, INT},
      {"strcpy", (Type[]){CHARS, CHARS, 0}, CHARS},
      {"strncpy", (Type[]){CHARS, CHARS, LONG, 0}, CHARS},
      {"strcat", (Type[]){CHARS, CHARS, 0}, CHARS},
      {"strncat", (Type[]){CHARS, CHARS, LONG, 0}, CHARS},
      {"strcmp", (Type[]){CHARS, CHARS, 0}, INT},
      {"strncmp", (Type[]){CHARS, CHARS, LONG, 0}, INT},
      {"strdup", (Type[]){CHARS, 0}, CHARS},
      {"strchr", (Type[]){CHARS, INT, 0}, CHARS},
      {"strstr", (Type[]){CHARS, CHARS, 0}, CHARS},

      //----------------------
      // I/O Operations
      //----------------------
      {"read", (Type[]){INT, CHARS, INT, 0}, INT},
      {"write", (Type[]){INT, CHARS, INT, 0}, INT},
      // {"printf",  (Type[]){CHARS, VARARG, 0}, INT},
      // {"scanf",   (Type[]){CHARS, VARARG, 0}, INT},
      {"putstr", (Type[]){CHARS, 0}, INT},
      {"putnbr", (Type[]){INT, 0}, INT},
      {"puts", (Type[]){CHARS, 0}, INT},
      {"putchar", (Type[]){INT, 0}, INT},
      {"getchar", (Type[]){0}, INT},
      {"socket", (Type[]){INT, INT, INT, 0}, INT},
      // {"fopen",   (Type[]){CHARS, CHARS, 0}, FILE_PTR},
      // {"fclose",  (Type[]){FILE_PTR, 0},               INT},

      //----------------------
      // Math Functions
      //----------------------
      {"abs", (Type[]){INT, 0}, INT},
      {"labs", (Type[]){LONG, 0}, LONG},
      // {"sqrt",    (Type[]){DOUBLE, 0},                 DOUBLE},
      // {"pow",     (Type[]){DOUBLE, DOUBLE, 0},         DOUBLE},
      // {"sin",     (Type[]){DOUBLE, 0},                 DOUBLE},
      // {"cos",     (Type[]){DOUBLE, 0},                 DOUBLE},

      //----------------------
      // System/Process
      //----------------------
      {"exit", (Type[]){INT, 0}, VOID},
      // {"system", (Type[]){CHARS, 0}, INT},
      // {"atexit",  (Type[]){VOID_FUNC_PTR, 0},          INT},

      //----------------------
      // Utility
      //----------------------
      // {"rand", (Type[]){0}, INT},
      // {"srand",   (Type[]){UNSIGNED_INT, 0},           VOID},
      // {"qsort",   (Type[]){PTR, LONG, LONG, COMPAR_FUNC_PTR, 0}, VOID},

      // Sentinel
      {NULL, NULL, 0}
   };
   for (int i = 0; builtins[i].name; i++)
      create_builtin(builtins[i].name, builtins[i].attrs, builtins[i].ret);
}


void create_struct(char *name, Token *attrs)
{
   Token *token = new_token("struct", 0, strlen("struct"), ID, 0);
   setName(token, name);

   for(int i = 0; attrs[i].name; i++)
   {
      Token *attr = NULL;
      if(attrs[i].type == STRUCT_CALL)
      {
         Token *st = get_struct(attr->name);
         if (check(!st, "Unkown data type [%s]\n", attr->name)) exit(1);
         attr = get_struct(attrs[i].name);
         attr->type = STRUCT_CALL;
      }
      else attr = new_token(NULL, 0, 0, attrs[i].type, 0);
      setName(attr, attrs[i].name);
      add_attribute(token, attr);
   }

   set_struct_size(token);
   new_struct(token);
}

void add_structs()
{
#if 0
   struct { char *name; Token *attrs; } structs[] = {
      {"Info", (Token[]){
         {.name = "heigh", .type = INT},
         {.name = "width", .type = INT},
         {.name = NULL, .type = 0},
      }},
      {"Region", (Token[]){
         {.name = "name", .type = CHARS},
         {.name = "size", .type = INT},
         {.name = NULL, .type = 0},
      }},
      {"User", (Token[]){
         {.name = "Region", .type = STRUCT_CALL},
         {.name = "Info", .type = STRUCT_CALL},
         {.name = NULL, .type = 0},
      }},
      {NULL, NULL}
   };

   for(int i = 0; structs[i].name; i++)
   {
      create_struct(structs[i].name, structs[i].attrs);
   }
#endif
}

void config()
{
   add_builtins();
   add_structs();
}