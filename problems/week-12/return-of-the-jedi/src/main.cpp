#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

typedef std::vector<int> vint;
typedef std::vector<vint> mint;
typedef std::vector<std::pair<int, int>> vtint;
typedef std::vector<vtint> adj;

void dfs(const adj &g, std::vector<bool> &visited, mint &longest, int src, int curr_node, int max)
{
    // calculates the longest edge on the path from src to all other nodes
    visited[curr_node] = true;
    longest[src][curr_node] = max;
    for (auto succ : g[curr_node])
    {
        if (visited[succ.first])
            continue;

        dfs(g, visited, longest, src, succ.first, std::max(max, succ.second));
    }
}

void testcase()
{
    int n, start; std::cin >> n >> start;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 1; j < n - i; j++)
        {
            int k; std::cin >> k;
            edge_desc e = boost::add_edge(i, i + j, G).first;
            weights[e] = k;
        }
    }

    std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    // transform mst edges into directed adjacency list
    long mst_cost = 0;
    adj mst_g(n);
    mint edge_in_mst(n, vint(n, 0));
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
    {
        int src = boost::source(*it, G);
        int dest = boost::target(*it, G);
        mst_g[src].push_back(std::make_pair(dest, weights[*it]));
        mst_g[dest].push_back(std::make_pair(src, weights[*it]));
        mst_cost += weights[*it];
        edge_in_mst[src][dest] = 1;
        edge_in_mst[dest][src] = 1;
    }

    mint longest(n, vint(n, -1));
    for (int i = 0; i < n; i++)
    {
        std::vector<bool> visited(n, false);
        // check for longest edge from node i to all other nodes in mst
        dfs(mst_g, visited, longest, i, i, std::numeric_limits<int>::min());
    }

    // iterate over all edges not in MST and look for cheapest edge to be replaced in MST
    int cheapest = std::numeric_limits<int>::max();
    edge_it e_beg, e_end;
    for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg)
    {
        int src = boost::source(*e_beg, G);
        int dest = boost::target(*e_beg, G);

        if (edge_in_mst[src][dest] == 1)
            continue;

        cheapest = std::min(cheapest, weights[*e_beg] - longest[src][dest]);
    }

    std::cout << mst_cost + cheapest << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}