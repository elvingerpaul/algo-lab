tags: Linear Programing, Binary Search

## Intuition
The core of this problem is to solve a simple linear programing problem. The constraints of the model however depends on how many days we decide to work on trench `A` or trench `P`. The goal is to find the minimum days required to work on both in them together.
Trying out all combinations and checking whether the LP is solvable is too slow, we need a more efficient way of finding the best combination of days.
The trick is to recognize that the number of filth that can be removed it a monotone increasing function. Thus the higher we choose the values `a` and `p` the higher the amount of filth removed will be. We can thus consider the amount of days to work on `A` and the amount of days to work on `P` to be represented in a 25x25 matrix and apply some sort of binary search on it.

We first look a minimum index `i` such that `(i,i)` working `i` days on `A` and `i` days on `P` makes the problem solvable, if not we output `Impossible!`. This can be done in a binary search maner. If found this means that the max number of days of work is at most `2i`. There is no need to check any pairs `(k,l)` such that `k + l >= 2i`. If `(i,i) = (0,0)` we also have found our solution already.

If `(1,1) <= (i,i) <= (24,24)` it means that the problem is not solvable for `(i-1, i-1)` otherwise our binary search would have returned that pair. Thus it is also not worth checking the pairs `(k,i-1) or (i-1,k)` for `k < i`. Essentially our solution is either `(i,i)` or it lies in the matrix `[i:, 0:i-1]` or in the matrix `[0:i-1, i:]` where we could potentially reduce the number of total days worked.
We will do a sequential search of them in the following manner. We show it for the matrix `[i:, 0:i-1]`, but the idea is the same for the other matrix:
- start at `(i, i-1)` -> note that we're now looking if its possible to have a solution with 1 days less of work
- check current entry `(k, l)`:
    - if LP not solvable move to `(k+1,l-1)` -> we move to the next potential entry while keeping the amount of hours worked in total the same
    - if LP solvable move to `(k, l-1)` -> it is possible to work less days while still solving the LP problem -> try if we can further reduce the days. In any case we have found a new uper bound on the max total days required
- repeat above step until one of the indexes rounds out of bounds.