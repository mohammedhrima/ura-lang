#include "ura.h"

// ----------------------------------------------------------------------------
// gen_ir helpers — extracted from large switch cases
// ----------------------------------------------------------------------------

// [FCALL] obj.method ret [TYPE]   (is_method_call = true)
// └──[ARGS]
//    ├──arg1
//    ├──arg2
//    └──[STRUCT_CALL] obj         ← last child = receiver
static void ir_method_call(Node *node)
{
   // resolve object type
   Node *obj_node = node->left->children[node->left->cpos - 1];
   gen_ir(obj_node);
   if (found_error) return;
   Token *obj = obj_node->token;

   // build qualified name: "User.greet"
   obj->Struct.ptr->token->used++; // mark struct def as used
   char *struct_name = obj->Struct.ptr->token->name;
   char *qname       = strjoin(struct_name, ".", node->token->name);
   setName(node->token, qname);
   free(qname);
   node->token->is_method_call = false;

   Node *func = get_function(node->token->name);
   if (!func) return;
   func->token->used++;
   node->token->Fcall.ptr = func;
   func = copy_node(func);

   // first arg already gen_ir'd (obj), do rest
   obj_node->token->used++;
   for (int i = 0; !found_error && i < node->left->cpos - 1; i++)
   {
      Node *carg = node->left->children[i];
      gen_ir(carg);
      if (check(carg->token->type == ID, "Indeclared variable %s", carg->token->name))
         break;
      carg->token->used++;
   }
   free_node(func);
   set_ret_type(node);
}

// [FCALL] name ret [TYPE]
// └──[ARGS]
//    ├──arg1
//    └──arg2
static void ir_regular_call(Node *node)
{
   Node *func = get_function(node->token->name);
   if (!func) return;
   func->token->used++;
   node->token->Fcall.ptr = func;
   func = copy_node(func);

   Node *call_args = node->left;
   Node *dec_args  = func->left;
   // TODO: check number of arguments
   for (int i = 0; !found_error && call_args && i < call_args->cpos; i++)
   {
      Node *carg = call_args->children[i];
      gen_ir(carg);
      carg->token->used++;
      Token *src = carg->token;
      if (check(src->type == ID, "Undeclared variable %s", carg->token->name)) break;

      if (i < dec_args->cpos)
      {
         bool param_is_ref = dec_args->children[i]->token->is_ref;
         if (param_is_ref)
            if (check(!src->name || src->type == FCALL,
                      "'%s': ref parameter requires a named variable",
                      dec_args->children[i]->token->name))
               break;

         Type param_type   = dec_args->children[i]->token->type;
         Type arg_type     = src->type;
         bool param_is_int = includes(param_type, NUMERIC_TYPES, 0);
         bool arg_is_int   = includes(arg_type,   NUMERIC_TYPES, 0);
         if (param_is_int && arg_is_int && param_type != arg_type)
         {
            Token *as_tok   = new_token(AS, src->space);
            as_tok->retType = param_type;
            Node  *tgt_node = new_node(new_token(param_type, src->space));
            Node  *as_node  = new_node(as_tok);
            as_node->left          = carg;
            as_node->right         = tgt_node;
            call_args->children[i] = as_node;
         }
      }
   }
   free_node(func);
   set_ret_type(node);
}

// [DOT] ret [FIELD_TYPE]
// ├──left:  [STRUCT_CALL] obj
// └──right: [ID] field   → resolved to field's type + index
static void ir_dot(Node *node)
{
   gen_ir(node->left);
   if (found_error) return;
   node->left->token->used++;
   // Also mark the struct definition itself as used
   if (node->left->token->Struct.ptr)
      node->left->token->Struct.ptr->token->used++;
   Type  retType = 0;

   Node *src     = NULL;
   if (node->left->token->type == STRUCT_CALL)
      src = get_struct(node->left->token->Struct.ptr->token->name);
   else if (node->left->token->retType == STRUCT_CALL && node->left->token->Struct.ptr)
      src = get_struct(node->left->token->Struct.ptr->token->name);
   else
   {
      check(1, "handle this case %s", to_string(node->left->token->type));
      return;
   }

   bool found = false;
   for (int i = 0; src && i < src->cpos; i++)
   {
      Node *src_child = src->children[i];
      if (strcmp(src_child->token->name, node->right->token->name) == 0)
      {
         found   = true;
         retType = src_child->token->type;
         node->right->token->Struct.index = i;
         node->right->token->type         = src_child->token->type;
         node->right->token->retType      = src_child->token->retType;
         if (src_child->token->type == STRUCT_CALL)
            node->right->token->Struct.ptr = src_child->token->Struct.ptr;
         else if (src_child->token->type == ARRAY_TYPE)
         {
            node->right->token->Array.elem_type = src_child->token->Array.elem_type;
            node->right->token->Array.depth     = src_child->token->Array.depth;
         }
         break;
      }
   }
   if (check(!found, ""))
   {
      debug("%s%k doesn't have attribute %k%s\n", RED,
            node->left->token, node->right->token, RESET);
      return;
   }

   node->token->retType = retType;
   if (retType == STRUCT_CALL)
      node->token->Struct.ptr = node->right->token->Struct.ptr;
   else if (retType == ARRAY_TYPE)
   {
      node->token->Array.elem_type = node->right->token->Array.elem_type;
      node->token->Array.depth     = node->right->token->Array.depth;
   }
}

// ----------------------------------------------------------------------------
// Semantic analysis / name resolution pass
// ----------------------------------------------------------------------------

void gen_ir(Node *node)
{
   if (found_error) return;
   Node *left  = node->left;
   Node *right = node->right;
   switch (node->token->type)
   {
   // [ID] varname   → resolved to declared token (type, llvm.elem, etc.)
   case ID:
   {
      Token *find = get_variable(node->token->name);
      if (find)
         node->token = find;
      break;
   }
   // [STRUCT_CALL] varname → StructType   (is_dec = declaration | usage)
   case STRUCT_CALL:
   {
      Node *src = get_struct(node->token->Struct.ptr->token->name);
      node->token->Struct.ptr = src;
      if (node->token->is_dec)
      {
         if (scope_pos == 1) node->token->is_global = true;
         else src->token->used++; // local declaration → always emit struct def
         new_variable(node->token);
         if (src->token->is_init)
         {
            // TODO: to be checked later
            char *qname   = strjoin(src->token->name, ".init", NULL);
            Node *init_fn = get_function(qname);
            free(qname);
            if (init_fn) init_fn->token->used++;
         }
      }
      break;
   }
   // [INT/BOOL/CHAR/CHARS/FLOAT/LONG/VOID] name   (is_dec → register var)
   // [INT] [42]                                     (literal → mark used)
   case INT: case BOOL: case CHAR: case ARRAY_TYPE:
   case FLOAT: case LONG: case CHARS: case PTR: case VOID:
   {
      if (node->token->is_dec)
      {
         if (scope_pos == 1) node->token->is_global = true;
         new_variable(node->token);
      }
      else node->token->used++;
      break;
   }
   // [TUPLE_UNPACK]
   // ├──[TYPE] a
   // ├──[TYPE] b
   // └──left: [FCALL] fn ret [TUPLE]
   case TUPLE_UNPACK:
   {
      // Register each LHS variable, then process RHS
      for (int i = 0; i < node->cpos; i++)
      {
         Node *lhs = node->children[i];
         if (lhs->token->is_dec)
         {
            if (scope_pos == 1) lhs->token->is_global = true;
            new_variable(lhs->token);
         }
      }
      gen_ir(node->left); // RHS function call
      break;
   }
   // [ASSIGN] ret [LEFT_TYPE]
   // ├──left:  lvalue
   // └──right: rvalue
   case ASSIGN:
   {
      // TODO: check compatibility
      // TODO: add all ir_bound checks in &ll places
      gen_ir(left);
      gen_ir(right);
      if (!compatible(left->token, right->token))
      {
         check(1, "Invalid assignement %k= %k", left->token, right->token);
         break;
      }
      bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound) &&
                      (!right->token->name || right->token->type == FCALL);
      if (check(error_op, "'%s': ref must be bound to a named variable", left->token->name))
         break;
      node->token->retType = left->token->retType;
      if (left->token->is_ref)
         left->token->ir_bound = true;
      break;
   }
   // [+=/-=/etc.] ret [LEFT_TYPE]
   // ├──left:  lvalue
   // └──right: rvalue
   case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN: case MOD_ASSIGN:
   {
      gen_ir(left);
      bool error_op = !left->token->is_param && (left->token->is_ref && !left->token->ir_bound);
      if (check(error_op, "'%s': ref must be bound to a named variable", left->token->name))
         break;
      gen_ir(right);
      node->token->retType = left->token->retType;
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      break;
   }
   // [NOT] ret [BOOL]        [BNOT] ret [LEFT_TYPE]
   // └──left: expr           └──left: expr
   case NOT: case BNOT:
   {
      // TODO: left must be boolean
      gen_ir(left);
      node->token->used++;
      node->left->token->used++;
      if (node->token->type == NOT)
         node->token->retType = BOOL;
      else
         node->token->retType = left->token->retType ?
                                left->token->retType : left->token->type;
      break;
   }
   // [ADD/SUB/MUL/DIV/MOD/AND/OR/BAND/BOR/BXOR/LSHIFT/RSHIFT/…] ret [TYPE]
   // ├──left:  expr
   // └──right: expr
   case ADD: case SUB: case MUL: case DIV: case EQUAL:
   case NOT_EQUAL: case LESS: case GREAT: case LESS_EQUAL:
   case GREAT_EQUAL: case MOD: case AND: case OR:
   case BAND: case BOR: case BXOR: case LSHIFT: case RSHIFT:
   {
      // TODO: check compatibility
      gen_ir(left);
      gen_ir(right);
      check(!left || !left->token, "error in assignment, left is NULL");
      check(!right || !right->token, "error in assignment, right is NULL");
      if (includes(node->token->type, ADD, SUB, MUL, DIV, MOD, 0))
         node->token->retType = left->token->retType;
      else if (includes(node->token->type, COMPARISON_OPS, AND, OR, 0))
         node->token->retType = BOOL;
      node->token->used++;
      node->left->token->used++;
      node->right->token->used++;
      set_ret_type(node);
      break;
   }
   // [WHILE]
   // ├──left:     condition
   // └──children: body stmts
   case WHILE:
   {
      enter_scope(node);
      gen_ir(node->left); // condition
      node->left->token->used++;
      for (int i = 0; i < node->cpos; i++) gen_ir(node->children[i]);
      exit_scope();
      node->token->used++;
      break;
   }
   // [IF]
   // ├──left:     condition
   // ├──children: body stmts
   // └──right: [ELIF]
   //           ├──left:     condition
   //           ├──children: body stmts
   //           └──right: [ELSE]
   //                     └──children: body stmts
   case IF:
   {
      enter_scope(node);
      Node *curr = node;
      while (curr && includes(curr->token->type, IF, ELIF, ELSE, 0))
      {
         if (includes(curr->token->type, IF, ELIF, 0))
         {
            check(curr->left == NULL, "error");
            gen_ir(curr->left); // condition
            curr->left->token->used++;
         }
         for (int i = 0; i < curr->cpos; i++)
            gen_ir(curr->children[i]);
         curr = curr->right;
      }
      exit_scope();
      node->token->used++;
      break;
   }
   // [FDEC] name ret [TYPE]
   // ├──[ARGS]
   // │  ├──[TYPE] param1
   // │  └──[TYPE] param2
   // └──children: body stmts
   case FDEC:
   {
      new_function(node);
      enter_scope(node);
      Node **params = (node->left ? node->left->children : NULL);
      for (int i = 0; params && i < node->left->cpos && !found_error; i++)
      {
         gen_ir(params[i]);
         params[i]->token->used++;
      }
      for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
         gen_ir(node->children[i]);
      exit_scope();
      break;
   }
   // [FCALL] name ret [TYPE]   →   ir_regular_call
   // [FCALL] obj.method        →   ir_method_call  (is_method_call=true)
   case FCALL:
   {
      if (node->token->is_method_call && node->left)
         ir_method_call(node);
      else
         ir_regular_call(node);
      break;
   }
   // [AS] ret [TARGET_TYPE]
   // ├──left:  expr
   // └──right: [TARGET_TYPE]
   case AS:
   {
      gen_ir(node->left);
      node->token->retType = node->right->token->type;
      break;
   }
   // [DOT] ret [FIELD_TYPE]     →   ir_dot
   // ├──left:  [STRUCT_CALL] obj
   // └──right: [ID] field
   case DOT:
   {
      ir_dot(node);
      break;
   }
   // [ACCESS] ret [ELEM_TYPE]
   // ├──left:  array / chars
   // └──right: index expr
   case ACCESS:
   {
      gen_ir(node->left);
      gen_ir(node->right);
      if (found_error) break;
      node->right->token->used++;
      Type retType = 0;
      Type type    = node->left->token->retType ? node->left->token->retType : node->left->token->type;
      int  depth   = node->left->token->Array.depth;
      switch (type)
      {
      case CHARS: retType = CHAR; break;
      case STACK: case HEAP: case ARRAY_TYPE: case ARRAY:
         if (depth > 1)
         {
            // still multi-dim: return sub-array type, depth-1
            retType                      = ARRAY;
            node->token->Array.elem_type = node->left->token->Array.elem_type;
            node->token->Array.depth     = depth - 1;
         }
         else
            retType = node->left->token->Array.elem_type;
         break;
      default:
         check(1, "handle this case %s", to_string(node->left->token->type));
         break;
      }
      node->token->retType = retType;
      break;
   }
   // [RETURN] ret [TYPE]        [RETURN] ret [TUPLE]
   // └──left: expr              ├──expr1
   //                            └──expr2
   case RETURN:
   {
      if (node->cpos > 0)
      {
         // Tuple return: process each child expression
         for (int i = 0; i < node->cpos; i++)
            gen_ir(node->children[i]);
         node->token->retType = TUPLE;
         break;
      }
      gen_ir(node->left);
      node->token->retType = node->left->token->type;
      if (node->left->token->type == STRUCT_CALL)
         node->token->Struct.ptr = node->left->token->Struct.ptr;
      node->left->token->used++;
      break;
   }
   // [OUTPUT]
   // └──[ARGS]
   //    ├──fmt expr
   //    └──value exprs...
   case OUTPUT:
   {
      for (int i = 0; i < node->left->cpos; i++)
      {
         gen_ir(node->left->children[i]);
         node->left->children[i]->token->used++; // keep globals/vars alive (mirrors FCALL arg handling)
      }
      // output() compiles to printf internally — codegen declares printf as needed
      node->token->retType = VOID;
      break;
   }
   // [TYPEOF] ret [CHARS]       [SIZEOF] ret [INT]
   // └──left: expr              └──left: expr
   case TYPEOF: case SIZEOF:
   {
      gen_ir(node->left);
      Token *type_tok = node->left->token;
      Type   type     = type_tok->type ? type_tok->type : type_tok->retType;
      if (node->token->type == TYPEOF)
      {
         node->token->retType     = CHARS;
         node->token->Chars.value = strdup(to_string(type));
      }
      else
      {
         node->token->retType   = INT;
         node->token->Int.value = 0; // placeholder
      }
      node->token->used++;
      break;
   }
   // [STACK] ret [ARRAY] elem_type    [HEAP] ret [ARRAY] elem_type
   // └──left: size expr               └──left: count expr
   case STACK: case HEAP:
   {
      for (int i = 0; i < node->cpos; i++)
      {
         gen_ir(node->children[i]);
         node->children[i]->token->used++;
      }
      node->token->used++;
      break;
   }
   // [STRUCT_DEF] Name
   // ├──[FIELD_TYPE] field1
   // ├──[FIELD_TYPE] field2
   // └──methods[]: [FDEC] ...
   case STRUCT_DEF:
   {
      for (int i = 0; node && i < node->mpos; i++)
         gen_ir(node->methods[i]);
      break;
   }
   // [ENUM_DEF] Name
   // ├──[INT] VariantA  (global constant)
   // └──...
   case ENUM_DEF:
   {
      for (int i = 0; i < node->cpos; i++)
         node->children[i]->token->used++;
      break;
   }
   // [BREAK]    [CONTINUE]
   case BREAK: case CONTINUE: break;
   default:
   {
      todo(1, "handle this case %s", to_string(node->token->type));
      break;
   }
   }
}
