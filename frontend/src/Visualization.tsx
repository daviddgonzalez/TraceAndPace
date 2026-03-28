import type { InceptaNode } from "./TreeStructure";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";
import Xarrow, { Xwrapper } from "react-xarrows";
import Node from "./Node";
import { useState, useEffect } from "react";

interface props {
  tree: InceptaNode;
  wasm: MainModule;
  treeIndex: number;
  rerenderTreeViews: () => void;
}

function Visualization({ tree, wasm, treeIndex, rerenderTreeViews }: props) {
  const [subViewNode, setSubViewNode] = useState<InceptaNode | null>(null);

  // scans changes to tree to see if it needs to modify visualization
  useEffect(() => {
    setSubViewNode(null);
  }, [tree]);

  const handleNodeClick = (clickedNode: InceptaNode) => {
    if (clickedNode.condensed) {
      const subViewJson = wasm.getSubView(treeIndex, clickedNode.key, 50);
      const parsed = JSON.parse(subViewJson);
      setSubViewNode(parsed);
    }
  };

  const displayNodes = (node: InceptaNode) => {
    // i will be implementing a BFS to display these nodes... look at meee

    let nodesToProcess: { node: InceptaNode; level: number }[] = [
      { node: node, level: 0 },
    ];
    let levels: InceptaNode[][] = [];
    let edges: { start: string; end: string }[] = [];

    while (nodesToProcess.length > 0) {
      let currentNode = nodesToProcess.shift();
      if (!currentNode || !currentNode.node) return null;

      // must initalize array level first
      if (!levels[currentNode.level]) levels[currentNode.level] = [];

      levels[currentNode.level].push(currentNode.node);

      // this is the current node we are working on, we add all the edges while adding to the queue
      const parentId = `tree-${treeIndex}-node-${currentNode.node.key}`;

      if (currentNode.node.children) {
        for (let child of currentNode.node.children) {
          if (child) {
            nodesToProcess.push({ node: child, level: currentNode.level + 1 });
            edges.push({
              start: parentId,
              end: `tree-${treeIndex}-node-${child.key}`,
            });
          }
        }
      }
    }

    return (
      <Xwrapper>
        {levels.map((level, i) => {
          return (
            <div
              className="d-flex justify-content-center gap-4"
              style={{ overflowX: "auto", paddingBottom: "40px" }}
              key={i}
            >
              {level.map((node) => {
                return (
                  <Node
                    node={node}
                    key={node.key}
                    id={`tree-${treeIndex}-node-${node.key}`}
                    onClick={handleNodeClick}
                  />
                );
              })}
            </div>
          );
        })}
        {edges.map((edge, i) => (
          <Xarrow
            key={`edge-${i}`}
            start={edge.start}
            end={edge.end}
            color="#0e6dfc"
            strokeWidth={2}
            path="straight"
            headSize={4}
            startAnchor="bottom"
            endAnchor="top"
          />
        ))}
      </Xwrapper>
    );
  };

  return (
    <>
      <div className="card" style={{ flex: "1 1 0", minWidth: "250px" }}>
        <div className="card-body">
          <div className="d-flex justify-content-between align-items-center mb-3">
            <h5 className="card-title mb-0">{"Tree Name"}</h5>
            <div className="d-flex gap-2">
              {subViewNode && (
                <button
                  className="btn btn-sm btn-outline-secondary"
                  onClick={() => setSubViewNode(null)}
                >
                  &larr; Back to Root
                </button>
              )}
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
          </div>
          {/* the below div is supposed to be for each level of a tree, but david is clowning */}
          <div
            className="d-flex flex-column align-items-center gap-2"
            style={{ overflowX: "auto" }}
          >
            {displayNodes(subViewNode || tree)}
          </div>
        </div>
      </div>
    </>
  );
}

export default Visualization;
