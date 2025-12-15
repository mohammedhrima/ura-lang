#include "./utils.c"

int main()
{
   input =
      "def int main():\n"
      "   int i = 0\n"
      "   while i < 5:\n"
      "      i = i + 1\n"
      "   end\n"
      "   return i\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}