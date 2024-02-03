tags: Max Flow

## Intuition
This is a very easy problem to code one you spot the trick behind it. We are asked to find a subset of nodes such that there is no edge towards a node outside this subset and such that the sum of conveniences of all nodes in the subset is strictily positive. We solve this using flows and consider the conveniences of the nodes to be some sort of supply and demand. A node with a convenience `c > 0` is considered to have a supply of `c` convenience units that it can distribute among its successors if any present in odrder to compensate for their potential negative convenience. We'll model it with an edge from the source to this node with a capacity equal to `c`. A node with a convenience `c' < 0` is considered as a node that has a demand of `c` convenience units that need to be provided by some other subset of nodes. We'll model it as an edge from the node to the sink of the graph with a capacity set to `-c'`. For each path in the graph from `u` to `v` we will add an edge with capacity equal to `inf`.

Let's no consider what happens:
- if the flow is equal to the amount of convenience we send in, this means that the convenience flowed from nodes with a positive conv to nodes with a negative conv and towards the source. Each node with a negative conv has their demand satisfied, so all potential semi-deead ends have a total sum of 0.
- if the flow is less than the amount we send it, this means that some of the conv we send in did not manage to make it out of the graph. Either because there was no path out of the garph at all (in case we have a dead end with only positive conveniences) or the paths towards the sink were already completely saturated. This means that we have a dead-end because the overshoot in conv cannot esacpe anymore and the sum of this dead-end is positive becuase all the demands have been fulfilled. Note that the problem satement doesn't forbid to group multiple dead-ends together, so we can simply take the sum of all these bottlenecks together which is simply the difference of `amount_conv_send_in - flow`
- note that it cannot happen that the flow is bigger than the amount of convenience we send into the graph.