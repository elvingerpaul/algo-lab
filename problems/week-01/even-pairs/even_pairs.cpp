#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vint;

void run_testcase(){
  int n, num;
  cin >> n;
  
  // intialize even with 1, because we consider a fictional 0 at index -1
  int sum = 0, even = 1, odd = 0;
  
  for (int i = 0; i < n; i++){
    cin >> num;
    sum += num;
    if (sum % 2 == 0)
      even++;
    else
      odd++;  
  }
  int num_pairs = even * (even - 1) / 2 + odd * (odd - 1) / 2;
  cout << num_pairs << endl;
}

int main(){
  
  int t;
  cin >> t;
  for (int i = 0; i < t; i++){
    run_testcase();
  }
  return 0;
}
