#include <fstream>
#include <iostream>

using namespace std;

const int MAX_SIZE = 1e5;

int valueCount;
int values[MAX_SIZE + 5];
pair<int, int> segmentTree[4 * MAX_SIZE + 5];

void propagate(int node)
{
    segmentTree[node * 2].second += segmentTree[node].second;
    segmentTree[node * 2 + 1].second += segmentTree[node].second;
    segmentTree[node].second = 0;
}

void update(int node, int left, int right, int updateLeft, int updateRight, int delta)
{
    if (left == right)
    {
        segmentTree[node].first += delta;
        return;
    }
    if (updateLeft <= left && right <= updateRight)
    {
        segmentTree[node].second += delta;
        return;
    }
    propagate(node);

    int middle = (left + right) / 2;
    if (updateLeft <= middle)
        update(node * 2, left, middle, updateLeft, updateRight, delta);
    if (middle < updateRight)
        update(node * 2 + 1, middle + 1, right, updateLeft, updateRight, delta);

    segmentTree[node].first =
        min(segmentTree[node * 2].first + segmentTree[node * 2].second,
            segmentTree[node * 2 + 1].first + segmentTree[node * 2 + 1].second);
}

int query(int node, int left, int right, int updateLeft, int updateRight)
{
    if (segmentTree[node].second || updateLeft <= left && right <= updateRight)
        return segmentTree[node].first + segmentTree[node].second;
    propagate(node);

    int middle = (left + right) / 2;
    int answerLeft = valueCount, answerRight = valueCount;
    if (updateLeft <= middle)
        answerLeft = query(node * 2, left, middle, updateLeft, updateRight);
    if (middle < updateRight)
        answerRight = query(node * 2 + 1, middle + 1, right, updateLeft, updateRight);
    return min(answerLeft, answerRight);
}

int main()
{
    ifstream cin("sirbun.in");
    ofstream cout("sirbun.out");

    cin >> valueCount;
    for (int valueIndex = 1; valueIndex <= valueCount; valueIndex++)
        cin >> values[valueIndex];

    for (int valueIndex = 1; valueIndex <= valueCount; valueIndex++)
        update(1, 1, valueCount, valueIndex, valueIndex, valueIndex - 1);

    long long answer = 0;
    int left = 1;
    for (int right = 1; right <= valueCount; right++)
    {
        update(1, 1, valueCount, 1, values[right], 1);
        int length;
        while (true)
        {
            length = right - left + 1;
            int minimum = query(1, 1, valueCount, 1, length);
            if (minimum >= length)
                break;
            update(1, 1, valueCount, 1, values[left], -1);
            ++left;
        }
        answer += length;
    }

    cout << answer;

    return 0;
}