#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

typedef std::vector<int> vint;
typedef std::vector<vint> adj;

std::tuple<int, int, int> solve(const adj &g, vint &costs, int node)
{
    int sum_not_covered = 0;
    int sum_covered = 0;
    int overhead_selected_child = std::numeric_limits<int>::max();
    for (int child : g[node])
    {
        auto [selected, covered, not_covered] = solve(g, costs, child);
        sum_not_covered += not_covered;
        sum_covered += covered;

        /*get the overhead if were to select this child after all, what is the price
        we would have to pay for selecting it*/
        overhead_selected_child = std::min(overhead_selected_child, selected - covered);
    }

    /* If we select this node, all children can be non-covered as they will be
    covered by this node. sum_not_covered is assumed to be the cheapest obviously*/
    int self_selected = costs[node] + sum_not_covered;

    /* We want this node to be covered, which can happen in two ways
    - either we simply select this node -> cost is self_selected
    - or we need to have at least one children cover it, meaning at least one
      children has to be selected. We thus need the overhead of selecting this
      children after all. overhead_selected_child is the difference between this
      children being selected rather than simply covered. Note that this diff
      could be 0
    Note that here we DO NOT rely on the parent to cover it, that will be the
    case below*/
    int self_covered = std::min(self_selected, sum_covered + overhead_selected_child);

    /* We want to get the cost of this node not being covered for now and be
    covered by the parent later on. For this we are simply interested in the cost
    of having all children covered (potentially be themselves but potentially
    also by their children)*/
    int self_not_covered = std::min(self_selected, sum_covered);

    return {self_selected, self_covered, self_not_covered};
}

void testcase()
{
    int n; std::cin >> n;

    adj g(n);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v; std::cin >> u >> v;
        g[u].push_back(v);
    }

    vint costs(n);
    for (int i = 0; i < n; i++)
        std::cin >> costs[i];

    // solution corresponds to the covered case of the root
    std::cout << std::get<1>(solve(g, costs, 0)) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}