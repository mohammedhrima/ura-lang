#include "utils.c"

/*
main():
   chars a = stack(10) as chars
   return 0
*/

int main()
{
   input =
      "def int main():\n"
      "   chars a = stack(10) as chars\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}