#include "./utils.c"

int main()
{
   input =
      "def int main():\n"
      "   chars str = \"Hello World\"\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}