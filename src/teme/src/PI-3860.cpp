#include <map>
#include <cmath>
#include <iostream>

using namespace std;

const int NMAX = 1e5;
int values[NMAX + 5];
int distinct[NMAX + 5], frequency[NMAX + 5];
int maxValue[20][NMAX + 5], minValue[20][NMAX + 5];

int main() {
    int valueCount, queryCount;

    cin >> valueCount;
    for (int valueIndex = 1; valueIndex <= valueCount; valueIndex++) {
        cin >> values[valueIndex];
        maxValue[0][valueIndex] = values[valueIndex];
        minValue[0][valueIndex] = values[valueIndex];
    }

    int position = 1;
    for (int valueIndex = 1; valueIndex <= valueCount; valueIndex++) {
        while (frequency[values[valueIndex]]) {
            frequency[values[position]]--;
            position++;
        }
        frequency[values[valueIndex]]++;
        distinct[valueIndex] = position;
    }

    for (int powerIndex = 1; (1 << powerIndex) <= valueCount; powerIndex++)
        for (int valueIndex = 1; valueIndex <= valueCount - (1 << powerIndex) + 1; valueIndex++) {
            maxValue[powerIndex][valueIndex] = max(maxValue[powerIndex - 1][valueIndex],
                                                   maxValue[powerIndex - 1][valueIndex + (1 << (powerIndex - 1))]);
            minValue[powerIndex][valueIndex] = min(minValue[powerIndex - 1][valueIndex],
                                                   minValue[powerIndex - 1][valueIndex + (1 << (powerIndex - 1))]);
        }

    cin >> queryCount;
    for (int queryIndex = 1; queryIndex <= queryCount; queryIndex++) {
        int startPosition, endPosition;
        cin >> startPosition >> endPosition;
        if (distinct[endPosition] <= startPosition) {
            int powerIndex = log2(endPosition - startPosition + 1);
            int minValueInRange = min(minValue[powerIndex][startPosition],
                                      minValue[powerIndex][endPosition - (1 << powerIndex) + 1]);
            int maxValueInRange = max(maxValue[powerIndex][startPosition],
                                      maxValue[powerIndex][endPosition - (1 << powerIndex) + 1]);
            cout << ((maxValueInRange - minValueInRange == endPosition - startPosition) ? "1" : "0");
        } else
            cout << "0";
    }
    return 0;
}