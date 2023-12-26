#include "utils.c"

Token *get_var(char *name)
{
    for (int i = 0; i < var_pos; i++)
        if (variables[i]->name && strcmp(variables[i]->name, name) == 0)
            return variables[i];
    return NULL;
}

Token *evaluate(Node *node)
{
    Token *left, *right, *to_find;
    Type type = node->token->type;
    switch (type)
    {
    case identifier_:
    case char_:
    case float_:
    case int_:
        break;
    case assign_:
    {
        // TODO: assign / initializing
        // TODO: deep / shalow copy
        left = evaluate(node->left);
        dprintf(asm_fd, "   /* assign %s */\n", left->name);
        right = evaluate(node->right);
        debug("assign %k and %k \n", left, right);
        if (!left->name || left->type != right->type)
            error("Invalid assignement");
        node->token = left;
        switch (left->type)
        {
        case int_:
            dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], ", left->ptr);
            if (right->ptr)
                dprintf(asm_fd, "QWORD PTR -%zu[rbp]\n", right->ptr);
            else
                dprintf(asm_fd, "%d\n", right->int_);
            break;
#if 0
            case float_:
                // TODO: check xmms, with multiple variables
                dprintf(asm_fd, "   movss   xmm1, DWORD PTR .FLT%zu[rip]\n", right->index_);
                dprintf(asm_fd, "   movss   DWORD PTR -%zu[rbp], xmm1\n", to_find->ptr);
                break;
            case char_:
                dprintf(asm_fd, "   lea     rax, .STR%zu[rip]\n", right->index_);
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], rax\n", to_find->ptr);
                break;
#endif
        default:
            error("add assembly for this one 0");
            break;
        }
        break;
    }
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        /*
            allocate in stack
            add left value
            and right value
        */
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != right->type)
            error("Uncompatible type in math operation");
        node->token->type = left->type;
        // has no name // optimization
        if (!left->ptr && !right->ptr)
        {
            debug("0. do %s between %k with %k\n", type_to_string(type), left, right);
            switch (node->token->type)
            {
            case int_:
                // node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
                if (type == add_)
                    node->token->int_ = left->int_ + right->int_;
                else if (type == sub_)
                    node->token->int_ = left->int_ - right->int_;
                else if (type == mul_)
                    node->token->int_ = left->int_ * right->int_;
                else if (type == div_)
                {
                    if (right->int_ == 0)
                        error("can't devide by 0 (int)");
                    node->token->int_ = left->int_ / right->int_;
                }
                break;
            case float_:
                node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
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
                        error("can't devide by 0 (float)");
                    res = l / r;
                }
                node->token->float_ = *(uint32_t *)(&res);
                break;
            case char_:
                node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
                if (type == add_)
                    node->token->char_ = strjoin(left->char_, right->char_);
                else
                    error("invalid operation for characters");
                break;
            default:
                error("math operation 0");
                break;
            }
        }
        else
        {
            debug("1. do %s between %k with %k\n", type_to_string(type), left, right);
            switch (node->token->type)
            {
            case int_:
                node->token->ptr = (ptr += 4);
                // set left
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], ", node->token->ptr);
                if (left->ptr)
                    dprintf(asm_fd, "QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    dprintf(asm_fd, "%d\n", left->int_);

                // set right
                dprintf(asm_fd, "   add     QWORD PTR -%zu[rbp], ", node->token->ptr);
                if (right->ptr)
                    dprintf(asm_fd, "QWORD PTR -%zu[rbp]\n", right->ptr);
                else
                    dprintf(asm_fd, "%d\n", right->int_);
                break;
            default:
                error("math operation 1");
                break;
            }
        }
        break;
    }
    case func_call_:
    {
        debug("found function call has name '%s'\n", node->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            debug("found output\n");
            output(evaluate(node->left));
        }
        break;
    }
    default:
        break;
    }
    if (ptr + 10 > rsp)
    {
        // TODO: protect this line from being printed in wrong place
        // after label for example
        rsp += 30;
        dprintf(asm_fd, "   sub     rsp, 30\n");
    }
    return node->token;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        // TODO: check if file ends with .hr
        error("require one file.hr as argument\n");
    }
#if 1
    file = fopen(argv[1], "r");
#else
    file = fopen("file.hr", "r");
#endif

#if 0
    char *filename = strdup(argv[1]);
    filename[strlen(filename) - 2] = 's';
    filename[strlen(filename) - 1] = 0;
    
    // opening file
    debug("open %s\n", argv[1]);
    debug("write to %s\n", filename);
    asm_fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0777);
    free(filename);
#else
    asm_fd = open("file.s", O_CREAT | O_TRUNC | O_RDWR, 0777);
#endif
    if (file == NULL || asm_fd < 0)
        error("Opening file");
    fseek(file, 0, SEEK_END);
    txt_len = ftell(file);
    if (!(text = calloc(txt_len + 1, sizeof(char))))
        error("Allocation");
    fseek(file, 0, SEEK_SET);
    fread(text, txt_len, sizeof(char), file);
    fclose(file);
    debug("%s\n\n", text);
    index_ = 1;
    tk_len = var_len = 100;
    tokens = calloc(tk_len, sizeof(Token *));
    variables = calloc(tk_len, sizeof(Token *));
    build_tokens();
    debug("\n");
    free(text);
    tk_pos = 0;
    // TODO: verify rsp position
    rsp = 30;
    Node *node = initialize();
    finalize(node);
    debug("\n");
}