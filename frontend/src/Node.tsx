import type { InceptaNode } from "./TreeStructure";

interface props {
  node: InceptaNode;
}

function Node({ node }: props) {
  return <>{node.value + " "}</>;
}

export default Node;
