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

void testcase(int n)
{
    int d; std::cin >> d;
    Program lp(CGAL::SMALLER, false, 0, false, 0);

    for (int i = 0; i < n; i++)
    {
        int norm = 0;
        for (int j = 0; j < d; j++)
        {
            int a_ij;
            std::cin >> a_ij;
            norm += a_ij * a_ij;
            lp.set_a(j, i, a_ij);
        }

        // all coefficients guaranteed to be integers
        lp.set_a(d, i, sqrt(norm));

        int b; std::cin >> b;
        lp.set_b(i, b);
    }

    lp.set_l(d, true, 0); // set lower bound for radius to be 0
    lp.set_c(d, -1);      // want to maximize radius r -> minimize -r

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_unbounded())
        std::cout << "inf\n";
    else if (s.is_infeasible())
        std::cout << "none\n";
    else
        std::cout << long(CGAL::to_double(-s.objective_value())) << std::endl;
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