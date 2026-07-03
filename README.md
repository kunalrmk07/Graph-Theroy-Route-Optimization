# Graph-Theory Analysis & Route Optimization

## Overview
This project simulates a **Smart Transportation Network** modeling over 500 weighted city nodes and connecting roads. It leverages advanced graph-theory algorithms to optimize global infrastructure costs, identify strategic urban development hubs, and ensure absolute network resilience against natural disasters and dynamic traffic congestion.

## Core Features

* **MST Infrastructure Optimization (Task 1):** Implements **Kruskal's Algorithm** utilizing a Disjoint Set Union (DSU) to compute the Minimum Spanning Tree. This minimizes total road construction and maintenance costs while guaranteeing 100% network connectivity.
* **Strategic City Identification (Task 2):** Utilizes **Degree Centrality** to rank urban nodes based on their connectivity. This module automatically identifies high-impact cities perfectly suited for strategic infrastructure like international airports, logistics hubs, and major railway junctions.
* **Disaster Recovery Routing (Task 3):** Features a fault-tolerant routing engine using a modified **Dijkstra’s Algorithm**. It instantly recomputes the optimal fallback shortest paths when user-defined nodes (cities) or edges (roads) are dynamically destroyed by simulated natural disasters.
* **Traffic-Aware Smart Routing (Task 4):** Simulates real-time urban congestion by applying dynamic weight multipliers to edge traversal costs. The system actively detects bottlenecks and computes efficient bypass routes to optimize travel time.
* **Critical Infrastructure Diagnostics (Task 5):** Diagnoses systemic single points of failure. The algorithm systematically eliminates each city node and uses Breadth-First Search (BFS) component counting to measure the resulting structural fractures, identifying the most critical vulnerabilities in the grid.

## Technical Stack
* **Language:** C++20 (or higher)
* **Data Structures:** Adjacency Lists, Disjoint Set Union (DSU), Priority Queues, Hash Sets/Maps
* **Algorithms:** Kruskal's MST, Dijkstra's Shortest Path, Breadth-First Search (BFS), Degree Centrality Analysis

## How to Build and Run

### Prerequisites
You need a C++ compiler (like GCC or Clang) installed on your system.

### Compilation
Open your terminal and compile the code using `g++`:

```bash
g++ main.cpp -o SmartNetwork
