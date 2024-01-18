/// 4
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

typedef std::pair<int, int> pint;

void testcase()
{
    int n; std::cin >> n;
    std::vector<int> times(n);
    std::vector<pint> idx_times(n);

    for (int i = 0; i < n; i++)
    {
        int t; std::cin >> t;
        times[i] = t; // time[i] = -1 means bomb i is deactivated
        idx_times[i] = std::make_pair(t, i);
    }

    std::sort(idx_times.begin(), idx_times.end(), [](const pint &p1, const pint &p2)
              { return p1.first < p2.first; });

    int timer = 0;

    for (int i = 0; i < n; i++)
    {
        int curr_bomb_idx = idx_times[i].second;

        if (times[curr_bomb_idx] != -1)
        {
            std::stack<int> s;
            s.push(curr_bomb_idx);

            while (!s.empty())
            {
                int idx = s.top();

                if (2 * idx + 2 < n)
                {
                    // check if there are lower bombs that need to be deactivated first
                    if (times[2 * idx + 1] != -1)
                        s.push(2 * idx + 1);
                    if (times[2 * idx + 2] != -1)
                        s.push(2 * idx + 2);
                }

                if (2 * idx + 2 >= n || (times[2 * idx + 1] == -1 && times[2 * idx + 2] == -1))
                {
                    // either no lower bombs or already deactivated

                    if (times[idx] <= timer)
                    { // we're too late
                        std::cout << "no\n";
                        return;
                    }

                    // deactivate bomb
                    times[idx] = -1;
                    timer++;
                    s.pop();
                }
            }
        }
    }

    std::cout << "yes\n";
}

int main()
{
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}