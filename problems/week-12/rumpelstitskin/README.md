tags: Max Flow Min Cost

## Intuition
We have two separate networks and we need to somehow ensure that for each unit of flow from network G into one of the islands we have one unit of flow from network H into the same islands. The trick to handle is is to simply reverse one network and have vertex capacities on the island nodes.

Network G becomes our source while network H becomes our sink. All straw from G to H has to flow through one of the islands. By setting the island capacities to 1 we can ensure that we don't spin more than one yarn of gold per islands. In addition we will apply the usual trick to avoid negative edge weights such that we can use the faster `successive_shortest_path_nonnegative_weights` algorithm.