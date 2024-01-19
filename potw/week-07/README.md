tags: greedy, DFS

## Intuition
Note that bombs are organised in the shape of a tree which makes it very easy to solve this problem using a DFS. To solve this problem, we will try to deactivate the bombs in increasing time order, i.e deactivate the bombs with lower explosion time first. If we want to deactivate a given bomb, we have the following two cases:
- the bomb is a leave in the tree, thus we can deactivate it given that we're still below the explosion time
- the bomb has children in the tree. We need to ensure that the children bombs are deactivated first. Note that because of our order, all of the children bombs have an explosion time that is greater than the bomb we're currently considering. Thus if one of the children bombs explodes, the current bomb will already have exploded. Note that it might be the case that a children bomb has already been deactivated. In order to keep track of this and avoid unnecessary work we will set the explosion time of the bomb to `-1`. Every time we have at least one children bomb that needs to be deactivated first we will start a DFS from the current bomb to recursivels deactivate all children bombs first if not already done. This could both be done recursively or iteratively using a stack (which I choose).