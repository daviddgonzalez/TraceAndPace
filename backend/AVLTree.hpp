#pragma once
#include <algorithm>
#include <queue>
#include <utility>
#include "BaseTree.hpp"

// This is a typed implementation of an AVL tree, the special restrictions on the tree are done using GatorTree
template <typename T>
class AVLTree : BaseTree<T> {
public:

    // I need the big five because i have heap allocated memory here
    AVLTree(const AVLTree& other) {
        if (other.root == nullptr)
            return;

        std::queue<Node*> nodes;

        nodes.push(other.root);

        while (!nodes.empty()) {
            insert(nodes.front()->id, nodes.front()->value);

            if (nodes.front()->left)
                nodes.push(nodes.front()->left);
            if (nodes.front()->right)
                nodes.push(nodes.front()->right);

            nodes.pop();
        }
    }
    AVLTree(AVLTree&& other) noexcept{
        root = other.root;
        other.root = nullptr;
    }
    AVLTree& operator=(const AVLTree& other) {
        if (&other == this)
            return *this;

        delete root;
        root = nullptr;

        if (other.root == nullptr)
            return *this;

        std::queue<Node*> nodes;

        nodes.push(other.root);

        while (!nodes.empty()) {
            insert(nodes.front()->id, nodes.front()->value);

            if (nodes.front()->left)
                nodes.push(nodes.front()->left);
            if (nodes.front()->right)
                nodes.push(nodes.front()->right);

            nodes.pop();
        }
        successfulSearch = false;

        return *this;
    }
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this == &other)
            return *this;

        delete root;
        root = other.root;
        other.root = nullptr;

        successfulSearch = false;

        return *this;
    }
    ~AVLTree() {
        delete root;
    }

    bool insert(const int& id, const T& item) {
        Node* toInsert = new Node(id,item);

        if (root == nullptr) {
            root = toInsert;
        }else {
            Node* current = root;
            while (current != nullptr) {
                if (current->id == id) {
                    delete toInsert;
                    return false;
                }
                if (id > current->id) {
                    if (current->right == nullptr) {
                        current->right = toInsert;
                        toInsert->parent = current;
                        current = nullptr;
                    }else
                        current = current->right;
                }
                else{
                    if (current->left == nullptr) {
                        current->left = toInsert;
                        toInsert->parent = current;
                        current = nullptr;
                    }else
                        current = current->left;
                }
            }
        }
        // traverse down the tree, insert it then call this function to traverse from the inserted node up to root
        balanceTree(toInsert);
        return true;
    }

    // removal logic, there is no balancing after removal, just bst removal
    bool remove(int id) {
        Node* node = findNode(id);
        Node* nodeHeightToUpdate;
        if (node == nullptr)
            return false;

        if (node->left && node->right) {
            // set replacement as inorder successor
            Node* replacement = node->right;

            while (replacement->left)
                replacement = replacement->left;

            // from this guy up I need to update the height
            nodeHeightToUpdate = replacement->parent;

            if (replacement != node->right) {
                replacement->parent->left = replacement->right;
                if (replacement->right) // before the pointer to replacement->right is changed i need to set it's new parent
                    replacement->right->parent = replacement->parent;
                replacement->right = node->right;

                if (replacement->right)
                    replacement->right->parent = replacement;
            }else // if the above if statement is false then nodeHeightToUpdate is going to be deleted
                nodeHeightToUpdate = replacement;

            replacement->left = node->left;
            if (replacement->left)
                replacement->left->parent = replacement;

            // making sure the parent has updated child
            if (node->parent) {
                replacement->parent = node->parent;
                if (node->parent->left == node)
                    node->parent->left = replacement;
                else
                    node->parent->right = replacement;
            }else {
                root = replacement;
                replacement->parent = nullptr;
            }

            node->right = nullptr;
            node->left = nullptr;
            delete node;
        }else {
            // this is the removal case that there is 0 - 1 children
            Node* replacement;
            nodeHeightToUpdate = node->parent;

            // only one of these is true and if neither is then this node is a leaf
            if (node->left)
                replacement = node->left;
            else if (node->right)
                replacement = node->right;
            else
                replacement = nullptr;

            if (node->parent) {
                if (node->parent->left == node)
                    node->parent->left = replacement;
                else
                    node->parent->right = replacement;
            }else {
                root = replacement;
            }

            if (replacement)
                replacement->parent = node->parent;

            node->left = nullptr;
            node->right = nullptr;
            delete node;
        }
        // this travels up the tree to make sure that the heights are updated correctly.
        // This makes sure that rotations and height calls are correct
        updateHeight(nodeHeightToUpdate);
        return true;
    }

    T find(int id) {
        Node* node = findNode(id);
        if (node != nullptr) {
            successfulSearch = true;
            return node->value;
        }
        successfulSearch = false;
        return T();
    }

    std::pair<bool,typename BaseTree<T>::Node*> search(int id) override{
        std::pair<bool, Node*> out;
        Node* node = findNode(id);
        if (node != nullptr) {
            out.first = true;
            out.second = node;
            return out;
        }
        out.first = false;
        out.second = nullptr;
        return out;
    }

    // i use height to check for balancing, this just takes the root height
    // i made a height function to be able to test
    int height() {
        if (root == nullptr)
            return 0;
        return root->height;
    }
private:
    using typename BaseTree<T>::Node;
    using BaseTree<T>::root;
    using BaseTree<T>::successfulSearch;

    // searches for an ID and returns a pointer to the corresponding node
    Node* findNode(int id) {
        Node* current = root;
        while (current != nullptr) {
            if (current->id == id)
                return current;

            if (id > current->id)
                current = current->right;
            else
                current = current->left;
        }
        return current;
    }

    // starts at the passed node and traverses up the tree checking for any rotations that need to be done
    // if it finds an imbalance it calls the according helper function for that rotation case
    void balanceTree(Node* node) {
        while (node) {
            int balanceFactor = (node->left ? node->left->height : 0) - (node->right ? node->right->height : 0);

            // balance factors for the two children. this feels very suboptimal because your recalculating the same numbers a few times.
            // I still think that having the heights instead of the balance factors is the better option
            int leftBF = node->left ? (node->left->left ? node->left->left->height : 0) - (node->left->right ? node->left->right->height : 0) : 0;
            int rightBF = node->right ? (node->right->left ? node->right->left->height : 0) - (node->right->right ? node->right->right->height : 0) : 0;

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
                if (node->parent->id > node->id)
                    node->parent->left = node;
                else
                    node->parent->right = node;
            }else //if (node->parent == nullptr)
                root = node;

            // if there was a rotation then we need to check the height
            if (balanceFactor < -1 || balanceFactor > 1) {
                if (node->right)
                    node->right->height  = std::max(node->right->right ? node->right->right->height : 0, node->right->left ? node->right->left->height : 0) + 1;
                if (node->left)
                    node->left->height = std::max(node->left->left ? node->left->left->height : 0, node->left->right ? node->left->right->height : 0) +1 ;
            }

            //should always update this nodes height
            node->height = std::max(node->left ? node->left->height : 0, node->right ? node->right->height : 0) + 1;
            node = node->parent;
        }
    }

    // all the different rotation cases
    Node* rotateRightRight(Node* node) {
        Node* out = node->right;
        out->parent = node->parent;
        node->parent = out;
        node->right = out->left;
        out->left = node;
        if (node->right)
            node->right->parent = node;
        return out;
    }
    Node* rotateLeftLeft(Node* node) {
        Node* out = node->left;
        out->parent = node->parent;
        node->parent = out;
        node->left = out->right;
        out->right = node;
        if (node->left)
            node->left->parent = node;
        return out;
    }
    Node* rotateRightLeft(Node* node) {
        Node* out = node->right->left;

        out->parent->left = out->right;
        if (out->right)
            out->right->parent = out->parent;
        out->right = node->right;
        node->right = out->left;
        if (out->left)
            out->left->parent = node;
        out->left = node;
        out->parent = node->parent;

        out->left->parent = out;
        out->right->parent = out;

        return out;
    }
    Node* rotateLeftRight(Node* node) {
        Node* out = node->left->right;

        out->parent->right = out->left;
        if (out->left)
            out->left->parent = out->parent;
        out->left = node->left;
        node->left = out->right;
        if (out->right)
            out->right->parent = node;
        out->right = node;
        out->parent = node->parent;

        out->left->parent = out;
        out->right->parent = out;

        return out;
    }
    std::vector<T> printPostOrderHelper(Node* node) {
        if (node == nullptr)
            return {};
        std::vector<T> left = printPostOrderHelper(node->left);
        std::vector<T> right = printPostOrderHelper(node->right);
        left.insert(left.end(), right.begin(), right.end());
        left.push_back(node->value);
        return left;
    }

    // will start at the passed node and work its way up the tree updating heights
    void updateHeight(Node* start) {
        while (start) {
            start->height = std::max(start->left ? start->left->height : 0, start->right ? start->right->height : 0) + 1;
            start->subTreeSize = start->left->subTreeSize + start->right->subTreeSize + 1;
            start = start->parent;
        }
    }
};
