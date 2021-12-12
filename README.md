# All Pairs Almost Shortest Path 

Implementation of `D. Dor, S. Halperin and U. Zwick, “All pairs almost shortest paths,” Proceedings of 37th Conference on Foundations of Computer Science, 1996, pp. 452-461, doi: 10.1109/SFCS.1996.548504.`.

Team project for SNU Theory of Computation, 2021 Fall. 

Implementation : Jongjin Kim(j2kim99), Suchan Kim(sc613), Wonseok Shin(gratus907).

### Graph Generator
Generates random connected graph, by 
1. Generate random spanning tree of `V` vertices, using Prufer sequence
2. Add `E - V + 1` more edges randomly. 

```sh
g++ graph_gen.cpp -o graph_gen
./graph_gen 20 100
```

Adjacency list is printed to stdout.
```
V E
u0 v0 
u1 v1 
... 
```
`0 <= ui, vi < V` holds.