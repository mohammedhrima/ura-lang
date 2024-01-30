#include <unistd.h>
#include <stdbool.h>

void putbool(bool bool_)
{
    if (bool_)
        write(1, "True", 5);
    else
        write(1, "False", 6);
}

int main()
{
    bool bool_;

    float a = 4.2;
    float b = 0.0;
    // int a = 5;
    // int b = 5;

#if 0
    bool_ = a >= b; 
    /*
    movss	xmm0, a
	comiss	xmm0, b
	setnb	al
	mov		bool_, al
    */
#elif 0
    bool_ = a > b;
    /*
    movss	xmm0, a
	comiss	xmm0, b
	seta	al
	mov	    bool_, al
    */
#elif 0
    bool_ = a == b;
    /*
    movss	xmm0, a
	ucomiss	xmm0, b
	setnp	al
	mov	    edx, 0
	movss	xmm0, a
	ucomiss	xmm0, b
	cmovne	eax, edx
	mov	    bool_, al
    */

#elif 0
    bool_ = a <= b; 
    /*
    movss   xmm0, b
    comiss	xmm0, a
	setnb	al
    */
#elif 0
    bool_ = a > b;
    /*
    movss   xmm0, a
    comiss  xmm0, b
    seta    al
    */
#elif 1
    bool_ = a != b;

#endif
    putbool(bool_);
    // // logic operators
    // bool_ = 5 == 5;
    // putbool(bool_);
    // bool_ = 5 < 5 && 1 == 1;
    // putbool(bool_);
    // bool_ = 5 > 5;
    // putbool(bool_);
    // bool_ = 5 <= 5;
    // putbool(bool_);
    // bool_ = 5 >= 5;
    // putbool(bool_);

}
