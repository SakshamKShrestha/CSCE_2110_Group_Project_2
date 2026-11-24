Smart Campus Navigation and Facility Analytics System (CSCE 2110, Project 2, Final)

PURPOSE
A C++ program that loads campus data from text files and lets the user work
with it through a menu: facility records (custom hash table), a campus graph
(BFS), and a department tree.

MODULE OVERVIEW
Facilities (Module A): Facility and HashTable store the campus facility records in a
custom hash table. They support lookup by ID and searches by name and category.
Campus graph (Module B): CampusGraph stores the buildings and their connections. It
supports BFS reachability, shortest path navigation and a connectivity report.
Department tree (Module C): DepartmentTree stores the department hierarchy as a general
tree. It supports adding and searching departments, the indented hierarchy display,
preorder, postorder and level-order traversals, and department statistics.
main.cpp loads the four data files in order and runs an 18 option menu that connects
all three modules.

DEPARTMENT TREE (Module C)
The department hierarchy is a general tree. Each node stores a department name, a pointer
to its parent and a vector of child nodes, so a department can have any number of
sub-departments. Most operations are recursive. Level-order traversal uses a queue. The
statistics report shows the total departments, the leaf departments and the tree height
(counted in edges, so a root-only tree has height 0). The departments.txt loader skips
blank and comment lines, trims spaces, and warns about duplicate names and lines with
missing or empty fields. The destructor frees every node.

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