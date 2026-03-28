#pragma once
#include "BaseTree.hpp"

template <typename T>
class SplayTree : public BaseTree<T>
{

public:
    using Node = typename BaseTree<T>::Node;

    BaseTree<T>* clone() override {
        return new SplayTree<T>(*this);
    }

    // --- SEARCH ---
    std::pair<bool, Node*> search(int key) override
    {
        Node* curr = this->root;
        Node* last = nullptr;

        while (curr != nullptr) {
            last = curr;
            int currKey = curr->values[0].first;

            if (key == currKey) {
                return {true, curr};
            }
            else if (key < currKey) {
                curr = curr->childrenNodes[0];
            }
            else {
                curr = curr->childrenNodes[1];
            }
        }

        return {false, last}; 
    }

    // --- FIND ---
    T find(int key) override
    {
        std::pair<bool, Node*> result = search(key);

        if (!result.first) {
            throw std::invalid_argument("Key does not exist");
        }

        // I(michael) cant think of a better easy way to calculate splaytree children size
        // but splay tree already has worst case O(n) so if there was a change made i just recalculate
        if (result.second != this->root) {
            splay(result.second);
            BaseTree<T>::calculateSubTreeSize(this->root);
        }

        return result.second->values[0].second;
    }

    // --- INSERT ---
    bool insert(int key, T value) override
    {
        std::pair<bool, Node*> result = search(key);

        if (result.first) {
            return false;
        }

        Node* parent = result.second;

        Node* newNode = new Node();
        newNode->values.push_back({key, value});
        newNode->childrenNodes.resize(2, nullptr);
        newNode->parent = nullptr;

        if (parent == nullptr) {
            this->root = newNode;
            return true;
        }

        if (key < parent->values[0].first) {
            parent->childrenNodes[0] = newNode;
        } else {
            parent->childrenNodes[1] = newNode;
        }

        newNode->parent = parent;

        splay(newNode);
        this->root = newNode;

        // I(michael) cant think of a better easy way to calculate splaytree children size
        // but splay tree already has worst case O(n) so if there was a change made i just recalculate
        BaseTree<T>::calculateSubTreeSize(this->root);

        return true;
    }

    // --- REMOVE ---
    bool remove(int key) override
    {
        std::pair<bool, Node*> result = search(key);

        if (!result.first) {
            return false;
        }

        Node* target = result.second;
        splay(target);
        this->root = target;

        Node* leftSub = target->childrenNodes[0];
        Node* rightSub = target->childrenNodes[1];

        if (leftSub) leftSub->parent = nullptr;
        if (rightSub) rightSub->parent = nullptr;

        delete target;

        if (!leftSub) {
            this->root = rightSub;
            return true;
        }

        // find max of left subtree
        Node* curr = leftSub;
        while (curr->childrenNodes[1]) {
            curr = curr->childrenNodes[1];
        }

        splay(curr);
        curr->childrenNodes[1] = rightSub;
        if (rightSub) rightSub->parent = curr;

        this->root = curr;

        BaseTree<T>::calculateSubTreeSize(this->root);

        return true;
    }

private:

    // --- ROTATIONS ---

    void rotateLeft(Node* x)
    {
        Node* y = x->childrenNodes[1];
        if (!y) return;

        x->childrenNodes[1] = y->childrenNodes[0];
        if (y->childrenNodes[0])
            y->childrenNodes[0]->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            this->root = y;
        else if (x == x->parent->childrenNodes[0])
            x->parent->childrenNodes[0] = y;
        else
            x->parent->childrenNodes[1] = y;

        y->childrenNodes[0] = x;
        x->parent = y;
    }

    void rotateRight(Node* x)
    {
        Node* y = x->childrenNodes[0];
        if (!y) return;

        x->childrenNodes[0] = y->childrenNodes[1];
        if (y->childrenNodes[1])
            y->childrenNodes[1]->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            this->root = y;
        else if (x == x->parent->childrenNodes[0])
            x->parent->childrenNodes[0] = y;
        else
            x->parent->childrenNodes[1] = y;

        y->childrenNodes[1] = x;
        x->parent = y;
    }

    // --- SPLAY ---
    void splay(Node* x)
    {
        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;

            if (!g) {
                // Zig
                if (x == p->childrenNodes[0])
                    rotateRight(p);
                else
                    rotateLeft(p);
            }
            else if (x == p->childrenNodes[0] && p == g->childrenNodes[0]) {
                // Zig-Zig
                rotateRight(g);
                rotateRight(p);
            }
            else if (x == p->childrenNodes[1] && p == g->childrenNodes[1]) {
                // Zig-Zig
                rotateLeft(g);
                rotateLeft(p);
            }
            else if (x == p->childrenNodes[1] && p == g->childrenNodes[0]) {
                // Zig-Zag
                rotateLeft(p);
                rotateRight(g);
            }
            else {
                // Zig-Zag
                rotateRight(p);
                rotateLeft(g);
            }
        }
    }
};