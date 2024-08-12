#include "ir.c"

char sign(Token *token)
{
    return '-';
}

void generate_asm()
{
    for (int i = 0; insts[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;
        switch (curr->type)
        {
        case assign_:
        {
            /*
                TODOS:
                    + check incompatible type
                    + left should be always an identifier
            */
            curr->ptr = left->ptr;
            pasm("/*assign_node %s*/\n", left->name);
            if (right->ptr)
            {
                mov("rax, QWORD PTR %c%zu[rbp]\n", sign(right), right->ptr);
                mov("QWORD PTR %c%zu[rbp], rax\n", sign(left), left->ptr);
            }
            else if(right->c)
                mov("QWORD PTR %c%zu[rbp], r%cx\n", sign(left), left->ptr, right->c);
            else
            {
                switch (right->type)
                {
                case int_:
                    mov("QWORD PTR %c%zu[rbp], %lld\n", sign(left), left->ptr, right->Int.value);
                    break;
                case bool_:
                    mov("BYTE PTR %c%zu[rbp], %d\n", sign(left), left->ptr, right->Bool.value);
                    break;
                case chars_:
                    lea("rax, .STR%zu[rip]\n", right->index);
                    mov("QWORD PTR %c%zu[rbp], rax\n", sign(left), left->ptr);
                    break;
                case float_:
                    movss("xmm0, DWORD PTR .FLT%zu[rip]\n", right->index);
                    movss("DWORD PTR %c%zu[rbp], xmm0\n", sign(left), left->ptr);
                    break;
                case char_:
                    // mov     BYTE PTR [rbp-1], 97
                    mov("BYTE PTR %c%zu[rbp], %d\n", sign(left), left->ptr, right->Char.value);
                    break;
                default:
                    error("handle this case in %s:%d\n", FUNC, LINE);
                    break;
                }
            }
            break;
        }
        case push_:
        {
            if(right->name) // destination
            {
                if(left->ptr)
                    mov("%s, QWORD PTR -%zu[rbp]\n", right->name, left->ptr);
                else if(left->c)
                {
                    // debug("hey"); exit(1);
                    push("r%cx\n", left->c);
                }
                else
                    switch(left->type)
                    {
                    // TODO: handle reference, built ins must take reference
                    case int_: mov("%s, %lld\n", right->name, left->Int.value); break;
                    case chars_:
                    {
                        lea("rax, .STR%zu[rip]\n", left->index);
                        // call(".strdup");
                        mov("%s, rax\n", right->name);
                        break;
                    }
                    default:
                    {
                        RLOG(FUNC, "%d: handle this case <%s>", LINE, to_string(left->type));
                        exit(1);
                    }
                    }
            }
            else
            {
                if(left->ptr) // TODO: test calling function inside function
                    push("QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                    switch(left->type)
                    {
                    case int_: push("%lld\n", left->Int.value); break;
                    default: RLOG(FUNC, "%d: handle this case", LINE); exit(1);
                    }
            }
            break;
        }
        case pop_:
        {
            if(right->name) // source
            {
                // if(left->ptr)
                mov("QWORD PTR -%zu[rbp], %s\n", left->ptr, right->name);
                // else
                //     switch(left->type)
                //     {
                //     // TODO: handle reference, built ins must take reference
                //     case int_: mov("%s, %lld\n", right->name, left->Int.value); break;
                //     case chars_:
                //     {
                //         lea("rdi, .STR%zu[rip]\n", left->index);
                //         call(".strdup");
                //         mov("%s, rax\n", right->name);
                //         break;
                //     }
                //     default: RLOG(FUNC, "%d: handle this case", LINE); exit(1);
                //     }
            }
            else
            {
                mov("rdi, QWORD PTR %zu[rbp]\n", right->ptr);
                mov("QWORD PTR -%zu[rbp], rdi\n", left->ptr);
                // if(left->ptr) // TODO: test calling function inside function
                // {

                // }
                //     push("QWORD PTR -%zu[rbp]\n", left->ptr);
                // else
                //     switch(left->type)
                //     {
                //     case int_: push("%lld\n", left->Int.value); break;
                //     default: RLOG(FUNC, "%d: handle this case", LINE); exit(1);
                //     }
            }
            break;
        }
        case int_: case bool_: case chars_:
        {
            if (curr->declare)
            {
                // if(curr->isarg)
                //     pasm("/*arg %s in %zu[rbp] */\n", curr->name, curr->ptr + 8);
                // else
                {
                    pasm("/*declare %s*/\n", curr->name);
                    mov("QWORD PTR %c%zu[rbp], 0\n", sign(curr), curr->ptr);
                }
            }
            else
            {
                if(curr->type == int_)
                    mov("rax, %ld\n", curr->Int.value);
                // if(curr->isarg) // TODO: to be checked
                //     push("%ld\n", curr->Int.value);
                // if(curr->type == chars_ && !curr->name)
                    ; // pasm(".STR%zu: .string %s\n", curr->index, curr->Chars.value);
            } 
            break;
        }
        case add_: case sub_: case mul_: case div_: // TODO: check all math_op operations
        {
            curr->c = 'a';
            if (left->ptr)
                mov("r%cx, QWORD PTR %c%zu[rbp]\n", curr->c, sign(left), left->ptr);
            else if (left->c && left->c != curr->c)
                mov("r%cx, r%cx\n", curr->c, left->c);
            else if (!left->c)
                mov("r%cx, %lld\n", curr->c, left->Int.value);

            if (right->ptr)
                math_op(curr->type, "r%cx, QWORD PTR %c%zu[rbp]\n", curr->c, sign(right), right->ptr);
            else if (right->c) // TODO: to be checked
                math_op(curr->type, "r%cx, r%cx\n", curr->c, right->c);
            else if (!right->c)
                math_op(curr->type, "r%cx, %lld\n", curr->c, right->Int.value);
            break;
        }
        case equal_: case not_equal_: case less_:
        case less_equal_: case more_: case more_equal_:
        {
            curr->c = 'a';
            if (left->ptr)
                mov("r%cx, QWORD PTR %c%zu[rbp]\n", curr->c, sign(left), left->ptr);
            else if (left->c && left->c != curr->c)
                mov("r%cx, r%cx\n", curr->c, left->c);
            else if (!left->c)
                mov("r%cx, %lld\n", curr->c, left->Int.value);

            if (right->ptr)
                mov("rbx, QWORD PTR %c%zu[rbp]\n", sign(right), right->ptr);
            else if (right->c && right->c != 'b') // TODO: to be checked
                mov("rbx, r%cx\n", right->c);
            else if (!right->c)
                mov("rbx, %lld\n", right->Int.value);
            cmp("rax, rbx\n","");
            relational_op(curr->type, "%cl\n", curr->c);
            curr->type = bool_;
            break;
        }
        case fcall_:
        {
            curr->c = 'a';
            call(curr->name); 
            break;
        }
        case fdec_:
        {
            pasm("%s:\n", curr->name);
            // if(strcmp(curr->name, "main") == 0)
            {
                push("rbp\n", "");
                mov("rbp, rsp\n","");
                pasm("sub     rsp, %zu\n", (((ptr) + 15) / 16) * 16);
            }
            break;
        }
        case jne_:
        {
            cmp("al, 1\n", "");
            jne(".%s%zu\n", curr->name, curr->index);
            break;
        }
        case jmp_:
        {
            jmp(".%s%zu\n", curr->name, curr->index);
            break;
        }
        case bloc_:
        {
            pasm(".%s%zu:\n", curr->name, curr->index);
            break;
        }
        case end_bloc_:
        {
            pasm(".end%s:\n", curr->name);
            break;
        }
        case ret_:
        {
            if(left)
            {
                if(left->ptr)
                    mov("rax, QWORD PTR -%zu[rbp]\n", left->ptr);
                else
                {
                    switch(left->type)
                    {
                        case int_: mov("rax, %ld\n", left->Int.value); break;
                        default: RLOG(FUNC, ":%d handle this case\n", LINE);
                    }
                }
            }
            pasm("leave\n");
            pasm("ret\n");
            break;
        }
        default:
            debug("%sGenerate asm: Unkown Instruction [%s]%s\n", 
                RED, to_string(curr->type), RESET);
            break;
        }
    }
}

void initialize()
{
    pasm(".intel_syntax noprefix\n");
    pasm(".include \"./import/header.s\"\n\n");
    pasm(".text\n");
    pasm(".globl	main\n");   
}

void finalize()
{
    // mov("rax, 0\n", "");
    // pasm("leave\n");
    // pasm("ret\n");
#if 1
    for (int i = 0; tokens[i]; i++)
    {
        Token *curr = tokens[i];
        // test char variable before making any modification
        if (curr->type == chars_ && !curr->name && !curr->ptr && curr->index)
            pasm(".STR%zu: .string %s\n", curr->index, curr->Chars.value);
        if (curr->type == float_ && !curr->name && !curr->ptr && curr->index)
            pasm(".FLT%zu: .long %zu /* %f */\n", curr->index, 
                *((unsigned int *)(&curr->Float.value)), curr->Float.value);
    }
#endif
    pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
}

/*
    TODOS:
        + don't exit till you compile full file
        + function if doesn't have a return, set 0 as default 
          return for in for example
*/

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        error("expected file as argument\n");
        exit(1);        
    }
    char *input = open_file(argv[1]);
    char *outputFile = strdup(argv[1]);
    outputFile[strlen(outputFile) - 1] = 's';
    asm_fd = fopen(outputFile, "w+");
    if(asm_fd == NULL)
    {
        error("openning %s\n", outputFile);
        exit(1);
    }
    free(outputFile);

#if TOKENIZE
    tokenize(input);
    for (int i = 0; i < tk_pos; i++)
        ptoken(tokens[i]);
    debug(SPLIT);
#endif
    create_builtin("write", (Type[]){int_, chars_, int_, 0}, int_);
    create_builtin("read", (Type[]){int_, chars_, int_, 0}, int_);
    create_builtin("exit", (Type[]){int_, 0}, int_);
    create_builtin("malloc", (Type[]){int_, 0}, ptr_);
    create_builtin("calloc", (Type[]){int_, int_, 0}, ptr_);

    Node *head = NULL;
#if AST
    head = new_node(NULL);
    Node *curr = head;
    curr->left = expr_node();
    while (curr->left)
    {
        curr->right = new_node(NULL);
        curr = curr->right;
        curr->left = expr_node();
    }
    curr = head;
    while (curr->left)
    {
        pnode(curr->left, NULL, 0);
        curr = curr->right;
    }
    if (tokens)
    {
#if IR
        enter_scoop("");
        curr = head;
        while (curr->left)
        {
            generate_ir(curr->left);
            curr = curr->right;
        }
        exit_scoop();
        insts = copy_insts(first_insts, insts, inst_pos, inst_size);
        print_ir();

#if OPTIMIZE
        int i = 0;
        bool optimized = false;
        while(i < MAX_OPTIMIZATION)
        {
            optimized = optimize_ir(i++) || optimized;
            insts = copy_insts(first_insts, insts, inst_pos, inst_size);
            print_ir();
            if(i == MAX_OPTIMIZATION && optimized)
            {
                optimized = false;
                i = 0;
            }
        }
#endif

#endif

#if ASM
        initialize();
        generate_asm();
        finalize();
#endif
    }
#endif
    clear(head, tokens, first_insts, input);
#if ASM
    return 0;
#else
    return 1;
#endif
}