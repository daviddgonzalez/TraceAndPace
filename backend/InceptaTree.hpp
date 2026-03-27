#include <vector>
#include <utility>
#include <string>

#include "json.hpp" // used from https://github.com/nlohmann/json

using json = nlohmann::json;

template <typename T>
struct InceptaNode
{
   std::pair<int, T> firstKeyAndVal;

    int nodeDepth; // We have zoom depth on the front end and node depth

    //zoom depth - how zoomed in the user is
    //node depth - how many levels deep this particular node is

    int totalNumOfNodes; // how many children the node has, present or not and i added itself

    int localChildCount; // amount of children that are displayed on screen

    bool condensed;

    InceptaNode(std::pair<int, T> val, int d, int totalKids, int localKids, bool condensed) : firstKeyAndVal(val), nodeDepth(d), totalNumOfNodes(totalKids),
                                                                                                    
                                                                                                    localChildCount(localKids), condensed(condensed) {};

    json nodeToJson() const {
        json nodeJson;

        nodeJson["key"] = firstKeyAndVal.first;
        nodeJson["value"] = firstKeyAndVal.second;

        nodeJson["nodeDepth"] = nodeDepth;
        nodeJson["totalNumOfNodes"] = totalNumOfNodes;

        nodeJson["localChildCount"] = localChildCount;

        nodeJson["condensed"] = condensed;

        return nodeJson;
    }
};

template <typename T>
struct InceptaTree
{

    std::vector<InceptaNode<T>> visibleNodes;

    int totalNodes;

    // We're storing the current depth on the frontend side in TreeState

    int displayToTotRatio;

    InceptaTree() {};

    InceptaTree(std::vector<InceptaNode<T>> visi, int tot, int displayRatio) : visibleNodes(visi), totalNodes(tot),
                                                                        displayToTotRatio(displayRatio) {};

    json treeToJson() const {
        json treeJson;

        treeJson["totalNodes"] = totalNodes;
        treeJson["displayToTotRatio"] = displayToTotRatio;

        treeJson["visibleNodes"] = json::array();

        for (const auto& node : visibleNodes) {
            treeJson["visibleNodes"].push_back(node.nodeToJson());
        }

        return treeJson;

    }
};