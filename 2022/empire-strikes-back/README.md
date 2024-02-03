tags: Linear Programing, Delaunay

## Intuition
This problem contains two subproblems:
- avoid that any bounty is within the radius of any shot
- distribute the total available amount of energy among all shots
Note that we can choose the radius individually for each shot and we just need to make sure that it doesn't include any bounty. The goal should thus be to increase the radius as much as possible such that as many particles as possible will be impacted by its energy. The radius of the explosion is thus limited to it nearest bounty neighbor. This should ring a bell to use the Delaunay triangulation.

Thus we will calculate the Delaunay triangulation of all tjhe bounty points. For each shot we can then efficiently check it's nearest neighbor which effectively sets an upper bound on its maximum radius.

If we know for each particle the number of shots it is affected by, we can formulate it as a LP constraint. Note that the number of particles is small (at most 30). Even though the number of shots is large, we can solve this easily with LP given that we keep the number of constraints small. We will choose `e_i` the energy released by each shot to be our unknowns and will add one constraint for each particle `j` that ensures that the sum of all energies that affect this paritcle exceeds its density `d_j` and is thereby destroyed. Note that this requires that we calculate the distances between each shot and each particle, which we can do since we have at most 30 particles and at most 10'000 shots. We will discard all shots that are not within the radius as described above using the Deaunay triangulation of all bounties.

`sum (over all shots i such that j is in radius of i) e_i / max(1, dist_{i,j}^2) >= d_j`

We need to ensure that we don't exceed the total available amount of energy.

`sum (over all shots i) e_i <= e`