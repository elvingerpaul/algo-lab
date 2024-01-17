#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;

void testcase(int n)
{
    std::vector<P> current(n);
    for (int i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        current[i] = P(x, y);
    }

    Triangulation t;
    t.insert(current.begin(), current.end());

    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        P new_loc(x, y);

        Triangulation::Vertex_handle v = t.nearest_vertex(new_loc);
        std::cout << long(CGAL::squared_distance(new_loc, v->point())) << std::endl;
    }
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