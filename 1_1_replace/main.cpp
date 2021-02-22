#include <iostream>
#include <fstream>
#include <optional>

int main(int argc, char * argv[]) {
    if (argc != 5) {
        std::cout << "Invalid arguments, use: replace <input file> <output file> <search string> <replace string>\n";
        return 1;
    }
    char * inputFileName = argv[1];
    std::ifstream input(inputFileName);
    if (!input.is_open()) {
        std::cout << "Failed to open input file " << inputFileName << "\n";
        return 1;
    }
    char * outputFileName = argv[2];
    std::ofstream output(outputFileName);
    if (!input.is_open()) { 
        std::cout << "Failed to open output file " << outputFileName << "\n";
        return 1;
    }
    std::string searchStr = argv[3];
    std::string replaceStr = argv[4];
    std::string resultingSearchStr = "";
    int expectedSearchChPos = 0;
    char expectedSearchCh = '\0';
    char ch;
    while (input.get(ch)) {
        if (searchStr.length() != 0) {
            expectedSearchCh = searchStr[expectedSearchChPos];
        }
        if (searchStr.length() != 0 && ch == expectedSearchCh) {
            resultingSearchStr += ch;
            if (resultingSearchStr == searchStr) {
                output << replaceStr;
                resultingSearchStr = "";
                expectedSearchChPos = 0;
            } else {
                expectedSearchChPos++;
            }
        } else {
            if (resultingSearchStr.length() != 0) {
                output << resultingSearchStr;
                resultingSearchStr = "";
                expectedSearchChPos = 0;
            }
            if (!output.put(ch)) {
                std::cout << "Failed to save data to file " << outputFileName << "\n";
                return 1;
            }
        }
    }
    if (!output.flush()) {
        std::cout << "Failed to save data to file " << outputFileName << "\n";
        return 1;
    }
    return 0;
}
