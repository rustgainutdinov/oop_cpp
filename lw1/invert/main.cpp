#include <iostream>   // Так надо
#include <vector>     // Для вектора
#include <fstream>

using namespace std;

const int ROWS_COUNT = 3;
const int COLUMNS_COUNT = 3;

void AssertValidArguments(int argc)
{
    if (argc != 2)
    {
        throw invalid_argument("Invalid argument    s, use: invert <input file>");
    }
}

vector<vector<double>> GetMatrixFromFile(const string &mFileName)
{
    fstream in(mFileName);
    if (!in.is_open())
    {
        throw invalid_argument("Failed to open input file");
    }
    vector<vector<double>> matrix(ROWS_COUNT, vector<double>(COLUMNS_COUNT));
    for (int i = 0; i < ROWS_COUNT; i++)
        for (int j = 0; j < COLUMNS_COUNT; j++)
            in >> matrix[i][j];
    return matrix;
}

template<class T>
void PrintMatrix(const vector<vector<T>> &matrix)
{
    for (const auto &row : matrix)
    {
        for (double el : row)
            cout << el << " ";
        cout << "\n";
    }
}


int GetDet(const vector<vector<double>> &m)
{
    return m[0][0] * m[1][1] * m[2][2] +
           m[1][0] * m[0][2] * m[2][1] +
           m[0][1] * m[1][2] * m[2][0] -
           m[2][0] * m[1][1] * m[0][2] -
           m[0][0] * m[1][2] * m[2][1] -
           m[2][2] * m[0][1] * m[1][0];
}

vector<vector<double>> GetTransportedMatrix(const vector<vector<double>> &matrix)
{
    vector<vector<double>> transportedMatrix(ROWS_COUNT, vector<double>(COLUMNS_COUNT));
    for (int i = 0; i < ROWS_COUNT; i++)
        for (int j = 0; j < COLUMNS_COUNT; j++)
            transportedMatrix[j][i] = matrix[i][j];
    return transportedMatrix;
}

vector<vector<double>> GetAlgebraicAdditionMatrix(const vector<vector<double>> &matrix, int excludeRow, int excludeCol)
{
    vector<vector<double>> algebraicAdditionMatrix(2, vector<double>(2));
    int algebraicAdditionI = 0;
    for (int i = 0; i < ROWS_COUNT; i++)
    {
        if (i == excludeRow) continue;
        int algebraicAdditionJ = 0;
        for (int j = 0; j < COLUMNS_COUNT; j++)
        {
            if (j == excludeCol) continue;
            algebraicAdditionMatrix[algebraicAdditionI][algebraicAdditionJ] = matrix[i][j];
            algebraicAdditionJ++;
        }
        algebraicAdditionI++;
    }
    return algebraicAdditionMatrix;
}

vector<vector<double>> FindInverseMatrix(const vector<vector<double>> &matrix)
{
    int det = GetDet(matrix);
    if (det == 0)
    {
        throw invalid_argument("Finding inverse matrix is impossible");
    }
    vector<vector<double>> invertedMatrix(ROWS_COUNT, vector<double>(COLUMNS_COUNT));
    vector<vector<double>> transportedMatrix = GetTransportedMatrix(matrix);
    for (int i = 0; i < ROWS_COUNT; i++)
    {
        for (int j = 0; j < COLUMNS_COUNT; j++)
        {
            vector<vector<double>> algebraicAdditionMatrix = GetAlgebraicAdditionMatrix(transportedMatrix, i, j);
            invertedMatrix[i][j] = (((i + j) % 2) == 0 ? 1 : -1) * (
                    algebraicAdditionMatrix[0][0] * algebraicAdditionMatrix[1][1] -
                    algebraicAdditionMatrix[0][1] * algebraicAdditionMatrix[1][0]
            );
        }
    }
    return invertedMatrix;
}

int main(int argc, const char *argv[])
{
    try
    {
        AssertValidArguments(argc);
        string mFileName = argv[1];
        vector<vector<double>> matrix = GetMatrixFromFile(mFileName);
        vector<vector<double>> invertedMatrix = FindInverseMatrix(matrix);
        PrintMatrix(invertedMatrix);
    }
    catch (const exception &e)
    {
        cout << e.what() << "\n";
        return 1;
    }
}
