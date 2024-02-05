tags: DP

## Intuition
If we simulate every possible placement ordering we have 2^n different placements possible. We can however drastically reduce and bound the number of possible states that we can be in.
Note that `m` is max 3, thus the max score of a round can be `3*1000`. Since we're not allowed to have negative round scores, it must hold that `num_disctinct * 1000 - 2^(abs(p-q)) > 0`. Thus in the extreme case we have that `3000 - 2^(abs(p-q)) > 0`, which limits the difference in the size of both queues to at most 12. Thus we have that -12 <= p-q <= 12 which is a total of 25 possible values.
In addition we always only care at most about the last 2 fighters at each entrance. There are 4 different types, thus there can be 16 different states of the queue at each entrance. We will introduce a dummy fighter type to ease calculations which will lead to 25 possible states a queue can be in.

We will define a DP table of dimensions `n x 25 x 25 x 25`, where `dp[i][s][n][diff]` holds the best score when we're at fighter i, the state of the south entrance being `s`, the state of the north entrance being `n` and the difference between both queues being `diff`. We will start with the first fighter and recursively fill out the table by saving the best possible outcome at a given state in the DP table such that it can be re-used in case we will encounter the same state again.