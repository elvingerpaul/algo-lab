#include <iostream>
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

double floor_to_long(const CGAL::Quotient<ET> &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return (long)a;
}

void testcase()
{
    long x_s, y_s, n; std::cin >> x_s >> y_s >> n;

    // create an LP with Ax <= b, no bounds
    Program lp(CGAL::SMALLER, false, 0, false, 0);

    const int X = 0; // x coeff of fire place
    const int Y = 1; // y coeff of fire place
    const int T = 2; // time it takes for first roman to arrive -> maximize

    for (int i = 0; i < n; i++)
    {
        long a, b, c, v;
        std::cin >> a >> b >> c >> v;

        // equations defined as ax + by + c = 0, need to find out if fire point
        // (x_f, y_f) needs to fullfill ax_f + b_f + c <= or >= 0
        // we can plug in start location because we're guaranteed its on same side
        // mul * a * x + mul * b * y + mul * c <= 0 is then always correct
        // for mul = 1 if we need to be below and mul = -1 if we need to be above
        long result = a * x_s + b * y_s + c;
        int mul = (result > 0) ? -1 : 1;

        // ensure fire place is within region bounded by legions -> no crossing
        // in addition we want that dist/v >= t, and maximize for t
        // dist/v = (|ax + by + c|/sqrt(a*a + b*b)) / v >= t
        // results in -a*x -b*y + v * sqrt(a*a + b*b) * t <= -c
        // note that we still need to take mul into account from above
        // below constraints contain condition for point to be in convex hull and
        // to be distant of at least t seconds from any legion
        lp.set_a(X, i, mul * a);
        lp.set_a(Y, i, mul * b);
        lp.set_a(T, i, v * sqrt(a * a + b * b));
        lp.set_b(i, -mul * c);
    }

    // maximize time T and set lower bound on it
    lp.set_l(T, true, 0);
    lp.set_c(T, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    std::cout << floor_to_long(-s.objective_value()) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}