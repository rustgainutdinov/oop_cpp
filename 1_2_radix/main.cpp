#include <iostream>
#include <map>
#include <cstdlib>

using namespace std;

const int MAX_NOTATION = 36;

class converting_exception : public exception
{
    const char *what() const noexcept override
    {
        return "Converting string to int error";
    }
};

map<char, int> hexToInt = {
        {'0', 0},
        {'1', 1},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
        {'A', 10},
        {'B', 11},
        {'C', 12},
        {'D', 13},
        {'E', 14},
        {'F', 15},
        {'G', 16},
        {'H', 17},
        {'I', 18},
        {'J', 19},
        {'K', 20},
        {'L', 21},
        {'M', 22},
        {'N', 23},
        {'O', 24},
        {'P', 25},
        {'Q', 26},
        {'R', 27},
        {'S', 28},
        {'T', 29},
        {'U', 30},
        {'V', 31},
        {'W', 32},
        {'X', 33},
        {'Y', 34},
        {'Z', 35},
};

void AssertValidArguments(int argc)
{
    if (argc != 4)
    {
        throw invalid_argument("Invalid arguments, use: radix <source notation> <destination notation> <value>");
    }
}

int GetIntNotation(const char *notationStr)
{
    char *p;
    int notation = (int) strtol(notationStr, &p, 10);
    if (notation < 2 || notation > MAX_NOTATION)
    {
        throw invalid_argument("Notation is out of range 2..36");
    }
    return notation;
}

int Exponentiate(int value, int radix, int numberOrder)
{
    for (int j = 0; j < numberOrder; j++)
    {
        if (((INT_MAX / radix) - value) < 0)
        {
            throw converting_exception();
        }
        value *= radix;
    }
    return value;
}

int StringToInt(const string &str, int radix, bool isNegative)
{
    int numberOrder = (int) (str.length() - 1);
    int resultValue = 0;
    const int maxVal = isNegative ? INT_MIN : INT_MAX;
    for (char ch: str)
    {
        ch = toupper(ch);
        if (hexToInt.find(ch) == hexToInt.end())
        {
            throw converting_exception();
        }
        int orderValue = hexToInt[ch];
        if (orderValue >= radix)
        {
            throw converting_exception();
        }
        orderValue = isNegative ? -orderValue : orderValue;
        int orderResultValue = Exponentiate(orderValue, radix, numberOrder);
        // != is XOR
        if ((orderResultValue - maxVal + resultValue) != 0 &&
            ((orderResultValue - maxVal + resultValue) > 0) != isNegative)
        {
            throw converting_exception();
        }
        resultValue += orderResultValue;
        numberOrder--;
    }
    return resultValue;
}

string IntToStr(int n, int radix, bool isNegative)
{
    string resultValue;
    map<int, char> intToHex;
    for (auto &i : hexToInt)
    {
        intToHex[i.second] = i.first;
    }
    do
    {
        resultValue += intToHex[abs(n % radix)];
    } while (n /= radix);
    resultValue += isNegative ? "-" : "";
    reverse(resultValue.begin(), resultValue.end());
    return resultValue;
}

string Translate(string value, int srcNotation, int dstNotation)
{
    bool isNegative = false;
    if (value[0] == '-')
    {
        value.erase(0, 1);
        isNegative = true;
    }
    int decimalValue = StringToInt(value, srcNotation, isNegative);
    return IntToStr(decimalValue, dstNotation, isNegative);
}

int main(int argc, const char *argv[])
{
    try
    {
        AssertValidArguments(argc);
        int srcNotation = GetIntNotation(argv[1]);
        int dstNotation = GetIntNotation(argv[2]);
        string value = argv[3];
        cout << Translate(value, srcNotation, dstNotation) << "\n";
    }
    catch (const exception &e)
    {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}
