#pragma once
#include <vector>
#include <string>
#include <thread>
#include <barrier>
#include <chrono>

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

void executeOneAction(std::string action, int treeIndex, int input) {
    if (action == "insert")
        trees.at(treeIndex)->insert(input,std::to_string(input));
    else if (action == "remove")
        trees.at(treeIndex)->remove(input);
    else if (action == "find")
        trees.at(treeIndex)->find(input);
}

std::vector<std::chrono::microseconds> runBulkCommands(std::vector<std::vector<std::string>> input) {
    std::vector<std::thread> threads;
    threads.reserve(trees.size());
    std::barrier barrier(trees.size());

    std::vector<std::chrono::microseconds> out(trees.size());

    for (int i = 0; i < trees.size(); i ++) {
        threads.emplace_back([i, &out, &barrier, &input]() {
            barrier.arrive_and_wait();

            auto start = std::chrono::high_resolution_clock::now();

            for (std::vector<std::string> command : input)
                executeOneAction(command[0], i, std::stoi(command[0]));

            auto end = std::chrono::high_resolution_clock::now();

            out.at(i) = (start - end); // todo: in the middle of timing everything to then send these numbers back to js to make the fake race for
        });
    }
}