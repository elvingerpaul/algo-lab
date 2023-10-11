## Intuition

We define the following quantities in a DFS search tree:
- dfs: the discovery time of each vertex in the depth-first search. A forward edge in the DFS search tree is an edge in G that goes from a lower dfs value to a larger dfs value. We call a backward edge an edge in G that goes from a larger dfs value to a smaller dfs value.
- low: the low point of each vertex in the depth-first search, which is the smallest vertex reachable from a given vertex with at most one back edge.

An edge (u,v) is a bridge if low(v) > dfs(u). Since our graph is not directed and we don't know the starting node of the DFS, we need to check both (u,v) and (v,u) for the above property.

The BGL library offers a function that calculates the biconnected_components in the graph and in addition calculates the discovery_time and lowpoint of each vertex, which we can then use to identify all the edges.

Complexity:
- biconnected_components O(V+E)
- sorting the bridges array O(E*log(E))

overall complexity should be O(V + E*log(E))