#include <iostream>
#include <vector>

typedef std::pair<int, int> tint;
typedef std::vector<tint> vtint;
typedef std::vector<vtint> adj;
typedef std::vector<int> vint;
typedef std::vector<vint> mint;

mint dp;
adj forward;
adj backward;

int solve(int fwd, int bwd, int n)
{
    if (fwd == n - 1 && bwd == n - 1)
        return 0;

    if (fwd == bwd && fwd != 0) // exclude the start case
        return -1;

    if (dp[fwd][bwd] != -2)
        return dp[fwd][bwd];

    int best = -1;
    if (fwd < bwd)
    {
        // advance foward pointer towards n-1 if possible
        for (auto f : forward[fwd])
        {
            int child_sol = solve(f.first, bwd, n);
            if (child_sol >= 0) // solution < 0 means there is no path from child to n-1
                best = std::max(best, child_sol + f.second);
        }
    }
    else
    {
        // advance backward pointer towards n-1 if possible
        for (auto b : backward[bwd])
        {
            int child_sol = solve(fwd, b.first, n);
            if (child_sol >= 0) // solution < 0 means there is no path from child to n-1
                best = std::max(best, child_sol + b.second);
        }
    }

    dp[fwd][bwd] = best;

    return best;
}

void testcase()
{
    int n, m; std::cin >> n >> m;

    forward = adj(n);
    backward = adj(n);

    for (int i = 0; i < m; i++)
    {
        int u, v, f; std::cin >> u >> v >> f;
        if (u < v)
            forward[u].push_back(std::make_pair(v, f));
        else
            backward[v].push_back(std::make_pair(u, f));
    }

    dp = mint(n, vint(n, -2));
    std::cout << solve(0, 0, n) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}