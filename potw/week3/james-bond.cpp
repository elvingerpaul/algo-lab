#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vint;
typedef vector<vint> mint;

void testcase(){
  int n, m, k; cin >> n >> m >> k;
  vint v(n);
  mint mat(n, vint(n));
  
  for (int i = 0; i < n; i++){
    cin >> v[i];  
  }
  
  // initialize diagonal, depending on if it is the palyer's turn or not
  for (int i = 0; i < n; i++)
    mat[i][i] = ((n-1) % m == k) ? v[i] : 0; 
  
  // fill out matrix diagonal by diagonal such that when accessing
  // mat[left][right], both mat[left+1][right] and mat[left][right-1] exist
  for (int i = 1; i < n; i++){
    for (int j = 0; j < n-i; j++){
      int left = j, right = j + i;
      
      if ((left + n - right - 1) % m == k){
        // tartget player's turn, he can choose the path that will yield maximal returns
        mat[left][right] = max(mat[left+1][right] + v[left], mat[left][right-1] + v[right]);
      } else {
        // opponent's turn, need to assume that opponent chooses path to minimize target player's return
        mat[left][right] = min(mat[left+1][right], mat[left][right-1]);
      }
    }
  }
  
  cout << mat[0][n-1] << endl;
}

int main(){
  int t; cin >> t;
  for (int i = 0; i < t; i++){
    testcase();
  }
}