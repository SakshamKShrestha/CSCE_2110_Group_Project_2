#ifndef FACILITY_H
#define FACILITY_H

#include <string>
#include <vector>

using namespace std;

struct Facility {
    string id;
    string name;
    string building;
    string category;
};

class FacilityDatabase {
private:
    vector<Facility> facilities;
    
    public:
    // Adds a facility record.
    void addFacility(const Facility& facility);
    
    void displayAll() const;
    
    void searchByName(const string& name) const;

void searchByCategory(const string& category) const;
int getTotal() const;

const vector<Facility>& getFacilities() const;
};

#endif