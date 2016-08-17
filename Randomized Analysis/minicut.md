**Problem Definition:** Given a connected undirected graph $G=(V,E)$, a subset $E' \in E$ is a cut if its removal form the graph disconnects the graph. The mincut is has the smallest number of edges among all possible cuts. 

There is an efficient randomized algorithm  which can solve it in cubic time and it is very easy to implement it.

By the observation that in most case, the number of edges in a cut is always much smaller than the number of edges in the graph, we may consider to randomly pick up an edges not in the cut. Then we contract the endpoint of this edge and may produce a multigraph(Different from the standard graph, there will be more than one edge between two vertices). We repeat the process, choosing another edge at random and merging its endpoint to reflect this edge is not in the cut until only two vertices left. 

Q1. Why do we produce a multigraph? 

**The crucial idea of this algorithm is that if the edge we picked is not in the mincut, the mincut cut set shouldn't be effected after contracting two edges.**
If we remove the repeat edges to generate the standard graph, the edges in the mincut set may be removed.

Q2. The possible that all the edges in the mincut isn't removed and mincut is returned. 

Let's assume that the mincut has size k, then every vertex of $G$ must have degree at least k. Thus $G$ has at least $\frac{kn}{2}$ edges. The probability that our first selection is in the mincut is at most $\frac{k}{nk/2} = \frac{2}{n}$($k$ edges from at least $\frac{kn}{2}$ edges). So the probability of not selecting an edge of the mincut at the first step is at least $1-\frac{2}{n}$. Then the graph remains $n-1$ edges, if we assume the first pick edge is not in mincut, the probability of our second pick still avoid the mincut is at least $1-\frac{2}{n-1}$. After this procedure, the possibility that the mincut set is return is at least
$$
\Pi_{i=0}^{n-3} (1-\frac{2}{n-i}) = \Pi_{i=0}^{n-3} \frac{n-i-2}{n-i} = \frac{2}{n(n-1)}
$$

It seem that for large n, the probability that the the return set is mincut is nearly 0. But this procedure can be finished in linear time. If we repeat it many times and update the mincut set in each trials, the probability that we don't get a mincut after $j$ trials is at most $(1-\frac{2}{n(n-1)})^j$. When $j=\frac{n^2}{2}$, an acceptable probability that mincut can be obtained is at least
$$
(1- \frac{2}{n(n-1)})^{\frac{n^2}{2}} \lt \frac{1}{e}
$$

The overall running time will be $O(n^3)$.

There is an significant improvement.

