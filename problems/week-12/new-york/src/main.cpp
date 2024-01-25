#include <iostream>
#include <vector>
#include <limits>
#include <set>

typedef std::vector<int> vint;
typedef std::vector<vint> adj;
typedef std::set<int> sint;
typedef std::multiset<int> mint;

// define as global such that they don't have to be passed around
sint sols;          // will hold all starting points -> set to avoid duplicates
mint sorted_window; // sorted multiset that holds all the elements that are on the path from idx[start_wd] to idx[end_wd]
vint idx;           // holds the indexes of the elements that form sliding window
vint temp;          // holds the temperatures of each point
adj g;

void solve(int curr_node, int start_wd, int end_wd, int k, size_t m)
{
    if (sorted_window.size() == m)
    {
        // multiset is sorted, min will be first element, max will be last element
        int min_element = *(sorted_window.begin());
        int max_element = *(sorted_window.rbegin());
        if (max_element - min_element <= k)
            sols.insert(idx[start_wd]);

        // size already m -> when adding child to window we need to remove element
        // that is currently at the front of window
        int idx_start_node = idx[start_wd];
        sorted_window.erase(sorted_window.find(temp[idx_start_node]));

        for (int child : g[curr_node])
        {
            sorted_window.insert(temp[child]);                    // add child element to window
            idx[end_wd + 1] = child;                              // this child denotes end of new window
            solve(child, start_wd + 1, end_wd + 1, k, m);         // solve problem for child
            sorted_window.erase(sorted_window.find(temp[child])); // remove child again from window, we're moving into a new subtree
        }

        // restore the window to how it was when this method was called
        sorted_window.insert(temp[idx_start_node]);
    }
    else
    {
        // window doesn't have size m yet
        for (int child : g[curr_node])
        {
            sorted_window.insert(temp[child]);                    // add child element to window
            idx[end_wd + 1] = child;                              // this child denotes end of new window
            solve(child, start_wd, end_wd + 1, k, m);             // solve problem for child
            sorted_window.erase(sorted_window.find(temp[child])); // remove child again from window
        }
    }
}

void testcase()
{
    int n, m, k; std::cin >> n >> m >> k;

    // clean up and intialize all datastructures
    sorted_window.clear();
    sols.clear();
    temp = vint(n);
    idx = vint(n);
    g = adj(n);

    for (int i = 0; i < n; i++)
        std::cin >> temp[i];

    for (int i = 0; i < n - 1; i++)
    {
        int u, v; std::cin >> u >> v;
        g[u].push_back(v);
    }

    // initialization -> sliding window contains only node 0
    sorted_window.insert(temp[0]);
    idx[0] = 0;
    solve(0, 0, 0, k, m);
    if (sols.size() == 0)
        std::cout << "Abort mission";
    else
        for (int s : sols)
            std::cout << s << " ";

    std::cout << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}