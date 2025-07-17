#include "./include/header.h"

void setAttrName(Token *parent, Token *child)
{
    if (parent)
    {
        child->is_attr = true;
        if (child->type != STRUCT_CALL)
            child->ptr = parent->ptr + child->offset;
        else
            child->ptr = parent->ptr + child->offset;

        char *name = strjoin(parent->name, ".", child->name);
        setName(child, name);
        free(name);
    }
    for (int i = 0; i < child->Struct.pos; i++)
    {
        Token *attr = child->Struct.attrs[i];
        if (attr->type == STRUCT_CALL)
        {
            char *name = attr->name;
            int space = attr->space;
            int offset = attr->offset;

            attr = get_struct(attr->Struct.name);
            setName(attr, name);
            attr->offset = offset;
            attr->space = space;
            attr->type = STRUCT_CALL;
            setAttrName(child, attr);
            child->Struct.attrs[i] = attr;
        }
        else
        {
            setAttrName(child, attr);
        }
    }
}

Token *new_variable(Token *token)
{
#if DEBUG
    debug(CYAN "new variable [%s] [%s] in scoop %k\n" RESET, token->name, to_string(token->type), scoop->token);
#endif
    for (int i = 0; i < scoop->vpos; i++)
    {
        Token *curr = scoop->vars[i];
        if (strcmp(curr->name, token->name) == 0) check(1, "Redefinition of %s\n", token->name);
    }
    if (token->ptr)
    {
        debug(RED"%k already has ptr"RESET, token);
        todo(1, "");
    }
    if (token->type == STRUCT_CALL)
    {
        token->ptr = ptr;
        if (token->is_arg) token->ptr += sizeofToken(token->Struct.attrs[0]);
        setAttrName(NULL, token);
        inc_ptr(sizeofToken(token)); // TODO: each struct must have attributes
    }
    else
    {
        inc_ptr(sizeofToken(token));
        token->ptr = ptr;
    }
    new_inst(token);
    add_variable(scoop, token);
    return token;
}

// ----------------------------------------------------------------------------
// Code Generation
// ----------------------------------------------------------------------------
// IR Generation - Produce intermediate representation
// Optimization - Apply optimization passes

Token *if_ir(Node *node)
{
    enter_scoop(node);

    Inst *inst = new_inst(node->token);
    setName(inst->token, "if");
    inst->token->type =  BLOC;
    inst->token->index = ++bloc_index;

    Token *cond = generate_ir(node->left); // TODO: check if it's boolean
    if (!cond) return NULL;
    setName(cond, "endif");
    cond->index = inst->token->index;
    --bloc_index;

    Token *next = cond;
    // code bloc
    for (int i = 0; i < node->cpos && !found_error; i++) generate_ir(node->children[i]);

    Inst *end = NULL;
    if (node->right->cpos)
    {
        end = new_inst(new_token(JMP, node->token->space + TAB));
        setName(end->token, "endif");
        end->token->index = node->token->index;
    }
    Node *subs = node->right;
    for (int i = 0; i < subs->cpos; i++)
    {
        Node *curr = subs->children[i];
        if (curr->token->type == ELIF)
        {
            curr->token->index = ++bloc_index;
            curr->token->type = BLOC;
            setName(curr->token, "elif");
            char *name = strdup(next->name);
            int index = next->index;
            {
                setName(next, "elif");
                next->index = curr->token->index;
                next = copy_token(next);
            }
            new_inst(curr->token); // elif bloc
            setName(curr->left->token, name);
            free(name);
            generate_ir(curr->left); // elif condition, TODO: check is boolean
            --bloc_index;
            curr->left->token->index = index;
            next = curr->left->token;
            for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
            end = new_inst(new_token(JMP, node->token->space + TAB));
            setName(end->token, "endif");
            end->token->index = node->token->index;
        }
        else if (curr->token->type == ELSE)
        {
            curr->token->index = ++bloc_index;
            curr->token->type = BLOC;
            setName(curr->token, "else");
            new_inst(curr->token);
            {
                setName(next, "else");
                next->index = curr->token->index;
                next = copy_token(next);
            }
            for (int j = 0; j < curr->cpos; j++) generate_ir(curr->children[j]);
            break;
        }

    }
    Token *new = new_token(BLOC, node->token->space);
    setName(new, "endif");
    new->index = node->token->index;
    new_inst(new);
    exit_scoop();
    return node->left->token;
}

Token *while_ir(Node *node)
{
    enter_scoop(node);
    node->token->type = BLOC;
    setName(node->token, "while");
    node->token->index = ++bloc_index;
    Inst *inst = new_inst(node->token);

    setName(node->left->token, "endwhile");
    generate_ir(node->left); // TODO: check if it's boolean
    node->left->token->index = node->token->index;

    for (int i = 0; i < node->cpos; i++) // while code bloc
    {
        Node *curr = node->children[i];
        Token *next;
        switch (curr->token->type)
        {
        case BREAK:
            next = copy_token(node->left->token);
            next->type = JMP;
            setName(next, "endwhile");
            new_inst(next); // jmp back to while loop
            break;
        case CONTINUE:
            next = copy_token(node->token);
            next->type = JMP;
            setName(next, "while");
            new_inst(next); // jmp back to while loop
            break;
        default:
            generate_ir(curr);
            break;
        }
    }

    Token *next = copy_token(node->token);
    next->type = JMP;
    setName(next, "while");
    next->space += TAB;
    new_inst(next); // jmp back to while loop

    next = copy_token(node->token);
    next->type = BLOC;
    setName(next, "endwhile");
    new_inst(next); // end while bloc
    exit_scoop();
    return inst->token;
}

Token *inialize_struct(Node *node)
{
    for (int i = 0; i < node->token->Struct.pos; i++)
    {
        Token *attr = node->token->Struct.attrs[i];
        if (attr->type == STRUCT_CALL)
        {
            Node *tmp = new_node(attr);
            // attr->is_attr = true;
            attr->ptr =  node->token->ptr + node->token->offset;
            inialize_struct(tmp);
            free_node(tmp);
        }
        else
        {
            attr->ptr = node->token->ptr + node->token->offset - attr->offset;
            Node *tmp = new_node(new_token(ASSIGN, node->token->space));
            tmp->token->ir_reg = attr->ir_reg;

            tmp->left = new_node(attr);
            tmp->right = new_node(new_token(DEFAULT, attr->space));
            to_default(tmp->right->token, tmp->left->token->type);
            generate_ir(tmp);
            free_node(tmp);
        }
    }
    // if (!node->token->is_attr) inc_ptr(node->token->offset);
    return node->token;
}

Token* inialize_variable(Node *node)
{
    node->token->is_data_type = false;
    if (node->token->type == STRUCT_CALL)
    {
        inialize_struct(node);
    }
    else
    {
        Node *assign = new_node(new_token(ASSIGN, node->token->space));
        assign->token->ir_reg = node->token->ir_reg;
        assign->left = copy_node(node);
        assign->right =  new_node(new_token(DEFAULT, node->token->space));
        to_default(assign->right->token, assign->left->token->type);
        generate_ir(assign);
        free_node(assign);
    }
    return node->token;
}

void set_func_dec_regs(Token *child, int *r_ptr, bool is_proto)
{
    if (!child->is_attr)
    {
        child->is_arg = true;
        new_variable(child);
    }

    // TODO: src should not be used I guess
    // if argument is struct call and not reference
    Token *src = new_token(child->type, child->space);
    int r = *r_ptr;
    if (r < regLen)
    {
        if (child->is_ref)
        {
            setReg(src, rregs[r]);
        }
        else
        {
            // TODO: add other data type and math operations
            switch (child->type)
            {
            case LONG: case CHARS: case PTR: setReg(src, rregs[r]); break;
            case FLOAT: setReg(src, rregs[r]); break; // TODO: to be checked
            case INT: case CHAR: case BOOL: setReg(src, eregs[r]); break;
            case STRUCT_CALL:
            {
                for (int j = 0; j < child->Struct.pos; j++)
                {
                    set_func_dec_regs(child->Struct.attrs[j],  &r, is_proto);
                    debug(RED"<<%k>>\n"RESET, child->Struct.attrs[j]);
                }
                // todo(1, "found");

                break;
            }
            default: todo(1, "set ir_reg for %s", to_string(child->type));
            };
        }
        r++;
    }
    else
    {
        // TODO:
        todo(1, "implement assigning function argument using PTR");
    }


    if (child->is_ref /*&& !child->has_ref*/)
    {
        src->is_ref = true;
        src->has_ref = true;
    }
    if (is_proto)
    {
        // do nothing !!
    }
    else if (child->type != STRUCT_CALL || child->is_ref)
    {
        if (src->is_ref) child->has_ref = true;
        Node *assign = new_node(new_token(ASSIGN, child->space));
        assign->left = new_node(child);
        assign->right =  new_node(src);

        generate_ir(assign);
        free_node(assign);
    }
    *r_ptr = r;
}

Token *func_dec_ir(Node *node)
{
    new_function(node);
    enter_scoop(node);
    int tmp_ptr = ptr;
    ptr = 0;

    Inst* inst = NULL;
    if (!node->token->is_proto) inst = new_inst(node->token);

    // parameters
    Node *curr = node->left;
    for (int i = 0, r = 0; curr && i < curr->cpos && !found_error; i++)
    {
        Node *child = curr->children[i];
        set_func_dec_regs(child->token, &r, node->token->is_proto);
    }

    if (node->token->is_proto) set_remove(node);
    // code bloc
    for (int i = 0; !node->token->is_proto && i < node->cpos; i++)
    {
        Node *child = node->children[i];
        generate_ir(child);
    }

    if (!node->token->is_proto)
    {
        // TODO: if RETURN not found add it
        Token *new = new_token(END_BLOC, node->token->space);
        new->name = strdup(node->token->name);
        new_inst(new);
        node->token->ptr = ptr;
        ptr = tmp_ptr;
    }
    exit_scoop();
    if (!node->token->is_proto) return inst->token;
    return NULL;
}

void set_func_call_regs(int *r_ptr, Token *src, Token *dist, Node *node)
{
    int r = *r_ptr;
    if (r < regLen)
    {
        // added because unfction declaration params do have ptrs
        dist->ptr = 0;
        if (dist->is_ref) setReg(dist, rregs[r]);
        else
        {
            // TODO: add other data type and math operations
            switch (dist->type)
            {
            case LONG: case CHARS: setReg(dist, rregs[r]); break;
            case INT:   setReg(dist, eregs[r]); break;
            case CHAR:  setReg(dist, eregs[r]); break;
            case FLOAT: setReg(dist, rregs[r]); break; // TODO: to be checked
            case BOOL:  setReg(dist, eregs[r]); break;
            case STRUCT_CALL:
            {
                for (int j = 0; j < dist->Struct.pos; j++)
                {
                    set_func_call_regs(&r, src->Struct.attrs[j], dist->Struct.attrs[j], node);
                    // debug(RED"%k\n"RESET, child->Struct.attrs[j]);
                }
                break;
            }
            default: todo(1, "set ir_reg for %s", to_string(dist->type));
            };
        }
        r++;
    }
    else
    {
        todo(1, "implement assigning function argument using PTR");
    }
    if (dist->is_ref)
    {
        dist->is_ref = true;
        dist->has_ref = false;
    }
    if (dist->type != STRUCT_CALL)
    {
        new_inst(dist);
        Node *assign = new_node(new_token(ASSIGN, node->token->space));
        assign->left = new_node(dist);
        assign->right = new_node(src);
        assign->token->ir_reg = dist->ir_reg;

        debug(RED);
        pnode(assign, NULL, 0);
        debug(RESET);

        generate_ir(assign);
        free_node(assign);
        *r_ptr = r;
    }
}

Token *func_call_ir(Node *node)
{
    if (strcmp(node->token->name, "output") == 0)
    {
        setReg(node->token, "eax");
        setName(node->token, "printf");
        Node *fcall = node;

        Node *assign = new_node(new_token(ASSIGN, node->token->space));
        Token *_register = new_token(CHARS, fcall->token->space + TAB);
        _register->creg = strdup("rdi");
        Token *varg = new_token(CHARS, fcall->token->space + TAB);
        varg->index = ++str_index;
        varg->Chars.value = strdup("\"");

        new_inst(_register);
        assign->left = new_node(_register);
        assign->right = new_node(varg);
        assign->token->ir_reg = _register->ir_reg;

        generate_ir(assign);
        free_node(assign);

        for (int i = 0; !found_error && i < fcall->cpos; i++)
        {
            Node *carg = fcall->children[i];
            Token *var = generate_ir(carg);
            if (check(var->type == ID, "Indeclared variable %s", carg->token->name)) break;
            Token *src = new_token(INT, var->space);

            int j = i + 1;
            if (j < regLen)
            {
                // added because unfction declaration params do have ptrs
                // TODO: add other data type and math operations
                src->ptr = 0;
                switch (var->type)
                {
                case CHARS:
                {
                    setReg(src, rregs[j]);
                    char *tmp = strjoin(varg->Chars.value, "%s", NULL);
                    free(varg->Chars.value);
                    varg->Chars.value = tmp;
                    break;
                }
                case INT:
                {
                    setReg(src, eregs[j]);
                    char *tmp = strjoin(varg->Chars.value, "%d", NULL);
                    free(varg->Chars.value);
                    varg->Chars.value = tmp;
                    break;
                }
                // case CHAR:  setReg(src, eregs[j]); break;
                // case FLOAT: setReg(src, rregs[j]); break; // TODO: to be checked
                // case BOOL:  setReg(src, eregs[j]); break;
                default: todo(1, "set ir_reg for %s", to_string(src->type));
                };
            }
            else
            {
                todo(1, "implement assigning function argument using PTR");
            }
            new_inst(src);
            assign = new_node(new_token(ASSIGN, node->token->space));
            assign->left = new_node(src);
            assign->right = new_node(var);
            assign->token->ir_reg = src->ir_reg;

            debug(RED);
            pnode(assign, NULL, 0);
            debug(RESET);

            generate_ir(assign);
            free_node(assign);
        }
        char *tmp = strjoin(varg->Chars.value, "\"", NULL);
        free(varg->Chars.value);
        varg->Chars.value = tmp;
    }
    else
    {
        Node *func = get_function(node->token->name);
        if (!func) return NULL;
        func = copy_node(func);
        node->token->retType = func->token->retType;
        setReg(node->token, func->token->creg);
        Node *fdec = func->left;
        Node *fcall = node;

        for (int i = 0, r = 0; !found_error && i < fcall->cpos && i < fdec->cpos; i++)
        {
            Node *darg = fdec->children[i];
            Node *carg = fcall->children[i]; // will always be ID

            Token *src = generate_ir(carg);

            if (check(src->type == ID, "Indeclared variable %s", carg->token->name)) break;
            Token *dist = copy_token(darg->token);
            set_func_call_regs(&r, src, dist, node);
        }
        free_node(func);
    }
    Inst* inst = new_inst(node->token);
    return inst->token;
}

Token *op_ir(Node *node)
{
    Token *left = generate_ir(node->left);
    Token *right = generate_ir(node->right);
    if (!right || !right) return NULL;

    check(!compatible(left, right), "invalid [%s] op between %s and %s\n",
          to_string(node->token->type), to_string(left->type), to_string(right->type));
    switch (node->token->type)
    {
    case ASSIGN:
    {
        node->token->ir_reg = left->ir_reg;
        node->token->retType = getRetType(node);
        if (left->is_ref) // ir_reg, ptr
        {
            if (right->is_ref) // ir_reg, ptr
            {
                if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
                if (left->has_ref) { node->token->assign_type = REF_REF;/* stop(1, "found")*/}
                else node->token->assign_type = REF_HOLD_REF;
            }
            else if (right->ptr || right->creg) // ptr
            {
                if (left->has_ref) node->token->assign_type = REF_ID;
                else node->token->assign_type = REF_HOLD_ID;
            }
            else // ir_reg, value
            {
                debug("line %d: %n\n", LINE, node);
                // if (check(!left->has_ref, "can not assign to reference that point to nothing")) break;
                node->token->assign_type = REF_VAL;
            }
            left->has_ref = true;
        }
        else if (left->ptr || left->creg) // ir_reg, ptr
        {
            if (right->is_ref) // ir_reg, ptr
            {
                if (check(!right->has_ref, "can not assign from reference that point to nothing")) break;
                node->token->assign_type = ID_REF;
            }
            else if (right->ptr) // ptr
                node->token->assign_type = ID_ID;
            else // ir_reg, value
                node->token->assign_type = ID_VAL;
        }
        else if (left->type == STRUCT_CALL)
        {
            stop(1, "check this");
            debug(">> %k\n", left);
            debug(">> %k\n", right);
            // TODO: check compatibility
            for (int i = 0; i < left->Struct.pos; i++)
            {
                Node *tmp = new_node(new_token(ASSIGN, node->token->space));
                tmp->left = new_node(left->Struct.attrs[i]);
                tmp->right = new_node(right->Struct.attrs[i]);
                op_ir(tmp);
                free_node(tmp);
            }
            return NULL;
            // exit(1);
        }
        else
        {
            pnode(node, NULL, 0);
            debug("<%k>\n", left);
            debug("<%k>\n", right);
            todo(1, "Invalid assignment");
        }
        break;
    }
    case ADD: case SUB: case MUL: case DIV: case ADD_ASSIGN:
    {
        // TODO: to be checked
        node->token->retType = getRetType(node);
        if (node->token->retType  == INT) setReg(node->token, "eax");
        else if (node->token->retType == FLOAT) setReg(node->token, "xmm0");
        else check(1, "handle this case");
        break;
    }
    case NOT_EQUAL: case EQUAL: case LESS:
    case MORE: case LESS_EQUAL: case MORE_EQUAL:
    {
        node->token->retType = BOOL;
        node->token->index = ++bloc_index;
        break;
    }
    default: check(1, "handle [%s]", to_string(node->token->type)); break;
    }
    Inst *inst = new_inst(node->token);
    inst->left = left;
    inst->right = right;

    return node->token;
}

Token *generate_ir(Node *node)
{
    if (found_error) return NULL;
    switch (node->token->type)
    {
    case ID:
    {
        Token *find = get_variable(node->token->name);
        if (find) return find;
        return node->token;
    }
    case INT: case BOOL: case CHAR: case STRUCT_CALL:
    case FLOAT: case LONG: case CHARS: case PTR:
    {
        if (!node->token->is_data_type) return node->token;
        // variable declaration
        new_variable(node->token);
        return inialize_variable(node);
    }
    case ASSIGN: case ADD_ASSIGN: case SUB_ASSIGN: case MUL_ASSIGN: case DIV_ASSIGN:
    case ADD: case SUB: case MUL: case DIV: case EQUAL: case NOT_EQUAL:
    case LESS: case MORE: case LESS_EQUAL: case MORE_EQUAL:
    {
        // check if right is DEFAULT, then initlize it, and return left
        return op_ir(node);
    }
    case IF:    return if_ir(node);
    case WHILE: return while_ir(node);
    case FDEC:  return func_dec_ir(node);
    case FCALL: return func_call_ir(node);
    case RETURN:
    {
        Token *left = generate_ir(node->left);
        new_inst(node->token)->left = left;
        return node->token;
    }
    case BREAK:
    {
        for (int i = scoopPos; i >= 0; i--)
        {
            Node *scoop = Gscoop[i];
            if (strcmp(scoop->token->name, "while") == 0)
            {
                Token *token = copy_token(node->token);
                token->type = JMP;
                token->index = scoop->token->index;
                setName(token, "endwhile");
                return new_inst(token)->token;
                break;
            }
        }
        // TODO: invalid syntax, break should be inside whie loop
        break;
    }
    case CONTINUE:
    {
        for (int i = scoopPos; i >= 0; i--)
        {
            Node *scoop = Gscoop[i];
            if (strcmp(scoop->token->name, "while") == 0)
            {
                Token *token = copy_token(node->token);
                token->type = JMP;
                token->index = scoop->token->index;
                setName(token, "while");
                return new_inst(token)->token;
                break;
            }
        }
        // TODO: invalid syntax, break should be inside whie loop
        break;
    }
    case DOT:
    {
        Token *left = generate_ir(node->left);
        Token *right = node->right->token;
        if (check(left->type == ID, "undeclared variable %s", left->name)) break;

        debug(RED SPLIT RESET);
        for (int i = 0; i < left->Struct.pos; i++)
        {
            Token *attr = left->Struct.attrs[i];
            debug("%k\n", attr);
            char *to_find = strjoin(left->name, ".", right->name);
            if (strcmp(to_find, attr->name) == 0)
            {
                free(to_find);
                return attr;
            }
            free(to_find);
        }
        debug(RED SPLIT RESET);

        todo(1, "%s has no attribute %s", left->name, right->name);
        return right;
        break;
    }
    case ACCESS:
    {
        debug("line %d: %n\n", LINE, node);
        Token *left = generate_ir(node->left);
        Token *right = generate_ir(node->right);
        // TODO: check if right is a number
        // TODO: check if left is aan iterable data type

        Inst *inst = new_inst(node->token);
        inst->token->creg = strdup("rax");
        inst->token->is_ref = true;
        inst->token->has_ref = true;
        switch (left->type)
        {
        case CHARS:
        {
            inst->token->offset = sizeof(char);
            inst->token->retType = CHAR;
            break;
        }
        case INT:
        {
            inst->token->offset = sizeof(int);
            inst->token->retType = INT;
            break;
        }
        default: todo(1, "handle this case");
        }
        inst->left = left;
        inst->right = right;
        return node->token;
    }
    case STRUCT_DEF: return node->token;
    case DEFAULT:
    {
        todo(1, "who is looking for default ?");
        return node->token;
    }
    default:
    {
        check(1, "handle this case %s", to_string(node->token->type));
        return node->token;
    }
    }
    return NULL;
}

bool optimize_ir()
{
    static int op = 0;
    static bool did_optimize = false;
    bool did_something = false;
    switch (op)
    {
    case 0:
    {
        debug(CYAN "OP[%d] calculate operations on values\n" RESET, op);
        for (int i = 0; insts[i]; i++)
        {
            Token *token = insts[i]->token;
            Token *left = insts[i]->left;
            Token *right = insts[i]->right;

            // TODO: check if left and right are compatible
            // test if left is function, and right is number ...
            if (
                includes(token->type, ADD, SUB, MUL, DIV, 0) && includes(left->type, INT, FLOAT, CHAR, 0)
                && compatible(left, right) && !left->name && !right->name)
            {
                did_something = true;
                did_optimize = true;
                switch (left->type)
                {
                case INT:
                    switch (token->type)
                    {
                    case ADD: token->Int.value = left->Int.value + right->Int.value; break;
                    case SUB: token->Int.value = left->Int.value - right->Int.value; break;
                    case MUL: token->Int.value = left->Int.value * right->Int.value; break;
                    case DIV: token->Int.value = left->Int.value / right->Int.value; break;
                    default: break;
                    }
                    break;
                default:
                    check(1, "handle this case\n", "");
                    break;
                }
                token->type = left->type;
                token->retType = 0;
                insts[i]->left = NULL;
                insts[i]->right = NULL;
                left->remove = true;
                right->remove = true;
                token->ir_reg = 0;
                setReg(token, NULL);
#if DEBUG
                debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
                if (i > 0) i -= 2;
            }
        }
        break;
    }
    case 1:
    {
        debug(CYAN "OP[%d] calculate operations on constants\n" RESET, op);
        for (int i = 1; insts && insts[i]; i++)
        {
            Token *curr_token = insts[i]->token;
            Token *curr_left = insts[i]->left;
            Token *curr_right = insts[i]->right;

            Token *prev_token = insts[i - 1]->token;
            // Token *prev_left = insts[i - 1]->left;
            Token *prev_right = insts[i - 1]->right;

            //  TODO: handle string also here X'D ma fiyach daba
            if (curr_token->type == ADD && prev_token->type == ADD)
            {
                if (curr_left == prev_token && !prev_right->name && !curr_right->name)
                {
                    // prev_right->type = INT;
                    curr_token->remove = true;
                    prev_right->Int.value += curr_right->Int.value;
                    i = 1;
                    copy_insts();
                    did_something = true;
                    did_optimize = true;
#if DEBUG
                    debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
                    continue;
                }
                // else
                // if(curr_right == prev_token && !prev_left->name && !curr_left->name)
                // {
                //   // prev_r->type = INT;
                //   // curr_token->remove = true;
                //   prev_left->Int.value += curr_left->Int.value;
                //   i = 1;
                //   copy_insts();
                //   optimize = true;
                //   continue;
                // }
            }
        }
        break;
    }
    case 2:
    {
        debug(CYAN "OP[%d] remove reassigned variables\n" RESET, op);
        for (int i = 0; insts[i]; i++)
        {
            Token *token = insts[i]->token;
            // if (token->declare)
            // {
            //    for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++)
            //    {
            //       if (insts[j]->token->type == ASSIGN && insts[j]->left->ir_reg == token->ir_reg )
            //       {
            //          // debug(RED"1. remove r%d %k\n"RESET, token->ir_reg, token);
            //          token->declare = false;
            //          token->remove = true;
            //          did_optimize = true;
            //          did_something = true;
            //          insts[j]->left->is_ref = token->is_ref;
            //          break;
            //       }
            //       else if ((insts[j]->left && insts[j]->left->ir_reg == token->ir_reg) ||
            //                (insts[j]->right && insts[j]->right->ir_reg == token->ir_reg)) {
            //          break;
            //       }
            //    }
            // }
            // else
            if (token->type == ASSIGN)
            {
                if (includes(token->assign_type, REF_HOLD_ID, REF_REF, 0)) continue;
                for (int j = i + 1; insts[j] && insts[j]->token->space == token->space; j++)
                {
                    if (!insts[j]->left || !insts[j]->right || !token->ir_reg) continue;
                    // TODO: to be checked
                    // I replaced insts[j]->left == insts[i]->left with insts[j]->left->ir_reg == insts[i]->left->ir_reg
                    if (insts[j]->token->type == ASSIGN && insts[j]->left->ir_reg == token->ir_reg)
                    {
                        // debug(RED"2. remove r%d %k\n"RESET, token->ir_reg, token);
                        token->remove = true;
#if DEBUG
                        debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
                        did_optimize = true;
                        did_something = true;
                        break;
                    }
                    else if (insts[j]->left->ir_reg == token->ir_reg || insts[j]->right->ir_reg == token->ir_reg)
                        break;
                }
            }
        }
        break;
    }
    case 3:
    {
        debug(CYAN"OP[%d] remove followed return instructions\n"RESET, op);
        for (int i = 1; insts[i]; i++)
        {
            if (insts[i]->token->type == RETURN && insts[i - 1]->token->type == RETURN)
            {
                did_optimize = true;
                did_something = true;
                insts[i]->token->remove = true;
#if DEBUG
                debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
                copy_insts();
                i = 1;
            }
        }
        break;
    }
    case 4:
    {
        // TODO: be carefull this one remove anything that don't have ir_reg
        debug(CYAN "OP[%d] remove unused instructions\n"RESET, op);
        for (int i = 0; insts[i]; i++)
        {
            Token *curr = insts[i]->token;
            if (!curr->ptr && !curr->ir_reg && !curr->creg && includes(curr->type, INT, CHARS, CHAR, FLOAT, BOOL, 0))
            {
                curr->remove = true;
                did_something = true;
                did_optimize = true;
#if DEBUG
                debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
            }
        }
        break;
    }
    case 5:
    {
        debug(CYAN "OP[%d] remove structs declarations\n"RESET, op);
        for (int i = 0; insts[i]; i++)
        {
            Token *curr = insts[i]->token;
            Token *left = insts[i]->left;
            // Token *right = insts[i]->right;
            if (curr->type == ASSIGN && left->type == STRUCT_CALL)
            {
                curr->remove = true;
#if DEBUG
                debug(RED"%d: remove %k\n", LINE, insts[i]->token);
#endif
                did_something = true;
                did_optimize = true;
            }
        }
        break;
    }
    case 6:
    {
        debug(CYAN "OP[%d] replace DEFAULT\n"RESET, op);
        for (int i = 0; insts[i]; i++)
        {
            Token *token = insts[i]->token;
            Token *right = insts[i]->right;
            Token *left = insts[i]->left;
            if (token->type == ASSIGN && right->type == DEFAULT)
            {
                todo(1, "there should not be any default remaining here")
                to_default(right, left->type);
            }
        }
        break;
    }
    default:
    {
        op = 0;
        if (!did_optimize) return false;
        did_optimize = false;
        return true;
        break;
    }
    }
    if (did_something)
    {
#if DEBUG
        print_ir();
#endif
    }
    op++;
    return true;
}

