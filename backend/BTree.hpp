#pragma once
#include "BaseTree.hpp"

template <typename T>
class VisualBTree : public BaseTree<T>
{

public:
    VisualBTree(const int keys = 2) : BaseTree<T>(), nodeKeys(keys), nodeNumChildren(nodeKeys + 1){
        if (nodeKeys <= 1){
            throw std::invalid_argument("B Tree must have at least 2 keys");
        }
    }

    // copy operations, calls Base class to handle copying structure for constructor and assignment.
    VisualBTree(const VisualBTree &other) : BaseTree<T>(other), nodeKeys(other.nodeKeys), nodeNumChildren(other.nodeNumChildren) {}

    VisualBTree &operator=(const VisualBTree &other)
    {
        BaseTree<T>::operator=(other);
        nodeKeys = other.nodeKeys;
        nodeNumChildren = other.nodeNumChildren;
        return *this;
    }

    // move operations, base class takes care of struc
    VisualBTree(VisualBTree &&other) noexcept : BaseTree<T>(std::move(other)), nodeKeys(other.nodeKeys), nodeNumChildren(other.nodeNumChildren) {} // move constructor

    VisualBTree &operator=(VisualBTree &&other) noexcept{
        BaseTree<T>::operator=(std::move(other)); // I'm using std::move to turn other into an rvalue reference to call the move assignment instead of copy assignment.
        nodeKeys = other.nodeKeys;
        nodeNumChildren = other.nodeNumChildren;
        return *this;
    }

    ~VisualBTree(){}; //When I made this destructor I tried expliclity calling base tree destructor - DONT, it'd be a double delete. this implicility calls it // ofc it is david, did you not learn anything about destructors and inheritance in links class - michael


    // search returns if it was successful and a pointer to the node
    // find returns just the value, throws an error if the key doesnt exist

    std::pair<bool, typename BaseTree<T>::Node *> search(int target) // typename here to tell teh compiler node is a struct not a variable, so the * is a pointer not mult
    // std::pair to act as a helper method for insert.
    { // nullptr if doesnt exist, pointer to node if it does exist
        typename BaseTree<T>::Node* curr = this->root;
        while (curr != nullptr){
            std::pair<bool, int> searchResult = binarySearch(curr->values, target);
            if (searchResult.first) {
                return {true, curr};
            }
            if (curr->childrenNodes.size() == 0) {
                // if curr is a leaf and we didnt find then return where it should be inserted
                return {false, curr};
            }
            curr = curr->childrenNodes[searchResult.second]; // if we didnt find the target, go down a level to where it should be.
        }
        return {false, nullptr}; // return nullptr bc the tree itself doesnt exist
    }

    T find(int nodeKey) override
    {

        std::pair<bool, typename BaseTree<T>::Node *> keyResult = search(nodeKey);

        if (!keyResult.first){
            throw std::invalid_argument("Key given does not exist in the tree");
        }

        typename BaseTree<T>::Node* nodePair = keyResult.second;
        std::pair<bool, int> nodeSearch = binarySearch(nodePair->values, nodeKey);

        int resultInd = nodeSearch.second;
        std::vector<std::pair<int, T>> nodeValues = nodePair->values; // could prolly do this in one line, figured its more readable
        std::pair<int, T> result = nodeValues[resultInd];

        return result.second;
    }

    bool insert(int k, T value) override{
        std::pair<bool, typename BaseTree<T>::Node *> treeSearch = search(k);
        // std::cout<< treeSearch.first<<"  " << treeSearch.second<<std::endl; // test search"
        if (treeSearch.first) {
            return false; // if the key already exists we dont insert and return false
        }
        typename BaseTree<T>::Node* current = treeSearch.second;

        if (current == nullptr){ // empty tree case

            this->root = new typename BaseTree<T>::Node();
            this->root->values.push_back({k, value});
            this->root->subTreeSize = 1;

            return true;
        }

        std::pair<bool, int> nodeSearch = binarySearch(current->values, k);
        int insertInd = nodeSearch.second;

        current->values.insert(current->values.begin() + insertInd, {k, value});
        current->subTreeSize++;

        typename BaseTree<T>::Node* incParents = current->parent;

        while(incParents!=nullptr){
            incParents->subTreeSize++;
            incParents = incParents->parent;
        }

        // if the node has too many keys we need to split it
        if (current->values.size() > nodeKeys){
            bTreeSplit(current);
        }

        return true;
    }


    bool remove(int k) override {
        std::pair<bool, typename BaseTree<T>::Node *> targetNodePair = search(k);
        if (targetNodePair.first == false) {
            return false;
        }

        typename BaseTree<T>::Node* targetNode = targetNodePair.second;
        typename BaseTree<T>::Node* parentNode = targetNode->parent;

        int targetPos = binarySearch(targetNode->values,k).second;

        int minBTreeKids = (nodeNumChildren + 1)/2; //I realized std::ceil wont work bc its ceil of(2) eg, so this raises it one
        int minKeys = minBTreeKids - 1;
        int maxBTreeKids = nodeNumChildren; // I wanted to write these explicitly, as for removing it involves both heavily so i js wanted to have a copy
        int maxKeys = nodeKeys;  //really I should rename the tree attributes to maxChildren and maxKeys


        if (targetNode->childrenNodes.size() > 0) { //If its an internal it'll go here
            typename BaseTree<T>::Node * biggestBeforeTarget = targetNode->childrenNodes[targetPos];

            while (biggestBeforeTarget->childrenNodes.size()>0) {
                biggestBeforeTarget = biggestBeforeTarget->childrenNodes.back();
            }

            targetNode->values[targetPos] = biggestBeforeTarget->values.back();

            targetNode = biggestBeforeTarget; //biggestBeforeTarget is a leaf (duh), we'll remove the duplicate key thats in the leaf now
            parentNode = targetNode->parent;
            targetPos = targetNode->values.size()-1;

        }

        typename BaseTree<T>::Node* incParents = targetNode->parent;

        while(incParents!=nullptr){
            incParents->subTreeSize--;
            incParents = incParents->parent;
        }

        //I made it so by the time it gets here its ALWAYS gonna be a leaf node ITS SO PEAK
            //here's leaf node cases
            if (targetNode->values.size() > minKeys) {
                targetNode->values.erase(targetNode->values.begin() + targetPos);
                targetNode->subTreeSize--;
                return true;
            }
            else {
                if (targetNode == this->root) {
                    targetNode->values.erase(targetNode->values.begin() + targetPos);
                    targetNode->subTreeSize--;
                    if (targetNode->values.size() == 0) {
                        delete targetNode;
                        this->root = nullptr;
                    }
                    return true;
                }
                int targetNodeInd = 0; //i'm defining the index of our target node inside the parent's array of kids
                for (int i = 0; i<parentNode->childrenNodes.size();++i) { // i and u are songs by kendrick lamar duckworth
                    if (parentNode->childrenNodes[i] == targetNode) {
                        targetNodeInd = i;
                        break;
                    }
                }
                if (targetNodeInd>0 && parentNode->childrenNodes[targetNodeInd-1]->values.size() > minKeys) {
                    
                    
                    typename BaseTree<T>::Node* leftNode = parentNode->childrenNodes[targetNodeInd-1];
                    targetNode->values.erase(targetNode->values.begin() + targetPos); //I messed up before by just trying to swap, you HAVE to preserve sorted order
                    targetNode->values.insert(targetNode->values.begin(), parentNode->values[targetNodeInd-1]);
                    parentNode->values[targetNodeInd-1] = leftNode->values.back();
                    
                    leftNode->values.pop_back();
                    leftNode->subTreeSize--;
                    return true;

                }
                else if (targetNodeInd < parentNode->childrenNodes.size()-1 && parentNode->childrenNodes[targetNodeInd+1]->values.size() > minKeys) {
                    
                    typename BaseTree<T>::Node* rightNode = parentNode->childrenNodes[targetNodeInd+1];
                    targetNode->values.erase(targetNode->values.begin() + targetPos); //same here as with borrowing from left case
                    targetNode->values.push_back(parentNode->values[targetNodeInd]);

                    parentNode->values[targetNodeInd] = rightNode->values.front();
                    rightNode->values.erase(rightNode->values.begin());

                    rightNode->subTreeSize--;

                    return true;
                }
                else {
                    
                    if (targetNodeInd == parentNode->childrenNodes.size()-1) { //no right child, we have to do left child merge
                        typename BaseTree<T>::Node* leftNode = parentNode->childrenNodes[targetNodeInd-1];
                        leftNode->values.push_back(parentNode->values[targetNodeInd-1]);
                        targetNode->values.erase(targetNode->values.begin() + targetPos); //Lets delete target before copying so we dont have to worry about index shifting.
                        
                        for (int f = 0; f<targetNode->values.size();++f) {
                            leftNode->values.push_back(targetNode->values[f]);
                        }

                        delete targetNode;

                        leftNode->subTreeSize = leftNode->values.size();
                        parentNode->values.erase(parentNode->values.begin() + targetNodeInd-1);
                        parentNode->childrenNodes.erase(parentNode->childrenNodes.begin() + targetNodeInd);
                    }
                   
                    else {
                        typename BaseTree<T>::Node* rightNode = parentNode->childrenNodes[targetNodeInd+1];
                        targetNode->values.push_back(parentNode->values[targetNodeInd]);
                        
                        for (int u = 0; u<rightNode->values.size();++u) {
                            targetNode->values.push_back(rightNode->values[u]);
                       
                        }

                        delete rightNode;
                        rightNode = nullptr; //Just to be redundant

                        parentNode->values.erase(parentNode->values.begin() + targetNodeInd);
                        parentNode->childrenNodes.erase(parentNode->childrenNodes.begin() + targetNodeInd + 1);
                        
                        targetNode->values.erase(targetNode->values.begin() + targetPos); //We didnt shift target pos cuz we added to the end
                        targetNode->subTreeSize = targetNode -> values.size();

                    }

                    if (parentNode == this->root && parentNode->values.size() == 0) { //here's the height shrinking case
                        this->root = parentNode->childrenNodes[0];
                        this->root->parent = nullptr;
                        delete parentNode;
                        return true;
                    }

                    if (parentNode->values.size() < minKeys) {
                        recursiveNodeMergerBS(parentNode);
                    }

                    return true;
            }
        }
    }

private:
    int nodeKeys;
    int nodeNumChildren;
    // number of keys in each node, kids is keys + 1
    // everything else is encapsulated in the base tree

    std::pair<bool, int> binarySearch(std::vector<std::pair<int, T>>& nodeVals, int target){

        int left = 0;
        int right = nodeVals.size() - 1;

        while (left <= right){
            int mid = left + (right - left) / 2; // to avoid overflow, unlikely though

            if(nodeVals[mid].first == target){
                std::pair<bool, int> result = {true, mid}; // true that we found the target and mid is the index
                return result;
            }

            else if (nodeVals[mid].first < target){
                left = mid + 1;
            }

            else{
                right = mid - 1;
            }
        }

        std::pair<bool, int> result = {false, left}; // false we didnt find the target, left is where it should be inserted
        return result;
    }

    void bTreeSplit(typename BaseTree<T>::Node *node){
        int middle = node->values.size() / 2;
        typename BaseTree<T>::Node* rightNode = new typename BaseTree<T>::Node();
        std::pair<int, T> middlePair = node->values[middle];

        // value shiz
        for (int i = middle + 1; i < node->values.size(); ++i){
            rightNode->values.push_back(node->values[i]);
        }

        int amtToRemove = node->values.size() - middle;

        for (int k = 0; k < amtToRemove; k++){
            node->values.pop_back();
        }

        // deal w kids
        if (node->childrenNodes.size() > 0){

            // I'm doing the same thing as the vals, but also updating parent ptr
            for (int u = middle + 1; u < node->childrenNodes.size(); ++u){
                rightNode->childrenNodes.push_back(node->childrenNodes[u]);
                node->childrenNodes[u]->parent = rightNode;
            }

            amtToRemove = node->childrenNodes.size() - (middle + 1);

            for (int q = 0; q < amtToRemove; q++){
                node->childrenNodes.pop_back();
            }
        }

        node->subTreeSize = node->values.size();

        for(typename BaseTree<T>::Node* child: node->childrenNodes){
            node->subTreeSize += child->subTreeSize;
        }

        rightNode->subTreeSize = rightNode->values.size();

        for(typename BaseTree<T>::Node* child: rightNode->childrenNodes){
            rightNode->subTreeSize += child->subTreeSize;
        }

        // I'm using the recursive calls to repeatedly raise the key/value pair

        if (node->parent == nullptr){                                                                           // we have to make a new root
            typename BaseTree<T>::Node *jesus = new typename BaseTree<T>::Node(); // I wrote jesus because he rises

            jesus->values.push_back(middlePair);
            jesus->childrenNodes.push_back(node);
            jesus->childrenNodes.push_back(rightNode);
            jesus->subTreeSize = 1 + node->subTreeSize + rightNode->subTreeSize;

            node->parent = jesus;
            rightNode->parent = jesus;
            this->root = jesus;
        }
        else{
            typename BaseTree<T>::Node* parentofCurr = node->parent;

            rightNode->parent = parentofCurr;
            int insInd = binarySearch(parentofCurr->values, middlePair.first).second;

            parentofCurr->values.insert(parentofCurr->values.begin() + insInd, middlePair);
            parentofCurr->childrenNodes.insert(parentofCurr->childrenNodes.begin() + insInd + 1, rightNode);

            if(parentofCurr->values.size() > nodeKeys){

                bTreeSplit(parentofCurr); // I'm gonna call it recursively if parent overflows
            }
        }
    }

    void recursiveNodeMergerBS(typename BaseTree<T>::Node* targetNode) {
        if (targetNode == this->root) {
            if (targetNode->values.size() == 0 && targetNode->childrenNodes.size() >0) {
                this->root = targetNode->childrenNodes[0];
                this->root->parent = nullptr;
                delete targetNode;
            }
            return;
        }

        int minBTreeKids = (nodeNumChildren + 1)/2; //recycled from original remove func
        int minKeys = minBTreeKids - 1;

        if (targetNode->values.size() >= minKeys) {
            return;
        }

        typename BaseTree<T>::Node* parentNode = targetNode->parent; //reduce reuse recycle! (from remove) (I'm writing this at 2am 3/19 losing it a little)
        int targetNodeInd = 0;
        for (int i = 0; i<parentNode->childrenNodes.size();++i) {
            if (parentNode->childrenNodes[i] == targetNode) {
                targetNodeInd = i;
                break;
            }
        }

        if (targetNodeInd < parentNode->childrenNodes.size()-1 && parentNode->childrenNodes[targetNodeInd+1]->values.size() > minKeys){
            typename BaseTree<T>::Node* rightNode = parentNode->childrenNodes[targetNodeInd+1];

            targetNode->values.push_back(parentNode->values[targetNodeInd]);
            parentNode->values[targetNodeInd] = rightNode->values.front();
            rightNode->values.erase(rightNode->values.begin());

            if (rightNode->childrenNodes.size()>0) {
                //we stole the smallest key from right to try and balance
                //so we gotta take the kid that comes with it

                typename BaseTree<T>::Node* stolenRightBaby = rightNode->childrenNodes[0];
                targetNode->childrenNodes.push_back(stolenRightBaby);
                stolenRightBaby->parent = targetNode;
                rightNode->childrenNodes.erase(rightNode->childrenNodes.begin());
            }

            targetNode->subTreeSize = targetNode->values.size();

            for(typename BaseTree<T>::Node* childNode: targetNode->childrenNodes){
                targetNode->subTreeSize += childNode->subTreeSize;
            }


            rightNode->subTreeSize = rightNode->values.size();

            for(typename BaseTree<T>::Node* childNode: rightNode->childrenNodes){
                 rightNode->subTreeSize+= childNode->subTreeSize;
            }

            return;
        }
        if (targetNodeInd>0 && parentNode->childrenNodes[targetNodeInd-1]->values.size() > minKeys) {
            typename BaseTree<T>::Node* leftNode = parentNode->childrenNodes[targetNodeInd-1];

            targetNode->values.insert(targetNode->values.begin(), parentNode->values[targetNodeInd-1]);
            parentNode->values[targetNodeInd-1] = leftNode->values.back();
            leftNode->values.pop_back();

            if (leftNode->childrenNodes.size() > 0) {
                typename BaseTree<T>::Node* stolenLeftBaby = leftNode->childrenNodes.back();
                targetNode->childrenNodes.insert(targetNode->childrenNodes.begin(), stolenLeftBaby);
                stolenLeftBaby->parent = targetNode;
                leftNode->childrenNodes.pop_back();
            }

            targetNode->subTreeSize = targetNode->values.size();

            for(typename BaseTree<T>::Node* childNode: targetNode->childrenNodes){
                targetNode->subTreeSize += childNode->subTreeSize;
            }

            
            leftNode->subTreeSize = leftNode->values.size();

            for(typename BaseTree<T>::Node* childNode: leftNode->childrenNodes){
                 leftNode->subTreeSize+= childNode->subTreeSize;
            }


            return;
        }
        else {
            if (targetNodeInd == parentNode->childrenNodes.size()-1) { //no right child, we have to do left child merge
                typename BaseTree<T>::Node* leftNode = parentNode->childrenNodes[targetNodeInd-1];
                leftNode->values.push_back(parentNode->values[targetNodeInd-1]);
                for (int f = 0; f<targetNode->values.size();++f) {
                    leftNode->values.push_back(targetNode->values[f]);
                }
                for (int g = 0; g<targetNode->childrenNodes.size();++g) {
                    leftNode->childrenNodes.push_back(targetNode->childrenNodes[g]);
                    targetNode->childrenNodes[g]->parent = leftNode;
                }
                leftNode -> subTreeSize = leftNode-> values.size();
                delete targetNode;

                leftNode->subTreeSize = leftNode->values.size();

                for(typename BaseTree<T>::Node* leftMerge: leftNode->childrenNodes){
                    leftNode->subTreeSize += leftMerge->subTreeSize;
                }


                parentNode->values.erase(parentNode->values.begin() + targetNodeInd-1);
                parentNode->childrenNodes.erase(parentNode->childrenNodes.begin() + targetNodeInd);
            }
            else {
                typename BaseTree<T>::Node* rightNode = parentNode->childrenNodes[targetNodeInd+1];
                targetNode->values.push_back(parentNode->values[targetNodeInd]);
                for (int u = 0; u<rightNode->values.size();++u) {
                    targetNode->values.push_back(rightNode->values[u]);
                }
                for (int v = 0; v<rightNode->childrenNodes.size();++v) {
                    targetNode->childrenNodes.push_back(rightNode->childrenNodes[v]);
                    rightNode->childrenNodes[v]->parent = targetNode;
                }
                delete rightNode;
                rightNode = nullptr; //Just to be redundant

                targetNode->subTreeSize = targetNode->values.size();

                for(typename BaseTree<T>::Node* rightMerge: targetNode->childrenNodes){
                    targetNode->subTreeSize += rightMerge->subTreeSize;
                }


                parentNode->values.erase(parentNode->values.begin() + targetNodeInd);
                parentNode->childrenNodes.erase(parentNode->childrenNodes.begin() + targetNodeInd + 1);

            }
            recursiveNodeMergerBS(parentNode);
        }
    }
};