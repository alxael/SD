#include <fstream>
#include <iostream>

using namespace std;

const int MAX_SIZE = 1e6;
pair<int, long long> segmentTree[4 * MAX_SIZE + 5];

void update(int node, int left, int right, int value)
{
    if (left == right)
    {
        segmentTree[node].first++;
        segmentTree[node].second += value;
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

void query(int node, int left, int right, int index, pair<int, long long> &result, int &largestValueNode)
{
    if (left == right)
        return;
    int middle = (left + right) / 2;
    if (index > middle)
    {
        result.first += segmentTree[node * 2].first;
        result.second += segmentTree[node * 2].second;
        largestValueNode = node;
        query(node * 2 + 1, middle + 1, right, index, result, largestValueNode);
    }
    else
        query(node * 2, left, middle, index, result, largestValueNode);
}

int main()
{
    // ifstream cin("files/teme.in");
    // ofstream cout("files/teme.out");

    int operationCount, operationType, operationValue;
    cin >> operationCount;
    for (int operationIndex = 1; operationIndex <= operationCount; operationIndex++)
    {
        cin >> operationType >> operationValue;
        if (operationType == 1)
            update(1, 1, MAX_SIZE, operationValue);
        else
        {
            int left = 1, right = MAX_SIZE, answerLargestValueNode = 0;
            pair<int, long long> answer;
            while (left <= right)
            {
                int middle = (left + right) / 2;

                pair<int, long long> queryResult;
                int queryLargestValueNode = 0;
                query(1, 1, MAX_SIZE, middle, queryResult, queryLargestValueNode);

                if (queryResult.first >= operationValue)
                {
                    answer = queryResult;
                    answerLargestValueNode = queryLargestValueNode;
                    right = middle - 1;
                }
                else
                    left = middle + 1;
            }

            int valuesToSubtract = answer.first - operationValue;
            if (segmentTree[answerLargestValueNode].first)
                answer.second -= (segmentTree[answerLargestValueNode].second / segmentTree[answerLargestValueNode].first) * valuesToSubtract;
            cout << answer.second << endl;
        }
    }
    return 0;
}