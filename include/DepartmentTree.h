#ifndef DEPARTMENTTREE_H    
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

struct DepartmentNode
{
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;

    DepartmentNode(const std::string& deptName)
    {
        name = deptName;
        parent = nullptr;
    }
};

class DepartmentTree
{
private:
    DepartmentNode* root;
    DepartmentNode* findHelper(DepartmentNode* current, const std::string& name) const;
    void displayHelper(const DepartmentNode* node, int depth) const;
        void preorderHelper(const DepartmentNode* node, bool& isFirst) const;
public:
    DepartmentTree();
    bool isEmpty() const;
    DepartmentNode* findDepartment(const std::string& name)const;
    bool addDepartment(const std::string& name, const std::string& parentName);
    void printPreorder() const;

        int getDepth(const DepartmentNode* node) const;
    void searchDepartment(const std::string& name) const;
    void displayHierarchy() const;
    void loadFromFile(const std::string& filePath, const std::string& fileName);
};
#endif