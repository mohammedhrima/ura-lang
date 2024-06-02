#include <stdio.h>
#include <gmp.h>
#include <assert.h>

typedef long long unsigned int number;

void rsa_keys(mpz_t n, mpz_t d, const mpz_t p, const mpz_t q, const mpz_t e)
{
    mpz_mul(n, p, q);

    mpz_t p_1, q_1, lambda, gcd, mul, mod;
    mpz_inits(p_1, q_1, lambda, gcd, mul, mod, NULL);
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_lcm(lambda, p_1, q_1);

    printf("lambda = %s\n", mpz_get_str(NULL, 0, lambda));
    // e must be bigger than 1
    assert(mpz_cmp_ui(e, 1) > 0);

    // e must be smaller than lambda
    assert(mpz_cmp(lambda, e) > 0);

    // GCD(e, lambda) must be 1
    mpz_gcd(gcd, e, lambda);
    assert(mpz_cmp_ui(gcd, 1) == 0);

    mpz_invert(d, e, lambda);

    // e * d MOD lambda must be 1
    mpz_mul(mul, e, d);
    mpz_mod(mod, mul, lambda);
    assert(mpz_cmp_ui(mod, 1) == 0);

    mpz_clears(gcd, p_1, q_1, mul, mod, lambda, NULL);
}

// RSA encryption
void encrypt(mpz_t encrypted,
             const mpz_t message,
             const mpz_t e,
             const mpz_t n)
{
    mpz_powm(encrypted, message, e, n);
}

// RSA decryption
void decrypt(mpz_t original,
             const mpz_t encrypted,
             const mpz_t d,
             const mpz_t n)
{
    mpz_powm(original, encrypted, d, n);
}

void display_gmp(const mpz_t message,
                 const mpz_t n,
                 const mpz_t e,
                 const mpz_t d)
{
    mpz_t encrypted, decrypted;
    mpz_inits(encrypted, decrypted, NULL);
    encrypt(encrypted, message, e, n);
    decrypt(decrypted, encrypted, d, n);

    // The decrypted message must be equal to the original
    assert(mpz_cmp(message, decrypted) == 0);

    printf("Public key = (e: %s, n: %s)\n", mpz_get_str(NULL, 0, e), mpz_get_str(NULL, 0, n));
    printf("Private key = (d: %s, n: %s)\n", mpz_get_str(NULL, 0, d), mpz_get_str(NULL, 0, n));
    printf("Original message: %s\n", mpz_get_str(NULL, 0, message));
    printf("Encrypted message: %s\n", mpz_get_str(NULL, 0, encrypted));
    printf("Decrypted message: %s\n", mpz_get_str(NULL, 0, decrypted));
    printf("\n");

    mpz_clears(encrypted, decrypted, NULL);
}

void display_num(const number msg,
                 const number pi,
                 const number qi,
                 const number ei)
{
    printf("Initializing with p = %llu, q = %llu, e = %llu\n", pi, qi, ei);
    mpz_t n, d, p, q, e, original;
    mpz_init_set_ui(p, pi);
    mpz_init_set_ui(q, qi);
    mpz_init_set_ui(e, ei);
    mpz_init_set_ui(original, msg);
    mpz_inits(n, d, NULL);
    rsa_keys(n, d, p, q, e);

    display_gmp(original, n, e, d);

    mpz_clears(n, d, e, p, q, original, NULL);
}

void display_str(const char *msg,
                 const char *pi,
                 const char *qi,
                 const char *ei)
{
    printf("Initializing with p = %s, q = %s, e = %s\n", pi, qi, ei);

    mpz_t n, d, p, q, e, original;
    mpz_init_set_str(p, pi, 10);
    mpz_init_set_str(q, qi, 10);
    mpz_init_set_str(e, ei, 10);
    mpz_init_set_str(original, msg, 10);
    mpz_inits(n, d, NULL);
    rsa_keys(n, d, p, q, e);

    display_gmp(original, n, e, d);
    mpz_clears(n, d, e, p, q, original, NULL);
}

int main()
{
    mpz_t msg, n, d, e;

    // Example taken from Wikipedia
    // https://en.wikipedia.org/wiki/RSA_(cryptosystem)#Key_generation
    display_num(65, 61, 53, 17);

    // Example from Twitter
    // https://twitter.com/kosamari/status/838738015010848769
    mpz_init_set_ui(msg, 123);
    mpz_init_set_ui(n, 323);
    mpz_init_set_ui(e, 5);
    mpz_init_set_ui(d, 29);
    display_gmp(msg, n, e, d);

    // Very small prime numbers
    display_num(123, 13, 19, 17);

    // With some prime numbers from
    // http://www.bigprimes.net/
    display_num(67890, 541, 461, 107);
    display_num(123456, 1181, 929, 173);

    // The PHP version takes around 10 seconds on a MacBook Air
    display_num(123456, 1181, 929, 1987);

    // The PHP takes around 40 seconds in a MacBook Air
    display_num(123456, 1181, 929, 17);

    // Very big numbers; using Mersenne primes,
    // something impossible in the PHP version
    display_str("1111119999999999911111111",
                "162259276829213363391578010288127",
                "618970019642690137449562111",
                "170141183460469231731687303715884105727");

    mpz_clears(n, d, e, msg, NULL);

    return 0;
}