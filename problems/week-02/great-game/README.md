## Intuition

For each node we maintain 2 arrays v_min and v_max. v_min\[i\] holds the minimum number of moves a player can guarantee for himself to get from i to n-1, v_max holds the maximum number of moves a player can guarantee for his opponent to get from i to n-1. We can solve the problem both recursively and iteratively. To find the minimum value, the player can choose the best move to the next node from which he needs to assume that his adversary will try to maximise the remaining numnber of moves. Similarly to find the max value of a node, a player will choose the successor node that will maximize the number of moves taking into accout that the opponent will minimize the number of moves from the successor node. Thus we have that for a given node i, we iterate over all successor nodes j such that
min\[i\] = min(min\[i\], max\[j\]+1)
max\[i\] = max(max\[i\], min\[j\]+1)

### Recursive solution
We can solve the problem recursively by running a depth first search. The DFS will ensure that for each node we have the information of all successor nodes available. In order to ensure that we start the DFS from the right node to ensure we have all the correct information, we will add a dummy node to the graph which holds an edge to the red and blue starting position. We will start the DFS from this node.

### Iterative solution
We can solve the problem iteratively by observing that a node i can only have successnor nodes j such that j > i. In this way we can simply loop over the min and max arrays backwards and fill in the information successively and because of the observartion we did we can be sure that all successor node information is available.