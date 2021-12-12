# apasp

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