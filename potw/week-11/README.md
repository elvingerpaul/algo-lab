tags: Min Cost Max Flow, Binary Search

## Intuition
We will model this as a Min Cost Max flow problem. We want to maximize the flow which is the number of suitcases transported by the elephants while not paying more than a given budget. We need to slightly adopt this because the original min cost max flow will aim for the maximum flow and only return the minimal cost for a given flow that is maximal.

Thus we do a binary search and control the number of suitcases potentially carried. If the source of our graph doesn't send more than `x` suitcases in, the flow can at most be `x`. For that flow we will be given the minimal cost. If the minimal cost is within our budget than it means we could potentially send even more suitcases throught the network so we will increase `x` in a binary search manner and recalculate the flow along with its minimal cost. On the contrary if the cost is not within our budget, then it means that the prices of the elephants are to expensive and thus we need to send fewer suitcases through.

Note that we also need to take into consideration that the network itself or the elephants can be the bottleneck. It could happen that we send `x` suitcases into the network but end up with a flow `f < x` for a cost that is within our budget. In that case we will not be able to increase our flow in any way and we actually already have the answer to the problem.