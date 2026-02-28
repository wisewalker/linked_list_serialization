#include "linkedList.h"

#include <cstdint>
#include <regex>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

LinkedList::LinkedList(const std::string& inletFilePath, const std::string& outletFilePath)
{
    if (inletFilePath.empty())
    {
        std::cout << "[!]Input file (inlet.in) path was not supplied!\n";
        return;
    }
    if (outletFilePath.empty())
    {
        std::cout << "[!]Output file (outlet.out) path was not supplied!\n";
        return;
    }

    if (!std::regex_match(inletFilePath, std::regex(R"(^\./([a-zA-Z\d\s\-_ ]+/)+([a-zA-Z\d\s\-_ ]+)\.in$)")))
    {
        std::cout << "[!]Input file (inlet.in) path does not match the pattern!\n";
        m_inletFilePath = "*UNDEFINED*";
        std::exit(-1);
    }
    m_inletFilePath = inletFilePath;

    if (!std::regex_match(outletFilePath, std::regex(R"(^\./([a-zA-Z\d\s\-_ ]+/)+([a-zA-Z\d\s\-_ ]+)\.out$)")))
    {
        std::cout << "[!]Output file (outlet.out) path does not match the pattern!\n";
        m_outletFilePath = "*UNDEFINED*";
        std::exit(-1);
    }
    m_outletFilePath = outletFilePath;

    std::cout << "[!]File paths were successfully set: input - \"" << m_inletFilePath << "\", output - \""
        << m_outletFilePath << "\"" << std::endl;
}

void LinkedList::setInletFilePath(const std::string& filePath)
{
    if (filePath.empty())
    {
        std::cout << "[!]Input file (inlet.in) path was not supplied!\n";
        clearList();
        std::exit(-1);
    }

    if (!std::regex_match(filePath, std::regex(R"(^\./([a-zA-Z\d\s\-_ ]+/)+([a-zA-Z\d\s\-_ ]+)\.in$)")))
    {
        std::cout << "[!]Input file (inlet.in) path does not match the pattern, changes were NOT applied!\n";
        clearList();
        std::exit(-1);
    }
    m_inletFilePath = filePath;

    std::cout << "[!]File path was successfully set: input - \"" << m_inletFilePath << "\"" << std::endl;
}

void LinkedList::setOutletFilePath(const std::string& filePath)
{
    if (filePath.empty())
    {
        std::cout << "[!]Outpit file (outlet.out) path was not supplied!\n";
        clearList();
        std::exit(-1);
    }

    if (!std::regex_match(filePath, std::regex(R"(^\./([a-zA-Z\d\s\-_ ]+/)+([a-zA-Z\d\s\-_ ]+)\.out$)")))
    {
        std::cout << "[!]Output file (outlet.out) path does not match the pattern, changes were NOT applied!\n";
        clearList();
        std::exit(-1);
    }
    m_outletFilePath = filePath;

    std::cout << "[!]File path was successfully set: output - \"" << m_outletFilePath << "\"" << std::endl;
}

const std::string& LinkedList::getInletFilePath() const
{
    return m_inletFilePath;
}

const std::string& LinkedList::getOutletFilePath() const
{
    return m_outletFilePath;
}

void LinkedList::fillList()
{
    //to clear earlier created
    clearList();

    if (m_inletFilePath == "*UNDEFINED*")
    {
        std::cout << "[!]Input file (inlet.in) path is undefined!\n";
        clearList();
        std::exit(-1);
    }

    std::ifstream inletFile(m_inletFilePath);
    if (!inletFile.is_open())
    {
        std::cout << "[!]Cannot open input file (inlet.in) with supplied file path, please check the path!\n";
        clearList();
        std::exit(-1);
    }

    std::cout << "[!]Init retreiving nodes data and creating nodes:\n";
    bool bFirstNode = true;
    ListNode* previousNode = nullptr;
    ListNode* currentNode = nullptr;
    while (!inletFile.eof())
    {
        //With each iteration extract data for current node
        std::string enteredNodeData;
        std::getline(inletFile, enteredNodeData);
        auto [nodeData, randIndex] = retreiveNodeData(enteredNodeData);

        //Save index for each Node to apply it later
        m_nodeRandIndicies.push_back(randIndex);

        //Create and configure new Node
        //[1]Save current node as a Previous for next iteration
        previousNode = currentNode;
        //[2]Create new Node (Current) (+supply connection, like previous<-----current):
        currentNode = createNode(previousNode, nodeData);
        //[3]Supply connection, like previous----->current with 2 possible execution branches:
        //1)HEAD which has NO node before it
        if (bFirstNode)
        {
            //assign HEAD (to keep it for future use)
            HEAD = currentNode;
            bFirstNode = false;
        }
        //2)Middle node with previous Node referring to it
        else
        {
            previousNode->next = currentNode;
        }
    }
    //assign TAIL (to keep it for future use)
    TAIL = currentNode;

    //close file (finished reading it)
    inletFile.close();
    std::cout << "[!]Completed retreiving nodes data and creating nodes!\n";

    //assign rand nodes
    auto CURR = HEAD;
    std::vector<int>::iterator randIndex = m_nodeRandIndicies.begin();
    while (CURR)
    {
        int stepsNum = *randIndex;
        if (stepsNum == -1)
            CURR->rand = nullptr;
        else
            CURR->rand = (*this)[stepsNum];

        CURR = CURR->next;
        ++randIndex;
    }

    std::cout << "[!]List was successfully filled with nodes!" << std::endl;
}

void LinkedList::clearList()
{
    if (HEAD == nullptr)
    {
        std::cout << "[!]List is already cleared!\n";
        return;
    }

    std::cout << "[!]Init clearing the list:\n";
    //clear list contents
    auto CURR = HEAD;
    while (CURR)
    {
        std::cout << "\tNode (" << CURR->data << ") is being deleted!\n";
        if (CURR->prev)
            delete CURR->prev;
        CURR = CURR->next;
    }

    //null head and tail ptrs
    HEAD = nullptr;
    TAIL = nullptr;
    //clear rand node indicies data
    m_nodeRandIndicies.clear();

    m_nodesNumber = 0;

    std::cout << "[!]List is successfully cleared!\n";
}

void LinkedList::serializeList()
{
    if (m_outletFilePath == "*UNDEFINED*")
    {
        std::cout << "[!]Output file path is undefined!\n";
        clearList();
        std::exit(-1);
    }

    std::ofstream outletFile(m_outletFilePath, std::ios::binary);
    if (!outletFile.is_open())
    {
        std::cout << "[!]Cannot open output file!\n";
        clearList();
        std::exit(-1);
    }

    auto CURR = HEAD;
    int randIndex = 0;

    std::cout << "[!]Init serialization process:\n";
    while (CURR)
    {
        //max data length <= 1000
        std::uint16_t dataLength = CURR->data.length();
        outletFile.write(reinterpret_cast<const char*>(&dataLength), sizeof(dataLength));

        outletFile.write(CURR->data.c_str(), dataLength);

        //max nodes(tuples) num <= 10^6 (1'000'000)
        std::int32_t randValue = m_nodeRandIndicies[randIndex++];
        outletFile.write(reinterpret_cast<const char*>(&randValue), sizeof(randValue));

        std::cout << "\tNode data pair(" << CURR->data << "-->" << randValue << ") was successfully serialized!\n";

        CURR = CURR->next;
    }
    std::cout << "[!]Serialization process is completed!\n";
}

void LinkedList::deserializeList(const std::string& enterSerializedListFilePath)
{
    //provide function with opportunity to redefine the serialized list file (outlet.out)
    if (!enterSerializedListFilePath.empty())
        m_outletFilePath = enterSerializedListFilePath;

    if (m_outletFilePath == "*UNDEFINED*")
    {
        std::cout << "[!]Output file path is undefined!\n";
        clearList();
        std::exit(-1);
    }
    if (!std::regex_match(m_outletFilePath, std::regex(R"(^\./([a-zA-Z\d\s\-_ ]+/)+([a-zA-Z\d\s\-_ ]+)\.out$)")))
    {
        std::cout << "[!]Output file (outlet.out) path does not match the pattern, changes were NOT applied!\n";
        clearList();
        std::exit(-1);
    }

    std::ifstream outletFile(m_outletFilePath, std::ios::binary);
    if (!outletFile.is_open())
    {
        std::cout << "[!]Cannot open output file!\n";
        clearList();
        std::exit(-1);
    }

    //clear current list
    clearList();

    std::cout << "[!]Init deserialization process:\n";
    //start deserializing and retrieving list nodes from binary file
    auto nodesData = new std::vector<std::string>;
    while (!outletFile.eof())
    {
        //retreive data length
        std::uint16_t dataLength;
        if (!outletFile.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength)))
            break;

        //retreive data
        std::string data;
        data.resize(dataLength);
        if (!outletFile.read(&data[0], dataLength))
            break;

        //retreive rand node index
        std::int32_t randNodeIndex;
        if (!outletFile.read(reinterpret_cast<char*>(&randNodeIndex), sizeof(randNodeIndex)))
            break;

        nodesData->push_back(data);
        m_nodeRandIndicies.push_back(randNodeIndex);
        std::cout << "\tNode data pair(" << data << "-->" << randNodeIndex << ") was successfully deserialized!\n";
    }
    std::cout << "[!]Deserialization process is completed!\n";

    fillList(nodesData);
}

void LinkedList::printListForward()
{
    std::cout << "[!]Print list forward:\n";
    auto CURR = HEAD;
    while (CURR)
    {
        std::cout << "\tNode(" << CURR << "-->" << CURR->data << ", rand_linked: "
            << CURR->rand << "-->" << (CURR->rand ? CURR->rand->data : "empty") << ")\n";

        CURR = CURR->next;
    }
    std::cout << std::endl;
}

void LinkedList::printListBackward()
{
    std::cout << "[!]Print list backward:\n";
    auto CURR = TAIL;
    while (CURR)
    {
        std::cout << "\tNode(" << CURR << "-->" << CURR->data << ", rand_linked: "
            << CURR->rand << "-->" << (CURR->rand ? CURR->rand->data : "empty") << ")\n";

        CURR = CURR->prev;
    }
    std::cout << std::endl;
}

const ListNode* LinkedList::operator[](int targetNodeIndex) const
{
    auto CURR = HEAD;
    for (int currentNodeIndex = 0; currentNodeIndex < targetNodeIndex; ++currentNodeIndex)
        CURR = CURR->next;

    return CURR;
}

ListNode* LinkedList::operator[](int targetNodeIndex)
{
    auto CURR = HEAD;
    for (int currentNodeIndex = 0; currentNodeIndex < targetNodeIndex; ++currentNodeIndex)
        CURR = CURR->next;

    return CURR;
}

LinkedList::~LinkedList()
{
    clearList();
}

ListNode* LinkedList::createNode(ListNode* const previousNode, const std::string& nodeData)
{
    ++m_nodesNumber;
    std::cout << "\tNode (" << nodeData << ") is being created!\n";
    return new ListNode{ previousNode, nullptr, nullptr, nodeData };
}

std::pair<std::string, int> LinkedList::retreiveNodeData(const std::string& enteredNodeData)
{
    //Retrieve Data contents for Node
    std::regex reg_exp(R"(^[^;]{1,1000})");
    std::smatch retreivedNodeData;
    std::regex_search(enteredNodeData, retreivedNodeData, reg_exp);
    std::string nodeData{ retreivedNodeData[0] };

    //Retrieve rand index for Node
    reg_exp = std::regex(R"([^;]+$)");
    std::regex_search(enteredNodeData, retreivedNodeData, reg_exp);
    int randNodeIndex = 0;
    {
        std::stringstream strStream;
        strStream << retreivedNodeData[0];
        strStream >> randNodeIndex;
    }

    std::cout << "\tNode data pair(" << nodeData << "-->" << randNodeIndex << ") was successfully retreived!\n";

    return std::make_pair(nodeData, randNodeIndex);
}

void LinkedList::fillList(std::vector<std::string>* nodesData)
{
    bool bFirstNode = true;
    ListNode* previousNode = nullptr;
    ListNode* currentNode = nullptr;
    for (size_t nodeIndex = 0; nodeIndex < nodesData->size(); ++nodeIndex)
    {
        //With each iteration extract data for current node
        std::string nodeData = (*nodesData)[nodeIndex];

        //Create and configure new Node
        //[1]Save current node as a Previous for next iteration
        previousNode = currentNode;
        //[2]Create new Node (Current) with 2 possible execution branches (+supply connection, like previous<-----current):
        currentNode = createNode(previousNode, nodeData);
        //[3]Supply connection, like previous----->current with 2 possible execution branches:
        //1)HEAD which has NO node before it
        if (bFirstNode)
        {
            //assign HEAD (to keep it for future use)
            HEAD = currentNode;
            bFirstNode = false;
        }
        //2)Middle node with previous Node referring to it
        else
        {
            previousNode->next = currentNode;
        }
    }
    //assign TAIL (to keep it for future use)
    TAIL = currentNode;

    //release the temporarily allocated nodes data
    delete nodesData;

    std::cout << "[!]Start filling the list:\n";
    //assign rand nodes
    auto CURR = HEAD;
    std::vector<int>::iterator randIndex = m_nodeRandIndicies.begin();
    while (CURR)
    {
        int stepsNum = *randIndex;
        if (stepsNum == -1)
            CURR->rand = nullptr;
        else
            CURR->rand = (*this)[stepsNum];

        std::cout << "\tNode data pair(" << CURR->data << "-->" << stepsNum << ") was successfully appended and configured!\n";
        CURR = CURR->next;
        ++randIndex;
    }

    std::cout << "[!]List was successfully filled with nodes (being deserialized earlier)!" << std::endl;
    printFileStats();
}

void LinkedList::printFileStats()
{
    std::ifstream inletFile(m_inletFilePath);
    if (!inletFile.is_open())
    {
        std::cout << "[!]Cannot open input file (inlet.in) with supplied file path, please check the path!\n";
        return;
    }
    inletFile.close();

    std::cout << "[!]Print input/output files statistics data:\n";
    std::cout << "\tInput file (inlet.in) size: " << std::filesystem::file_size(m_inletFilePath) << " bytes\n";
    std::cout << "\tOutput file (outlet.out) size: " << std::filesystem::file_size(m_outletFilePath) << " bytes\n";
}