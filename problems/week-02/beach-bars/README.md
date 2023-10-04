## Intuition

We solve this problem using a sliding window using a left and right pointer. Whenever the left and right pointer are less than 200m away from each other we can increase the window size by increasing the right pointer otherwise we decrese the window size by incrementing the left pointer.

If the size of the window is not dividable by 2, there are 2 potential places that can be optimal which we need to take into account.

Integer division in C++ always rounds toward 0, we need to be careful when our integer division result is negative as we're no longer rounding down as we would to with a positive result but we're rounding towards 0, i.e we're rounding up when the result is negative. We therefore need to explicitly floor the division result to make sure that we get consisten behaviour when calculating the window middle of the window size.