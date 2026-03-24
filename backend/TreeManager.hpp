#pragma once
#include <vector>
#include <string>
#include <thread>
#include <barrier>

#include "AVLTree.hpp"
#include "BaseTree.hpp"


std::vector<BaseTree<std::string>*> trees;


void addTree() {
    trees.push_back(new AVLTree<std::string>);
}

void insertToTrees(int number) {
    if (trees.empty())
        return;

    std::vector<std::thread> threads;
    threads.reserve(trees.size());
    std::barrier barrier(trees.size());

    for (int i = 0; i < trees.size(); i++) {
        BaseTree<std::string>* tree = trees.at(i);
        threads.emplace_back([tree, i, &number, &barrier]() {
            barrier.arrive_and_wait();
            tree->insert(number, std::to_string(number));
            tree->treeToJsonFile("Tree" + std::to_string(i));
        });
    }

    for (std::thread& thread : threads)
        thread.join();
}

bool removeFromTrees(int number) {
    if (trees.empty())
        return true;

    std::atomic out = true;

    std::vector<std::thread> threads;
    threads.reserve(trees.size());
    std::barrier barrier(trees.size());

    for (int i = 0; i < trees.size(); i++) {
        BaseTree<std::string>* tree = trees.at(i);
        threads.emplace_back([tree, i, &barrier, &number, &out]() {
            // copying the pointer but the rest should be by reference
            barrier.arrive_and_wait();
            if (!tree->remove(number))
                out = false;
            tree->treeToJsonFile("Tree" + std::to_string(i));
        });
    }

    for (std::thread& thread : threads)
        thread.join();

    return out;
}

bool findInTrees(int number) {
    if (trees.empty())
        return true;

    std::atomic out = true;

    std::vector<std::thread> threads;
    threads.reserve(trees.size());
    std::barrier barrier(trees.size());

    for (int i = 0; i < trees.size(); i++) {
        BaseTree<std::string>* tree = trees.at(i);
        threads.emplace_back([tree, i, &out, &number, &barrier]() {
            barrier.arrive_and_wait();

            if (std::to_string(number) != tree->find(number))
                out = false;
            tree->treeToJsonFile("Tree" + std::to_string(i));
        });
    }
    return out;
}

void removeTree(int index) {
    delete trees.at(index);
    trees.erase(trees.begin() + index);
}
