#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vint;
typedef vector<vint> mint;

void do_testcase(){
  int n;
  cin >> n;
  
  mint m(n+1, vint(n+1, 0));
  
  int sum = 0, num = 0;
  for (int i = 1; i <= n; i++){
    sum = 0;
    for (int j = 1; j <= n; j++){
      cin >> num;
      sum += num;
      m[i][j] = sum;
    }
  }
  
  for (int j = 1; j <= n; j++){
    sum = 0;
    for (int i = 1; i <= n; i++){
      sum += m[i][j];
      m[i][j] = sum;
    }
  }
  
  // a submatrix (i1, i2, j1, j2) is pair if among the cumulated sums
  // m[i2][j2], m[i1-1][j1-1], m[i1-1][j2], m[i1][j2-1] we have
  // 4 even values, 2 even and 2 odd values or 4 odd values
  int ctr = 0;
  for (int i1 = 1; i1 <= n; i1++){
    for (int i2 = i1; i2 <= n; i2++){
      int even = 0, odd = 0;
      
      // for fixed i1 and i2 check how many of m[i1][k] and m[i2][k] are both odd, both even or odd and even
      for (int k = 1; k <= n; k++){
        int res = m[i2][k] - m[i1-1][k];
        if (res % 2 == 0)
          even++;
        else
          odd++;
      }
      // we have now reduced the problem to even pairs here
      ctr += even * (even - 1) / 2 + odd * (odd - 1) / 2 + even;
    }
  }
  
  cout << ctr << endl;
}


int main(){
  int t;
  cin >> t;
  
  for (int i = 0; i < t; i++)
    do_testcase();
    
  return 0;
}