tags: Tree, DFS, Sliding Window

## Intuition
Simulate a sliding window of size `m` over the tree. We use a multiset to represent the sliding window which by default stores elements in increasing order. Thus to check the maximum difference between max and min of the curretn window, we can simply retrieve the first and last element of the multiset and calculate the difference.

In addition we keep an array `idx` that tells us the start and end point of the current window. If we move forward we know the index of the element that constitutes the start of the window, we can thus remove the element from the multiset and store the next element as the new end element of the window.

We will process the tree in a DFS manner because we are interested in paths. After having processed a subtree and moving to process the next subtree (pass to the next child) we need to make sure that the sliding window is restores to the state before we processed the first subtree. The same as to when we have processed all subtrees and move back up in the tree. The parent node is always responsible for adding the child to the sliding window as well as remove it again once its subtree has been processed.