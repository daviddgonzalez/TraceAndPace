#pragma once
#include <vector>
#include <string>
#include <thread>
#include <barrier>
#include <chrono>
#include <functional>

#include "AVLTree.hpp"
#include "BaseTree.hpp"
#include "BTree.hpp"
#include "SplayTree.hpp"


std::vector<BaseTree<std::string>*> trees;


void addAVLTree() {
    trees.push_back(new AVLTree<std::string>);
}
void addBTree() {
    trees.push_back(new VisualBTree<std::string>(3));
}
void addSplayTree() {
    trees.push_back(new SplayTree<std::string>);
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

std::vector<double> runBulkCommands(std::string inputString) {
    std::vector<std::vector<std::string>> input;

    std::istringstream stringStream(inputString);
    std::string line;
    while (std::getline(stringStream, line)) {
        std::istringstream lineStream(line);
        std::string command, value;
        lineStream >> command >> value;
        input.emplace_back(std::vector{command,value});
    }

    std::vector<std::thread> threads;
    threads.reserve(trees.size());
    std::barrier barrier(trees.size());

    std::vector<double> out(trees.size());
    std::vector<long long> times(trees.size());

    for (int i = 0; i < trees.size(); i ++) {
        threads.emplace_back([i, &times, &barrier, &input]() {
            barrier.arrive_and_wait();

            auto start = std::chrono::high_resolution_clock::now();

            for (std::vector<std::string> command : input)
                executeOneAction(command[0], i, std::stoi(command[1]));

            auto end = std::chrono::high_resolution_clock::now();

            times.at(i) = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        });
    }

    for (std::thread& thread :threads)
        thread.join();

    long long minTime = *std::ranges::min_element(times);
    double oneFrame = 1/60.0;

    for (int i = 0; i < times.size(); i++)
        out.at(i) = static_cast<double>(times.at(i))/minTime * oneFrame;

    return out;
}

void insertCSV(std::string csv, bool hasHeader, bool mustHash) {
    std::vector<std::pair<int,std::string>> data;

    std::istringstream stringStream(csv);
    std::string line;

    if (hasHeader)
        std::getline(stringStream, line); // ditch the header

    std::hash<std::string>* hasher = nullptr; // this way hasher is only instantiated if we need it (i have a feeling this is just a basic functor)
    if (mustHash)
        hasher = new std::hash<std::string>();

    while (std::getline(stringStream, line)) {
        std::string workingString;
        std::istringstream rowStream;
        std::getline(rowStream,workingString, ',');
        std::size_t key;
        if (mustHash)
            key = static_cast<int>((*hasher)(workingString));
        else
            key = std::stoi(workingString);
        rowStream >> workingString;
        data.push_back({key,workingString});
    }
    // we are now done with hasher
    delete hasher;

    for (std::pair<int,std::string> datum : data)
        for (BaseTree<std::string>* tree : trees)
            tree->insert(datum.first,datum.second);
}


std::string getWholeView(int treeIndex, int limitOfDisplayedNodes){
    std::string temp = trees[treeIndex]->getWholeViewJson(limitOfDisplayedNodes);
    return temp;
}

std::string getSubView(int treeIndex, int key, int limitOfDisplayedNodes){
    std::string temp = trees[treeIndex]->getSubViewJson(key, limitOfDisplayedNodes);
    return temp;
}