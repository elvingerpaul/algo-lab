#include <iostream>
#include <vector>
#include <limits>

typedef std::vector<int> vint;
typedef std::vector<vint> mint;

void testcase()
{
    int n; std::cin >> n;

    vint a(n);
    for (int i = 0; i < n; i++)
        std::cin >> a[i];

    vint b(n);
    for (int i = 0; i < n; i++)
        std::cin >> b[i];

    mint dp(n + 1, vint(n + 1, std::numeric_limits<int>::max()));
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            dp[i][j] = std::min(dp[i - 1][j - 1], std::min(dp[i - 1][j], dp[i][j - 1]));
            dp[i][j] += (a[i - 1] - 1) * (b[j - 1] - 1);
        }
    }

    std::cout << dp[n][n] << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}