tags: Maximum Independent Set, Max bi-partite matching

## Intuition
Frist note that the chessboard can be modeled as a bipartite graph where a node `n` with coordinates `(i,j)` is in set A if `i+j % 2 == 0` otherwise it is in set B. Next note that conflicting knight positions are always in different parititions.

Thus the maximum number of knights on a chessboard such that they don't threaten each other can be abstracted into finding a Maximum Independent Set of the graph, where there are edges between threatening knight positions. Even though this problem is in general NP-complete, we can try to transform the problem first into a Maximum Bipartite Matching problem and then apply Koenig's theorem which states that `In a bipartite graph, the number of edges in a maximum matching is equal to the number of vertices in a minimum vertex cover.` Thus once we have the matching size and the number of holes in the board we can easily calculate the maximum number of knights on the board.