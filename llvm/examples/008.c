#include "./utils.c"

/*
def int main():
   int a = 1
   ref int b = a
   b = 2
   return a
end

Note: 'ref int b = a' creates a reference to a
Setting b = 2 should change a to 2
So this returns 2
*/

int main()
{
   input =
      "def int main():\n"
      "   int a = 1\n"
      "   ref int b = a\n"
      "   b = 2\n"
      "   return a\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}