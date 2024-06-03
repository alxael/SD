#include <queue>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream cin("timbre.in");
    ofstream cout("timbre.out");

    int stampCount, stampTypeCount, stampsPerPack;
    vector <pair<int, int>> stampTypes;
    priority_queue<int, vector<int>, greater<int>> costs;

    cin >> stampCount >> stampTypeCount >> stampsPerPack;

    for(int stampTypeIndex = 0; stampTypeIndex < stampTypeCount; stampTypeIndex++) {
        int stamp, cost;
        cin >> stamp >> cost;
        stampTypes.push_back(pair<int, int>(stamp, cost));
        if(stamp >= stampCount) {
            costs.push(cost);
        }
    }
    int currentPack = 0, totalCost = 0;
    while(stampCount - currentPack * stampsPerPack > 0) {
        totalCost += costs.top();
        costs.pop();
        currentPack++;

        for(int stampTypeIndex = 0; stampTypeIndex < stampTypeCount; stampTypeIndex++) {
            if(stampTypes[stampTypeIndex].first >= stampCount - currentPack * stampsPerPack &&
               stampTypes[stampTypeIndex].first < stampCount - (currentPack - 1) * stampsPerPack) {
                costs.push(stampTypes[stampTypeIndex].second);
            }
        }
    }
    cout << totalCost;
    return 0;
}