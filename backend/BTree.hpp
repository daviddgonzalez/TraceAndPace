#include "BaseTree.hpp"

template <typename T>
class VisualBTree : public BaseTree<T>
{

public:
    VisualBTree(int keys = 2) : BaseTree<T>(), nodeKeys(keys), nodeNumChildren(nodeKeys + 1)
    {
        if (nodeKeys <= 1)
        {
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
    VisualBTree(VisualBTree &&other) : BaseTree<T>(std::move(other)), nodeKeys(other.nodeKeys), nodeNumChildren(other.nodeNumChildren) {} // move constructor

    VisualBTree &operator=(VisualBTree &&other)
    {
        BaseTree<T>::operator=(std::move(other)); // I'm using std::move to turn other into an rvalue reference to call the move assignment instead of copy assignment.
        nodeKeys = other.nodeKeys;
        nodeNumChildren = other.nodeNumChildren;
        return *this;
    }

    ~VisualBTree()
    {
        BaseTree<T>::~BaseTree();
    };

    // search returns if it was successful and a pointer to the node
    // find returns just the value, throws an error if the key doesnt exist

    std::pair<bool, typename BaseTree<T>::Node *> search(int target) // typename here to tell teh compiler node is a struct not a variable, so the * is a pointer not mult
    // std::pair to act as a helper method for insert.
    { // nullptr if doesnt exist, pointer to node if it does exist
        typename BaseTree<T>::Node *curr = this->head;
        while (curr != nullptr)
        {
            std::pair<bool, int> searchResult = binarySearch(curr->values, target);
            if (searchResult.first)
                return {true, curr};
            if (curr->childrenNodes.size() == 0) // if curr is a leaf and we didnt find then return where it should be inserted
                return {false, curr};
            curr = curr->childrenNodes[searchResult.second]; // if we didnt find the target, go down a level to where it should be.
        }
        return {false, nullptr}; // return nullptr bc the tree itself doesnt exist
    }

    T find(int nodeKey) override
    {

        std::pair<bool, typename BaseTree<T>::Node *> keyResult = search(nodeKey);

        if (!keyResult.first)
        {
            throw std::invalid_argument("Key given does not exist in the tree");
        }

        typename BaseTree<T>::Node *nodePair = keyResult.second;
        std::pair<bool, int> nodeSearch = binarySearch(nodePair->values, nodeKey);

        int resultInd = nodeSearch.second;
        std::vector<std::pair<int, T>> nodeValues = nodePair->values; // could prolly do this in one line, figured its more readable
        std::pair<int, T> result = nodeValues[resultInd];

        return result.second;
    }

    bool insert(int k, T value) override
    {
        std::pair<bool, typename BaseTree<T>::Node *> treeSearch = search(k);
        // std::cout<< treeSearch.first<<"  " << treeSearch.second<<std::endl; // test search"
        if (treeSearch.first)
            return false; // if the key already exists we dont insert and return false
        typename BaseTree<T>::Node *current = treeSearch.second;

        if (current == nullptr)
        { // empty tree case

            this->head = new typename BaseTree<T>::Node();
            this->head->values.push_back({k, value});
            return true;
        }

        std::pair<bool, int> nodeSearch = binarySearch(current->values, k);
        int insertInd = nodeSearch.second;

        current->values.insert(current->values.begin() + insertInd, {k, value});

        // if the node has too many keys we need to split it
        if (current->values.size() > nodeKeys)
        {
            bTreeSplit(current);
        }

        return true;
    }

private:
    int nodeKeys;
    int nodeNumChildren;
    // number of keys in each node, kids is keys + 1
    // everything else is encapsulated in the base tree

    std::pair<bool, int> binarySearch(std::vector<std::pair<int, T>> &nodeVals, int target)
    {

        int left = 0;
        int right = nodeVals.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2; // to avoid overflow, unlikely though

            if (nodeVals[mid].first == target)
            {
                std::pair<bool, int> result = {true, mid}; // true that we found the target and mid is the index
                return result;
            }

            else if (nodeVals[mid].first < target)
            {
                left = mid + 1;
            }

            else
            {
                right = mid - 1;
            }
        }

        std::pair<bool, int> result = {false, left}; // false we didnt find the target, left is where it should be inserted
        return result;
    }

    void bTreeSplit(typename BaseTree<T>::Node *node)
    {
        int middle = node->values.size() / 2;
        typename BaseTree<T>::Node *rightNode = new typename BaseTree<T>::Node();
        std::pair<int, T> middlePair = node->values[middle];

        // value shiz
        for (int i = middle + 1; i < node->values.size(); ++i)
        {

            rightNode->values.push_back(node->values[i]);
        }

        int amtToRemove = node->values.size() - middle;

        for (int k = 0; k < amtToRemove; k++)
        {
            node->values.pop_back();
        }

        // deal w kids
        if (node->childrenNodes.size() > 0)
        {

            // I'm doing the same thing as the vals, but also updating parent ptr
            for (int u = middle + 1; u < node->childrenNode.size(); ++u)
            {
                rightNode->childrenNodes.push_back(node->childrenNodes[u]);
                node->childrenNodes[u]->parent = rightNode;
            }

            amtToRemove = node->childrenNodes.size() - (middle + 1);

            for (int q = 0; q < amtToRemove; q++)
            {
                node->childrenNodes.pop_back();
            }
        }

        // I'm using the recursive calls to repeatedly raise the key/value pair

        if (node->parent == nullptr)
        {                                                                           // we have to make a new root
            typename ::BaseTree<T>::Node *jesus = new typename BaseTree<T>::Node(); // I wrote jesus because he rises

            jesus->values.push_back(middlePair);
            jesus->childrenNodes.push_back(node);
            jesus->childrenNodes.push_back(rightNode);

            node->parent = jesus;
            rightNode->parent = jesus;
            this->head = jesus;
        }
        else
        {
            typename BaseTree<T>::Node *parentofCurr = node->parent;

            rightNode->parent = parentofCurr;
            int insInd = binarySearch(parentofCurr->values, middlePair.first).second;

            parentofCurr->values.insert(parentofCurr->values.begin() + insInd, middlePair);
            parentofCurr->childrenNodes.insert(parentofCurr->childrenNodes.begin() + insInd + 1, rightNode);

            if (parentofCurr->values.size() > nodeKeys)
            {

                bTreeSplit(parentofCurr); // I'm gonna call it recursively if parent overflows
            }
        }
    }
};