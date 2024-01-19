tags: Min Cost Max Flow

## Intuition
This is a straight min cost max flow problem. Because of the size of the test sets we want to use the `successive_shortest_path_nonnegative_weights` algorithm which requires that we have non-negative cost.
Maximizing the profits would be equal to minimizing the negativ profits, this however would mean that we would have negative costs on the edges. We can mitigate this by adding `max_bid` to all biddings where `max_bid` is the highest bid between any bidder and any property. We simply need to again adjust the final cost returned by the algotihm.