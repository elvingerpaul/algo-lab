tags: Linear Programming

## Intuition
Mines are organized as a tree where the root is the entry to the mines. Without dangerous mines one could simply sum all the resources from the mines, however dangerous mines cause bottlenecks and become our unknowns (numer of dangerous mines is low). Each dnagerous mine will have an unknown for each mineral -> num_dangerous_mines * num_minerals unkowns.
Variable `x_{i,j}` denotes the amount of mineral `j` passed from dangerous mine `i` towards the entry. The following constraints need to be met:
- `x_{i,j}` must be <= than the sum of all quantities of mineral `j` obtained from the children of the mine in the tree.
- the sum of `x_{i,j}` over all minerals `j` must be smaller than half of the mines dangerous threshold `d`

Traverse tree in DFS manner to get the amounts of minerals as well as the unknowns from all child subtrees.

In addition we add unknowns `y_j` which denotes the amount of mineral `j` bought in the shop. At the entry node we need to satisfy the demand which we can set as a constraint once we know the minerals from subtrees and all the unkowns plus taking the potential buy-ins from the shop into account (which of course must not exceed the shop's supply).

We will minimize the sum of `y_j * p_j` under all the above constraints, where `p_j` is the unit price of mineral `j` at the shop.
