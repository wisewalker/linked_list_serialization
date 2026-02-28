#pragma once

#include <string>
#include <vector>

struct ListNode { // ListNode модифицировать нельзя
    ListNode* prev = nullptr; // указатель на предыдущий элемент или nullptr
    ListNode* next = nullptr; // указатель на следующий элемент или nullptr
    ListNode* rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr` 
    std::string data; // произвольные пользовательские данные
};

class LinkedList
{
private:
    int m_nodesNumber = 0;
    std::string m_inletFilePath = "*UNDEFINED*";
    std::string m_outletFilePath = "*UNDEFINED*";

    ListNode* HEAD = nullptr;
    ListNode* TAIL = nullptr;

    std::vector<int> m_nodeRandIndicies;

public:
    LinkedList(const std::string& inletFilePath = std::string(""), const std::string& outletFilePath = std::string(""));

    void setInletFilePath(const std::string& filePath);

    void setOutletFilePath(const std::string& filePath);

    const std::string& getInletFilePath() const;

    const std::string& getOutletFilePath() const;

    void fillList();

    void clearList();

    void serializeList();

    void deserializeList(const std::string& enterSerializedListFilePath);

    void printListForward();

    void printListBackward();

    const ListNode* operator[](int targetNodeIndex) const;

    ListNode* operator[](int targetNodeIndex);

    ~LinkedList();
private:
    ListNode* createNode(ListNode* const previousNode, const std::string& nodeData);

    std::pair<std::string, int> retreiveNodeData(const std::string& enteredNodeData);

    void fillList(std::vector<std::string>* nodesData);

    void printFileStats();
};