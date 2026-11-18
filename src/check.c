void add_error(int s, int e, char *msg, char *help) {
    uraError error = { .s = s, .e = e, .msg = msg, .help = help };
    append(ura.errors, error);
}

void check_errors(void) {
    if (ura.errors_count == 0)
        return;
    char *cwd = getcwd2();
    size_t cwd_len = strlen(cwd);
    for (int i = 0; i < ura.errors_count; i++) {
        uraError *error = &ura.errors[i];
        File *file = file_at(error->s);
        char *buff = file->buff;
        int s = error->s - file->start;
        int e = error->e - file->start;
        char *path = file->path;
        if (strncmp2(path, cwd, cwd_len) && path[cwd_len] == '/')
            path += cwd_len + 1;
        int line = 1;
        int start = 0;
        for (int j = 0; j < s; j++) {
            if (buff[j] == '\n') {
                line++;
                start = j + 1;
            }
        }
        int end = line_end_in(buff, s);
        int col = s - start;
        int width = snprintf(NULL, 0, "%d", line);

        fprintf(
            stderr, "%s: %s:%d:%d %s\n", RED("error"), path, line, col + 1, error->msg
        );
        fprintf(stderr, "%*s |\n", width, "");
        fprintf(stderr, "%d | %.*s\n", line, end - start, buff + start);
        fprintf(stderr, "%*s | %*s", width, "", col, "");
        for (int j = s; j < e || j == s; j++)
            fprintf(stderr, "^");
        fprintf(stderr, "\n");
        if (error->help)
            fprintf(stderr, "help: %s\n", error->help);
    }
    fprintf(
        stderr,
        "%s: aborting due to %d error%s\n",
        RED("error"),
        (int)ura.errors_count,
        ura.errors_count > 1 ? "s" : ""
    );
    exit(1);
}

void check_type_first(Token *token) {
    int end = 0;
    int depth = 0;
    while (peek(end)->type == LT || depth > 0) {
        if (peek(end)->type == LAST)
            break;
        if (peek(end)->type == LT)
            depth++;
        if (peek(end)->type == GT)
            depth--;
        end++;
    }
    if (peek(end)->type == LBRACK && peek(end + 1)->type == RBRACK)
        end += 2;
    Token *name = peek(end);
    if (name->type != ID || name->line != token->line)
        return;
    Token *last = ura.tokens[ura.exe_pos + end - 1];
    add_error(
        token->s,
        last->e,
        "the type goes after the name",
        format(
            "use %s %.*s%.*s",
            name->name,
            last->e - token->s,
            text_at(token->s),
            line_end(name->e) - name->e,
            text_at(name->e)
        )
    );
    ura.exe_pos += end;
}

char *macro_to_text(Node *node) {
    Token *token = node->token;
    if (token->type == CHARS)
        return token->chars.value;
    if (token->type == ID && strcmp2(token->name, "os")) {
#if defined(_WIN32)
        return "windows";
#elif defined(__APPLE__)
        return "macos";
#else
        return "linux";
#endif
    }
    if (token->type == ID && strcmp2(token->name, "arch")) {
#if defined(__aarch64__) || defined(__arm64__)
        return "arm64";
#else
        return "x86_64";
#endif
    }
    return NULL;
}

bool macro_cond(Node *cond) {
    Token *token = cond->token;
    switch (token->type) {
    case AND:
        return macro_cond(cond->left) && macro_cond(cond->right);
    case OR:
        return macro_cond(cond->left) || macro_cond(cond->right);
    case NOT:
        return !macro_cond(cond->operand);
    case INT:
        return token->num.value != 0;
    case EQ:
    case NQ: {
        char *left = macro_to_text(cond->left);
        char *right = macro_to_text(cond->right);
        if (left && right)
            return strcmp2(left, right) == (token->type == EQ);
        break;
    }
    case FN_CALL: {
        bool is_installed = strcmp2(token->name, "installed");
        bool has_one_arg = cond->children_count == 1;
        bool has_name = has_one_arg && cond->children[0]->token->type == CHARS;
        if (is_installed && has_name)
            return ucg_find_lib(cond->children[0]->token->chars.value) != NULL;
        break;
    }
    default:
        break;
    }
    add_error(
        token->s,
        token->e,
        "@if checks os, arch and installed(\"lib\")",
        "use @if os == \"linux\" and installed(\"SDL2\"):"
    );
    return false;
}

Node *find_drop(Node *type) {
    if (type == NULL || type->token->type != ID || type->declared == NULL)
        return NULL;
    Node *st = type->declared;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != FN_DEC || child->args->symbol == NULL)
            continue;
        if (!strcmp2(child->token->name, "drop"))
            continue;
        return child;
    }
    return NULL;
}

bool needs_drop(Node *type) {
    if (find_drop(type))
        return true;
    if (type == NULL || type->token->type != ID || type->declared == NULL)
        return false;
    Node *st = type->declared;
    for (int i = 0; i < st->children_count; i++) {
        Node *child = st->children[i];
        if (child->token->type != DECLARE)
            continue;
        if (needs_drop(child->var->data_type))
            return true;
    }
    return false;
}

bool lives_in_variable(Node *node) {
    bool is_member = includes(node->token->type, DOT, DOUBLE_DOTS, 0);
    Node *call = is_member ? node->right : node;
    if (call->token->type == FN_CALL)
        return call->to_call && ret_type_of(call->to_call)->token->type == REF;
    if (is_member)
        return true;
    return includes(node->token->type, VAR, PARAM, ACCESS, 0);
}

bool is_local_variable(Node *node) {
    if (!includes(node->token->type, VAR, PARAM, 0) || is_ref_variable(node))
        return false;
    for (int i = ura.scopes_count - 1; i >= 0; i--) {
        Node *scope = ura.scopes[i];
        if (find_in_scope(scope, node->token->name) == node->declared)
            return true;
        if (scope->token->type == FN_DEC)
            return false;
    }
    return false;
}

void check_type(Node *value, Node *expected, char *help) {
    if (expected && value->token->type == LCBRACK) {
        check_struct_value(value, expected);
        return;
    }
    Node *found = type_of(value);
    bool unknown = expected && expected->token->type == ID && !expected->declared;
    if (found == NULL || expected == NULL || unknown)
        return;
    Token *token = value->token;
    if (is_number(value) && is_number(expected)) {
        int bits = expected->token->shift - !expected->token->is_uint;
        long long max = bits == 64 ? ~0ULL >> 1 : (1ULL << bits) - 1;
        long long min = expected->token->is_uint ? 0 : -max - 1;
        long long number = token->num.value;
        if (number < min || number > max)
            add_error(
                token->s,
                token->e,
                format("%lld doesn't fit in %s", number, type_name(expected)),
                format("%s goes from %lld to %lld", type_name(expected), min, max)
            );
        token->shift = expected->token->shift;
        token->is_uint = expected->token->is_uint;
        return;
    }
    bool is_real = value->token->type == FLOAT && !value->token->is_type;
    if (is_real && expected->token->type == FLOAT) {
        token->shift = expected->token->shift;
        return;
    }
    if (!same_type(found, expected))
        add_error(
            token->s,
            token->e,
            format("expected '%s', found '%s'", type_name(expected), type_name(found)),
            help
        );
}

void check_struct_value(Node *value, Node *expected) {
    Token *token = value->token;
    if (expected->token->type != ID) {
        add_error(
            token->s,
            token->e,
            format("expected '%s', found a struct value", type_name(expected)),
            NULL
        );
        return;
    }
    if (expected->declared == NULL)
        return;
    value->data_type = expected;
    Node *st = expected->declared;
    for (int i = 0; i < value->children_count; i++) {
        Node *attr = value->children[i];
        Token *name = attr->token;
        attr->declared = find_attr(st, name->name, &name->attr.index);
        if (attr->declared == NULL) {
            add_error(
                name->s,
                name->e,
                format("'%s' has no attribute '%s'", st->token->name, name->name),
                NULL
            );
            continue;
        }
        check_type(attr->left, attr->declared->data_type, NULL);
        check_copy(&attr->left, attr->declared->data_type);
    }
}

void check_copy(Node **value, Node *type) {
    Node *found = type_of(*value);
    bool is_struct = type && type->token->type == ID && type->declared;
    bool is_same = is_struct && found && same_type(found, type);
    if (!lives_in_variable(*value) || !is_same)
        return;
    Node *assign = find_method(type->declared, "=", value, 1, false);
    if (assign) {
        Node *copy = new_node(new_token(COPY, (*value)->token));
        copy->to_call = assign;
        copy->to_copy = *value;
        *value = copy;
        return;
    }
    if (!needs_drop(type))
        return;
    Token *token = (*value)->token;
    char *name = type_name(type);
    add_error(
        token->s,
        token->e,
        format("'%s' can't be copied: it needs a drop and has no operator =", name),
        format("define operator =(other &%s), or take it as &%s", name, name)
    );
}

void check_args(Node *fn, Node **args, int count, int self) {
    for (int i = 0; i < count && i + self < fn->args->children_count; i++) {
        Node *type = fn->args->children[i + self]->data_type;
        if (type == NULL || args[i]->token->type == REF)
            continue;
        if (type->token->type != REF) {
            check_copy(&args[i], type);
            continue;
        }
        Node *ref = new_node(new_token(REF, args[i]->token));
        ref->operand = args[i];
        args[i] = ref;
    }
}

void analyze_operator(Node *node, Node *st, char *symbol) {
    Token *op = node->token;
    Node *method = find_method(st, symbol, &node->right, 1, false);
    if (method == NULL) {
        Node *right = type_of(node->right);
        add_error(
            op->s,
            op->e,
            format(
                "'%s' has no operator '%s' that takes '%s'",
                st->token->name,
                symbol,
                right ? type_name(right) : "?"
            ),
            NULL
        );
        return;
    }
    check_args(method, &node->right, 1, 1);
    Node *call = new_node(new_token(FN_CALL, op));
    call->token->name = symbol;
    call->to_call = method;
    append(call->children, node->right);
    op->type = DOT;
    node->right = call;
}

void check_name(Node *node) {
    Token *token = node->token;
    if (token->is_type) {
        node->declared = find_struct(token->name);
        if (node->declared == NULL)
            add_error(
                token->s,
                token->e,
                format("unknown type '%s'", token->name),
                format("declare it: struct %s:", token->name)
            );
        return;
    }
    Node *var = find_variable(token->name);
    if (var == NULL) {
        add_error(
            token->s,
            token->e,
            format("undeclared variable '%s'", token->name),
            format("declare it first, like: %s i32 = 0", token->name)
        );
        return;
    }
    token->type = var->token->type;
    node->data_type = var->data_type;
    node->declared = var;
}

void check_declare(Node *node) {
    Token *token = node->var->token;
    if (find_in_scope(ura.scope, token->name) != node->var)
        add_error(
            token->s, token->e, format("'%s' is already declared", token->name), NULL
        );
}

void check_return(Node *node) {
    Node *fn = find_scope(FN_DEC);
    if (fn == NULL)
        return;
    check_type(node->left, ret_type_of(fn), NULL);
    if (!is_local_variable(node->left))
        check_copy(&node->left, ret_type_of(fn));
}

void check_cond(Node *cond) {
    char *help = NULL;
    if (includes(cond->token->type, VAR, PARAM, 0))
        help = format("write '%s != 0'", cond->token->name);
    check_type(cond, bool_type(), help);
}

void check_static_call(Node *node) {
    Token *name = node->left->token;
    Node *st = find_struct(name->name);
    if (st == NULL) {
        add_error(name->s, name->e, format("unknown type '%s'", name->name), NULL);
        return;
    }
    Node *call = node->right;
    for (int i = 0; i < call->children_count; i++)
        analyze(call->children[i]);
    call->to_call =
        find_method(st, call->token->name, call->children, call->children_count, true);
    if (call->to_call == NULL) {
        add_error(
            call->token->s,
            call->token->e,
            format("'%s' has no pub fn '%s'", st->token->name, call->token->name),
            NULL
        );
        return;
    }
    check_args(call->to_call, call->children, call->children_count, 0);
}

void check_link(Node *node) {
    Token *token = node->token;
    if (token->name == NULL)
        return;
    token->chars.value = ucg_find_lib(token->name);
    if (token->chars.value == NULL)
        add_error(
            token->s,
            token->e,
            format("library '%s' not found", token->name),
            "install it, or set URA_LIB_PATH to the folder that holds it"
        );
}

void check_access(Node *node) {
    Node *type = type_of(node->left);
    bool is_struct = type && type->token->type == ID && type->declared;
    if (is_struct) {
        analyze_operator(node, type->declared, "[]");
        return;
    }
    check_type(node->index, new_type(INT, 32, NULL), NULL);
    if (type == NULL || type->token->type == ARRAY)
        return;
    Token *base = node->left->token;
    add_error(
        base->s,
        base->e,
        format("'%s' is '%s', not an array", token_text(base), type_name(type)),
        NULL
    );
}

void check_member(Node *node) {
    Node *type = type_of(node->left);
    if (type == NULL)
        return;
    Token *base = node->left->token;
    if (type->token->type != ID) {
        add_error(
            base->s,
            base->e,
            format("'%s' is '%s', not a struct", token_text(base), type_name(type)),
            NULL
        );
        return;
    }
    if (type->declared == NULL)
        return;
    Node *st = type->declared;
    if (node->right->token->type == FN_CALL) {
        Node *call = node->right;
        for (int i = 0; i < call->children_count; i++)
            analyze(call->children[i]);
        call->to_call = find_method(
            st, call->token->name, call->children, call->children_count, false
        );
        if (call->to_call == NULL) {
            add_error(
                call->token->s,
                call->token->e,
                format("'%s' has no method '%s'", st->token->name, call->token->name),
                NULL
            );
            return;
        }
        check_args(call->to_call, call->children, call->children_count, 1);
        return;
    }
    Token *attr = node->right->token;
    node->right->declared = find_attr(st, attr->name, &attr->attr.index);
    if (node->right->declared == NULL)
        add_error(
            attr->s,
            attr->e,
            format("'%s' has no attribute '%s'", st->token->name, attr->name),
            NULL
        );
}

void check_call(Node *node) {
    Token *token = node->token;
    char *name = token->name;
    node->to_call = find_function(name);
    if (node->to_call == NULL) {
        Node *like = find_function_like(name, 2);
        if (find_variable(name))
            add_error(
                token->s,
                token->e,
                format("'%s' is a variable, not a function", name),
                NULL
            );
        else
            add_error(
                token->s,
                token->e,
                format("no function named '%s'", name),
                like ? format("did you mean '%s'?", like->token->name) : NULL
            );
        return;
    }
    bool takes_type = includes(node->to_call->token->type, SIZEOF, TYPEOF, DROP, 0);
    if (takes_type && node->children_count != 1)
        add_error(
            token->s,
            token->e,
            format("'%s' takes one argument, got %d", name, node->children_count),
            format("use %s(x)", name)
        );
    Node *params = node->to_call->args;
    for (int i = 0; i < node->children_count; i++) {
        analyze(node->children[i]);
        if (params == NULL || i >= params->children_count)
            continue;
        Node *param = params->children[i];
        if (param->token->type == VARIADIC)
            continue;
        check_type(node->children[i], param->data_type, NULL);
        check_copy(&node->children[i], param->data_type);
    }
    bool is_drop = node->to_call->token->type == DROP;
    bool has_one_arg = node->children_count == 1;
    if (is_drop && has_one_arg && !lives_in_variable(node->children[0])) {
        Token *arg = node->children[0]->token;
        add_error(arg->s, arg->e, "drop takes a variable, an attribute or an element", NULL);
    }
}

void check_operation(Node *node) {
    Node *left_type = type_of(node->left);
    bool is_struct = left_type && left_type->token->type == ID && left_type->declared;
    if (is_struct) {
        Node *st = left_type->declared;
        Node *right_type = type_of(node->right);
        bool is_conversion = right_type && !same_type(right_type, left_type);
        bool has_assign = find_method(st, "=", &node->right, 1, false) != NULL;
        if (node->token->type != ASSIGN || (is_conversion && has_assign)) {
            analyze_operator(node, st, token_text(node->token));
            return;
        }
        check_copy(&node->right, left_type);
    }
    bool is_logic = includes(node->token->type, AND, OR, 0);
    Node *left_bool = type_of(node->left);
    bool is_left_bool = !left_bool || same_type(left_bool, bool_type());
    if (is_ref_binding(node))
        check_type(node->right, node->left->data_type, NULL);
    else if (!is_logic && is_number(node->left))
        check_type(node->left, type_of(node->right), NULL);
    else if (!is_logic)
        check_type(node->right, type_of(node->left), NULL);
    else if (is_left_bool)
        check_type(node->right, bool_type(), NULL);
    else
        check_type(node->left, bool_type(), NULL);
}
