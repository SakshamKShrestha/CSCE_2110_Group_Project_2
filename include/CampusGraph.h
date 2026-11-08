#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>

class CampusGraph{
    private:
        struct Building {
            std::string name;
            std::vector<int> neighbors;
        };

        std::vector<Building> buildings;
        int connectionCount = 0;

        static std::string trim(const std::string& text);
        static std::string normalize(const std::string& text);

public:
        int findBuilding(const std::string& buildingName) const;

        std::string getBuildingName(int index)const;

        bool addBuilding(const std::string& name);

        bool addConnection(const std::string& from, const std::string& to);

        void loadBuildings(const std::string& filename);
        void loadConnections(const std::string& filename);

        void displayGraph() const;
        void displayGraphCounts() const;

        void findReachableBuildings(const std::string& start) const;

        void findShortestPath(const std::string& start, const std::string& destination) const;

void displayConnectivityReport() const;
};

#endif