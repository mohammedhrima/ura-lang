#include "./utils.c"

int main()
{
   input =
      "protoFunc int printf(chars fmt, ...)\n"
      "\n"
      "def int main():\n"
      "   int result = 10\n"
      "   printf(\"func1 returned: %d\\n\", result)\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}