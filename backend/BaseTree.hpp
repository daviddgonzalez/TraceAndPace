#pragma once
#include <filesystem>
#include <vector>
#include <utility>
#include <iostream>

template <typename T>
class BaseTree
{
public:
    BaseTree() = default;

    BaseTree(const BaseTree &other)
    {
        head = copyNode(other.head);
    }

    virtual BaseTree &operator=(const BaseTree &other)
    {
        if (this == &other)
            return *this;

        delete head;

        if (other.head == nullptr)
        {

            head = nullptr;
            return *this;
        }

        head = copyNode(other.head);
        return (*this);
    }
    BaseTree(BaseTree &&other)
    {

        head = other.head;
        other.head = nullptr;
    };
    virtual BaseTree &operator=(BaseTree &&other)
    {

        if (this == &other)
            return *this;

        delete head;

        head = other.head;
        other.head = nullptr;

        return *this;
    }
    virtual ~BaseTree()
    {
        delete head;
    }

    virtual bool insert(int i, T value) = 0; // bool if successful or not
    virtual bool remove(int i) = 0;          // same for ts one
    virtual T find(int i) = 0;

protected:
    struct Node
    {
        // these are the values each node could hold. It's a vector because of B trees
        std::vector<std::pair<int, T>> values;
        std::vector<Node *> childrenNodes;
        Node *parent = nullptr;

        ~Node()
        {
            for (Node *node : childrenNodes)
                delete node;
        }
    };

    Node *head = nullptr;

private:
    // im making this private but if its needed in the future make it protected
    Node *copyNode(Node *other)
    {
        Node *out = new Node();

        if (other == nullptr)
        {
            return nullptr;
        }

        out->values = other->values; // making this out->value = other->value; out/other are pointers
        for (Node *node : other->childrenNodes)
        {
            out->childrenNodes.push_back(copyNode(node));
            out->childrenNodes.back()->parent = out;
        }

                return out;
    }
};
