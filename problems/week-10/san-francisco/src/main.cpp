#include <iostream>
#include <vector>

typedef std::vector<long> vlong;
typedef std::vector<vlong> mlong;
typedef std::vector<std::pair<int, int>> vedge;
typedef std::vector<vedge> adj;
typedef std::pair<int, int> pint;

void testcase()
{
    int n, m; std::cin >> n >> m;
    long x; std::cin >> x;
    int k; std::cin >> k;

    adj g(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, p;
        std::cin >> u >> v >> p;
        g[u].push_back(std::make_pair(v, p));
    }

    for (int i = 1; i < n; i++)
    { // relay all weyaya holes to successors of 0
        if (g[i].size() == 0)
            for (auto succ : g[0])
                g[i].push_back(succ);
    }

    // DP - d[k][n] = max points to score with k moves starting from node n
    // will have to check d[:][0] to see if there is a k such that we can score
    // at least x
    mlong d(k + 1, vlong(n, 0)); // initialization

    for (int i = 1; i <= k; i++)
    {

        for (int j = 0; j < n; j++)
        {
            long best = 0;
            for (auto succ : g[j])
                best = std::max(best, d[i - 1][succ.first] + succ.second);

            d[i][j] = best;
        }
    }

    for (int i = 0; i <= k; i++)
        if (d[i][0] >= x)
        {
            std::cout << i << "\n";
            return;
        }

    std::cout << "Impossible\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}