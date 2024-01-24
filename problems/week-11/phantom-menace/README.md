tags: Max Flow, Min Cut

## Intuition
We are looking for the minimum number of vertexes to remove in order to completely separate all potential sources from all potential destinations. This is essentially a mincut problem but we need to transform the vertexes into edges somehow. This can easily be done by replacing vertex `v` with `v_in --1--> v_out`. Have an edge from the source to each potential starting position with cap 1 and an edge from each potential destination to the sink with cap 1. An edge `(u,v)` will be represented as `u_out --1--> v_in`.

The max flow from the source to sink will correspond to the minimum cut, aka the min number of vertexes to remove in order to separate sources from destinations.