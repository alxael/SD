#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

const int NMAX = 5e2;
int mx[10][NMAX + 5][NMAX + 5];
// mx[k][i][j] -> holds maximum value for squares of size (1 << k) with top left corner at line i, column j

int main()
{
    ifstream in("plantatie.in");
    ofstream out("plantatie.out");

    int n, m, x, y, z;
    in >> n >> m;
    for (int lineIndex = 1; lineIndex <= n; lineIndex++)
        for (int columnIndex = 1; columnIndex <= n; columnIndex++)
            in >> mx[0][lineIndex][columnIndex];

    for (int dimension = 1; dimension <= (int)log2(n); dimension++)
        for (int lineIndex = 1; lineIndex <= n - (1 << dimension) + 1; lineIndex++)
            for (int columnIndex = 1; columnIndex <= n - (1 << dimension) + 1; columnIndex++)
                mx[dimension][lineIndex][columnIndex] = max(mx[dimension - 1][lineIndex][columnIndex],
                                                            max(mx[dimension - 1][lineIndex + (1 << (dimension - 1))][columnIndex],
                                                                max(mx[dimension - 1][lineIndex][columnIndex + (1 << (dimension - 1))],
                                                                    mx[dimension - 1][lineIndex + (1 << (dimension - 1))][columnIndex + (1 << (dimension - 1))])));

    for (int queryIndex = 1; queryIndex <= m; queryIndex++)
    {
        in >> x >> y >> z;
        int dimension = log2(z);
        out << max(mx[dimension][x][y],
                    max(mx[dimension][x + z - (1 << dimension)][y],
                        max(mx[dimension][x][y + z - (1 << dimension)],
                            mx[dimension][x + z - (1 << dimension)][y + z - (1 << dimension)])))
             << endl;
    }
    return 0;
}