// A lot of this code was "inspired" by I (michael capelli's) project 1 code

#pragma once
#include "BaseTree.hpp"

template <typename T>
class AVLTree : public BaseTree<T> {
public:
    AVLTree() = default;

    bool insert(int id, T item) override {
        Node* toInsert = new Node();
        toInsert->values.push_back({id, item});
        toInsert->childrenNodes.assign(2,nullptr); // i greatly dislike how this looks

        if (root == nullptr) {
            root = toInsert;
        }else {
            Node* current = root;
            while (current != nullptr) {
                if (current->values.front().first == id) {
                    delete toInsert;
                    return false;
                }
                if (id > current->values.front().first) {
                    if (current->childrenNodes[1] == nullptr) {
                        current->childrenNodes[1] = toInsert;
                        toInsert->parent = current;
                        current = nullptr;
                    }else
                        current = current->childrenNodes[1];
                }
                else{
                    if (current->childrenNodes[0] == nullptr) {
                        current->childrenNodes[0] = toInsert;
                        toInsert->parent = current;
                        current = nullptr;
                    }else
                        current = current->childrenNodes[0];
                }
            }
        }
        // traverse down the tree, insert it then call this function to traverse from the inserted node up to root
        balanceTree(toInsert);
        return true;
    }

    // removal logic, there is no balancing after removal, just bst removal
    bool remove(int id) override {
        Node* node = findNode(id);
        Node* nodeHeightToUpdate;
        if (node == nullptr)
            return false;

        if (node->childrenNodes[0] && node->childrenNodes[1]) {
            // set replacement as inorder successor
            Node* replacement = node->childrenNodes[1];

            while (replacement->childrenNodes[0])
                replacement = replacement->childrenNodes[0];

            // from this guy up I need to update the height
            nodeHeightToUpdate = replacement->parent;

            if (replacement != node->childrenNodes[1]) {
                replacement->parent->childrenNodes[0] = replacement->childrenNodes[1];
                if (replacement->childrenNodes[1]) // before the pointer to replacement->childrenNodes[1] is changed i need to set it's new parent
                    replacement->childrenNodes[1]->parent = replacement->parent;
                replacement->childrenNodes[1] = node->childrenNodes[1];

                if (replacement->childrenNodes[1])
                    replacement->childrenNodes[1]->parent = replacement;
            }else // if the above if statement is false then nodeHeightToUpdate is going to be deleted
                nodeHeightToUpdate = replacement;

            replacement->childrenNodes[0] = node->childrenNodes[0];
            if (replacement->childrenNodes[0])
                replacement->childrenNodes[0]->parent = replacement;

            // making sure the parent has updated child
            if (node->parent) {
                replacement->parent = node->parent;
                if (node->parent->childrenNodes[0] == node)
                    node->parent->childrenNodes[0] = replacement;
                else
                    node->parent->childrenNodes[1] = replacement;
            }else {
                root = replacement;
                replacement->parent = nullptr;
            }

            node->childrenNodes[1] = nullptr;
            node->childrenNodes[0] = nullptr;
            delete node;
        }else {
            // this is the removal case that there is 0 - 1 children
            Node* replacement;
            nodeHeightToUpdate = node->parent;

            // only one of these is true and if neither is then this node is a leaf
            if (node->childrenNodes[0])
                replacement = node->childrenNodes[0];
            else if (node->childrenNodes[1])
                replacement = node->childrenNodes[1];
            else
                replacement = nullptr;

            if (node->parent) {
                if (node->parent->childrenNodes[0] == node)
                    node->parent->childrenNodes[0] = replacement;
                else
                    node->parent->childrenNodes[1] = replacement;
            }else {
                root = replacement;
            }

            if (replacement)
                replacement->parent = node->parent;

            node->childrenNodes[0] = nullptr;
            node->childrenNodes[1] = nullptr;
            delete node;
        }
        // this travels up the tree to make sure that the heights are updated correctly.
        // This makes sure that rotations and height calls are correct
        updateHeight(nodeHeightToUpdate);
        return true;
    }
    T find(int id) override {
        Node* node = findNode(id);
        if (node != nullptr) {
            successfulSearch = true;
            return node->values.front().second;
        }
        successfulSearch = false;
        return T();
    }
    int height() override {return BaseTree<T>::height();}
protected:
    // making each access to the parents members less verbose
    using typename BaseTree<T>::Node;
    using BaseTree<T>::successfulSearch;
    using BaseTree<T>::root;

    Node* findNode(int id) {
        Node* current = root;
        while (current != nullptr) {
            if (current->values.front().first == id)
                return current;

            if (id > current->values.front().first)
                current = current->childrenNodes[1];
            else
                current = current->childrenNodes[0];
        }
        return current;
    }
    // starts at the passed node and traverses up the tree checking for any rotations that need to be done
    // if it finds an imbalance it calls the according helper function for that rotation case
    void balanceTree(Node* node) {
        while (node) {
            int balanceFactor = (node->childrenNodes[0] ? node->childrenNodes[0]->height : 0) - (node->childrenNodes[1] ? node->childrenNodes[1]->height : 0);

            // balance factors for the two children. this feels very suboptimal because your recalculating the same numbers a few times.
            // I still think that having the heights instead of the balance factors is the better option
            int leftBF = node->childrenNodes[0] ? (node->childrenNodes[0]->childrenNodes[0] ? node->childrenNodes[0]->childrenNodes[0]->height : 0) - (node->childrenNodes[0]->childrenNodes[1] ? node->childrenNodes[0]->childrenNodes[1]->height : 0) : 0;
            int rightBF = node->childrenNodes[1] ? (node->childrenNodes[1]->childrenNodes[0] ? node->childrenNodes[1]->childrenNodes[0]->height : 0) - (node->childrenNodes[1]->childrenNodes[1] ? node->childrenNodes[1]->childrenNodes[1]->height : 0) : 0;

            if (balanceFactor > 1 && leftBF >= 0)
                node = rotateLeftLeft(node);
            else if (balanceFactor > 1)
                node = rotateLeftRight(node);
            else if (balanceFactor < -1 && rightBF <= 0)
                node = rotateRightRight(node);
            else if (balanceFactor < -1)
                node = rotateRightLeft(node);

            // this makes sure that after rotation the parents left and right pointers point to the right places
            if ((balanceFactor < -1 || balanceFactor > 1) && node->parent) {
                if (node->parent->values.front().first > node->values.front().first)
                    node->parent->childrenNodes[0] = node;
                else
                    node->parent->childrenNodes[1] = node;
            }else //if (node->parent == nullptr)
                root = node;

            // if there was a rotation then we need to check the height
            if (balanceFactor < -1 || balanceFactor > 1) {
                if (node->childrenNodes[1])
                    node->childrenNodes[1]->height  = std::max(node->childrenNodes[1]->childrenNodes[1] ? node->childrenNodes[1]->childrenNodes[1]->height : 0, node->childrenNodes[1]->childrenNodes[0] ? node->childrenNodes[1]->childrenNodes[0]->height : 0) + 1;
                if (node->childrenNodes[0])
                    node->childrenNodes[0]->height = std::max(node->childrenNodes[0]->childrenNodes[0] ? node->childrenNodes[0]->childrenNodes[0]->height : 0, node->childrenNodes[0]->childrenNodes[1] ? node->childrenNodes[0]->childrenNodes[1]->height : 0) +1 ;
            }

            //should always update this nodes height
            node->height = std::max(node->childrenNodes[0] ? node->childrenNodes[0]->height : 0, node->childrenNodes[1] ? node->childrenNodes[1]->height : 0) + 1;
            node = node->parent;
        }
    }

    // all the different rotation cases
    Node* rotateRightRight(Node* node) {
        Node* out = node->childrenNodes[1];
        out->parent = node->parent;
        node->parent = out;
        node->childrenNodes[1] = out->childrenNodes[0];
        out->childrenNodes[0] = node;
        if (node->childrenNodes[1])
            node->childrenNodes[1]->parent = node;
        return out;
    }
    Node* rotateLeftLeft(Node* node) {
        Node* out = node->childrenNodes[0];
        out->parent = node->parent;
        node->parent = out;
        node->childrenNodes[0] = out->childrenNodes[1];
        out->childrenNodes[1] = node;
        if (node->childrenNodes[0])
            node->childrenNodes[0]->parent = node;
        return out;
    }
    Node* rotateRightLeft(Node* node) {
        Node* out = node->childrenNodes[1]->childrenNodes[0];

        out->parent->childrenNodes[0] = out->childrenNodes[1];
        if (out->childrenNodes[1])
            out->childrenNodes[1]->parent = out->parent;
        out->childrenNodes[1] = node->childrenNodes[1];
        node->childrenNodes[1] = out->childrenNodes[0];
        if (out->childrenNodes[0])
            out->childrenNodes[0]->parent = node;
        out->childrenNodes[0] = node;
        out->parent = node->parent;

        out->childrenNodes[0]->parent = out;
        out->childrenNodes[1]->parent = out;

        return out;
    }
    Node* rotateLeftRight(Node* node) {
        Node* out = node->childrenNodes[0]->childrenNodes[1];

        out->parent->childrenNodes[1] = out->childrenNodes[0];
        if (out->childrenNodes[0])
            out->childrenNodes[0]->parent = out->parent;
        out->childrenNodes[0] = node->childrenNodes[0];
        node->childrenNodes[0] = out->childrenNodes[1];
        if (out->childrenNodes[1])
            out->childrenNodes[1]->parent = node;
        out->childrenNodes[1] = node;
        out->parent = node->parent;

        out->childrenNodes[0]->parent = out;
        out->childrenNodes[1]->parent = out;

        return out;
    }
    // will start at the passed node and work its way up the tree updating heights
    // now also updates the number of children too(in a similar way)
    void updateHeight(Node* start) {
        while (start) {
            start->height = std::max(start->childrenNodes[0] ? start->childrenNodes[0]->height : 0, start->childrenNodes[1] ? start->childrenNodes[1]->height : 0) + 1;
            start->subTreeSize = (start->childrenNodes[0] ? start->childrenNodes[0]->subTreeSize : 0) + (start->childrenNodes[1] ? start->childrenNodes[1]->subTreeSize : 0) + 1;
            start = start->parent;
        }
    }
};
