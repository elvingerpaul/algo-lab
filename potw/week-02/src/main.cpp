#include <iostream>
#include <vector>

void testcase()
{
    int n, k; std::cin >> n >> k;

    std::vector<int> cards(n);
    for (int i = 0; i < n; i++)
        std::cin >> cards[i];

    int left = 0;
    int right = 0;
    int best_left = 0;
    int best_right = 0;
    int window_sum = cards[0];
    int best_diff = std::abs(window_sum - k);

    if (k == 0)
    { // edge case
        std::cout << "0 0\n";
        return;
    }

    // window_sum includes elements from [left, right]
    while (true)
    {
        if ((window_sum < k && right == n - 1) || left == n)
            break;

        if (left == n)
            break;

        if (window_sum < k)
            window_sum += cards[++right];
        else
            window_sum -= cards[left++];

        int diff = std::abs(window_sum - k);
        if (diff < best_diff)
        {
            best_diff = diff;
            best_left = left;
            best_right = right;
            if (diff == 0)
                break; // will never get better than this
        }
    }

    std::cout << best_left << " " << best_right << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}