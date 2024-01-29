tags: DP

## Intuition
The key thing about this problem is to realize and convice yourself that there is a best solution that involves always taking one package from one stack while a subset of packages from the other set.
Process the stack bottom up using a DP table. `DP[i][j]` contains the minimum score when considering stack elements a1 to ai and b1 to bj.