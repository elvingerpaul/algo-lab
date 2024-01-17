tags: Delaunay Triagulation, Voronoi diagram, priority queues (Dijkstra)

Disclaimer, inspiration from [here](https://github.com/simon-hrabec/Algolab-2020/tree/main/problems/Week%2008%20-%20H1N1)

## Intuition
We build a Delaunay triangulation out of all the sick people. Our goal is to leverage the closely related Voronoi Diagram, because if we move along the edges of the Voronoi diagram, we'll be staying as far aways as possible from the vertexes of the traingulation which are the sick people. The question thus is if there is a path using only the Voronoi diagram edges in order to get out of the convex hull.

We can consider the vertexes in the Voronoi diagram as representing the faces of the Delaunay triangulation and the edges in the diagram as a way of moving from one face to a neighboring face. Each edge has a "weight" which will be equal to the distance of the two vertices (two sick people) split by the Voronoi edge and essentially refers to the maximum amount that we are guaranteed to be distant from any two of them.

If the "weights" of all the Voronoi edges are known we can preprocess the whole diagram in order to get the guaranteed maximum distance (maximum minimum distance) that we can keep to any sick person while escaping the convex hull. In order to so we apply a Dijkstra approach. Note that Dijkstra provides us the minimal distance from a given start node to all reachable nodes in the graph. Our start node in this case is a dummy point that we assigne the idea 0 that is outside the convex hull and represents all the infinite faces of the Delaunay triangulation. The goal is now to find the maximum minimum distance to each face of the triangulation which we can obtain by slighlty updating the Dijkstra criteria to update the current best max min distance found in each round.

Once the preprocessing is done, we only have to locate a person and use the information from the preprocessing to get the maximum minimum distance guaranteed when trying to escape the convex hull from the location of the escapor.

Note the preprocessing is necessary to pass testcase 4 and 5. Doing a BFS/DFS for each escapor is too slow for test cases 4 and 5.
