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

#include <iostream>
#include <vector>

typedef std::vector<int> vint;
typedef std::vector<vint> mint;

struct Result
{
    vint sums; // holds the sum of minerals in the subtree
    mint vars; // holds the indexes of each danger variable in the subtree
};

struct Mine
{
    int d;
    vint r;
    int idx; // only set for dangerous mines
};

struct Mineral
{
    int amount;
    int supply;
    int unit_price;
};

typedef std::vector<Mine> vmine;
typedef std::vector<Mineral> vmineral;

vmine mines;
mint g;
Program lp;

int floor_to_int(const CGAL::Quotient<ET> &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return (int)a;
}

Result dfs(int mine, int m)
{
    vint sums(m, 0);
    mint vars(m);

    // gather results of all children
    for (int succ : g[mine])
    {
        Result res = dfs(succ, m);

        // accumulate all resources
        for (int i = 0; i < m; i++)
            sums[i] += res.sums[i];

        // concatenate all variables
        for (int i = 0; i < m; i++)
            for (auto v : res.vars[i])
                vars[i].push_back(v);
    }

    if (mines[mine].d >= 0)
    {
        // danger mine put constraints

        int cstr_offset = (m + 1) * mines[mine].idx;
        int var_offset = m * mines[mine].idx; // each danger mine has one var for each resource

        // sum of all resources must be at most half of danger value
        for (int i = 0; i < m; i++)
        {
            lp.set_a(var_offset + i, cstr_offset, 1);
        }
        lp.set_b(cstr_offset, 0.5 * mines[mine].d);
        cstr_offset++;

        // for each mineral the outcome can be at most half of the sum of all inputs
        for (int i = 0; i < m; i++)
        {
            for (int var_idx : vars[i])
                lp.set_a(var_idx, cstr_offset, -0.5);
            lp.set_a(var_offset + i, cstr_offset, 1);
            lp.set_b(cstr_offset, 0.5 * sums[i]);
            cstr_offset++;
        }

        // set all accumulated sums to 0 again and add vars of this danger mine to result
        sums = vint(m, 0);
        vars = mint(m);
        for (int i = 0; i < m; i++)
            vars[i].push_back(var_offset + i);
    }
    else
    {
        // nomrmal mine, add resources of this mine to accumulation
        for (int i = 0; i < m; i++)
            sums[i] += mines[mine].r[i];
    }

    return {sums, vars};
}

void testcase()
{
    int n, m;
    std::cin >> n >> m;

    mines = vmine(n);
    int danger_idx = 0; // assign each danger a unique id
    for (int i = 0; i < n; i++)
    {
        int d;
        std::cin >> d;

        vint r(m);
        for (int j = 0; j < m; j++)
            std::cin >> r[j];

        if (d >= 0)
            mines[i] = {d, r, danger_idx++};
        else
            mines[i] = {d, r, -1};
    }

    g = mint(n);
    vint degrees(n, 0);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        std::cin >> u >> v;
        g[v].push_back(u); // add edge reverse
        degrees[u]++;      // count in degrees to find source
    }

    vmineral mineral(m);
    for (int i = 0; i < m; i++)
    {
        int c, s, p;
        std::cin >> c >> s >> p;
        mineral[i] = {c, s, p};
    }

    // look for entry mine
    int src = 0;
    for (int i = 0; i < n; i++)
        if (degrees[i] == 0)
        {
            src = i;
            break;
        }

    lp = Program(CGAL::SMALLER, true, 0, false, 0);
    Result res = dfs(src, m); // guaranteed that src is not danger mine

    // ensure bought amount s_i of mineral i is <= supply by setting upperbound
    int var_offset = m * danger_idx;
    for (int i = 0; i < m; i++)
        lp.set_u(var_offset + i, true, mineral[i].supply);

    // sum of vars + potential buy-in for each resource must be greater than required amount
    int cstr_offset = danger_idx * (m + 1);
    for (int i = 0; i < m; i++)
    {
        for (int var_idx : res.vars[i])
        {
            lp.set_a(var_idx, cstr_offset, -1);
        }
        lp.set_a(var_offset + i, cstr_offset, -1); // add potential buy-in
        lp.set_b(cstr_offset, res.sums[i] - mineral[i].amount);
        cstr_offset++;
    }

    // set optimization goal -> minimize price to pay in shop
    for (int i = 0; i < m; i++)
        lp.set_c(var_offset + i, mineral[i].unit_price);

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_optimal())
        std::cout << floor_to_int(s.objective_value()) << "\n";
    else
        std::cout << "Impossible!\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--)
        testcase();
    return 0;
}