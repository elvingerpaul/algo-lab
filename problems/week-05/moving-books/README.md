tags: greedy

## Intuition
We follow the following greedy approach. We sort the strength of friends in increasing order and use a multiset for the weights of the boxes. Note that a multiset internally maintains a sorted order, i.e insertion, lookups and deletion can be done in O(log n).
We will repeat the following process until the multiset is empty:
- Loop over the friends in increasing order and assign each friend the heaviest remaining package that it can carry, then remove it from the multiset.
- If a friend can no longer carry any package it will no longer be considered in future iterations, which is the reason for increasing the variable `start` that denotes the beginning of each loop.

Note that the function `upper_bound` returns the fist element strictly greater than a given value which is why we always need to consider the value before the returned iterator.