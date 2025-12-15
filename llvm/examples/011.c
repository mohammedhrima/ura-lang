#include "utils.c"

/*
main():
   try:
      int x = 10
      int y = 0
      if y == 0:
         throw 1
      end
      int result = x / y
   catch int error:
      return error
   end
   return 0
*/

int main()
{
   input =
      "def int main():\n"
      "   try:\n"
      "      int x = 10\n"
      "      int y = 0\n"
      "      if y == 0:\n"
      "         throw 1\n"
      "      end\n"
      "      int result = x / y\n"
      "   catch int error:\n"
      "      return error\n"
      "   end\n"
      "   return 0\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}