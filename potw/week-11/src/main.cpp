/// 4
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
    int c, g, b, k, a;
    std::cin >> c >> g >> b >> k >> a;

    graph G(c);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    int sum_elephants = 0;
    for (int i = 0; i < g; i++)
    {
        int x, y, d, e;
        std::cin >> x >> y >> d >> e;
        sum_elephants += e;
        adder.add_edge(x, y, e, d);
    }

    // or edge_desc source_edge = boost::edge(v_source, k, G).first;
    edge_desc source_edge = adder.add_edge(v_source, k, sum_elephants, 0);
    adder.add_edge(a, v_sink, sum_elephants, 0);

    int left = 0;
    int right = sum_elephants;
    int best = 0;
    while (left <= right)
    {
        int middle = (left + right) / 2;

        // try to send middle suitcases through network
        // by adjusting the capacity of the source edge, we control what the max flow can be
        c_map[source_edge] = middle;
        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
        int cost = boost::find_flow_cost(G);

        if (cost <= b)
        {
            // we're within budget
            int s_flow = c_map[source_edge] - rc_map[source_edge];
            if (s_flow < middle)
            {
                // all capacities saturated -> cannot send more suitcases through
                std::cout << s_flow << "\n";
                return;
            }

            best = std::max(best, s_flow);
            left = middle + 1; // try to send even more suitcases through
        }
        else
        {
            right = middle - 1; // to expensive, try with less suitcases
        }
    }

    std::cout << best << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}