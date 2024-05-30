#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

const int MAX_SIZE = 400;
int matrix[MAX_SIZE + 5][MAX_SIZE + 5];
int matrixGcdLines[MAX_SIZE + 5][MAX_SIZE + 5];
int matrixGcdColumns[MAX_SIZE + 5][MAX_SIZE + 5];

inline int gcd(int a, int b)
{
    if (a == 0 || b == 0)
        return 0;
    int r;
    while (b)
    {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

inline void generateGcdColumns(int lineCount, int columnCount, int width)
{
    int lineGcd[10][MAX_SIZE + 5];
    for (int lineIndex = 1; lineIndex <= lineCount; lineIndex++)
    {
        for (int columnIndex = 1; columnIndex <= columnCount; columnIndex++)
            lineGcd[0][columnIndex] = matrix[lineIndex][columnIndex];
        for (int powerIndex = 1; powerIndex <= log2(columnCount); powerIndex++)
            for (int columnIndex = 1; columnIndex <= columnCount - (1 << powerIndex) + 1; columnIndex++)
                lineGcd[powerIndex][columnIndex] = gcd(lineGcd[powerIndex - 1][columnIndex], lineGcd[powerIndex - 1][columnIndex + (1 << (powerIndex - 1))]);
        for (int columnIndex = 1; columnIndex <= columnCount - width + 1; columnIndex++)
        {
            int powerIndex = (int)log2(width);
            matrixGcdColumns[lineIndex][columnIndex] = gcd(lineGcd[powerIndex][columnIndex], lineGcd[powerIndex][columnIndex + width - (1 << powerIndex)]);
        }
    }
}

inline void generateGcdLines(int lineCount, int columnCount, int height)
{
    int columnGcd[10][MAX_SIZE + 5];
    for (int columnIndex = 1; columnIndex <= columnCount; columnIndex++)
    {
        for (int lineIndex = 1; lineIndex <= lineCount; lineIndex++)
            columnGcd[0][lineIndex] = matrixGcdColumns[lineIndex][columnIndex];
        for (int powerIndex = 1; powerIndex <= log2(lineCount); powerIndex++)
            for (int lineIndex = 1; lineIndex <= lineCount - (1 << powerIndex) + 1; lineIndex++)
                columnGcd[powerIndex][lineIndex] = gcd(columnGcd[powerIndex - 1][lineIndex], columnGcd[powerIndex - 1][lineIndex + (1 << (powerIndex - 1))]);
        for (int lineIndex = 1; lineIndex <= lineCount - height + 1; lineIndex++)
        {
            int powerIndex = (int)log2(height);
            matrixGcdLines[lineIndex][columnIndex] = gcd(columnGcd[powerIndex][lineIndex], columnGcd[powerIndex][lineIndex + height - (1 << powerIndex)]);
        }
    }
}

inline void cleanUp(int lineCount, int columnCount)
{
    for (int lineIndex = 1; lineIndex <= lineCount; lineIndex++)
        for (int columnIndex = 1; columnIndex <= columnCount; columnIndex++)
        {
            matrix[lineIndex][columnIndex] = 0;
            matrixGcdLines[lineIndex][columnIndex] = 0;
            matrixGcdColumns[lineIndex][columnIndex] = 0;
        }
}

int main()
{
    ifstream in("euclid.in");
    ofstream out("euclid.out");

    int testCount, lineCount, columnCount, height, width, value;

    in >> testCount;
    for (int testIndex = 1; testIndex <= testCount; testIndex++)
    {
        in >> lineCount >> columnCount >> height >> width;
        for (int lineIndex = 1; lineIndex <= lineCount; lineIndex++)
            for (int columnIndex = 1; columnIndex <= columnCount; columnIndex++)
                in >> matrix[lineIndex][columnIndex];

        generateGcdColumns(lineCount, columnCount, width);
        generateGcdLines(lineCount, columnCount, height);

        int ans = 0;
        for (int lineIndex = 1; lineIndex <= lineCount; lineIndex++)
            for (int columnIndex = 1; columnIndex <= columnCount; columnIndex++)
                ans = max(ans, matrixGcdLines[lineIndex][columnIndex]);
        out << "Case #" << testIndex << ": " << ans << endl;

        cleanUp(lineCount, columnCount);
    }
    return 0;
}