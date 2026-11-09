#include "Facility.h"
#include <iostream>
#include <cctype>

using namespace std;

static string normalize(const string& text)
{
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos)
    {
        return "";
    }
    size_t last = text.find_last_not_of(" \t\r\n");
    string result = text.substr(first, last - first + 1);
    for (size_t i = 0; i < result.size(); i++)
    {
        result[i] = static_cast<char>(tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}


void FacilityDatabase::addFacility(const Facility& facility) {
    facilities.push_back(facility); 
}
void printFacility(const Facility& f) {
    cout << "\nFacility ID: " << f.id
         << "\nFacility Name: " << f.name
         << "\nBuilding: " << f.building
         << "\nCategory: " << f.category
         << "\n-----------------------------\n";
}

void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        cout << "No facility records available.\n";
        return;
    }

    cout << "\n===== ALL FACILITIES =====\n";

    for (const Facility& f : facilities) {
        printFacility(f);
    }

    cout << "Total facilities: " << facilities.size() << "\n";
}
void FacilityDatabase::searchByName(const string& name) const {
    int matches = 0;

    for (const Facility& f : facilities) {
        if (normalize(f.name) == normalize(name)) {
            printFacility(f);
            matches++;
        }
    }
if (matches == 0) {
        cout << "No facilities found with name: "
             << name << "\n";
    }

    cout << "Matches: " << matches << "\n";
}
void FacilityDatabase::searchByCategory(
    const string& category) const {

    int matches = 0;

    for (const Facility& f : facilities) {
        if (normalize(f.category) == normalize(category)) {
            printFacility(f);
            matches++;
        }
    }

    if (matches == 0) {
        cout << "No facilities found in category: "
             << category << "\n";
    }

    cout << "Matches: " << matches << "\n";
}

int FacilityDatabase::getTotal() const {
    return facilities.size();
}

const vector<Facility>& FacilityDatabase::getFacilities() const {
    return facilities;
}