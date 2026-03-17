#pragma once

template <typename T>
class BaseTree {
public:
    BaseTree() = default;
    BaseTree(BaseTree& other) {
        root = copyNode(other.root);
    }
    virtual BaseTree& operator=(BaseTree& other) {
        if (this == &other)
            return *this;
        delete root;
        root = copyNode(other.root);
        return &this;
    }
    BaseTree(BaseTree&& other) {
        root = other.root;
        other.root = nullptr;
    };
    virtual BaseTree& operator=(BaseTree&& other){
        if (this == &other)
            return *this;
        delete root;
        root = other.root;
        other.root = nullptr;
    }
    virtual ~BaseTree() {
        delete root;
    }

    virtual bool insert(const int& i, const T& value) = 0;
    virtual bool remove(const int& i) = 0;
    virtual T find(const int& i) = 0;
    virtual int height() {
        if (root)
            return root->height;
        return 0;
    };
protected:
    struct Node {
        // these are the values each node could hold. It's a vector because of B trees
        std::vector<std::pair<int,T>> values;
        std::vector<Node*> childrenNodes;
        static const int NUM_CHILDREN;
        Node* parent = nullptr;
        int height = 0;

        Node() {
            values.reserve(NUM_CHILDREN);
        }

        ~Node() {
            for (Node* node : childrenNodes)
                delete node;
        }
    };

    
private:
    Node* root = nullptr;
    // im making this private but if its needed in the future make it protected
    Node* copyNode(Node* other) {
        Node* out = new Node();
        out->values = other->values;
        for (Node* node : other->childrenNodes) {
            out->childrenNodes.push_back(copyNode(node));
            out->childrenNodes.back()->parent = out;
            out->childrenNodes.back()->height = node->height;
        }
        return out;
    }
};
