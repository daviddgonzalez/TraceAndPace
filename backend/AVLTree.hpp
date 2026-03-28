// A lot of this code was "inspired" by I (michael capelli's) and (david gonzalez's) project 1 code

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

    // removal logic, I absolutely mogged michael's code
     bool remove(int g) override{ // successful/unsuccessful
        if(removeHelper(root, g)) {
            return true;
        }
        else {
            return false;
        }
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

    std::pair<bool, typename BaseTree<T>::Node* > search(int id)override {
        Node* node = findNode(id);
        if (node != nullptr) {
            return {true,node};
        }
        return{false,nullptr};
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
    void updateHeight(Node* start) {
        while (start) {
            start->height = std::max(start->childrenNodes[0] ? start->childrenNodes[0]->height : 0, start->childrenNodes[1] ? start->childrenNodes[1]->height : 0) + 1;
            start = start->parent;
        }
    }



    //THIS IS AWESOME DAVID AVL TREE CODE
    bool removeHelper(Node*& r, const int& g) {
        if (r == nullptr){return false;}

        bool success = false;
        if (g < r->values.front().first) {
            success = (removeHelper(r->childrenNodes[0],g));
        }
        else if (g > r->values.front().first) {
            success =(removeHelper(r->childrenNodes[1], g));
        }
        else {
            success = true;

            //4 Cases when node is alr found
            // 1. Node has no kids (easy)
            if (r->childrenNodes[0] == nullptr && r->childrenNodes[1] == nullptr) {
                delete r;
                r = nullptr;
            }
            // 2. Node has left kid
            else if (r->childrenNodes[1] == nullptr) {
                Node* temp = r;
                r = r->childrenNodes[0]; // updates parent bc we passed in by reference not value
                delete temp; // r doesnt store actual value of r now
            }

            //3. Node has right kid
            else if (r->childrenNodes[0] == nullptr) { //mirrored case of 2
                Node* temp = r;
                r = r->childrenNodes[1];
                delete temp;
            }

            // 4. Node has two kids (have to find the inorder succession)
            else {
                Node* inOrderSuccessor = r->childrenNodes[1];
                while (inOrderSuccessor->childrenNodes[0]) {
                    inOrderSuccessor = inOrderSuccessor->childrenNodes[0]; //is now the left most thing on the right subtree.
                }
                r->values.front().first = inOrderSuccessor->values.front().first;
                r->values.front().second = inOrderSuccessor->values.front().second;

                removeHelper(r->childrenNodes[1], r->values.front().first); // we have to pass r->childrenNodes[1] and have it go thru the
                // tree bc if we passed successor it would update the successor pointer and not the tree's pointer
                //have to pass r->values.front().first because inOrder is going to be deleted so it would have a dangling reference
            }
        }
        if (!success || r == nullptr) {
            return success; // return false if we didnt find node or if it was null bc we dont have to balance in these cases
        }

        r->height = 1 + std::max(heightCheck(r->childrenNodes[0]),heightCheck(r->childrenNodes[1])); // O(log n) bc it cuts the amount of nodes it could
        //visit in half every time it recursively calls

        rotate(r);

        return true;
    }

    int heightCheck(Node* r) {
        if (r == nullptr) {
           return 0;
        }
        return r->height;
    }

     void rotate(Node*& r) {
        if (r == nullptr) {
            return;
        }
        int parentBF = calcBalanceFactor(r);

        if (abs(parentBF) <= 1){ //should never do this but js in case?
            return;
        }
        int rightBF = calcBalanceFactor(r->childrenNodes[1]);;

        int leftBF = calcBalanceFactor(r->childrenNodes[0]);

        int childBF = parentBF == -2 ? rightBF : leftBF; //if tree is right heavy we care about right kid,
                                                        // if tree is left heavy we care about left kid

        if (parentBF == -2) {
            if (childBF <=0) { //right right case
                leftRotation(r);
            }
            else { //right left case
                rightRotation(r->childrenNodes[1]);
                leftRotation(r);
            }
        }
        else {
            if (childBF >= 0) { //left left case
                rightRotation(r);
            }
            else { //left right case
                leftRotation(r->childrenNodes[0]);
                rightRotation(r);
            }
        }
    }

    int calcBalanceFactor(Node* r) {
        if (r==nullptr) {
            return 0;
        }
        int leftHeight = heightCheck(r->childrenNodes[0]);
        int rightHeight = heightCheck(r->childrenNodes[1]);

        return(leftHeight-rightHeight);
    }

    void leftRotation(typename BaseTree<T>::Node*& r) {
        typename BaseTree<T>::Node* rightKid = r->childrenNodes[1]; // should always exist
        r->childrenNodes[1] = rightKid->childrenNodes[0];
        rightKid->childrenNodes[0] = r;

        r->height = 1 + std::max(heightCheck(r->childrenNodes[0]), heightCheck(r->childrenNodes[1]));
        rightKid->height = 1 + std::max(heightCheck(rightKid->childrenNodes[0]), heightCheck(rightKid->childrenNodes[1]));

        r = rightKid;
    }

    void rightRotation(typename BaseTree<T>::Node*& r) {
        typename BaseTree<T>::Node* leftKid = r->childrenNodes[0]; //should always exist
        r->childrenNodes[0] = leftKid->childrenNodes[1];
        leftKid->childrenNodes[1] = r;

        r->height = 1 + std::max(heightCheck(r->childrenNodes[0]),heightCheck(r->childrenNodes[1]));
        leftKid->height = 1 + std::max(heightCheck(leftKid->childrenNodes[0]),heightCheck(leftKid->childrenNodes[1]));


        r = leftKid;
    }    

};
