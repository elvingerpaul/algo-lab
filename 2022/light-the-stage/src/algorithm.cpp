#include <iostream>
#include <algorithm>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_handle VH;
typedef K::Point_2 P;

struct Person
{
	P p;
	long r;
};

typedef std::vector<P> vpoints;
typedef std::vector<Person> vperson;
typedef std::vector<int> vint;

void testcase()
{
	int m, n; std::cin >> m >> n;

	vperson persons(m);
	for (int i = 0; i < m; i++)
	{
		long x, y, r; std::cin >> x >> y >> r;
		persons[i] = {P(x, y), r};
	}

	long h; std::cin >> h;

	vpoints lamps(n);
	for (int i = 0; i < n; i++)
	{
		long x, y; std::cin >> x >> y;
		lamps[i] = P(x, y);
	}

	int left = 0;
	int right = n - 1;
	vint out(m, n);
	while (left <= right)
	{
		int middle = (left + right) / 2;

		P *begin_itr = &lamps[left];
		P *end_itr = std::next(&lamps[middle]);

		Triangulation t;
		t.insert(begin_itr, end_itr);

		int num_survivors = m;
		for (int i = 0; i < m; i++)
		{
			if (out[i] < middle)
			{
				num_survivors--;
				continue;
			}

			Person pers = persons[i];
			VH nearest = t.nearest_vertex(pers.p);
			K::FT dist = h + pers.r;
			if (CGAL::squared_distance(pers.p, nearest->point()) < dist * dist)
			{
				out[i] = std::min(out[i], middle);
				num_survivors--;
			}
		}

		if (num_survivors == 0)
			right = middle - 1;
		else
			left = middle + 1;
	}

	int highest_round = *std::max_element(out.begin(), out.end());
	for (int i = 0; i < m; i++)
		if (out[i] == highest_round)
			std::cout << i << " ";

	std::cout << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}