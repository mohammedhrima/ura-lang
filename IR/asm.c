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
            pasm(".FLT%zu: .long %zu /* %f */\n", curr->index,  *((uint32_t *)(&curr->Float.value)), curr->Float.value);
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
            case assign_:
            {
                curr->ptr = left->ptr;
                pasm("/*assign_node %s*/\n", left->name);
                if(right->ptr)
                {
                    Type type = right->type;
                    if(type == int_)
                        ;
                }
                if(right->ptr || right->creg)
                {

                }

            }
            case struct_: case end_struct_: case void_: break;
            default:
                error("Generate asm: Unkown Instruction [%s]\n", to_string(curr->type));
            break;
        }
    }
    finalize();
}