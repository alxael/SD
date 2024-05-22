#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> arr;
    int n, x;

    cin >> n;
    for (int index = 0; index < n; index++)
    {
        cin >> x;
        arr.push_back(x);
    }

    for (int config = 0; config < (1 << n); config++)
    {
        vector<int> values;
        for (int bit = 0; bit < n; bit++)
            if (config & (1 << bit))
                values.push_back(arr[bit]);
        sort(values.rbegin(), values.rend());

        bool sign = true;
        int sum = 0;
        string output;
        for (const auto &value : values) {
            sum += value * (sign ? 1 : -1);
            output += sign ? '+' : '-';
            output += to_string(value);
            sign = !sign;
        }

        cout << output << " = " << sum << endl;
    }
    return 0;
}