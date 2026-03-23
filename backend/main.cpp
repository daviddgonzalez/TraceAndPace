#include <iostream>
#include <emscripten/bind.h>
#include "TreeManager.hpp"

// This is the main entry point of out cpp code and where all the embindings will be

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

    emscripten::function("addTree", &addTree);
    emscripten::function("insertToTrees", &insertToTrees);
    emscripten::function("removeFromTrees", &removeFromTrees);
    

    // i removed any exporting of objects, decided that all the objects will be managed in cpp
    // we will only export functions that will be called on button presses if that makes sense
}