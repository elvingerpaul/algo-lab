#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long ceil_to_long(const CGAL::Quotient<ET> x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return (long)a;
}

void testcase()
{
    int n, m; std::cin >> n >> m;
    long s; std::cin >> s;

    std::vector<std::pair<int, int>> points(n + m);

    Program lp(CGAL::SMALLER, false, 0, false, 0);
    const int b = 0;
    const int c1 = 1;
    const int c2 = 2;
    const int l = 3;

    long sum_x = 0;
    long sum_y = 0;

    // sewage canal c_s: x + by + c1 = 0
    // water canal c_w: -bx + y + c2 = 0
    // choosing coefs for x and y as (1, b) and (-b, 1) guarantees both lines
    // are perpendicular to each other

    // noble houses
    for (int i = 0; i < n; i++)
    {
        int x, y;
        std::cin >> x >> y;
        points[i] = std::make_pair(x, y);
        sum_x -= x;
        sum_y -= y;

        // constraint x + by + c1 <= 0 (noble house to the left or on the line)
        lp.set_a(b, i, y);
        lp.set_a(c1, i, 1);
        lp.set_b(i, -x);
    }

    // common houses
    for (int i = 0; i < m; i++)
    {
        int x, y;
        std::cin >> x >> y;
        points[n + i] = std::make_pair(x, y);
        sum_x += x;
        sum_y += y;

        // constraint x + by + c1 >= 0 (common house to the right or on the line)
        lp.set_a(b, n + i, -y);
        lp.set_a(c1, n + i, -1);
        lp.set_b(n + i, x);
    }

    // check if Cersei's constraint is feasible: nobles left, commons right
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));
    if (!sol.is_optimal())
    {
        std::cout << "Yuck!\n";
        return;
    }

    // add Cersei's constraint and check feasability, sum of sewage pipes must be <= s
    // sewage pipes are horizontal -> distance to c_s is difference in x coord
    // c_s: x + by + c1 = 0 -> x = -by - c1
    // noble houses on the left: dist_to_c_s = x_c_s - x_noble = -b * y_noble - c1 - x_noble
    // common houses to the right: dist_to_cs = x_common - x_c_s = x_common + b * y_noble + c1
    // (sum_x_common - sum_x_noble) + b (sum_y_common - sum_y_noble) + c1 * (m - n) <= s
    int cstr_idx = n + m;
    if (s != -1)
    {
        lp.set_a(b, cstr_idx, sum_y);
        lp.set_a(c1, cstr_idx, m - n);
        lp.set_b(cstr_idx, s - sum_x);
        cstr_idx++;
    }

    // add Tyrion's desire, water pipes should all have length at most l -> minimize l
    // water pipes are vertical -> distance to c_w is difference in y coord
    // c_w: -bx + y + c2 = 0 -> y = bx - c2
    // -l <= y_house - y_c_w <= l
    for (int i = 0; i < n + m; i++)
    {
        lp.set_a(b, cstr_idx, -points[i].first);
        lp.set_a(c2, cstr_idx, -1);
        lp.set_a(l, cstr_idx, -1);
        lp.set_b(cstr_idx, -points[i].second);

        cstr_idx++;
        lp.set_a(b, cstr_idx, points[i].first);
        lp.set_a(c2, cstr_idx, 1);
        lp.set_a(l, cstr_idx, -1);
        lp.set_b(cstr_idx, points[i].second);
        cstr_idx++;
    }

    // minimize for the length l which has a lower bound of 0
    lp.set_l(l, true, 0);
    lp.set_c(l, 1);

    sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));

    if (!sol.is_optimal())
        std::cout << "Bankrupt!\n";
    else
        std::cout << ceil_to_long(sol.objective_value()) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}