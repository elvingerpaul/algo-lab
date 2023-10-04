#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vint;
typedef vector<vint> mint;

void testcase()
{
    int n, m, k;
    cin >> n >> m >> k;
    vint v(n);

    // seq_sum[i] = j if sum(seq_sum[i:j]) = k else -1, j excluded
    vint seq_sum(n, -1);

    // read in evil in each horcrux
    for (int i = 0; i < n; i++)
    {
        cin >> v[i];
    }

    // sliding window to find sequences of sum equal to k
    int left = 0, right = 0, sum = 0, seq_len = 0, prev_seq_len = 0;
    while (left <= right && right < n)
    {

        if (sum == k)
        {
            seq_sum[right - 1] = left;
            sum -= v[left++];
            sum += v[right++];
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

    // sum might be to large and we can still increase left pointer
    while (left < n && sum > k)
        sum -= v[left++];

    // check if there is a sequence ending in the very last horcrux
    if (sum == k)
        seq_sum[n - 1] = left;

    // DP solution, dp[i][j] = maximum number of horcruxes destroyed up to index i with j members
    mint dp(m, vint(n + 1, 0));

    // DP initialization for one member
    for (int i = 1; i <= n; i++)
    {
        seq_len = (seq_sum[i - 1] != -1) ? (i - seq_sum[i - 1]) : 0;
        dp[0][i] = max(dp[0][i - 1], seq_len);
    }

    for (int i = 1; i < m; i++)
    {
        for (int j = 1; j <= n; j++)
        {

            if (seq_sum[j - 1] != -1 && dp[i - 1][seq_sum[j - 1]] != 0)
            {
                // we can add another disjoint interval by using sequence that ends in j-1
                seq_len = j - seq_sum[j - 1];
                prev_seq_len = dp[i - 1][seq_sum[j - 1]];
            }
            else
            {
                // we cannot add a new disjoint interval, because no sequence ends in j-1
                seq_len = 0;
                prev_seq_len = 0;
            }

            // keep current best, or potential new best by adding new disjoint interval
            dp[i][j] = max(dp[i][j - 1], seq_len + prev_seq_len);
        }
    }

    if (dp[m - 1][n] != 0)
        cout << dp[m - 1][n] << endl;
    else
        cout << "fail" << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
        testcase();
}