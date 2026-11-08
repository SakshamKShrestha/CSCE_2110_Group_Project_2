#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <string>

using namespace std;

class HashTable {
private:
static const int TABLE_SIZE = 253;

    struct Node {
        Facility data;
        Node* next;

        Node(const Facility& facility) {
            data = facility;
            next = nullptr;
        }
    };

   
    Node* table[TABLE_SIZE];

    int entries;
    int collisions;

   
    
    int hashFunction(const string& id) const;

public:

    // Constructor.
    HashTable();

//destructor 
    ~HashTable();

    
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool insert(const Facility& facility);

    
    Facility* findBuilding(const string& id) const;

    
    int getCapacity() const;
    int getEntries() const;
    int getCollisions() const;
};

#endif