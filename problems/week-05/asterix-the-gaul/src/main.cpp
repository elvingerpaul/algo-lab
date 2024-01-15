#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

typedef std::vector<long> vlong;

typedef std::pair<long, long> plong;
typedef std::vector<plong> vplong;

vplong calculate_subset_sums(const vplong &moves, int start_idx, int size, long time_lim)
{
    vplong sums;
    for (int i = 0; i < (1 << size); i++)
    {
        long dist = 0;
        long tot_time = 0;

        for (int j = 0; j < size; j++)
        {
            if (i & (1 << j))
            {
                tot_time += moves[start_idx + j].first;
                dist += moves[start_idx + j].second;

                if (tot_time >= time_lim)
                    // we're already past time limit
                    break;
            }
        }

        // only add it if we're below the time limit
        if (tot_time < time_lim)
            sums.push_back(std::make_pair(tot_time, dist));
    }

    return sums;
}

bool split_lists(const vplong &first_half, vplong second_half, long t, long d)
{

    // sort second half in increasing total times
    std::sort(second_half.begin(), second_half.end(),
              [](const plong &p1, const plong &p2) -> bool
              {
                  return p1.first < p2.first;
              });

    // assume (t1, d1) and (t2, d2) with t1 < t2 but d1 > d2 then modify (t2, d2)
    // to (t2, d1) as in time t2 we can still run the longer distance d2
    for (int i = 1; i < int(second_half.size()); i++)
        second_half[i].second = std::max(second_half[i].second, second_half[i - 1].second);

    // for each pair in first half, find largest t such that we still remain in time limit
    for (plong p1 : first_half)
    {
        long rem_time = t - p1.first - 1;

        auto itr = std::upper_bound(second_half.begin(), second_half.end(), std::make_pair(rem_time, 0),
                                    [](const plong &p1, const plong &p2) -> bool
                                    {
                                        return p1.first < p2.first;
                                    });

        // return true if there is a matching element in 2nd half with which we're
        // above required distance
        if ((itr != second_half.begin()) && (p1.second + (itr - 1)->second >= d))
        {
            return true;
        }
    }

    return false;
}

void testcase()
{
    int n, m;
    std::cin >> n >> m;
    long d, t;
    std::cin >> d >> t;

    vplong moves(n);
    for (int i = 0; i < n; i++)
    {
        long dist, ti;
        std::cin >> dist >> ti;
        moves[i] = std::make_pair(ti, dist);
    }

    vlong potions(m);
    for (int i = 0; i < m; i++)
        std::cin >> potions[i];

    // split moves into two halves and calculate subset sums
    int size_a = n / 2;
    int size_b = n - size_a;
    vplong first_half = calculate_subset_sums(moves, 0, size_a, t);
    vplong second_half = calculate_subset_sums(moves, size_a, size_b, t);

    // verify if possible without any gulps
    if (split_lists(first_half, second_half, t, d))
    {
        std::cout << "0\n";
        return;
    }

    long left = 0;
    long right = m - 1;
    while (left <= right)
    {
        long middle = (left + right) / 2;

        // add potion to each move
        for (int i = 0; i < n; i++)
            moves[i].second += potions[middle];

        first_half = calculate_subset_sums(moves, 0, size_a, t);
        second_half = calculate_subset_sums(moves, size_a, size_b, t);

        if (split_lists(first_half, second_half, t, d))
            right = middle - 1;
        else
            left = middle + 1;

        // restore original moves
        for (int i = 0; i < n; i++)
            moves[i].second -= potions[middle];
    }

    if (left < m)
        std::cout << left + 1 << std::endl;
    else
        std::cout << "Panoramix captured" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}