#include <iostream>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

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
    int n, m, s;
    std::cin >> n >> m >> s;

    graph G(n + m + s);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    // capacity on houses sold in states, no cost
    for (int i = 0; i < s; i++)
    {
        int cap; std::cin >> cap;
        adder.add_edge(n + m + i, v_sink, cap, 0);
    }

    // link property to the state it is part of
    for (int i = 0; i < m; i++)
    {
        int state; std::cin >> state; // 1 <= state <= s, careful off by one index
        adder.add_edge(n + i, n + m + state - 1, 1, 0);
    }

    // find highest bid and store bids temporarily to avoid neg costs
    std::vector<std::vector<int>> bids(n, std::vector<int>(m));
    int max_bid = std::numeric_limits<int>::min();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int bid;
            std::cin >> bid;
            max_bid = std::max(max_bid, bid);
            bids[i][j] = bid;
        }
    }

    // add bids with corresponding non neg cost between bidder and property
    for (int i = 0; i < n; i++)
    {
        adder.add_edge(v_source, i, 1, 0); // each buyer can buy at most one property
        for (int j = 0; j < m; j++)
            adder.add_edge(i, n + j, 1, max_bid - bids[i][j]);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);

    int flow = 0;
    out_edge_it e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source, G), G); e != eend; ++e)
        flow += c_map[*e] - rc_map[*e];

    std::cout << flow << " " << flow * max_bid - cost << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}