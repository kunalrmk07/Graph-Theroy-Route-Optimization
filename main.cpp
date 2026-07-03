#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <map>
#include <random>
#include <iomanip>

using namespace std;

const double INF = 1e9;

// Structure to represent an Edge
struct Edge {
    int u, v;
    double weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set Union (DSU) for Kruskal's Algorithm
class DSU {
    vector<int> parent, rank;
public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) parent[root_i] = root_j;
            else if (rank[root_i] > rank[root_j]) parent[root_j] = root_i;
            else {
                parent[root_j] = root_i;
                rank[root_i]++;
            }
            return true;
        }
        return false;
    }
};

class SmartTransportationNetwork {
private:
    int V;
    vector<vector<pair<int, double>>> adj;
    vector<Edge> edges;

public:
    SmartTransportationNetwork(int vertices) : V(vertices) {
        adj.resize(V);
    }

    // Generates a random connected graph
    void generateRandomNetwork(int extraEdges) {
        mt19937 rng(42); // Seeded for reproducibility
        uniform_int_distribution<int> distNode(0, V - 1);
        uniform_real_distribution<double> distWeight(10.0, 500.0);

        // Ensure graph is connected (Tree spanning all nodes)
        for (int i = 1; i < V; i++) {
            int u = distNode(rng) % i;
            int v = i;
            double w = distWeight(rng);
            addEdge(u, v, w);
        }

        // Add extra random edges for complexity
        for (int i = 0; i < extraEdges; i++) {
            int u = distNode(rng);
            int v = distNode(rng);
            if (u != v) {
                addEdge(u, v, distWeight(rng));
            }
        }
    }

    void addEdge(int u, int v, double w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edges.push_back({u, v, w});
    }

    // ==========================================
    // TASK 1: MST OPTIMIZATION (Kruskal's)
    // ==========================================
    void task1_MST() {
        cout << "\n--- TASK 1: MST OPTIMIZATION ---\n";
        double originalCost = 0, mstCost = 0;
        for (auto& edge : edges) originalCost += edge.weight;

        vector<Edge> sortedEdges = edges;
        sort(sortedEdges.begin(), sortedEdges.end());

        DSU dsu(V);
        vector<Edge> mstEdges;

        for (auto& edge : sortedEdges) {
            if (dsu.unite(edge.u, edge.v)) {
                mstCost += edge.weight;
                mstEdges.push_back(edge);
            }
        }

        double savings = ((originalCost - mstCost) / originalCost) * 100.0;
        cout << fixed << setprecision(2);
        cout << "Original Cost: " << originalCost << "\n";
        cout << "MST Cost: " << mstCost << "\n";
        cout << "Cost Saved: " << savings << "%\n";
        cout << "[All " << mstEdges.size() << " structural spans successfully selected]\n";
    }

    // ==========================================
    // TASK 2: STRATEGIC CITY IDENTIFICATION
    // ==========================================
    void task2_Centrality() {
        cout << "\n--- TASK 2: STRATEGIC CITY IDENTIFICATION ---\n";
        vector<pair<int, int>> degreeMap; // {degree, city_id}

        for (int i = 0; i < V; i++) {
            degreeMap.push_back({adj[i].size(), i});
        }

        // Sort descending by degree
        sort(degreeMap.rbegin(), degreeMap.rend());

        for (int i = 0; i < 3; i++) {
            cout << "Rank " << i + 1 << " -> City " << degreeMap[i].second
                 << " -> Degree " << degreeMap[i].first << "\n";
        }

        cout << "\nTop Development Candidates:\n";
        cout << " * Airport: City " << degreeMap[0].second << "\n";
        cout << " * Logistics Hub: City " << degreeMap[1].second << "\n";
        cout << " * Railway Junction: City " << degreeMap[2].second << "\n";
    }

    // ==========================================
    // TASK 3: DISASTER RECOVERY ROUTING
    // ==========================================
    void task3_DisasterRecovery(int src, int dest, set<int> destroyedNodes, set<pair<int,int>> destroyedEdges) {
        cout << "\n--- TASK 3: DISASTER RECOVERY ROUTING ---\n";
        vector<double> dist(V, INF);
        vector<int> parent(V, -1);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            double d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;
            if (destroyedNodes.count(u)) continue;

            for (auto& edge : adj[u]) {
                int v = edge.first;
                double weight = edge.second;

                if (destroyedNodes.count(v)) continue;

                // Check if edge is destroyed
                pair<int,int> e1 = {u, v}, e2 = {v, u};
                if (destroyedEdges.count(e1) || destroyedEdges.count(e2)) continue;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[dest] == INF) {
            cout << "Fallback state: No valid route available after disaster.\n";
        } else {
            cout << "Recovery Path Distance = " << dist[dest] << " km\nPath: ";
            vector<int> path;
            for (int v = dest; v != -1; v = parent[v]) path.push_back(v);
            reverse(path.begin(), path.end());
            for(int i=0; i<path.size(); i++) {
                cout << path[i] << (i == path.size()-1 ? "" : " -> ");
            }
            cout << "\n";
        }
    }

    // ==========================================
    // TASK 4: TRAFFIC-AWARE SMART ROUTING
    // ==========================================
    void task4_TrafficRouting(int src, int dest) {
        cout << "\n--- TASK 4: TRAFFIC-AWARE SMART ROUTING ---\n";
        // Simple Dijkstra but dynamically scaling some edges
        mt19937 rng(123);
        uniform_real_distribution<double> trafficMult(1.0, 3.0);

        vector<double> dist(V, INF);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            double d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto& edge : adj[u]) {
                int v = edge.first;
                // Simulate real-world traffic delay mapping
                double dynamicWeight = edge.second * trafficMult(rng);

                if (dist[u] + dynamicWeight < dist[v]) {
                    dist[v] = dist[u] + dynamicWeight;
                    pq.push({dist[v], v});
                }
            }
        }
        cout << "[System actively calculated congestion bypass routes based on elevated edge penalties]\n";
        cout << "Traffic Route Distance = " << fixed << setprecision(2) << dist[dest] << " km\n";
    }

    // ==========================================
    // TASK 5: CRITICAL INFRASTRUCTURE ANALYSIS
    // ==========================================
    // Helper function for BFS component counting
    int countComponents(int ignoreNode) {
        vector<bool> visited(V, false);
        int components = 0;
        if (ignoreNode != -1) visited[ignoreNode] = true;

        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                components++;
                queue<int> q;
                q.push(i);
                visited[i] = true;
                while (!q.empty()) {
                    int curr = q.front();
                    q.pop();
                    for (auto& edge : adj[curr]) {
                        int v = edge.first;
                        if (!visited[v] && v != ignoreNode) {
                            visited[v] = true;
                            q.push(v);
                        }
                    }
                }
            }
        }
        return components;
    }

    void task5_CriticalAnalysis() {
        cout << "\n--- TASK 5: CRITICAL INFRASTRUCTURE ANALYSIS ---\n";
        int maxComponents = 0;
        int criticalCity = -1;

        // Node Elimination logic
        for (int i = 0; i < V; i++) {
            int comps = countComponents(i);
            if (comps > maxComponents) {
                maxComponents = comps;
                criticalCity = i;
            }
        }

        cout << "Most Critical City: City " << criticalCity << "\n";
        cout << "Disconnected Components Created: " << maxComponents << "\n";
        cout << "[Removal of City " << criticalCity << " breaks network graph into " << maxComponents << " disconnected subgrids]\n";
    }
};

int main() {
    int TOTAL_CITIES = 500;
    SmartTransportationNetwork graph(TOTAL_CITIES);

    // Generate base spanning tree + 1500 random roads to make a dense network
    graph.generateRandomNetwork(1500);

    // Execute Tasks
    graph.task1_MST();
    graph.task2_Centrality();

    // Simulate Disaster: City 44 and Edge between 10 and 20 are destroyed
    set<int> destroyedCities = {44, 99};
    set<pair<int,int>> destroyedRoads = {{10, 20}};
    graph.task3_DisasterRecovery(0, 250, destroyedCities, destroyedRoads);

    graph.task4_TrafficRouting(0, 250);
    graph.task5_CriticalAnalysis();

    return 0;
}