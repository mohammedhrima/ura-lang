#include "./utils.c"

/*
Complete variadic function implementation in your language
*/

int main()
{
   input =
      "def int sum(int count, ...):\n"
      "   va_list args\n"
      "   va_start(args, count)\n"
      "   int total = 0\n"
      "   int i = 0\n"
      "   while (i < count):\n"
      "      total = total + va_arg(args, int)\n"
      "      i = i + 1\n"
      "   end\n"
      "   va_end(args)\n"
      "   return total\n"
      "end\n"
      "\n"
      "def int main():\n"
      "   int result = sum(3, 10, 20, 30)\n"
      "   return result\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}