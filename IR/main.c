#include "asm.c"

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
    for (size_t i = 0; i < tk_pos; i++)
        ptoken(tokens[i]);
    debug(SPLIT);
#endif
    

    Node *head = NULL;
#if AST
    enter_scoop("");
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
#if BUILTINS
        create_builtin("write", (Type[]){int_, chars_, int_, 0}, int_);
        create_builtin("read", (Type[]){int_, chars_, int_, 0}, int_);
        create_builtin("exit", (Type[]){int_, 0}, int_);
        create_builtin("malloc", (Type[]){int_, 0}, ptr_);
        create_builtin("calloc", (Type[]){int_, int_, 0}, ptr_);
        create_builtin("strdup", (Type[]){chars_, 0}, chars_);
        create_builtin("strlen", (Type[]){chars_, 0}, int_);
        create_builtin("free", (Type[]){ptr_, 0}, void_);
        create_builtin("strcpy", (Type[]){chars_, chars_, 0}, chars_);
        create_builtin("strncpy", (Type[]){chars_, chars_, int_, 0}, chars_);
        create_builtin("puts", (Type[]){chars_, 0}, int_);
        create_builtin("putstr", (Type[]){chars_, 0}, int_);
        create_builtin("putchar", (Type[]){char_, 0}, int_);
        create_builtin("putnbr", (Type[]){int_, 0}, int_);
        create_builtin("putbool", (Type[]){bool_, 0}, int_);
        create_builtin("putfloat", (Type[]){float_, 0}, int_);
#endif
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
    generate_asm();
#endif
    }
#endif
    clear(head, tokens, first_insts, input);
#if ASM
    return found_error;
#else
    return 1;
#endif
}