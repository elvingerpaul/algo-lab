#include <iostream>

using namespace std;

void run_testcase()
{
    int n;
    cin >> n;
    int max = 1, dominoe = 1;

    for (int i = 0; i < n; i++)
    {
        cin >> dominoe;

        if (i < max && i + dominoe > max)
            max = i + dominoe;
    }

    cout << min(n, max) << endl;
}

int main()
{
    int t;
    cin >> t;

    for (int i = 0; i < t; i++)
        run_testcase();

    return 0;
}