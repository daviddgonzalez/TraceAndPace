#include <iostream>
#include <emscripten/bind.h>
#include "BaseTree.hpp"

// int main() {
//
// }

// test hello world
std::string helloWorld() {
    return "c++ speaking";
}

// This is how we get Emscripten to make the .js files to comunicate with the frontend.
EMSCRIPTEN_BINDINGS(testing_module) {
    emscripten::function("helloWorld", &helloWorld);
}