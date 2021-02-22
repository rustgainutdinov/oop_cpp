#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>

const int MAX_NOTATION = 36;

std::map<char, int> hexToInt =  {
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

int exponentiate(int value, int radix, int numberOrder, bool & wasError) {
    for (int j = 0; j < numberOrder; j++) {
        if (((INT_MAX / radix) - value) < 0) {
            wasError = true;
            return 0;
        }
        value *= radix;
    }
    return value;
}

int StringToInt(const std::string& str, int radix, bool & wasError) {
    int numberOrder = (int) (str.length() - 1);
    int resultValue = 0;
    for (int i = 0; i < str.length(); i++) {
        char ch = toupper(str[i]);
        if (hexToInt.find(ch) == hexToInt.end()) {
            wasError = true;
            return 0;
        }
        int orderNumber = hexToInt[ch];
        if (orderNumber >= radix) {
            wasError = true;
            return 0;
        }
        int orderResultValue = exponentiate(orderNumber, radix, numberOrder, wasError);
        if (wasError) {
            return 0;
        }
        if ((orderResultValue - INT_MAX + resultValue) > 0) {
            wasError = true;
            return 0;
        }
        resultValue += orderResultValue;
        numberOrder--;
    }
    return resultValue;
}

void reverseStr(std::string& str)
{
    int n = (int) str.length();
    for (int i = 0; i < n / 2; i++) {
        std::swap(str[i], str[n - i - 1]);
    }
}

std::string IntToStr(int n, int radix) {
    std::map<int, char> intToHex;
    for (std::map<char, int>::iterator i = hexToInt.begin(); i != hexToInt.end(); ++i)
        intToHex[i->second] = i->first;
    std::string str = "";
    do {
        str += intToHex[n % radix];
    } while (n /= radix);
    reverseStr(str);
    return str;
}


int main(int argc, const char * argv[]) {
    if (argc != 4) {
        std::cout << "Invalid arguments, use: radix <source notation> <destination notation> <value>\n";
        return 1;
    }
    char * p;
    int srcNotation = (int) strtol(argv[1], &p, 10);
    if (srcNotation < 2 || srcNotation > MAX_NOTATION) {
        std::cout << "Source notation is out of range 2..36\n";
        return 1;
    }
    int dstNotation = (int) strtol(argv[2], &p, 10);
    if (dstNotation < 2 || dstNotation > MAX_NOTATION) {
        std::cout << "Destination notation is out of range 2..36\n";
        return 1;
    }
    std::string initValue = argv[3];
    bool isNegative = false;
    if (initValue[0] == '-') {
        initValue.erase(0, 1);
        isNegative = true;
    }
    bool wasError = false;
    int decimalSystemValue = StringToInt(initValue, srcNotation, wasError);
    if (wasError) {
        std::cout << "Converting string to int error\n";
        return 1;
    }
    if (isNegative) {
        std::cout << '-';
    }
    std::cout << IntToStr(decimalSystemValue, dstNotation) << "\n";
    return 0;
}
