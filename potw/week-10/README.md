tags: Linear Programing

## Intuition
Let's start by identifying the unknowns that we have:
- the sewage canal `c_s`
- the water canal `c_w`
- the minimal maximum length `l` of all water pipes

#### Sewage and Water canal need to cross at a 90 degree angle
The equation of a line in a 2-dim plane is given by `ax + by + c = 0` for some coefficients `a`, `b` and `c`. We can thus define the sewage canal `c_s` to be defined by the equation `ax + by + c1 = 0`.
Any line perpendicular to `c_s` is of the form `-bx + ay + c2 = 0`. Thus by having parameters `a` and `b` in both equations and expressing the water canal `c_w` as `-bx + ay + c2 = 0` allows us to ensure that the two lines are always perpendicular to each other. Thus we're interested in finding the coefficients `a`, `b`, `c1` and `c2`.

#### Cersei's constraint: All noble houses to the left and all common houses to the right of `c_s`
For each noble house `(x_n, y_n)` it needs to hold that `ax_n + by_n + c1 <= 0` and for each common house `(x_c, y_c)` it needs to hold that `ax_c + by_c + c1 >= 0`. Note that points are allowed to be on the line. Choosing all coefficients `a`, `b`, `c1` and `c2` equal to 0 will satisfy both constraints above, but will not result in `c_s` and `c_w` to be lines. We will thus normalize by `a`, which essentially removes `a` as an unknown. So we'll continue to refer to our canals as follows:
- `c_s`: x + by + c1 = 0
- `c_w`: -bx + y + c2 = 0
Thus for each noble house `(x_n, y_n)` we will add a constraint
- `x_n + by_n + c1 <= 0`
And for each common house `(x_c, y_c)` we will add a constraint
- `-bx_c + y_c + c1 >= 0` -> `bx_c - y_c -c1 <= 0`

#### Tywin's constraint: Sum of sewage pipe lengths must not exceed `s`
Important to notice here (which initially I did not) is that the sewage pipes must be horizontal. Using the formula to calculate the shortest distance from a point to a line `|ax + by + c|/sqrt(a*a + b*b)` is thus not correct and would also make the problem non-linear. The length of each sewage pipe is simply the difference in the `x` coordinates of a noble house `(x_n, y_n)` / common house `(x_c, y_c)` and the sewage canal. Since noble houses are to the left of the canal and common houses to the right, we can calculate the distance to `c_s` for each of them as follows:
- noble house: dist = x_c_s - x_n = -by_n - c1 - x_n
- common house: dist = x_c - x_c_s = x_c + by_c + c1

We thus need to add the constraint that the sum over all distances should not exceed `s`
- `b * (sum(y_c) - sum(y_n)) + c1 * (num_common_houses - num_noble_houses) + sum(x_c) - sum(x_n) <= s`

#### Jaime's desire: minimize the maximum lenght over all water pipes
Again as above, note that water pipes must be vertical so their length is simply the difference in y coordintes between a house `(x_h, y_h)` and the water canal. This length should be `<= l`. Let  `l` be another unknown variable to out problem. Since we don't if `y_h` is above the water canal or below we will have to provide both a lower and upper bound to the difference.
- -l <= y_h - y_c_w <= l -> `-l <= y_h - bx_h + c2 <= l`

If we now set a lower bound on `l >= 0` and minimize for `l` under all the constraints above we will obtain our solution.