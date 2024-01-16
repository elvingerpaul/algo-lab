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
    int m, n, k, c; std::cin >> m >> n >> k >> c;

    graph G(2 * m * n);
    edge_adder adder(G);

    // add special vertexes source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    int out_offset = m * n;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {

            int out_node_id = out_offset + i * m + j;

            // ensure at most C can flow thorugh a node
            adder.add_edge(i * m + j, out_node_id, c);

            int exit_cap = 0;
            if (i == 0)
            {
                // first row
                exit_cap++;
            }
            else
            {
                // edge up
                adder.add_edge(out_node_id, (i - 1) * m + j, 1);
            }

            if (i == n - 1)
            {
                // last row
                exit_cap++;
            }
            else
            {
                // edge down
                adder.add_edge(out_node_id, (i + 1) * m + j, 1);
            }

            if (j == 0)
            {
                // first col
                exit_cap++;
            }
            else
            {
                // edge left
                adder.add_edge(out_node_id, i * m + j - 1, 1);
            }

            if (j == m - 1)
            {
                // last col
                exit_cap++;
            }
            else
            {
                // edge right
                adder.add_edge(out_node_id, i * m + j + 1, 1);
            }

            if (exit_cap > 0)
            {
                adder.add_edge(out_node_id, v_sink, exit_cap);
            }
        }
    }

    // read in start positions of knigths and add edge with cap 1 to its in_node
    for (int i = 0; i < k; i++)
    {
        int x, y; std::cin >> x >> y;
        adder.add_edge(v_source, y * m + x, 1);
    }

    std::cout << boost::push_relabel_max_flow(G, v_source, v_sink) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--)
        testcase();
    return 0;
}