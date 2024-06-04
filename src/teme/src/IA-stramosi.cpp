#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

// descendant[i][j] -> the (1<<i)th descendant of j
const int NMAX = 25e4;
int descendant[20][NMAX + 5];

int main() {
    ifstream cin("stramosi.in");
    ofstream cout("stramosi.out");

    int memberCount, queryCount;

    cin >> memberCount >> queryCount;
    for (int memberIndex = 1; memberIndex <= memberCount; memberIndex++)
        cin >> descendant[0][memberIndex];

    for (int powerIndex = 1; (1 << powerIndex) <= memberCount; powerIndex++) {
        descendant[powerIndex - 1][0] = 0;
        for (int memberIndex = 1; memberIndex <= memberCount; memberIndex++)
            descendant[powerIndex][memberIndex] = descendant[powerIndex - 1][descendant[powerIndex - 1][memberIndex]];
    }

    for(int queryIndex = 1; queryIndex <= queryCount; queryIndex ++) {
        int person, descendantsCount;
        cin >> person >> descendantsCount;

        int current = person;
        while(descendantsCount > 0) {
            int powerIndex = log2(descendantsCount);
            current = descendant[powerIndex][current];
            descendantsCount -= (1 << powerIndex);
        }

        cout << current << endl;
    }

    return 0;
}