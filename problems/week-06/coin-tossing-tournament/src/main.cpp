#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    graph;
typedef traits::vertex_descriptor vertex_desc;

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
        c_map[rev_e] = 0;
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void testcase()
{
    int n, m; std::cin >> n >> m;

    // 0 to n-1: players, n to n+m-1: games
    graph G(n + m);
    edge_adder adder(G);

    // add special vertexes source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < m; i++)
    {
        int a, b, c; std::cin >> a >> b >> c;

        int game_node_id = n + i;
        adder.add_edge(v_source, game_node_id, 1);

        if (c == 1)
        { // player A gets the point
            adder.add_edge(game_node_id, a, 1);
        }
        else if (c == 2)
        { // player B gets the point
            adder.add_edge(game_node_id, b, 1);
        }
        else
        { // result not known, both players could get the point
            adder.add_edge(game_node_id, a, 1);
            adder.add_edge(game_node_id, b, 1);
        }
    }

    long total_score = 0;
    for (int i = 0; i < n; i++)
    {
        int score; std::cin >> score;
        total_score += score;
        adder.add_edge(i, v_sink, score);
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    std::cout << ((flow == m && flow == total_score) ? "yes" : "no") << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--)
        testcase();
    return 0;
}