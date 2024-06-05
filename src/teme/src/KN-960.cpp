#include <fstream>
#include <iostream>

using namespace std;

const int MAX_SIZE = 1e6;
pair<int, long long> segmentTree[4 * MAX_SIZE + 5];

void update(int node, int left, int right, int value) {
    if (left == right) {
        segmentTree[node].first++;
        segmentTree[node].second += left;
        return;
    }
    int middle = (left + right) / 2;
    if (left <= value && value <= middle)
        update(node * 2, left, middle, value);
    if (middle + 1 <= value && value <= right)
        update(node * 2 + 1, middle + 1, right, value);
    segmentTree[node].first = segmentTree[2 * node].first + segmentTree[2 * node + 1].first;
    segmentTree[node].second = segmentTree[2 * node].second + segmentTree[2 * node + 1].second;
}

long long query(int node, int left, int right, int value, long long sum) {
    if (left == right)
        return sum + value * right;
    int middle = (left + right) / 2;
    if (segmentTree[2 * node].first >= value)
        return query(node * 2, left, middle, value, sum);
    else
        return query(node * 2 + 1, middle + 1, right, value - segmentTree[2 * node].first,
                     sum + segmentTree[2 * node].second);
}

int main() {
//    ifstream cin("expectedgoals.in");
//    ofstream cout("expectedgoals.out");

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int operationCount, operationType, operationValue;
    cin >> operationCount;
    for (int operationIndex = 1; operationIndex <= operationCount; operationIndex++) {
        cin >> operationType >> operationValue;
        if (operationType == 1)
            update(1, 1, MAX_SIZE, operationValue);
        else
            cout << query(1, 1, MAX_SIZE, operationValue, 0) << endl;
    }
    return 0;
}