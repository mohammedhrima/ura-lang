#include "header.h"

// DEV TOOLS
void error_msg(int line, char *fmt, ...)
{
    // TODO: update err messages
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%sline %d: ", RED, line);
    vfprintf(stderr, fmt, ap);
    fprintf(stdout, "%s\n", RESET);
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
    error_msg(line, "err unkown type (%d | %c)\n", type, type);
    return NULL;
}

#define tts(arg) type_to_string(__LINE__, arg)
#define err(...) error_msg(__LINE__, __VA_ARGS__)

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
                    fprintf(stdout, "%s", tts((Type)va_arg(args, Type)));
                    break;
                case 'k':
                {
                    Token *token = (Token *)va_arg(args, Token *);
                    if (token)
                    {
                        fprintf(stdout, "%s ", tts(token->type));
                        if (token->name)
                            fprintf(stdout, "<name:%5s> ", token->name);
                        if (token->is_ref)
                            fprintf(stdout, "isref ");
                        if (token->ptr)
                            fprintf(stdout, "PTR(%zu) ", token->ptr);
                        else
                            switch (token->type)
                            {
                            case chars_:
                                fprintf(stdout, "'%s' ", token->chars_);
                                if (token->chars_index_)
                                    fprintf(stdout, "STR%zu, ", token->chars_index_);
                                break;
                            case char_:
                                fprintf(stdout, "'%c' ", token->int_);
                                break;
                            case int_:
                                fprintf(stdout, "%d ", token->int_);
                                break;
                            case float_:
                                fprintf(stdout, "%zu (%.2f) ", token->float_, *(float *)(&token->float_));
                                if (token->float_index_)
                                    fprintf(stdout, "FLT%zu ", token->float_index_);
                                break;
                            case bool_:
                                fprintf(stdout, "%s ", token->bool_ ? "True" : "False");
                                break;
                            case array_:
                                fprintf(stdout, "depth: %zu, ", token->depth);
                                if (token->child_type)
                                    fprintf(stdout, "child: %s, ", tts(token->child_type));
                                if (token->arrlen_)
                                    fprintf(stdout, "elems: %zu, ", token->arrlen_);
                                break;
                            case func_call_:
                                fprintf(stdout, "ret (%s) ", tts(token->ret_type));
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
                    err("in debug function (%c)", conv[i]);
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
        err("is null\n");
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
void pasm(char *fmt, ...)
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
    // TODO: verify each condition and test  it carefully
    switch (token->type)
    {
    case chars_:
    {
        pasm("   /* call _putstr */\n");
        if (token->is_ref)
        {
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            pasm("   mov     rax, QWORD PTR [rax]\n");
        }
        else if (token->ptr)
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
        else if (token->chars_index_)
            pasm("   lea     rax, STR%zu[rip]\n", token->chars_index_);
        else
            err("output char");
        pasm("   mov     rdi, rax\n");
        pasm("   call    _putstr\n");
        break;
    }
    case char_:
    {
        pasm("   /* call _putchar */\n");
        if (token->is_ref)
        {
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            pasm("   mov     al, BYTE PTR [rax]\n");
        }
        else if (token->ptr)
            pasm("   mov     al, BYTE PTR -%zu[rbp]\n", token->ptr);
        else if (token->int_)
            pasm("   mov     al, %lld\n", token->int_);
        else
            err("output int");
        pasm("   mov     edi, eax\n");
        pasm("   call    _putchar\n");
        break;
    }
    case int_:
    {
        pasm("   /* call _putnbr */\n");
        if (token->is_ref)
        {
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            pasm("   mov     rax, QWORD PTR [rax]\n");
        }
        else if (token->ptr)
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
        else if (token->int_)
            pasm("   mov   rax, %lld\n", token->int_);
        else
            err("output int");
        pasm("   mov     rdi, rax\n");
        pasm("   call    _putnbr\n");
        break;
    }
    case bool_:
    {
        pasm("   /* call _putbool */\n");
        if (token->is_ref)
        {
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            pasm("   mov     rax, QWORD PTR [rax]\n");
        }
        else if (token->ptr)
            pasm("   movzx   eax, BYTE PTR -%zu[rbp]\n", token->ptr);
        else if (token->c)
            pasm("   movzx   eax, %cl\n", token->c);
        else
            err("output bool");
        pasm("   mov	  edi, eax\n");
        pasm("   call	  _putbool\n");
        break;
    }
    case float_:
        // TODO: handle float
        // err("Err in output float not handled yet");
        pasm("   /* call _putfloat */\n");
        if (token->is_ref)
        {
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", token->ptr);
            pasm("   mov     eax, DWORD PTR [rax]\n");
        }
        else if (token->ptr)
            pasm("   mov     eax, DWORD PTR -%zu[rbp]\n", token->ptr);
        else if (token->float_index_)
            pasm("   mov     eax, DWORD PTR FLT%zu[rip]\n", token->float_index_);
        else
            err("output float");
        pasm("   movd    xmm0, eax\n");
        pasm("   call    _putfloat\n");
        break;
    case array_:
        // TODO: handle array somehow
        err("Err in output array");
        break;
    case identifier_:
        break;
    default:
        err("Err in output unknown type %s",
            tts(token->type));
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
    if (lb_pos && node->token->type != if_ && node->token->type != while_)
    {
        char *tmp = strjoin(LABELS[lb_pos]->name, name);
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
        err("in label exit\n");
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
    if (token->is_ref)
    {
        token->ptr = (ptr += 8);
        pasm("   mov     QWORD PTR -%zu[rbp], 0 /* declare %s (ref) */\n", token->ptr, token->name);
    }
    else
        switch (token->type)
        {
        case chars_:
            token->ptr = (ptr += 8);
            pasm("   mov     QWORD PTR -%zu[rbp], 0 /* declare %s */\n", token->ptr, token->name);
            break;
        case int_:
            token->ptr = (ptr += 8);
            pasm("   mov     QWORD PTR -%zu[rbp], 0 /* declare %s */\n", token->ptr, token->name);
            break;
        case char_:
            token->ptr = (ptr += 1);
            pasm("   mov     BYTE PTR -%zu[rbp], 0 /* declare %s */\n", token->ptr, token->name);
            break;
        case float_:
            token->ptr = (ptr += 4);
            pasm("   mov     DWORD PTR -%zu[rbp], 0 /* declare %s */\n", token->ptr, token->name);
            break;
        case bool_:
            token->ptr = (ptr += 1);
            pasm("   mov     BYTE PTR -%zu[rbp], 0 /* declare %s */\n", token->ptr, token->name);
            break;
        case array_:
            token->ptr = (ptr += 8);
            pasm("   mov     QWORD PTR -%zu[rbp], 0 /* declare %s */\n", token->ptr, token->name);
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
    debug("get_var %s from label: %s, pos: %d\n", name, LABEL->name, lb_pos);
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
    debug("new function, name: %s, return type: %t, in Label %s\n", node->token->name, node->token->type,
          LABEL->name[0] ? LABEL->name : "global");
    return (LABEL->FUNCTIONS[LABEL->func_pos++] = node);
}

Node *get_func(char *name)
{
    debug("get_func %s from label: %s / has pos\n", name, LABEL->name[0] ? LABEL->name : "global", lb_pos);
    for (int j = lb_pos; j > 0; j--)
    {
        Label *curr = LABELS[j];
        char *tmp = strjoin(curr->name, name);
        debug("loop: label %s, has %d functions, get %s\n", curr->name[0] ? curr->name : "global",
              curr->func_pos, tmp);
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

void free_token(Token *token)
{
    // if(token->name)
    //     fre
    // switch (token->type)
    // {
    // case chars_:
    //     free(token->chars_);
    //     break;

    // default:
    //     break;
    // }
}

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
            free(token->name);
            token->name = NULL;
            token->type = bool_;
            token->bool_ = true;
            break;
        }
        else if (strcmp(token->name, "False") == 0)
        {
            free(token->name);
            token->name = NULL;
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
    case chars_:
        token->chars_index_ = index_;
        index_++;
        token->chars_ = calloc(e - s + 1, sizeof(char));
        strncpy(token->chars_, text + s, e - s);
        break;
    case char_:
        token->int_ = text[s];
        break;
    case int_:
        while (s < e)
            token->int_ = 10 * token->int_ + text[s++] - '0';
        break;
    case float_:
        float f = 0.0;
        token->float_index_ = index_;
        index_++;
        while (s < e && isdigit(text[s]))
            f = 10 * f + text[s++] - '0';
        s++;
        float c = 10;
        while (s < e && isdigit(text[s]))
        {
            f = f + (float)(text[s++] - '0') / c;
            c *= 10;
        }
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
        if (text[e] == '#')
        {
            e++;
            while (text[e] && text[e] != '#')
                e++;
            if (!text[e])
                err("Expected end of comment");
            e++;
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
                    err("Expected new line after dots");
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
            e++;
            if (text[s] == '\'')
                e++;
            else
                while (text[e] && text[e] != text[s])
                    e++;
            if (text[e++] != text[s])
                err("Syntax");
            new_token(s + 1, e - 1, text[s] == '\"' ? chars_ : char_, col);
            continue;
        }
        err("tokenizing: %s", text + s);
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

void pnode(Node *node, int col)
{
    Node *tmp = NULL;
    int curr = col;
    if (node)
    {
        Token *token = node->token;
        while (curr)
        {
            debug("   ");
            curr--;
        }
        switch (token->type)
        {
        case if_:
        case elif_:
        case else_:
        case while_:
        {
            debug("%s%t %s %s\n", GREEN, token->type, token->name, RESET);
            tmp = node->left;
            if (token->type != else_) // condition
                pnode(tmp->left, col + 1);
            // code bloc
            tmp = node->left->right;
            while (tmp)
            {
                pnode(tmp->left, col + 1);
                tmp = tmp->right;
            }
            // elif / else nodes
            tmp = node;
            while (token->type == if_ && tmp->right)
            {
                pnode(tmp->right, col);
                tmp = tmp->right;
            }
            break;
        }
        case class_:
        {
            debug("%s%t %s %s\n", GREEN, token->type, token->name, RESET);

            // attributes
            tmp = node->left;
            if (tmp)
                pnode(tmp, col + 1);
            tmp = node->right;
            while (tmp)
            {
                pnode(tmp->left, col + 1);
                tmp = tmp->right;
            }

            // tmp = node->left;
            // // constractors
            // while(tmp)
            // {
            //     print_node(tmp->left, col + 1);
            //     tmp = tmp->right;
            // }
            break;
        }
        case func_call_:
        case func_dec_:
        {
            debug("%s%k%s\n", GREEN, token, RESET);
            // arguments
            tmp = node->left;
            while (tmp)
            {
                pnode(tmp->left, col + 1);
                tmp = tmp->right;
            }
            // code bloc
            tmp = node->right;
            while (tmp)
            {
                pnode(tmp->left, col + 1);
                tmp = tmp->right;
            }

            break;
        }
        case array_:
        {
            debug("%s%k%s\n", GREEN, token, RESET);
            while (node)
            {
                pnode(node->left, col + 1);
                node = node->right;
            }
            break;
        }
        default:
        {
            debug("%k\n", token);
            pnode(node->left, col + 1);
            pnode(node->right, col + 1);
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
    if (node->token)
    {
        new->token = calloc(1, sizeof(Token));
        memcpy(new->token, node->token, sizeof(Token));
        if (tk_pos + 2 > tk_len)
        {
            tk_len *= 2;
            TOKENS = realloc(TOKENS, tk_len * sizeof(Token *));
        }
        TOKENS[tk_pos++] = new->token;
    }
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
    error_msg(line, "Unexpected %s\n", tts(type));
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
        Node *right = assign();
        Node *tmp = right;
        Type type = token->type;
        if (type != assign_)
        {
            type = type == add_assign_ ? add_ : type == sub_assign_ ? sub_
                                            : type == mul_assign_   ? mul_
                                            : type == div_assign_   ? div_
                                                                    : 0;
            tmp = new_node(new_token(0, 0, type, token->col));
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
        node->token->label_index_ = Label_index++;
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
    if (token = check(less_than_, grea_than_, less_than_equal_, grea_than_equal_, 0))
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

Node *unary()
{
    Token *token = check(add_, sub_, 0);
    Node *left = prime();
    if (token && token->type == sub_)
    {
        Node *node = new_node(new_token(0, 0, neg_, left->token->col));
        node->left = left;
        return node;
    }
    return left;
}

Node *prime()
{
    Node *node = NULL;
    Token *token = NULL;
    Node *tmp = NULL;
    Type type = 0;
    if (token = check(identifier_, 0))
    {
        node = new_node(token);
        if (check(lparent_, 0))
        {
            node->token->ret_type = void_;
            node->token->type = func_call_;
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
            if (
                strcmp(node->token->name, "main") == 0 ||
                strcmp(node->token->name, "constractor") == 0 ||
                strcmp(node->token->name, "destractor") == 0)
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
        while (token = check(lbracket_, 0))
        {
            tmp = new_node(token);
            tmp->left = node;

            node = tmp;
            if (TOKENS[exe_pos]->type != rbracket_)
                node->right = new_node(expect(__LINE__, identifier_, int_, 0));
            expect(__LINE__, rbracket_, 0);
        }
        return node;
    }
    else if (token = check(func_dec_, 0))
    {
        /*
            TODOS:
                - expect return if function has ret_type
                - return reference
        */
        if (!(token = check(chars_, char_, int_, float_, bool_, void_, 0)))
            err("Expected a valid datatype for function declaration");
        type = token->type;
        if (!(token = expect(__LINE__, identifier_, 0)))
            err("Expected name for function declaration");
        token->type = func_dec_;
        node = new_node(token);
        node->token->ret_type = type;
        char *new_name = strjoin(node->token->name, "_");
        free(node->token->name);
        node->token->name = new_name;
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

        tmp = node;
        int deep = 1;
        type = none_;
        while (!check(rbracket_, 0))
        {
            tmp->left = prime();
            type == none_ ? type = tmp->left->token->type : none_;
            if (TOKENS[exe_pos]->type != rbracket_)
                expect(__LINE__, coma_);
            if (tmp->left->token->type != type)
                err("in getting array 0");
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

        return node;
    }
    else if (token = check(array_, 0))
    {
        free(token->name);
        token->name = NULL;
        node = new_node(token);
        expect(__LINE__, lbracket_);
        int deep = 1;
        while (check(lbracket_))
            deep++;
        node->token->depth = deep;
        // data type
        node->token->child_type = expect(__LINE__, chars_, char_, int_, float_, bool_, 0)->type;
        while (deep)
        {
            expect(__LINE__, rbracket_);
            deep--;
        }
        token = expect(__LINE__, identifier_, 0);
        node->token->name = token->name;
    }
    else if (token = check(ref_, 0))
    {
        token = expect(__LINE__, chars_, char_, int_, float_, bool_, 0);
        if (!token->name)
            err("expected a identifier after ref keyword");
        type = token->type;
        node = new_node(expect(__LINE__, identifier_, 0));
        node->token->type = type;
        node->token->is_ref = true;
        return node;
    }
    else if (token = check(chars_, char_, int_, float_, bool_, 0))
    {
        if (token->name) // it might be int variable or int value
        {
            type = token->type;
            token = expect(__LINE__, identifier_);
            token->type = type;
        }
        node = new_node(token);
    }
    else if (token = check(class_, 0))
    {
        /*
        class User:
            attributes:
                int x
                int y
                int z
            constractor():
                x = 10
                y = 7
            constractor():
                x = 10
                y = 7
            destractor():
                x = 0
                y = 0
        */
        err("check class\n");
#if 0
        free(token->name);
        token->name = NULL;
        debug("found class\n");
        // class class_name:
        node = new_node(token);
        node->token->name = expect(__LINE__ , identifier_, 0)->name;
        expect(__LINE__, dots_, 0);

        // attributes
        node->left = new_node(expect(__LINE__, identifier_, 0));
        expect(__LINE__, dots_, 0);
        node->left->token->type = func_dec_;
        node->left->token->ret_type = class_;
        tmp = node->left;
        while (TOKENS[exe_pos]->col > node->left->token->col)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            // TODO: should be data type with name
            tmp->left = prime();
        }
        // constractor / destractor
        tmp = node;
        while (TOKENS[exe_pos]->col > node->token->col)
        {
            tmp->right = new_node(NULL);
            tmp = tmp->right;
            tmp->left = prime();
            tmp->left->token->ret_type = class_;
        }
        // print_node(node, 0);
#endif
    }
    else if (token = check(if_, 0))
    {
        /*
            end         : label - 1
            conditon    : label
        */
        node = new_node(token);
        Label_index++;
        node->token->label_index_ = Label_index++; // current label index
        node->left = new_node(NULL);
        tmp = node->left;

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
            tmp = tmp1;
            tmp->right = new_node(token);
            tmp1 = tmp1->right; // set second tmp

            tmp = tmp->right;
            tmp->token->label_index_ = Label_index++;

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
        /*
            condition   : label
            bloc        : label + 1
            end         : label + 2
        */
        node = new_node(token);
        node->token->label_index_ = Label_index++;
        Label_index += 2;
        node->left = new_node(NULL);
        tmp = node->left;

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
    else if (token = check(not_, 0))
    {
        node = new_node(token);
        // expect(__LINE__, lparent_);
        node->left = expr();
        // expect(__LINE__, rparent_);
    }
    else if (token = check(return_, 0))
    {
        node = new_node(token);
        node->left = expr();
    }
    else if (token = check(eof_, continue_, break_, 0))
        node = new_node(token);
    else
    {
        err("Unexpected %s in prime", token ? tts(token->type) : "(null)");
    }
    return node;
}

void initialize()
{
    pasm(".intel_syntax noprefix\n");
    pasm(".include \"/wolf-c/import/header.s\"\n\n");
    pasm(".text\n");
    pasm(".globl	main\n");

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
        pnode(curr->left, 0);
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
        if (!TOKENS[i]->name && TOKENS[i]->chars_ && TOKENS[i]->type == chars_ && !TOKENS[i]->ptr)
            pasm("STR%zu: .string \"%s\"\n", TOKENS[i]->chars_index_, TOKENS[i]->chars_);
        if (!TOKENS[i]->name && TOKENS[i]->float_index_ && TOKENS[i]->type == float_)
            pasm("FLT%zu: .long %zu /* %f */\n", TOKENS[i]->float_index_, *((float *)(&TOKENS[i]->float_)),
                 TOKENS[i]->float_);
    }
    pasm(".section	.note.GNU-stack,\"\",@progbits\n");
}

bool skip_check;
Token *evaluate(Node *node)
{
    /*
        mov	rax, QWORD PTR -8[rbp]
        mov	eax, DWORD PTR [rax]
    */
    Token *left = NULL, *right = NULL;
    Type type = node->token->type;
    bool is_ref;
    switch (type)
    {
    case identifier_:
    {
        left = get_var(node->token->name);
        if (!left)
            err("Undeclared variable '%s'", node->token->name);
        node->token = left;
        break;
    }
    case char_:
    case chars_:
    case float_:
    case int_:
    case bool_:
    {
        if (node->token->name)
        {
            if (get_var(node->token->name))
                err("redefinition of variable %s");
            new_variable(node->token);
        }
        break;
    }
    case array_:
    {
        // TODO: use heap allocation
        pnode(node, 0);
        is_ref = node->token->is_ref;
        if (node->token->name)
        {
            if (get_var(node->token->name))
                err("redefinition of variable %s", node->token->name);
            new_variable(node->token);
        }
        else
        { 
            node->token->ptr = (ptr += 8);
            size_t len = 100;
            size_t r = 0;
            size_t l = 0;
            size_t i = 0;

            Node **queue = calloc(len, sizeof(Node *));
            Node *tmp = node;
            while (tmp)
            {
                queue[r++] = tmp->left;
                if (r + 1 == len)
                    queue = realloc(queue, (len *= 2) * sizeof(Node *));
                tmp = tmp->right;
            }
            node->token->arrlen_ = r;
            while (queue[l]->token->type == array_)
            {
                Node *tmp = queue[l++];
                i = 0;
                while (tmp)
                {
                    queue[r++] = tmp->left;
                    tmp = tmp->right;
                    i++;
                }
                queue[l - 1]->token->arrlen_ = i;
            }

            pasm("   mov     rax, %zu\n", node->token->arrlen_);
            if (node->token->depth > 1) // array
                pasm("   mov     rsi, 8\n");
            else // int, char, chars, bool ...
            {
                switch (node->token->child_type)
                {
                case int_:
                    pasm("   mov     rsi, 8\n");
                    break;
                default:
                    err("handle this case (%s)", tts(node->token->child_type));
                    break;
                }
            }
            pasm("   mov     rdi, rax\n");
            pasm("   call    calloc@PLT\n");
            pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);

            l = 0;
            i = 0;
            while (l < r)
            {
                Token *token = queue[l]->token;
#if 0
                if (!token->ptr)
                    token->ptr = token->type == char_ ? (ptr += 1) : (ptr += 8);
                if (token->type == array_)
                {
                    // pasm("   mov     QWORD PTR -%zu[rbp], 0\n", token->ptr);
                    pasm("   lea     rax, -%zu[rbp]\n", queue[r]->left->token->ptr);
                    pasm("   mov     QWORD PTR -%zu[rbp], rax\n", token->ptr);
                }
                else
#endif
                if (token->type == int_)
                {
                    // TODO: check if it has ptr or value
                    pasm("   mov     QWORD PTR [rax], %ld\n", token->int_);
                    pasm("   add     rax, 8\n");
                }
                else if (token->type == char_)
                {
                    // TODO: check if it has ptr or value
                    pasm("   mov     QWORD PTR [rax], %ld\n", token->int_);
                    pasm("   add     rax, 1\n");
                }
                else
                    err("handle this case");
                l++;
            }
            node->token->ptr = queue[0]->token->ptr;
            free(queue);
        }
        break;
    }
    case neg_:
    {
        left = evaluate(node->left);
        if (left->type != int_ && left->type != float_)
            err("Invalid unary operation 0");

        if (!left->name)
        {
            node->token->type = left->type;
            switch (left->type)
            {
            case int_:
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
            curr->left = new_node(new_token(0, 0, 0, 0));
            memcpy(curr->left->token, left, sizeof(Token));
            curr->left->token->type = identifier_;
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
                curr->right->token->float_index_ = index_++;
            }
            else
                err("Invalid unary operation 1");
            pnode(curr, 0);
            node->token = evaluate(curr);
            free_node(curr);
        }
        break;
    }
    case lbracket_:
    {
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != array_
            //&& left->type != lbracket_
            && left->type != chars_)
            err("Expected array to iterate over %s", tts(left->type));
        if (!left->name && left->type != array_ && left->type != chars_) // TODO: clean this shit
            err("Expected identifier in brackets");
        if (right->type != int_)
            err("Expected number inside brackets");

        switch (left->type)
        {
        case chars_:
        {
            node->token->is_ref = true;
            node->token->has_ref = true;
            node->token->type = char_;
            node->token->ptr = (ptr += 8);
            pasm("   /* %s[] (%s) */\n", left->name, tts(node->token->type));
            // if (right->ptr)
            // {
            //     pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", left->ptr);
            //     pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", right->ptr);
            //     pasm("   lea     rbx, 0[0+rbx*8]\n");
            //     pasm("   add     rax, rbx\n");
            //     pasm("   mov     rax, [rax]\n");
            //     pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
            // }
            // else
            {
                pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", left->ptr);
                pasm("   add     rbx, %ld\n", right->int_);
                pasm("   mov     QWORD PTR -%zu[rbp], rbx\n", node->token->ptr);
            }
            break;
        }
        case array_:
        {
            node->token->ptr = (ptr += 8);
            if (left->depth <= 1)
                node->token->type = left->child_type;
            else
            {
                node->token->type = left->type;
                node->token->child_type = left->child_type;
            }
            // TODO: this approach works only for array of integers
            pasm("   /* %s[] (%s) */\n", left->name, tts(node->token->type));
            if (right->ptr)
            {
                pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", left->ptr);
                pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", right->ptr);
                pasm("   lea     rbx, 0[0+rbx*8]\n");
                pasm("   add     rax, rbx\n");
                pasm("   mov     rax, [rax]\n");
                pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
            }
            else
            {
                pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", left->ptr);
                pasm("   add     rax, %ld\n", right->int_ * 8);
                pasm("   mov     rax, [rax]\n");
                pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
            }
            break;
        }
        default:
            err("Invalid operation");
            break;
        }
        break;
    }
    case assign_:
    {
        left = evaluate(node->left);
        right = evaluate(node->right);

        debug("%sassign:\n     %k\n     %k%s\n\n", RED, left, right, RESET);
        if ((!left->name && !left->is_ref) || !left->ptr ||
            (right->type != func_call_ && left->type != right->type) ||
            (right->type == func_call_ && left->type != right->ret_type))
            err("Invalid assignement %s / %s", tts(left->type), tts(right->type));

        if (right->is_ref && !right->has_ref)
            err("%s must have a reference before get used", right->name);
        if (!skip_check && left->is_ref && !left->has_ref && !right->name)
            err("reference %s must be assigned to variable first time", left->name);

        node->token = left;
#if 0
        if (left->is_ref && right->is_ref)
        {
            err("left is ref , right is ref\n");
            pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
            pasm("   mov     QWORD PTR -%zu[rbp], rax /* assign %s */\n", left->ptr, left->name);
        }
        else
#endif
        if (left->is_ref && !left->has_ref)
        {
            left->has_ref = true;
            pasm("   lea     rax, -%zu[rbp]\n", right->ptr);
            pasm("   mov     QWORD PTR -%zu[rbp], rax\n", left->ptr);
        }
        else
        {
            switch (left->type)
            {
            case int_:
            {
                if (right->ptr)
                {
                    pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                    if (right->is_ref)
                        pasm("   mov     rax, QWORD PTR [rax] /* assign from ref %s */\n", right->name);
                }
                else if (right->type != func_call_)
                    pasm("   mov     rax, %ld \n", right->int_);
                if (left->is_ref)
                {
                    pasm("   mov     rbx,  QWORD PTR -%zu[rbp]\n", left->ptr);
                    pasm("   mov     QWORD PTR [rbx], rax /* assign ref %s */\n", left->name);
                }
                else
                    pasm("   mov     QWORD PTR -%zu[rbp], rax /* assign %s */\n", left->ptr, left->name);
                break;
            }
            case char_:
            {
                if (right->ptr)
                {
                    if (right->is_ref)
                    {
                        pasm("   mov     rax, QWORD PTR -%zu[rbp] /* assign from ref %s */\n",
                             right->ptr, right->name);
                        pasm("   mov     al, BYTE PTR [rax]\n");
                    }
                    else
                        pasm("   mov     al, BYTE PTR -%zu[rbp]\n", right->ptr);
                }
                else if (right->type != func_call_)
                    pasm("   mov     al, %ld    /* '%c' */\n", right->int_, (char)right->int_);
                if (left->is_ref)
                {
                    pasm("   mov     rbx,  QWORD PTR -%zu[rbp]\n", left->ptr);
                    pasm("   mov     BYTE PTR [rbx], al /* assign ref %s */\n", left->name);
                }
                else
                    pasm("   mov     BYTE PTR -%zu[rbp], al /* assign %s */\n", left->ptr, left->name);
                break;
            }
            case float_:
            {
                if (right->ptr)
                {
                    if (right->is_ref)
                    {
                        pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                        pasm("   movd    xmm1, DWORD PTR [rax] /* assign from ref %s */\n", right->name);
                    }
                    else
                        pasm("   movss   xmm1, DWORD PTR -%zu[rbp]\n", right->ptr);
                }
                else
                    pasm("   movss   xmm1, DWORD PTR FLT%zu[rip]\n", right->float_index_);
                if (left->is_ref)
                {
                    pasm("   mov     rbx,  QWORD PTR -%zu[rbp]\n", left->ptr);
                    pasm("   mov     QWORD PTR [rbx], rax /* assign ref %s */\n", left->name);
                }
                else
                    pasm("   movss   DWORD PTR -%zu[rbp], xmm1 /* assign %s */\n", left->ptr, left->name);
                break;
            }
            case chars_:
            {
                // TODO: free allocated memory
                if (right->ptr)
                    pasm("   mov     rax, QWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->chars_index_)
                {
#if 0
                    int i = strlen(right->chars_) - 1;
                    pasm("   mov     BYTE PTR -%zu[rbp], 0\n", (ptr += 1));
                    while (i >= 0)
                    {
                        pasm("   mov     BYTE PTR -%zu[rbp], %d\n", (ptr += 1), right->chars_[i]);
                        i--;
                    }
                    pasm("   lea     rax, QWORD PTR -%zu[rbp]\n", ptr);
#else
                    pasm("   lea     rax, STR%zu[rip]\n", right->chars_index_);
#endif
                }
                else
                    err("something went wrong");
                pasm("   mov     rdi, rax\n");
                pasm("   call    _strdup\n");
                pasm("   mov     QWORD PTR -%zu[rbp], rax /* assign  %s */\n", left->ptr, left->name);
                break;
            }
            case bool_:
            {
                if (right->ptr)
                {
                    pasm("   mov     al, BYTE PTR -%zu[rbp]\n", right->ptr);
                    pasm("   mov     BYTE PTR -%zu[rbp], al /* assign  %s */\n", left->ptr, left->name);
                }
                else if (right->c)
                    pasm("   mov     BYTE PTR -%zu[rbp], %cl\n", left->ptr, right->c);
                else
                    pasm("   mov     BYTE PTR -%zu[rbp], %d /* assign  %s */\n", left->ptr, right->bool_, left->name);
                break;
            }
            case array_:
            {
                // TODO: check if left already has value / check right also
                // having different sizes may causes problem
                size_t len = 0;
                if (right->ptr)
                {
                    pasm("   lea     rax, QWORD PTR -%zu[rbp] /* assign to %s */\n", right->ptr, left->name);
                    pasm("   mov     QWORD PTR -%zu[rbp], rax\n", left->ptr);
                }
                else
                    err("handle this one ");
                break;
            }
            default:
                err("add assembly for this one ");
                break;
            }
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
        if (left->type != right->type && !(
                                             (left->type == char_ && right->type == int_) ||
                                             (left->type == int_ && right->type == char_)))
            err("Uncompatible type in math operation");
        node->token->type = left->type;
        // has no name (optimization)
        if (!left->ptr && !right->ptr)
        {
            debug("0. do %t between %k with %k\n", type, left, right);
            // left->index_ = 0;
            // right->index_ = 0;
            switch (node->token->type)
            {
            case char_:

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
                        err("can't devide by 0 (int)");
                    node->token->int_ = left->int_ / right->int_;
                }
                break;
            case float_:
                left->float_index_ = 0;
                right->float_index_ = 0;
                node->token->float_index_ = index_++;
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
                        err("can't devide by 0 (float)");
                    res = l / r;
                }
                node->token->float_ = *(uint32_t *)(&res);
                break;
            case chars_:
                left->chars_index_ = 0;
                right->chars_index_ = 0;
                if (type == add_)
                    node->token->chars_ = strjoin(left->chars_, right->chars_);
                else
                    err("invalid math operation for characters");
                break;
            default:
                err("math operation 0");
                break;
            }
        }
        else
        {
            // TODO: handle for char data type
            debug("1. do %t between %k with %k\n", type, left, right);
            char *str;
            switch (node->token->type)
            {
            case char_:
                // TODO: handle reference
                node->token->ptr = (ptr += 1);
                // set left
                pasm("   mov     al, ");
                if (left->ptr)
                    pasm("BYTE PTR -%zu[rbp]\n", left->ptr);
                else
                    pasm("%ld\n", left->int_);
                char *op = type == add_ ? "add     al, " : type == sub_ ? "sub     al, "
                                                                        : NULL;
                if (!op)
                    err("Invalid operation");
                // set right
                pasm("   %s", op);
                if (right->ptr)
                    pasm("BYTE PTR -%zu[rbp]\n", right->ptr);
                else
                    pasm("%d\n", right->int_);
                pasm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
                break;
            case int_:
                node->token->ptr = (ptr += 8);
                // set left
                pasm("   mov     rax, ");
                if (left->ptr)
                {
                    pasm("QWORD PTR -%zu[rbp]\n", left->ptr);
                    if (left->is_ref)
                        pasm("mov     rax, QWORD PTR[rax]\n", left->ptr);
                }
                else
                    pasm("%d\n", left->int_);
                // set right
                pasm("   %s", type == add_   ? "add     rax, "
                              : type == sub_ ? "sub     rax, "
                              : type == mul_ ? "imul    rax, "
                              : type == div_ ? "cdq\n   mov     rbx, "
                                             : NULL);
                if (right->ptr)
                    pasm("QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    pasm("%d\n", right->int_);
                if (type == div_)
                    pasm("   idiv    rbx\n");
                pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
                break;
            case float_:
                node->token->ptr = (ptr += 4);
                // set left
                pasm("   movss   xmm1, ");
                if (left->ptr)
                    pasm("DWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->float_index_)
                    pasm("DWORD PTR FLT%zu[rip]\n", left->float_index_);
                else
                    err("something went wrong");
                // set right
                pasm("   %s", type == add_   ? "addss   xmm1, "
                              : type == sub_ ? "subss   xmm1, "
                              : type == mul_ ? "mulss   xmm1, "
                              : type == div_ ? "divss   xmm1, "
                                             : NULL);
                if (right->ptr)
                    pasm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->float_index_)
                    pasm("DWORD PTR FLT%zu[rip]\n", right->float_index_);
                else
                    pasm("%zu\n", right->float_);
                pasm("   movss   DWORD PTR -%zu[rbp], xmm1\n", node->token->ptr);
                break;
            case chars_:
                if (type != add_)
                    err("math operation 2");
                node->token->ptr = (ptr += 8);
                if (left->ptr)
                    pasm("   mov     rdi, QWORD PTR -%zu[rbp]\n", left->ptr);
                // else if (left->chars_index_)
                // {
                //     pasm("   lea     rax, STR%zu[rip]\n", left->chars_index_);
                //     pasm("   mov     rdi, rax\n");
                // }
                else
                    err("in char joining 1");

                if (right->ptr)
                    pasm("   mov     rsi, QWORD PTR -%zu[rbp]\n", right->ptr);
                // else if (right->chars_index_)
                // {
                //     pasm("   lea     rax, STR%zu[rip]\n", right->chars_index_);
                //     pasm("   mov     rsi, rax\n");
                // }
                else
                    err("in char joining 2");
                pasm("   call	  _strjoin\n");
                pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);

                break;
            default:
                err("math operation 1");
                break;
            }
        }
        break;
    }
    // logic operators
    case not_:
    {
        Token *left = evaluate(node->left);
        node->token->type = bool_;
        if (left->type != bool_)
            err("Expect boolean value or condition after not");
        if (left->ptr)
        {
            node->token->ptr = ptr++;
            pasm("   mov     al, BYTE PTR -%zu[rbp]\n", left->ptr);
            pasm("   xor     al, 1\n");
            pasm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
        }
        else if (left->c)
        {
            node->token->ptr = ptr++;
            pasm("   mov     BYTE PTR -%zu[rbp], %cl\n", node->token->ptr, left->c);
            pasm("   xor     BYTE PTR -%zu[rbp], 1\n", node->token->ptr);
        }
        else
            node->token->bool_ = !left->bool_;
    }
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
            err("Uncompatible type in logic operation");
        // has no name (optimization)
        if (!left->ptr && !right->ptr)
        {
            debug("0. do %t between %k with %k\n", type, left, right);
            // left->index_ = 0;
            // right->index_ = 0;
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
                left->float_index_ = 0;
                right->float_index_ = 0;
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
            case chars_:
                left->chars_index_ = 0;
                right->chars_index_ = 0;
                if (type == equal_)
                    node->token->bool_ = (strcmp(left->chars_, right->chars_) == 0);
                else if (type == not_equal_)
                    node->token->bool_ = (strcmp(left->chars_, right->chars_) != 0);
                else
                    err("Invalid logic operation on char");
                break;
            default:
                err("logic operation 0");
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
            case int_:
                // set left
                pasm("   mov     rax, ");
                if (left->ptr)
                    pasm("QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    pasm("%d\n", left->int_);

                pasm("   cmp     rax, ");
                if (right->ptr)
                    pasm("QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    pasm("%d\n", right->int_);

                pasm("   %s   al\n",
                     type == equal_             ? "sete "
                     : type == not_equal_       ? "setne"
                     : type == less_than_       ? "setl "
                     : type == less_than_equal_ ? "setle"
                     : type == grea_than_       ? "setg "
                     : type == grea_than_equal_ ? "setge"
                                                : NULL);

#if BOOL_PTR
                pasm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            case char_:
                // set left
                pasm("   mov     al, ");
                if (left->ptr)
                    pasm("BYTE PTR -%zu[rbp]\n", left->ptr);
                else
                    pasm("%d\n", left->int_);

                pasm("   cmp     al, ");
                if (right->ptr)
                    pasm("BYTE PTR -%zu[rbp]\n", right->ptr);
                else
                    pasm("%d\n", right->int_);

                pasm("   %s   al\n",
                     type == equal_             ? "sete "
                     : type == not_equal_       ? "setne"
                     : type == less_than_       ? "setl "
                     : type == less_than_equal_ ? "setle"
                     : type == grea_than_       ? "setg "
                     : type == grea_than_equal_ ? "setge"
                                                : NULL);

#if BOOL_PTR
                pasm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
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
                pasm("   movss   xmm0, ");
                if (left->ptr)
                    pasm("DWORD PTR -%zu[rbp]\n", left->ptr);
                else if (left->float_index_)
                    pasm("DWORD PTR FLT%zu[rip]\n", left->float_index_);
                else
                    pasm("%zu\n", left->float_);

                pasm("   %s xmm0, ", type != equal_ ? "comiss " : "ucomiss");
                if (right->ptr)
                    pasm("DWORD PTR -%zu[rbp]\n", right->ptr);
                else if (right->float_index_)
                    pasm("DWORD PTR FLT%zu[rip]\n", right->float_index_);
                else
                    pasm("%zu\n", right->float_);

                pasm("   %s   al\n",
                     type == grea_than_         ? "seta "
                     : type == grea_than_equal_ ? "setnb"
                     : type == equal_           ? "setnp"
                     : type == not_equal_       ? "setp "
                                                : NULL);
                if (type == equal_)
                {
                    pasm("   mov	  edx, 0\n");
                    pasm("   movss	  xmm0, ");
                    if (left->ptr)
                        pasm("DWORD PTR -%zu[rbp]\n", left->ptr);
                    else if (left->float_index_)
                        pasm("DWORD PTR FLT%zu[rip]\n", left->float_index_);
                    else
                        pasm("%zu\n", left->float_);
                    pasm("   ucomiss xmm0, ");
                    if (right->ptr)
                        pasm("DWORD PTR -%zu[rbp]\n", right->ptr);
                    else if (right->float_index_)
                        pasm("DWORD PTR FLT%zu[rip]\n", right->float_index_);
                    else
                        pasm("%zu\n", right->float_);
                    pasm("   cmovne  eax, edx\n");
                }
#if BOOL_PTR
                pasm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            case chars_:
                if (type != equal_)
                    err("logic operation 3");
                if (left->ptr)
                    pasm("   mov     rsi, QWORD PTR -%zu[rbp]\n", left->ptr);
                // else if (left->chars_index_)
                // {
                //     pasm("   lea     rax, STR%zu[rip]\n", left->chars_index_);
                //     pasm("   mov     rsi, rax\n");
                // }
                else
                    err("in char equal");

                if (right->ptr)
                    pasm("   mov     rdi, QWORD PTR -%zu[rbp]\n", right->ptr);
                // else if (right->chars_index_)
                // {
                //     pasm("   lea     rax, STR%zu[rip]\n", right->chars_index_);
                //     pasm("   mov     rdi, rax\n");
                // }
                else
                    err("in char equal 2");
                pasm("   call	  _strcmp\n");
#if BOOL_PTR
                pasm("   mov     BYTE PTR -%zu[rbp], al\n", node->token->ptr);
#endif
                break;
            default:
                err("logic operation 4");
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
                err("0.Expected boolean value");

            pasm("   /* %s operation %d */\n", tts(node->token->type), i++);
            if (left->ptr)
                pasm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
            else if (left->c)
                pasm("   cmp     %cl, 1\n", left->c);
            else
            {
                pasm("   mov     al, %d\n", left->bool_);
                pasm("   cmp     al, 1\n");
            }
            if (tmp->token->type == or_)
                pasm("   je      %s%zu\n", LABEL->name, node->token->label_index_);
            else if (tmp->token->type == and_)
                pasm("   jne     %s%zu\n", LABEL->name, node->token->label_index_);
            tmp = tmp->right;
            if (tmp->token->type == and_ || tmp->token->type == or_)
                pasm("%s%zu:\n", LABEL->name, tmp->token->label_index_);
        }
        right = evaluate(tmp);
        if (right->type != bool_)
            err("0.Expected boolean value");
        pasm("%s%zu:\n", LABEL->name, node->token->label_index_);
        node->token->c = 'a';
        node->token->type = bool_;
        break;
    }
    case if_:
    {
        enter_label(node);
        Node *curr = node->left;
        size_t end_index = node->token->label_index_ - 1;
        left = evaluate(curr->left);
        if (left->type != bool_)
            err("Expected a valid condition in if statement");

        pasm("%s%zu: /* if statement */\n", LABEL->name, node->token->label_index_);
        if (left->ptr)
            pasm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
        else if (left->c)
            pasm("   cmp     %cl, 1\n", left->c);
        else
        {
            pasm("   mov     al, %d\n", left->bool_);
            pasm("   cmp     al, 1\n");
        }
        if (node->right)
            pasm("   jne     %s%zu    /* jmp next statement */\n", LABEL->name, node->right->token->label_index_);
        else
            pasm("   jne     %s%zu    /* jmp end statemnt */\n", LABEL->name, end_index);
        curr = curr->right;
        // if statment bloc
        while (curr)
        {
            evaluate(curr->left);
            curr = curr->right;
        }
        curr = node->left;
        if (node->right)
            pasm("   jmp     %s%zu    /* jmp end statement */\n", LABEL->name, end_index);

        // elif / else statement
        curr = node->right;
        while (curr)
        {
            if (curr->token->type == elif_)
            {
                Node *tmp0 = curr->left;
                // evaluate elif
                pasm("%s%zu: /* elif statement */\n", LABEL->name, curr->token->label_index_);
                Node *tmp = tmp0;

                left = evaluate(tmp->left);
                if (left->type != bool_)
                    err("Expected a valid condition in elif statement");
                if (left->ptr)
                    pasm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
                else if (left->c)
                    pasm("   cmp     %cl, 1\n", left->c);
                else
                {
                    pasm("   mov     al, %d\n", left->bool_);
                    pasm("   cmp     al, 1\n");
                }
                if (curr->right)
                    pasm("   jne     %s%zu /* jmp next statement */\n", LABEL->name, curr->right->token->label_index_);
                else
                    pasm("   jne     %s%zu /* jmp end statemnt */\n", LABEL->name, end_index);
                tmp = tmp->right;
                while (tmp)
                {
                    evaluate(tmp->left);
                    tmp = tmp->right;
                }
                if (curr->right)
                    pasm("   jmp     %s%zu /* jmp end statement */\n", LABEL->name, end_index);
            }
            else if (curr->token->type == else_)
            {
                Node *tmp = curr->left->right;
                pasm("%s%zu: /* else statement */\n", LABEL->name, curr->token->label_index_);
                while (tmp)
                {
                    evaluate(tmp->left);
                    tmp = tmp->right;
                }
            }
            curr = curr->right;
        }
        // end statement bloc
        pasm("%s%zu: /* end statement */\n", LABEL->name, end_index);
        exit_label(node);
        break;
    }
    case while_:
    {
        enter_label(node);
        Node *curr = node->left;
        pasm("   jmp     %s%zu /* jmp to while condition*/\n", LABEL->name, node->token->label_index_);
        // while bloc
        pasm("%s%zu: /* while bloc*/\n", LABEL->name, node->token->label_index_ + 1);
        Node *tmp = curr->right;
        while (tmp)
        {
            evaluate(tmp->left);
            tmp = tmp->right;
        }
        // while condition
        pasm("%s%zu: /* while condition */\n", LABEL->name, node->token->label_index_);
        left = evaluate(curr->left);
        if (left->type != bool_)
            err("Expected a valid condition in if statment");
        if (left->ptr)
            pasm("   cmp     BYTE PTR -%zu[rbp], 1\n", left->ptr);
        else if (left->c)
            pasm("   cmp     %cl, 1\n", left->c);
        else
        {
            pasm("   mov     al, %d\n", left->bool_);
            pasm("   cmp     al, 1\n");
        }
        pasm("   je      %s%zu /* je to while bloc*/\n", LABEL->name, node->token->label_index_ + 1);
        pasm("%s%zu: /* end while */\n", LABEL->name, node->token->label_index_ + 2);
        exit_label(node);
        break;
    }
    case func_call_:
    {
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
            Node *dec = NULL;
            Node *call = node;
            char *name = strjoin(call->token->name, "_");
            if (!(dec = get_func(name)))
            {
                visualize();
                err("Undeclared function '%s'\n", call->token->name);
            }
            call->token->type = dec->token->ret_type;
            if (dec->left)
            {

                // push arguments:
                int stack_len = 100;
                int stack_pos = 0;
                Token **stack = calloc(stack_len, sizeof(Token *));

                Node *d_tmp = dec->left;
                Node *c_tmp = call->left;

                // check arguments conptability
                while (d_tmp && c_tmp)
                {
                    c_tmp->left->token = evaluate(c_tmp->left);
                    if (d_tmp->left->token->type != c_tmp->left->token->type)
                        err("Uncompatible type in function call");
                    Token *token = c_tmp->left->token;

                    stack[stack_pos] = calloc(1, sizeof(Token));
                    memcpy(stack[stack_pos], c_tmp->left->token, sizeof(Token));
                    if (d_tmp->left->token->is_ref && c_tmp->left->token->is_ref)
                        stack[stack_pos]->is_ref = false;
                    else
                    {
                        stack[stack_pos]->is_ref = d_tmp->left->token->is_ref;
                        stack[stack_pos]->has_ref = true;
                    }
                    stack_pos++;
                    if (stack_pos + 2 > stack_len)
                    {
                        stack = realloc(stack, 2 * stack_len * sizeof(Token *));
                        stack_len *= 2;
                    }
                    d_tmp = d_tmp->right;
                    c_tmp = c_tmp->right;
                }
                if (d_tmp)
                    err("'%s' require arguments", call->token->name);
                stack_pos--;

                // push arguments
                c_tmp = call->left;
                while (stack_pos >= 0)
                {
                    // declaration stack
                    Token *token = stack[stack_pos];
                    if (token->is_ref)
                    {
                        if (!token->ptr)
                            err("Expected reference");
                        pasm("   lea     rax, -%zu[rbp]\n", token->ptr);
                        pasm("   push    rax /*ref %s*/\n", token->name);
                    }
                    else if (token->ptr)
                        pasm("   push    QWORD PTR -%zu[rbp] /*%s*/\n", token->ptr, token->name);
                    else
                    {
                        switch (token->type)
                        {
                        case chars_:
                            pasm("   lea     rax, STR%zu[rip]\n", token->chars_index_);
                            pasm("   mov     rdi, rax\n");
                            pasm("   call    _strdup\n");
                            break;
                        case int_:
                            pasm("   mov     rax, %ld\n", token->int_);
                            break;
                        case char_:
                            pasm("   mov     al, %ld\n", token->int_);
                            break;
                        default:
                            err("handle this case");
                            break;
                        }
                        pasm("   push    rax\n");
                    }
                    free(stack[stack_pos]);
                    stack_pos--;
                    c_tmp = c_tmp->right;
                }
                free(stack);
            }
            if (dec->token->ret_type != void_)
            {
                debug("function return in call\n");
                call->token->ptr = (ptr += 8);
                pasm("   mov     QWORD PTR -%zu[rbp], 0 /*%s result*/\n", call->token->ptr, call->token->name);
                pasm("   lea     rax, -%zu[rbp]\n", call->token->ptr);
            }
            pasm("   call    %s\n", dec->token->name);
            free(name);
            call->token->ret_type = dec->token->ret_type;
        }

        break;
    }
    case func_dec_:
    {
        new_func(node);
        char *name = node->token->name;
        enter_label(node);
        pasm("%s:\n", name);
        size_t tmp_rbp = ptr;
        ptr = 0;
        pasm("   push    rbp\n");
        pasm("   mov     rbp, rsp\n");
        pasm("   sub     rsp, %zu\n", rsp);
        if (node->token->ret_type != void_)
        {
            debug("function does return (%t)\n", node->token->ret_type);
            // TODO: depends on return type define return ptr
            node->token->ptr = (tmp_rbp += 8);
            ptr += 8;
            pasm("   mov     QWORD PTR -%zu[rbp], rax\n", node->token->ptr);
        }

        // arguments
        size_t tmp_ptr = 16;
        Node *tmp = node->left;
        while (tmp)
        {
            Token *token = evaluate(tmp->left);
            if (token->is_ref)
                token->has_ref = true;
            pasm("   mov	  rax, QWORD PTR %zu[rbp]\n", tmp_ptr);
            switch (token->type)
            {
            case int_:
            case chars_:
            case array_:
                pasm("   mov     QWORD PTR -%zu[rbp], rax\n", token->ptr);
                break;
            case char_:
                pasm("   mov     BYTE PTR -%zu[rbp], al\n", token->ptr);
                break;
            default:
                err("handle this case");
                break;
            }
            tmp = tmp->right;
            tmp_ptr += 8;
        }

        // code bloc
        skip_check = true;
        tmp = node->right;
        while (tmp)
        {
            if (tmp->left->token->type == func_dec_)
                pasm("   jmp     end_%s%s\n", LABEL->name, tmp->left->token->name);
            if (tmp->left->token->type == return_)
            {
                debug("found return\n");
                Token *token = evaluate(tmp->left->left);
                if (token->ptr) // TODO: this line may cause problem
                {
                    pasm("   mov	  rax, QWORD PTR -%zu[rbp]\n", token->ptr);
                    pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", node->token->ptr);
                    pasm("   mov     QWORD PTR [rbx], rax\n");
                }
                else
                {
                    switch (token->type)
                    {
#if 0
    case chars_:
    pasm("   lea     rax, STR%zu[rip]\n", token->chars_index_);
    pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", node->token->ptr);
    pasm("   mov     QWORD PTR [rbx], rax\n");
    break;
#endif
                    case int_:
                        pasm("   mov     rax, %ld\n", token->int_);
                        pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", node->token->ptr);
                        pasm("   mov     QWORD PTR [rbx], rax\n");
                        break;
                    case bool_:
                        if (token->c)
                            pasm("   mov     al, %cl\n", token->c);
                        else
                            pasm("   mov     al, %ld\n", token->bool_);
                        pasm("   mov     rbx, QWORD PTR -%zu[rbp]\n", node->token->ptr);
                        pasm("   mov     BYTE PTR [rbx], al\n");
                        break;
                    case array_:
                        err("handle this one ");
                        break;
                    default:
                        err("handle this one ");
                        break;
                    }
                }
            }
            else
                evaluate(tmp->left);
            tmp = tmp->right;
        }
        skip_check = false;
        pasm("   leave\n");
        pasm("   ret\n");
        pasm("end_%s:\n\n", name);
        exit_label(node);
        ptr = tmp_rbp;
        break;
    }
    case continue_:
    case break_:
    {
        int lb_tmp = lb_pos;
        while (lb_tmp >= 0)
        {
            // TODO: to be check ther might be function that starts with 'while' as name
            if (strncmp(LABELS[lb_tmp]->name, "while", strlen("while")) == 0)
            {
                if (node->token->type == break_)
                    pasm("   jmp      %s%zu /* break while*/\n", LABELS[lb_tmp]->name,
                         LABELS[lb_tmp]->node->token->label_index_ + 2);
                else if (node->token->type == continue_)
                    pasm("   jmp      %s%zu /* continue while*/\n", LABELS[lb_tmp]->name,
                         LABELS[lb_tmp]->node->token->label_index_);
                break;
            }
            lb_tmp--;
        }
        if (lb_tmp == 0)
            err("Expected a loop to do %s\n", tts(node->token->type));
        break;
    }
    default:
        err("in evaluate %t", type);
        break;
    }
    if (ptr + 10 > rsp)
    {
        pasm("   sub     rsp, %zu\n", rsp * 2);
        rsp = rsp + rsp * 2;
    }
    return node->token;
}

int main(int argc, char **argv)
{
    if (argc != 2 || strlen(argv[1]) < 3 || strcmp(argv[1] + strlen(argv[1]) - 2, ".w"))
        err("require file.w as argument\n");
    file = fopen(argv[1], "r");

    argv[1][strlen(argv[1]) - 1] = 's';
    asm_fd = fopen(argv[1], "w+");
    if (!file || !asm_fd)
        err("Opening file");
    fseek(file, 0, SEEK_END);
    size_t txt_len = ftell(file);
    if (!(text = calloc(txt_len + 1, sizeof(char))))
        err("Allocation");
    fseek(file, 0, SEEK_SET);
    fread(text, txt_len, sizeof(char), file);
    fclose(file);
    debug("%s\n\n", text);
    Label_index = 1;
    index_ = 1;

    build_tokens();
    debug("\n");
    free(text);
    rsp = 100;
    initialize();
    finalize();
    debug("\nresult: (exiting with %d)\n", EXIT_STATUS);
    exit(EXIT_STATUS);
}