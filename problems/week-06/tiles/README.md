tags: maxflow, max cardinality matching

## Intuition
This problem can both be solved using a maxflow or maximum cardinality matching

#### Maxflow
Consider the garden as a 2-dim grid, where each coordinate is a node in the graph. Divide the garden into a bi-partite graph where direct neighbors in the grid must be in different partitions. Add an edge between each pair of neighbors in the grid with a capacity of 1 if both are `.`. Add a source vertex that contains an edge to each node in partition A with capacity 1. Add a sink vertex and an edge from each node in partition B to the sink with capacity 1. If the maxflow of the graph is equal to half the number of `.`-nodes in the graph then the problem is solvable.

#### Maximum cardinality matching
Consider the garden as a 2-dim grid and add an edge between two nodes in the graph given both are `.`. A matching corrsponds to the tiles in the garden. If the size of the matching is equal to half the number of `.`-nodes in the graph then the problem is solvable.