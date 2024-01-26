tags: Linear Programing, Delaunay Triangulation

## Intuition
This problem is to be solved with Linear Programming. The unknowns of the problem will be `a_{w,s}`, i.e the amount of beer in liters delivered from warehouse `w` to stadium `s`. This resulst in a total of `num_warehouses * num_stadiums` constraints. We add the following constraints
- for each warehouse `w` ensure the delivered amount doesn't exceed the supply: `sum (over all stadiums s) a_{w,s} <= supply` -> will result in `num_warehouses` constraint
- for each stadium `s` ensure that we deliver the exact: `demand <= sum (over all warehouses w) a_{w,s} <= demand` -> will result in `2 * num_stadiums` constraints
- for each stadium `s` ensure that we don't exceed the alcool limit: `sum (over all warehouses w) a_{w,s} * alcool_w <= 100 * limit_s` -> will result in `num_stadium` constraints. `alcool_w` is the amount of alcool in the beer delivered by warehouse `w` in %. Note that the bound is multiplied by 100. We do this so that we can keep all our coefficient as integers.

This results in a total of `num_stadiums * num_warehouses = 20 * 200 = 4000` unknowns in the worst case and `num_warehouses + 2*num_stadiums + num_stadiums = 200 + 40 + 20` constraints in the worst case which is tractable for our LP solver.

We want to optimize the total profit, so under the above constraints we want to optimize `sum (over all a_{w,s}) a_{w,s} * (r_{w,s} - (t_{w,s}/100))`, which is equivalent to minimizing its negative counter part. Again we can scale the whole thing by 100 to ensure we only have integer coefficients. We only need to divide the outcome by 100 again in that case.

## Efficiently calculating the number of intersections with contour lines
In order to efficiently calculate `t_{w,s}` it is worth taking a precise look at the test cases (which I didn't...) and notice that for all testcases it is guaranteed that the total number of contour lines that will intersect with any segment is at most 100. So even for the last test case only for 100 contour lines will there be intersections even though in total there can be up to 1000000 contour lines. Here we use the Delaunay triangulation over the union of all warehouses and stadiums. For each contour line we can then calculate its closest vertex and check if it is within intersection distance. Only for those that do we will check which segments `(w,s)` are impacted and will update its revenue.