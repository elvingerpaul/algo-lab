## Intuition

This problem can be solved by splitting it into 2 parts, one part that will be solved using a sliding window and the second part using a dynamic programming approach.

Part 1: We're interested in calculating all the pairs (left,right) such that sum(\[v_left:v_right\]) = k, where v_left and v_right are both included. We can solve this using a sliding window and maintaining a sum that holds the value of the current window. If the sum is equal to k, we've found a sequence that starts in v_left and ends in v_right. We keep an array seq_sum, where seq_sum\[i\] holds the starting point j of a sequence that starts in i and ends in j where the sum is equal to k, otherwise seq_sum\[j\] = -1. If the sum > k, we will shrink the window size by increasing the left pointer. If sum < k, we will increase the window size by increasing the right pointer.

Part 2: We know know all possible sequences where the sum is equal to k and have them stored in the seq_sum array. We now need to find the maximum number of horcruxes that can be distroyed with exactly m agents. This is equivalent to finding exactly m sequences that are disjoint (i.e don't overlap) and whose sum is maximal. We'll define it as a DP problem using a 2-dim matrix, where dp\[i\]\[j\] = the maximum number of horcruxes that can be destroyed with exactly i agents, when we're given the first j horcruxes. We can then fill out the table according to the follwoing rule 

if a sequence s is ending in j with length l, i.e seq_sum\[j\] != -1 and there exists a sequence s' with i-1 agents that is ending before s is starting:

dp\[i\]\[j\] = max(dp\[i\]\[j-1\], dp\[i-1\]\[j - l - 1\])

The above is either we keep the current best for j agents or we can use the ending sequence s to try to improve it by using the maximum number of horcruxes that can be destroyed with i-1 agents and j - l - 1 horcruxes.
Otherwise if we don't have a new sequence s ending in j, we simply keep the current best for i agents, i.e
dp\[i\]\[j\] = dp\[i\]\[j-1\]

Note we add a dummy column in the very front of the table and initialize j = 1 such that the access dp\[i\]\[j-1\] is always well defined.