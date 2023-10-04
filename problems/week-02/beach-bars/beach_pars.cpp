#include <iostream>
#include <vector>
#include <limits>
#include <bits/stdc++.h>
#include <cmath>

using namespace std;

typedef vector<int> vint;

void testcase()
{
    int n;
    cin >> n;
    vint v(n);

    // it could be that there are always 2 middle points that are optimal
    // therefor we have to consider that each pair has pair [left, right] has 2 potential best locations
    vint best_locations(2 * n);

    for (int i = 0; i < n; i++)
        cin >> v[i];

    // make sure that all parasols appear in decreasing order
    sort(v.begin(), v.end());

    int left = 0, right = 0, max = 0, min = numeric_limits<int>::max(), next = 0;
    while (left <= right && right < n)
    {

        // we are more than 100m away from the parasols
        if (v[right] - v[left] > 200)
        {
            left++;
            continue;
        }

        // need to explicitly floor integer division, to ensure we round to smallest
        // integer even if numbers are negative, instead of rounding towards 0
        int middle = floor(double(v[right] + v[left]) / 2);
        bool is_inbetween = (v[right] + v[left]) % 2 != 0;
        int dist = v[right] - middle;

        if (right - left + 1 > max)
        {
            max = right - left + 1;
            min = dist;

            next = 0;
            best_locations[next++] = middle;

            // if we are between two positions we need to add both to best_locations
            if (is_inbetween)
                best_locations[next++] = middle + 1;
        }
        else if (right - left + 1 == max)
        {
            // minimum can't be updated
            if (dist > min)
            {
                right++;
                continue;
            }

            // found new min, need to make sure we delete all previous best_locations
            if (dist < min)
                next = 0;

            min = dist;
            best_locations[next++] = middle;

            // if we are between two positions we need to add both to best_locations
            if (is_inbetween)
                best_locations[next++] = middle + 1;
        }

        right++;
    }

    cout << max << " " << min << endl;

    int ptr = 0;
    while (next > 0)
    {
        cout << best_locations[ptr++] << " ";
        next--;
    }

    cout << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
        testcase();
}