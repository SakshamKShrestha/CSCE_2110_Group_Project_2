
#include "../include/CampusGraph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cctype>

using namespace std;

string CampusGraph::trim(const string& text) {
    const string whitespace = " \t\r\n";
    size_t first = text.find_first_not_of(whitespace); 
    if (first == string::npos) return "";

    size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

string CampusGraph::normalize(const string& text) {
    string result = trim(text);

    for (char& c : result) {
        c = static_cast<char>(
            tolower(static_cast<unsigned char>(c))
        );
    }
    return result;
}

int CampusGraph::findBuilding(const string& buildingName) const {
    string target = normalize(buildingName);

    for (size_t i = 0; i < buildings.size(); ++i) {
        if (normalize(buildings[i].name) == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

string CampusGraph::getBuildingName(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= buildings.size()) {
        return "";
    }
    return buildings[index].name;
}

bool CampusGraph::addBuilding(const string& name) {
    string cleaned = trim(name);

    if (cleaned.empty() || findBuilding(cleaned) != -1) {
        return false;
    }

    buildings.push_back({cleaned, {}});
    return true;
}

bool CampusGraph::addConnection(const string& from,
                                const string& to) {
    int a = findBuilding(from);
    int b = findBuilding(to);

    if (a == -1 || b == -1 || a == b) return false;

    for (int neighbor : buildings[a].neighbors) {
        if (neighbor == b) return false;
    }

    buildings[a].neighbors.push_back(b);
    buildings[b].neighbors.push_back(a);
    ++connectionCount;
    return true;
}

void CampusGraph::loadBuildings(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: cannot open " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected\n";
        return;
    }

    string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (getline(file, line)) {
        ++lineNumber;
        string cleaned = trim(line);

        if (cleaned.empty() || cleaned[0] == '#') continue;

        if (cleaned.find('|') != string::npos) {
            cout << "Warning: " << filename << " line "
                 << lineNumber << ": invalid field count\n";
            ++rejected;
            continue;
        }

        if (!addBuilding(cleaned)) {
            cout << "Warning: " << filename << " line "
                 << lineNumber << ": duplicate or empty building\n";
            ++rejected;
            continue;
        }

        ++accepted;
    }

    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected\n";
}

void CampusGraph::loadConnections(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: cannot open " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected\n";
        return;
    }

    string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (getline(file, line)) {
        ++lineNumber;
        string cleaned = trim(line);

        if (cleaned.empty() || cleaned[0] == '#') continue;

        size_t separator = cleaned.find('|');

        if (separator == string::npos ||
            cleaned.find('|', separator + 1) != string::npos) {
            cout << "Warning: " << filename << " line "
                 << lineNumber << ": invalid field count\n";
            ++rejected;
            continue;
        }

        string from = trim(cleaned.substr(0, separator));
        string to = trim(cleaned.substr(separator + 1));

        string reason;

        if (from.empty() || to.empty()) {
            reason = "empty building field";
        } else if (findBuilding(from) == -1) {
            reason = "unknown building: " + from;
        } else if (findBuilding(to) == -1) {
            reason = "unknown building: " + to;
        } else if (normalize(from) == normalize(to)) {
            reason = "self-connection";
        } else if (!addConnection(from, to)) {
            reason = "duplicate connection";
        }

        if (!reason.empty()) {
            cout << "Warning: " << filename << " line "
                 << lineNumber << ": " << reason << '\n';
            ++rejected;
        } else {
            ++accepted;
        }
    }

    cout << filename << ": " << accepted << " accepted, "
         << rejected << " rejected\n";
}
void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings are loaded.\n";
        return;
    }

    cout << "\nCampus Graph:\n";

    for (const Building& building : buildings) {
        cout << building.name << ": ";

        if (building.neighbors.empty()) {
            cout << "(no connections)";
        } else {
            for (size_t i = 0; i < building.neighbors.size(); ++i) {
                if (i > 0) cout << ", ";
                cout << buildings[building.neighbors[i]].name;
            }
        }
        cout << '\n';
    }
}

void CampusGraph::displayGraphCounts() const {
    cout << "\nTotal buildings: " << buildings.size() << '\n';
    cout << "Total connections: " << connectionCount << '\n';

    if (buildings.empty()) {
        cout << "No buildings are loaded.\n";
        return;
    }

    for (const Building& building : buildings) {
        cout << building.name
             << " - Degree: " << building.neighbors.size() << '\n';
    }
}
void CampusGraph::findReachableBuildings(const string& start) const {
    int startIndex = findBuilding(start);

    if (startIndex == -1) {
        cout << "Unknown building: " << trim(start) << '\n';
        return;
    }
    vector<bool> visited(buildings.size(), false);
    vector<int> distance(buildings.size(), -1);
    queue<int> q;

    visited[startIndex] = true;
    distance[startIndex] = 0;
    q.push(startIndex);

    int count = 0;
    cout << "\nReachable buildings from "
         << buildings[startIndex].name << ":\n";

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        ++count;

        cout << buildings[current].name
             << " (distance " << distance[current] << ")\n";

        for (int neighbor : buildings[current].neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;
                q.push(neighbor);
            }
        }
    }

    cout << "Reachable: " << count
         << " of " << buildings.size() << " buildings\n";
}

void CampusGraph::findShortestPath(const string& start,
                                   const string& destination) const {
    int source = findBuilding(start);
    int target = findBuilding(destination);

    if (source == -1) {
        cout << "Unknown building: " << trim(start) << '\n';
        return;
    }

    if (target == -1) {
        cout << "Unknown building: " << trim(destination) << '\n';
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> parent(buildings.size(), -1);
    queue<int> q;

    visited[source] = true;
    q.push(source);

    while (!q.empty() && !visited[target]) {
        int current = q.front();
        q.pop();

        for (int neighbor : buildings[current].neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (!visited[target]) {
        cout << "No path exists between "
             << buildings[source].name << " and "
             << buildings[target].name << ".\n";
        return;
    }

    vector<int> path;

    for (int at = target; at != -1; at = parent[at]) {
        path.push_back(at);
    }

    reverse(path.begin(), path.end());

    cout << "\nShortest path:\n";
    for (int index : path) {
        cout << buildings[index].name << '\n';
    }

    cout << "Number of connections: " << path.size() - 1 << '\n';
}
void CampusGraph::displayConnectivityReport() const {
    cout << "\nGraph Connectivity Report\n";
    cout << "Total buildings: " << buildings.size() << '\n';
    cout << "Total connections: " << connectionCount << '\n';

    if (buildings.empty()) {
        cout << "No buildings are loaded.\n";
        cout << "Connected components: 0\n";
        return;
    }

    size_t highestDegree = 0;
    for (const Building& building : buildings) {
        highestDegree = max(highestDegree, building.neighbors.size());
    }

    cout << "Highest degree (" << highestDegree << "): ";
    bool first = true;

    for (const Building& building : buildings) {
        if (building.neighbors.size() == highestDegree) {
            if (!first) cout << ", ";
            cout << building.name;
            first = false;
        }
    }
    cout << '\n';

    cout << "Isolated buildings: ";
    first = true;

    for (const Building& building : buildings) {
        if (building.neighbors.empty()) {
            if (!first) cout << ", ";
            cout << building.name;
            first = false;
        }
    }

    if (first) cout << "None";
    cout << '\n';

    vector<bool> visited(buildings.size(), false);
    int components = 0;

    for (size_t i = 0; i < buildings.size(); ++i) {
        if (visited[i]) continue;

        ++components;
        queue<int> q;
        q.push(static_cast<int>(i));
        visited[i] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbor : buildings[current].neighbors) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }

    cout << "Connected components: " << components << '\n';
}
