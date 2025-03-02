#include "./include/header.h"

Wolfc obj;

int main(int argc, char **argv)
{
   obj = (Wolfc) {.filename = argv[1]};
   open_file();
   tokenize();
   generate_ast();
   generate_ir();
   optimize_ir();
   generate_asm();
   free_memory();
}