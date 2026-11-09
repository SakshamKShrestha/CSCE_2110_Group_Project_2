
#include "Facility.h"
#include "HashTable.h"
#include "CampusGraph.h"
#include "DepartmentTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

string trim(string text) {
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos)
        return "";

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}

string uppercase(string text) {
    for (char& c : text) {
        c = toupper(static_cast<unsigned char>(c));
    }

    return text;
}

bool sameText(string a, string b) {
    return uppercase(trim(a)) == uppercase(trim(b));
}

vector<string> splitLine(string line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, '|')) {
        fields.push_back(trim(field));
    }

    if (!line.empty() && line.back() == '|') {
        fields.push_back("");
    }

    return fields;
}

bool validID(string id) {
    if (id.length() != 4 || id[0] != 'F')
        return false;

    for (int i = 1; i < 4; i++) {
        if (!isdigit(static_cast<unsigned char>(id[i])))
            return false;
    }

    return true;
}

vector<string> loadBuildings(string filename) {
    vector<string> buildings;
    ifstream file(filename);

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    if (!file.is_open()) {
        cout << "ERROR: Cannot open " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected\n";
        return buildings;
    }

    string line;

    while (getline(file, line)) {
        lineNumber++;

        string name = trim(line);

        if (name.empty() || name[0] == '#')
            continue;

        bool duplicate = false;

        for (string building : buildings) {
            if (sameText(name, building)) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            cout << "WARNING: " << filename
                 << ", line " << lineNumber
                 << ": duplicate building name\n";

            rejected++;
            continue;
        }

        buildings.push_back(name);
        accepted++;
    }

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected\n";

    return buildings;
}

bool buildingExists(
    const vector<string>& buildings,
    string name) {

    for (string building : buildings) {
        if (sameText(building, name))
            return true;
    }

    return false;
}

void loadFacilities(
    string filename,
    const vector<string>& buildings,
    HashTable& hashTable,
    FacilityDatabase& database) {

    ifstream file(filename);

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    if (!file.is_open()) {
        cout << "ERROR: Cannot open " << filename << endl;

        cout << filename
             << ": 0 accepted, 0 rejected"
             << " | Capacity: " << hashTable.getCapacity()
             << " | Entries: " << hashTable.getEntries()
             << " | Collisions: " << hashTable.getCollisions()
             << endl;

        return;
    }

    string line;

    while (getline(file, line)) {
        lineNumber++;

        string cleaned = trim(line);

        if (cleaned.empty() || cleaned[0] == '#')
            continue;

        vector<string> fields = splitLine(line);

        if (fields.size() != 4) {
            cout << "WARNING: " << filename
                 << ", line " << lineNumber
                 << ": incorrect number of fields\n";

            rejected++;
            continue;
        }

        Facility f;

        f.id = uppercase(fields[0]);
        f.name = fields[1];
        f.building = fields[2];
        f.category = fields[3];

        if (f.id.empty() || f.name.empty() ||
            f.building.empty() || f.category.empty()) {

            cout << "WARNING: " << filename
                 << ", line " << lineNumber
                 << ": empty required field\n";

            rejected++;
            continue;
        }

        if (!validID(f.id)) {
            cout << "WARNING: " << filename
                 << ", line " << lineNumber
                 << ": invalid ID format\n";

            rejected++;
            continue;
        }

        if (!buildingExists(buildings, f.building)) {
            cout << "WARNING: " << filename
                 << ", line " << lineNumber
                 << ": unknown building "
                 << f.building << endl;

            rejected++;
            continue;
        }

        for (string building : buildings) {
            if (sameText(building, f.building)) {
                f.building = building;
                break;
            }
        }

        if (!hashTable.insert(f)) {
            cout << "WARNING: " << filename
                 << ", line " << lineNumber
                 << ": duplicate facility ID "
                 << f.id << endl;

            rejected++;
            continue;
        }

        database.addFacility(f);
        accepted++;
    }

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected"
         << " | Capacity: " << hashTable.getCapacity()
         << " | Entries: " << hashTable.getEntries()
         << " | Collisions: " << hashTable.getCollisions()
         << endl;
}

void searchByID(HashTable& hashTable) {
    string id;

    cout << "Enter Facility ID: ";
    getline(cin, id);

    id = uppercase(trim(id));

    if (!validID(id)) {
        cout << "Invalid format: an ID is F followed "
             << "by exactly three digits.\n";
        return;
    }

    Facility* result = hashTable.findBuilding(id);

    if (result == nullptr) {
        cout << "Not found: no facility with ID "
             << id << endl;
    }
    else {
        cout << "\nFound: "
             << result->id << " | "
             << result->name << " | "
             << result->building << " | "
             << result->category << endl;
    }
}

void categoryReport(const FacilityDatabase& database) {
    const vector<Facility>& facilities =
        database.getFacilities();

    cout << "\n===== CATEGORY REPORT =====\n";

    if (facilities.empty()) {
        cout << "No facilities available.\n";
        cout << "Total: 0\n";
        return;
    }

    vector<string> categories;
    vector<int> counts;

    for (const Facility& f : facilities) {
        int index = -1;

        for (int i = 0; i < categories.size(); i++) {
            if (sameText(categories[i], f.category)) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            categories.push_back(f.category);
            counts.push_back(1);
        }
        else {
            counts[index]++;
        }
    }

    cout << left << setw(25) << "Category"
         << setw(12) << "Count"
         << "Percentage\n";

    for (int i = 0; i < categories.size(); i++) {
        double percentage =
            (counts[i] * 100.0) / facilities.size();

        cout << left << setw(25) << categories[i]
             << setw(12) << counts[i]
             << fixed << setprecision(1)
             << percentage << "%\n";
    }

    cout << "Total: " << facilities.size() << endl;
}

int main(int argc, char* argv[]) {
    string directory = "data/";

    if (argc > 1) {
        directory = argv[1];

        if (!directory.empty() &&
            directory.back() != '/' &&
            directory.back() != '\\') {
            directory += "/";
        }
    }

    HashTable hashTable;
    FacilityDatabase database;
    CampusGraph graph;
    DepartmentTree tree;

    cout << "===== LOADING DATA =====\n";

    vector<string> buildings =
        loadBuildings(directory + "buildings.txt");
    for (size_t i = 0; i < buildings.size(); i++)
    {
        graph.addBuilding(buildings[i]);
    }
    graph.loadConnections(directory + "connections.txt");

    loadFacilities(
        directory + "facilities.txt",
        buildings,
        hashTable,
        database
    );

    tree.loadFromFile(directory + "departments.txt", "departments.txt");

    int choice =-1;

    do {
                cout << "\n===== SMART CAMPUS MENU =====\n";
        cout << "1. View all facilities\n";
        cout << "2. Search facility by ID\n";
        cout << "3. View campus graph\n";
        cout << "4. Display graph counts and degrees\n";
        cout << "5. Find reachable buildings (BFS)\n";
        cout << "6. Add department\n";
        cout << "7. Search department\n";
        cout << "8. Display department hierarchy\n";
        cout << "9. Run preorder traversal\n";
        cout << "0. Exit\n";
        cout << "\nEnter choice: ";

        string input;
        getline(cin, input);

        stringstream ss(input);
        char extra;

        if (!(ss >> choice) || (ss >> extra)) {
            cout << "Invalid input. Please enter a number.\n";
                        choice = -1;
            continue;
        }

        switch (choice) {
        case 1:
            database.displayAll();
            break;

        case 2:
            searchByID(hashTable);
            break;

        case 3:
            graph.displayGraph();
            break;

        case 4:
            graph.displayGraphCounts();
            break;

        case 5: {
            string start;
            cout << "Enter starting building: ";
            getline(cin, start);
            start = trim(start);
            if (start.empty()) {
                cout << "Name cannot be blank.\n";
                break;
            }
            graph.findReachableBuildings(start);
            break;
        }

        case 6: {
            string name;
            string parentName = "NONE";

            cout << "Enter department name: ";
            getline(cin, name);
            name = trim(name);
            if (name.empty()) {
                cout << "Name cannot be blank.\n";
                break;
            }

            if (!tree.isEmpty()) {
                cout << "Enter parent department: ";
                getline(cin, parentName);
                parentName = trim(parentName);
                if (parentName.empty()) {
                    cout << "Name cannot be blank.\n";
                    break;
                }
            }

            if (tree.addDepartment(name, parentName)) {
                cout << "Department added: " << name << endl;
            }
            else {
                cout << "Could not add " << name
                     << ": duplicate name or unknown parent.\n";
            }
            break;
        }

        case 7: {
            string name;
            cout << "Enter department name: ";
            getline(cin, name);
            name = trim(name);
            if (name.empty()) {
                cout << "Name cannot be blank.\n";
                break;
            }
            tree.searchDepartment(name);
            break;
        }

        case 8:
            tree.displayHierarchy();
            break;

        case 9:
            tree.printPreorder();
            break;

        case 0:
            cout << "Exiting program.\n";
            break;

        default:
            cout << "Invalid menu choice.\n";
        }

    } while (choice != 0);

    return 0;
}
