#include <iostream>
#include <queue>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef K::Point_2 P;
typedef Triangulation::Vertex_handle VH;
typedef Triangulation::Face_handle FH;

void testcase(int n)
{
    std::vector<P> infected(n);
    for (int i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        infected[i] = P(x, y);
    }

    Triangulation t;
    t.insert(infected.begin(), infected.end());

    // iterate over all finite faces and assign them a "node_id"
    // infinite faces all have the "node_id" 0
    int num_nodes = 1;
    for (Triangulation::Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
        f->info() = num_nodes++;

    // calculate max dist between neighboring faces and store in adj list
    std::vector<std::vector<std::pair<int, long>>> graph(num_nodes);
    for (Triangulation::Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
    {
        for (int i = 0; i < 3; i++)
        {
            FH neighbor = f->neighbor(i);
            VH v1 = f->vertex((i + 1) % 3);
            VH v2 = f->vertex((i + 2) % 3);
            long squared_dist = CGAL::squared_distance(v1->point(), v2->point());

            int neighbor_id = (t.is_infinite(neighbor)) ? 0 : neighbor->info();
            graph[f->info()].push_back(std::make_pair(neighbor_id, squared_dist));

            if (neighbor_id == 0)
                // since undirected graph we need edges in both directions but we
                // will not iterate over infinite edges, reason we add it here
                graph[0].push_back(std::make_pair(f->info(), squared_dist));
        }
    }

    // for each path we now want to find the max minimum disk size that is
    // guaranteed to pass, will use Dijkstra idea and will start by putting
    // infinite face with id 0 and minimum disk size inf into the queue
    std::vector<long> best(num_nodes, -1);
    std::priority_queue<std::pair<long, int>> q;
    q.push(std::make_pair(std::numeric_limits<long>::max(), 0));
    while (!q.empty())
    {
        auto el = q.top();
        q.pop(); // remove element from queue

        long max_size_from_src = el.first;
        int src = el.second;

        for (auto neighbor : graph[src])
        {
            int dest = neighbor.first;
            long max_size_to_dest = neighbor.second;
            // keep current best or take bottleneck between src->out or src <-> dest
            long new_best = std::max(best[dest], std::min(max_size_from_src, max_size_to_dest));

            // push destination face to queue only if we were able to improve
            if (new_best != best[dest])
            {
                best[dest] = new_best;
                q.push(std::make_pair(new_best, dest));
            }
        }
    }

    // best[i] should now hold the maximum disk size that is guaranteed to pass
    // for face with id i. For each person check which facet it belongs to and
    // verify if its required distance satisfies the max minimum disk size found
    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        long d; std::cin >> d;
        P escapor(x, y);

        // make sure starting point is far away enough
        VH nearest = t.nearest_vertex(escapor);
        if (CGAL::squared_distance(escapor, nearest->point()) < d)
        {
            std::cout << "n";
            continue;
        }

        // if we're outside convex hull of sick people and far away enough -> trivial
        FH f = t.locate(escapor);
        if (t.is_infinite(f))
        {
            std::cout << "y";
            continue;
        }

        if (best[f->info()] >= 4 * d)
            std::cout << "y";
        else
            std::cout << "n";
    }

    std::cout << std::endl;
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