#include <iostream>
#include <limits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL graph definitions
// =====================
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

// Custom edge adder class
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost)
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
    }
};

void testcase()
{
    int n; std::cin >> n;

    graph G(n);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < n; i++)
    {
        int a, c; std::cin >> a >> c;
        adder.add_edge(v_source, i, a, c);
    }

    // count number of students to be served and get maximal price across all days
    int num_students = 0;
    int max_price = std::numeric_limits<int>::min();
    std::vector<std::pair<int, int>> stud_prices(n);
    for (int i = 0; i < n; i++)
    {
        int s, p; std::cin >> s >> p;
        num_students += s;
        max_price = std::max(max_price, p);
        stud_prices[i] = std::make_pair(s, p);
    }

    // using knowledge of max price, ensure that the no edge weights are negative
    for (int i = 0; i < n; i++)
    {
        int num_students = stud_prices[i].first;
        int price = stud_prices[i].second;
        adder.add_edge(i, v_sink, num_students, max_price - price);
    }

    for (int i = 1; i < n; i++)
    {
        int v, e; std::cin >> v >> e;
        adder.add_edge(i - 1, i, v, e);
    }

    // cycle_cancel is too slow for test set 3
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);

    int flow = 0;
    out_edge_it e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source, G), G); e != eend; ++e)
        flow += c_map[*e] - rc_map[*e];

    std::cout << (flow == num_students ? "possible " : "impossible ");
    std::cout << flow << " " << -(cost - flow * max_price) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}