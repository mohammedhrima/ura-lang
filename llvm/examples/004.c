#include "./utils.c"

int main()
{
   input =
      "def int add_multiply(int a, int b, int c):\n"
      "   int sum = a + b\n"
      "   int result = sum * c\n"
      "   return result\n"
      "end\n"
      "\n"
      "def int main():\n"
      "   int x = add_multiply(5, 3, 2)\n"
      "   return x\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}