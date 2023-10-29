#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef std::vector<P> VP;

bool is_contained(const VP& t, P s1, P s2){
  /*checks if a segement [s1,s2] is contained within triangle defined by lines 
  l(t[0], t[1]), l(t[2], t[3]), l(t[4], t[5]). The points might be given in any
  order (not necessarily clock or counter-clock order)*/
  bool l1 = (CGAL::orientation(t[0], t[1], t[2]) > 0) ?
            CGAL::orientation(t[0], t[1], s1) >= 0 && CGAL::orientation(t[0], t[1], s2) >= 0 :
            CGAL::orientation(t[0], t[1], s1) <= 0 && CGAL::orientation(t[0], t[1], s2) <= 0;
  
  bool l2 = (CGAL::orientation(t[2], t[3], t[4]) > 0) ?
            CGAL::orientation(t[2], t[3], s1) >= 0 && CGAL::orientation(t[2], t[3], s2) >= 0 :
            CGAL::orientation(t[2], t[3], s1) <= 0 && CGAL::orientation(t[2], t[3], s2) <= 0;
            
  bool l3 = (CGAL::orientation(t[4], t[5], t[0]) > 0) ?
            CGAL::orientation(t[4], t[5], s1) >= 0 && CGAL::orientation(t[4], t[5], s2) >= 0 :
            CGAL::orientation(t[4], t[5], s1) <= 0 && CGAL::orientation(t[4], t[5], s2) <= 0;

  return l1 && l2 && l3;
}

void testcase(){
  int m; std::cin >> m;
  int n; std::cin >> n;
  
  VP p(m);
  for (int i = 0; i < m; i++){
    int x; std::cin >> x;
    int y; std::cin >> y;
    p[i] = P(x,y);
  }
  
  std::vector<std::vector<int>> inter(n);
  for (int j = 0; j < n; j++){
    VP tri(6);
    for (int i = 0; i < 6; i++){
      int x; std::cin >> x;
      int y; std::cin >> y;
      tri[i] = P(x, y);
    }
    
    for (int i = 0; i < m-1; i++){
      if (is_contained(tri, p[i], p[i+1])){
        inter[j].push_back(i);
      }
    }
  }
  
  // sliding window
  int sum = 0;
  int mini = std::numeric_limits<int>::max();
  std::vector<int> presence(m-1, 0);
  
  for (int left = 0, right = 0; left <= right && left < n; left++){
    while (sum < m-1 && right < n){
      // we're missing paths, increase right pointer
      for(int path: inter[right]){
        // adding new path to window
        if (++presence[path] == 1)
          sum++;
      }
      right++;
    }
    
    if (sum == m-1)
      mini = std::min(mini, right-left);
      
    for (int path: inter[left]){
      // path no longer present in current window
      if (--presence[path] == 0)
        sum--;
    }
  }
  
  std::cout << mini << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int c; std::cin >> c;
  while (c--) testcase();
  return 0;
}