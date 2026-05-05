#include <bits/stdc++.h>
#include <omp.h>   // ✅ REQUIRED for omp_get_wtime()

using namespace std;

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
    double start = omp_get_wtime();
    g.sequentialBFS(startVertex);
    double end = omp_get_wtime();
    cout << "Sequential BFS: " << (end - start) * 1000 << " ms\n";

    start = omp_get_wtime();
    g.parallelBFS(startVertex);
    end = omp_get_wtime();
    cout << "Parallel BFS: " << (end - start) * 1000 << " ms\n";

    /* -------- DFS -------- */
    start = omp_get_wtime();
    g.sequentialDFS(startVertex);
    end = omp_get_wtime();
    cout << "Sequential DFS: " << (end - start) * 1000 << " ms\n";

    start = omp_get_wtime();
    g.parallelDFS(startVertex);
    end = omp_get_wtime();
    cout << "Parallel DFS: " << (end - start) * 1000 << " ms\n";

    return 0;
}