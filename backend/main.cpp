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

    // tree functions
    emscripten::function("addAVLTree", &addAVLTree);
    emscripten::function("addBTree", &addBTree);
    emscripten::function("addSplayTree", &addSplayTree);

    emscripten::function("insertToTrees", &insertToTrees);
    emscripten::function("removeFromTrees", &removeFromTrees);
    emscripten::function("findInTrees", &findInTrees);
    emscripten::function("numOfTrees", static_cast<int(*)()>([](){ return (int)trees.size(); })); // i did this because it makes me chuckle
    emscripten::function("removeTree", &removeTree);

    emscripten::function("executeOneAction", &executeOneAction);

    emscripten::register_vector<double>("doubleVector");
    emscripten::function("runBulkCommands", &runBulkCommands);
    emscripten::function("insertCSV", &insertCSV);

    // i removed any exporting of objects, decided that all the objects will be managed in cpp
    // we will only export functions that will be called on button presses if that makes sense
}