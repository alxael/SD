#include <set>
#include <tuple>
#include <iostream>

using namespace std;

int main()
{
    int n, a, b;
    multiset<tuple<int, int, bool>> ab;

    cin >> n;
    for (int index = 0; index < n; index++)
    {
        cin >> a >> b;
        ab.insert(tuple<int, int, bool>{a, b, false});
    }

    bool changed = true;
    while (changed)
    {
        changed = false;

        auto mn = *ab.begin();
        auto mx = *ab.rbegin();

        if (get<0>(mn) < get<1>(mn) && !get<2>(mn))
        {
            tuple<int, int, bool> aux(get<1>(mn), get<0>(mn), true);
            ab.erase(mn);
            ab.insert(aux);
            changed = true;
        }

        if (get<0>(mx) > get<1>(mx) && !get<2>(mx))
        {
            tuple<int, int, bool> aux(get<1>(mx), get<0>(mx), true);
            ab.erase(mx);
            ab.insert(aux);
            changed = true;
        }
    }

    cout << get<0>(*ab.rbegin()) - get<0>(*ab.begin());
    return 0;
}