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


char* defaultReg(Token *token)
{
    switch(token->type)
    {
    case chars_: return "rax";
    case int_:   return "eax";
    case char_:  return "al";
    case float_: return "xmm0";
    default: error("%s: Unkown type [%s]\n", FUNC, to_string(token->type)); break;
    }
    return NULL;
}

void paddr(Token *token)
{
    switch(token->type)
    {
    case chars_: pasm("QWORD PTR -%ld[rbp]", token->ptr); break;
    case int_:   pasm("DWORD PTR -%ld[rbp]", token->ptr); break;
    case char_:  pasm("BYTE PTR -%ld[rbp]", token->ptr); break;
    case float_: pasm("DWORD PTR -%ld[rbp]", token->ptr); break;
    default: error("%s: Unkown type [%s]\n", FUNC, to_string(token->type)); break;
    }
}

void pvalue(Token *token)
{
    switch (token->type)
    {
    case int_:  pasm("%lld", token->Int.value); break;
    case bool_: pasm("%d", token->Bool.value); break;
    case char_: pasm("%d\n", token->Char.value); break;
    default: error("handle this case in %s:%d\n", FUNC, LINE); break;
    }
}

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
            case assign_:
            {
                curr->ptr = left->ptr;
                pasm("    /*assign_node %s*/\n", left->name);
                if(right->ptr || right->creg)
                {
                    char *reg = right->creg ? right->creg : defaultReg(right);
                    if(right->ptr)
                    {
                        left->type == float_ ? movss("%s, ", reg) : mov("%s, ", reg);
                        paddr(right);
                        pasm("\n");
                    }
                    left->type == float_ ? movss("", "") : mov("", "");
                    paddr(left);
                    pasm(", %s\n", reg);
                }
                else
                {
                    switch (right->type)
                    {
                    case int_: case bool_: case char_:
                        mov("", "");
                        paddr(left);
                        pasm(", ");
                        pvalue(right);
                        pasm("\n");
                        break;
                    case chars_:
                        lea("rax, .STR%zu[rip]\n", right->index);
                        mov("QWORD PTR -%ld[rbp], rax\n", left->ptr);
                        break;
                    case float_:
                        movss("xmm0, DWORD PTR .FLT%zu[rip]\n", right->index);
                        movss("DWORD PTR -%ld[rbp], xmm0\n", left->ptr);
                        break;
                    default:
                        error("handle this case in %s:%d\n", FUNC, LINE);
                        break;
                    }
                }
                break;
            }
            case ret_:
            {
                if(left->ptr)
                {
                    mov("", "");
                    pasm("%s, ", defaultReg(left));
                    paddr(left);
                    pasm("\n");
                }
                else
                {
                    switch(left->type)
                    {
                        case int_: mov("eax, %ld\n", left->Int.value); break;
                        case void_:mov("eax, 0\n",""); break;
                        default:
                        {
                            error("%s:%d handle this case [%s]\n", FUNC, LINE, to_string(left->type)); 
                            exit(1);
                            break;
                        }
                    }
                }
                pasm("    leave\n");
                pasm("    ret\n");
                break;
            }
            case fdec_:
            {
                pasm("%s:\n", curr->name);
                push("rbp\n", "");
                mov("rbp, rsp\n", "");
                pasm("    sub     rsp, %zu\n", (((ptr) + 15) / 16) * 16);
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