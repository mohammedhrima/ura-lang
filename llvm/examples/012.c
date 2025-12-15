#include "utils.c"

int main()
{
   printf("=== Test 1: Invalid negative index ===\n");
   input =
      "protoFunc int printf(chars, ...)\n"
      "\n"
      "def int main():\n"
      "   printf(\"Before allocation\\n\")\n"
      "   chars u = stack(2) as chars\n"
      "   printf(\"After allocation\\n\")\n"
      "   u[-10] = 'a'\n"
      "   printf(\"This should not print\\n\")\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}