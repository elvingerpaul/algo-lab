#include <iostream>

using namespace std;

void do_testcase(){
  int n = 0, num = 0, sum = 0;
  cin >> n;
  
  for (int i = 0; i < n; i++){
    cin >> num;
    sum += num;
  }
  
  cout << sum << endl;
}

int main(){
  int t;
  cin >> t;
  for (int i = 0; i < t; i++)
    do_testcase();
}