#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_long(const CGAL::Quotient<ET> &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

void testcase(int n, int m)
{
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    // set upper and lower bounds for each product
    for (int i = 0; i < n; i++)
    {
        int min, max; std::cin >> min >> max;
        lp.set_b(i, max);
        lp.set_b(n + i, -min); // create an upper bound on the min
    }

    for (int j = 0; j < m; j++)
    {
        int unit_price; std::cin >> unit_price;
        lp.set_c(j, unit_price);

        for (int i = 0; i < n; i++)
        {
            int amount_ij;
            std::cin >> amount_ij;
            lp.set_a(j, i, amount_ij);
            lp.set_a(j, n + i, -amount_ij);
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible())
        std::cout << "No such diet.\n";
    else
        std::cout << long(floor_to_long(s.objective_value())) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n, m; std::cin >> n >> m;
    while (n != 0 || m != 0)
    {
        testcase(n, m);
        std::cin >> n >> m;
    }
    return 0;
}