#include <iostream>
#include <vector>
#include <string>

using namespace std;

string multiply(string num1, string num2)
{
    // Convert the input numbers from strings to vectors of integers
    vector<int> vec1(num1.size());
    for (int i = 0; i < num1.size(); i++)
        vec1[i] = num1[num1.size() - i - 1] - '0';
    vector<int> vec2(num2.size());
    for (int i = 0; i < num2.size(); i++)
        vec2[i] = num2[num2.size() - i - 1] - '0';

    // Initialize the result vector with zeros
    vector<int> result(vec1.size() + vec2.size());
    // Multiply each digit in vec2 with vec1 and add the result to the appropriate position in the result vector
    for (int i = 0; i < vec2.size(); i++)
    {
        int carry = 0;
        for (int j = 0; j < vec1.size(); j++)
        {
            int product = vec1[j] * vec2[i] + carry + result[i + j];
            carry = product / 10;
            result[i + j] = product % 10;
        }
        result[i + vec1.size()] = carry;
    }

    // Remove leading zeros from the result vector and convert it back to a string
    while (result.size() > 1 && result.back() == 0)
        result.pop_back();
    string str(result.size(), '0');
    for (int i = 0; i < result.size(); i++)
        str[result.size() - i - 1] = result[i] + '0';
    return str;
}

int main()
{
    string num1 = "4154";
    string num2 = "51454";
    cout << multiply(num1, num2) << endl;

    num1 = "654154154151454545415415454";
    num2 = "63516561563156316545145146514654";
    cout << multiply(num1, num2) << endl;

    return 0;
}