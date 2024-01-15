tags: subset sums, split and lists, binary search

### Intuition
This is a version of the knapsack problem which is known to be NP-complete. Note that n is small and only for the last set of tests we have that n > 20 (also hinting to use split and list for the last test case).

#### First group of testcases
For each testcase, we calculate the sums both in distance and time for all subsets. For each subset, check if we can cover distance D in time strictly less than T. If yes we have a solution in which Panoramix will not be captured. As there are n moves, this will result in runtime O(2^n + m), since we need to calculate the subset sums of all subgroups.

#### Second and thrid group of testcases
The potions can be read in in sorted order. We will search for the minimum number of gulps using binary search. For a given step in the binary search add the number of additional steps to each move that can be achieved when drinking the given number of gulps and recalculate the subset sums. If it is possible to cover distance at least D in time strictly less than T, then Panoramix will not be captured however there might be a solution with even less gulps. For that reason we will adjust the `right` variable in the binary search to `middle-1` and will repeat the above step with first restoring the initial moves. If we cannot cover the distance in time strictly less than T, we need to potentially drink more gulps to cover a larger distance. In that case we increase the variable `left` to `middle+1`. The runtime will be O(log(m) * n * 2^n) as in log(m) steps we will have found the smallest number of gulps or we found that Panoramix will be captured.

#### Fourth testcase
The runtime O(log(m) * 2^n) is to slow for cases where n > 20. We will use split and lists in order to decrease the runtime. Split the moves into 2 sets of roughly size n/2. Calculate the subset sums in each of the two groups and order the second group by increasing time. Note that we add the gulps in the same maner as above and look for the minimum number using binary search. We will iterate over each pair (t1, d1) in the first group and look for a pair (t2, d2) in the second group such that t1 + t2 < T and are interested in the largest distance d1 + d2 we can cover in it. The binary search idea from above is identical.
In order to ensure that we find the largest distance d2 in the second group that can be covered in time t2, we will preprocess the second group as follows. Assume (t1, d1) and (t2, d2) with t1 < t2 but d1 > d2 then modify (t2, d2) to (t2, d1) as in time t2 we can still run the longer distance d2. That way when looking for an uppercound on t2, we will always make sure that we get the largest distance d2 along that we can conver in it.
The runtime is now O(log(m) * n * (2^(n/2))).