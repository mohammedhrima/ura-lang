#include "header.h"

/*
    TODOS:
        - handle not
*/

// DEV TOOLS
void error(int line, char *fmt, ...)
{
    // TODO: update error messages
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%sline %d: ", RED, line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "%s\n", RESET);
    exit(1);
};

char *type_to_string(int line, Type type)
{
    for (int i = 0; i < sizeof(DataTypes) / sizeof(*DataTypes); i++)
        if (DataTypes[i].type == type)
            return DataTypes[i].name;
    for (int i = 0; i < sizeof(Symbols) / sizeof(*Symbols); i++)
        if (Symbols[i].type == type)
            return Symbols[i].name;
    for (int i = 0; i < sizeof(Specials) / sizeof(*Specials); i++)
        if (Specials[i].type == type)
            return Specials[i].name;
    error(line, "error unkown type (%d | %c)\n", type, type);
    return NULL;
}

void debug(char *conv, ...)
{
#if DEBUG
    size_t len = strlen(conv);
    size_t i = 0;

    va_list args;
    va_start(args, conv);
    while (i < len)
    {
        if (conv[i] == '%')
        {
            i++;
            if (strncmp(conv + i, "zu", 2) == 0)
            {
                fprintf(stdout, "%zu", va_arg(args, size_t));
                i++;
            }
            else if (strncmp(conv + i, "ld", 2) == 0)
            {
                fprintf(stdout, "%ld", va_arg(args, long long));
                i++;
            }
            else
            {
                switch (conv[i])
                {
                case 'c':
                    fprintf(stdout, "%c", va_arg(args, int));
                    break;
                case 's':
                    fprintf(stdout, "%s", va_arg(args, char *));
                    break;
                case 'p':
                    fprintf(stdout, "%p", (size_t)(va_arg(args, void *)));
                    break;
                case 'x':
                    fprintf(stdout, "%x", (size_t)va_arg(args, void *));
                    break;
                case 'X':
                    fprintf(stdout, "%X", (size_t)va_arg(args, void *));
                    break;
                case 'd':
                    fprintf(stdout, "%d", (int)va_arg(args, int));
                    break;
                case 'f':
                    fprintf(stdout, "%f", va_arg(args, double));
                    break;
                case '%':
                    fprintf(stdout, "%%");
                    break;
                case 't':
                    fprintf(stdout, "%s", type_to_string(__LINE__, (Type)va_arg(args, Type)));
                    break;
                case 'k':
                {
                    Token *token = (Token *)va_arg(args, Token *);
                    if (token)
                    {
                        fprintf(stdout, "%s ", type_to_string(__LINE__, token->type));
                        if (token->name)
                            fprintf(stdout, "<name:%5s> ", token->name);
                        switch (token->type)
                        {
                        case char_:
                            fprintf(stdout, "'%s' ", token->char_);
                            if (token->index_)
                                fprintf(stdout, "STR%zu, ", token->index_);
                            break;
                        case int_:
                            fprintf(stdout, "%d ", token->int_);
                            break;
                        case float_:
                            fprintf(stdout, "%zu (%.2f) ", token->float_,
                                    *(float *)(&token->float_));
                            if (token->index_)
                                fprintf(stdout, "FLT%zu ", token->index_);
                            break;
                        case bool_:
                            fprintf(stdout, "%s ", token->bool_ ? "True" : "False");
                            break;
                        case array_:
                            fprintf(stdout, "depth: %zu, ", token->depth);
                            if (token->child_type)
                                fprintf(stdout, "child: %s, ", type_to_string(__LINE__, token->child_type));
                            break;
                        default:
                            break;
                        }
                        fprintf(stdout, "in COL%zu", token->col);
                    }
                    else
                        fprintf(stdout, "(null)");
                    break;
                }
                default:
                    error(__LINE__, "in debug function (%c)", conv[i]);
                    break;
                }
            }
        }
        else
            fprintf(stdout, "%c", conv[i]);
        i++;
    }
#endif
}

void visualize()
{
    debug("%sVISUALIZE%s\n", GREEN, RESET);
    Label *curr = LABEL;
    if (curr == NULL)
        error(__LINE__, "is null\n");
    debug("%sLabel: %s\n", GREEN, curr->name ? curr->name : "");
    debug("     variables:\n");
    for (int i = 0; i < curr->var_pos; i++)
        debug("         %k\n", curr->VARIABLES[i]);
    debug("     functions:\n");
    for (int i = 0; i < curr->func_pos; i++)
        debug("         %k\n", curr->FUNCTIONS[i]->token);
    debug("%s\n", RESET);
}

// UTILS
void print_asm(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(asm_fd, fmt, ap);
}

char *strjoin(char *string1, char *string2)
{
    size_t len = 0;
    len = string1 ? len + strlen(string1) : len;
    len = string2 ? len + strlen(string2) : len;

    char *res = calloc(len + 1, sizeof(char));
    string1 &&strcpy(res, string1);
    string2 &&strcpy(res + strlen(res), string2);
    return res;
}

// BUILTINS
void output(Token *token)
{
    debug("output %k\n", token);
    switch (token->type)
    {
    case char_:
    {
        print_asm("   /* call _putstr */\n");
        if (token->ptr)
        {
            print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   mov     rdi, rax\n");
            print_asm("   call    _putstr\n");
        }
        else if (token->index_)
        {
            print_asm("   lea     rax, STR%zu[rip]\n", token->index_);
            print_asm("   mov     rdi, rax\n");
            print_asm("   call    _putstr\n");
        }
        else
            error(__LINE__, "output char");
        break;
    }
    case int_:
    {
        print_asm("   /* call _putnbr */\n");
        if (token->ptr)
        {
            print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            print_asm("   mov     rdi, rax\n");
            print_asm("   call    _putnbr\n");
        }
        else if (token->int_)
        {
            print_asm("   mov   rax, %lld\n", token->int_);
            print_asm("   mov   rdi, rax\n");
            print_asm("   call  _putnbr\n");
        }
        else
            error(__LINE__, "output int");
        break;
    }
    case bool_:
    {
        print_asm("   /* call _putbool */\n");
        if (token->ptr)
            print_asm("   movzx   eax, BYTE PTR -%zu[rbp]\n", token->ptr);
        else if (token->c)
            print_asm("   movzx   eax, %cl\n", token->c);
        else
            error(__LINE__, "output bool");
        print_asm("   mov	   edi, eax\n");
        print_asm("   call	   _putbool\n");
        break;
    }
    case float_:
        // TODO: handle float
        error(__LINE__, "Error in output float not handled yet");
        break;
    case array_:
        // TODO: handle array somehow
        error(__LINE__, "Error in output array");
        break;
    case identifier_:
        break;
    default:
        error(__LINE__, "Error in output unknown type %s",
              type_to_string(__LINE__, token->type));
        break;
    }
}

void enter_label(Node *node)
{
    if (lb_len == 0)
    {
        lb_len = 20;
        LABELS = calloc(lb_len, sizeof(Label *));
    }
    else if (lb_pos + 2 > lb_len)
    {
        lb_len *= 2;
        LABELS = realloc(LABELS, lb_len * sizeof(Label *));
    }
    char *name = strdup(node->token->name);
    int curr = lb_pos;
    if (curr > 1)
    {
        char *tmp = strjoin(LABELS[curr - 1]->name, name);
        free(name);
        name = tmp;
    }
    Label *new = calloc(1, sizeof(Label));
    new->name = name;
    new->node = node;
    new->var_len = 100;
    new->func_len = 100;
    new->VARIABLES = calloc(new->var_len, sizeof(Token *));
    new->FUNCTIONS = calloc(new->func_len, sizeof(Node *));
    debug("%sENTER LABEL '%s' %s\n", GREEN, name, RESET);
    lb_pos++;
    LABELS[lb_pos] = new;
    LABEL = LABELS[lb_pos];
}

void exit_label(Node *node)
{
    debug("%sEXIT LABEL '%s' %s\n", GREEN, LABEL->name, RESET);
    if (LABEL->node != node)
        error(__LINE__, "in label exit\n");
    free(LABEL->VARIABLES);
    free(LABEL->FUNCTIONS);
    free(LABEL->name);
    free(LABEL);
    LABELS[lb_pos] = NULL;
    lb_pos--;
    LABEL = LABELS[lb_pos];
    debug("CURRENT LABEL '%s' \n", LABEL->name);
}

// HANDLE VARIABLES
Token *new_variable(Token *token)
{
    switch (token->type)
    {
    case char_:
        token->ptr = (ptr += 8);
        break;
    case int_:
        token->ptr = (ptr += 8);
        print_asm("   mov     QWORD PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
        break;
    case float_:
        token->ptr = (ptr += 4);
        print_asm("   mov     DWORD PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
        break;
    case bool_:
        token->ptr = (ptr += 1);
        print_asm("   mov     BYTE PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
        break;
    case array_:
        token->ptr = (ptr += 8);
        print_asm("   mov     QWORD PTR -%zu[rbp], %d /* declare %s */\n",
                  token->ptr, 0, token->name);
        break;
    default:
        break;
    }
    if (LABEL->var_pos + 2 > LABEL->var_len)
    {
        LABEL->var_len *= 2;
        LABEL->VARIABLES = realloc(LABEL->VARIABLES, LABEL->var_len * sizeof(Token *));
    }
    debug("new variable %k in ptr: %zu\n", token, token->ptr);
    return (LABEL->VARIABLES[LABEL->var_pos++] = token);
}

Token *get_var(char *name)
{
    debug("label pos: %d\n", lb_pos);
    debug("get_var %s from %s label\n", name, LABEL->name);
    for (int j = lb_pos; j > 0; j--)
    {
        Label *curr = LABELS[j];
        for (int i = 0; i < curr->var_pos; i++)
        {
            Token *var = curr->VARIABLES[i];
            if (var->name && strcmp(var->name, name) == 0)
                return var;
        }
    }
    return NULL;
}

// HANDLE FUNCTIONS
Node *new_func(Node *node)
{
    if (LABEL->func_pos + 2 > LABEL->func_len)
    {
        LABEL->func_len *= 2;
        LABEL->FUNCTIONS = realloc(LABEL->FUNCTIONS, LABEL->func_len * sizeof(Node *));
    }
    char *name = strjoin(LABEL->name, node->token->name);
    free(node->token->name);
    node->token->name = name;
    debug("new function, name: %s, return type: %t, in Label %s\n",
          node->token->name, node->token->type, LABEL->name[0] ? LABEL->name : "global");
    return (LABEL->FUNCTIONS[LABEL->func_pos++] = node);
}

Node *get_func(char *name)
{
    debug("label pos: %d\n", lb_pos);
    debug("get_func %s from %s label\n", name, LABEL->name[0] ? LABEL->name : "global");
    for (int j = lb_pos; j > 0; j--)
    {
        Label *curr = LABELS[j];
        char *tmp = strjoin(curr->name, name);
        debug("loop: label %s, has %d functions, get %s\n",
              curr->name[0] ? curr->name : "global", curr->func_pos, tmp);
        for (int i = 0; i < curr->func_pos; i++)
        {
            Node *func = curr->FUNCTIONS[i];
            if (strcmp(func->token->name, tmp) == 0)
            {
                free(tmp);
                return func;
            }
        }
        free(tmp);
    }
    return NULL;
}

// HANDLE TOKENS
Token *new_token(int s, int e, Type type, size_t col)
{
    if (tk_len == 0)
    {
        tk_len = 100;
        TOKENS = calloc(tk_len, sizeof(Token *));
    }
    else if (tk_pos + 2 > tk_len)
    {
        tk_len *= 2;
        TOKENS = realloc(TOKENS, tk_len * sizeof(Token *));
    }
    Token *token = calloc(1, sizeof(Token));
    token->type = type;
    token->col = col;
    switch (type)
    {
    case identifier_:
        token->name = calloc(e - s + 1, sizeof(char));
        strncpy(token->name, text + s, e - s);
        if (strcmp(token->name, "True") == 0)
        {
            token->type = bool_;
            token->bool_ = true;
            break;
        }
        else if (strcmp(token->name, "False") == 0)
        {
            token->type = bool_;
            token->bool_ = false;
            break;
        }
        for (int i = 0; DataTypes[i].name; i++)
            if (strcmp(DataTypes[i].name, token->name) == 0)
            {
                token->type = DataTypes[i].type;
                goto CLABEL0;
            }
        for (int i = 0; Specials[i].name; i++)
            if (strcmp(Specials[i].name, token->name) == 0)
            {
                token->type = Specials[i].type;
                goto CLABEL0;
            }
    CLABEL0:
        break;
    case char_:
        token->index_ = index_;
        index_++;
        token->char_ = calloc(e - s + 1, sizeof(char));
        strncpy(token->char_, text + s, e - s);
        break;
    case int_:
        while (s < e)
            token->int_ = 10 * token->int_ + text[s++] - '0';
        break;
    case float_:
        float f = 0.0;
        token->index_ = index_;
        index_++;
        while (s < e && isdigit(text[s]))
            f = 10 * f + text[s++] - '0';
        s++;
        while (s < e && isdigit(text[s]))
            f = f + (float)(text[s++] - '0') / 10;
        token->float_ = *(uint32_t *)(&f);
        break;
    default:
        break;
    }
    debug("token %k \n", token);
    return (TOKENS[tk_pos++] = token);
}

void build_tokens()
{
    int e = 0;
    size_t col = 0;
    while (text[e])
    {
        Token *token = NULL;
        int s = e;
        if (text[e] == '\n')
        {
            col = 0;
            while (text[e] == '\n')
                e++;
            s = e;
            while (text[e] != '\n' && isspace(text[e]))
                e++;
            col = e - s;
            debug("COLUMN %d\n", col);
            continue;
        }
        if (isspace(text[e]) && text[e] != '\n')
        {
            while (isspace(text[e]) && text[e] != '\n')
                e++;
            continue;
        }
        if (strlen(text + e) > 1 && strncmp(text + e, "/*", 2) == 0)
        {
            e += 2;
            while (text[e + 1] && strncmp(text + e, "*/", 2))
                e++;
            if (!text[e + 1])
                error(__LINE__, "Expected end of comment");
            e += 2;
            continue;
        }
        for (int i = 0; Symbols[i].name; i++)
        {
            if (strncmp(Symbols[i].name, text + e, strlen(Symbols[i].name)) == 0)
            {
                token = new_token(s, e + strlen(Symbols[i].name), Symbols[i].type, col);
                e += strlen(Symbols[i].name);
                break;
            }
        }
        if (token && token->type)
        {
            if (token->type == dots_)
            {
                while (isspace(text[e]) && text[e] != '\n')
                    e++;
                if (text[e] != '\n')
                    error(__LINE__, "Expected new line after dots");
            }
            token = NULL;
            continue;
        }
        if (isalpha(text[e]))
        {
            while (isalnum(text[e]))
                e++;
            if (e > s)
            {
                new_token(s, e, identifier_, col);
                continue;
            }
        }
        if (isdigit(text[e]))
        {
            Type type = int_;
            while (isdigit(text[e]))
                e++;
            if (text[e] == '.')
            {
                type = float_;
                e++;
            }
            while (isdigit(text[e]))
                e++;
            new_token(s, e, type, col);
            continue;
        }
        if (strchr("\"\'", text[e]))
        {
            char quote = text[e++];
            while (text[e] && text[e] != quote)
                e++;
            if (text[e++] != quote)
                error(__LINE__, "Syntax");
            new_token(s + 1, e - 1, char_, col);
            continue;
        }

        error(__LINE__, "tokenizing: %s", text + s);
    }
    new_token(0, 0, eof_, col);
}

// ABSTRACT TREE
void free_node(Node *node)
{
    if (node)
    {
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

void print_node(Node *node, int col)
{
    Node *tmp = NULL;
    int curr = col;
    if (node)
    {
        while (curr)
        {
            debug("   ");
            curr--;
        }
        switch (node->token->type)
        {
        case if_:
        case elif_:
        case else_:
        case while_:
        {
            debug("%s%t %s %s\n", GREEN, node->token->type,
                  node->token->name, RESET);

            tmp = node->left;
            if (node->token->type != else_) // condition
                print_node(tmp->left, col + 1);
            // code bloc
            tmp = node->left->right;
            while (tmp)
            {
                print_node(tmp->left, col + 1);
                tmp = tmp->right;
            }
            // elif / else nodes
            tmp = node;
            while (node->token->type == if_ && tmp->right)
            {
                print_node(tmp->right, col);
                tmp = tmp->right;
            }
            break;
        }
        case func_call_:
        case func_dec_:
        {
            debug("%s%t %s return (%t)%s\n", GREEN, node->token->type,
                  node->token->name, node->token->ret_type, RESET);
            // arguments
            tmp = node->left;
            while (tmp)
            {
                print_node(tmp->left, col + 1);
                tmp = tmp->right;
            }
            // code bloc
            tmp = node->right;
            while (tmp)
            {
                print_node(tmp->left, col + 1);
                tmp = tmp->right;
            }
            break;
        }
        case array_:
        {
            debug("%s%t %s depth: %d %s\n", GREEN,
                  node->token->type,
                  node->token->name ? node->token->name : "",
                  node->token->depth,
                  RESET);

            while (node)
            {
                print_node(node->left, col + 1);
                node = node->right;
            }
            break;
        }
        default:
        {
            debug("%k\n", node->token);
            print_node(node->left, col + 1);
            print_node(node->right, col + 1);
            break;
        }
        }
    }
}

Node *new_node(Token *token)
{
    Node *new = calloc(1, sizeof(Node));
    new->token = token;
    debug("new node %k\n", new->token);
    return new;
}

Node *copy_node(Node *node)
{
    Node *new = calloc(1, sizeof(Node));
    memcpy(new, node, sizeof(Node));
    if (node->left)
        new->left = copy_node(node->left);
    if (node->right)
        new->right = copy_node(node->right);
    return new;
}

Token *check(Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == TOKENS[exe_pos]->type)
            return TOKENS[exe_pos++];
        type = va_arg(ap, Type);
    }
    return NULL;
}

Token *expect(int line, Type type, ...)
{
    va_list ap;
    va_start(ap, type);
    while (type)
    {
        if (type == TOKENS[exe_pos]->type)
            return TOKENS[exe_pos++];
        type = va_arg(ap, Type);
    }
    error(line, "Unexpected %s\n", type_to_string(line, type));
    return NULL;
}

Node *expr()
{
    return assign();
}

Node *assign()
{
    Node *left = logic();
    Token *token;
    if (token = check(assign_, add_assign_, sub_assign_, mul_assign_, div_assign_, 0))
    {
        // TODO: see what you can do for left
        Node *right = assign();
        Node *tmp = right;
        if (token->type != assign_)
        {
            tmp = new_node(new_token(0, 0,
                                     token->type == add_assign_   ? add_
                                     : token->type == sub_assign_ ? sub_
                                     : token->type == mul_assign_ ? mul_
                                     : token->type == div_assign_ ? div_
                                                                  : 0,
                                     token->col));
            tmp->left = new_node(new_token(0, 0, 0, 0));
            memcpy(tmp->left->token, left->token, sizeof(Token));
            tmp->left->token->type = identifier_;
            tmp->right = right;
        }
        token->type = assign_;
        Node *node = new_node(token);
        node->left = left;
        node->right = tmp;
        return node;
    }
    return left;
}

Node *logic()
{
    Node *left = equality();
    Token *token;
    if (token = check(or_, and_, 0))
    {
        Node *node = new_node(token);
        node->token->index_ = Label_index++;
        node->left = left;
        node->right = logic();
        return node;
    }
    return left;
}

Node *equality()
{
    Node *left = comparison();
    Token *token;
    if (token = check(equal_, not_equal_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = equality();
        left = node;
    }
    return left;
}

Node *comparison()
{
    Node *left = add_sub();
    Token *token;
    if (token = check(less_than_, grea_than_,
                      less_than_equal_, grea_than_equal_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = comparison();
        left = node;
    }
    return left;
}

Node *add_sub()
{
    Node *left = mul_div();
    Token *token;
    if (token = check(add_, sub_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = add_sub();
        return node;
    }
    return left;
}

Node *mul_div()
{
    Node *left = unary();
    Token *token;

    if (token = check(mod_, 0))
    {
        Node *right = mul_div();
        Node *div = new_node(new_token(0, 0, div_, token->col));

        div->left = copy_node(left);
        div->right = copy_node(right);
        Node *mul = new_node(new_token(0, 0, mul_, token->col));
        mul->left = right;
        mul->right = div;

        token->type = sub_;
        Node *node = new_node(token);
        node->left = left;
        node->right = mul;
        return node;
    }
    if (token = check(mul_, div_, 0))
    {
        Node *node = new_node(token);
        node->left = left;
        node->right = mul_div();
        return node;
    }
    return left;
}

// TODO: handle negative number / be carefull of casting long to in for example
Node *unary()
{
    Token *token = check(add_, sub_, 0);
    Node *left = brackets();
    if (token && token->type == sub_)
    {
#if 0
        if (minus_int == NULL)
        {
            minus_int = calloc(1, sizeof(Token));
            minus_int->type = int_;
            minus_int->int_ = -1;
            minus_float = calloc(1, sizeof(Token));
            minus_float->type = float_;
            float f = -1.0;
            minus_float->float_ = *(uint32_t *)(&f);
            minus_float->index_ = index_++;
        }
#endif
        Node *node = new_node(new_token(0, 0, neg_, left->token->col));
        node->left = left;
        return node;
    }
    return left;
}

#define BRAC 0
Node *brackets()
{
    Node *node = prime();
#if BRAC
    Token *token;
    if (token = check(lbracket_, 0))
    {
        Node *tmp = new_node(token);
        tmp->left = node;
        tmp->right = new_node(expect(__LINE__, identifier_, int_, 0));
        expect(__LINE__, rbracket_, 0);
    }
#endif
    return node;
}

Node *prime()
{
    Node *node = NULL;
    Token *token = NULL;
    if (token = check(identifier_, 0))
    {
        node = new_node(token);
        if (check(lparent_, 0))
        {
            debug("found function call\n");
            node->token->ret_type = void_;
            node->token->type = func_call_;
            Node *tmp;
            if (TOKENS[exe_pos]->type != rparent_)
            {
                // function arguments
                node->left = new_node(NULL);
                tmp = node->left;
                // arguments
                while (TOKENS[exe_pos]->type != rparent_ && TOKENS[exe_pos]->type != eof_)
                {
                    tmp->left = expr();
                    if (TOKENS[exe_pos]->type == rparent_ || TOKENS[exe_pos]->type == eof_)
                        break;
                    expect(__LINE__, coma_);
                    tmp->right = new_node(NULL);
                    tmp = tmp->right;
                }
            }
            expect(__LINE__, rparent_);
            if (strcmp(node->token->name, "main") == 0)
            {
                node->token->type = func_dec_;
                expect(__LINE__, dots_);
                tmp = node;
                // main's code bloc
                while (TOKENS[exe_pos]->col > node->token->col && TOKENS[exe_pos]->type != eof_)
                {
                    tmp->right = new_node(NULL);
                    tmp = tmp->right;
                    tmp->left = expr();
                }
            }
        }
#if !BRAC
        while (token = check(lbracket_, 0))
        {
            Node *tmp = new_node(token);
            tmp->left = node;

            node = tmp;
            if (TOKENS[exe_pos]->type != rbracket_)
                node->right = new_node(expect(__LINE__, identifier_, int_, 0));
            expect(__LINE__, rbracket_, 0);
        }
#endif
        return node;
    }
    else if (check(lparent_, 0))
    {
        node = expr();
        expect(__LINE__, rparent_);
    }
    else if (token = check(lbracket_, 0))
    {
        token->name = NULL;
        node = new_node(token);
        node->token->type = array_;

        Node *tmp = node;
        int deep = 1;
        Type type = none_;
        while (!check(rbracket_, 0))
        {
            tmp->left = prime();
            type == none_ ? type = tmp->left->token->type : none_;
            if (TOKENS[exe_pos]->type != rbracket_)
                expect(__LINE__, coma_);
            if (tmp->left->token->type != type)
                error(__LINE__, "in getting array 0");
            // TODO: check for empty arrays
            if (tmp->left->token->type == array_)
                deep = tmp->left->token->depth + 1;
            if (check(rbracket_))
                break;
            tmp->right = new_node(NULL);
            tmp = tmp->right;
        }
        node->token->child_type = type;
        node->token->depth = deep;
        // print_node(node, 0);
        return node;
    }
    else if (token = check(array_, 0))
    {
        free(token->name);
        token->name = NULL;
        debug("found array\n");
        node = new_node(token);
        expect(__LINE__, lbracket_);
        int deep = 1;
        while (check(lbracket_))
            deep++;
        node->token->depth = deep;
        // data type
        node->token->child_type = expect(__LINE__, char_, int_, float_, bool_, 0)->type;
        while (deep)
        {
            expect(__LINE__, rbracket_);
            deep--;
        }
        debug("array has deep %d\n", node->token->depth);
        token = expect(__LINE__, identifier_, 0);
        node->token->name = token->name;
    }
    else if (token = check(char_, int_, float_, bool_, 0))
    {
        if (token->name)
        {
            Type type = token->type;
            token = expect(__LINE__, identifier_);
            token->type = type;
        }
        node = new_node(token);
    }
    else if (token = check(eof_))
        node = new_node(token);
    else if (token = check(if_, 0))
    {
        free(token->name);
        token->name = "";
        node = new_node(token);
        node->token->index_ = Label_index++; // current label index
        Label_index++;
        /*
            cur Label = node->token->index_
            end Label = node->token->index_ - 1
        */
        node->left = new_node(NULL);
        Node *tmp = node->left;

        // the condition bloc
        tmp->left = expr();
        expect(__LINE__, dots_);
        // statement bloc
        while (TOKENS[exe_pos]->col > node->token->col && TOKENS[exe_pos]->type != eof_)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
        // elif
        Node *tmp1 = node;
        while (token = check(elif_, else_, 0))
        {
            free(token->name);
            token->name = "";

            tmp = tmp1;
            tmp->right = new_node(token);
            tmp1 = tmp1->right; // set second tmp

            tmp = tmp->right;
            tmp->token->index_ = Label_index++;

            tmp->left = new_node(NULL);
            tmp = tmp->left;
            if (token->type != else_)
                tmp->left = expr(); // condition
            expect(__LINE__, dots_);
            // code bloc
            while (TOKENS[exe_pos]->col > node->token->col && TOKENS[exe_pos]->type != eof_)
            {
                tmp->right = new_node(NULL);
                tmp = tmp->right;
                tmp->left = expr();
            }
            if (token->type == else_)
                break;
        }
    }
    else if (token = check(while_, 0))
    {
        free(token->name);
        token->name = "";
        node = new_node(token);
        node->token->index_ = Label_index++;
        Label_index++;
        node->left = new_node(NULL);
        Node *tmp = node->left;

        tmp->left = expr();
        expect(__LINE__, dots_);

        tmp = node->left;
        while (TOKENS[exe_pos]->col > node->token->col && TOKENS[exe_pos]->type != eof_)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
    }
    else if (token = check(func_dec_, 0))
    {
        if (!(token = check(int_, float_, bool_, char_, void_, 0)))
            error(__LINE__, "Expected a valid datatype for function declaration");
        Type type = token->type;
        if (!(token = expect(__LINE__, identifier_, 0)))
            error(__LINE__, "Expected name for function declaration");
        token->type = func_dec_;
        node = new_node(token);
        node->token->ret_type = type;
        char *new_name = strjoin(node->token->name, "_");
        free(node->token->name);
        node->token->name = new_name;
        Node *tmp;
        expect(__LINE__, lparent_);
        if (TOKENS[exe_pos]->type != rparent_)
        {
            // function arguments
            node->left = new_node(NULL);
            tmp = node->left;
            // arguments
            while (TOKENS[exe_pos]->type != rparent_ && TOKENS[exe_pos]->type != eof_)
            {
                tmp->left = expr();
                if (TOKENS[exe_pos]->type == rparent_ || TOKENS[exe_pos]->type == eof_)
                    break;
                expect(__LINE__, coma_);
                tmp->right = new_node(NULL);
                tmp = tmp->right;
            }
        }
        expect(__LINE__, rparent_);

        expect(__LINE__, dots_);
        tmp = node;
        // code bloc
        while (TOKENS[exe_pos]->col > node->token->col && TOKENS[exe_pos]->type != eof_)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = expr();
        }
    }
    else
        error(__LINE__, "Unexpected %s in prime",
              token ? type_to_string(__LINE__, token->type) : "(null)");
    return node;
}

void initialize()
{
    print_asm(".intel_syntax noprefix\n");
    print_asm(".include \"import/header.s\"\n\n");
    print_asm(".text\n");
    print_asm(".globl	main\n");

    Node *curr = new_node(NULL);
    Node *head = curr;
    curr->left = expr();
    while (TOKENS[exe_pos]->type != eof_)
    {
        curr->right = new_node(NULL);
        curr = curr->right;
        curr->left = expr();
    };
    debug("%sABSTRACT TREE:%s\n", GREEN, RESET);
    curr = head;
    while (curr)
    {
        print_node(curr->left, 0);
        curr = curr->right;
    }
#if 1
    Node *new = new_node(new_token(0, 0, 0, 0));
    new->token->name = "";
    enter_label(new);
    curr = head;
    while (curr)
    {
        evaluate(curr->left);
        curr = curr->right;
    }
#endif
    free_node(new);
    free_node(head);
}

void finalize()
{
    for (int i = 0; i < tk_pos; i++)
    {
        // test char variable before making any modification
        if (!TOKENS[i]->name && TOKENS[i]->index_ && TOKENS[i]->type == char_)
            print_asm("STR%zu: .string \"%s\"\n", TOKENS[i]->index_, TOKENS[i]->char_);
        if (!TOKENS[i]->name && TOKENS[i]->index_ && TOKENS[i]->type == float_)
            print_asm("FLT%zu: .long %zu /* %f */\n", TOKENS[i]->index_,
                      *((float *)(&TOKENS[i]->float_)),
                      TOKENS[i]->float_);
    }
    print_asm(".section	.note.GNU-stack,\"\",@progbits\n");
}

Token *evaluate(Node *node)
{
    Token *left = NULL, *right = NULL;
    Type type = node->token->type;
    switch (type)
    {
    case identifier_:
    {
        left = get_var(node->token->name);
        if (!left)
            error(__LINE__, "Undeclared variable '%s'", node->token->name);
        node->token = left;
        break;
    }
    case char_:
    case float_:
    case int_:
    case bool_:
    {
        if (node->token->name)
        {
            if (get_var(node->token->name))
                error(__LINE__, "redefinition of variable");
            new_variable(node->token);
        }
        break;
    }
    case array_:
    {
        if (node->token->name)
        {
            if (get_var(node->token->name))
                error(__LINE__, "redefinition of variable");
            new_variable(node->token);
        }
        else
        {
            debug("%sfound array without name%s\n", GREEN, RESET);
            size_t len = 100;
            size_t r = 0;
            size_t l = 0;

            Node **queue = calloc(len, sizeof(Node *));
            Node *tmp = node;
            while (tmp)
            {
                queue[r++] = tmp->left;
                if (r + 1 == len)
                    queue = realloc(queue, (len *= 2) * sizeof(Node *));
                tmp = tmp->right;
            }
            while (queue[l]->token->type == array_)
            {
                // TODO: check it it has ptr
                Node *tmp = queue[l++];
                while (tmp)
                {
                    queue[r++] = tmp->left;
                    tmp = tmp->right;
                }
            }
            l = 0;
            while (r > 0)
            {
                r--;
                debug("loop %d\n", r);
                Token *token = queue[r]->token;
                if (!token->ptr)
                    token->ptr = (ptr += 8);
                if (token->type == array_)
                {
                    // print_asm("   mov     QWORD PTR -%zu[rbp], 0\n", token->ptr);
                    print_asm("   lea     rax, -%zu[rbp]\n", queue[r]->left->token->ptr);
                    print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", token->ptr);
                }
                else if (token->type == int_)
                {
                    // TODO: check it it has ptr or value
                    print_asm("   mov     QWORD PTR -%zu[rbp], %ld\n", token->ptr, token->int_);
                }
                else
                    error(__LINE__, "handle this case");
            }
            node->token->ptr = queue[0]->token->ptr;
            free(queue);
        }
        break;
    }
    case neg_:
    {
        // TODO: negative float has a different behaviour !!!
        left = evaluate(node->left);
        if (left->type != int_ && left->type != float_)
            error(__LINE__, "Invalid unary operation 0");

        if (!left->name)
        {
            node->token->type = left->type;
            switch (left->type)
            {
            case int_:
                // TODO: protect INT_MIN
                node->token->int_ = -1 * left->int_;
                break;
            case float_:
                float f = -1 * (*(float *)(&left->float_));
                left->float_ = *(uint32_t *)(&f);
                node->token = left;
                break;
            default:
                break;
            }
        }
        else
        {
            Node *curr = new_node(new_token(0, 0, mul_, left->col));
            curr->left = new_node(left);
            if (left->type == int_)
            {
                curr->right = new_node(new_token(0, 0, int_, left->col));
                curr->right->token->int_ = -1;
            }
            else if (left->type == float_)
            {
                curr->right = new_node(new_token(0, 0, float_, left->col));
                float f = -1.0;
                curr->right->token->float_ = *(uint32_t *)(&f);
                curr->right->token->index_ = index_++;
            }
            else
                error(__LINE__, "Invalid unary operation 1");
            node->token = evaluate(curr);
        }
        break;
    }
    case lbracket_:
    {
#if 1
        //-48[rbp+rax*8]
        left = evaluate(node->left);
        right = evaluate(node->right);
        /*
            TODOS:
                + check if right has number or ptr
                + left must have name and ptr
        */
        if (left->type != array_ && left->type != lbracket_)
            error(__LINE__, "Expected array to iterate over %s", type_to_string(__LINE__, left->type));
        if (!left->name && left->type != array_)
            error(__LINE__, "Expected identifier in brackets");
        if (right->type != int_) // TODO: check that in parsing
            error(__LINE__, "Expected number inside brackets");
        debug("left: %k | right: %k\n", left, right);

        debug("<%k> <%k> \n", left, right);
        node->token->ptr = (ptr += 8);
        if (left->depth <= 1)
            node->token->type = left->child_type;
        else
        {
            node->token->type = left->type;
            node->token->child_type = left->child_type;
        }

        // TODO; this approach works only for array of integers
        print_asm("   /* %s[] (%s) */\n", left->name, type_to_string(__LINE__, node->token->type));
        if (right->ptr)
        {
            print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", left->ptr);
            print_asm("   mov     rbx, QWORD PTR -%zu[rbp]\n", right->ptr);
            print_asm("   lea     rbx, 0[0+rbx*8]\n");
            print_asm("   add     rax, rbx\n");
            print_asm("   mov     rax, [rax]\n");
            print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
        }
        else
        {
            print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", left->ptr);
            print_asm("   add     rax, %ld\n", right->int_ * 8);
            print_asm("   mov     rax, [rax]\n");
            print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
        }
        print_node(node, 0);
        // exit(1);
#endif
        break;
    }
    case assign_:
    {
        /*
            TODOS:
                + assign / initializing
                + deep / shallow copy
        */
        left = evaluate(node->left);
        right = evaluate(node->right);
        debug("assign:\n     %k\n     %k\n\n", left, right);

        if (!left->name || left->type != right->type || !left->ptr)
            error(__LINE__, "Invalid assignement %s / %s",
                  type_to_string(__LINE__, left->type), type_to_string(__LINE__, right->type));
        node->token = left;
        switch (left->type)
        {
        case int_:
        {
            if (right->ptr)
            {
                print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     QWORD PTR -%zu[rbp], rax /* assign %s */\n",
                          left->ptr, left->name);
            }
            else
                print_asm("   mov     QWORD PTR -%zu[rbp], %d /* assign %s */\n",
                          left->ptr, right->int_, left->name);
            break;
        }
        case float_:
        { // TODO: check xmms, with multiple LABELS[lb_pos]->VARIABLES
            if (right->ptr)
            {
                print_asm("   movss   xmm1, DWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1 /* assign %s */\n",
                          left->ptr, left->name);
            }
            else
            {
                print_asm("   movss   xmm1, DWORD PTR FLT%zu[rip]\n", right->index_);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1 /* assign %s */\n",
                          left->ptr, left->name);
            }
            break;
        }
        case char_:
        {
            /*
            TODO:
                + check right has ptr, then load value from there
                + strdup from right
        */
#if 0
            if (left->sub_type == fix_)
            {
#endif
            if (right->index_)
                print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
            else
                print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
#if 0
            }
            else if (left->sub_type == dyn_)
            {
                if (right->index_)
                    print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
                else
                    print_asm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     rdi, rax\n");
                print_asm("   call    _strdup\n");
            }

            else
                error(__LINE__,"in assign char\n");
#endif
            print_asm("   mov     QWORD PTR -%zu[rbp], rax /* assign  %s */\n",
                      left->ptr, left->name);
            break;
        }
        case bool_:
        {
            if (right->ptr)
            {
                print_asm("   mov     al, BYTE PTR -%zu[rbp]\n", right->ptr);
                print_asm("   mov     BYTE PTR -%zu[rbp], al /* assign  %s */\n",
                          left->ptr, left->name);
            }
            else if (right->c)
                print_asm("   mov     BYTE PTR -%zu[rbp], %cl\n", left->ptr, right->c);
            else
                print_asm("   mov     BYTE PTR -%zu[rbp], %d /* assign  %s */\n",
                          left->ptr, right->bool_, left->name);
            break;
        }
        case array_:
        {
            // TODO: check if left already has value / check right also
            // having different sizes may causes problem
            size_t len = 0;
            if (right->ptr)
            {
                print_asm("   lea     rax, QWORD PTR -%zu[rbp] /* assign to %s */\n", right->ptr, left->name);
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", left->ptr);
            }
            else
                error(__LINE__, "handle this one");
            break;
        }
        default:
            error(__LINE__, "add assembly for this one 0");
            break;
        }
        visualize();
        break;
    }
    // arithmetic operators
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != right->type)
            error(__LINE__, "Uncompatible type in math operation");
        node->token->type = left->type;
        // has no name (optimization)
        if (!left->name && !right->name)
        {
            debug("0. do %t between %k with %k\n", type, left, right);
            left->index_ = 0;
            right->index_ = 0;
            switch (node->token->type)
            {
            case int_:
                if (type == add_)
                    node->token->int_ = left->int_ + right->int_;
                else if (type == sub_)
                    node->token->int_ = left->int_ - right->int_;
                else if (type == mul_)
                    node->token->int_ = left->int_ * right->int_;
                else if (type == div_)
                {
                    if (right->int_ == 0)
                        error(__LINE__, "can't devide by 0 (int)");
                    node->token->int_ = left->int_ / right->int_;
                }
                break;
            case float_:
                node->token->index_ = index_++;
                float l = *(float *)(&left->float_);
                float r = *(float *)(&right->float_);
                float res;
                if (type == add_)
                    res = l + r;
                else if (type == sub_)
                    res = l - r;
                else if (type == mul_)
                    res = l * r;
                else if (type == div_)
                {
                    if (r == 0)
                        error(__LINE__, "can't devide by 0 (float)");
                    res = l / r;
                }
                node->token->float_ = *(uint32_t *)(&res);
                break;
            case char_:
                node->token->index_ = index_++;
                if (type == add_)
                    node->token->char_ = strjoin(left->char_, right->char_);
                else
                    error(__LINE__, "invalid math operation for characters");
                break;
            default:
                error(__LINE__, "math operation 0");
                break;
            }
        }
        else
        {
            debug("1. do %t between %k with %k\n", type, left, right);
            char *str;
            switch (node->token->type)
            {
            case int_:
                node->token->ptr = (ptr += 8);
                // set left
                print_asm("   mov     rax, ");
                if (left->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    print_asm("%d\n", left->int_);
                // set right
                print_asm("   %s", type == add_   ? "add     rax, "
                                   : type == sub_ ? "sub     rax, "
                                   : type == mul_ ? "imul    rax, "
                                   : type == div_ ? "cdq\n   mov     rbx, "
                                                  : NULL);
                if (right->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    print_asm("%d\n", right->int_);
                if (type == div_)
                    print_asm("   idiv    rbx\n");
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
                break;
            case float_:
                node->token->ptr = (ptr += 4);
                // set left
                print_asm("   movss   xmm1, ");
                if (left->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", left->index_);
                else
                    print_asm("%zu\n", left->float_);
                // set right
                print_asm("   %s", type == add_   ? "addss   xmm1, "
                                   : type == sub_ ? "subss   xmm1, "
                                   : type == mul_ ? "mulss   xmm1, "
                                   : type == div_ ? "divss   xmm1, "
                                                  : NULL);
                if (right->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                else
                    print_asm("%zu\n", right->float_);
                print_asm("   movss   DWORD PTR -%zu[rbp], xmm1\n", node->token->ptr);
                break;
            case char_:
                if (type != add_)
                    error(__LINE__, "math operation 2");
                node->token->ptr = (ptr += 8);
                if (left->ptr)
                    print_asm("   mov     rdi, QWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", left->index_);
                    print_asm("   mov     rdi, rax\n");
                }
                else
                    error(__LINE__, "in char joining 1");

                if (right->ptr)
                    print_asm("   mov     rsi, QWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
                    print_asm("   mov     rsi, rax\n");
                }
                else
                    error(__LINE__, "in char joining 2");
                print_asm("   call	  _strjoin\n");
                print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);

                break;
            default:
                error(__LINE__, "math operation 1");
                break;
            }
        }
        break;
    }
    // logic operators
    case not_:
        error(__LINE__, "handle not logic operator");
        break;
    case not_equal_:
    case equal_:
    case less_than_:
    case grea_than_:
    case less_than_equal_:
    case grea_than_equal_:
    {
        node->token->type = bool_;
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != right->type)
            error(__LINE__, "Uncompatible type in logic operation");
        // has no name (optimization)
        if (!left->ptr && !right->ptr)
        {
            debug("0. do %t between %k with %k\n", type, left, right);
            left->index_ = 0;
            right->index_ = 0;
            switch (left->type)
            {
            case int_:
                if (type == equal_)
                    node->token->bool_ = (left->int_ == right->int_);
                else if (type == not_equal_)
                    node->token->bool_ = (left->int_ != right->int_);
                else if (type == less_than_)
                    node->token->bool_ = (left->int_ < right->int_);
                else if (type == grea_than_)
                    node->token->bool_ = (left->int_ > right->int_);
                else if (type == less_than_equal_)
                    node->token->bool_ = (left->int_ <= right->int_);
                else if (type == grea_than_equal_)
                    node->token->bool_ = (left->int_ >= right->int_);

                break;
            case float_:
                if (type == equal_)
                    node->token->bool_ = (left->float_ == right->float_);
                else if (type == not_equal_)
                    node->token->bool_ = (left->float_ != right->float_);
                else if (type == less_than_)
                    node->token->bool_ = (left->float_ < right->float_);
                else if (type == grea_than_)
                    node->token->bool_ = (left->float_ > right->float_);
                else if (type == less_than_equal_)
                    node->token->bool_ = (left->float_ <= right->float_);
                else if (type == grea_than_equal_)
                    node->token->bool_ = (left->float_ >= right->float_);
                break;
            case char_:
                if (type == equal_)
                    node->token->bool_ = (strcmp(left->char_, right->char_) == 0);
                else if (type == not_equal_)
                    node->token->bool_ = (strcmp(left->char_, right->char_) != 0);
                else
                    error(__LINE__, "Invalid logic operation on char");
                break;
            default:
                error(__LINE__, "logic operation 0");
                break;
            }
        }
        else
        {
            debug("1. do %t between %k with %k\n", type, left, right);
#define BOOL_PTR 0
#if BOOL_PTR
            node->token->ptr = (ptr += 1);
#else
            node->token->c = 'a';
#endif
            char *str;
            switch (left->type)
            {
            // TODO: handle heap allocated LABELS[lb_pos]->VARIABLES
            case int_:
                // set left
                print_asm("   mov     rax, ");
                if (left->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    print_asm("%d\n", left->int_);

                print_asm("   cmp     rax, ");
                if (right->ptr)
                    print_asm("QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    print_asm("%d\n", right->int_);

                print_asm("   %s   al\n", type == equal_             ? "sete "
                                          : type == equal_           ? "setne"
                                          : type == less_than_       ? "setl "
                                          : type == less_than_equal_ ? "setle"
                                          : type == grea_than_       ? "setg "
                                          : type == grea_than_equal_ ? "setge"
                                                                     : NULL);

#if BOOL_PTR
                print_asm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            case float_:
                if ((type == less_than_ && (type = grea_than_)) ||
                    (type == less_than_equal_ && (type = grea_than_equal_)))
                {
                    Token *tmp = left;
                    left = right;
                    right = tmp;
                }
                print_asm("   movss   xmm0, ");
                if (left->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", left->index_);
                else
                    print_asm("%zu\n", left->float_);

                print_asm("   %s xmm0, ", type != equal_ ? "comiss " : "ucomiss");
                if (right->ptr)
                    print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                    print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                else
                    print_asm("%zu\n", right->float_);

                print_asm("   %s   al\n",
                          type == grea_than_         ? "seta "
                          : type == grea_than_equal_ ? "setnb"
                          : type == equal_           ? "setnp"
                          : type == not_equal_       ? "setp "
                                                     : NULL);
                if (type == equal_)
                {
                    print_asm("   mov	  edx, 0\n");
                    print_asm("   movss	  xmm0, ");
                    if (left->ptr)
                        print_asm("DWORD PTR -%zu[rbp]\n", left->ptr);
                    else if (left->index_)
                        print_asm("DWORD PTR FLT%zu[rip]\n", left->index_);
                    else
                        print_asm("%zu\n", left->float_);
                    print_asm("   ucomiss xmm0, ");
                    if (right->ptr)
                        print_asm("DWORD PTR -%zu[rbp]\n", right->ptr);
                    else if (right->index_)
                        print_asm("DWORD PTR FLT%zu[rip]\n", right->index_);
                    else
                        print_asm("%zu\n", right->float_);
                    print_asm("   cmovne  eax, edx\n");
                }
#if BOOL_PTR
                print_asm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            case char_:
                if (type != equal_)
                    error(__LINE__, "logic operation 3");
                if (left->ptr)
                    print_asm("   mov     rsi, QWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", left->index_);
                    print_asm("   mov     rsi, rax\n");
                }
                else
                    error(__LINE__, "in char equal");

                if (right->ptr)
                    print_asm("   mov     rdi, QWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->index_)
                {
                    print_asm("   lea     rax, STR%zu[rip]\n", right->index_);
                    print_asm("   mov     rdi, rax\n");
                }
                else
                    error(__LINE__, "in char equal 2");
                print_asm("   call	  _strcmp\n");
#if BOOL_PTR
                print_asm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            default:
                error(__LINE__, "logic operation 4");
                break;
            }
        }
        break;
    }
    case and_:
    case or_:
    {
        Node *tmp = node;
        // last right is last node in or/and
        int i = 0;
        while (tmp->token->type == and_ || tmp->token->type == or_)
        {
            left = evaluate(tmp->left);
            if (left->type != bool_)
                error(__LINE__, "0.Expected boolean value");

            print_asm("   /* %s operation %d */\n", type_to_string(__LINE__, node->token->type), i++);
            if (left->ptr)
                print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
            else if (left->c)
                print_asm("   cmp     %cl, 1\n", left->c);
            else
            {
                print_asm("   mov     al, %d\n", left->bool_);
                print_asm("   cmp     al, 1\n");
            }
            if (tmp->token->type == or_)
                print_asm("   je      %s%zu\n", LABEL->name, node->token->index_);
            else if (tmp->token->type == and_)
                print_asm("   jne     %s%zu\n", LABEL->name, node->token->index_);
            tmp = tmp->right;
            if (tmp->token->type == and_ || tmp->token->type == or_)
                print_asm("%s%zu:\n", LABEL->name, tmp->token->index_);
        }
        right = evaluate(tmp);
        if (right->type != bool_)
            error(__LINE__, "0.Expected boolean value");
        print_asm("%s%zu:\n", LABEL->name, node->token->index_);
        node->token->c = 'a';
        node->token->type = bool_;
        break;
    }
#if 1
    case if_:
    {
        Node *curr = node->left;
        size_t end_index = node->token->index_ + 1;
        left = evaluate(curr->left);
        if (left->type != bool_)
            error(__LINE__, "Expected a valid condition in if statement");

        print_asm("%s%zu: %43s\n", LABEL->name, node->token->index_, "/* if statement */");
        if (left->ptr)
            print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
        else if (left->c)
            print_asm("   cmp     %cl, 1\n", left->c);
        else
        {
            print_asm("   mov     al, %d\n", left->bool_);
            print_asm("   cmp     al, 1\n");
        }
        if (node->right)
            print_asm("   jne     %s%zu %39s\n", LABEL->name, node->right->token->index_, "/* jmp next statement */");
        else
            print_asm("   jne     %s%zu %15s\n", LABEL->name, end_index, "/* jmp end statemnt */");
        curr = curr->right;
        // if statment bloc
        while (curr)
        {
            evaluate(curr->left);
            curr = curr->right;
        }
        curr = node->left;
        if (node->right)
            print_asm("   jmp     %s%zu %38s\n", LABEL->name, end_index, "/* jmp end statement */");

        // elif / else statement
        curr = node->right;
        while (curr)
        {

            if (curr->token->type == elif_)
            {
                Node *tmp0 = curr->left;
                // evaluate elif
                print_asm("%s%zu: %45s\n", LABEL->name, curr->token->index_, "/* elif statement */");
                Node *tmp = tmp0;

                left = evaluate(tmp->left);
                if (left->type != bool_)
                    error(__LINE__, "Expected a valid condition in elif statement");
                if (left->ptr)
                    print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
                else if (left->c)
                    print_asm("   cmp     %cl, 1\n", left->c);
                else
                {
                    print_asm("   mov     al, %d\n", left->bool_);
                    print_asm("   cmp     al, 1\n");
                }
                if (curr->right)
                    print_asm("   jne     %s%zu %39s\n", LABEL->name,
                              curr->right->token->index_, "/* jmp next statement */");
                else
                    print_asm("   jne     %s%zu %38s\n", LABEL->name, end_index, "/* jmp end statemnt */");
                tmp = tmp->right;
                while (tmp)
                {
                    evaluate(tmp->left);
                    tmp = tmp->right;
                }
                if (curr->right)
                    print_asm("   jmp     %s%zu %38s\n", LABEL->name,
                              end_index, "/* jmp end statement */");
            }
            else if (curr->token->type == else_)
            {
                Node *tmp = curr->left->right;
                print_asm("%s%zu: %45s\n", LABEL->name, curr->token->index_, "/* else statement */");
                while (tmp)
                {
                    evaluate(tmp->left);
                    tmp = tmp->right;
                }
            }
            curr = curr->right;
        }
        // end statement bloc
        print_asm("%s%zu: %44s\n", LABEL->name, end_index, "/* end statement */");

        break;
    }
    case while_:
    {
        Node *curr = node->left;
        print_asm("   jmp     %s%zu %46s\n", LABEL->name,
                  node->token->index_ - 1, "/* jmp to while loop condition*/");
        // while loop bloc
        print_asm("%s%zu: %44s\n", LABEL->name, node->token->index_, "/* while loop bloc*/");
        Node *tmp = curr->right;
        while (tmp)
        {
            evaluate(tmp->left);
            tmp = tmp->right;
        }
        // while loop condition
        print_asm("%s%zu: %53s\n", LABEL->name, node->token->index_ - 1,
                  "/* while loop condition */");
        left = evaluate(curr->left);
        if (left->type != bool_)
            error(__LINE__, "Expected a valid condition in if statment");
        if (left->ptr)
            print_asm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
        else if (left->c)
            print_asm("   cmp     %cl, 1\n", left->c);
        else
        {
            print_asm("   mov     al, %d\n", left->bool_);
            print_asm("   cmp     al, 1\n");
        }
        print_asm("   je      %s%zu %43s\n", LABEL->name, node->token->index_,
                  "/* je to while loop bloc*/");
        break;
    }
#endif
    case func_call_:
    {
        debug("found function call has name '%s'\n", node->token->name);
        print_node(node, 0);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            Node *tmp = node->left;
            while (tmp)
            {
                output(evaluate(tmp->left));
                tmp = tmp->right;
            }
        }
        else
        {
            char *name = strjoin(node->token->name, "_");
            Node *func;
            if (!(func = get_func(name)))
            {
                visualize();
                error(__LINE__, "Undeclared function '%s'\n", node->token->name);
            }
            if (func->left)
            {
                // push arguments:
                int stack_len = 100;
                int stack_pos = 0;
                Token **stack = calloc(stack_len, sizeof(Token *));
                Node *tmp = func->left;
                while (tmp)
                {
                    stack[stack_pos++] = get_var(tmp->left->token->name);
                    if (stack_pos + 2 > stack_len)
                    {
                        stack = realloc(stack, 2 * stack_len * sizeof(Token *));
                        stack_len *= 2;
                    }
                    tmp = tmp->right;
                }
                stack_pos--;
                tmp = func->left;
                while (stack_pos >= 0)
                {
                    Token *token = stack[stack_pos];
                    //  TODO: check argument type if variable or value....
                    Token *arg = tmp->left->token;
                    if (arg->type != token->type)
                        error(__LINE__, "Incompatible type in function call\n");
                    if (token->ptr)
                        print_asm("   push    QWORD PTR -%zu[rbp]\n", token->ptr);
                    stack_pos--;
                    tmp = tmp->right;
                }
                free(stack);
            }
            print_asm("   call    %s\n", func->token->name);
            free(name);
        }
        break;
    }
    case func_dec_:
    {
        new_func(node);
        char *name = node->token->name;
        enter_label(node);
        print_asm("%s:\n", name);
        print_asm("   push    rbp\n");
        print_asm("   mov     rbp, rsp\n");
        print_asm("   sub     rsp, %zu\n", rsp);

        // arguments
        Node *tmp = node->left;
        while (tmp)
        {
            Token *token = evaluate(tmp->left);
            // TODO: I did only test for integers, check other data types
            print_asm("   mov	  rax, QWORD PTR %zu[rbp]\n", token->ptr + 8);
            print_asm("   mov     QWORD PTR -%zu[rbp], rax\n", token->ptr);
            tmp = tmp->right;
        }

        // code bloc
        tmp = node->right;
        while (tmp)
        {
            if (tmp->left->token->type == func_dec_)
                print_asm("   jmp     end_%s%s\n", name, tmp->left->token->name);

            evaluate(tmp->left);
            tmp = tmp->right;
        }
        print_asm("   leave\n");
        print_asm("   ret\n");
        print_asm("end_%s:\n\n", name);
        exit_label(node);

        break;
    }
    default:
        error(__LINE__, "in evaluate %t", type);
        break;
    }
    if (ptr + 10 > rsp)
    {
        // TODO: protect this line from being printed in wrong place
        // after label for example
        print_asm("   sub     rsp, %zu\n", rsp * 2);
        rsp = rsp + rsp * 2;
    }
    return node->token;
}

int main(int argc, char **argv)
{
    // TODO: check if file ends with .w
    if (argc != 2)
        error(__LINE__, "require one file.w as argument\n");
    file = fopen(argv[1], "r");
    asm_fd = fopen("file.s", "w");

    if (!file || !asm_fd)
        error(__LINE__, "Opening file");
    fseek(file, 0, SEEK_END);
    size_t txt_len = ftell(file);
    if (!(text = calloc(txt_len + 1, sizeof(char))))
        error(__LINE__, "Allocation");
    fseek(file, 0, SEEK_SET);
    fread(text, txt_len, sizeof(char), file);
    fclose(file);
    debug("%s\n\n", text);
    Label_index = 1;
    index_ = 1;

    build_tokens();
    debug("\n");
    free(text);
    rsp = 30;
    initialize();
    finalize();
    debug("\nresult: (exiting with %d)\n", EXIT_STATUS);
    exit(EXIT_STATUS);
}