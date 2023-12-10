#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef Delaunay::Vertex_handle Vh;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef K::Point_2 P;
typedef std::pair<P, Index> IPoint;
typedef std::pair<P, P> PairPoint;
typedef K::Segment_2 S;

void testcase()
{
  int n; std::cin >> n;
  int m; std::cin >> m;
  long p; std::cin >> p;

  std::vector<IPoint> points;
  points.reserve(n);
  for (int i = 0; i < n; i++)
  {
    int x, y; std::cin >> x >> y;
    points.emplace_back(P(x, y), i);
  }

  // calculate Delaunay triangulation with jammer nodes only
  Delaunay t;
  t.insert(points.begin(), points.end());

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

  std::sort(edges.begin(), edges.end(),
            [](const Edge &e1, const Edge &e2) -> bool
            {
              return std::get<2>(e1) < std::get<2>(e2);
            });

  // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
  // in theory; for all practical purposes alpha(n) is constant, so linear time.
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && n_components != 1; ++e)
  {
    long length = std::get<2>(*e);

    // break if we're surpassing the power range
    if (length > p)
      break;

    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2)
    {
      uf.link(c1, c2);
      n_components--;
    }
  }

  boost::disjoint_sets_with_storage<> uf_b(n), uf_a(n);
  ;
  EdgeV::const_iterator e_a = edges.begin(), e_b = edges.begin();
  K::FT a = 0, b = 0, req_dist = 0;
  for (int i = 0; i < m; i++)
  {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    P src = P(x1, y1);
    P dest = P(x2, y2);

    // find the nearest jammer from both src and dest
    Vh n_src = t.nearest_vertex(src);
    Vh n_dest = t.nearest_vertex(dest);
    K::FT dist_src = CGAL::squared_distance(n_src->point(), src);
    K::FT dist_dest = CGAL::squared_distance(n_dest->point(), dest);
    K::FT max_dist = 4 * std::max(dist_src, dist_dest);

    Index vi_src = n_src->info();
    Index vi_dest = n_dest->info();

    if (max_dist <= p && uf.find_set(vi_src) == uf.find_set(vi_dest))
    {
      std::cout << "y";

      req_dist = 0;
      while (uf_b.find_set(vi_src) != uf_b.find_set(vi_dest) && e_b != edges.end())
      {
        Index c1 = uf_b.find_set(std::get<0>(*e_b));
        Index c2 = uf_b.find_set(std::get<1>(*e_b));
        req_dist = std::get<2>(*e_b);
        if (c1 != c2)
          uf_b.link(c1, c2);
        e_b++;
      }

      b = std::max({max_dist, req_dist, b});
    }
    else
    {
      std::cout << "n";
    }

    // grow EMST until all missions are supported
    req_dist = 0;
    while (uf_a.find_set(vi_src) != uf_a.find_set(vi_dest) && e_a != edges.end())
    {
      Index c1 = uf_a.find_set(std::get<0>(*e_a));
      Index c2 = uf_a.find_set(std::get<1>(*e_a));
      req_dist = std::get<2>(*e_a);
      if (c1 != c2)
        uf_a.link(c1, c2);
      e_a++;
    }

    a = std::max({max_dist, req_dist, a});
  }

  std::cout << "\n"
            << (long)a << "\n"
            << (long)b << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
  return 0;
}