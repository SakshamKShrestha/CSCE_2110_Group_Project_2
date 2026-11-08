#include "DepartmentTree.h"
#include <cctype>
#include <iostream>
#include <fstream>
static std::string toLowerCopy(const std::string& text)
{
    std::string result = text;
    for (size_t i = 0; i < result.size(); i++)
    {
        result[i] = static_cast<char>(std:: tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

static std::string trimText(const std::string& text)
{
    const std::string whitespace = " \t\r\n";
    size_t start = text.find_first_not_of(whitespace);
    if (start == std::string::npos)
    {
        return "";
    }
    size_t end = text.find_last_not_of(whitespace);
    return text.substr(start, end - start + 1);
}

DepartmentTree::DepartmentTree()
{
    root = nullptr;
}

bool DepartmentTree::isEmpty() const
{
    return root == nullptr;
}


DepartmentNode* DepartmentTree::findHelper(DepartmentNode* current, const std::string& lowerName) const
{
    if (current == nullptr)
    {
        return nullptr;
    }
    if (toLowerCopy(current->name) == lowerName)
    {
        return current;
    }
    for (size_t i = 0; i < current->children.size(); i++)
    {
        DepartmentNode* found = findHelper(current->children[i], lowerName);
        if (found != nullptr)
        {
            return found;
        }
    }
    return nullptr;
}


DepartmentNode* DepartmentTree::findDepartment(const std::string& name) const
{
    return findHelper(root, toLowerCopy(name));
}


bool DepartmentTree::addDepartment(const std::string& name, const std::string& parentName)
{
    if (name.empty())
    {
        return false;
    }
    if (findDepartment(name) != nullptr)
    {
        return false;
    }
    if (isEmpty())
    {
        root = new DepartmentNode(name);
        return true;
    }
    DepartmentNode* parentNode = findDepartment(parentName);
    if (parentNode == nullptr)
    {
        return false;
    }
    DepartmentNode* newNode = new DepartmentNode(name);
    newNode->parent = parentNode;
    parentNode->children.push_back(newNode);
    return true;
}


int DepartmentTree::getDepth(const DepartmentNode* node) const
{
    int depth = 0;
    while (node->parent != nullptr)
    {
        depth++;
        node = node->parent;
    }
    return depth;
}


void DepartmentTree::searchDepartment(const std::string& name) const
{
    DepartmentNode* found = findDepartment(name);
    if (found == nullptr)
    {
        std::cout << "Department not found: " << name << std::endl;
        return;
    }
    std::cout << "Name: " << found->name << std::endl;
    if (found->parent == nullptr)
    {
        std::cout << "Parent: None" << std::endl;
    }
    else
    {
        std::cout << "Parent: " << found->parent->name << std::endl;
    }
    std::cout << "Direct children: " << found->children.size() << std::endl;
    std::cout << "Depth: " << getDepth(found) << std::endl;

}

void DepartmentTree::displayHelper(const DepartmentNode* node, int depth) const
{
    for (int i = 0; i < depth * 4; i++)
    {
        std::cout << " ";
    }
    std::cout << node->name << std::endl;
    for (size_t i = 0; i < node->children.size(); i++)
    {
        displayHelper(node->children[i], depth + 1);
    }
}


void DepartmentTree::displayHierarchy() const
{
    if (isEmpty())
    {
        std::cout << "The department tree is empty." << std::endl;
        return;
    }
    displayHelper(root, 0);
}

void DepartmentTree::preorderHelper(const DepartmentNode* node, bool& isFirst) const
{
    if (!isFirst)
    {
        std::cout << " -> ";
    }
    std::cout << node->name;
    isFirst = false;
    for (size_t i = 0; i < node->children.size(); i++)
    {
        preorderHelper(node->children[i], isFirst);
    }
}


void DepartmentTree::printPreorder() const
{
    if (isEmpty())
    {
        std::cout << "The department tree is empty." << std::endl;
        return;
    }
    bool isFirst = true;
    preorderHelper(root, isFirst);
    std::cout << std::endl;
}

void DepartmentTree::loadFromFile(const std::string& filePath, const std::string& fileName)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Warning: could not open " << fileName << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (std::getline(file, line))
    {
        lineNumber++;
        std::string trimmed = trimText(line);
        if (trimmed.empty() || trimmed[0] == '#')
        {
            continue;
        }

        std::string reason = "";
        size_t bar = trimmed.find('|');
        if (bar == std::string::npos || trimmed.find('|', bar + 1) != std::string::npos)
        {
            reason = "wrong number of fields";
        }
        else
        {
            std::string name = trimText(trimmed.substr(0, bar));
            std::string parentName = trimText(trimmed.substr(bar + 1));
            if (name.empty() || parentName.empty())
            {
                reason = "empty field";
            }
            else if (findDepartment(name) != nullptr)
            {
                reason = "duplicate department name";
            }
            else if (isEmpty() && toLowerCopy(parentName) != "none")
            {
                reason = "first department must be the root with parent NONE";
            }
            else if (!isEmpty() && findDepartment(parentName) == nullptr)
            {
                reason = "unknown parent department";
            }
            else
            {
                addDepartment(name, parentName);
            }
        }

        if (reason.empty())
        {
            accepted++;
        }
        else
        {
            rejected++;
            std::cout << "Warning: " << fileName << ", line " << lineNumber << ": " << reason << std::endl;
        }
    }

    std::cout << fileName << ": " << accepted << " accepted, " << rejected << " rejected" << std::endl;
}