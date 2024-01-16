tags: max flow

## Intuition
Create a graph that contains one node per game and one per player. Each game has an inflow of 1 (the number of points it can distribute). Depending on the value of `c`, we either know the winner, in that case we will have only one outflow edge to that player with a capacity of one. Or we don't know the winner in that case we'll have an outflow edge to both players with a capacity of one.
In addition we add one edge from each player to the sink with a capacity equal to the apparent score of that player. If the max flow is equal to the total number of games and equal to the sum of each player's apparent score than the result is plausible.