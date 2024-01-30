tags: greedy, DP

## Intuition
A simple greedy approach will not help us apart for test cases 1 as the potions of type A have no greedy order. The key obeservation to make for this challenge is that both `H` and `n` are relatively small and thus we could try to leverage a DP approach to solve the problem.

### Test cases 1 & 2 (inspiration from [here](https://github.com/haeggee/algolab/tree/main/problems/week05-severus_snape))
The idea of DP is that in order to find the minimum number of potions of type A to consider, we can try to solve the following subproblem. `What is the maximum power one can achieve with k potions and achieving H happiness?` So we are trying to find the maximum power one can achieve parametrized by happiness and the number of potions of type A. Consider a three dimensional table where an entry `dp[i][j][h]` holds the maximum power one can achieve by choosing `j` potions out of the first `i` potions and obtaining at least happiness `h`.

- Initialization: set `dp[i][1][h] = max(dp[i-1][1][h], potion_a[i].p)` if `potion_a[i].h >= h` else ignore value of current potiton and set to the value obtained by consdering only i-1 first potions `dp[i-1][1][h]`. Everything else initialized to `-inf`.
- Filling out table: set `dp[i][j][h] = max(dp[i-1][j][h], dp[i-1][j-1][offset_h] + potion_a[i].p)` with `offset_h = max(0, h - potion[i].h)`. Meaning we either choose the value from using `j` potions out of the first `i-1` potions that achieve happiness `h` or we consume potion `i`. In that case we only need `j-1` potitions out of the first `i-1` potions that achieve happiness `h  - potion[i].h`. 

Once the table is filled out we are looking for the smallest idx `j` in the second axis such that the value of `dp[n][j][H] >= P`. It refers to minimum number of potions `j` that we take out of all potions that manage to achieve at least power `P`.

### Test cases 3 & 4
Once the DP table is filled out we will iteratively and greedly sum up the potions of type B in decreasing wit order. Every time our accumulation is above W, we look for a matching `j` in the second axis of the table such that we satisfy the power constraint taking into account the penalty we have to pay based on the number of potions of type B for accumulation and verify that the accumulated sum still satisfies the `> W` criteria with the found index `j` and the incurred penalty, otherwise we add another potion of type B to the accumulated sum and repeat the verification process until. This continues until we either find an index `j` where all conditions still hold or we conclude it is not possible to satisfy the criteria once there are no more potions of type B to consume. In that case we output `-1`.