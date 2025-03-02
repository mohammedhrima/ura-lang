#include "./include/header.h"

Wolfc obj;

int main(int argc, char **argv)
{
   obj = (Wolfc) {.filename = argv[1]};
   open_file();
   parse();
   generate();
   free_memory();
}