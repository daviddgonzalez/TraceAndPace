#pragma once
#include <vector>
#include <utility>
#include <stdexcept>
#include "json.hpp" //used from https://github.com/nlohmann/json
#include <fstream>
using json = nlohmann::json;


template <typename T>
class BaseTree {
public:
    BaseTree() = default;

    BaseTree(const BaseTree& other) {
        root = copyNode(other.root);
    }

    BaseTree& operator=(const BaseTree& other) {
        if (this == &other)
            return *this;
        delete root;
        if (other.root == nullptr) {
            root = nullptr;
            return *this;
        }
        root = copyNode(other.root);
        return *this; // changed from &this to *this
    }

    BaseTree(BaseTree&& other) noexcept {
        root = other.root;
        other.root = nullptr;
    };

    BaseTree& operator=(BaseTree&& other) noexcept {
        if (this == &other)
            return *this;
        delete root;
        root = other.root;
        other.root = nullptr;
        return *this;
        // make return types consistent, if if statement is triggered you return *this else you dont return anything.
    }

    virtual ~BaseTree() {
        delete root;
    }

    virtual bool insert(int i, T value) = 0;
    virtual bool remove(int i) = 0;
    virtual T find(int i) = 0;
    virtual int height() { return root ? root->height : 0; }


    std::string treeToJsonToString() {
        json jsonOfTree = nodetoMiniJson(root);

        return(jsonOfTree.dump(2)); //I read the docs, this just adds 2 spaces to make it more readable, change if you need to
    }

    bool treeToJsonToFile() {
        std::string stringOfJsonOfTree = treeToJsonToString(root);

        std::ofstream writeToJsonFile(filename); //INSERT FILE NAME HERE WHEN WE DECIDE WHERE TO PUT
        if (writeToJsonFile.is_open()) {
            writeToJsonFile<<stringOfJsonOfTree;
            writeToJsonFile.close();
            return true;
        }
        return false;

    }

protected:
    bool successfulSearch = false;

    struct Node {
        // these are the values each node could hold. It's a vector because of B trees
        std::vector<std::pair<int, T>> values;
        std::vector<Node*> childrenNodes;

        //Please dont make numchildren static - every tree should be able to have its own # of children
        //And js let every tree handle it separately
        // ... fine

        Node* parent = nullptr;
        int height = 0;

        Node() = default; //michael please dont change it messes with BTree

        ~Node() {
            for (Node* node : childrenNodes)
                delete node;
        }
    };

    Node* root = nullptr;

private:
    // im making this private but if its needed in the future make it protected
    Node* copyNode(Node* other) {
        if (other == nullptr) {
            return nullptr;
        }

        Node* out = new Node();

        out->values = other->values; // making this out->value = other->value; out/other are pointers
        for (Node* node : other->childrenNodes) {
            out->childrenNodes.push_back(copyNode(node));
            out->childrenNodes.back()->parent = out;
            out->childrenNodes.back()->height = node->height;
        }

        return out;
    }

    json nodetoMiniJson(Node* recursiveNode) {

        if (recursiveNode == nullptr) {
            return nullptr; //Don't worry this is converted by json library to a json null I think
        }


        json miniJson;

        miniJson["height"] = recursiveNode->height;
        miniJson["values"] = json::array(); //Literally is the equivalent of std::vector for json library btw

        for (const auto& keyAndTemplateVal : recursiveNode->values) {
            miniJson["values"].push_back({"key", keyAndTemplateVal.first}, {"value", keyAndTemplateVal.second});

        }

        miniJson["children"] = json::array();
        for (Node* childrenNode: recursiveNode->childrenNodes) {
            if (childrenNode != nullptr) {
                miniJson["children"].push_back(nodetoMiniJson(childrenNode));
            }
        }
    }

};
