#include <queue>
#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream cin("bleach.in");
    ofstream cout("bleach.out");

    int enemyCount, maxPositionShift, enemyPower;
    priority_queue<int, vector<int>, greater<int>> enemyPowers;
    cin >> enemyCount >> maxPositionShift;

    for (int enemyIndex = 0; enemyIndex < enemyCount; enemyIndex++) {
        cin >> enemyPower;
        enemyPowers.push(enemyPower);
    }

    long long necessaryPower = 0, currentPower = 0;
    while(!enemyPowers.empty()) {
        enemyPower = enemyPowers.top();
        enemyPowers.pop();

        if(currentPower < enemyPower) {
            necessaryPower += enemyPower - currentPower;
            currentPower = enemyPower * 2;
        } else
            currentPower += enemyPower;
    }
    cout << necessaryPower;
    return 0;
}