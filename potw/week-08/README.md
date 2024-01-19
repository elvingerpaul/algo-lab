tags: maxflow, minimal flow constraints

## Intuition
We have `l` locations that each have each a certain number of soldiers `g_l` stationed in it and require a minimum number of soldiers `d_l` to be present for a potential war. At the same time we have paths between these locations that have constraints on the number of soldiers that need to flow and that are maximum allowed to flow through. It does seem natural to model this problem as a maxflow problem.

We consider each location `l` to be a node in the graph that is supplied a certain number of soldiers `g_l` and that demands at least `d_l` soldiers to be present for a potential war. This can easily be modeled by introducing a source node with an edge to each location with `g_l` as capcity to model the supply. In addition we introduce a sink node and an edge from each location `l` to the sink with capacity `d_l` to model the demand.

Intuitively if we have a maximum flow of `sum(over all locations l) d_l`, this means that each location has its demand satisfied and enough soldiers present to fight a potential war.

We now in addition need to take minimum and maximum capcities between the locations into account. Assume that on the path from location `f` to location `t` we have a minimum capacity of `min_c` and maximum capacity of `max_c`. How do we now model this in our graph?

This essentially translates to that the the supply `f` given by the source `min_c` needs to be send to `t` for sure. We can model this by decreasing the supply that is send from the source to `f` by `min_c`. At the same time we need to increase the supply from the source to `t` because in addition of the supply that `t` already gets from the source, it will also get the `min_c` soldiers from `f`. What is left to set the capacity of the edge from `f` to `t` to `max_c - min_c` because `min_c` soldiers already flow thorugh for sure and there can be at most `max_x - min_c` soldiers in addition without violating the max capacity constraint on the edge. This way we simulate that at least `min_c` soldiers need to flow from `f` to `t`. Note that we have not modified any capacities from the locations to the sink node, we still require them to be the same in order to check that each location still has a sufficient amount of soldiers present.

Careful the input datasets contain duplicated edes between the same pair of locations. We consolidate all of them into a single edge.