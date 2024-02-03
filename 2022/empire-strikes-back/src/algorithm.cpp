#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_handle VH;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef K::Point_2 P;

struct Particle
{
	P p;
	int d;
};

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::vector<Particle> vp;
typedef std::vector<P> vpoints;

void testcase()
{
	int a, s, b, e; std::cin >> a >> s >> b >> e;

	vp particles(a);
	for (int i = 0; i < a; i++)
	{
		int x, y, d; std::cin >> x >> y >> d;
		particles[i] = {P(x, y), d};
	}

	vpoints shots(s);
	for (int i = 0; i < s; i++)
	{
		int x, y; std::cin >> x >> y;
		shots[i] = P(x, y);
	}

	vpoints bounties(b);
	for (int i = 0; i < b; i++)
	{
		int x, y; std::cin >> x >> y;
		bounties[i] = P(x, y);
	}

	Triangulation t;
	t.insert(bounties.begin(), bounties.end());

	// energy of each shot are unknowns
	Program lp(CGAL::SMALLER, true, 0, false, 0);

	// ensure each particle is destroyed
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < s; j++)
		{
			K::FT denom = CGAL::squared_distance(particles[i].p, shots[j]);

			if (b != 0)
			{
				// if there are bounties, check whether the radius of this shot can extend to this particle
				// without having a bounty within that radius
				VH nearest_bounty = t.nearest_vertex(shots[j]);
				K::FT squared_dist_to_bounty = CGAL::squared_distance(shots[j], nearest_bounty->point());

				if (squared_dist_to_bounty <= denom)
					continue;
			}

			if (denom == 0)
				lp.set_a(j, i, -1);
			else
				lp.set_a(j, i, -1 / (double)denom);
		}
		lp.set_b(i, -particles[i].d);
	}

	// ensure we don't exceed the total energy available
	for (int j = 0; j < s; j++)
		lp.set_a(j, a, 1);
	lp.set_b(a, e);

	Solution sol = CGAL::solve_linear_program(lp, ET());
	assert(sol.solves_linear_program(lp));

	if (sol.is_optimal())
		std::cout << "y\n";
	else
		std::cout << "n\n";
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
