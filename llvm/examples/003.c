#include "./utils.c"

int main()
{
   input =
      "protoFunc int putchar(char c)\n"
      "\n"
      "def int main():\n"
      "   chars str = \"abcd\"\n"
      "   char c = str[0]\n"
      "   putchar(c)\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}