#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int>>
    graph_t;
typedef property_map<graph_t, edge_weight_t>::type weight_m;
typedef graph_traits<graph_t>::edge_descriptor edge_desc;
typedef std::vector<edge_desc>::iterator edge_itr;
typedef std::vector<graph_t> vgraph;
typedef std::vector<int> vint;

void add_mst_to_shared_graph(graph_t &species_g, graph_t &shared_g)
{
    // calculate MST of species graph and adds it to shared graph
    std::vector<edge_desc> spanning_tree;
    kruskal_minimum_spanning_tree(species_g, std::back_inserter(spanning_tree));

    property_map<graph_t, edge_weight_t>::type species_weights = get(edge_weight, species_g);
    property_map<graph_t, edge_weight_t>::type shared_weights = get(edge_weight, shared_g);

    // runn over all edges from MST and add them to shared graph
    for (edge_itr it = spanning_tree.begin(); it != spanning_tree.end(); ++it)
    {
        int src = source(*it, species_g);
        int dest = target(*it, species_g);
        edge_desc edge_id = add_edge(src, dest, shared_g).first;
        shared_weights[edge_id] = species_weights[*it];
    }
}

void testcase()
{
    int n; std::cin >> n;
    int e; std::cin >> e;
    int s; std::cin >> s;
    int a; std::cin >> a;
    int b; std::cin >> b;

    // define a graph for each species with n nodes
    vgraph gs(s);
    for (int i = 0; i < s; i++)
        gs[i] = graph_t(n);

    // read in edges and edge weight for each species
    weight_m weights;
    edge_desc edge_id;
    for (int i = 0; i < e; i++)
    {
        int t1; std::cin >> t1;
        int t2; std::cin >> t2;

        for (int j = 0; j < s; j++)
        {
            weights = get(edge_weight, gs[j]);
            edge_id = add_edge(t1, t2, gs[j]).first;
            std::cin >> weights[edge_id];
        }
    }

    // read in hives, not needed though
    int hive;
    for (int i = 0; i < s; i++)
        std::cin >> hive;

    // define shared graph, that will contain edges of each species' MST (multigraph)
    graph_t shared_g(n);
    for (int i = 0; i < s; i++)
        add_mst_to_shared_graph(gs[i], shared_g);

    // find shortest path from a to b using species' MST
    vint dist_map(n);
    dijkstra_shortest_paths(shared_g, a,
                            distance_map(make_iterator_property_map(dist_map.begin(), get(vertex_index, shared_g))));
    std::cout << dist_map[b] << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}