#include <iostream>
#include <vector>
#include <limits>

using namespace std;

typedef vector<int> vint;
typedef vector<vint> mint;

void play_rec(const mint& g, vint& v_min, vint& v_max, int pos, int n){
  for(int i: g[pos]){
    if (v_min[i] == -1)
      // ensure result of successors is known
      play_rec(g, v_min, v_max, i, n);
  }
  
  int mini = numeric_limits<int>::max();
  int maxi = numeric_limits<int>::min();
  
  for(int i: g[pos]){
    mini = min(mini, v_max[i]);
    maxi = max(maxi, v_min[i]);
  }
  
  v_min[pos] = mini + 1;
  v_max[pos] = maxi + 1;
}

void run_testcase_rec(){
    // recursive solution
  int n, m, r, b, u, v;
  cin >> n >> m >> r >> b;
  
  mint g(n+1, vint());
  for (int i = 0; i < m; i++){
    cin >> u >> v;
    g[u].push_back(v);
  }
  
  // dummy edge to red and blue start pos to ensure they are part of dfs
  g[0].push_back(r);
  if (r != b)
    g[0].push_back(b);
  
  vint v_min(n+1, -1);
  vint v_max(n+1, -1);
  v_min[n] = 0;
  v_max[n] = 0;
  
  play_rec(g, v_min, v_max, 0, n);
  
  if (v_min[r] < v_min[b] || (v_min[r] == v_min[b] && v_min[r] % 2 == 1))
    cout << "0" << endl;
  else
    cout << "1" << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  for (int i = 0; i < t; i++){
    run_testcase_rec();
  }
  return 0;
}