#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>
#include <mutex>

using namespace std;
using namespace std::chrono;

class Graph {
    int V;
    vector<vector<int>> adj;
    mutex cout_mutex; // Mutex for thread-safe output

public:
Graph(int V) {
    this->V = V;         // 'this->V' refers to the class member; 'V' is the parameter
    adj.resize(V);
}

    
    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }
    
    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        queue<int> q;
        
        visited[startVertex] = true;
        q.push(startVertex);
        
        while (!q.empty()) {
            int levelSize = q.size();
            vector<pair<int, int>> visited_nodes; // (vertex, thread_id)
            
            // Process current level in parallel
            #pragma omp parallel
            {
                #pragma omp for
                for (int i = 0; i < levelSize; i++) {
                    int v;
                    #pragma omp critical
                    {
                        v = q.front();
                        q.pop();
                    }
                    
                    int thread_id = omp_get_thread_num();
                    
                    // Record visited node with thread info
                    #pragma omp critical
                    {
                        visited_nodes.emplace_back(v, thread_id);
                    }
                    
                    // Explore neighbors
                    for (int neighbor : adj[v]) {
                        if (!visited[neighbor]) {
                            #pragma omp critical
                            {
                                if (!visited[neighbor]) {
                                    visited[neighbor] = true;
                                    q.push(neighbor);
                                }
                            }
                        }
                    }
                }
            }
            
            // Print visited nodes in order after processing the level
            for (auto& node : visited_nodes) {
                lock_guard<mutex> guard(cout_mutex);
                cout << "Visited " << node.first << " by thread " << node.second << endl;
            }
        }
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    
    cout << "Parallel BFS starting from vertex 0:\n";
    auto start = high_resolution_clock::now();
    g.parallelBFS(0);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Parallel BFS took " << duration.count() << " microseconds\n";
    
    return 0;
}