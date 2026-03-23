#pragma once
#include <vector>
#include <string>

#include "AVLTree.hpp"
#include "BaseTree.hpp"


std::vector<BaseTree<std::string>*> trees;

/// all these methods are inherently flawed... they do all these things in series not parallel
// TODO: make it so that they do it right

void addTree() {
    trees.push_back(new AVLTree<std::string>);
}
void insertToTrees(int number) {
    for (BaseTree<std::string>* tree : trees)
        tree->insert(number, std::to_string(number));
}
bool removeFromTrees(int number) {
    for (BaseTree<std::string>* tree : trees)
        if (!tree->remove(number))
            return false; // there should never be a world where an element is found in one tree but not another
    return true;
}
bool findInTrees(int number) {
    for (BaseTree<std::string>* tree : trees)
        if (std::to_string(number) != tree->find(number))
            return false;
    return true;
}
int numOfTrees() {
    return trees.size();
}