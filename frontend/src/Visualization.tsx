import type { InceptaTree } from "./TreeStructure";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";
import Node from "./Node";

interface props {
  tree: InceptaTree;
  wasm: MainModule;
  treeIndex: number;
  rerenderTreeViews: () => void;
}

function Visualization({ tree, wasm, treeIndex, rerenderTreeViews }: props) {
  return (
    <>
      <div className="card" style={{ width: "18rem" }}>
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
          {/* returns an array of nodes */}
          {tree.visibleNodes.map((node, i) => (
            <Node key={i} node={node} />
          ))}
        </div>
      </div>
    </>
  );
}

export default Visualization;
