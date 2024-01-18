#include <iostream>
#include <vector>
#include <limits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::pair<int, int> point;
typedef std::vector<point> vpoint;

double floor_to_double(const CGAL::Quotient<ET> &x)
{
    // note we're operating on a negative value, thus we will use floor which will
    // correspond to ceiling its absolute value
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

void testcase()
{
    int n, m, h, w; std::cin >> n >> m >> h >> w;

    vpoint new_posters(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        std::cin >> x >> y;
        new_posters[i] = std::make_pair(x, y);
    }

    vpoint old_posters(m);
    for (int i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        old_posters[i] = std::make_pair(x, y);
    }

    // parameters are scaling factors of new posters -> n variables
    Program lp(CGAL::SMALLER, true, 1, false, 0);

    // add constraint between each pair of new posters
    for (int i = 0; i < n - 1; i++)
    {
        // need a non collision condition between each pair of posters
        for (int j = i + 1; j < n; j++)
        {
            // choose the larger x-growth or y-growth as condition because as soon
            // as the second dimension (the one that takes longer to collision)
            // overlapps we will have a conflict, other dimension by then already
            // overlapped
            int x1 = new_posters[i].first;
            int y1 = new_posters[i].second;
            int x2 = new_posters[j].first;
            int y2 = new_posters[j].second;
            IT x_growth = 2.0 * std::abs(x1 - x2) / w;
            IT y_growth = 2.0 * std::abs(y1 - y2) / h;

            lp.set_a(i, i * n + j, 1);
            lp.set_a(j, i * n + j, 1);
            lp.set_b(i * n + j, std::max(x_growth, y_growth));
        }
    }

    // add upper bound for each new poster based on all old posters
    for (int i = 0; i < n; i++)
    {
        int new_x = new_posters[i].first;
        int new_y = new_posters[i].second;
        ET b = std::numeric_limits<double>::max();
        for (int j = 0; j < m; j++)
        {
            int old_x = old_posters[j].first;
            int old_y = old_posters[j].second;

            IT x_growth = 2.0 * std::abs(new_x - old_x) / w - 1;
            IT y_growth = 2.0 * std::abs(new_y - old_y) / h - 1;

            // take the max between both growths because again the dimension that
            // takes longer to collision will be the bottleneck on the scaling
            // we need to take the minimum among all old posters
            b = std::min(b, std::max(x_growth, y_growth));
        }

        lp.set_u(i, true, b);
    }

    for (int i = 0; i < n; i++)
        lp.set_c(i, (-2) * (w + h));

    Solution s = CGAL::solve_linear_program(lp, ET());
    std::cout << long(-floor_to_double(s.objective_value())) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}