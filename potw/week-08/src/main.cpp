#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity)
    {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void testcase()
{
    int l, p; std::cin >> l >> p;
    graph G(l + 2);
    edge_adder adder(G);

    // store adjcacency matrix of graph
    std::vector<std::vector<int>> adj(l + 2, std::vector<int>(l + 2, 0));

    // Add special vertices source and sink
    const int v_source = l;
    const int v_sink = l + 1;

    long total_soldiers_required = 0;

    for (int i = 0; i < l; i++)
    {
        int g, d; std::cin >> g >> d;
        total_soldiers_required += d;
        adj[v_source][i] = g;
        adj[i][v_sink] = d;
    }

    for (int i = 0; i < p; i++)
    {
        int f, t, c_min, c_max; std::cin >> f >> t >> c_min >> c_max;
        adj[v_source][t] += c_min;    // dest location has an additional c_min soldiers arriving for sure over f -> t
        adj[v_source][f] -= c_min;    // src location has c_min soldiers less left to send elsewhere than location f
        adj[f][t] += (c_max - c_min); // max soldiers that can take this path in addition to c_min
    }

    for (int i = 0; i < l + 2; i++)
    {
        for (int j = 0; j < l + 2; j++)
        {
            if (adj[i][j] > 0)
                adder.add_edge(i, j, adj[i][j]);
            // else if (adj[i][j] < 0)
            //  std::cout << "negative edge weight for " << i << ", " << j << std::endl;
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << (flow == total_soldiers_required ? "yes\n" : "no\n");
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--)
        testcase();
    return 0;
}