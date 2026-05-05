// #include <iostream>
// #include <vector>
// #include <queue>
// #include <stack>
// #include <omp.h>
// #include <chrono>
// #include <iomanip>
// #include <cstdlib>
#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

class Graph
{
private:
    int numVertices;
    vector<vector<int>> adj;

public:
    Graph(int vertices) : numVertices(vertices), adj(vertices) {}

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    /* -------- SEQUENTIAL BFS -------- */
    void sequentialBFS(int start)
    {
        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            for (int neighbor : adj[node])
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    /* -------- PARALLEL BFS -------- */
    void parallelBFS(int start)
    {
        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++)
            {
                int neighbor = adj[node][i];

                if (!visited[neighbor])
                {
                    #pragma omp critical
                    {
                        if (!visited[neighbor])
                        {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
            }
        }
    }

    /* -------- SEQUENTIAL DFS -------- */
    void sequentialDFS(int start)
    {
        vector<bool> visited(numVertices, false);
        stack<int> s;

        s.push(start);

        while (!s.empty())
        {
            int node = s.top();
            s.pop();

            if (!visited[node])
            {
                visited[node] = true;

                for (int neighbor : adj[node])
                {
                    if (!visited[neighbor])
                        s.push(neighbor);
                }
            }
        }
    }

    /* -------- PARALLEL DFS -------- */
    void parallelDFS(int start)
    {
        vector<bool> visited(numVertices, false);
        stack<int> s;

        visited[start] = true;
        s.push(start);

        while (!s.empty())
        {
            int node = s.top();
            s.pop();

            #pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++)
            {
                int neighbor = adj[node][i];

                if (!visited[neighbor])
                {
                    #pragma omp critical
                    {
                        if (!visited[neighbor])
                        {
                            visited[neighbor] = true;
                            s.push(neighbor);
                        }
                    }
                }
            }
        }
    }
};

/* ---------------- MAIN ---------------- */
int main()
{
        cout << fixed << setprecision(4);


    int numVertices = 30000;
    int numEdges = 350000;

    cout << "Auto-generating graph...\n";

    Graph g(numVertices);

    // Generate random edges
    for (int i = 0; i < numEdges; i++)
    {
        int u = rand() % numVertices;
        int v = rand() % numVertices;
        if (u != v)
            g.addEdge(u, v);
    }

    int startVertex = 0;

    /* -------- BFS -------- */
    auto start = high_resolution_clock::now();
    g.sequentialBFS(startVertex);
    auto end = high_resolution_clock::now();
    cout << "Sequential BFS: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    g.parallelBFS(startVertex);
    end = high_resolution_clock::now();
    cout << "Parallel BFS: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    /* -------- DFS -------- */
    start = high_resolution_clock::now();
    g.sequentialDFS(startVertex);
    end = high_resolution_clock::now();
    cout << "Sequential DFS: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    start = high_resolution_clock::now();
    g.parallelDFS(startVertex);
    end = high_resolution_clock::now();
    cout << "Parallel DFS: "
         << duration_cast<milliseconds>(end - start).count() << " ms\n";

    return 0;
}
