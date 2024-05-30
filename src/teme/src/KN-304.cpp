#include <limits>
#include <fstream>
#include <iostream>

using namespace std;

struct Node
{
    long long sum;
    long long maxPrefix;
    long long maxSuffix;
    long long maxSequence;

    void setLeaf(long long value) { sum = maxPrefix = maxSuffix = maxSequence = value; }
};

const int MAX_SIZE = 1e6;
int values[MAX_SIZE + 5];
Node segmentTree[2][4 * MAX_SIZE + 5];

void build(int node, int left, int right)
{
    if (left == right)
    {
        segmentTree[0][node].setLeaf(((left & 1) ? 1 : -1) * values[left]);
        segmentTree[1][node].setLeaf(((left & 1) ? -1 : 1) * values[left]);
        return;
    }

    int middle = (left + right) / 2;
    build(2 * node, left, middle);
    build(2 * node + 1, middle + 1, right);

    for (int treeIndex = 0; treeIndex <= 1; treeIndex++)
    {
        segmentTree[treeIndex][node].sum = segmentTree[treeIndex][2 * node].sum + segmentTree[treeIndex][2 * node + 1].sum;
        segmentTree[treeIndex][node].maxPrefix = max(segmentTree[treeIndex][2 * node].maxPrefix,
                                                     segmentTree[treeIndex][2 * node].sum + segmentTree[treeIndex][2 * node + 1].maxPrefix);
        segmentTree[treeIndex][node].maxSuffix = max(segmentTree[treeIndex][2 * node + 1].maxSuffix,
                                                     segmentTree[treeIndex][2 * node + 1].sum + segmentTree[treeIndex][2 * node].maxSuffix);
        segmentTree[treeIndex][node].maxSequence = max(segmentTree[treeIndex][node].maxPrefix,
                                                       max(segmentTree[treeIndex][node].maxSuffix,
                                                           max(segmentTree[treeIndex][2 * node].maxSequence,
                                                               segmentTree[treeIndex][2 * node + 1].maxSequence)));
        segmentTree[treeIndex][node].maxSequence = max(segmentTree[treeIndex][node].maxSequence,
                                                       segmentTree[treeIndex][2 * node].maxSuffix + segmentTree[treeIndex][2 * node + 1].maxPrefix);
    }
}

void update(int node, int left, int right, int position, int value)
{
    if (left == right)
    {
        segmentTree[0][node].setLeaf(((left & 1) ? 1 : -1) * value);
        segmentTree[1][node].setLeaf(((left & 1) ? -1 : 1) * value);
        return;
    }

    int middle = (left + right) / 2;
    if (position <= middle)
        update(2 * node, left, middle, position, value);
    else
        update(2 * node + 1, middle + 1, right, position, value);

    for (int treeIndex = 0; treeIndex <= 1; treeIndex++)
    {
        segmentTree[treeIndex][node].sum = segmentTree[treeIndex][2 * node].sum + segmentTree[treeIndex][2 * node + 1].sum;
        segmentTree[treeIndex][node].maxPrefix = max(segmentTree[treeIndex][2 * node].maxPrefix,
                                                     segmentTree[treeIndex][2 * node].sum + segmentTree[treeIndex][2 * node + 1].maxPrefix);
        segmentTree[treeIndex][node].maxSuffix = max(segmentTree[treeIndex][2 * node + 1].maxSuffix,
                                                     segmentTree[treeIndex][2 * node + 1].sum + segmentTree[treeIndex][2 * node].maxSuffix);
        segmentTree[treeIndex][node].maxSequence = max(segmentTree[treeIndex][node].maxPrefix,
                                                       max(segmentTree[treeIndex][node].maxSuffix,
                                                           max(segmentTree[treeIndex][2 * node].maxSequence,
                                                               segmentTree[treeIndex][2 * node + 1].maxSequence)));
        segmentTree[treeIndex][node].maxSequence = max(segmentTree[treeIndex][node].maxSequence,
                                                       segmentTree[treeIndex][2 * node].maxSuffix + segmentTree[treeIndex][2 * node + 1].maxPrefix);
    }
}

long long sumPositive = 0, maxSumPositive = 0;
long long sumNegative = 0, maxSumNegative = 0;

void query(int node, int left, int right, int leftQuery, int rightQuery)
{
    if (right < leftQuery || left > rightQuery)
        return;
    if (leftQuery <= left && right <= rightQuery)
    {
        if (sumPositive + segmentTree[0][node].maxPrefix > maxSumPositive)
            maxSumPositive = sumPositive + segmentTree[0][node].maxPrefix;
        sumPositive = max(sumPositive + segmentTree[0][node].sum,
                          max(segmentTree[0][node].maxSuffix,
                              segmentTree[0][node].sum));
        if (segmentTree[0][node].maxSequence > maxSumPositive)
            maxSumPositive = segmentTree[0][node].maxSequence;
        if (sumPositive > maxSumPositive)
            maxSumPositive = sumPositive;

        if (sumNegative + segmentTree[1][node].maxPrefix > maxSumNegative)
            maxSumNegative = sumNegative + segmentTree[1][node].maxPrefix;
        sumNegative = max(sumNegative + segmentTree[1][node].sum,
                          max(segmentTree[1][node].maxSuffix,
                              segmentTree[1][node].sum));
        if (segmentTree[1][node].maxSequence > maxSumNegative)
            maxSumNegative = segmentTree[1][node].maxSequence;
        if (sumNegative > maxSumNegative)
            maxSumNegative = sumNegative;
        return;
    }

    int middle = (left + right) / 2;
    query(2 * node, left, middle, leftQuery, rightQuery);
    query(2 * node + 1, middle + 1, right, leftQuery, rightQuery);
}
int main()
{
    // ifstream cin("files/teme.in");
    // ofstream cout("files/teme.out");

    int valueCount, queryCount;
    cin >> valueCount >> queryCount;
    for (int valueIndex = 1; valueIndex <= valueCount; valueIndex++)
        cin >> values[valueIndex];

    build(1, 1, valueCount);

    for (int queryIndex = 1; queryIndex <= queryCount; queryIndex++)
    {
        int queryType, queryValueOne, queryValueTwo;
        cin >> queryType >> queryValueOne >> queryValueTwo;
        if (queryType == 1)
            update(1, 1, valueCount, queryValueOne, queryValueTwo);
        else
        {
            sumPositive = 0;
            sumNegative = 0;
            maxSumPositive = numeric_limits<long long>::min();
            maxSumNegative = numeric_limits<long long>::min();

            query(1, 1, valueCount, queryValueOne, queryValueTwo);

            long long answer = (queryValueOne & 1) ? maxSumPositive : maxSumNegative;

            sumPositive = 0;
            sumNegative = 0;
            maxSumPositive = numeric_limits<long long>::min();
            maxSumNegative = numeric_limits<long long>::min();

            if (queryValueOne < queryValueTwo)
            {
                query(1, 1, valueCount, queryValueOne + 1, queryValueTwo);
                answer = max(answer, ((queryValueOne & 1) ? maxSumNegative : maxSumPositive));
            }

            cout << answer << endl;
        }
    }

    return 0;
}