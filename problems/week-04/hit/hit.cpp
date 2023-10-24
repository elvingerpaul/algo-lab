#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void testcase(int n){
  long x; std::cin >> x;
  long y; std::cin >> y;
  long a; std::cin >> a;
  long b; std::cin >> b;
  K::Point_2 r1(x, y), r2(a, b);
  K::Ray_2 ray(r1, r2);
  
  bool intersect = false;
  for (int i = 0; i < n; i++){
    long r; std::cin >> r;
    long s; std::cin >> s;
    long t; std::cin >> t;
    long u; std::cin >> u;
    if (!intersect){
      K::Point_2 s1(r, s), s2(t, u);
      K::Segment_2 seg(s1, s2);
      
      if (CGAL::do_intersect(ray, seg)){
        intersect = true;  
      }
    }
  }
  
  std::cout << (intersect ? "yes" : "no") << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false); // decisive for test set 4!
  int n; std::cin >> n;
  
  while (n != 0){
    testcase(n);
    std::cin >> n;
  }
  return 0;
}