tags: Delaunay Triangulation, EMST, growing disks

## Intuition
This problems can be abstracted to the growing disk problem and we're interested in two special values.
- The maximum number of bones `a` covered in a single component, when growing disks up to a radius of size `r`.
- The radius `b` that we need to let disk grow to until we have at least one component with a at least `k` bones in it.
Each of the above subproblems requires that we have a way to efficiently look up the number of bones in a given component. For both problems we will grow the EMST to a certain size.

In order to reduce the number of edges that we will consider for out EMST, we will first build a Delaunay triangulation of all tree points. Note that we don't include the set of bones in to it yet.

We will now iterate over each bone and add an edge to its nearest tree with the distance squared and multiplied by 4. This is necessary to have a uniform comparison at the end. When comparing edges between 2 trees, each tree created a shadow of distance `r`. Two trees thus have overlapping shadows if their distance is `<= 2r` or equivalently their squared distance is `<= 4r^2`. For an edge between a bone and a tree however only the tree creates a shadow of radius `r` and the bone will be in that shadow if the distance `d <= r` or equivialently `d^2 <= r^2`. Since we will later on compare the distance of edges no matter if tree->tree or bone->tree against the value `s = 4r^2` we will scale the distance of bone to tree edges by `4` such that we have a unifrom comparison.

We will also define an array `num_bones` of length `n+m` that contains the number of bones in each component whose id is `0 <= id < n+m`. Note that initializing this means, to set the entry of each bone component to 1.

We will now grow the EMST including all edges that have a squared distance `<= s = 4r^2`. Each time we merge two components `c1` and `c2` into a new component `c_new` we will also merge the number of bones in it so we have to set `num_bones[c_new] = num_bones[c1] + num_bones[c2]`. We thus have a way to look up the number of bones per component in constant time. While growing the EMST we will also check upon merging two components if have more than `k` bones in the new component. The frist time this is satisfied we can calculate our value `b`.

To find value `a` we will grow the EMST using all edges that have a squared dist `<= s` and will set a to the highest number of bones we had in each merging step until then. Note that we might have to continue to grow the EMST in order to find `b`. This is the purpose of the second for loop.

