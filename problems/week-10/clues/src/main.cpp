#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>

// BGL definitions -> for checking bi-partiteness of graph with 2-coloring
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef std::vector<boost::default_color_type> partition_t;
typedef boost::property_map<graph, boost::vertex_index_t>::type index_map_t;
typedef boost::iterator_property_map<partition_t::iterator, index_map_t> partition_map_t;
typedef boost::graph_traits<graph> traits;
typename traits::vertex_iterator vertex_iter, vertex_end;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef Delaunay::Vertex_handle VH;
typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef K::Point_2 P;
typedef std::pair<P, Index> IPoint;

bool verify_triangulation(const Delaunay &t, long r)
{
    // verifies that all edges in the triangulation have a squared length > r*r
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        if (t.segment(e).squared_length() <= r * r)
            return false;

    return true;
}

void testcase()
{
    int n, m; std::cin >> n >> m;
    long r; std::cin >> r;

    std::vector<IPoint> stations;
    stations.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        stations.emplace_back(P(x, y), i);
    }

    Delaunay t;
    t.insert(stations.begin(), stations.end());

    EdgeV edges;
    edges.reserve(3 * n);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        Index i1 = e->first->vertex((e->second + 1) % 3)->info();
        Index i2 = e->first->vertex((e->second + 2) % 3)->info();
        // ensure smaller index comes first
        if (i1 > i2)
            std::swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }

    std::sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2)
              { return std::get<2>(e1) < std::get<2>(e2); });

    graph g(n);
    boost::disjoint_sets_with_storage<> uf(n);

    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e)
    {
        if (std::get<2>(*e) > r * r)
            break;

        Index v1 = std::get<0>(*e);
        Index v2 = std::get<1>(*e);
        Index c1 = uf.find_set(v1);
        Index c2 = uf.find_set(v2);
        boost::add_edge(v1, v2, g);

        if (c1 != c2)
        {
            uf.link(c1, c2);
        }
    }

    // check that delaunay triangulation is bipartite
    partition_t partition(boost::num_vertices(g));
    partition_map_t partition_map(partition.begin(), get(boost::vertex_index, g));
    bool bipartite = is_bipartite(g, get(boost::vertex_index, g), partition_map);

    // still need to check that no interference for a given color
    std::vector<IPoint> white_points, black_points;
    if (bipartite)
    {
        for (boost::tie(vertex_iter, vertex_end) = vertices(g); vertex_iter != vertex_end; ++vertex_iter)
        {
            if (partition[*vertex_iter] == boost::color_traits<boost::default_color_type>::white())
                white_points.push_back(stations[*vertex_iter]);
            else
                black_points.push_back(stations[*vertex_iter]);
        }

        Delaunay white_t, black_t;
        white_t.insert(white_points.begin(), white_points.end());
        black_t.insert(black_points.begin(), black_points.end());
        // check that all edges are longer than r
        bipartite = bipartite && verify_triangulation(white_t, r);
        bipartite = bipartite && verify_triangulation(black_t, r);
    }

    for (int i = 0; i < m; i++)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        if (!bipartite)
        {
            // there is interference somewhere in the network
            std::cout << "n";
            continue;
        }

        P a(x1, y1);
        P b(x2, y2);
        if (CGAL::squared_distance(a, b) <= r * r)
        {
            // direct communication possible
            std::cout << "y";
        }
        else
        {
            // check if both a and b close enough to stations and within same component
            VH a_nearest = t.nearest_vertex(a);
            VH b_nearest = t.nearest_vertex(b);
            Index a_parent = uf.find_set(a_nearest->info());
            Index b_parent = uf.find_set(b_nearest->info());
            long dist_a_nearest = CGAL::squared_distance(a, a_nearest->point());
            long dist_b_nearest = CGAL::squared_distance(b, b_nearest->point());

            if (dist_a_nearest <= r * r && dist_b_nearest <= r * r && a_parent == b_parent)
                std::cout << "y";
            else
                std::cout << "n";
        }
    }

    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}