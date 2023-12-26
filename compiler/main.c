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
    case int_:
    case float_:
        return node->token;
    case assign_:
    {
        // TODO: assign / initializing
        // TODO: deep / shalow copy
        left = evaluate(node->left);
        right = evaluate(node->right);
        debug("assign %k and %k \n", left, right);
        if (!left->name || left->type != right->type)
            error("Invalid assignement");
        else
            to_find = left;
        // if (left->type == identifier_)
        //     to_find->index_ = right->index_;
        /*
        TODO:
            check if right has ptr
                + load value from ptr
            else
                + mov value
        */
        if (right->ptr)
        {
            switch (to_find->type)
            {
            case int_:
                // printf("%ld", node->right->token->int_);
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], QWORD PTR -%zu[rbp]\n", to_find->ptr, right->ptr);
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
        }
        else
        {
            switch (to_find->type)
            {
            case int_:
                // printf("%ld", node->right->token->int_);
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], %ld\n", to_find->ptr, right->int_);
                break;
            case float_:
                // TODO: check xmms, with multiple variables
                dprintf(asm_fd, "   movss   xmm1, DWORD PTR .FLT%zu[rip]\n", right->index_);
                dprintf(asm_fd, "   movss   DWORD PTR -%zu[rbp], xmm1\n", to_find->ptr);
                break;
            case char_:
                dprintf(asm_fd, "   lea     rax, .STR%zu[rip]\n", right->index_);
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], rax\n", to_find->ptr);
                break;
            default:
                error("add assembly for this one 1");
                break;
            }
        }
        break;
    }
    case add_:
    case sub_:
    case mul_:
    case div_:
    {
        left = evaluate(node->left);
        right = evaluate(node->right);
        if (left->type != right->type)
            error("Uncompatible type in math operation");
        node->token->type = left->type;
        if (!left->name && !right->name)
        {
            if (left->type == identifier_ && !(left = get_var(left->name)))
                error("Undeclared variable, left");
            if (right->type == identifier_ && !(right = get_var(right->name)))
                error("Undeclared variable, right");
            debug("do %s between %k with %k\n", type_to_string(node->token->type), left, right);
            // set label index
            node->token->index_ = left->index_ < right->index_ ? left->index_ : right->index_;
            left->index_ = 0;
            right->index_ = 0;
            switch (left->type)
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
                        error("can't devide by 0 (int)");
                    node->token->int_ = left->int_ / right->int_;
                }
                break;
            case float_:
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
                left->index_ = 0;
                right->index_ = 0;
                if (type == add_)
                    node->token->char_ = strjoin(left->char_, right->char_);
                else
                    error("invalid operation for characters");
                break;
            default:
                error("math operation 0");
                break;
            }
            return node->token;
        }
        else if (!right->name)
        {
            debug("do %s between %k with %k\n", type_to_string(node->token->type), left, right);
            /*
            + left is variable
            + right is a number
            + TODO: check overflow
            + allocate new space in stack frame
            + mov value pointer by left
            + add value of right
            */
            switch (left->type)
            {
            case int_:
                node->token->ptr = (ptr += 4);
                dprintf(asm_fd, "   mov     QWORD PTR -%zu[rbp], QWORD PTR -%zu[rbp]\n", node->token->ptr, left->ptr);
                dprintf(asm_fd, "   add     QWORD PTR -%zu[rbp], %d\n", node->token->ptr, right->int_);
                break;
#if 0
            case float_:
                node->token->ptr = (ptr += 4);
                dprintf(asm_fd, "   movss   xmm1, QWORD PTR -%zu[rbp]\n", left->ptr);
                dprintf(asm_fd, "   addss   xmm1, QWORD PTR -%zu[rbp]\n", right->float_);
                dprintf(asm_fd, "   movss   QWORD PTR -%zu[rbp], xmm1\n", node->token->ptr);
                break;
            case char_:
                // node->token->ptr = (ptr += 8);
                // break;
#endif
            default:
                error("math operation 1");
                break;
            }
            return node->token;
        }
        // else if (!left->name)
        // {
        // }
        // else if (left->name && right->name)
        // {
        // }
        else
            error("write the assembly this operation");
        break;
    }
    case func_call_:
    {
        printf("found function call has name '%s'\n", node->token->name);
        if (strncmp("output", node->token->name, strlen("output")) == 0)
        {
            printf("found output\n");
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
    // printf("\n");
    return NULL;
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
    printf("open %s\n", argv[1]);
    printf("write to %s\n", filename);
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
    printf("%s", text);

    index_ = 1;
    tk_len = var_len = 100;
    tokens = calloc(tk_len, sizeof(Token *));
    variables = calloc(tk_len, sizeof(Token *));
    build_tokens();
    free(text);
    tk_pos = 0;
    // TODO: verify rsp position
    rsp = 30;
    Node *node = initialize();
    finalize(node);
}