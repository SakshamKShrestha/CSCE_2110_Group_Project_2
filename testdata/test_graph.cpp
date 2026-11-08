
#include "../include/CampusGraph.h"
#include <iostream>
using namespace std;

// Temporary test program for Module B
int main() {
    CampusGraph graph;

    // Load instructor data files.
    graph.loadBuildings("data/buildings.txt");
    graph.loadConnections("data/connections.txt");

    cout << "\n===== TEST 1: CAMPUS GRAPH =====\n";
    graph.displayGraph();

    cout << "\n===== TEST 2: GRAPH STATISTICS =====\n";
    graph.displayGraphCounts();

    cout << "\n===== TEST 3: BFS FROM LIBRARY =====\n";
    graph.findReachableBuildings("Library");

    cout << "\n===== TEST 4: BFS FROM OBSERVATORY =====\n";
    graph.findReachableBuildings("Observatory");

    cout << "\n===== TEST 5: SHORTEST PATH =====\n";
    graph.findShortestPath("Library", "Engineering Building");

    cout << "\n===== TEST 6: PATH TIE-BREAKING =====\n";
    graph.findShortestPath("Library", "Gym");

    cout << "\n===== TEST 7: NO PATH =====\n";
    graph.findShortestPath("Library", "Observatory");

    cout << "\n===== TEST 8: CONNECTIVITY REPORT =====\n";
    graph.displayConnectivityReport();

    cout << "\n===== TEST 9: CASE-INSENSITIVE LOOKUP =====\n";
    cout << "Index of library: "
         << graph.findBuilding("  library  ") << '\n';

    cout << "\nAll test operations completed.\n";
    return 0;
}

