#include <iostream>
#include <unordered_set>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef std::vector<int> vint;
typedef adjacency_list<vecS,
                       vecS,
                       directedS,
                       no_property,
                       property<edge_weight_t, int>>
    graph_t;
typedef property_map<graph_t, edge_weight_t>::type weight_m;
typedef graph_traits<graph_t>::edge_descriptor edge_desc;

void add_teleport_edges(graph_t &G, const vint &teleport_nodes, const vint &components, int num_components)
{
    /**
     * rather than adding a direct edge between two teleport nodes in same
     * component, we create a new vertex for the corresponding component and add
     * an edge from the telport node to the component with a weight that is equal
     * to the size of the component-1 and an edge from the new node to the teleport
     * node with a weight equal to 0.
     *
     * That way two nodes part of the same component can reach each other over the
     * newly added component node and the "distance" it takes to get there is equal
     * to the size of the component.
     *
     * This approach only requires to iterate over the number of teleport nodes
     * twice, while adding an edge between the two nodes directly would require
     * looping over all possible pairs of vertices and check if they are part of
     * the same component which woul leave us with O(n^2) instead of O(T)
     */
    weight_m weights = get(edge_weight, G);

    // count the size of each component required for the weight of the edge
    vint component_size(num_components, 0);
    for (auto i : teleport_nodes)
        component_size[components[i]]++;

    // add edges to make component node available from each of its teleport nodes
    // add edges to freely make teleport nodes available from its component nodes
    int n = num_vertices(G);
    for (auto i : teleport_nodes)
    {
        int size = component_size[components[i]] - 1;
        edge_desc edge_id = add_edge(i, n + components[i], G).first;
        weights[edge_id] = size;
        edge_id = add_edge(n + components[i], i, G).first;
        weights[edge_id] = 0;
    }
}

int get_best_warehouse(const graph_t &G, int src, int k)
{
    /**
     * runs Dijkstra starting from the customer node. Because we added the edges
     * reverse order at the beginning we will know the distance to each of the k
     * warehouses and we can simply return the minimum of it
     */
    vint dist_map(num_vertices(G));
    dijkstra_shortest_paths(G, src,
                            distance_map(make_iterator_property_map(dist_map.begin(), get(vertex_index, G))));

    int len = dist_map[0];
    for (int i = 1; i < k; i++)
    {
        len = std::min(len, dist_map[i]);
    }

    return len;
}

void testcase()
{
    int n; std::cin >> n;
    int m; std::cin >> m;
    int k; std::cin >> k;
    int t; std::cin >> t;

    vint teleport_nodes(t);
    for (int i = 0; i < t; i++)
        std::cin >> teleport_nodes[i];

    graph_t G(n);
    weight_m weights = get(edge_weight, G);

    for (int i = 0; i < m; i++)
    {
        int u; std::cin >> u;
        int v; std::cin >> v;
        int c; std::cin >> c;
        edge_desc edge_id = add_edge(v, u, G).first; // reverse edge to simplify dijkstra!
        weights[edge_id] = c;
    }

    // run strong components to detect which teleport nodes are linked to each other
    vint components(num_vertices(G));
    int num_components = strong_components(G,
                                           make_iterator_property_map(components.begin(), get(vertex_index, G)));

    add_teleport_edges(G, teleport_nodes, components, num_components);
    int len = get_best_warehouse(G, n - 1, k);

    if (len <= 1e6)
        std::cout << len << std::endl;
    else
        std::cout << "no" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}