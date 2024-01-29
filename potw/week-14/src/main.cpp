#include <iostream>
#include <vector>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property, boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> undirected_graph;
typedef boost::graph_traits<undirected_graph> undirected_traits;
typedef undirected_traits::vertex_descriptor u_vertex_desc;

typedef std::vector<int> vint;
typedef std::vector<vint> mint;

void testcase()
{
    int n, m, a, s, c, d;
    std::cin >> n >> m >> a >> s >> c >> d;

    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    for (int i = 0; i < m; i++)
    {
        char w; std::cin >> w;
        int x, y, z; std::cin >> x >> y >> z;

        edge_desc e;
        e = boost::add_edge(x, y, G).first;
        weights[e] = z;
        if (w == 'L')
        {
            e = boost::add_edge(y, x, G).first;
            weights[e] = z;
        }
    }

    vint agents(a);
    for (int i = 0; i < a; i++)
        std::cin >> agents[i];

    vint shelters(s);
    for (int i = 0; i < s; i++)
        std::cin >> shelters[i];

    // run Dijkstra once for each agent and store paths between all agents and shelters
    mint paths(a, vint(s, -1));

    for (int i = 0; i < a; i++)
    {
        vint dist_map(n);
        boost::dijkstra_shortest_paths(G, agents[i],
                                       boost::distance_map(boost::make_iterator_property_map(
                                           dist_map.begin(), boost::get(boost::vertex_index, G))));

        for (int j = 0; j < s; j++)
            paths[i][j] = dist_map[shelters[j]];
    }

    int left = 0;
    int right = std::numeric_limits<int>::max();

    while (left < right)
    {
        int middle = (left + right) / 2;

        undirected_graph uG(a + s * c);
        for (int i = 0; i < a; i++)
            for (int j = 0; j < s; j++)
                // if capacity of shelter is k > 1, duplicate each shelter node with
                // incident edges, however k-th agent can use shelter only if
                // arriving at least -k*d before launch of avalanche, so there needs
                // to be an edge with lower dist, to ensure that we can process the
                // whole waiting queue in time
                for (int k = 0; k < c; k++)
                    if (paths[i][j] != -1 && paths[i][j] <= middle - k * d)
                        boost::add_edge(i, a + c * j + k, uG);

        std::vector<u_vertex_desc> mate(a + s * c);
        boost::checked_edmonds_maximum_cardinality_matching(uG, &mate[0]);
        int matching_size = boost::matching_size(uG, &mate[0]);

        if (matching_size == a)
            right = middle;
        else
            left = middle + 1;
    }

    std::cout << right + d << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}