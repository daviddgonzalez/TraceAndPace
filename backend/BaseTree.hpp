#pragma once

#include <vector>
#include <utility>
#include "json.hpp" //used from https://github.com/nlohmann/json
#include <fstream>
#include <string>
#include <cmath>
#include <queue>
#include <functional>

#include "InceptaTree.hpp"

using json = nlohmann::json;

template <typename T>
class BaseTree{

protected:

     struct Node{
        // these are the values each node could hold. It's a vector because of B trees
        std::vector<std::pair<int, T>> values;
        std::vector<Node *> childrenNodes;

        // Please dont make numchildren static - every tree should be able to have its own # of children
        // And js let every tree handle it separately
        //  ... fine

        Node *parent = nullptr;
        int height = 0;
        int subTreeSize = 0;

        Node() = default; // michael please dont change it messes with BTree

        ~Node()
        {
            for (Node *node : childrenNodes)
                delete node;
        }
    };


public:
    BaseTree() = default;

    BaseTree(const BaseTree &other){
        root = copyNode(other.root);
    }

    BaseTree &operator=(const BaseTree &other){
        if (this == &other)
            return *this;
        delete root;
        if (other.root == nullptr){
            root = nullptr;
            return *this;
        }
        root = copyNode(other.root);
        return *this; // changed from &this to *this
    }

    BaseTree(BaseTree &&other) noexcept
    {
        root = other.root;
        other.root = nullptr;
    };

    BaseTree &operator=(BaseTree &&other) noexcept
    {
        if (this == &other)
            return *this;
        delete root;
        root = other.root;
        other.root = nullptr;
        return *this;
        // make return types consistent, if if statement is triggered you return *this else you dont return anything.
    }

    virtual ~BaseTree()
    {
        delete root;
    }

    virtual bool insert(int i, T value) = 0;
    virtual bool remove(int i) = 0;
    virtual T find(int i) = 0;
    virtual int height() { return root ? root->height : 0; }


    //These have our old way of loading the ENTIRE tree into memory which is super inefficient
    //The code might be useful later though? So let's keep it for now.

    // std::string treeToJsonString()
    // {
    //     json jsonOfTree = nodetoMiniJson(root);

    //     return (jsonOfTree.dump(2));
    //     // I read the docs, this just adds 2 spaces to make it more readable, change if you need to
    // }

    // bool treeToJsonFile(const std::string &filename)
    // {
    //     std::string stringOfJsonOfTree = treeToJsonString();

    //     std::ofstream writeToJsonFile("../frontend/src/assets/" + filename);
    //     if (writeToJsonFile.is_open())
    //     {
    //         writeToJsonFile << stringOfJsonOfTree;
    //         writeToJsonFile.close();
    //         return true;
    //     }
    //     return false;
    // }

    InceptaTree<T> createInceptaTree(int limitOfDisplayedNodes)
    {
        return inceptaTreeHelper(root, 0, limitOfDisplayedNodes);
    }

    InceptaTree<T> createInceptaSubTree(Node* subRoot, int currDepth, int limitOfDisplayedNodes)
    {
        return inceptaTreeHelper(subRoot, currDepth, limitOfDisplayedNodes);
    }

    std::string getWholeViewJson(int limitOfDisplayedNodes){
       json wholeView = treeCondenser(root, 50);
        return tempTree.dump();
    }

    std::string getSubViewJson(int key, int limit){
        Node* subtreeRoot = this->find(key);
        //later problem
    }

protected:


    bool successfulSearch = false;

    Node *root = nullptr;

private:
    // im making this private but if its needed in the future make it protected
    Node *copyNode(Node *other)
    {
        if (other == nullptr)
        {
            return nullptr;
        }

        Node *out = new Node();

        out->values = other->values; // making this out->value = other->value; out/other are pointers
        for (Node *node : other->childrenNodes)
        {
            out->childrenNodes.push_back(copyNode(node));
            out->childrenNodes.back()->parent = out;
            out->childrenNodes.back()->height = node->height;
        }

        return out;
    }

    json nodetoMiniJson(Node *recursiveNode)
    {
        if (recursiveNode == nullptr)
        {
            return nullptr; // Don't worry this is converted by json library to a json null I think
        }

        json miniJson;

        miniJson["height"] = recursiveNode->height;
        miniJson["values"] = json::array(); // Literally is the equivalent of std::vector for json library btw

        for (const auto &keyAndTemplateVal : recursiveNode->values)
        {
            miniJson["values"].push_back({{"key", keyAndTemplateVal.first}, {"value", keyAndTemplateVal.second}});
        }

        miniJson["children"] = json::array();
        for (Node* kidNode: recursiveNode->childrenNodes)
        {
            if (kidNode != nullptr)
            {
                miniJson["children"].push_back(nodetoMiniJson(kidNode));
            }
        }
        return miniJson;
    }

    json treeCondenser(Node* subtreeRoot, int budgetPerSubTree){
        if(subtreeRoot == nullptr){
            return json(nullptr);
        }

        if(budgetPerSubTree<=0){
             json condensedInceptaNode;

             condensedInceptaNode["key"] = subtreeRoot->values[0].first;
             condensedInceptaNode["value"] = subtreeRoot->values[0].second;

             condensedInceptaNode["condensed"] = true;

             condensedInceptaNode["totalNumOfNodes"] = subtreeRoot->subTreeSize;
             condensedInceptaNode["displayedNodes"] = 1;
             condensedInceptaNode["children"] = json::array(); //You'll fill this when you call getSubtreeView

             return condensedInceptaNode;
        }

        --budgetPerSubTree;

        json expandedInceptaNode;

        expandedInceptaNode["key"] = subtreeRoot->values[0].first;
        expandedInceptaNode["value"] = subtreeRoot->values[0].second;

        expandedInceptaNode["condensed"] = false;

        expandedInceptaNode["totalNumOfNodes"] = subtreeRoot->subTreeSize;
        expandedInceptaNode["children"] = json::array(); //we'll fill this recursively

        int amtDisplayedNodes = 1;


        int totalSubtreeWeight = 0;

        for(Node* kid : subtreeRoot->childrenNodes){
            if(kid!=nullptr){
                totalSubtreeWeight+=kid->subTreeSize;
            }
        }

        int childBudget = 0;
        for(Node* child : subtreeRoot->childrenNodes){
            if(child == nullptr) continue;

            if(totalSubtreeWeight == 0){
                childBudget = 0;
            }
            else{
                childBudget =std::floor((budgetPerSubTree-1)* child->subTreeSize / (double)totalSubtreeWeight); //This is meant to split the budget proportionally to size of subtree
            }

            json childNodeInJson = treeCondenser(child, childBudget);
            if (!childNodeInJson.is_null()) {
                
                amtDisplayedNodes += childNodeInJson["displayedNodes"].get<int>();

                expandedInceptaNode["children"].push_back(childNodeInJson);
            }
        }

        expandedInceptaNode["displayedNodes"] = amtDisplayedNodes;
        return expandedInceptaNode;

    }

    InceptaTree<T> inceptaTreeHelper(Node *subTreeRoot, int currDepth = 0, int limitOfDisplayedNodes = 50){
        if (subTreeRoot == nullptr) {
            return InceptaTree<T>({}, 0, 50);
        }
        int totalNodes = traverseAndCount(subTreeRoot);
        int displayToTotRatio = std::ceil((double)totalNodes / limitOfDisplayedNodes);

        InceptaNode<T> displayRoot(subTreeRoot->values[0], currDepth, totalNodes, subTreeRoot->subTreeSize, false);

        std::vector<InceptaNode<T>> topNodes;
        topNodes.push_back(displayRoot);

        std::function<bool(Node*, Node*)> compBySubTreeSize = [](Node* left, Node* right){
            return left->subTreeSize <right->subTreeSize; //I may have to reverse this dunno which is min/max heap
        };

        std::priority_queue<Node*, std::vector<Node*>, std::function<bool(Node*, Node*)>> maxHeapBySubTreeSize(compBySubTreeSize);

        for(Node* child : subTreeRoot->childrenNodes){
            if(child != nullptr)
                maxHeapBySubTreeSize.push(child);
        }

        while(topNodes.size() < limitOfDisplayedNodes && maxHeapBySubTreeSize.empty() == false){
            Node* nodeBiggestSubtree = maxHeapBySubTreeSize.top();
            maxHeapBySubTreeSize.pop();

            if(nodeBiggestSubtree->subTreeSize == 1){
                InceptaNode<T> temp = expandInceptaNode(nodeBiggestSubtree,currDepth);
                topNodes.push_back(temp);
            }

            else if(nodeBiggestSubtree->subTreeSize + topNodes.size() < limitOfDisplayedNodes){
                InceptaNode<T> temp = expandInceptaNode(nodeBiggestSubtree,currDepth);
                topNodes.push_back(temp);
                for(Node* kid : nodeBiggestSubtree->childrenNodes){
                    if(kid != nullptr)
                        maxHeapBySubTreeSize.push(kid);
                }
            }

            else{
                InceptaNode<T> temp = condenseInceptaNode(nodeBiggestSubtree,currDepth);
                topNodes.push_back(temp);
            }
        }
        while(maxHeapBySubTreeSize.empty() == false){
            topNodes.push_back(condenseInceptaNode(maxHeapBySubTreeSize.top(),currDepth));
            maxHeapBySubTreeSize.pop();
        }

        InceptaTree<T> temp = {topNodes, totalNodes, displayToTotRatio};
        return temp;
    }

    InceptaNode<T> expandInceptaNode(Node* nodeTBE, int depth){ //dont pass it nullptr - what if i do?
        // DONE: Its good :)
        InceptaNode<T> result = {nodeTBE->values[0], depth,nodeTBE->subTreeSize, static_cast<int>(nodeTBE->childrenNodes.size()), false};
        return result;
    }

    InceptaNode<T> condenseInceptaNode(Node* nodeTBE, int depth){ //dont pass it nullptr
        InceptaNode<T> result = {nodeTBE->values[0], depth, nodeTBE->subTreeSize, 0, true};
        return result;
    }


    int traverseAndCount(Node *subtreeRoot)
    {
        if (subtreeRoot == nullptr)
        {
            return 0;
        }

        int count = 1;
        for (Node *child : subtreeRoot->childrenNodes)
        {
            count += traverseAndCount(child);
        }

        return count;
    }


};
