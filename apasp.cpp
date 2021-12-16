#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <cassert>

#define K 2

using namespace std;
using AdjList = vector<vector<int>>;

template <typename T>
struct Matrix
{
    vector<T> data;
    int n_rows;
    int n_cols;

public:
    T &operator()(int row, int col)
    {
        return data[row * n_cols + col];
    }

    Matrix() {}

    Matrix(int n_rows, int n_cols, T value)
        : data(n_rows * n_cols, value), n_rows(n_rows), n_cols(n_cols)
    {
    }

    void print()
    {
        for (int u = 0; u < n_rows; ++u)
        {
            for (int v = 0; v < n_cols; ++v)
            {
                cout << (*this)(u, v) << ' ';
            }
            cout << '\n';
        }
    }
};

void add_edge(AdjList &graph, int u, int v)
{
    graph[u].push_back(v);
    graph[v].push_back(u);
}

void merge_graphs(AdjList &to, AdjList &from)
{
    assert(from.size() == to.size());

    int n = from.size();
    for (int u = 0; u < n; ++u)
    {
        to[u].insert(to[u].end(), from[u].begin(), from[u].end());
    }
}

// Returns edges incident to a vertex of degree < `threshold`.
AdjList get_edge_subset(AdjList &graph, int threshold)
{
    AdjList new_graph(graph.size());

    for (int u = 0; u < (int)graph.size(); ++u)
    {
        int u_deg = graph[u].size();
        for (int v : graph[u])
        {
            int v_deg = graph[v].size();
            if (u_deg < threshold || v_deg < threshold)
            {
                new_graph[u].push_back(v);
            }
        }
    }

    return new_graph;
}

// Returns a list of vertices dominating other vertices of deg >= `threshold`,
// and edges connecting dominated vertices and dominating ones.
// Size of the dominating set: O((V log V) / `threshold`)
// Time complexity: O(V + E)
pair<vector<int>, AdjList> dominate(AdjList &graph, int threshold)
{
    // number of dominated neighbors (not including itself)
    vector<int> dominating_count(graph.size());

    // bucket queue (key: dominating count, value: node index)
    vector<vector<int>> bucket(graph.size());

    for (int u = 0; u < (int)graph.size(); ++u)
    {
        for (int v : graph[u])
        {
            int v_deg = graph[v].size();
            if (v_deg >= threshold)
            {
                ++dominating_count[u];
            }
        }

        if (dominating_count[u] > 0)
        {
            bucket[dominating_count[u]].push_back(u);
        }
    }

    vector<bool> is_dominated(graph.size());
    vector<int> dominating_set;
    AdjList new_graph(graph.size());

    for (int i = graph.size() - 1; i > 0; --i)
    {
        while (!bucket[i].empty())
        {
            // a vertex currently dominating the largest number of vertices
            int u = bucket[i].back();
            bucket[i].pop_back();
            if (i != dominating_count[u])
            {
                continue; // stale key
            }

            dominating_set.push_back(u);

            int u_deg = graph[u].size();
            if (u_deg >= threshold && !is_dominated[u])
            {
                is_dominated[u] = true;

                // update neighbors of dominating vertex
                for (int v : graph[u])
                {
                    if (--dominating_count[v] > 0)
                    {
                        bucket[dominating_count[v]].push_back(v);
                    }
                }
            }

            for (int v : graph[u])
            {
                int v_deg = graph[v].size();
                if (v_deg >= threshold && !is_dominated[v])
                {
                    is_dominated[v] = true;
                    add_edge(new_graph, u, v);

                    // update neighbors of dominated vertex
                    for (int x : graph[v])
                    {
                        if (x != u && --dominating_count[x] > 0)
                        {
                            bucket[dominating_count[x]].push_back(x);
                        }
                    }
                }
            }
        }
    }

    return make_pair(dominating_set, new_graph);
}

// Runs Dijkstra's algorithm with integer weights.
// Time complexity: O(V + E)
// @param weight: 2D array storing estimated distance
// @param s: source node index
void dijkstra(AdjList &graph, Matrix<int> &weight, int s)
{
    int max_dist = 2 * (graph.size() + K);
    vector<int> dist(graph.size(), max_dist);

    // bucket queue (key: distance, value: node index)
    vector<vector<int>> bucket(max_dist);

    dist[s] = 0;
    bucket[0].push_back(s);

    for (int i = 0; i < (int)graph.size() + K; ++i)
    {
        while (!bucket[i].empty())
        {
            int u = bucket[i].back();
            bucket[i].pop_back();
            if (i != dist[u])
            {
                continue;
            }

            // confirmed to be the shortest distance on this graph
            if (i < weight(s, u))
            {
                weight(s, u) = i;
                weight(u, s) = i;
            }

            for (int v : graph[u])
            {
                if (dist[v] > dist[u] + weight(u, v))
                {
                    dist[v] = dist[u] + weight(u, v);
                    bucket[dist[v]].push_back(v);
                }
            }
        }
    }
}

// See Theorem 3.1
Matrix<int> apasp_a(AdjList &G)
{
    int n = G.size(); // V
    int m = 0;        // E
    for (auto edge_list : G)
    {
        m += edge_list.size();
    }
    m /= 2;

    int s1 = lround(sqrt(m / n));

    AdjList E2 = get_edge_subset(G, s1);

    vector<int> D1;
    AdjList Es;
    tie(D1, Es) = dominate(G, s1);

    Matrix<int> d(n, n, n);
    for (int u = 0; u < n; ++u)
    {
        for (int v : G[u])
        {
            d(u, v) = 1;
        }
    }

    vector<bool> is_visited(n);
    for (int u : D1)
    {
        is_visited[u] = true;
        dijkstra(G, d, u);
    }

    merge_graphs(E2, Es);
    for (int u = 0; u < n; ++u)
    {
        if (is_visited[u])
        {
            continue;
        }

        AdjList new_graph = E2;
        for (int v : D1)
        {
            add_edge(new_graph, u, v);
        }
        dijkstra(new_graph, d, u);
    }

    return d;
}

// See Theorem 3.2
Matrix<int> apasp_b(AdjList &G)
{
    int n = G.size(); // V

    int s1 = lround(pow(n, 2. / 3));
    int s2 = lround(pow(n, 1. / 3));

    AdjList E2 = get_edge_subset(G, s1);
    AdjList E3 = get_edge_subset(G, s2);

    vector<int> D1, D2;
    AdjList Es1, Es2;
    tie(D1, Es1) = dominate(G, s1);
    tie(D2, Es2) = dominate(G, s2);
    merge_graphs(Es1, Es2);

    Matrix<int> d(n, n, n);
    for (int u = 0; u < n; ++u)
    {
        for (int v : G[u])
        {
            d(u, v) = 1;
        }
    }

    for (int u : D1)
    {
        dijkstra(G, d, u);
    }

    for (int u : D2)
    {
        dijkstra(E2, d, u);
    }

    merge_graphs(E3, Es1);
    for (int u : D1)
    {
        for (int v = 0; v < n; ++v)
        {
            if (u != v)
            {
                add_edge(E3, u, v);
            }
        }
    }
    for (int u : D2)
    {
        for (int v : D2)
        {
            if (u != v)
            {
                add_edge(E3, u, v);
            }
        }
    }

    for (int u = 0; u < n; ++u)
    {
        AdjList new_graph = E3;
        for (int v : D2)
        {
            if (u != v)
            {
                add_edge(new_graph, u, v);
            }
        }
        dijkstra(new_graph, d, u);
    }

    return d;
}

int main(int argc, char *argv[])
{
    int node_count, edge_count;
    cin >> node_count >> edge_count;

    AdjList G(node_count);

    for (int i = 0; i < edge_count; ++i)
    {
        int u, v;
        cin >> u >> v;
        add_edge(G, u, v);
    }

    Matrix<int> d;

    bool sparse = (pow(node_count, 1.5)*pow(edge_count, 0.5) < pow(node_count, 2.333));
    d = sparse ? apasp_a(G) : apasp_b(G);

    d.print();
}