#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("pikachu.in");
    ofstream cout("pikachu.out");

    int valueCount, value, length;
    vector<int> values;

    cin >> valueCount >> length;
    for (int valueIndex = 0; valueIndex < valueCount; valueIndex++) {
        cin >> value;
        values.emplace_back(value);
    }

    if (length <= 1) {
        cout << 0;
        return 0;
    }

    if (length == 2) {
        int solution = abs(values[1] - values[0]);
        for (int valueIndex = 1; valueIndex < values.size(); valueIndex++)
            solution = min(solution, abs(values[valueIndex] - values[valueIndex - 1]));
        cout << solution;
        return 0;
    }

    set<int> current;
    vector<int> auxiliary;
    long long solution = 0, sum = 0;

    for (int valueIndex = 0; valueIndex < length; valueIndex++) {
        current.insert(values[valueIndex]);
        auxiliary.emplace_back(values[valueIndex]);
    }
    nth_element(auxiliary.begin(), auxiliary.begin() + (length + 1) / 2 - 1, auxiliary.end());
    int median = auxiliary[(length + 1) / 2 - 1];
    for (int valueIndex = 0; valueIndex < length; valueIndex++)
        sum += abs(median - values[valueIndex]);

    solution = sum;
    for (int valueIndex = length; valueIndex < values.size(); valueIndex++) {
        sum -= abs(median - values[valueIndex - length]);
        auto it = current.find(median);

        auto itprev = it;
        itprev--;
        int previous = *itprev;

        auto itnext = it;
        itnext++;
        int next = *itnext;

        int previousMedian = median;
        current.erase(current.find(values[valueIndex - length]));
        current.insert(values[valueIndex]);

        int deletedValue = values[valueIndex - length];
        int insertedValue = values[valueIndex];

        if (deletedValue == median) {
            if (previous <= insertedValue && insertedValue <= next) median = insertedValue;
            else median = (insertedValue < previous) ? previous : next;
        }
        if (deletedValue == previous) {
            if (median <= insertedValue && insertedValue <= next) median = insertedValue;
            else if (insertedValue >= median) median = next;
        }
        if (deletedValue == next) {
            if (previous <= insertedValue && insertedValue <= median) median = insertedValue;
            else if (insertedValue < previous) median = previous;
        }
        if (deletedValue < previous) {
            if (insertedValue > next) median = next;
            else if (insertedValue > median) median = insertedValue;
        }
        if (deletedValue > next) {
            if (insertedValue < previous) median = previous;
            else if (insertedValue < median) median = insertedValue;
        }

        sum += abs(median - insertedValue);
        if (length % 2 == 0) sum += previousMedian - median;
        solution = min(solution, sum);
    }

    cout << solution;
    return 0;
}
