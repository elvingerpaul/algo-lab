#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
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

typedef std::vector<int> vint;
typedef std::vector<vint> mint;

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

void add_adv_knight(const mint &board, edge_adder &adder, int i, int j, int k, int l, int n)
{
    if (k >= 0 && k < n && l >= 0 && l < n && board[k][l])
        adder.add_edge(i * n + j, k * n + l, 1);
}

void testcase()
{
    int n; std::cin >> n;

    mint board(n, vint(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            std::cin >> board[i][j];

    graph G(n * n);
    edge_adder adder(G);

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    int num_holes = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {

            int curr_node = i * n + j;

            if (board[i][j] == 0)
            {
                num_holes++;
                continue; // current position is a hole
            }

            if ((i + j) % 2 == 1)
            {
                adder.add_edge(curr_node, v_sink, 1);
                continue; // add only edges from A to B
            }

            adder.add_edge(v_source, curr_node, 1);

            // add all conflicting knights if they are on the board
            add_adv_knight(board, adder, i, j, i - 1, j - 2, n);
            add_adv_knight(board, adder, i, j, i - 1, j + 2, n);
            add_adv_knight(board, adder, i, j, i + 1, j - 2, n);
            add_adv_knight(board, adder, i, j, i + 1, j + 2, n);
            add_adv_knight(board, adder, i, j, i - 2, j - 1, n);
            add_adv_knight(board, adder, i, j, i - 2, j + 1, n);
            add_adv_knight(board, adder, i, j, i + 2, j - 1, n);
            add_adv_knight(board, adder, i, j, i + 2, j + 1, n);
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << n * n - num_holes - flow << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}