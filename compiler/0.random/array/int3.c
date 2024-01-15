void putnbr(long n)
{
}

int main()
{
#if 1
    // int n = 16;
    long arr[] = {11, 22, 33};
    long *arr1 = &arr[0];
    long i = 0;
    // while (i < 3)
    // {
        putnbr(arr1[i]);
        i++;
    // }
#else
    int i = 5;
    printf("%d\n", i);
#endif
}

// mov     QWORD PTR -8[rbp], 0 /* declare arr */
// mov     QWORD PTR -16[rbp], 11 
// mov     QWORD PTR -24[rbp], 22 
// mov     QWORD PTR -32[rbp], 33 
// lea     rax, QWORD -32[rbp] /*head of array*/
// mov     rax, [rax]
// mov     QWORD PTR -40[rbp], rax 
// sub     rsp, 60
// mov     rax, QWORD PTR -40[rbp] /* assign to arr */
// mov     QWORD PTR -8[rbp], rax
// mov     QWORD PTR -48[rbp], 0 /* declare i */
// mov     rax, QWORD PTR -48[rbp]
// lea     rdx, 0[0+rax*8]
// mov     rax, QWORD PTR -8[rbp]
// add     rax, rdx
// mov     QWORD PTR -56[rbp], rax