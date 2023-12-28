
#include <bits/stdc++.h>

using namespace std;

class BigInt
{
public:
    string digits;
    BigInt(unsigned long long n)
    {
        do
        {
            digits.push_back(n % 10);
            n /= 10;
        } while (n);
    }
};
ostream &operator<<(ostream &out, const BigInt &a)
{
    for (int i = a.digits.size() - 1; i >= 0; i--)
        cout << (short)a.digits[i];
    return cout;
};
int main()
{
    BigInt x(100);
    std::cout << x << std::endl;
}