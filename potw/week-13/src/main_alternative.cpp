#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::vector<int> vint;
typedef std::vector<vint> mint;
typedef std::vector<std::pair<int, int>> vtint;

vtint danger;
mint resources;
mint g;
Program lp;

long floor_to_long(const CGAL::Quotient<ET> &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return (long)a;
}

void dfs_mines(int root, int curr, int m)
{
    int danger_idx_root = danger[root].second;

    for (int succ : g[curr])
    {

        if (danger[succ].first >= 0)
        {
            int danger_idx_succ = danger[succ].second;

            int var_offset = danger_idx_succ * m;
            int cstr_offset = danger_idx_root * (m + 1);
            for (int i = 0; i < m; i++) // each resource must be halved after mine
                lp.set_a(var_offset + i, cstr_offset + i, -1);
        }
        else
        {
            // add resources of successor to root and continue recursively at succ
            for (int i = 0; i < m; i++)
                resources[root][i] += resources[succ][i];
            dfs_mines(root, succ, m);
        }
    }

    if (root == curr)
    {
        int danger_threshold = danger[root].first;

        int var_offset = danger_idx_root * m;
        int cstr_offset = danger_idx_root * (m + 1);
        for (int i = 0; i < m; i++)
        {
            // each resource must be halfed afer mine
            lp.set_a(var_offset + i, cstr_offset + i, 2);
            lp.set_b(cstr_offset + i, resources[root][i]);

            // sum of all resources must not exceed mine capacity
            lp.set_a(var_offset + i, cstr_offset + m, 2);
        }

        // sum of resources must not exceed mine capacity
        lp.set_b(cstr_offset + m, danger_threshold);
    }
}

void dfs_root(int root, int curr, int m, int offset)
{
    // get all resources from roots children and set constraints for dangerous children
    for (int succ : g[curr])
    {

        if (danger[succ].first >= 0)
        {
            int danger_idx_succ = danger[succ].second;

            int var_offset = danger_idx_succ * m;
            for (int i = 0; i < m; i++)
                lp.set_a(var_offset + i, offset + i, -1);
        }
        else
        {
            // add resources of successor to root and continue recursively at succ
            for (int i = 0; i < m; i++)
                resources[root][i] += resources[succ][i];
            dfs_root(root, succ, m, offset);
        }
    }
}

void testcase()
{
    int n, m; std::cin >> n >> m;

    int danger_idx = 0;
    danger = vtint(n);
    resources = mint(n, vint(m, 0));
    lp = Program(CGAL::SMALLER, true, 0, false, 0);

    for (int i = 0; i < n; i++)
    {
        int d; std::cin >> d;
        if (d >= 0)
            danger[i] = std::make_pair(d, danger_idx++);
        else
            danger[i] = std::make_pair(d, -1);

        for (int j = 0; j < m; j++)
            std::cin >> resources[i][j];
    }

    g = mint(n);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v; std::cin >> u >> v;
        g[v].push_back(u);
    }

    for (int i = 0; i < n; i++)
        if (danger[i].first >= 0)
            dfs_mines(i, i, m);

    int cstr_offset = (m + 1) * danger_idx;
    int var_offset = danger_idx * m;

    // need to sum up all resrouces from children, source assumed to be at 0
    dfs_root(0, 0, m, cstr_offset);

    // set constraint on required amount for each resource
    for (int i = 0; i < m; i++)
    {
        int c, s, p; std::cin >> c >> s >> p;
        lp.set_u(var_offset + i, true, s);
        lp.set_a(var_offset + i, offset + i, -1);
        lp.set_b(offset + i, resources[0][i] - c);
        lp.set_c(var_offset + i, p);
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if (s.is_optimal())
        std::cout << floor_to_long(s.objective_value()) << "\n";
    else
        std::cout << "Impossible!\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}