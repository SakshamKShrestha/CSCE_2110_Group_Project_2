#include "Facility.h"
#include <iostream>
#include <cctype>

using namespace std;

void FacilityDatabase::addFacility(const Facility& facility) {
    facilities.push_back(facility);
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