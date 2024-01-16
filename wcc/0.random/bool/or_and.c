#include <stdbool.h>

int main()
{
    bool a = false;
    bool b = false;
    bool c = false;
    bool d = true;
    bool e = true;
    bool f = false;
    bool z = a && b && c && d || d && e && f;
}
