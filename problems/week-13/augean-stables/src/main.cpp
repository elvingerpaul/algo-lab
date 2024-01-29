#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

struct Stall
{
    int f;
    int c;
    int k;
    int l;
    int m;
};

struct Work
{
    int days_a;
    int days_p;
    long score;
};

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef std::vector<int> vint;
typedef std::vector<Stall> vstall;

const int H1 = 0;
const int H2 = 1;
const int H3 = 2;

vstall stalls;
vint a;
vint p;

int solve_lp(int idx_a, int idx_p, int n)
{
    Program lp(CGAL::SMALLER, true, 0, true, 1);

    for (int q = 0; q < n; q++)
    {
        Stall st = stalls[q];
        lp.set_a(H1, q, -(st.k + a[idx_a] * a[idx_a]));
        lp.set_a(H2, q, -(st.l + p[idx_p] * p[idx_p]));
        lp.set_a(H3, q, -(st.m + a[idx_a] * p[idx_p]));
        lp.set_b(q, st.c - st.f);
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    return s.is_optimal();
}

int find_diag_idx(int n)
{
    int low = 0;
    int up = 24;
    int best = 25;
    while (low <= up)
    {
        int middle = (low + up) / 2;
        if (solve_lp(middle, middle, n))
        {
            best = std::min(best, middle);
            up = middle - 1;
        }
        else
        {
            low = middle + 1;
        }
    }
    return best;
}

void testcase()
{
    int n; std::cin >> n;

    stalls = vstall(n);
    for (int i = 0; i < n; i++)
    {
        int f, c, k, l, m; std::cin >> f >> c >> k >> l >> m;
        stalls[i] = {f, c, k, l, m};
    }

    a = vint(25, 0);
    for (int i = 1; i < 25; i++)
    {
        int ai; std::cin >> ai;
        a[i] = a[i - 1] + ai;
    }

    p = vint(25, 0);
    for (int i = 1; i < 25; i++)
    {
        int pi; std::cin >> pi;
        p[i] = p[i - 1] + pi;
    }

    int idx = find_diag_idx(n);
    if (idx == 25)
    {
        std::cout << "Impossible!\n";
        return;
    }

    int max_hours = 2 * idx; // we have a valid solution for a = idx and p = idx already

    // check matrix [idx:, 0:idx-1]
    int i = idx;
    int j = idx - 1;
    while (i < 25 && j >= 0)
    {
        if (solve_lp(i, j, n))
        {
            max_hours = std::min(max_hours, i + j);
            j--;
        }
        else
        {
            i++;
            j--;
        }
    }

    // check matrix [0:idx-1, idx:]
    i = idx - (2 * idx - max_hours) - 1; // might already have found a better max_hours -> allows better start pos
    j = idx;
    while (i >= 0 && j < 25)
    {
        if (solve_lp(i, j, n))
        {
            max_hours = std::min(max_hours, i + j);
            i--;
        }
        else
        {
            i--;
            j++;
        }
    }

    std::cout << max_hours << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}