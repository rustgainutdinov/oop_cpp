#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

using namespace std;

enum CryptMode
{
    ENCRYPT,
    DECRYPT
};

string const ENCRYPT_WORKING_MODE_TEXT = "encrypt";
string const DECRYPT_WORKING_MODE_TEXT = "decrypt";

void AssertValidArguments(int argc)
{
    if (argc != 5)
    {
        throw invalid_argument(
                "Invalid arguments, use: \"crypt encrypt <input file> <output file> <key>\" to crypt, or\n"
                "\"crypt decrypt <input file> <output file> <key>\" to crypt");
    }
}

CryptMode GetCryptMode(char **argv)
{
    string cryptMode = argv[1];
    if (cryptMode == ENCRYPT_WORKING_MODE_TEXT)
    {
        return ENCRYPT;
    }
    else if (cryptMode == DECRYPT_WORKING_MODE_TEXT)
    {
        return DECRYPT;
    }
    throw invalid_argument("Unknown working mode");
}

int GetCryptKey(char *argv[])
{
    istringstream iss(argv[4]);
    int cryptKey;
    if (iss >> cryptKey && cryptKey > 0 && cryptKey < 255)
    {
        return cryptKey;
    }
    throw invalid_argument("Invalid crypt key");
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
    return OpenFile<ifstream>(argv[2]);
}

ofstream GetOutput(char **argv)
{
    return OpenFile<ofstream>(argv[3]);
}

vector<int> GetBufBits(int buf)
{
    vector<int> bits(8);
    int mask = 0x1;
    for (int i = 0; i < 8; i++)
    {
        bits[i] = buf & mask;
        mask <<= 1;
    }
    return bits;
}

int ProcessBuf(int buf, CryptMode cryptMode, int cryptKey)
{
    int processedBuf;
    if (cryptMode == ENCRYPT)
    {
        vector<int> bits = GetBufBits(buf);
        processedBuf = bits[0] << 2;
        processedBuf += bits[1] << 2;
        processedBuf += bits[2] << 2;
        processedBuf += bits[3] << 3;
        processedBuf += bits[4] << 3;
        processedBuf += bits[5] >> 5;
        processedBuf += bits[6] >> 5;
        processedBuf += bits[7] >> 2;
        processedBuf ^= cryptKey;
    }
    else
    {
        buf ^= cryptKey;
        vector<int> bits = GetBufBits(buf);
        processedBuf = bits[0] << 5;
        processedBuf += bits[1] << 5;
        processedBuf += bits[2] >> 2;
        processedBuf += bits[3] >> 2;
        processedBuf += bits[4] >> 2;
        processedBuf += bits[5] << 2;
        processedBuf += bits[6] >> 3;
        processedBuf += bits[7] >> 3;
    }
    return processedBuf;
}

void Crypt(ifstream &input, ofstream &output, CryptMode cryptMode, int cryptKey)
{
    char buf;
    do
    {
        input.read(&buf, sizeof(char));
        char cryptBuf = (char) ProcessBuf((int) buf, cryptMode, cryptKey);
        output.write(&cryptBuf, sizeof(char));
    } while (input.gcount() > 0);
}

int main(int argc, char *argv[])
{
    try
    {
        AssertValidArguments(argc);
        CryptMode cryptMode = GetCryptMode(argv);
        int cryptKey = GetCryptKey(argv);
        ifstream input = GetInput(argv);
        ofstream output = GetOutput(argv);
        Crypt(input, output, cryptMode, cryptKey);
    }
    catch (exception &e)
    {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}