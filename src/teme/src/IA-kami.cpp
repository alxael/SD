#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream cin("kami.in");
    ofstream cout("kami.out");

    int levelCount, queryCount, level, value;
    vector <int> values;

    cin >> levelCount;
    values.resize(levelCount + 1);
    for(int levelIndex = 1; levelIndex <= levelCount; levelIndex ++)
        cin >> values[levelIndex];

    cin >> queryCount;
    for(int queryIndex = 0; queryIndex < queryCount; queryIndex ++) {
        int operationType;
        cin >> operationType >> level;
        if(operationType == 0) {
            cin >> value;
            values[level] = value;
        } else {
            int current = values[level];
            while(level > 1 && values[level - 1] < current) {
                level --;
                current += values[level];
                if(current < 0) {
                    level = 1;
                    break;
                }
            }
            cout << level - 1 << endl;
        }
    }
    return 0;
}