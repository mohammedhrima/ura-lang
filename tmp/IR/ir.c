#include "parse.c"

// INSTRUCTIONS
Inst **first_insts;
Inst **insts = NULL;
size_t inst_size;
size_t inst_pos;
size_t reg_pos;

size_t bloc_index;
size_t str_index;
size_t float_index;
// STACK POINTER
long ptr = 0;
size_t arg_ptr;

#if AST
void enter_scoop(char *name)
{
  GLOG("ENTER SCOOP", "%s\n", name);
  if (global_scoop == NULL)
  {
    bloc_size = 10;
    global_scoop = calloc(bloc_size, sizeof(Scoop));
  }
  else if (scoop_pos + 1 == (ssize_t)bloc_size)
  {
    Scoop *tmp = calloc(bloc_size * 2, sizeof(Scoop));
    memcpy(tmp, global_scoop, scoop_pos * sizeof(Scoop));
    bloc_size *= 2;
    free(global_scoop);
    global_scoop = tmp;
  }
  scoop_pos++;
  global_scoop[scoop_pos] = (Scoop){};
  global_scoop[scoop_pos].name = name;
  curr_scoop = &global_scoop[scoop_pos];
}

void exit_scoop()
{
  GLOG(" EXIT SCOOP", "%s\n", curr_scoop->name);
  free(curr_scoop->functions);
  free(curr_scoop->variables);
  free(curr_scoop->structs);
  global_scoop[scoop_pos] = (Scoop){};
  scoop_pos--;
  // if(scoop_pos >= 0)
  curr_scoop = &global_scoop[scoop_pos];
  // else
  // curr_scoop = NULL;
}

#if IR
void add_inst(Inst *inst)
{
  if (first_insts == NULL)
  {
    inst_size = 100;
    first_insts = calloc(inst_size, sizeof(Inst *));
  }
  else if (inst_pos + 1 == inst_size)
  {
    Inst **tmp = calloc(inst_size * 2, sizeof(Inst *));
    memcpy(tmp, first_insts, inst_pos * sizeof(Inst *));
    free(first_insts);
    first_insts = tmp;
    inst_size *= 2;
  }
  first_insts[inst_pos++] = inst;
}

// NAMESPACE
#if 0
Token *get_namespace(char *name) // TODO: maybe you need to use get scoop
{
    return NULL;
}
#endif

// VARIABLES / FUNCTIONS
Token *get_variable(char *name)
{
  CLOG("get var", "%s\n", name);
  CLOG("Scoop", "%s\n", curr_scoop->name);
  for (size_t i = 0; i < curr_scoop->var_pos; i++)
  {
    Token *var = curr_scoop->variables[i];
    if (strcmp(var->name, name) == 0)
    {
      GLOG("found", "in [%s] scoop\n", curr_scoop->name);
      return curr_scoop->variables[i];
    }
  }
  error("%s:%d '%s' Not found\n", FUNC, LINE, name);
  exit(1);
  return NULL;
}

Token *new_variable(Token *token)
{
  CLOG("new var", "%s\n", token->name);
  // TODO: check here the global variables
  // Token **variables = curr_scoop->variables;
  CLOG("Scoop", "%s\n", curr_scoop->name);
  for (size_t i = 0; i < curr_scoop->var_pos; i++)
  {
    Token *var = curr_scoop->variables[i];
    if (strcmp(var->name, token->name) == 0)
    {
      debug("%sRedefinition of %s%s\n", RED, token->name, RESET);
      exit(1);
    }
  }
  if (curr_scoop->variables == NULL)
  {
    curr_scoop->var_size = 10;
    curr_scoop->variables = calloc(curr_scoop->var_size, sizeof(Token *));
  }
  else if (curr_scoop->var_pos + 1 == curr_scoop->var_size)
  {
    curr_scoop->var_size *= 2;
    Token **tmp = calloc(curr_scoop->var_size, sizeof(Token *));
    memcpy(tmp, curr_scoop->variables, curr_scoop->var_pos * sizeof(Token *));
    free(curr_scoop->variables);
    curr_scoop->variables = tmp;
  }
  // GLOG("", "in [%s] scoop\n", curr_scoop->name);
  curr_scoop->variables[curr_scoop->var_pos++] = token;
  return token;
}

Node **builtins_functions;
size_t builtins_pos;
size_t builtins_size;

void create_builtin(char *name, Type *params, Type retType)
{
  Node *func = new_node(new_token(name, 0, strlen(name), 0, fdec_));
  func->token->retType = retType;
  func->left = new_node(NULL);
  Node *curr = func->left;
  int i = 0;
  while (params[i])
  {
    curr->right = new_node(NULL);
    curr = curr->right;
    curr->left = new_node(new_token("", 0, 0, 0, params[i]));
    i++;
  }
  if (builtins_functions == NULL)
  {
    builtins_size = 2;
    builtins_functions = calloc(builtins_size, sizeof(Node *));
  }
  else if (builtins_pos + 1 == builtins_size)
  {
    Node **tmp = calloc(builtins_size * 2, sizeof(Node *));
    memcpy(tmp, builtins_functions, builtins_pos * sizeof(Node *));
    free(builtins_functions);
    builtins_size *= 2;
    builtins_functions = tmp;
  }
  builtins_functions[builtins_pos++] = func;
  debug("%screate function [%s]%s\n", GREEN, func->token->name, RESET);
}

Node *get_function(char *name)
{
  // TODO: remove output from here
  CLOG("get func", "%s\n", name);
  CLOG("Scoop", "%s\n", curr_scoop->name);
  char *builtins[] = {"output", 0};
  for (int i = 0; builtins[i]; i++)
    if (strcmp(name, builtins[i]) == 0)
      return NULL;
  for (size_t i = 0; i < builtins_pos; i++)
  {
    if (strcmp(name, builtins_functions[i]->token->name) == 0)
      return builtins_functions[i];
  }
  // if(strcmp(name, "write") == 0)
  // {
  //     Node *func = new_node(new_token("write", 0, 5, 0, fdec_));
  //     Node *curr = func;
  //     curr->left = new_node(NULL);
  //     curr = curr->left;
  //     curr->right = new_node(NULL);
  //     curr = curr->right;
  //     curr->left = new_node(new_token(0, 0, 0, 0, int_));
  //     curr->right = new_node(NULL);
  //     curr = curr->right;
  //     curr->left = new_node(new_token(0, 0, 0, 0, chars_));
  //     curr->right = new_node(NULL);
  //     curr = curr->right;
  //     curr->left = new_node(new_token(0, 0, 0, 0, int_));
  //     return func;
  // }
  for (ssize_t j = scoop_pos; j >= 0; j--)
  {
    Scoop *scoop = &global_scoop[j];
    for (size_t i = 0; i < scoop->func_pos; i++)
    {
      Node *func = scoop->functions[i];
      if (strcmp(func->token->name, name) == 0)
        return func;
    }
  }
  error("%s:%d '%s' Not found\n", FUNC, LINE, name);
  exit(1);
  return NULL;
}

// TODO: create a list fo built in functions
Node *new_function(Node *node)
{
  CLOG("new func", "%s\n", node->token->name);
  CLOG("Scoop", "%s\n", curr_scoop->name);
  char *builtins[] = {"output", 0};
  for (int i = 0; builtins[i]; i++)
  {
    if (strcmp(node->token->name, builtins[i]) == 0)
    {
      error("%s is a built in function\n", node->token->name);
      exit(1);
    }
  }
  for (size_t i = 0; i < curr_scoop->func_pos; i++)
  {
    Node *func = curr_scoop->functions[i];
    if (strcmp(func->token->name, node->token->name) == 0)
    {
      error("Redefinition of %s\n", node->token->name);
      exit(1);
    }
  }
  if (curr_scoop->functions == NULL)
  {
    curr_scoop->func_size = 10;
    curr_scoop->functions = calloc(curr_scoop->func_size, sizeof(Node *));
  }
  else if (curr_scoop->func_pos + 1 == curr_scoop->func_size)
  {
    curr_scoop->func_size *= 2;
    Node **tmp = calloc(curr_scoop->func_size, sizeof(Node *));
    memcpy(tmp, curr_scoop->functions, curr_scoop->func_pos * sizeof(Node *));
    free(curr_scoop->functions);
    curr_scoop->functions = tmp;
  }
  curr_scoop->functions[curr_scoop->func_pos++] = node;
  return node;
}

Inst *new_inst(Token *token)
{
  debug("new instruction has type %s\n", to_string(token->type));
  Inst *new = calloc(1, sizeof(Inst));
  new->token = token;
  if (token->name && token->declare)
  {
    new_variable(token);
    // if (token->declare)
    if (token->type == struct_)
    {
      // token->ptr = ptr;
      // ptr += token->offset;
      // token->sreg = ++reg_pos;
      token->sreg = ++reg_pos;
    }
    else
    {
      // if(token->isarg)
      //     token->ptr = (arg_ptr += 8);
      // else
      token->ptr = (ptr += sizeofToken(token));

      token->sreg = ++reg_pos;
    }
  }
  else
  {
    switch (token->type)
    {
    case add_:
    case sub_:
    case mul_:
    case div_:
    case equal_:
    case less_:
    case more_:
    case less_equal_:
    case more_equal_:
    case not_equal_:
    case fcall_:
      token->sreg = ++reg_pos;
      break;
    default:
      break;
    }
  }
  add_inst(new);
  return new;
}

Token *generate_ir(Node *node)
{
  // debug("gen-ir: %s\n", to_string(node->token->type));
  Inst *inst = NULL;
  switch (node->token->type)
  {
  case id_:
  {
    Token *token = get_variable(node->token->name);
    return token;
    break;
  }
  case module_:
  {
    return node->token;
    break;
  }
  case if_:
  {
    // Node *tmp = node;
    Node *curr = node->left;

    // condition
    // Token *result =
    generate_ir(curr->left); // TODO: check if it's boolean

    node->token->type = jne_;
    node->token->name = strdup("endif");
    node->token->index = ++bloc_index;

    Token *lastInst = copy_token(node->token);
    new_inst(lastInst); // jne to endif

    curr = curr->right;
    while (curr->left) // if code bloc
    {
      generate_ir(curr->left);
      curr = curr->right;
    }
    Inst *endInst = NULL;
    if (node->right)
    {
      endInst = new_inst(new_token("endif", 0, 5, node->token->space, jmp_));
      endInst->token->index = node->token->index;
    }

    curr = node->right;
    while (curr)
    {
      if (curr->left->token->type == elif_)
      {
        curr->left->token->index = ++bloc_index;
        curr->left->token->type = bloc_;
        curr->left->token->name = strdup("elif");

        {
          free(lastInst->name);
          lastInst->name = strdup("elif");
          lastInst->index = curr->left->token->index;
          lastInst = copy_token(lastInst);
        }

        new_inst(curr->left->token);
        Node *tmp = curr->left;
        generate_ir(tmp->left); // elif condition, TODO: check is boolean

        new_inst(lastInst);

        tmp = tmp->right;
        while (tmp->left)
        {
          generate_ir(tmp->left);
          tmp = tmp->right;
        }
      }
      else if (curr->left->token->type == else_)
      {
        curr->left->token->index = ++bloc_index;
        curr->left->token->type = bloc_;
        curr->left->token->name = strdup("else");
        new_inst(curr->left->token);

        {
          free(lastInst->name);
          lastInst->name = strdup("else");
          lastInst->index = curr->left->token->index;
          lastInst = copy_token(lastInst);
        }

        Node *tmp = curr->left;
        tmp = tmp->right;
        while (tmp->left)
        {
          generate_ir(tmp->left);
          tmp = tmp->right;
        }
        break;
      }
      if (curr->right)
      {
        endInst = new_inst(new_token("endif", 0, 5, node->token->space, jmp_));
        endInst->token->index = node->token->index;
      }
      curr = curr->right;
    }

    Token *new = new_token("endif", 0, 5, node->token->space, bloc_);

    new->index = node->token->index;
    new_inst(new);
    // free_token(lastInst);
    return node->left->token;
    break;
  }
  case while_:
  {
    // condition
    node->token->type = bloc_;
    node->token->name = strdup("while");
    node->token->index = ++bloc_index;
    inst = new_inst(node->token);

    // Token *result =
    generate_ir(node->left); // TODO: check if it's boolean
    Token *end = copy_token(node->token);
    end->type = jne_;
    if (end->name)
      free(end->name);
    end->name = strdup("endwhile");
    new_inst(end);

    Node *curr = node->right;
    while (curr) // if code bloc
    {
      generate_ir(curr->left);
      curr = curr->right;
    }

    Token *lastInst = copy_token(node->token);
    lastInst->type = jmp_;
    free(lastInst->name);
    lastInst->name = strdup("while");
    new_inst(lastInst); // jne to endif

    lastInst = copy_token(node->token);
    lastInst->type = bloc_;
    free(lastInst->name);
    lastInst->name = strdup("endwhile");
    new_inst(lastInst); // jne to endif
    break;
  }
  case fdec_:
  {
    new_function(node);
    enter_scoop(node->token->name);
    Token *fcall = copy_token(node->token);

    size_t tmp_ptr = ptr;
    ptr = 0;

    fcall->type = fdec_;
    inst = new_inst(fcall);
    Node *curr;

    pnode(node, NULL, 0);
    // inside_function = true;
    if (node->left) // arguments
    {
      // TODO: make it compatibel with data type
      char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
      char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
      size_t i = 0;

      // size_t ptr = 8;

      Token **list = NULL;
      size_t list_pos = 0;
      size_t list_size = 0;

      curr = node->left->right;
      while (curr && curr->left)
      {
        Inst *inst = new_inst(new_token(NULL, 0, 0, node->token->space, pop_));
        inst->left = generate_ir(curr->left);
        // debug(RED"pop [%s]\n" RESET, to_string(inst->left->type));
        // exit(1);
        curr->left->token->declare = false;
        // inst->left = curr->left->token;
        if (eregs[i])
        {
          if (inst->left->type == int_)
            inst->right = new_token(eregs[i], 0, strlen(eregs[i]), node->token->space, 0);
          else
            inst->right = new_token(rregs[i], 0, strlen(rregs[i]), node->token->space, 0);
          i++;
        }
        else
        {
          inst->right = new_token(NULL, 0, 0, node->token->space, 0);
          inst->right->ptr = (ptr += 8);
          if (list == NULL)
          {
            list_size = 10;
            list = calloc(list_size, sizeof(Token *));
          }
          else if (list_pos + 1 == list_size)
          {
            Token **tmp = calloc(list_size * 2, sizeof(Token *));
            memcpy(tmp, list, list_pos * sizeof(Token *));
            free(list);
            list_size *= 2;
            list = tmp;
          }
          list[list_pos++] = inst->right;
        }
        // Token *arg = generate_ir(curr->left);
        curr = curr->right;
      }
      i = 0;
      while (i < list_pos / 2)
      {
        size_t tmp = list[i]->ptr;
        list[i]->ptr = list[list_pos - i - 1]->ptr;
        list[list_pos - i - 1]->ptr = tmp;
        i++;
      }
      free(list);
    }
    debug("%s:%d ptr is %zu\n", FUNC, LINE, ptr);
    ptr = tmp_ptr;
    // debug(SPLIT);
    // pnode(node, NULL, 0);
    // debug(SPLIT);
    // pnode(node->right, NULL, 0);
    // debug(SPLIT);
    // exit(1);
    curr = node->right;
    while (curr)
    {
      generate_ir(curr->left);
      curr = curr->right;
    }
    // exit(1);
    Token *new = new_token(NULL, 0, 0, node->token->space, end_bloc_);
    new->name = strdup(node->token->name);
    new->index = node->token->index;
    // new->index = node->token->index;
    new_inst(new);

    exit_scoop();
    break;
  }
  case ret_:
  {
    inst = new_inst(node->token);
    inst->left = generate_ir(node->left);
    break;
  }
  case fcall_:
  {
    // pnode(node, NULL, 0);
    size_t tmp_arg_ptr = arg_ptr;
    size_t tmp_ptr = ptr;
    arg_ptr = 8;
    ptr = 8; // TODO: to be checked
    if (strcmp(node->token->name, "output") == 0)
    {
      Node *curr = node;
      char *fname = NULL;
      char *regname = NULL;
#if 0
            while(curr->left)
            {
                pnode(curr->left, NULL, 0);
                curr = curr->right;
            }
#endif
      // exit(1);
      curr = node;
      while (curr->left)
      {
        // debug("loop\n");
        // Node *arg = curr->left;
        Token *left = generate_ir(curr->left);
        fname = NULL;
        switch (left->type)
        {
        // TODO: add other types / maybe you will remove it
        case chars_:
          fname = "putstr";
          regname = "rdi";
          break;
        case int_:
          fname = "putnbr";
          regname = "edi";
          break;
        default:
          error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type));
          exit(1);
        }
        if (fname)
        {
          inst = new_inst(new_token(NULL, 0, 0, node->token->space, push_));
          // TODO: it causes problem in output("hello world")
          // TODO: remove use register depends on data type
          // left = copy_token(left);
          left->declare = false;
          inst->left = left;
          inst->right = new_token(regname, 0, strlen(regname), node->token->space, 0);
          // new_inst(left);
          inst = new_inst(new_token(fname, 0, strlen(fname), node->token->space, fcall_));
          inst->token->isbuiltin = true;
          inst->left = left;
        }
        curr = curr->right;
      }
    }
    else
    {
      Node *func = get_function(node->token->name);
      node->token->retType = func->token->retType;
      Node *arg = func->left->right;
      debug("has the following arguments\n");
      while (arg)
      {
        ptoken(arg->left->token);
        arg = arg->right;
      }
      // TODO: make it compatibel with data type
      char *eregs[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d", NULL};
      char *rregs[] = {"rdi", "rsi", "rdx", "rcx", "r8d", "r9d", NULL};
      int i = 0;
      int j = 0;
      int k = 0;
      // size_t ptr = 8;

      Node *curr = node;
      arg = func->left->right;
      while (curr->left && arg)
      {
        Token *left = generate_ir(curr->left);
        Inst *inst = new_inst(new_token(NULL, 0, 0, node->token->space, push_));
        inst->left = left;
        // debug("%s => %s\n",
        // to_string(inst->left->type),
        // to_string(arg->left->token->type));
        if (
            inst->left->type != arg->left->token->type &&
            inst->left->retType != arg->left->token->type &&
            !(inst->left->type == chars_ && arg->left->token->type == ptr_))
        {
          error("%s:%d Incompatible type for function call <%s>\n", FUNC, LINE, func->token->name);
          // TODO: add line after
          exit(1);
        }
        if (eregs[i])
        {
          if (inst->left->type == int_)
            inst->right = new_token(eregs[i], 0, strlen(eregs[i]), node->token->space, 0);
          else
            inst->right = new_token(rregs[i], 0, strlen(rregs[i]), node->token->space, 0);
          i++;
        }
        else
          inst->right = new_token(NULL, 0, 0, node->token->space, 0);
        curr = curr->right;
        arg = arg->right;
        j++;
      }
      if (arg)
      {
        k = j;
        while (arg)
        {
          arg = arg->right;
          k++;
        }
        error(
            "function [%s] takes %d argument(s) but receive %d\n", func->token->name, k, j);
      }
      else if (curr->left)
      {
        k = j;
        while (curr->left)
        {
          curr = curr->right;
          k++;
        }
        error(
            "function [%s] takes %d argument(s) but receive %d\n", func->token->name, j, k);
      }
      new_inst(node->token);
    }
    arg_ptr = tmp_arg_ptr;
    ptr = tmp_ptr;
    return node->token;
    break;
  }
  case dot_:
  {
    // TODO: use namespaces here
    debug("found dot\n");
    Token *left = generate_ir(node->left);
    if (left->type == module_)
    {
      // is module
      // TODO: module must have a valid name like "sys"
      debug("left is module\n");
      // Token *right =
      generate_ir(node->right);

      // exit(1);
    }
    else
    {
      // is attribute or method
      error("handle this case in dot\n");
    }
    return left;
    // exit(1);
    break;
  }
  case bool_:
  case int_:
  case chars_:
  case char_:
  case float_:
  case void_:
  {
    inst = new_inst(node->token);
    break;
  }
  case assign_:
  case add_:
  case sub_:
  case div_:
  case mul_:
  case not_equal_:
  case equal_:
  case less_:
  case more_:
  case less_equal_:
  case more_equal_:
  {
    Token *left = generate_ir(node->left);
    Token *right = generate_ir(node->right);
    // TODO: fix this later for cases like char + int
    if (
        left->type != right->type && left->type != right->retType &&
        !(left->type == chars_ && right->retType == ptr_))
    {
      debug("%s:%d left[%s], right [%s]\n", FUNC, LINE, to_string(left->retType), to_string(right->type));
      error("%s:%d Incompatible type for [%s] and [%s]", FUNC, LINE,
            to_string(left->type), to_string(right->type));
      exit(1);
    }
    inst = new_inst(node->token);
    inst->left = left;
    inst->right = right;
    switch (node->token->type)
    {
    case assign_:
      node->token->retType = left->type;
      break;
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
      if (left->type != int_ && left->type != float_)
      {
        error("Invalid operation [%s] between [%s] and [%s]\n",
              to_string(node->token->type), to_string(left->type), to_string(right->type));
        exit(1);
      }
      node->token->retType = left->type;
      if (right->type == int_)
        node->token->creg = "eax";
      else if (right->type == float_)
        node->token->creg = "xmm0";
      break;
    }
    case not_equal_:
    case equal_:
    case less_:
    case more_:
    case less_equal_:
    case more_equal_:
      node->token->retType = bool_;
      node->token->index = ++bloc_index;
      debug(RED "this is [%d]\n" RESET, node->token->sreg);
      break;
    default:
      break;
    }
    // inst->token->type = left->type; // TODO: to be checked
    break;
  }
  case and_:
  case or_:
  {
    // TODO: check if left nad right are type is bool or retType is bool
    Token *left = generate_ir(node->left);

    node->token->index = ++bloc_index;
    Token *end = copy_token(node->token);
    if (node->token->type == and_)
    {
      end->type = jne_;
      if (end->name)
        free(end->name);
      end->name = strdup(to_string(node->token->type));
      new_inst(end);
    }
    else if (node->token->type == or_)
    {
      end->type = je_;
      if (end->name)
        free(end->name);
      end->name = strdup(to_string(node->token->type));
      new_inst(end);
    }
    Token *right = generate_ir(node->right);
    end = copy_token(end);
    end->type = bloc_;
    new_inst(end);

    node->token->type = bool_;
    node->token->creg = "al";

    return node->token;
    break;
  }
  case struct_:
  {
    if (node->token->declare)
    {
      inst = new_inst(node->token);
      Node *curr = node->left->right;

      while (curr)
      {
        curr->left->token->declare = false;
        generate_ir(curr->left);
        curr->left->token->ptr = node->token->offset -
                                 (ptr + curr->left->token->offset);
        curr->left->token->declare = true;
        curr = curr->right;
      }
      new_inst(new_token(inst->token->name, 0,
                         strlen(inst->token->name), inst->token->space, end_struct_));
      debug(RED "struct offset %zu\n" RESET, node->token->offset);
      ptr += node->token->offset;
    }
    else
      return node->token;
    break;
  };
  default:
  {
    error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(node->token->type));
    exit(1);
    break;
  }
  }
  return inst->token;
}

void print_ir()
{
  debug(SPLIT);
  int j = 0;
  for (int i = 0; insts && insts[i]; i++)
  {
    Token *curr = insts[i]->token;
    Token *left = insts[i]->left;
    Token *right = insts[i]->right;
    if (curr->remove)
    {
      error("%s:%d this condtion must never exists\n", FUNC, LINE);
      exit(1);
      continue;
    }
    switch (curr->type)
    {
    case assign_:
    {
      curr->sreg = left->sreg;
      debug("r%.2d: %s [%s] ", curr->sreg, to_string(curr->type), to_string(left->type));
      debug("%s in (%d) to ", left->name, left->sreg);
      if (right->sreg)
        debug("r%.2d", right->sreg);
      else
      {
        switch (right->type)
        { // TODO: handle the other cases
        case int_:
          debug("%lld", right->Int.value);
          break;
        case bool_:
          debug("%s", right->Bool.value ? "True" : "False");
          break;
        case float_:
          debug("%f", right->Float.value);
          break;
        case char_:
          debug("%c", right->Char.value);
          break;
        case chars_:
          debug("%s", right->Chars.value);
          break;
        default:
          error("%s:%d: handle this case\n", FUNC, LINE);
          exit(1);
          break;
        }
      }
      debug("\n");
      break;
    }
    case fcall_:
    {
      debug("r%.2d: call %s\n", curr->sreg, curr->name);
      break;
    }
    case add_:
    case sub_:
    case mul_:
    case div_:
    case equal_:
    case less_:
    case more_:
    case less_equal_:
    case more_equal_:
    case not_equal_:
    {
      // TODO: set invalid operation for boolean type
      debug("r%.2d: %s ", curr->sreg, to_string(curr->type));
      if (left->sreg)
        debug("r%.2d", left->sreg);
      else
        switch (left->type)
        {
        case int_:
          debug("%lld", left->Int.value);
          break;
        case char_:
          debug("%c", left->Char.value);
          break;
        case chars_:
          debug("%s", left->Chars.value);
          break;
        case float_:
          debug("%f", left->Float.value);
          break;
        default:
          break;
        }

      if (left->name)
        debug(" (%s)", left->name);
      debug(" to ");
      if (right->sreg)
        debug("r%.2d", right->sreg);
      else
        switch (right->type)
        {
        case int_:
          debug("%lld", right->Int.value);
          break;
        case char_:
          debug("%c", right->Char.value);
          break;
        case chars_:
          debug("%s", right->Chars.value);
          break;
        case float_:
          debug("%f", right->Float.value);
          break;
          ;
        default:
          break;
        }

      if (right->name)
        debug(" (%s)", right->name);
      debug("\n");
      break;
    }
    case int_:
    case char_:
    case bool_:
    case chars_:
    case float_:
    {
      debug("r%.2d: [%s] ", curr->sreg, to_string(curr->type));
      if (curr->declare)
        debug("declare %s PTR=[%zu]", curr->name, curr->ptr);
      else if (curr->name)
        debug("variable %s ", curr->name);
      else if (curr->type == int_)
        debug("value %lld ", curr->Int.value);
      else if (curr->type == char_)
        debug("value %c ", curr->Char.value);
      else if (curr->type == bool_)
        debug("value %s ", curr->Bool.value ? "True" : "False");
      else if (curr->type == float_)
      {
        curr->index = ++float_index;
        debug("value %f ", curr->Float.value);
      }
      else if (curr->type == chars_)
      {
        curr->index = ++str_index;
        debug("value %s in STR%zu ", curr->Chars.value, curr->index);
      }
      else
        error("handle this case in generate ir line %d\n", LINE);
      // if(curr->isarg)
      //     debug(" [argument]");
      debug("\n");
      break;
    }
    case void_:
    {
      debug("r%.2d: [%s]\n", curr->sreg, to_string(curr->type));
      break;
    }
    case struct_:
    {
      debug("r%.2d: [%s] ", curr->sreg, to_string(curr->type));
      if (curr->declare)
        debug("declare %s PTR=[%zu]", curr->name, curr->ptr);
      debug("\n");
      break;
    }
    case push_:
    {
      // TODO: check all cases
      debug("rxx: push ");
      // if(left->ptr)
      debug("PTR r%.2d ", left->sreg);
      if (right->name)
        debug("to %s", right->name);
      debug("\n");
      break;
    }
    case pop_:
    {
      // TODO: check all cases
      debug("rxx: pop ");
      if (left->ptr)
        debug("PTR [%zu] ", left->ptr);
      debug("from ");
      if (right->name)
        debug("%s", right->name);
      else
        debug("[%zu]", right->ptr);
      debug("\n");
      break;
    }
    /*
        TODO:
            + if function has datatype must have return
            + return value must be compatible with function
    */
    case ret_:
    {
      debug("rxx: return ");
      ptoken(left);
      break;
    }
    case jne_:
      debug("rxx: jne %s%zu\n", curr->name, curr->index);
      break;
    case je_:
      debug("rxx: je %s%zu\n", curr->name, curr->index);
      break;
    case jmp_:
      debug("rxx: jmp %s%zu\n", curr->name, curr->index);
      break;
    case bloc_:
    case end_bloc_:
    case end_struct_:
    case fdec_:
      debug("%s%zu: [%s]\n", curr->name, curr->index, to_string(curr->type));
      break;
    default:
      debug("%sPrint IR: Unkown inst [%s]%s\n", RED, to_string(curr->type), RESET);
      break;
    }
    j++;
  }
  debug("%.2d Instructions on total\n", j);
  debug(SPLIT);
}

bool optimize_ir(int op_index)
{
#if 1
  bool did_optimize = false;
  switch (op_index)
  {
  case 0:
  {
    debug("OPTIMIZATION %d (calculate operations on constant type 0)\n", op_index);
    for (int i = 0; insts[i]; i++)
    {
      Token *token = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      if (check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i]->token->type))
      {
        if (
            // TODO: check it left nad right are compatible
            check_type((Type[]){int_, float_, chars_, 0}, left->type) &&
            check_type((Type[]){int_, float_, chars_, 0}, right->type) &&
            !left->name && !right->name)
        {

          switch (left->type)
          {
          case int_:
            switch (token->type)
            {
            case add_:
              token->Int.value = left->Int.value + right->Int.value;
              break;
            case sub_:
              token->Int.value = left->Int.value - right->Int.value;
              break;
            case mul_:
              token->Int.value = left->Int.value * right->Int.value;
              break;
            case div_:
              token->Int.value = left->Int.value / right->Int.value;
              break;
            default:
              break;
            }
            break;
          case float_:
            left->index = 0;
            right->index = 0;
            token->index = ++float_index;
            switch (token->type)
            {
            case add_:
              token->Float.value = left->Float.value + right->Float.value;
              break;
            case sub_:
              token->Float.value = left->Float.value - right->Float.value;
              break;
            case mul_:
              token->Float.value = left->Float.value * right->Float.value;
              break;
            case div_:
              token->Float.value = left->Float.value / right->Float.value;
              break;
            default:
              break;
            }
          // case chars_:
          //     switch(token->type)
          //     {
          //     case add_: token->Chars.value = strjoin(left, right); break;
          //     default:
          //         error("Invalid %s op in chars\n", to_string(token->type)); break;
          //     }
          default:
            error("%s: %d handle this case\n", FUNC, LINE);
            break;
          }
          token->type = left->type;
          left->remove = true;
          right->remove = true;
          insts = copy_insts(first_insts, insts, inst_pos, inst_size);
          i = 0;
          did_optimize = true;
        }
      }
      if (check_type((Type[]){int_, float_, chars_, bool_, char_, 0}, token->type) && !token->name)
      {
        // token->c = 0;
        // free(token->creg);
        token->creg = NULL;
        // debug("found to remove in r%d\n", token->r1);
        token->remove = true;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        i = 0;
        did_optimize = true;
      }
    }
    break;
  }
  case 1:
  {
    // TODO: do comparision operation on numbers etc...
    debug("OPTIMIZATION %d (calculate operations on numbers type 1)\n", op_index);
    int i = 1;
    while (insts[i])
    {
      Token *token = insts[i]->token;
      Token *left = insts[i]->left;
      Token *right = insts[i]->right;
      if (
          //  TODO: handle string also here X'D ma fiyach daba
          check_type((Type[]){add_, sub_, mul_, div_, 0}, token->type) &&
          insts[i - 1]->token->type == add_ &&
          left == insts[i - 1]->token &&
          !insts[i - 1]->right->name &&
          !right->name)
      {
        // debug("%sfound %s\n", RED, RESET);
        token->remove = true;
        switch (token->type)
        {
        case add_:
          insts[i - 1]->right->Int.value += right->Int.value;
          break;
        case sub_:
          insts[i - 1]->right->Int.value -= right->Int.value;
          break;
        case mul_:
          insts[i - 1]->right->Int.value *= right->Int.value;
          break;
        case div_:
          insts[i - 1]->right->Int.value /= right->Int.value;
          break;
        default:
          break;
        }
        // debug("value is %lld\n", insts[i - 1]->right->Int.value);
        if (insts[i + 1]->left == token)
          insts[i + 1]->left = insts[i - 1]->token;
        i = 1;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        did_optimize = true;
        continue;
      }
      i++;
    }
    break;
  }
  case 2:
  {
    debug("OPTIMIZATION %d (remove reassigned variables)\n", op_index);
    for (int i = 0; insts[i]; i++)
    {
      if (insts[i]->token->declare)
      {
        int j = i + 1;
        while (insts[j] && insts[j]->token->space == insts[i]->token->space)
        {
          debug("line %d: ", LINE);
          ptoken(insts[j]->token);
          if (insts[j]->token->type == assign_ && insts[j]->left == insts[i]->token)
          {
            insts[i]->token->declare = false;
            insts[i]->token->remove = true;
            did_optimize = true;
            break;
          }
          if (
              (insts[j]->left && insts[j]->left->sreg == insts[i]->token->sreg) ||
              (insts[j]->right && insts[j]->right->sreg == insts[i]->token->sreg))
            break;
          j++;
        }
      }
      else if (insts[i]->token->type == assign_)
      {
        int j = i + 1;
        while (insts[j] && insts[j]->token->space == insts[i]->token->space)
        {
          if (!insts[j]->left || !insts[j]->right || !insts[i]->token)
          {
            j++;
            continue;
          }
          if (
              insts[j]->token->type == assign_ &&
              insts[j]->left == insts[i]->left)
          {
            insts[i]->token->remove = true;
            did_optimize = true;
            break;
          }
          // if the variable is used some where
          else if (
              insts[j]->left->sreg == insts[i]->token->sreg ||
              insts[j]->right->sreg == insts[i]->token->sreg)
            break;
          j++;
        }
      }
    }
    break;
  }
  case 3:
  {
    debug("OPTIMIZATION %d (remove followed return instructions)\n", op_index);
    for (int i = 1; insts[i]; i++)
    {
      if (insts[i]->token->type == ret_ && insts[i - 1]->token->type == ret_)
      {
        did_optimize = true;
        insts[i]->token->remove = true;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        i = 1;
      }
    }
    break;
  }
  case 4:
  {
    debug("OPTIMIZATION %d (remove unused instructions)\n", op_index);
    for (int i = 1; insts[i]; i++)
    {
      if (
          check_type((Type[]){add_, sub_, mul_, div_, 0}, insts[i - 1]->token->type) &&
          insts[i]->left->sreg != insts[i - 1]->token->sreg &&
          insts[i]->right->sreg != insts[i - 1]->token->sreg)
      {
        did_optimize = true;
        insts[i - 1]->token->remove = true;
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        i = 1;
      }
    }
    break;
  }
  default:
    break;
  }
  return did_optimize;
#endif
}

#endif
#endif
