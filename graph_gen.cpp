#include <bits/stdc++.h>
using namespace std; 

minstd_rand rng(chrono::high_resolution_clock::now().time_since_epoch().count());
template <typename T> T randint(T a, T b) { return uniform_int_distribution<T>(a, b)(rng);}

vector<vector<int>> generate_graph(int V, int E) {
    vector<vector<int>> G(V);
    vector<vector<bool>> vst(V, vector<bool>(V, 0));
    // Generate random spanning tree with Prufer sequence
    vector<int> prufer_seq(V-2, 0);
    set <int> L;
    for (int i = 0; i < V-2; i++) {
        prufer_seq[i] = randint(0, V-1);
    }
    vector<int> degree(V, 1);
    for (int x : prufer_seq) degree[x]++;
    for (int i = 0; i < V; i++) {
        if (degree[i] == 1) L.insert(i);
    }
    for (int i = 0; i < V-2; i++) {
        int u = *L.begin(), p = prufer_seq[i];
        degree[u]--;
        degree[p]--;
        G[u].push_back(p); G[p].push_back(u);
        vst[u][p] = vst[p][u] = 1;
        L.erase(u);
        if (degree[p] == 1) L.insert(p);
    }
    int a = *L.begin(), b = *L.rbegin();
    vst[a][b] = vst[b][a] = 1; 
    G[a].push_back(b); G[b].push_back(a);


    // Add (E - V + 1) more edges, randomly 
    vector <pair<int, int>> edge_list; 
    for (int i = 0; i < V; i++) 
        for (int j = i+1; j < V; j++) 
            if (!vst[i][j]) 
                edge_list.push_back({i, j});
    random_shuffle(edge_list.begin(), edge_list.end());
    for (int i = 0; i < E - V + 1; i++) {
        int a, b; tie(a, b) = edge_list[i];
        G[a].push_back(b); G[b].push_back(a);
    }
    return G;
}

void print_graph(vector<vector<int>> graph) {
    vector <pair<int, int>> edge_list;
    for (int i = 0; i < graph.size(); i++)
        for (int j : graph[i])
            if (i < j) 
                edge_list.push_back({i, j});
    printf("%d %d\n", (int)graph.size(), (int)edge_list.size());
    for (auto p : edge_list) {
        printf("%d %d\n", p.first, p.second);
    }
}

int main(int argc, char* argv[]) {
    int V, E; 
    if (argc >= 2) {
        V = atoi(argv[1]);
        E = atoi(argv[2]);
        E = max(E, V - 1);
    }
    else {
        V = 10, E = 20;
    }
    vector<vector<int>> graph = generate_graph(V, E);
    print_graph(graph);
}