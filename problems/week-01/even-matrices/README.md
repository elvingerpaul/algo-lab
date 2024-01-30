## Intuition

Same prodecure as even pairs except over 2 dimension.

Preprocess the matrix such that m[i][j] holds the sum of all elements m[:i][:j] included. Then we know that a matrix (i1,i2,j1,j2) is even if m[i2][j2] + m[i1-1][j1-1] - m[i2][j1-1] - m[i1-1][j2] is even.

Or similarly expressed the matrix (i1,i2,j1,j2) is even if from the 4 quantities m[i2][j2], m[i1-1][j1-1], m[i2][j1-1] and m[i1-1][j2] there are either 4 even values, 2 even and 2 odd values or 4 odd values. When the preprocessing is done we can calculate the number of such quadruples for a fixed i1 and i2 by iterating over all columns j and calculating the number of even and odd pairs of m[i1][j] - m[i2][j1]. Once we know these we can repeat the even pairs problem.