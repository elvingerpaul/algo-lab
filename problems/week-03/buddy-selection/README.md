## Intuition

We transform the problem into a maximum cardinality matching problem. Each person is a node in the graph and we add an edge between two persons if and only if they share more than f interests. In that case it is sufficient to check that the maximum cardinality matching has size n/2.

Note to check that two persons have more than f common interests:
- set_intersection function is too slow with normal sets, got incorrect response with unordered_sets
- manually counting common element by checking presence of elements of s1 in s2 is also too slow. Normal set have complexity of O(log n) for basic operations, element are stored in tree like structure.
- need to use unordered sets and manual checking of presence. Unordered sets are implemented using hash tables underneath and have a complexity of O(1) for basic operations.