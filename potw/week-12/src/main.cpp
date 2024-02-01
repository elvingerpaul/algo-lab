#include <iostream>
#include <vector>

struct Edge
{
    int succ;
    int rats;
};

typedef std::vector<Edge> vEdge;
typedef std::vector<vEdge> adj;
typedef std::vector<int> vint;
typedef std::vector<vint> mint;

mint dp;
adj forward;
adj backward;

int solve(int up, int down, int n)
{
    if (dp[up][down] != -2)
        return dp[up][down];

    int best = -1;
    if (up < down)
    { // advance up towards n-1
        for (Edge e : forward[up])
        {
            if (e.succ == down && down != n - 1) // not allowed to use same place twice except n-1
                continue;

            int num_rats = solve(e.succ, down, n);
            if (num_rats >= 0) // if result < 0, no valid path up to n-1
                best = std::max(best, num_rats + e.rats);
        }
    }
    else
    { // advance down towards n-1
        for (Edge e : backward[down])
        {
            if (e.succ == up && up != n - 1) // not allowed to use same place twice except n-1
                continue;

            int num_rats = solve(up, e.succ, n);
            if (num_rats >= 0) // if result < 0, no valid paht up to n-1
                best = std::max(best, num_rats + e.rats);
        }
    }

    dp[up][down] = best;
    return best;
}

void testcase()
{
    int n, m;
    std::cin >> n >> m;

    forward = adj(n);
    backward = adj(n);
    for (int i = 0; i < m; i++)
    {
        int u, v, f; std::cin >> u >> v >> f;
        if (u < v)
            forward[u].push_back({v, f});
        else
            backward[v].push_back({u, f});
    }
    // dp[up][down] = max number of rats that can be caught when starting in up,
    // going all the way up to n-1 and ending in down
    // dp[up][down] = -2 -> value not calculated yet
    // dp[up][down] = -1 -> no valid path leading up to n-1
    dp = mint(n, vint(n, -2));
    dp[n - 1][n - 1] = 0;
    std::cout << solve(0, 0, n) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}