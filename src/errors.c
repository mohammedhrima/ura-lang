#include "header.h"

void diag_flush(void) {
    if (ura.error_file) {
        char *file = ura.error_file;
        int line = ura.error_line;
        const char *func = ura.error_func;
        fprintf(stderr, DIM("raised at %s:%d %s") "\n", file, line, func);
    }
    ura.error_file = NULL;
}

void _error_at(char *file, const char *func, int line, Token *token, char *message) {
    diag_flush();
    ura.errors_count++;
    ura.error_file = file;
    ura.error_func = func;
    ura.error_line = line;

    char *content = ura.curr_content;
    size_t s = token->s;
    while (s > 0 && content[s - 1] != '\n')
        s--;
    size_t e = token->s;
    while (content[e] && content[e] != '\n')
        e++;
    size_t col = token->s - s + 1;
    // TODO: chekc this one
    char *name = ura.curr_file->name; 

    size_t row = token->line;
    fprintf(stderr, RED("error:") " %s:%zu:%zu %s\n", name, row, col, message);

    int width = snprintf(NULL, 0, "%zu", row);
    fprintf(stderr, "%*s |\n", width, "");
    fprintf(stderr, "%zu | %.*s\n", token->line, (int)(e - s), content + s);
    fprintf(stderr, "%*s | %*s", width, "", (int)(token->s - s), "");
    for (size_t i = token->s; i < token->e || i == token->s; i++)
        fputc('^', stderr);
    fprintf(stderr, "\n");
}

void _help(char *message) {
    fprintf(stderr, CYAN("help:") " %s\n", message);
}

bool assert_literal_is_valid(Token *literal, bool closed) {
    char *content = ura.curr_content;
    bool is_char = content[literal->s] == '\'';
    size_t len = literal->e - literal->s - 2;
    bool escaped = content[literal->s + 1] == '\\';
    if (!closed && !is_char)
        error_at(literal, "unterminated string");
    else if (!closed)
        error_at(literal, "unterminated character");
    else if (is_char && len == 0)
        error_at(literal, "empty character literal");
    else if (is_char && len != (escaped ? 2 : 1)) {
        char *text = ura_alloc(len + 1, sizeof(char));
        strncpy(text, content + literal->s + 1, len);
        error_at(literal, "a character holds one letter");
        help("for a string, write '\"%s\"'", text);
    } else
        return true;
    return false;
}

bool assert_number_fits(Token *number, Type type, bool report) {
    // clang-format off
    struct { char *name; long min; long max; } range[END + 1] = {
        [I8]  = { "i8",  -128,    127     },
        [I32] = { "i32", INT_MIN, INT_MAX },
    };
    // clang-format on
    long value = number->i32.value;
    if (value >= range[type].min && value <= range[type].max)
        return true;
    if (report)
        error_at(number, "'%K' doesn't fit in '%s'", number, range[type].name);
    return false;
}

Node *recover(Token *start) {
    while (peek(0)->type != END) {
        Token *token = peek(0);
        if (token->line > start->line && token->space <= start->space)
            break;
        next();
    }
    return new_node(new_token(ERR, start));
}

Token *assert_next_is(Type type, char *message) {
    Token *token = peek(0);
    if (token->type == type)
        return next();
    Token *prev = ura.tokens[ura.exe_pos - 1];
    error_at(token->line == prev->line ? token : prev, message);
    return NULL;
}

bool assert_character_is_known(Token *token) {
    error_at(token, "unexpected character '%c'", ura.curr_content[token->s]);
    return false;
}

bool assert_is_struct_or_func(Token *token) {
    if (includes(token->type, FN_DEC, PROTO, STRUCT_DEC, 0))
        return true;
    error_at(token, "expected struct/fn after template");
    return false;
}

bool assert_templates_param(Token *token) {
    if (token->type == ID)
        return true;
    error_at(token, "expected names after template");
    help("use template<T, V>");
    return false;
}

bool assert_template_args_match(Node *template, Token *name, Node *args) {
    size_t want = template->left->children_count;
    size_t got = args->children_count;
    if (want == got)
        return true;
    char *plural = want == 1 ? "" : "s";
    char *type = name->name;
    error_at(name, "'%s' takes %zu type argument%s, got %zu", type, want, plural, got);

    char *params = ura_strdup("");
    for (size_t i = 0; i < want; i++) {
        if (i > 0)
            params = strjoin(params, ", ", NULL);
        params = strjoin(params, template->left->children[i]->token->name, NULL);
    }
    help("it is declared 'template<%s>'", params);
    return false;
}

bool assert_token_is_expected(Token *token) {
    if (includes(token->type, ELIF, ELSE, 0))
        error_at(token, "'%K' without a matching 'if'", token);
    else
        error_at(token, "unexpected '%K'", token);
    return false;
}

bool assert_ref_is_single(void) {
    Token *first = peek(0);
    Token *second = peek(1);
    bool double_amp = first->type == AND && is_data_type(second);
    bool two_refs = first->type == REF && second->type == REF;
    if (!double_amp && !two_refs)
        return true;
    error_at(double_amp ? first : second, "a ref can't point to a ref");
    return false;
}

bool assert_type_follows(Node *type, Token *after) {
    if (type) return true;
    error_at(after, "expected a type after '%K'", after);
    return false;
}

bool assert_type_is_known(Node *type) {
    Token *name = peek(0);
    Token *prev = ura.tokens[ura.exe_pos - 1];
    if (type || name->type != ID || name->line != prev->line)
        return true;
    error_at(name, "unknown type '%s'", name->name);
    return false;
}

bool assert_struct_not_inside_itself(Node *type) {
    if (type != ura.scope)
        return true;
    char *name = type->token->name;
    error_at(peek(0), "struct '%s' can't contain itself", name);
    help("use a ref: '&%s'", name);
    return false;
}

bool assert_array_type_is_closed(Token *open, Node *type) {
    Token *close = peek(0);
    if (close->type == RBRACK) {
        next();
        return true;
    }
    error_at(close->line == open->line ? close : open, "expected ']' after '['");
    if (close->type == I32)
        help("arrays have no size in the type: '%N[]'", type);
    return false;
}

bool assert_arguments_are_separated(Token *call) {
    Token *after = peek(0);
    if (after->type == COMA && peek(1)->type == RPARENT) {
        error_at(peek(1), "expected an argument after ','");
        next();
        return true;
    }
    if (after->type == COMA)
        next();
    if (includes(after->type, COMA, RPARENT, 0))
        return true;
    if (after->line == call->line)
        error_at(after, "expected ',' between arguments");
    else
        error_at(call, "expected ')' to close the call to '%s'", call->name);
    return false;
}

bool assert_access_is_closed(Node *access) {
    if (peek(0)->type == RBRACK) {
        next();
        return true;
    }
    error_at(access->token, "unclosed '['");
    return false;
}

bool assert_array_literal_valid(Node *literal) {
    Token *open = literal->token;
    Token *curr = peek(0);
    bool ends = curr->line > open->line && curr->space <= open->space;
    if (ends || curr->type == END) {
        error_at(open, "unclosed '['");
        return false;
    }
    if (literal->children_count && curr->type != COMA) {
        error_at(curr, "expected ',' between elements");
        return false;
    }
    return true;
}

bool assert_is_struct_member(Node *attr) {
    Token *at = attr->token;
    if (includes(at->type, VAR_DEC, FN_DEC, 0))
        return true;
    if (at->type == ID)
        error_at(at, "attribute '%s' needs a type", at->name);
    else
        error_at(at, "a struct holds only attributes and methods");
    return false;
}

bool assert_address_is_single(Token *amp) {
    if (amp->type == REF && peek(0)->type != REF)
        return true;
    error_at(amp, "can't take the address of an address");
    return false;
}

bool assert_parenthesis_is_closed(Token *open) {
    if (peek(0)->type == RPARENT) {
        next();
        return true;
    }
    error_at(open, "unclosed '('");
    return false;
}

bool assert_variadic_is_last(Node *arg) {
    if (peek(0)->type == RPARENT)
        return true;
    error_at(arg->token, "'...' must be the last parameter");
    return false;
}

bool assert_parameter_has_type(Node *arg) {
    if (arg->token->type != ID)
        return true;
    error_at(arg->token, "parameter '%s' needs a type", arg->token->name);
    return false;
}

bool assert_function_parameters_are_separated(void) {
    Token *after = peek(0);
    if (after->type == COMA)
        next();
    if (includes(after->type, COMA, RPARENT, 0))
        return true;
    if (after->type == ID)
        error_at(after, "expected ',' between parameters");
    else
        error_at(after, "expected ')' to close the parameters");
    return false;
}

bool assert_template_parameters_are_separated(void) {
    Token *after = peek(0);
    if (after->type == COMA)
        next();
    if (includes(after->type, COMA, GT, 0))
        return true;
    if (after->type == ID)
        error_at(after, "expected ',' between parameters");
    else
        error_at(after, "expected '>' to close the parameters");
    return false;
}


bool assert_condition_follows(Token *keyword) {
    if (peek(0)->type != DOTS)
        return true;
    error_at(keyword, "expected a condition after '%K'", keyword);
    return false;
}

bool assert_else_has_no_condition(Token *keyword) {
    Token *after = peek(0);
    if (after->type == DOTS || after->line != keyword->line)
        return true;
    error_at(after, "'else' takes no condition");
    help("use 'elif'");
    return false;
}

bool assert_operand_follows(Token *op) {
    Token *after = peek(0);
    if (after->line == op->line && after->type != END)
        return true;
    error_at(op, "expected an expression after '%K'", op);
    return false;
}

bool assert_type_is_printable(Node *value, char *spec) {
    if (spec)
        return true;
    error_at(value->token, "can't output '%N'", type_of(value));
    return false;
}

bool assert_call_is_unambiguous(Node *call, Node *found) {
    if (!found)
        return true;
    error_at(call->token, "ambiguous call to '%s'", call->token->name);
    return false;
}

// TODO: to be reviewed
bool assert_type_fits(Node *expected, Node *value, bool report) {
    Token *token = value ? value->token : NULL;
    Type kind = token ? token->type : VOID;
    Type want = expected ? expected->token->type : VOID;
    if (kind == ERR || want == ERR)
        return true;

    if (kind == NULL_ && want == REF) {
        if (report)
            value->left = expected;
        return true;
    }

    if (kind == I32 && !token->is_type && want == I8) {
        if (!assert_number_fits(token, I8, report))
            return false;
        if (report) {
            long number = token->i32.value;
            token->type = I8;
            token->i8.value = number;
        }
        return true;
    }

    if (kind == ARRAY_LIT && want == ARRAY) {
        bool fits = true;
        for (size_t i = 0; i < value->children_count; i++) {
            Node *element = value->children[i];
            fits = assert_type_fits(expected->left, element, report) && fits;
        }
        if (report && fits)
            value->left = expected;
        return fits;
    }

    Node *actual = value ? type_of(value) : NULL;
    bool fits = expected == actual;
    if (expected && actual && want == actual->token->type) {
        if (want == STRUCT_DEC)
            fits = strcmp(expected->token->name, actual->token->name) == 0;
        else if (includes(want, REF, ARRAY, 0))
            fits = assert_type_fits(expected->left, actual->left, false);
        else
            fits = true;
    }
    if (report && !fits)
        error_at(token, "expected '%N', found '%N'", expected, actual);
    return fits;
}

void assert_condition_is_bool(Node *cond) {
    static Node *b1;
    if (!b1) {
        b1 = new_node(new_token(BOOL, NULL));
        b1->token->is_type = true;
    }
    if (assert_type_fits(b1, cond, true))
        return;
    Node *type = type_of(cond);
    bool number = type && includes(type->token->type, I8, I32, 0);
    bool literal = includes(cond->token->type, I8, I32, 0);
    if (number && !literal) // 'while a != 0' instead of 'while a'
        help("write '%K != 0'", cond->token);
}

bool assert_operands_are_valid(Node *node) {
    bool left = !node->left || node->left->token->type != ERR;
    bool right = !node->right || node->right->token->type != ERR;
    if (left && right)
        return true;
    node->token->type = ERR;
    return false;
}
// TODO: to be cheked
bool assert_not_declared_twice(Node *node) {
    Token *var = node->left->token;
    Node *scope = ura.scope;
    Node *args = NULL;
    if (includes(scope->token->type, FN_DEC, PROTO, 0))
        args = scope->left;
    size_t body = scope->children_count;
    size_t params = 0;
    if (args)
        params = args->children_count;

    Node *twin = NULL;
    for (size_t i = 0; i < body + params && !twin; i++) {
        Node *dec = NULL;
        if (i < body)
            dec = scope->children[i];
        else
            dec = args->children[i - body];
        if (dec->token->type != VAR_DEC)
            continue;
        if (strcmp(dec->left->token->name, var->name) == 0)
            twin = dec->left;
    }
    if (!twin || twin == node->left)
        return true;
    error_at(var, "'%s' is already declared", var->name);
    return false;
}
// TODO: to be cheked
bool assert_name_is_declared(Node *node) {
    if (node->left)
        return true;
    char *name = node->token->name;
    if (find_type_by_name(name))
        error_at(node->token, "'%s' is a type, not a value", name);
    else {
        error_at(node->token, "'%s' not found", name);
        Node *self = find_variable("self");
        Node *struct_dec = NULL;
        if (self)
            struct_dec = self->left->left;
        size_t count = 0;
        if (struct_dec)
            count = struct_dec->children_count;
        for (size_t i = 0; i < count; i++) {
            Node *attr = struct_dec->children[i];
            if (attr->token->type != VAR_DEC)
                continue;
            if (strcmp(attr->left->token->name, name) == 0) {
                help("did you mean 'self.%s'?", name);
                break;
            }
        }
    }
    node->token->type = ERR;
    return false;
}

bool assert_indexing_an_array(Node *node) {
    if (!assert_operands_are_valid(node))
        return false;
    Node *type = type_of(node->left);
    if (!type || type->token->type != ARRAY) {
        Token *left = node->left->token;
        error_at(left, "'%K' is '%N', not an array", left, type);
        node->token->type = ERR;
        return false;
    }
    Node *index = type_of(node->right);
    if (!index || !includes(index->token->type, I8, I32, 0)) {
        error_at(node->right->token, "expected 'i32', found '%N'", index);
        node->token->type = ERR;
        return false;
    }
    return true;
}

bool assert_receiver_is_struct(Node *node) {
    Node *type = type_of(node->left);
    if (type && type->token->type == STRUCT_DEC)
        return true;
    Token *left = node->left->token;
    error_at(left, "'%K' is '%N', not a struct", left, type);
    node->token->type = ERR;
    return false;
}

bool assert_attribute_exists(Node *node, Node *struct_dec) {
    Token *attr = node->right->token;
    if (attr->type == ATTR)
        return true;
    char *type = struct_dec->token->name;
    error_at(attr, "struct '%s' has no attribute '%K'", type, attr);
    node->token->type = ERR;
    return false;
}

bool assert_address_of_variable(Node *node) {
    if (includes(node->left->token->type, VAR_LOAD, DOT, 0))
        return true;
    error_at(node->left->token, "'&' expects a variable or a struct attribute");
    node->token->type = ERR;
    return false;
}

bool assert_assignment_is_valid(Node *node, bool bare_name) {
    Node *left = node->left;
    Node *right = node->right;
    bool left_failed = left->token->type == ERR;
    if (bare_name && left_failed && right->token->type != ERR) {
        char *name = left->token->name;
        help("declare it: '%s %N = %K'", name, type_of(right), right->token);
    }
    if (!assert_operands_are_valid(node))
        return false;
    if (!includes(left->token->type, VAR, VAR_LOAD, DREF, DOT, ACCESS, 0)) {
        error_at(left->token, "can't assign to a value");
        node->token->type = ERR;
        return false;
    }
    if (left->token->type == VAR && left->left->token->type == REF) {
        Node *type = type_of(right);
        if (!type || !includes(type->token->type, REF, NULL_, 0)) {
            Token *value = right->token;
            Token *var = left->token;
            Token *pointer = left->left->left->token;
            char *name = var->name;
            error_at(value, "'%s' is a ref, initialize it with an address", name);
            help("write '%K &%K = &%K'", var, pointer, value);
            return false;
        }
    }
    if (assert_type_fits(type_of(left), right, true))
        return true;
    node->token->type = ERR;
    return false;
}

// TODO: to be reviewed
bool assert_operator_fits_operands(Node *node) {
    if (!assert_operands_are_valid(node))
        return false;
    Node *left = node->left;
    Node *right = node->right;
    Token *op = node->token;
    if (right->token->type == NULL_ && left->token->type != NULL_)
        right->left = type_of(left);
    if (left->token->type == NULL_ && right->token->type != NULL_)
        left->left = type_of(right);
    Node *null = left->token->type == NULL_ ? left : right;
    Node *other = null == left ? right : left;
    Node *type = null->left;
    bool is_ref = type && type->token->type == REF;
    if (null->token->type == NULL_ && !is_ref && other->token->type != NULL_) {
        error_at(null->token, "'null' needs a ref on the other side");
        node->token->type = ERR;
        return false;
    }
    if (includes(op->type, AND, OR, 0)) {
        Node *b1 = type_of(node);
        bool fits = assert_type_fits(b1, left, true);
        if (fits && assert_type_fits(b1, right, true))
            return true;
        node->token->type = ERR;
        return false;
    }
    Node *ltype = type_of(left);
    Node *rtype = type_of(right);
    bool left_number = ltype && includes(ltype->token->type, I8, I32, 0);
    bool right_number = rtype && includes(rtype->token->type, I8, I32, 0);
    bool unary = left->token->s == op->s;
    if (!includes(op->type, EQ, NQ, 0) && (!left_number || !right_number)) {
        if (unary)
            error_at(op, "can't apply '%K' to '%N'", op, rtype);
        else
            error_at(op, "can't apply '%K' to '%N' and '%N'", op, ltype, rtype);
        node->token->type = ERR;
        return false;
    }
    if (ltype && ltype->token->type == STRUCT_DEC) {
        error_at(op, "can't compare '%N' values", ltype);
        node->token->type = ERR;
        return false;
    }
    bool literal = left->token->type == I32 && !left->token->is_type;
    Node *expected = literal ? rtype : ltype;
    Node *value = literal ? left : right;
    if (assert_type_fits(expected, value, true))
        return true;
    node->token->type = ERR;
    return false;
}

bool assert_return_matches_function(Node *node, Node *fn) {
    Node *type = fn->right;
    if (!node->left && type) {
        char *name = fn->token->name;
        error_at(node->token, "'%s' must return a value of type '%N'", name, type);
        return false;
    }
    if (!node->left || assert_type_fits(type, node->left, true))
        return true;
    node->token->type = ERR;
    return false;
}

bool assert_inside_loop(Node *node) {
    if (node->left)
        return true;
    error_at(node->token, "'%K' outside of a loop", node->token);
    node->token->type = ERR;
    return false;
}

bool assert_declaration_is_valid(Node *parent, size_t i) {
    Node *node = parent->children[i];
    size_t count = node->children_count;

    Token *token = node->token;
    char *name = token->name;
    switch (token->type) {
    case STRUCT_DEC: {
        for (size_t j = 0; j < i; j++) {
            Node *other = parent->children[j];
            if (other->token->type != STRUCT_DEC)
                continue;
            if (strcmp(name, other->token->name) != 0)
                continue;
            error_at(token, "struct '%s' is already defined", name);
            return false;
        }
        bool has_attr = false;
        for (size_t j = 0; j < count; j++)
            has_attr = has_attr || node->children[j]->token->type == VAR_DEC;
        if (!has_attr)
            error_at(token, "struct '%s' has no attributes", name);
        break;
    }
    case FN_DEC: {
        for (size_t j = 0; j < i; j++) {
            Node *other = parent->children[j];
            if (other->token->type != FN_DEC)
                continue;
            if (strcmp(name, other->token->name) != 0)
                continue;
            if (!same_params(node, other))
                continue;
            if (assert_type_fits(node->right, other->right, false))
                error_at(token, "'%s' is already defined with these parameters", name);
            else
                error_at(token, "'%s' can't be overloaded on its return type", name);
            return false;
        }
        break;
    }
    case RETURN: {
        error_at(token, "'return' outside of a function");
        return false;
    }
    case PROTO: {
        for (size_t j = 0; j < parent->children_count; j++) {
            Node *other = parent->children[j];
            if (other == node) // never match yourself
                continue;
            if (other->token->type != FN_DEC)
                continue;
            if (!node->token->name || !other->token->name)
                continue;
            if (strcmp(node->token->name, other->token->name) != 0)
                continue;

            if (!same_params(node, other))
                continue;
            if (!assert_type_fits(node->right, other->right, false)) {
                Token *fn = node->token;
                error_at(fn, "'%s' can't be overloaded on its return type", fn->name);
                return false;
            }
            return false; // keep fn, drop proto
        }
        break;
    }
    default:
        break;
    }
    return true;
}
// TODO: to be cheked
bool assert_builtin_takes_one(Node *call) {
    size_t got = call->left->children_count;
    if (got == 1)
        return true;
    char *name = call->token->name;
    error_at(call->token, "'%s' takes 1 argument, got %zu", name, got);
    call->token->type = ERR;
    return false;
}

bool assert_function_matches_call(Node *call) {
    if (call->right)
        return true;
    call->token->type = ERR;
    char *name = call->token->name;
    Node *struct_dec = ura.scope->token->type == STRUCT_DEC ? ura.scope : NULL;
    Node *fn = NULL;
    size_t count = 0;
    for (size_t i = ura.scopes_count; i > 0; i--) {
        Node *scope = ura.scopes[i - 1];
        if (scope->token->type == STRUCT_DEC && scope != struct_dec)
            continue;
        for (size_t j = 0; j < scope->children_count; j++) {
            Node *curr = scope->children[j];
            if (!includes(curr->token->type, FN_DEC, PROTO, 0))
                continue;
            if (strcmp(curr->token->name, name) != 0)
                continue;
            fn = curr;
            count++;
        }
        if (struct_dec)
            break;
    }
    if (count == 0 && struct_dec) {
        char *type = struct_dec->token->name;
        error_at(call->token, "struct '%s' has no method '%s'", type, name);
        return false;
    }
    if (count == 0 && find_variable(name)) {
        error_at(call->token, "'%s' is a variable, not a function", name);
        return false;
    }
    if (count == 0) {
        error_at(call->token, "function '%s' not found", name);
        Node *self = find_variable("self");
        Node *struct_dec = NULL;
        if (self)
            struct_dec = self->left->left;
        size_t methods = 0;
        if (struct_dec)
            methods = struct_dec->children_count;
        for (size_t i = 0; i < methods; i++) {
            Node *method = struct_dec->children[i];
            if (!includes(method->token->type, FN_DEC, PROTO, 0))
                continue;
            if (strcmp(method->token->name, name) == 0) {
                help("did you mean 'self.%s()'?", name);
                break;
            }
        }
        return false;
    }
    error_at(call->token, "no overload of '%s' takes these arguments", name);
    if (count > 1) {
        help("%zu functions are named '%s', none fits these arguments", count, name);
        return false;
    }

    size_t skip = struct_dec ? 1 : 0;
    size_t want = fn->left->children_count - skip;
    size_t got = call->left->children_count - skip;
    if (fn->token->is_variadic ? got < want : got != want) {
        char *plural = want == 1 ? "" : "s";
        help("'%s' takes %zu argument%s, got %zu", name, want, plural, got);
        return false;
    }
    for (size_t i = skip; i < fn->left->children_count; i++) {
        Node *var = fn->left->children[i]->left;
        Node *arg = call->left->children[i];
        Node *expected = var->left;
        Node *actual = type_of(arg);
        if (assert_type_fits(expected, arg, false))
            continue;
        Token *given = arg->token;
        char *param = var->token->name;
        help("'%K' is '%N', but '%s' wants '%N'", given, actual, param, expected);
        bool is_ref = expected->token->type == REF;
        if (is_ref && assert_type_fits(expected->left, arg, false))
            help("pass its address: '&%K'", given);
        return false;
    }
    return false;
}
