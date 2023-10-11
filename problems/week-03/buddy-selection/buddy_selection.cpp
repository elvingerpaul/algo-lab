#include <iostream>
#include <vector>
#include <unordered_set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef vector<unordered_set<string>> vset;
typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS>
    graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_desc;

void testcase()
{
    int n, c, f;
    cin >> n >> c >> f;

    vset interests(n);
    graph_t G(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < c; j++)
        {
            string interest;
            cin >> interest;
            interests[i].insert(interest);
        }

        for (int j = 0; j < i; j++)
        {
            // manual set intersection, set_intersection function turns out to be much slower
            // basic operations with simple sets, log(n) while O(1) with unordered_sets
            int common = 0;
            for (auto el : interests[j])
                if (interests[i].count(el) > 0)
                    common++;

            if (common > f)
                boost::add_edge(i, j, G);
        }
    }

    vector<vertex_desc> mate(n);
    boost::edmonds_maximum_cardinality_matching(G, &mate[0]);

    // n/2 edges in the matchings means a mate for everyone
    cout << ((boost::matching_size(G, &mate[0]) == n / 2) ? "not optimal" : "optimal") << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
        testcase();
}