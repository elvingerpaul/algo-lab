#include <iostream>
#include <vector>
#include <limits>

using namespace std;

typedef vector<int> vint;
typedef vector<vint> mint;

int play_iter(const mint& g, int n, int r, int b){
  vint v_min(n, -1);
  vint v_max(n, -1);
  v_min[n-1] = 0;
  v_max[n-1] = 0;
  
  for (int i = n-2; i >= 0; i--){
    int mini = numeric_limits<int>::max();
    int maxi = numeric_limits<int>::min();
    for (int j: g[i]){
      mini = min(mini, v_max[j]);
      maxi = max(maxi, v_min[j]);
    }
    v_min[i] = mini + 1;
    v_max[i] = maxi + 1;
  }
  
  return (v_min[r] < v_min[b] || (v_min[r] == v_min[b] && v_min[r] % 2 == 1)) ? 0 : 1;
}

void run_testcase_iter(){
// iterative solution
  int n, m, r, b, u, v;
  cin >> n >> m >> r >> b;
  
  mint g(n, vint());
  for (int i = 0; i < m; i++){
    cin >> u >> v;
    g[u-1].push_back(v-1);
  }
  
  cout << play_iter(g, n, r-1, b-1) << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  for (int i = 0; i < t; i++){
    run_testcase_iter();
  }
  return 0;
}