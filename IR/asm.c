#include "ir.c"

void initialize()
{
    pasm(".intel_syntax noprefix\n");
    pasm(".include \"./import/header.s\"\n\n");
    pasm(".text\n");
    pasm(".globl	main\n");   
}

void finalize()
{
#if TOKENIZE
    for (int i = 0; tokens[i]; i++)
    {
        Token *curr = tokens[i];
        // test char variable before making any modification
        if(curr->type == chars_ && !curr->name && !curr->ptr && curr->index)
            pasm(".STR%zu: .string %s\n", curr->index, curr->Chars.value);
        if(curr->type == float_ && !curr->name && !curr->ptr && curr->index)
            pasm(".FLT%zu: .long %zu /* %f */\n", curr->index,  
            *((uint32_t *)(&curr->Float.value)), curr->Float.value);
        if(curr->creg) free(curr->creg);
    }
    pasm(".section	.note.GNU-stack,\"\",@progbits\n\n");
#endif
}

/*
TODOS:
    + in assign:
        + check incompatible type
        + left should be always an identifier
*/

void generate_asm()
{
    initialize();
    for(size_t i = 0; insts[i]; i++)
    {
        Token *curr = insts[i]->token;
        Token *left = insts[i]->left;
        Token *right = insts[i]->right;

        switch(curr->type)
        {
            case int_: case bool_: case chars_: case char_: case float_:
            {
                if(curr->declare)
                {
                    pasm("//declare [%s]\n", curr->name);
                    pasm("mov %a, 0\n", curr);
                }
                else if(!curr->isarg)
                    error("%s:%d handle this case\n", FUNC, LINE);
                break;
            }
            case assign_:
            {
                curr->ptr = left->ptr;
                pasm("//assign [%s]\n", left->name);
                if(right->ptr || right->creg)
                {
                    char *inst = left->type == float_ ? "movss " : "mov ";
                    if(right->ptr && !right->creg) pasm("%i%r, %a\n", inst, left, right);
                    pasm("%i%a, %r\n", inst, left, left);
                }
                else
                {
                    switch (right->type)
                    {
                    case int_: case bool_: case char_:
                        pasm("mov %a, %v\n", left, right);
                        break;
                    case chars_:
                        pasm("lea rax, .STR%zu[rip]\n", right->index);
                        pasm("mov QWORD PTR -%ld[rbp], rax\n", left->ptr);
                        break;
                    case float_:
                        pasm("movss xmm0, DWORD PTR .FLT%zu[rip]\n", right->index);
                        pasm("movss DWORD PTR -%ld[rbp], xmm0\n", left->ptr);
                        break;
                    default:
                        error("handle this case in %s:%d\n", FUNC, LINE);
                        break;
                    }
                }
                break;
            }
            case add_: case sub_: case mul_: case div_: // TODO: check all math_op operations
            {
                // TODO: use rax for long etc...
                // curr->creg = strdup("eax");
                Type type = curr->type;
                if(left->ptr)
                    pasm("mov %r, %a\n", curr, left);
                else if(left->creg && strcmp(left->creg, right->creg))
                    pasm("mov %r, %r\n", curr, left);
                else if(!left->creg)
                    pasm("mov %r, %v\n", curr, left);
                char *inst = NULL;
                switch(curr->type)
                {
                    case add_: inst = "add "; break;
                    case sub_: inst = "sub "; break;
                    case mul_: inst = "mul "; break;
                    case div_: inst = "div "; break;
                    default: break;
                }
                if(right->ptr)
                    pasm("%i%r, %a\n", inst, curr, right);
                else if(right->creg)
                    pasm("%i%r, %r\n", inst, curr, right);
                else if(!right->creg)
                    pasm("%i%r, %v\n", inst,  curr, left);
                curr->type = left->type;
                break;
            }
            case ret_:
            {
                if(left->ptr)
                {
                    // mov("", ""), pasm("%s, ", defaultReg(left)), paddr(left), pasm("\n");
                    pasm("mov %r, %a\n", left, left);
                }
                else
                {
                    switch(left->type)
                    {
                        case int_: pasm("mov eax, %ld\n", left->Int.value); break;
                        case void_:pasm("mov eax, 0\n"); break;
                        default:
                        {
                            error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type)); 
                            exit(1);
                            break;
                        }
                    }
                }
                pasm("leave\n");
                pasm("ret\n");
                break;
            }
            case fdec_:
            {
                pasm("%s:\n", curr->name);
                pasm("push rbp\n");
                pasm("mov rbp, rsp\n");
                pasm("sub rsp, %zu\n", (((ptr) + 15) / 16) * 16);
                break;
            }
            case end_bloc_:
            {
                pasm(".end%s:\n", curr->name);
                break;
            }
            case struct_: case end_struct_: case void_: break;
            default: error("%s: Unkown type [%s]\n", FUNC, to_string(curr->type)); break;
            break;
        }
    }
    finalize();
}