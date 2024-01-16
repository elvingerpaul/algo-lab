tags: maxflow with vertex capacity

## Intuition
This is a normal maxflow problem with the additional challenge that we have a capacity on the vertex. In order to simulate this we can simply split a vertex into two. The first "half" only has inflow while the second "half" only has outflow. We can add an edge between the first and second half of each vertex with the corresponding capacity `C`. That way we ensure that no more than `C` can flow through the node.