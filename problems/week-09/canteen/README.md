tags: min cost max flow

## Intuition
This is a classical min cost max flow problem. Each day will be modelled as a node that has a supply which is equal to the number of menus that can be produced on that day (edge from source to daily node with capacity equal to num of menus) and a demand which is equal to the number of students visiting the canteen that day (edge from daily node to sink with capacity equal to num students). In addition a node can pass over a number of menues to the next day node if there are remaining menus.
Each edge will in addition have a cost. Note that we're interested in minimizing the losses. Thus as a first step one would think of modelling the cost as positive edge weights and the revenues as negative edge weights. This indeed also works however it prevents us from using the potentially faster `successive_shortest_paht_nonnegative_weights` algorithm. Using the `cycle_cancel` algorithm that works with negative edge costs we however only manage to pass the first 2 test sets while being way too slow for the third test case.

We thus need a way to find a way to somehow adapt the negative revenue costs. We first need to find out what the maximum revenue per menu is becuase this correspond to the edge with the smallest negativ weight. Let's call this `max_price`. If we now add `max_price` too each edge from the daily node to the sink, we will still obtain the same value for the max flow however we now have `max_price * flow` additonal costs because there are `flow` menus being sold and each has become `max_price` more expensive, so we need to subtract it again from the minimal cost found by the algorithm.