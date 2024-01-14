void putnbr(long n)
{

}

int main()
{
#if 1
    long *arr[] = {(long[]){11, 12, 13, 14}};
    long **arr1 = &arr[0];
    long *arr2 = arr1[0];
    int i = 2;
    putnbr(arr2[i]);
#else
    int i = 5;
    printf("%d\n", i);
#endif
}
