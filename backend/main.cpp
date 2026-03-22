#include <iostream>
#include <emscripten/bind.h>
#include "BaseTree.hpp"
#include "AVLTree.hpp"

// int main() {
//
// }

// test hello world
std::string helloWorld() {
    return "c++ speaking";
}

// This is how we get Emscripten to make the .js files to communicate with the frontend.
EMSCRIPTEN_BINDINGS(testing_module) {
    emscripten::function("helloWorld", &helloWorld);

    emscripten::class_<AVLTree<std::string>>("AVLTree")
        .constructor<>()
        .function("insert", &AVLTree<std::string>::insert)
        .function("remove", &AVLTree<std::string>::remove)
        .function("find", &AVLTree<std::string>::find)
        .function("height", &AVLTree<std::string>::height);
}
