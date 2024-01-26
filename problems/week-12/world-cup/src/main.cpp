#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Vertex_handle VH;

typedef K::Point_2 P;
typedef std::vector<P> points;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Warehouse
{
    int x;
    int y;
    int s;
    int a;
};

struct Stadium
{
    int x;
    int y;
    int d;
    int u;
};

struct Contour
{
    long x;
    long y;
    long r;
};

typedef std::vector<Warehouse> vw;
typedef std::vector<Stadium> vs;
typedef std::vector<Contour> vc;
typedef std::vector<int> vint;
typedef std::vector<vint> mint;

long floor_to_long(const CGAL::Quotient<ET> &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return (long)a;
}

void testcase()
{
    int n, m, c;
    std::cin >> n >> m >> c;

    // union of warehouses and stadiums for delaunay triangulation
    points p; p.reserve(n + m);

    vw warehouses(n);
    for (int i = 0; i < n; i++)
    {
        int x, y, s, a;
        std::cin >> x >> y >> s >> a;
        warehouses[i] = {x, y, s, a};
        p.emplace_back(P(x, y));
    }

    vs stadiums(m);
    for (int i = 0; i < m; i++)
    {
        int x, y, d, u; std::cin >> x >> y >> d >> u;
        stadiums[i] = {x, y, d, u};
        p.emplace_back(P(x, y));
    }

    mint revenues(n, vint(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            int r; std::cin >> r;
            revenues[i][j] = 100 * r; // *100 to opearate on percent -> keep coefficients integers
        }

    vc contours(c);
    for (int i = 0; i < c; i++)
    {
        long x, y, r; std::cin >> x >> y >> r;
        contours[i] = {x, y, r};
    }

    // unknows are a_{w,s} >= 0, the amount of beer delivered from w to s
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    // ensure deliveries of each warehouse doesn't surpass its supply
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            lp.set_a(i * m + j, i, 1); // sum of all a_{w,s}
        lp.set_b(i, warehouses[i].s);
    }

    int num_cstr = n;
    // ensure each stadium is delivered its exact amount, need to provide >= and <=
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            lp.set_a(j * m + i, num_cstr, 1);      // <= d
            lp.set_a(j * m + i, num_cstr + 1, -1); // >= d
        }
        lp.set_b(num_cstr, stadiums[i].d);
        lp.set_b(num_cstr + 1, -stadiums[i].d);
        num_cstr += 2;
    }

    // ensure we stay below alcool upper bound for each stadium
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            lp.set_a(j * m + i, num_cstr, warehouses[j].a);
        lp.set_b(num_cstr, stadiums[i].u * 100); // * 100, because we operate in percentage and want to keep integer coefficients
        num_cstr++;
    }

    // build Delaunay triangulation to discard possible contour lines
    Delaunay t;
    t.insert(p.begin(), p.end());

    // discard all contours that don't have at least one warehouse/stad in their interior
    // testcases guarantee that there will not be more than 100
    for (int i = 0; i < c; i++)
    {
        P center(contours[i].x, contours[i].y);
        long r = contours[i].r;
        VH v = t.nearest_vertex(center);
        if (CGAL::squared_distance(v->point(), center) <= r * r)
        {

            // check all routes for intersections, 1 point must be in circle the other one not
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    P w(warehouses[i].x, warehouses[i].y);
                    P s(stadiums[j].x, stadiums[j].y);
                    K::FT dist_w = CGAL::squared_distance(w, center);
                    K::FT dist_s = CGAL::squared_distance(s, center);
                    if ((dist_w <= r * r && dist_s > r * r) || (dist_w > r * r && dist_s <= r * r))
                        revenues[i][j]--;
                }
            }
        }
    }

    // objective is to maximize benefits which is the sum of all a_{w,s}*r_{w,s}
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            lp.set_c(i * m + j, -revenues[i][j]);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (!s.is_optimal())
        std::cout << "RIOT!\n";
    else
        std::cout << floor_to_long(-s.objective_value() / 100) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}