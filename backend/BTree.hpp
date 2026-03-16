#include "BaseTree.hpp"

template <typename T>
class BTree : public BaseTree<T>
{
    BTree(int keys = 2) : BaseTree<T>(), keys(keys), numChildren(keys + 1)
    { // constructor
        if (keys <= 1)
        {
            throw std::invalid_argument("B Tree must have at least 2 keys");
        }
    }

    BTree(BTree &other) : BaseTree<T>(other), keys(other.keys), numChildren(other.numChildren) {} // copy constructor

    BTree &operator=(BTree &other)
    { // copy assignment
        // if (this == &other) BaseTree<T>::operator=(other); should take care of this == other
        //     return *this;
        BaseTree<T>::operator=(other);
        keys = other.keys;
        numChildren = other.numChildren;
        return *this;
    }

    BTree(BTree &&other) : BaseTree<T>(std::move(other)), keys(other.keys), numChildren(other.numChildren) {} // move constructor

    BTree &operator=(BTree &&other)
    {                                             // move assignment
        BaseTree<T>::operator=(std::move(other)); // std::move turns other into an rvalue reference to call the move assignment instead of copy assignment.
        keys = other.keys;
        numChildren = other.numChildren;
        return *this;
    }

    ~BTree()
    {
        BaseTree<T>::~BaseTree();
    }; // destructor

private:
    int keys;
    int numChildren;
    // number of keys in each node, kids is keys + 1
    // everything else is encapsulated in the base tree
}