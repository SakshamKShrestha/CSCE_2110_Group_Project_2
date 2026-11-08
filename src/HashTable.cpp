
#include "HashTable.h"
#include <cctype>

using namespace std;

// Converts facility ID to uppercase.
string normalizeID(string id) {
    for (char& c : id) {
        c = toupper(static_cast<unsigned char>(c));
    }

    return id;
}


HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }

    entries = 0;
    collisions = 0;
}


int HashTable::hashFunction(const string& id) const {
    unsigned int hash = 0;

    for (char c : id) {
        hash = (hash * 31 + c) % TABLE_SIZE;
    }

    return hash;
}


bool HashTable::insert(const Facility& facility) {

    Facility newFacility = facility;

    newFacility.id = normalizeID(newFacility.id);

    
    if (findBuilding(newFacility.id) != nullptr) {
        return false;
    }

    int index = hashFunction(newFacility.id);

    
    if (table[index] != nullptr) {
        collisions++;
    }

    Node* newNode = new Node(newFacility);

    newNode->next = table[index];
    table[index] = newNode;

    entries++;

    return true;
}


Facility* HashTable::findBuilding(const string& id) const {

    string key = normalizeID(id);

    int index = hashFunction(key);

    Node* current = table[index];

    while (current != nullptr) {

        if (current->data.id == key) {
            return &(current->data);
        }

        current = current->next;
    }

    return nullptr;
}


int HashTable::getCapacity() const {
    return TABLE_SIZE;
}


int HashTable::getEntries() const {
    return entries;
}


int HashTable::getCollisions() const {
    return collisions;
}

// Destructor.
HashTable::~HashTable() {

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node* current = table[i];

        while (current != nullptr) {

            Node* temp = current;

            current = current->next;

            delete temp;
        }
    }
}
