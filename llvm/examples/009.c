#include "utils.c"

/*
main():
   int a = 1
   char b = a as char
   b = 'e'
   return 0
*/

int main()
{
   input =
      "def int main():\n"
      "   int a = 1\n"
      "   char b = a as char\n"
      "   b = 'e'\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}