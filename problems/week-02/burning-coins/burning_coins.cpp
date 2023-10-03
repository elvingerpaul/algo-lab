#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vint;
typedef vector<vint> mint;

void testcase()
{
    int n;
    cin >> n;

    vint v(n);
    mint m(n, vint(n));

    for (int i = 0; i < n; i++)
        cin >> v[i];

    // DP: initialization of main diagonal
    for (int i = 0; i < n; i++)
        m[i][i] = ((n - 1) % 2 == 0) ? v[i] : 0;

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            int left = j, right = j + i;

            if ((left + n - right - 1) % 2 == 0)
                // our turn
                m[left][right] = max(m[left + 1][right] + v[left], m[left][right - 1] + v[right]);
            else
                // opponent's turn need to assume strategy that minimizes our winning
                m[left][right] = min(m[left + 1][right], m[left][right - 1]);
        }
    }

    cout << m[0][n - 1] << endl;
}

int main()
{
    ios_base::sync_with_stdio(false);

    int t;
    cin >> t;
    for (int i = 0; i < t; i++)
        testcase();
}