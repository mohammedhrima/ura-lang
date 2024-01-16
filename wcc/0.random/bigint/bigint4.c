#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define BASE 10
#define N 156
#define LIMIT 1000000
#define PRIMES 1000000

int count;

int sieve[LIMIT];

int primes[PRIMES];

#define rep(i, n) for (i = 0; i < n; i++)

#define POSITIVE 1
#define NEGATIVE 0

struct BigInteger
{
    int digits[N];
    int sign;
    int length;
};

typedef struct BigInteger BigInteger;

BigInteger stack[10000];

BigInteger add(BigInteger, BigInteger);
BigInteger subtract(BigInteger, BigInteger);
BigInteger multiply(BigInteger, BigInteger);
BigInteger *knuth_divide(BigInteger, BigInteger, BigInteger *);
void multiply_one_digit(int *, int *, int);
void shift_left(int *, int);
int check_zero(BigInteger);

BigInteger init()
{
    int i;
    BigInteger c;
    rep(i, N)
        c.digits[i] = 0;
    c.sign = 1;
    c.length = 1;
    return c;
}

void make_int(int A[], int n)
{
    int i;
    i = 0; // indexing starts at 0
    while (n)
    {
        A[i++] = n % BASE; // start from the least significant digit
        n /= BASE;         // get rid of each non significant digit at a time
    }
    /* fill the rest of the array up with zeros */
    while (i < N)
        A[i++] = 0;
}

void increment(int A[])
{
    int i;
    i = 0;
    while (i < N)
    {
        A[i]++; // increment the digit
        /* if it overflows (i.e., it was 9, now it's 10, too
         * big to be a digit) then...  	 */
        if (A[i] == BASE)
        {
            /* make it zero and index to the next
             * significant digit  */
            A[i] = 0;
            i++;
        }
        else
            /* otherwise, we are done */
            break;
    }
}

/* check whether A  > B
   if A > B return 1
   else return 0
   if A == B return 1

*/
int check_max(BigInteger A, BigInteger B)
{

    int i;
    // start from the most significant digit
    for (i = N - 1; i >= 0; i--)
    {
        if (A.digits[i] > B.digits[i])
        { // if the most signifant digit is greater
            return 1;
        }
        else if (A.digits[i] == B.digits[i]) // current digit is equal
            continue;
        else
            return 0;
    }

    return 2; // returns 2 if A == B
}

// subtracting two BigIntergers

BigInteger subtract(BigInteger A, BigInteger B)
{
    int sign;
    BigInteger temp;
    BigInteger C = init();
    // Both the BigIntegers are of same sign
    // eg subtract(-89785,-458)  subtract(4587, 584)
    if ((A.sign == POSITIVE && B.sign == POSITIVE) || A.sign == NEGATIVE && B.sign == NEGATIVE)
    {
        int boo = check_max(A, B); // check which is greater
        sign = A.sign;
        if (!boo)
        { // if |A| < |B| swap (A, B)
            temp = A;
            A = B;
            B = temp;
            sign = B.sign == POSITIVE ? NEGATIVE : POSITIVE; // if |B| > |A| sign of result is equal to -(sign of B)
        }
        int i = 0, diff; // diff is at digit
        int borrow = 0;  // borrow from the previous digit initially 0
        for (i = 0; i < N; i++)
        {
            diff = A.digits[i] - B.digits[i] - borrow;
            if (diff < 0)
            {
                diff += BASE; // add 10 to the diff
                borrow = 1;
            }
            else
                borrow = 0;
            C.digits[i] = diff;
        }
        rep(i, N) if (C.digits[N - 1 - i] != 0) break;
        C.length = N - i; // set the length
        if (N - i == 0)   // if all the digits are zero
            C.length = 1;

        C.sign = sign;
    }
    else if (A.sign == POSITIVE && B.sign == NEGATIVE)
    { // eg subtract(548, -458)
        B.sign = POSITIVE;
        C = add(A, B);
    }
    else
    {
        B.sign = NEGATIVE; // eg subtract(-458, 5878)
        C = add(A, B);
    }
    return C;
}

/*  make the most significant digit as least significant and vice versa
   eg invert(24587, 0) = 78542 	*/
BigInteger invert(BigInteger A, int offset)
{
    int i;
    BigInteger I = init();
    I.length = A.length + offset;
    rep(i, A.length)
        I.digits[i + offset] = A.digits[A.length - 1 - i];
    return I;
}

// copy a BigInteger to another number
BigInteger get_copy(BigInteger A)
{
    BigInteger C = init();
    C.length = A.length;
    memcpy(C.digits, A.digits, 128 * sizeof(int));
    return C;
}

// adding two BigIntegers
BigInteger add(BigInteger A, BigInteger B)
{
    int i, carry, sum;
    BigInteger C = init();
    // if signs of both the integers is same
    if ((A.sign == POSITIVE && B.sign == POSITIVE) || (A.sign == NEGATIVE && B.sign == NEGATIVE))
    {
        /* no carry yet */
        carry = 0;
        /* go from least to most significant digit */
        for (i = 0; i < N; i++)
        {
            /* the i'th digit of C is the sum of the
             * i'th digits of A and B, plus any carry
             */
            sum = A.digits[i] + B.digits[i] + carry;
            /* if the sum exceeds the base, then we have a carry. */
            if (sum >= BASE)
            {
                carry = 1;
                /* make sum fit in a digit (same as sum %= BASE) */
                sum -= BASE;
            }
            else
                /* otherwise no carry */
                carry = 0;
            /* put the result in the sum */
            C.digits[i] = sum;
        }
        rep(i, N) // get the length of the result
            if (C.digits[N - 1 - i] != 0) break;
        C.length = N - i;
    }
    else
    {
        // Both the integers are of opposite sign
        int boo = check_max(A, B);
        BigInteger TEMP = init();
        if (!boo && A.sign == POSITIVE)
        { // |A|  < |B|  and  A is positive
            B.sign = POSITIVE;
            C = subtract(B, A);
            C.sign = NEGATIVE;
        }
        else if (!boo && A.sign == NEGATIVE)
        { // |A| < |B| and A is negative
            A.sign = POSITIVE;
            C = subtract(B, A);
        }
        else if (boo && A.sign == POSITIVE)
        { // |A| > |B| and A is positive
            B.sign = POSITIVE;
            C = subtract(A, B);
        }
        else
        { // |A|  < |B|  and A is negative
            A.sign = POSITIVE;
            C = subtract(A, B);
            C.sign = NEGATIVE;
        }
        return C;
    }
    C.sign = A.sign;
    return C;
    /* if we get to the end and still have a carry, we don't have
     * anywhere to put it, so panic!  But that's not going to happen 'coz i am using 156 Digits
     */
    if (carry)
    {
        printf("overflow in addition!\n");
        return C;
    }
}

// print only the integer part
void print_the_integer(BigInteger A)
{
    int i;
    A.sign == POSITIVE ? printf("+") : printf("-");
    rep(i, A.length)
    {
        printf("%d", A.digits[A.length - 1 - i]);
    }
    printf("\n");
}

// prints the integer with following zeroes
void print_full_integer(BigInteger A)
{
    int i;
    A.sign == POSITIVE ? printf("+") : printf("-");
    rep(i, 128)
    {
        printf("%d", A.digits[i]);
    }
    printf("\n");
}

// multiply two BigIntegers
BigInteger multiply(BigInteger A, BigInteger B)
{
    int i, j;
    BigInteger P = init();
    BigInteger C = init();
    /* C will accumulate the sum of partial products.  It's initially 0. */
    /* for each digit in A... */
    for (i = 0; i < N; i++)
    {
        /* multiply B by digit A[i] */
        multiply_one_digit(B.digits, P.digits, A.digits[i]);
        /* shift the partial product left i bytes */
        shift_left(P.digits, i);
        /* add result to the running sum */
        C = add(C, P);
    }
    C.sign = A.sign == B.sign ? 1 : 0; // if both are of same sign
    if (C.length == 0)                 // if C  == 0
        C.length = 1;

    return C;
}

// find the length of BigInteger without the trailing zeroes
int find_len(BigInteger A)
{
    int i;
    for (i = N - 1; i >= 0; i--)
    {
        if (A.digits[i] != 0)
            return i + 1;
    }
}

void shift_right(int A[], int n)
{
    int i;
    /* going from left to right, move everything over to the
     * left n spaces
     */
    for (i = 0; i < N - n; i++)
        A[i] = A[i + n];

    /* fill the last n digits with zeros */
    while (i < N - 1)
        A[i--] = 0;
}

// divides two BigIntegers using Knuth Basic Division

BigInteger *knuth_divide(BigInteger A, BigInteger B, BigInteger C[])
{
    int i, j, m, n, num, k = 0, l;
    BigInteger P = init();
    BigInteger R = init();
    BigInteger T = init();
    BigInteger REM = init();

    BigInteger ten = init();
    ten.length = 2;
    ten.digits[0] = 0;
    ten.digits[1] = 1;
    C[0] = init();
    C[1] = init();
    int boo = check_max(A, B);
    // printf("i am here\n");
    for (i = N - 1; i >= 0, A.digits[i] == 0; i--)
        ;
    for (j = N - 1; j >= 0, B.digits[j] == 0; j--)
        ;
    n = i;
    m = j;
    n++;
    k = 0;
    l = 0;
    if (m == 0)
    {
        //		printf("i am here\n");
        for (i = n; i >= 1; i--)
        {
            num = A.digits[i] * 10 + A.digits[i - 1];
            A.digits[i - 1] = num % B.digits[m];
            C[0].digits[k++] = num / B.digits[m];
        }
        if (A.digits[0] >= B.digits[m])
        {
            C[0].digits[k++] = A.digits[0] / B.digits[m];
            C[1].digits[0] = A.digits[0] % B.digits[m];
        }
        else
            C[1].digits[0] = A.digits[0];
        i = 0;
        if (check_zero(C[0])) // unexpected behaviour
            while (C[0].digits[i] == 0)
            {
                //	printf("i am here\n");
                if (i == N - 1)
                {
                    i = 0;
                    break;
                }
                i++;
            }
        shift_right(C[0].digits, i);
        C[0].length = k - i;
        C[0] = invert(C[0], 0);
    }
    else if (m >= n)
    {
        for (i = n - 1; i >= 0; i--)
            C[1].digits[l++] = A.digits[i];
        C[1].length = l;
        C[1] = invert(C[1], 0);
    }
    else
    {
        //	printf("i am here\n");
        int div_first_digit, qt;
        BigInteger I = invert(A, 1);
        for (i = 0; i < n - m; i++)
        {
            qt = 0;
            P = init();
            R = init();
            REM = init();
            T = init();
            memcpy(P.digits, I.digits + i, sizeof(int) * (m + 2));
            P.length = m + 2;
            R = invert(P, 0);
            num = P.digits[1] + P.digits[0] * 10;
            div_first_digit = B.digits[m];
            qt = num / div_first_digit;
            BigInteger QT = init();
            QT.digits[0] = qt;
            T = multiply(B, QT);
            while (!check_max(R, T))
            {
                T = subtract(T, B);
                qt -= 1;
            }
            C[0].digits[k++] = qt;
            REM = subtract(R, T);
            rep(j, m + 1)
            {
                I.digits[i + 1 + j] = REM.digits[m - j];
            }
        }
        i = 0;
        //		 print_full_integer(C[0]);
        if (check_zero(C[0]))
        {
            // printf("digit %d\n",C[0].digits[0]);
            while (C[0].digits[i] == 0)
            {
                if (i == N - 1)
                {
                    i = 0;
                    break;
                }
                //	printf("came once\n");
                i++;
            }
        }
        // printf("i %d\n",i );
        shift_right(C[0].digits, i);
        C[0].length = k - i;
        C[0] = invert(C[0], 0);
        C[1] = REM;
        // added sign funda in division
        if (A.sign == NEGATIVE && B.sign == NEGATIVE)
            C[1].sign = NEGATIVE;
        else if (A.sign == POSITIVE && B.sign == NEGATIVE)
            C[0].sign = NEGATIVE;
        else if (A.sign == NEGATIVE && B.sign == POSITIVE)
        {
            C[0].sign = C[1].sign = NEGATIVE;
        }
    }

    /*	printf(" Quotient \n");
      print_the_integer(C[0]);
      printf(" Remainder \n");
      print_the_integer(C[1]);
       }
  */
}

void multiply_one_digit(int A[], int B[], int n)
{
    int i, carry;
    /* no extra overflow to add yet */
    carry = 0;
    /* for each digit, starting with least significant... */
    for (i = 0; i < N; i++)
    {
        /* multiply the digit by n, putting the result in B */
        B[i] = n * A[i];
        /* add in any overflow from the last digit */
        B[i] += carry;
        /* if this product is too big to fit in a digit... */
        if (B[i] >= BASE)
        {

            /* handle the overflow */

            carry = B[i] / BASE;
            B[i] %= BASE;
        }
        else
            /* no overflow */
            carry = 0;
    }
    if (carry)
        printf("overflow in multiplication!\n");
}

void shift_left(int A[], int n)
{
    int i;
    /* going from left to right, move everything over to the
     * left n spaces
     */
    for (i = N - 1; i >= n; i--)
        A[i] = A[i - n];
    /* fill the last n digits with zeros */
    while (i >= 0)
        A[i--] = 0;
}

// check if BigInteger == 1

int check_one(BigInteger A)
{
    int i;
    for (i = N - 1; i >= 1; i--)
    {
        if (A.digits[i] >= 1)
            return 0;
    }
    if (A.digits[0] == 1)
        return 1;
    else
        return 0;
}

// check if BigInteger == 0

int check_zero(BigInteger A)
{
    int i;
    rep(i, N)
    {
        if (A.digits[i] != 0)
            return 1;
    }
    return 0;
}

BigInteger nea(BigInteger p0, BigInteger p1)
{
    BigInteger l = init();
    BigInteger m = init();
    BigInteger s = init();
    BigInteger mul = init();
    BigInteger T[2];
    T[0] = T[1] = init();
    int tos = 0, flag = 0, i, b = 0;
    l = p0;
    m = p1;
    tos = 0;
    do
    {
        knuth_divide(l, m, T);
        stack[tos] = init();
        stack[tos++] = T[0];
        //		print_the_integer(T[0]);
        s = subtract(l, multiply(m, T[0]));
        // /print_the_integer(s);
        b = 1 - b;
        l = m;
        m = s;
        if (!check_zero(s))
        {
            flag = 1;
            break;
        }
        // print_the_integer(s);
    } while (!check_one(s));

    if (flag)
    {
        printf("inverse doesnt exist\n");
        l = init();
        return l;
    }

    if (b % 2 == 0)
    {
        m = init();
        m.digits[0] = 1;
    }
    else
    {
        m = init();
        m.digits[0] = 1;
        m.sign = NEGATIVE;
    }
    s = init();

    // printf("tos %d\n",tos);
    for (i = tos - 1; i >= 0; i--)
    {
        mul = init();
        mul = multiply(m, stack[i]);
        l = add(mul, s);
        s = m;
        m = l;
    }
    if (l.sign == NEGATIVE)
        l = add(l, p0);
    return l;
}

int enhanced_euclid(int p0, int p1)
{
    int temp;
    int l, m, t, s, tos, flag = 0, i, b = 0;
    l = p0;
    m = p1;
    int stack[100];
    tos = 0;
    do
    {
        t = l / m;
        stack[tos++] = t;
        //		printf("t  %d\n",t );
        s = l - m * t;
        //		printf("s %d\n",s );
        b = 1 - b;
        l = m;
        m = s;
        if (s == 0)
        {
            flag = 1;
            break;
        }

    } while (s != 1);
    if (flag)
    {
        printf("Inverse doesn\'t exist\n");
        return -1;
    }
    if (b == 0)
        m = 1;
    else
        m = -1;
    s = 0;
    for (i = tos - 1; i >= 0; i--)
    {
        l = m * stack[i] + s;
        s = m;
        m = l;
    }
    if (l < 0)
        l += p0;
    return l;
}

int check_greater_one(BigInteger A)
{
    int i;
    rep(i, N)
    {
        if (A.digits[i] > 1)
            return 1;
    }
    return 0;
}

BigInteger extended_euclid(BigInteger A, BigInteger B)
{
    BigInteger T = init();
    BigInteger NT = init();
    BigInteger R = init();
    BigInteger NR = init();
    BigInteger Q = init();
    BigInteger TMP = init();
    BigInteger DUP_A = init();
    BigInteger C[2];
    BigInteger mul;
    mul = init();
    C[0] = C[1] = init();
    if (B.sign == NEGATIVE)
    {
        printf("b is negative\n");
        B.sign = POSITIVE;
    }

    if (A.sign == NEGATIVE)
    {
        printf("a is negative\n");
        DUP_A = A;
        DUP_A.sign = POSITIVE;
        knuth_divide(DUP_A, B, C);
        A = subtract(B, C[1]);
    }
    NT.digits[0] = 1;
    C[0] = C[1] = init();
    R = B;
    knuth_divide(A, B, C);
    NR = C[1];
    while (check_zero(NR))
    {
        knuth_divide(R, NR, C);
        TMP = NT;
        mul = multiply(C[0], NT);

        NT = subtract(T, multiply(C[0], NT));
        T = TMP;

        TMP = NR;
        NR = subtract(R, multiply(C[0], NR));
        R = TMP;
    }
    if (check_greater_one(R))
    {
        printf("No MMI exist\n");
        T = init();
        return T;
    }
    if (T.sign == NEGATIVE)
        T = add(T, B);
    return T;
}

int mul_inv(int a, int b)
{
    int t, nt, r, nr, q, tmp;
    if (b < 0)
        b = -b;
    if (a < 0)
        a = b - (-a % b);
    t = 0;
    nt = 1;
    r = b;
    nr = a % b;
    t = 0;
    nt = 1;
    r = b;
    nr = a % b;
    while (nr != 0)
    {
        q = r / nr;
        tmp = nt;
        nt = t - q * nt;
        t = tmp;
        tmp = nr;
        nr = r - q * nr;
        r = tmp;
    }
    if (r > 1)
        return -1; /* No inverse */
    if (t < 0)
        t += b;
    return t;
}

void store_prime()
{
    int i, j;

    rep(i, LIMIT)
        sieve[i] = i;
    sieve[0] = sieve[1] = 0;
    for (i = 0; i <= sqrt(LIMIT); i++)
    {
        if (sieve[i])
        {
            for (j = sieve[i] * 2; j < LIMIT; j += sieve[i])
            {
                sieve[j] = 0;
            }
        }
    }
    count = 0;
    for (i = 0; i < LIMIT && count < PRIMES; i++)
    {
        if (sieve[i])
        {
            primes[count++] = sieve[i];
        }
    }
}

int gcd(int a, int b)
{
    int t;
    while (b)
    {
        t = a;
        a = b;
        b = t % b;
    }
    return a;
}

int power(int a, int b, int MOD)
{
    long long x = 1, y = a;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x * y);
            if (x > MOD)
                x %= MOD;
        }
        y = (y * y);
        if (y > MOD)
            y %= MOD;
        b /= 2;
    }
    return x;
}

void find_euler_totient()
{
    int n, i, result, flag = 0, g, a;
    long long ans;
    scanf("%d", &a);
    scanf("%d", &n);

    result = n;
    for (i = 0; i <= n && i < count; i++)
    {
        if (n % primes[i] == 0)
        {
            flag = 1;
            result = (result * (primes[i] - 1)) / primes[i];
        }
    }
    if (!flag)
        result--;

    printf("totient %d\n", result);
    g = gcd(a, n);
    if (g == 1)
    {
        ans = power(a, result - 1, n);
        printf("%lld\n", ans);
    }
    else
        printf("inverse doenn't exist\n");
}

int main(int argc, char *argv[])
{

    time_t start_time, end_time, elapsed;
    store_prime();
    // find_euler_totient();
    //  		printf("the inverse is %d\n", mul_inv(23 , 113));
    // printf("the inverse is %d\n", enhanced_euclid(1237, 4571));
    //		printf("the inverse is%d\n", enhanced_euclid(5, 4));

    //	printf("%d\n", enhanced_euclid(9, 7));
    //	printf("%d\n", enhanced_euclid(31415926, 27182845));
    time(&start_time);
    int i, t;
    BigInteger result;
    BigInteger qr[2];
    qr[0] = init();
    qr[1] = init();
    result = init();
    BigInteger d1, d2;
    int len_d1, len_d2, len_r, len_q;
    int dig1_int[N], dig2_int[N];
    // char dig1[N], dig2[N];
    // scanf("%d", &t);
    // while (t--)
    // {
    // scanf("%s", dig1);
    // scanf("%s", dig2);
    char dig1[] = "123";
    char dig2[] = "456";
    len_d1 = strlen(dig1);
    len_d2 = strlen(dig2);

    rep(i, N)
        dig1_int[i] = 0;

    rep(i, N)
        dig2_int[i] = 0;

    rep(i, len_d1)
    {
        dig1_int[i] = dig1[len_d1 - i - 1] - '0';
    }

    rep(i, len_d2)
    {
        dig2_int[i] = dig2[len_d2 - i - 1] - '0';
    }
    d1.sign = POSITIVE;
    d2.sign = POSITIVE;
    d1.length = len_d1;
    d2.length = len_d2;
    memcpy(d1.digits, dig1_int, N * sizeof(int));
    memcpy(d2.digits, dig2_int, N * sizeof(int));
    //	result = add(d1, d2);
    // knuth_divide(d1, d2, qr);
    //    print_the_integer(qr[0]);
    // print_the_integer(qr[1]);
    result = nea(d2, d1);
    print_the_integer(result);
    result = extended_euclid(d1, d2);
    print_the_integer(result);
    // }
    //	printf("time=%.3lf sec.\n", (double) (clock())/CLOCKS_PER_SEC);
    // printf("Finished in about %.4f seconds. \n", difftime(end_time, start_time));
    // int b = check_max(d1, d2);
    // printf(" max result %d\n",b);
    // BigInteger C = get_copy(d1);
    // rint_full_integer(C);
    // BigInteger I = invert(d1);
    // print_full_integer(I);
    return 0;
}