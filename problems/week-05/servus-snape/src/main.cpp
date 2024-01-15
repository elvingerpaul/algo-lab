#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef std::pair<int, int> pint;
typedef std::vector<pint> vpint;
typedef std::vector<int> vint;

typedef std::vector<long> vlong;
typedef std::vector<vlong> mlong;
typedef std::vector<mlong> tlong;

void testcase()
{
    int n, m; std::cin >> n >> m;
    // test cases 3 and 4 fail if a and b are not of type long -> why?
    long a, b; std::cin >> a >> b;
    long power, happ, wit; std::cin >> power >> happ >> wit;

    // read in type A
    vpint type_a(n);
    for (int i = 0; i < n; i++)
    {
        int p, h;
        std::cin >> p >> h;
        type_a[i] = std::make_pair(p, h);
    }

    // read in type B
    vint type_b(m);
    for (int i = 0; i < m; i++)
    {
        std::cin >> type_b[i];
    }

    std::sort(type_b.begin(), type_b.end(), std::greater<int>());

    // dp table to find min number of potions of type A
    // dp[i][j][h] = max power that can be achieved using j out of i first potions
    // achieving at least power h
    tlong dp(n + 1, mlong(n + 1, vlong(happ + 1, std::numeric_limits<long>::min())));

    // init: save highest power for single potion that achieves happiness h
    for (int h = 0; h <= happ; h++)
    {
        for (int i = 1; i <= n; i++)
        {
            dp[i][1][h] = dp[i - 1][1][h];
            if (type_a[i - 1].second >= h && dp[i][1][h] < type_a[i - 1].first)
                dp[i][1][h] = type_a[i - 1].first;
        }
    }

    for (int j = 2; j <= n; j++)
    {
        for (int h = 0; h <= happ; h++)
        {
            for (int i = j; i <= n; i++)
            {
                int prev_h = std::max(0, h - type_a[i - 1].second);
                dp[i][j][h] = std::max(dp[i - 1][j][h], dp[i - 1][j - 1][prev_h] + type_a[i - 1].first);
            }
        }
    }

    long sum_wit = 0;
    for (int i = 0; i < m; i++)
    {
        sum_wit += type_b[i];
        long mag_penalty = (i + 1) * b;

        if (sum_wit >= wit)
        {
            // search for idx that ensures sufficient power including magic penalty
            for (int j = 1; j <= n; j++)
            {
                if (sum_wit - j * a < wit)
                    break;

                // line below as follows would fail test 3 & 4 -> why?
                // if (dp[n][j][happ] - mag_penalty >= power){
                if (dp[n][j][happ] >= power + mag_penalty)
                {
                    std::cout << i + j + 1 << std::endl;
                    return;
                }
            }
        }
    }

    std::cout << "-1" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}