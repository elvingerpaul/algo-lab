Disclaimer: inspiration from [here](https://github.com/haeggee/algolab/tree/main/problems/week13-potw-clues)

tags: Delaunay triangulation, bipartite graph

## Intuition
Not so trivial as it might seem. Before diving into the problem I want to point out one crucial detail that for me was not so clear from the problem statement. As soon as there is some interference anywhere in the network we output `n` no matter if Watson and Holmes are impacted by this or not. This means it could be that there is interference in some partition `P1` of the network while Holmes and Watson are relying on a differnt partition `P2` that doesn't contain any interference. Once this detail is clear it slightly simplifies our lives. Our high level structure assuming all necessary details are known:
- If there is any interference anywhere in the network, output `n` for all clues for that testcase.
- Else If the two clues are within less than distance `r` from one another, output `y`. They don't rely on any intermediate stations.
- Else If both clues `a` and `b` are within distance `d_a <= r` and `d_b <= r` from two stations `s_a` and `s_b` respectively and if these two stations are part of the same network component, output `y`.
- Else output `n`.

### Checking for Interference
As the problem statement already mentions it we want to assign two send frequencies `0` and `1` to the nodes part of the network of stations such that no two nodes that are within distance `r` from one another have the same send frequency. This should remind us of coloring a graph with 2 colors or equivalently checking whether the graph is bipartite. The first thing to note here is that we should only consider edges between two stations that are within distance `r`. As the network of `1 <= n <= 9 * 10^4` nodes could be complete, thus ending up with `O(n^2)` edges, building the graph with all its edges and then running some BFS on it to check for bipartiteness is not an option.

In order to reduce the number of edges in the graph to `O(n)` we can apply the usual trick of first calculating the Delaunay triangulation and then only retain the edges of a length up to `r`. We can then check for bipariteness of the resulting graph. If the graph is not bipartite, this means that we have interference and the outcome of the problem is already known.

The key point however (that I did not get) is that bipartiteness of the graph is not sufficient to conclude there is no interference. It is very well possible for there to be interference if the graph is correctly colored. Thus in addition we need to check that between none of the stations assigned to partition `A` are within distance `r` and the same for the stations assigned to partition `B`. This can again be done by building the Delaunay triangulations for these separate graphs and checking that none of the resulting edges are longer than `r` otherwise we have interference.

If we indeed end up having a network with no interference, all that is left to check is to find the nearest station of each clue and check that it is not further away than distance `r` once again. We can easily do this using the original Delaunay triangulation. Both stations in addition need to be part of the same partition.
