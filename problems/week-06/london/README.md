tags: maxflow

## Intuition
We treat the underlying problem as a maxflow problem. Some observations to made
- We only have to deal with uppercase letters A-Z
- We want each character in the node to be "covered". We don't care about any order, we only care about how often a given character needs to be "covered"
- Consider the characters on the front page and backpage as unordered pairs {frontpage_char, backpage_char}, where only one of the two characters can be used to cover a letter from the note.
- For each character A to Z add a vertex to the graph and an edge to the sink with capacitiy equal to the occurence of the char in the note. If we later have an outflow of this vertex equal to its occurence, then this means we were able to cover all occurences of the char in the note.
- Count how many occurences of unordered pair {frontpage_char, backpage_char} there are and for each unordered pair add a node to the graph. Also add an edge from the source to the new node with a capacity that is equal to its occurence. Add an edge from the new node to both frontpage_char and backpage_char with a capacity in both cases equal to the occurence of the unorderpair. The inflow can be translated into how often we will use this pair to cover a letter in the node. The outflow of the node to frontpage_char will translate into how many of the frontpage_char will be covered by this pair and similar for the backpage_char. Note that we cannot use the pair more often than the inflow and we can use it to either cover the frontpage_char or the backpage_char. Pay attention to pairs where frontpage_char = backpage_char. We should only add one edge from the new node to frontpage_char with a capacity equal to it's occurence.
- Run the maxflow algorithm. If maxflow == size(note), then we were able to "cover" all characters with the unordered pairs.

## Runtime complexity
- counting occurence of chars in note: O(n)
- counting occurence of unordered pairs: O(w*h)
- max num nodes in the graph: 2 + 26 + 26*26/2
- running maxflow algorithm by Goldberg, Trajan O(|V|^3) where V is the set of vertices of the graph
- overall runtime: O(n + w*h + (28 + 26 * 26/2)^3)
