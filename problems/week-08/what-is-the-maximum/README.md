tags: Linear Programing

## Intuition
Very simple linear optimization problem. For problems of type `p=1`, we will instead of maximizing, minimize the negatif objectiv function which is equivalent. This however requires that we also multiply the outcome with -1 at the end. Use the `CGAL::to_double` function in order to cast from `CGAL::Quotient<ET>` to `double`.