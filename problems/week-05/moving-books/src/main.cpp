#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

typedef std::vector<int> vint;

void testcase(){
  int n, m; std::cin >> n >> m;
  
  vint friends(n);
  for (int i = 0; i < n; i++)
    std::cin >> friends[i];
    
  int max_weight = 0;
  std::multiset<int> weights;
  for (int i = 0; i < m; i++){
    int weight; std::cin >> weight;
    max_weight = std::max(max_weight, weight);
    weights.insert(weight);
  }
  
  // sort friends by increasing strength
  std::sort(friends.begin(), friends.end());
  
  // check if it is possible to carry all boxes
  if (max_weight > friends[n-1]){
    std::cout << "impossible\n";
    return;
  }
  
  // as long as there are boxes to carry assign each friend biggest possible box
  int start = 0;
  int num_carries = 0;
  while (!weights.empty()){
    
    for (int i = start; i < n && weights.size() > 0; i++){
      auto itr = weights.upper_bound(friends[i]);
      
      // this friend cannot carry one of the remaining packages
      if (itr == weights.begin()){
        // this friend no longer needs to be considered for future iterations
        start = i+1;
        continue;
      }
      
      // friend i will carry the package before itr
      weights.erase(--itr);
    }
    num_carries++;
  }
  
  std::cout << (num_carries-1)*3 + 2 << std::endl;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
    return 0;
}