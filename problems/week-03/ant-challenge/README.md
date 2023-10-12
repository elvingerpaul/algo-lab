## Intuition

We need to divide the problem, into two subproblems. The first subproblem consists of extracting the edges that are part of each species' private network. The second problem consists of finding the shortest paths from a to b given each species' private network.

Problem 1 is equivalent to solving the minimu spanning tree for each species. The hives actually don't matter, all we're interested is to get the edges that form the MST. For each species we add the edges to a so called shared graph. Note that this shared graph is a multigraph as their can be mulitple edges between two vertices, as they can originate from different private networks.

Problem 2 consists fo finding the shortest path between a and b in the shared graph which is simply running the Dijkstra algorithm on the multigraph.

Complexity:
- Subproblem 1: O(s * e * log(e))
- Subproblem 2: O(n * log(n) + s * e)