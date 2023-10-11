#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int>>
    weighted_g;
typedef property_map<weighted_g, edge_weight_t>::type weight_m;
typedef graph_traits<weighted_g>::edge_descriptor edge_desc;

int spanning_tree(const weighted_g &G, const weight_m &weights)
{

    std::vector<edge_desc> spanning_tree;
    kruskal_minimum_spanning_tree(G, std::back_inserter(spanning_tree));

    int total_weight = 0;
    for (std::vector<edge_desc>::iterator it = spanning_tree.begin(); it != spanning_tree.end(); ++it)
    {
        total_weight += weights[*it];
    }

    return total_weight;
}

int longest_path(const weighted_g &G)
{
    int num_vert = num_vertices(G);
    std::vector<int> distances(num_vert);

    dijkstra_shortest_paths(G, 0,
                            distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, G))));

    int maxi = 0;
    for (int i = 0; i < num_vert; i++)
    {
        maxi = std::max(maxi, distances[i]);
    }

    return maxi;
}

void testcase()
{
    int n, m;
    std::cin >> n >> m;

    weighted_g G(n);
    weight_m weights = get(edge_weight, G);
    edge_desc edge_id;

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        edge_id = add_edge(u, v, G).first;
        weights[edge_id] = w;
    }

    int total_weight = spanning_tree(G, weights);
    int max_length = longest_path(G);
    std::cout << total_weight << " " << max_length << std::endl;
}

int main()
{
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++)
        testcase();
}