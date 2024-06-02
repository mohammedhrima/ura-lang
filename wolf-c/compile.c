#include "header.h"

void init_asm()
{
    pasm(".intel_syntax noprefix\n");
    // pasm(".include \"/wolf-c/import/header.s\"\n\n");
    pasm("\n");
    pasm(".text\n");
    pasm(".globl	main\n\n");
    pasm("main:\n");
    push("rbp\n");
    mov("rbp, rsp\n");
    sub("rsp, 100\n");
}

void finalize_asm()
{
    mov("rax, 0\n");
    pasm("leave\n");
    pasm("ret\n\n");
}

