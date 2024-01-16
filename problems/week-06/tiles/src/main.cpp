#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    d_graph;
typedef traits::vertex_descriptor d_vertex_desc;

typedef std::vector<char> vchar;
typedef std::vector<vchar> mchar;

class edge_adder
{
    d_graph &G;

public:
    explicit edge_adder(d_graph &G) : G(G) {}

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

void testcase_matching()
{
    // solve the problem using maximum cardinality matching
    int w, h; std::cin >> w >> h;
    int count = 0;
    mchar garden(h, vchar(w));
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            char c;
            std::cin >> c;
            if (c == '.')
                count++;

            garden[i][j] = c;
        }
    }

    // build graph with w*h nodes
    graph G(w * h);
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (garden[i][j] == 'x')
                continue;

            if (i + 1 < h && garden[i + 1][j] == '.')
                boost::add_edge(i * w + j, (i + 1) * w + j, G);

            if (j + 1 < w && garden[i][j + 1] == '.')
                boost::add_edge(i * w + j, i * w + j + 1, G);
        }
    }

    std::vector<vertex_desc> mate_map(w * h);
    boost::edmonds_maximum_cardinality_matching(G,
                                                boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,
                                             boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

    if (count % 2 == 1 || matching_size * 2 != count)
        std::cout << "no" << std::endl;
    else
        std::cout << "yes" << std::endl;
}

void testcase_maxflow()
{
    // solve the problem using maximum flows
    int w, h; std::cin >> w >> h;
    int count = 0;
    mchar garden(h, vchar(w));
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            char c;
            std::cin >> c;
            if (c == '.')
                count++;

            garden[i][j] = c;
        }
    }

    d_graph G(w * h);
    edge_adder adder(G);

    // add special vertexes source and sink
    const d_vertex_desc v_source = boost::add_vertex(G);
    const d_vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {

            if ((i + j) % 2 == 1 && garden[i][j] == '.')
            {
                // partition A
                adder.add_edge(v_source, i * w + j, 1);

                if (i - 1 >= 0 && garden[i - 1][j] == '.')
                    adder.add_edge(i * w + j, (i - 1) * w + j, 1);

                if (i + 1 < h && garden[i + 1][j] == '.')
                    adder.add_edge(i * w + j, (i + 1) * w + j, 1);

                if (j - 1 >= 0 && garden[i][j - 1] == '.')
                    adder.add_edge(i * w + j, i * w + j - 1, 1);

                if (j + 1 < w && garden[i][j + 1] == '.')
                    adder.add_edge(i * w + j, i * w + j + 1, 1);
            }
            else
            {
                // partition B
                adder.add_edge(i * w + j, v_sink, 1);
            }
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if (count % 2 == 1 || flow * 2 != count)
        std::cout << "no" << std::endl;
    else
        std::cout << "yes" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while (n--)
        testcase_matching();
        // testcase_maxflow();
    return 0;
}