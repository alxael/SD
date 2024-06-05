#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

const int MOD = 666013;

struct Query {
    int type, position, value;

    Query() : type(0), position(0), value(0) {}
};

template<typename Type>
class BinaryIndexedTree {
private:
    int valueCount;
    vector<Type> tree;

    inline static int findNextPowerOfTwo(const int x) {
        int answer = 1;
        while (answer < x) {
            answer <<= 1;
        }
        return answer;
    }

public:

    BinaryIndexedTree(int valueCount, Type initializationValue = 0) : valueCount(valueCount + 1),
                                                                tree(valueCount + 1, initializationValue) {
        for (int valueIndex = 1; valueIndex <= valueCount; valueIndex++) {
            int nextValueIndex = valueIndex + (valueIndex & (-valueIndex));
            if (nextValueIndex <= valueCount) {
                tree[nextValueIndex] += tree[valueIndex];
            }
        }
    }

    Type query(int position) {
        Type answer = 0;
        for (int valueIndex = ++position; valueIndex > 0; valueIndex -= (valueIndex & (-valueIndex)))
            answer += tree[valueIndex];
        return answer;
    }

    void update(int position, Type value) {
        for (int valueIndex = ++position; valueIndex <= valueCount; valueIndex += (valueIndex & (-valueIndex)))
            tree[valueIndex] += value;
    }

    Type query(int left, int right) {
        return query(right) - query(left - 1);
    }

    int findKth(Type value) {
        int position = 0;
        for (int valueIndex = findNextPowerOfTwo(valueCount); valueIndex > 0; valueIndex >>= 1) {
            if (position + valueIndex <= valueCount && tree[position + valueIndex] < value) {
                value -= tree[position + valueIndex];
                position += valueIndex;
            }
        }
        return position + 1;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<Query> queries;
    int queryCount;
    cin >> queryCount;
    for (int queryIndex = 1; queryIndex <= queryCount; queryIndex++) {
        Query query;
        cin >> query.type;
        if (query.type != 3)
            cin >> query.position >> query.value;
        queries.push_back(query);
    }

    BinaryIndexedTree<long long> positions(queryCount, 1);
    reverse(queries.begin(), queries.end());
    for (auto &query: queries) {
        if (query.type != 3) {
            query.position = positions.findKth(query.position + 1) - 1;
            positions.update(query.position, -1);
        }
    }
    reverse(queries.begin(), queries.end());
    BinaryIndexedTree<long long> values(queryCount);
    int lastOperationOfTypeOne = 0, lastOperationOfTypeTwo = 0;
    long long answer = 0;
    for (const auto &query: queries) {
        if (query.type == 1) {
            values.update(query.position, query.value);
            lastOperationOfTypeOne = query.position;
        } else if (query.type == 2) {
            values.update(query.position, query.value);
            lastOperationOfTypeTwo = query.position;
        } else {
            int left = min(lastOperationOfTypeOne, lastOperationOfTypeTwo);
            int right = max(lastOperationOfTypeOne, lastOperationOfTypeTwo);
            answer += values.query(left, right);
        }
    }

    cout << answer % MOD << endl;
    return 0;
}