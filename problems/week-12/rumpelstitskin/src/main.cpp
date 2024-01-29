#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                                                                              boost::property<boost::edge_weight_t, long>>>>>
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator

typedef std::vector<int> vint;

// Custom edge adder class
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    edge_desc add_edge(int from, int to, long capacity, long cost)
    {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;      // new assign cost
        w_map[rev_e] = -cost; // new negative cost
        return e;
    }
};

void testcase()
{
    int n, p_g, p_h, e_g, e_h, f_g, f_h, s_g, s_h;
    std::cin >> n >> p_g >> p_h >> e_g >> e_h >> f_g >> f_h >> s_g >> s_h;

    vint a(n);
    int max_pleasure = std::numeric_limits<int>::min();
    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
        max_pleasure = std::max(max_pleasure, a[i]);
    }

    graph G(p_g + p_h + 2 * n); // p_g nodes for G, p_h for H and 2*n for islands (in and out)
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    for (int i = 0; i < e_g; i++)
    {
        // read in edges between cities in G
        int u, v, c; std::cin >> u >> v >> c;
        adder.add_edge(u, v, c, 0);
    }

    for (int i = 0; i < e_h; i++)
    {
        // read in edges between cities in H -> reverse them in network
        int u, v, c; std::cin >> u >> v >> c;
        adder.add_edge(p_g + v, p_g + u, c, 0);
    }

    for (int i = 0; i < f_g; i++)
    {
        // read in edges from G to islands
        int u, v, c; std::cin >> u >> v >> c;
        adder.add_edge(u, p_g + p_h + v, c, 0);
    }

    for (int i = 0; i < f_h; i++)
    {
        // read in edges from H to islands -> reverse them in network
        int u, v, c; std::cin >> u >> v >> c;
        adder.add_edge(p_g + p_h + v + n, p_g + u, c, 0);
    }

    for (int i = 0; i < n; i++)
        // island_in -> island_out with cap 1 and pleasure score
        adder.add_edge(p_g + p_h + i, p_g + p_h + n + i, 1, max_pleasure - a[i]);

    // castle G becomes our source and castle H becomes our sink
    // all straw from G to H needs to flow through islands
    edge_desc src_edge = adder.add_edge(v_source, 0, s_g, 0);
    adder.add_edge(p_g, v_sink, s_h, 0);

    // calculate flows in both networks
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    int flow = c_map[src_edge] - rc_map[src_edge];

    std::cout << flow << " " << flow * max_pleasure - cost << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}