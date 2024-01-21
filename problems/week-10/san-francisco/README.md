tags: DP, graphs

## Intuition
This is a simple DP problem that relies on an underlying graph but there is no need to acutally build the graph, it is sufficient to store it as an adjacency list. A few things one should pay attention to here:
- We have a multigraph as there can be several edges with different points between the same pair of nodes -> don't use an adjacency matrix since we cannot reduce all of them to a single edge.
- There are these so-called `Weayaya` that have no outgoing edges. Whenever a player is here they can "jump for free" to node `0` and continue from there. The jump to 0 doesn't count as a move. In order to model this in our graph we can simply add edges from all `Weyaya` nodes to all the successors of node `0`.

To solve the problem we define a 2-dim DP table `d` of dimensions `k x n`, where `d[k][n]` holds the maximum number of points one can score when starting from node `n` with `k` moves. Thus in order to check if it is possible to score at least `x` points with at most `k moves` starting from node `0` we simply need to iterate over all entries `d[:][0]` and pick the smallest value `k` that satisfies `dp[k][0] >= x`. If no such value `k` exists we can simply output `Impossible`.

What is left is to specify how we calculate the entries in the DP table.
- Initialization: `dp[0][:] = 0` since we cannot score any points with 0 moves no matter where we start from
- Update: Proceed row by row, `dp[k][i] = max_{over all successor nodes j of i}(dp[k-1][j] + points[i -> j])` where `points[i -> j]` is the number of points one obtains when going from i to j