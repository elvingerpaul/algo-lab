#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef EK::Point_2 EP;
typedef EK::Segment_2 ES;
typedef EK::Ray_2 ER;

double floor_to_long(const EK::FT &x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

void testcase(int n)
{
    long x; std::cin >> x;
    long y; std::cin >> y;
    long a; std::cin >> a;
    long b; std::cin >> b;
    EK::Point_2 r1(x, y), r2(a, b);
    EK::Ray_2 ray(r1, r2);

    std::vector<ES> segments(n);
    for (int i = 0; i < n; i++)
    {
        long r; std::cin >> r;
        long s; std::cin >> s;
        long t; std::cin >> t;
        long u; std::cin >> u;
        EK::Point_2 s1(r, s), s2(t, u);
        EK::Segment_2 seg(s1, s2);
        segments[i] = seg;
    }

    // shuffle input, might be in bad order that makes us compute a lot of intersections
    random_shuffle(segments.begin(), segments.end());

    EP best_point;
    ES best_seg;
    bool found = false;
    for (int i = 0; i < n; i++)
    {
        ES seg = segments[i];
        EP s1 = seg.source();
        EP s2 = seg.target();

        if (!found && CGAL::do_intersect(ray, seg))
        {
            // no collision found yet, need to use ray for collision detection
            found = true;
            auto o = CGAL::intersection(ray, seg);
            if (const EP *op = boost::get<EP>(&*o))
            { // point intersection
                best_point = *op;
                best_seg = ES(r1, *op);
            }
            else if (const ES *op = boost::get<ES>(&*o))
            { // segment intersection
                best_point = CGAL::compare_distance_to_point(r1, s1, s2) < 0 ? s1 : s2;
                best_seg = ES(r1, best_point);
            }
            else
            {
                throw std::runtime_error("strange segment intersection");
            }
        }
        else if (CGAL::do_intersect(best_seg, seg))
        {
            // found alread collisions, compare to best_seg found so far
            auto o = CGAL::intersection(ray, seg);
            if (const EP *op = boost::get<EP>(&*o))
            { // point intersection
                best_point = *op;
                best_seg = ES(r1, *op);
            }
            else if (const ES *op = boost::get<ES>(&*o))
            { // segment intersection
                best_point = CGAL::compare_distance_to_point(r1, s1, s2) < 0 ? s1 : s2;
                best_seg = ES(r1, best_point);
            }
            else
            {
                throw std::runtime_error("strange segment intersection");
            }
        }
    }

    if (found)
        std::cout << long(floor_to_long(best_point.x())) << " " << long(floor_to_long(best_point.y())) << std::endl;
    else
        std::cout << "no" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;

    while (n != 0)
    {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}