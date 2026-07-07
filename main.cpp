#include<bits/stdc++.h>
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

class DSU {
    vector<int>rank;
    vector<int>par;
    vector<int>sz;
public:
    DSU(int n){
        rank.resize(n+1,0);
        par.resize(n+1,0);
        sz.resize(n+1,1);
        for(int i=0;i<=n;i++) {
            par[i]=i;
        }
    }
    int find_ultimate_par(int u){
        if(par[u]==u) {
            return u;
        }
        return par[u]=find_ultimate_par(par[u]);
    }
    bool unite(int x,int y){
        int u=find_ultimate_par(x);
        int v=find_ultimate_par(y);
        if(u==v) return false;
        if(rank[u]>rank[v]) {
            par[v]=u;
        }
        else if (rank[v]>rank[u]) {
            par[u]=v;
        }
        else {
            rank[u]++;
            par[v]=u;
        }
        return true;
    }
    void unite_by_size(int x,int y) {
        int u=find_ultimate_par(x);
        int v=find_ultimate_par(y);
        if(u==v) return;
        if(sz[u]>sz[v]) {
            sz[u]+=sz[v];
            par[v]=u;
        }
        else {
            sz[v]+=sz[u];
            par[u]=v;
        }
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
    void get_input(int edges) {
        int u,v;
        double w;
        while (cin >> u >> v >> w) {
            addEdge(u, v, w);
        }
    }
    void addEdge(int u, int v, double w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edges.push_back({u, v, w});
    }
    // task1
    void task1_MST() {
        cout << "\nTASK 1: MST OPTIMIZATION\n";
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
    // task2
    void task2_Centrality() {
        cout << "\nTASK 2: STRATEGIC CITY IDENTIFICATION\n";
        vector<pair<int, int>> degreeMap; // {degree, city_id}

        for (int i = 0; i < V; i++) {
            degreeMap.push_back({adj[i].size(), i});
        }
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
    // task3
    void task3_DisasterRecovery(int src, int dest, set<int> destroyedNodes, set<pair<int,int>> destroyedEdges) {
        cout << "\nTASK 3: DISASTER RECOVERY ROUTING\n";
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
    // task4
    void task4_TrafficRouting(int src, int dest) {
        cout << "\nTASK 4: TRAFFIC-AWARE SMART ROUTING\n";
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
        cout << "\nTASK 5: CRITICAL INFRASTRUCTURE ANALYSIS\n";
        int maxComponents = 0;
        int criticalCity = -1;
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
    // freopen("CITY_INPUT.txt", "r", stdin);
    int TOTAL_CITIES = 500;
    SmartTransportationNetwork graph(TOTAL_CITIES);
    graph.get_input(801);
    graph.task1_MST();
    graph.task2_Centrality();
    // Simulate Disaster
    set<int> destroyedCities = {44, 99};
    set<pair<int,int>> destroyedRoads = {{10, 20}};
    graph.task3_DisasterRecovery(0, 250, destroyedCities, destroyedRoads);
    graph.task4_TrafficRouting(0, 250);
    graph.task5_CriticalAnalysis();
    return 0;
}
