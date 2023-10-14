## Intuition

Tags: Sliding window, prefix sum, binary seach, sorting

As we cannot visit an islands more than once and given the network topology of waterways that all meet in 0 (Pyke), one can extract that this problem is about finding paths of islands where the number of men needed to conquer each island is exactly equalt to the number of men k at our disposal. Two situations can occur, either the path is within a single waterway or the path is across two waterways and must pass through Pyke (0). There cannot be more than two waterways involved as we're not allowed to visit an islands more than once.

Solutions tried out:
- run johnson all pair shortest paths: not enough memory to allocate 2-dimensional matrix (nxn) where n is the number of islands to store distance between each pair of nodes
- concatenate each pair of waterways and run sliding window on each of them. Requires to reverse one array and remove origin from it (to avoid having pyke in the concatenation twice). Too slow for testcases 3 and 4
- calculate prefix sums for each node. For each node search for remainder in other waterway if current prefix too small, search for islands within same waterway if current prefix too large. Need to consider n^2 pairs of waterways -> too slow for testcases 3 and 4
- Final solution: calculate prefix sums for each island. Let (pref, dist, waterway) be a triplet for each islands that contains its prefix sum, its distance from pyke (0) and the waterway it is path of. Group triplets of all islands in common vector and sort it 1. after prefix and 2. after distance from 0 increasing. We're always interested in forming the longest path, with a different waterway. Thus when looking for remainder in differnt waterway, always look for the one furhtest away from 0. Do a sliding window for all paths within the same waterway.