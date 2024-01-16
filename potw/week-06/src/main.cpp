#include <iostream>
#include <vector>
#include <CGAL/Gmpq.h>

struct Trip
{
    long y0;
    long x1;
    long y1;
    int idx;

    Trip(long y0, long x1, long y1, int idx) : y0(y0), x1(x1), y1(y1), idx(idx) {}
};

CGAL::Gmpq get_slope(const Trip &t)
{
    return CGAL::Gmpq(t.y1 - t.y0, t.x1);
}

void testcase()
{
    int n; std::cin >> n;

    std::vector<Trip> trips(n, Trip(0, 0, 0, 0));
    for (int i = 0; i < n; i++)
    {
        long y0, x1, y1;
        std::cin >> y0 >> x1 >> y1;
        trips[i] = Trip(y0, x1, y1, i);
    }

    std::sort(trips.begin(), trips.end(), [](const Trip &t1, const Trip &t2) -> bool
              { return t1.y0 > t2.y0; });

    std::vector<bool> lives(n, true);

    CGAL::Gmpq min_slope = get_slope(trips[0]);

    for (int i = 1; i < n; i++)
    {
        CGAL::Gmpq current_slope = get_slope(trips[i]);

        if (CGAL::abs(current_slope) <= CGAL::abs(min_slope))
            min_slope = current_slope;
        else if (current_slope > min_slope)
            lives[trips[i].idx] = false;
    }

    min_slope = get_slope(trips[n - 1]);
    for (int i = n - 2; i >= 0; i--)
    {
        CGAL::Gmpq current_slope = get_slope(trips[i]);

        if (CGAL::abs(current_slope) < CGAL::abs(min_slope) 
            || (CGAL::abs(current_slope) == CGAL::abs(min_slope) && current_slope > min_slope))
            min_slope = current_slope;
        else if (current_slope < min_slope)
            lives[trips[i].idx] = false;
    }

    for (int i = 0; i < n; i++)
        if (lives[i])
            std::cout << i << " ";

    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}