tags: Linear Programing

## Intuition
We need to create a nutrition plan such that for each nutrient we fullfill both a minimal and maximal quantity. Since we're given both the unit price `p_j` per food as well as the amount per unit `a_ij` of nutrient `i` contained in food `j`, it seems natural to solve this problem by choosing the number of units `u_j` for each food as our unknowns. We need to fulfill the follwoing constraints:
- For each nutrient `i` we need to meat the minimal quantitiy
    `sum(over all foods j) u_j * a_ij >= min_i` which is equivalent to
    `-sum(over all foods j) u_j * a_ij <= -min_i`
- For each nutrient `i` we must not exceed the maximal quantity
    `sum(over all foods j) u_j * a_ij <= max_i`

We want to minimize the cost which fulfills the above conditions, which can be expressed as minimizing
    `sum(over all food j) u_j * p_j`