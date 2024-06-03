#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("proc2.in");
    ofstream cout("proc2.out");

    int processorCount, taskCount, startTime, duration;
    priority_queue<int, vector<int>, greater<>> processors;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> tasks;

    cin >> processorCount >> taskCount;
    for(int processorIndex = 1; processorIndex <= processorCount; ++processorIndex)
        processors.push(processorIndex);

    for (int taskIndex = 1; taskIndex <= taskCount; ++taskIndex){
        cin >> startTime >> duration;
        while(!tasks.empty() && tasks.top().first <= startTime){
            processors.push(tasks.top().second);
            tasks.pop();
        }
        cout << processors.top() << "\n";
        tasks.push(pair<int, int>(startTime + duration, processors.top()));
        processors.pop();
    }

    return 0;
}