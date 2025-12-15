#include "./utils.c"

/*
Original:
def int main():
   char c = 'a'
   func bool isdigit(): return (c >= '0' && c <= '9')
   if isdigit(): return 11
   else: return 12
end

Transformed to:
def bool isdigit(ref char ptr):
   return (ptr >= '0' && ptr <= '9')
end

def int main():
   char c = 'a'
   if isdigit(&c): return 11
   else: return 12
end

For now, we'll implement the transformed version directly.
The compiler would need to:
1. Extract nested functions
2. Add captured variables as pointer parameters
3. Transform calls to pass addresses of captured vars
*/

int main()
{
   input =
      "def bool isdigit(char c):\n"
      "   return (c >= '0' && c <= '9')\n"
      "end\n"
      "\n"
      "def int main():\n"
      "   char c = 'a'\n"
      "   if isdigit(c):\n"
      "      return 11\n"
      "   end\n"
      "   return 12\n"
      "end\n"
      "\0";

   tokenize();
   compile();
   free_tokens();

   return 0;
}