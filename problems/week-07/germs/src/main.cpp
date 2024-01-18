#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef std::size_t Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef K::Point_2 P;

typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

void testcase(int n)
{
    int l, b, r, t; std::cin >> l >> b >> r >> t;
    std::vector<std::pair<P, Index>> points(n);

    for (int i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        points[i] = std::make_pair(P(x, y), i);
    }

    Triangulation tri;
    tri.insert(points.begin(), points.end());

    // extract edges and sort by (squared) length
    // This step takes O(n log n) time (for the sorting).
    EdgeV edges;
    edges.reserve(3 * n + n); // we need to add one edge per point for boundary
    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); ++e)
    {
        Index i1 = e->first->vertex((e->second + 1) % 3)->info();
        Index i2 = e->first->vertex((e->second + 2) % 3)->info();
        // ensure smaller index comes first
        if (i1 > i2)
            std::swap(i1, i2);
        edges.emplace_back(i1, i2, tri.segment(e).squared_length());
    }

    // add an edge to closest boundary for each node
    for (int i = 0; i < n; i++)
    {
        int x = points[i].first.x();
        int y = points[i].first.y();
        int min_x = std::min(std::abs(x - l), std::abs(x - r));
        int min_y = std::min(std::abs(y - b), std::abs(y - t));
        long min_xy = std::min(min_x, min_y); // to ensure that we can square the distance

        size_t node_id = points[i].second;
        edges.emplace_back(node_id, node_id + n, 4 * min_xy * min_xy);
    }

    std::sort(edges.begin(), edges.end(),
              [](const Edge &e1, const Edge &e2) -> bool
              {
                  return std::get<2>(e1) < std::get<2>(e2);
              });

    // check whether a bacteria is still alive or not
    int num_alive = n;
    std::vector<bool> alive(n, true);

    int f_out = -1, m_out = -1, l_out = -1;
    EdgeV::const_iterator e = edges.begin();

    for (int i = 0;; i++)
    {
        double current_size = (0.5 + i * i) * (0.5 + i * i);

        for (; e != edges.end() && std::get<2>(*e) <= 4 * current_size; ++e)
        {
            // add edges as long as we're below the current size
            Index v1 = std::get<0>(*e);
            Index v2 = std::get<1>(*e);

            if (v1 < (Index)n && alive[v1])
            {
                num_alive--;

                if (num_alive == n - 1)
                    f_out = i;

                if ((n % 2 == 1 && num_alive == n / 2) || (n % 2 == 0 && num_alive == n / 2 - 1))
                    m_out = i;

                if (num_alive == 0)
                    l_out = i;

                alive[v1] = false;
            }

            if (v2 < (Index)n && alive[v2])
            {
                num_alive--;

                if (num_alive == n - 1)
                    f_out = i;

                if ((n % 2 == 1 && num_alive == n / 2) || (n % 2 == 0 && num_alive == n / 2 - 1))
                    m_out = i;

                if (num_alive == 0)
                    l_out = i;

                alive[v2] = false;
            }
        }

        if (f_out != -1 && m_out != -1 && l_out != -1)
            // note that we could be iterating for quite a while until leaving this loop -> dangerous but tests pass
            // prefer the master solution which guarnatees an upper bound on number of iterations
            break;
    }

    std::cout << f_out << " " << m_out << " " << l_out << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while (n)
    {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}