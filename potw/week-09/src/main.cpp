#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef K::Point_2 P;
typedef std::pair<P, Index> IPoint;
typedef std::vector<int> vint;

long floor_to_long(const K::FT &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return long(a);
}

void testcase()
{
    int n; std::cin >> n;
    int m; std::cin >> m;
    K::FT s; std::cin >> s;
    int k; std::cin >> k;

    std::vector<IPoint> p;
    p.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        p.emplace_back(P(x, y), i);
    }

    Delaunay t;
    t.insert(p.begin(), p.end());
    EdgeV edges;
    edges.reserve(3 * n + m);             // there can be no more in a planar graph
    std::vector<int> num_bones(n + m, 0); // hold number of bones for each component

    for (int i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        P bone(x, y);

        // add an edge from each bone to its closest tree
        Delaunay::Vertex_handle v = t.nearest_vertex(bone);
        K::FT dist = CGAL::squared_distance(v->point(), bone);
        edges.emplace_back(v->info(), n + i, 4 * dist); // note that a bone doesn't create shadow, reason for *4
        num_bones[n + i] = 1;
    }

    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    {
        Index i1 = e->first->vertex((e->second + 1) % 3)->info();
        Index i2 = e->first->vertex((e->second + 2) % 3)->info();
        // ensure smaller index comes first
        if (i1 > i2)
            std::swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(),
              [](const Edge &e1, const Edge &e2) -> bool
              {
                  return std::get<2>(e1) < std::get<2>(e2);
              });

    int a = 0;
    K::FT b = -1;

    boost::disjoint_sets_with_storage<> uf(n + m);
    EdgeV::const_iterator e = edges.begin();
    for (; e != edges.end(); ++e)
    {
        K::FT dist = std::get<2>(*e);
        if (dist > s)
            break;

        Index v1 = std::get<0>(*e);
        Index v2 = std::get<1>(*e);
        Index c1 = uf.find_set(v1);
        Index c2 = uf.find_set(v2);
        if (c1 != c2)
        {
            int sum_bones = num_bones[c1] + num_bones[c2];
            a = std::max(a, sum_bones);
            uf.link(c1, c2);
            num_bones[uf.find_set(v1)] = sum_bones;

            if (sum_bones >= k && b == -1)
                b = dist;
        }
    }

    // continue to increase radius until we have at least one component that has
    // sufficient number of bones in it
    for (; e != edges.end() && b == -1; ++e)
    {
        K::FT dist = std::get<2>(*e);
        Index v1 = std::get<0>(*e);
        Index v2 = std::get<1>(*e);
        Index c1 = uf.find_set(v1);
        Index c2 = uf.find_set(v2);
        if (c1 != c2)
        {
            int sum_bones = num_bones[c1] + num_bones[c2];
            uf.link(c1, c2);
            num_bones[uf.find_set(v1)] = sum_bones;

            if (sum_bones >= k && b == -1)
                b = dist;
        }
    }

    std::cout << a << " " << floor_to_long(b) << "\n";
    return;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}