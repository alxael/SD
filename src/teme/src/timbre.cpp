#include <queue>
#include <iostream>

using namespace std;

int main()
{
    auto compare = [](pair<int, int> a, pair<int, int> b)
    {
        if (a.second == b.second)
            return a.first < b.first;
        return a.second > b.second;
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, function<bool(pair<int, int>, pair<int, int>)>> q(compare);
    int n, m, k, x, y, ans = 0;

    cin >> n >> m >> k;
    for (int index = 0; index < m; index++)
    {
        cin >> x >> y;
        q.push(pair<int, int>(x, y));
    }

    int pos = 1;
    while (!q.empty() && pos <= n)
    {
        auto top = q.top();
        q.pop();
        
        cout << top.first << " " << top.second << endl;
    }
    return 0;
}