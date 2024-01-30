#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Triple
{
    int first;
    int second;
    int third;

    Triple(int f, int s, int t) : first(f), second(s), third(t) {}
};

typedef vector<Triple> tint;
typedef vector<int> vint;
typedef vector<vint> mint;

bool comperator(Triple p1, Triple p2)
{
    // fist sort according to prefix, then according to num hops from 0
    return (p1.first < p2.first) || (p1.first == p2.first && p1.second < p2.second);
}

int binary_search(const tint &t, int key, int w)
{
    // find the triplet with prefix sum equal to key and that is furthest from 0
    // in a different waterway than w, if no such triplet exists return -1
    int left = 0, right = t.size() - 1;

    while (left <= right)
    {
        int middle = (left + right) / 2;

        if (t[middle].first == key)
        {

            while (middle < right && t[middle + 1].first == key)
            {
                middle++;
            }

            if (t[middle].third != w)
            {
                return t[middle].second;
            }
            else if (t[middle - 1].first == key)
            {
                return t[middle - 1].second;
            }
            else
            {
                return -1;
            }
        }
        else if (t[middle].first > key)
        {
            right = middle - 1;
        }
        else
        {
            left = middle + 1;
        }
    }

    return -1;
}

int sliding_window(const vint &v, int k)
{
    // return max window size whose value is equal to k
    int sum = 0, left = 0, right = 0, maxi = 0;
    int size = v.size();
    while (left <= right && right < size)
    {
        if (sum == k)
        {
            maxi = max(maxi, right - left);
            sum += v[right++];
            sum -= v[left++];
        }
        else if (sum < k)
        {
            sum += v[right++];
        }
        else
        {
            sum -= v[left++];
        }
    }

    // might still need to increase left if window value to big
    while (sum > k && left < size)
    {
        sum -= v[left++];
    }

    if (sum == k)
        maxi = max(maxi, right - left);

    return maxi;
}

void testcase()
{
    int n; cin >> n;
    int k; cin >> k;
    int w; cin >> w;

    vint c(n);
    for (int i = 0; i < n; i++)
        cin >> c[i];

    mint water_simple(w);
    mint water_prefix(w);
    tint t;

    for (int i = 0; i < w; i++)
    {
        int l; cin >> l;
        int sum = 0;
        water_prefix[i] = vint(l);
        water_simple[i] = vint(l);

        for (int j = 0; j < l; j++)
        {
            int isl; cin >> isl;
            sum += c[isl];
            water_prefix[i][j] = sum;
            water_simple[i][j] = c[isl];
            t.push_back(Triple(sum, j, i));
        }
    }

    // sliding window for highest num of islands that can potentially be conquered in single waterway
    int maxi = 0;
    for (int i = 0; i < w; i++)
    {
        maxi = max(maxi, sliding_window(water_simple[i], k));
    }

    // check for paths across 2 waterways, iterate over all nodes and
    // check for missing remainder in different waterways furthest from 0
    sort(t.begin(), t.end(), comperator);
    for (int i = 0; i < w; i++)
    {
        int waterway_size = water_prefix[i].size();
        for (int j = 0; j < waterway_size; j++)
        {
            if (water_prefix[i][j] < k)
            {
                int len = binary_search(t, (k - water_prefix[i][j] + c[0]), i);
                if (len != -1)
                {
                    maxi = max(maxi, j + len + 1);
                }
            }
        }
    }

    cout << maxi << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--)
        testcase();
    return 0;
}