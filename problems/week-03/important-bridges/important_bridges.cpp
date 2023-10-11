#include <iostream>
#include <vector>
#include <iterator>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace boost;

typedef std::vector<int> vint;
typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int>>
    graph_t;

bool topological(std::pair<int, int> p1, std::pair<int, int> p2)
{
    return p1.first < p2.first || (p1.first == p2.first && p1.second < p2.second);
}

void testcase()
{
    int n, m;
    std::cin >> n >> m;

    graph_t G(n);

    int u, v;
    for (int i = 0; i < m; i++)
    {
        std::cin >> u >> v;
        add_edge(u, v, G);
    }

    property_map<graph_t, edge_weight_t>::type component = get(edge_weight, G);
    vint dfs(n);
    vint low(n);

    // need to use bgl_named_params becasue C++ doesn't provide handling for
    // default params of template functions, need separation by "." instead of ","
    // see https://www.boost.org/doc/libs/1_83_0/libs/graph/doc/bgl_named_params.html
    biconnected_components(G, component,
                           discover_time_map(make_iterator_property_map(dfs.begin(), get(vertex_index, G)))
                               .lowpoint_map(make_iterator_property_map(low.begin(), get(vertex_index, G))));

    std::vector<std::pair<int, int>> bridges;

    // found a bridge (u,v) if low[v] > dfs[u]
    // as graph is non-directed need to check both (u,v) and (v,u)
    graph_traits<graph_t>::edge_iterator ebeg, eend;
    for (tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg)
    {
        int src = source(*ebeg, G), trgt = target(*ebeg, G);
        if (low[trgt] > dfs[src] || low[src] > dfs[trgt])
            bridges.push_back(std::make_pair(std::min(src, trgt), std::max(src, trgt)));
    }

    // sort bridges in lexicographic order
    std::sort(bridges.begin(), bridges.end(), topological);

    int bridges_size = bridges.size();
    std::cout << bridges_size << std::endl;
    for (int i = 0; i < bridges_size; i++)
        std::cout << bridges[i].first << " " << bridges[i].second << std::endl;
}

int main()
{
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++)
        testcase();
    return 0;
}