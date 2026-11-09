Smart Campus Navigation and Facility Analytics System (CSCE 2110, Project 2, Milestone 1)

PURPOSE
A C++ program that loads campus data from text files and lets the user work
with it through a menu: facility records (custom hash table), a campus graph
(BFS), and a department tree.

BUILD (g++ with C++17)
g++ -std=c++17 -Iinclude src/main.cpp src/Facility.cpp src/HashTable.cpp src/CampusGraph.cpp src/DepartmentTree.cpp -o campus

RUN
./campus                      uses the default data directory, data/
./campus testdata/<folder>    uses a different data directory

DATA FILES (loaded in this order from the data directory)
buildings.txt, connections.txt, facilities.txt, departments.txt

DIRECTORY LAYOUT
data/       the four instructor data files
include/    header files
src/        implementation files, including main.cpp
testdata/   our own test files
README.txt, CONTRIBUTIONS.txt, design_document.txt, test_results.txt, .gitignore