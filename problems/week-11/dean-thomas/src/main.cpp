#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>

// Epec kernel is required, because of constructions and
// squared dist doesn't fit into double
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef size_t Index;
typedef std::pair<bool, K::FT> FI;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FI, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef Delaunay::Face_handle FH;
typedef Delaunay::Vertex_handle VH;
typedef Delaunay::Finite_faces_iterator Face_iterator;

typedef K::Point_2 P;
typedef std::pair<P, Index> IPoint;
typedef std::vector<IPoint> vpoint;

void testcase()
{
    int n, m; std::cin >> n >> m;
    K::FT d; std::cin >> d;

    vpoint dementors;
    dementors.reserve(n);
    for (int i = 0; i < n; i++)
    {
        K::FT x, y; std::cin >> x >> y;
        dementors.emplace_back(P(x, y), i);
    }

    Delaunay t;
    t.insert(dementors.begin(), dementors.end());

    std::priority_queue<std::pair<K::FT, FH>> q;

    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    {

        /* initialization of face: calculate if we get biggest potential dist from
        dementors in center of face or by moving into one of its infinite faces if
        it exists
        Attention: t.dual(f) -> construction */
        K::FT bot = CGAL::squared_distance(f->vertex(0)->point(), t.dual(f));
        for (int i = 0; i < 3; i++)
        {
            if (!t.is_infinite(f->neighbor(i)))
                continue;
            bot = std::max(bot, t.segment(f, i).squared_length());
        }

        f->info() = std::make_pair(false, bot);
        q.push(std::make_pair(bot, f));
    }

    /* Dijkstra approach: we know the current best distance we can get for each
    face when either moving to its center or to a potential infinite neighbor
    face. Always pop the face f with the current biggest known distance and check
    if its finite neighboring faces can benefit from moving to f. After
    having potentially updated all neighbors and added them to the queue again,
    f is processed. There is no way it could be later on improved. Number of
    processed faces is guaranteed to decrease by one in each while iteration
    -> termination guaranteed. */
    while (!q.empty())
    {
        auto f = q.top();
        q.pop();
        FH face = f.second;

        if (face->info().first) // face already processed
            continue;

        for (int i = 0; i < 3; i++)
        {
            FH neighbor = face->neighbor(i);
            if (neighbor->info().first || t.is_infinite(neighbor)) // neighbor already processed
                continue;

            auto bot_to_neighbor = std::min(face->info().second, t.segment(face, i).squared_length());
            neighbor->info().second = std::max(neighbor->info().second, bot_to_neighbor);
            q.push(std::make_pair(neighbor->info().second, neighbor));
        }

        face->info().first = true;
    }

    for (int i = 0; i < m; i++)
    {
        long x, y; std::cin >> x >> y;
        double s; std::cin >> s;
        P p(x, y);

        K::FT single_dist = (d + s) * (d + s);
        K::FT double_dist = 4 * single_dist;
        VH nearest_v = t.nearest_vertex(p);

        if (CGAL::squared_distance(nearest_v->point(), p) >= double_dist)
        {
            // initial locatioin is more than 2d + 2s away from closest dementor
            std::cout << "y";
        }
        else if (CGAL::squared_distance(nearest_v->point(), p) < single_dist)
        {
            // inital location is whithin d + s of a potential dementor
            std::cout << "n";
        }
        else
        {
            FH location = t.locate(p);
            if (t.is_infinite(location) || location->info().second >= double_dist)
                // either we're already in an infinite face or we can get to a face
                // where we are more than 2d+2s away from closest dementor
                std::cout << "y";
            else
                std::cout << "n";
        }
    }

    std::cout << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}