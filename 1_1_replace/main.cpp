#include <iostream>
#include <fstream>
#include <optional>

using namespace std;

void AssertValidArguments(int argc)
{
    if (argc != 5)
    {
        throw invalid_argument(
                "Invalid arguments, use: Replace <input file> <output file> <search string> <Replace string>");
    }
}


template<typename T>
T OpenFile(const string &fileName)
{
    T file(fileName, ios_base::binary);
    if (!file.is_open())
    {
        throw invalid_argument("Failed to open file \"" + fileName + "\"");
    }
    return file;
}

ifstream GetInput(char **argv)
{
    return OpenFile<ifstream>(argv[1]);
}

ofstream GetOutput(char **argv)
{
    return OpenFile<ofstream>(argv[2]);
}

void PutChToFile(char ch, ofstream &file)
{
    if (!file.put(ch))
    {
        throw invalid_argument("Failed to save data to file");
    }
}

//TODO: use size_t instead int
void Replace(ifstream &input, ofstream &output, const string &searchStr, const string &replaceStr)
{
    string resultingSearchStr;
    int expectedSearchChPos = 0;
    char ch;
    while (input.get(ch))
    {
        if (searchStr.empty())
        {
            PutChToFile(ch, output);
            continue;
        }
        bool isChEqualExpectedCh = (ch == searchStr[expectedSearchChPos]);
        if (isChEqualExpectedCh)
        {
            resultingSearchStr += ch;
            expectedSearchChPos++;
        }
        else
        {
            output << resultingSearchStr;
            PutChToFile(ch, output);
        }
        if (resultingSearchStr == searchStr)
        {
            output << replaceStr;
        }
        if ((resultingSearchStr == searchStr) || (!isChEqualExpectedCh))
        {
            resultingSearchStr.clear();
            expectedSearchChPos = 0;
        }
    }
    if (!output.flush())
    {
        throw invalid_argument("Failed to save data to file");
    }
}

int main(int argc, char *argv[])
{
    try
    {
        AssertValidArguments(argc);
        string searchStr = argv[3];
        string replaceStr = argv[4];
        ifstream input = GetInput(argv);
        ofstream output = GetOutput(argv);
        Replace(input, output, searchStr, replaceStr);
    }
    catch (const exception &e)
    {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}