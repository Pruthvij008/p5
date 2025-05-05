#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    // Add an edge to the graph (undirected)
    void addEdge(int u, int v) {

        adj[u].push_back(v);
        adj[v].push_back(u); // For undirected graph
    }

    // ============== Serial DFS (Baseline) ==============
    void serialDFS(int startVertex) {
        if (startVertex < 0 || startVertex >= V) {
            cerr << "Error: Start vertex " << startVertex << " is invalid!\n";
            return;
        }

        vector<bool> visited(V, false);
        cout << "Serial DFS Order: ";
        auto start = high_resolution_clock::now();
        serialDFSUtil(startVertex, visited);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "\nSerial DFS Time: " << duration.count() << " μs\n";
    }

    void serialDFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";
        
        for (int v : adj[u]) {
            if (!visited[v]) {
                serialDFSUtil(v, visited);
            }
        }
    }

    // ============== Parallel DFS (OpenMP) ==============
    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);
        cout << "Parallel DFS Order: ";
        auto start = high_resolution_clock::now();
    
        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFSUtil(startVertex, visited);
            }
        }
    
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "\nParallel DFS Time: " << duration.count() << " μs\n";
    }
    
    void parallelDFSUtil(int u, vector<bool>& visited) {
        // Mark visited without atomic because each node is visited once in DFS
        visited[u] = true;
    
        #pragma omp critical
        cout << u << " ";
    
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
    
            if (!visited[v]) {
                #pragma omp task
                parallelDFSUtil(v, visited);
            }
        }
    
        #pragma omp taskwait
    }
    
};

int main() {
    // Example graph
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    // Run and compare
    g.serialDFS(0);
    cout << "\n----------------------------------------\n";
    g.parallelDFS(0);

    return 0;
}