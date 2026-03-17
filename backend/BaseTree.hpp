#pragma once
#include <filesystem>
#include <vector>
#include <pair>
#include <utility>

template <typename T>
class BaseTree
{
public:
    BaseTree() = default;
    BaseTree(BaseTree &other)
    {
        head = copyNode(other.head);
    }
    virtual BaseTree &operator=(BaseTree &other)
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
        return *this; // changed from &this to *this
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
        return *this; // make return types consistent, if if statement is triggered you return *this else you dont return anything.
    }
    virtual ~BaseTree()
    {
        delete head;
    }

    virtual void insert(int i, T value) = 0;
    virtual void remove(int i) = 0;
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
    Node *copyNode(Node *other)
    {
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
};
