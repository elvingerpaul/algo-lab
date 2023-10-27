#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Min_circle_2_traits_2<EK> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef EK::Point_2 EP;

double ceil_to_double(const EK::FT& x){
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n){
  std::vector<EP> points(n);
  for (int i = 0; i < n; i++){
    long x; std::cin >> x;
    long y; std::cin >> y;
    points[i] = EP(x,y);
  }
  
  Min_circle mc(points.begin(), points.end(), true);
  Traits::Circle c = mc.circle();
  std::cout << long(ceil_to_double(CGAL::sqrt(c.squared_radius()))) << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n){
    testcase(n);
    std::cin >> n;
  }
  return 0;
}