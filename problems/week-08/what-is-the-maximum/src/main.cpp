#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int p){
  int a, b; std::cin >> a >> b;
  
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  
  Program lp;
  
  if (p == 1){
    lp = Program(CGAL::SMALLER, true, 0, false, 0);
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
    lp.set_c(X, a); lp.set_c(Y, -b);
  } else {
    lp = Program(CGAL::LARGER, false, 0, true, 0);
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
    lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_unbounded())
    std::cout << "unbounded\n";
  else if (s.is_infeasible())
    std::cout << "no\n";
  else if (p == 1)
    std::cout << std::floor(CGAL::to_double(s.objective_value()) * (-1)) << std::endl;
  else
    std::cout << std::ceil(CGAL::to_double(s.objective_value())) << std::endl;
}


int main(){
  int p; std::cin >> p;
  while(p){
    testcase(p);
    std::cin >> p;
  }
  return 0;
}