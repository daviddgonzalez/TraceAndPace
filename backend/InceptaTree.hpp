#include <vector>
#include <utility>
#include <string>

#include "json.hpp" // used from https://github.com/nlohmann/json

using json = nlohmann::json;

template <typename T>
struct InceptaNode
{
    int key;

    int zoomDepth; // how far zoomed in you are

    int totalNumOfChildren; // how many children the node has, present or not.

    std::pair<int, T> sampleNodeValue;

    std::vector<std::pair<int, T>> sampleChildValues;
    int localChildCount; // amount of children that are displayed on screen

    std::string nodeType;

    bool isItALeaf;

    InceptaNode() {};

    InceptaNode(int k, int d, int totalKids, std::pair<int, T> val, std::vector<std::pair<int, T>> kidVals, int localKids, std::string type, bool leaf) : key(k), zoomDepth(d), totalNumOfChildren(totalKids),
                                                                                                                                                          sampleNodeValue(val), sampleChildValues(kidVals),
                                                                                                                                                          localChildCount(localKids), nodeType(type), isItALeaf(leaf) {};
};

template <typename T>
struct InceptaTree
{

    std::vector<InceptaNode<T>> visibleNodes;

    int totalNodes;

    int newRequestedDepth; // We're storing the current depth on the frontend side in TreeState

    int displayToTotRatio;

    InceptaTree() {};

    InceptaTree(std::vector<InceptaNode<T>> visi, int tot, int reqDepth, int displayRatio) : visibleNodes(visi), totalNodes(tot),
                                                                                             newRequestedDepth(reqDepth), displayToTotRatio(displayRatio) {};
};