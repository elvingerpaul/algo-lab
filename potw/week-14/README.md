tags: Shortest paths (Dijkstra), Maximum Cardinality Matching, Binary Search

## Intuition
- Caluclate shortest paths between any pairs of agents and shelters
- Using binary search look for last agent to seek shelter by builing a bipartite graph between all agents and shelters and adding edeges that have a distance of at most dist. Check that the cardinality of matching is equal to the number of agents otherwise increase dist or decrease dist if all agents can seek shelter and look for dist in binary search manner by repeating step 2.