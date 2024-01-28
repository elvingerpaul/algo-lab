#include <iostream>
#include <vector>

typedef std::vector<int> vint;
typedef std::vector<vint> mint;
typedef std::vector<mint> tint;
typedef std::vector<tint> qint;

vint fighters;
qint dp;

struct Window
{
    // window has always size 3 and might be filled with dummy fighters, type = 0
    int f1;
    int f2;
    int f3;
};

int count_distinct(const Window &w, int m)
{
    // ignore dummy fighters and only consider last m-1 fighters
    vint distinct(5, 0);
    distinct[w.f2] = 1;
    distinct[w.f3] = 1;
    if (m == 3)
        distinct[w.f1] = 1;

    int count = 0;
    for (int i = 1; i < 5; i++)
        count += distinct[i];

    return count;
}

int get_window_idx(const Window &w, size_t m)
{
    // there are 5 possible fighter types including dummy figthers
    // map them to [0,24] by only considering the last m-1 elements
    return (m == 2) ? w.f3 : (5 * w.f2 + w.f3);
}

int solve(int fighter, int count_n, int count_s, Window window_n, Window window_s, int n, size_t m, int k)
{

    if (fighter == n) // no more fighters to process
        return 0;

    if (std::abs(count_n - count_s) > 12) // not possible to achieve pos score with diff more than 12
        return 0;

    int diff_idx = count_n - count_s + 12; // 0 < count_n - count_s + 12 < 24
    int north_idx = get_window_idx(window_n, m);
    int south_idx = get_window_idx(window_s, m);

    if (dp[fighter][north_idx][south_idx][diff_idx] != -1)
    {
        return dp[fighter][north_idx][south_idx][diff_idx];
    }

    int max_score = 0;

    // put fighter to north queue if positive round score
    Window new_window_n = {window_n.f2, window_n.f3, fighters[fighter]};
    int count_dist_n = count_distinct(new_window_n, m);
    int round_score_n = count_dist_n * 1000 - (1 << std::abs(count_n + 1 - count_s));
    if (round_score_n > 0)
        max_score = round_score_n + solve(fighter + 1, count_n + 1, count_s, new_window_n, window_s, n, m, k);

    // put fighter to south queue if positive round score
    Window new_window_s = {window_s.f2, window_s.f3, fighters[fighter]};
    int count_dist_s = count_distinct(new_window_s, m);
    int round_score_s = count_dist_s * 1000 - (1 << std::abs(count_n - count_s - 1));
    if (round_score_s > 0)
        max_score = std::max(max_score, round_score_s + solve(fighter + 1, count_n, count_s + 1, window_n, new_window_s, n, m, k));

    dp[fighter][north_idx][south_idx][diff_idx] = max_score;

    return max_score;
}

void testcase()
{
    int n, k, m; std::cin >> n >> k >> m;

    fighters = vint(n);
    for (int i = 0; i < n; i++)
    {
        int fighter_type;
        std::cin >> fighter_type;
        fighters[i] = fighter_type + 1;
    }

    // n fighters
    // max 25 possible configurations for last 2 fighters in north and south
    // (including dummy fighters to make windows full size -> dummy will be ignored)
    // difference in soldiers: -12 < diff < 12 -> otherwise round is negative
    dp = qint(n, tint(25, mint(25, vint(25, -1))));

    // start clean for each testcase
    Window window_n = {0, 0, 0};
    Window window_s = {0, 0, 0};

    std::cout << solve(0, 0, 0, window_n, window_s, n, m, k) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}