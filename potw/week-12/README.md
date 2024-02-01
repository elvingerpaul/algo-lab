tags: DP

## Intuition
A first trap to not fall into for this project is to avoid trying to solve it using flows. It can actually be easily solved with DP once one has discovered the underlying structure.
We can sort of slightly modify the problem statement in that we are looking for two disjoint paths `a` and `b` (no common vertexes) that both start at node 0 and end at node n-1. Both paths however use a different set of edges. Path `a` which will be our increasing path can only use forward eges while path `b` which will corrrespond to our decreasing path is only allowed to use backward edges.
To simplify this we will create two graphs `forward` and `backward` that contain the set of forward edges and backward edges respecitvely however the edges are always directed from the node with the smaller index to the node with the bigger index, i.e `(u,v)` such that `u < v`.

### DP
Let us now define a DP table of dimensions `n x n` where `n` is the number of public places and let `dp[i][j]` correspond to the maxium number of rats that we can collect when starting our path `a` in `i` going all the way up to `n-1` and coming back to `i` again. The solution will be in `dp[0][0]`. Below we use `w(i -> j)` to be the weight/rats when traversing edge i->j. We will fill the table out in the following manner
- initialization: `dp[i][j] = -2` for `0 <= i, j < n`
- update:
    - if i == j == n-1: `dp[i][j] = 0`
    - if i == j != 0: `dp[i][j] = -1` -> not allowed to use the same vertex twice, there is no valid path up to n-1
    - if i < j: `dp[i][j] = max (dp[i][j], dp[k][j] + w(i -> k)) over all successors k of i if dp[k][j] != -1` -> move our forward pointer closer to n-1. Note that if `dp[k][j] = -1`, this means that there is no valid path up to n-1 in this configuration.
    - else: `dp[i][j] = max (dp[i][j], dp[i][k] + w(j -> k)) over all successors k of j if dp[i][k] != -1` -> move our backward pointer closer to n-1

We will start by solving `d[0][0]` and we will then recursively increase the forward and backward pointer step by step towards n-1 and solve the problem for the successors first. Note that we're guaranteed that there always exist vertex disjoint increasing and decreasing paths from 0 to n-1 and back.