import type { InceptaNode } from "./TreeStructure";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";
import Node from "./Node";

interface props {
  tree: InceptaNode;
  wasm: MainModule;
  treeIndex: number;
  rerenderTreeViews: () => void;
}

function Visualization({ tree, wasm, treeIndex, rerenderTreeViews }: props) {
  const displayNodes = (node: InceptaNode) => {
    // i will be implementing a BFS to display these nodes... look at meee

    let nodesToProcess: { node: InceptaNode; level: number }[] = [
      { node: node, level: 0 },
    ];
    let levels: InceptaNode[][] = [];

    while (nodesToProcess.length > 0) {
      let currentNode = nodesToProcess.shift();
      if (!currentNode || !currentNode.node) return null;

      // must initalize array level first
      if (!levels[currentNode.level]) levels[currentNode.level] = [];

      levels[currentNode.level].push(currentNode.node);
      if (currentNode.node.children)
        for (let child of currentNode.node.children)
          if (child)
            nodesToProcess.push({ node: child, level: currentNode.level + 1 });
    }

    return (
      <>
        {levels.map((level, i) => {
          return (
            <>
              <div
                className="d-flex justify-content-center gap-2"
                style={{ overflowX: "auto" }}
                key={i}
              >
                {level.map((node) => {
                  return <Node node={node} key={node.key} />;
                })}
              </div>
            </>
          );
        })}
      </>
    );
  };

  return (
    <>
      <div className="card" style={{ flex: "1 1 0", minWidth: "250px" }}>
        <div className="card-body">
          <div className="d-flex justify-content-between">
            <h5 className="card-title">{"Tree Name"}</h5>
            <button
              type="button"
              className="btn-close"
              aria-label="Close"
              onClick={() => {
                wasm.removeFromTrees(treeIndex);
                rerenderTreeViews();
              }}
            ></button>
          </div>
          {/* the below div is supposed to be for each level of a tree, but david is clowning */}
          <div
            className="d-flex flex-column align-items-center gap-2"
            style={{ overflowX: "auto" }}
          >
            {/* returns an array of nodes */}
            {displayNodes(tree)}
          </div>
        </div>
      </div>
    </>
  );
}

export default Visualization;
