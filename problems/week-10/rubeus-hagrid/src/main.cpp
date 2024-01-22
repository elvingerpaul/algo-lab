#include <iostream>
#include <vector>
#include <algorithm>

struct Edge
{
    int v;
    long w;
    long sum_sub;
    int count_sub;

    Edge(int v, long w, long sum_sub, int count_sub) : v(v), w(w), sum_sub(sum_sub), count_sub(count_sub) {}
};

typedef std::pair<long, int> sub_prop;
typedef std::vector<sub_prop> vsub;
typedef std::vector<long> vlong;
typedef std::vector<Edge> vedge;
typedef std::vector<vedge> adj;

// calculate traversal time of subtrees and the number of nodes in each subtree
sub_prop cost_subtree(const adj &g, vsub &c_sub, int node, long cost)
{
    c_sub[node].first += 2 * cost; // we want the cost of traversing subtree and getting back to root
    c_sub[node].second += 1; // number of nodes in this subtree
    for (Edge e : g[node])
    {
        auto v_prop = cost_subtree(g, c_sub, e.v, e.w);
        c_sub[node].first += v_prop.first;
        c_sub[node].second += v_prop.second;
    }
    return c_sub[node];
}

long num_galleons(const adj &g, const vlong &galleons, const int node, long timer)
{
    long sum = galleons[node] - timer;
    for (Edge e : g[node])
    {
        sum += num_galleons(g, galleons, e.v, timer + e.w);
        timer += e.sum_sub; // we have processed entire subtree
    }
    return sum;
}

void testcase()
{
    int n; std::cin >> n;

    vlong galleons(n + 1, 0);
    for (int i = 1; i <= n; i++)
        std::cin >> galleons[i];

    adj g(n + 1);
    for (int i = 0; i < n; i++)
    {
        int u, v; std::cin >> u >> v;
        long l; std::cin >> l;
        g[u].push_back(Edge(v, l, 0, 0));
    }

    // calculate time to traverse entire subtree in DFS manner
    vsub c_sub(n + 1, std::make_pair(0, 0));
    cost_subtree(g, c_sub, 0, 0);

    for (int i = 0; i <= n; i++)
        for (Edge &e : g[i])
        {
            e.sum_sub = c_sub[e.v].first;
            e.count_sub = c_sub[e.v].second;
        }

    // sort each successor list by taking traversal time as well as number of nodes of subtrees into account
    for (vedge &ve : g)
        std::sort(ve.begin(), ve.end(), [](const Edge &e1, const Edge &e2)
                  { return e1.sum_sub * e2.count_sub < e2.sum_sub * e1.count_sub; });

    // calculate num of galleons
    std::cout << num_galleons(g, galleons, 0, 0) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}