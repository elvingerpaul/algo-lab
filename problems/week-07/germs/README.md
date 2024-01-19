tags: Delaunay triangulation, EMST

## Intuition (my first solution, not ideal)
The idea we follow in this solution is to simulate the growth of the bacterias in each hour (it works though if the number of hours the bacterias live becomes very big, we might actually fail with this approach in case the simulation takes too long). A bacteria dies if it touches either a boundary or a neighboring bacteria, aka its nearest neighbor in a graph. We can thus consider this process as building an EMST where adding an edge means we have a collision and thus the bacterias die if they did not already.
We can thus calculate the Delaunay triangulation of all bacterias which will contain a linear number of edges. In addition for each bacteria we will add one edge to its nearest boundary. We then sort all these edges with increasing distance and then simulate the bacteria growth process. For each hour we consider how big the bacterias have grown and add edges to the EMST until we reach the size of the bacteria (note that we're operating on squared distances and that edges to the boundary are slightly different in the sense that only one vertex of the end is a bacteria that grows). Each time we add an edge we check if bacterias at either end are still alive or not and keep track of the first, median and last bacterias.

Note that this solution does not require to use square roots, but has a dangerous downside that the solution could become arbitrarily slow if we need to simulate a lot of hours.

## Note on the master solution
The master solution on the other hand doesn't simulate the growth process of the bacterias but simply holds the smallest distance of a bacteria to either its neighbor or its closest boundary. It also builds the delaunay triangulation and then iterates over the edges of it because we know the edge of closest neighboring bacteria needs to be part of it. Thus we iterate once only all edges and for each bacteria retain its closest distance to an obstacle (bacteria/boundary). The solution then sorts the distane in increasing order and we can then easily get the distance for the first bacteria to die, the median and the last one. All that is left to do is to transform the distance into hours and round accordingly. This requires to use square roots.