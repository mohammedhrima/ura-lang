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
                // else if(curr->isarg)
                    // error("%s:%d handle this case\n", FUNC, LINE);
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
                        pasm("lea %r, .STR%zu[rip]\n", right, right->index);
                        pasm("mov %a, %r\n", left, left);
                        break;
                    case float_:
                        pasm("movss %r, DWORD PTR .FLT%zu[rip]\n", right, right->index);
                        pasm("movss %a, %r\n", left, left);
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
                Type type = curr->type;
                char *inst = left->type == float_ ? "movss" : "mov";

                if(left->ptr)
                    pasm("%i%r, %a\n", inst, curr, left);
                else if(left->creg && strcmp(left->creg, right->creg))
                    pasm("%i%r, %r\n", inst, curr, left);
                else if(!left->creg)
                    pasm("%i%r, %v\n", inst, curr, left);
                
                switch(curr->type)
                {
                    case add_: inst = left->type == float_ ? "addss "  : "add " ; break;
                    case sub_: inst = left->type == float_ ? "subss "  : "sub " ; break;
                    case mul_: inst = left->type == float_ ? "imulss " : "imul "; break;
                    case div_: inst = left->type == float_ ? "divss "  : "div " ; break;
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
            case equal_: case not_equal_:
            case less_ : case less_equal_:
            case more_ : case more_equal_:
            {
                char *inst = left->type == float_ ? "movss" : "mov";
                if(left->ptr)
                    pasm("%i%r, %a\n", inst, left, left);
                else if(left->creg /*&& strcmp(left->creg, r->creg)*/)
                    pasm("%i%r, %r\n", inst, left, left);
                else if(!left->creg)
                    pasm("%i%r, %v\n", inst, left, left);
                
                char *reg = NULL;
                
                #define setCase(case_, var, value) case case_: var = value; break; 
                switch(left->type)
                {
                    setCase(int_,   reg, "ebx");
                    setCase(float_, reg, "xmm1");
                    setCase(char_,  reg, "bl");
                    setCase(bool_,  reg, "ebx");
                    default: error("%s: Unkown type [%s]\n", FUNC, to_string(left->type)); break;
                }
                if(right->ptr)
                    pasm("%i%s, %a\n", inst, reg, right);
                else if(right->creg)
                    pasm("%i%s, %r\n", inst, reg, right);
                else if(!right->creg)
                    pasm("%i%s, %v\n", inst, reg, right);
                inst = left->type == float_ ? "ucomiss" : "cmp";
                pasm("%i%r, %s\n", inst, left, reg);
                switch(curr->type)
                {
                    setCase(equal_,     inst, "sete");
                    setCase(not_equal_, inst, "setne");
                    setCase(less_,      inst, "setl");
                    setCase(less_equal_,inst, "setle");
                    setCase(more_,      inst, "setg");
                    setCase(more_equal_,inst, "setge");
                    default: error("%s: Unkown type [%s]\n", FUNC, to_string(left->type)); break;
                }
                curr->retType = bool_;
                curr->creg = "al";
                pasm("%i%r\n", inst, curr);
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
            case fcall_:
            {
                pasm("%i%s\n", "call", curr->name);
                break;
            }
            case push_:
            {
                if(right->name)
                {
                    if(left->ptr)
                        pasm("mov %s, %a\n", right->name, left);
                    else if(left->creg)
                        pasm("mov %s, %r\n", right->name, left);
                    else
                    {
                        switch (left->type)
                        {
                        case int_: case bool_: case char_:
                            pasm("mov %s, %v\n", right->name, left);
                            break;
                        case chars_:
                            pasm("lea %r, .STR%zu[rip]\n", left, left->index);
                            pasm("mov %s, %r\n", right->name, left);
                            break;
                        case float_:
                            pasm("movss %r, DWORD PTR .FLT%zu[rip]\n", left, left->index);
                            pasm("movss %s, %r\n", right->name, left);
                            break;
                        default:
                            error("handle this case in %s:%d\n", FUNC, LINE);
                            break;
                        }
                        // pasm("mov %s, %v\n", right->name, left);
                        // error("%s:%d handle this case\n");
                    }
                }
                else
                {
                    if(left->ptr)
                    {
                        pasm("mov %r, %a\n", left, left);
                        pasm("push rax\n");
                    }
                    else if(left->creg)
                    {
                        pasm("mov %r, %a\n", left, left);
                        pasm("push rax\n");
                    }
                    else
                    {
                        error("%s:%d handle this case\n", FUNC, LINE);
                    }
                }
                break;
            }
            case pop_:
            {
                if(right->name)
                {
                    pasm("//pop [%s]\n", left->name);
                    if(left->ptr)
                    {
                        switch(left->type)
                        {
                            case int_: pasm("mov DWORD PTR -%zu[rbp], %s\n", left->ptr, right->name); break;
                            default:
                            {
                                error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type)); 
                                exit(1);
                                break;
                            }
                        }
                    }
                    else
                    {
                        error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type)); 
                        exit(1);
                    }
                }
                else
                {

                    // error("%s:%d handle this case\n", FUNC, LINE);
                }
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
                        case int_: pasm("mov %r, %ld\n", left, left->Int.value); break;
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
            case je_:
            {
                pasm("%ial, 1\n", "cmp");
                pasm("%i.%s%zu\n", "je", curr->name, curr->index);
                // curr->creg = "al";
                break;
            }
            case jne_:
            {
                pasm("%ial, 1\n", "cmp");
                pasm("%i.%s%zu\n", "jne", curr->name, curr->index);
                // curr->creg = "al";
                break;
            }
            case jmp_:
            {
                pasm("%i.%s%zu\n", "jmp", curr->name, curr->index);
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
            case struct_: case end_struct_: case void_: break;
            default: error("%s: Unkown type [%s]\n", FUNC, to_string(curr->type)); break;
            break;
        }
    }
    finalize();
}