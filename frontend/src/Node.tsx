import type { InceptaNode } from "./TreeStructure";

interface props {
  node: InceptaNode;
  id: string;
  onClick?: (node: InceptaNode) => void;
}

// scales font size down as the string gets longer
function getFontSize(len: number): string {
  if (len < 3) return "1.4rem";
  if (len < 10) return String(4.2 / len) + "rem";
  return "0.42rem";
}

function Node({ node, id, onClick }: props) {
  return (
    <div
      id={id}
      className="rounded-circle d-flex justify-content-center align-items-center"
      style={{
        width: "50px",
        height: "50px",
        minWidth: "50px",
        minHeight: "50px",
        border: !node.condensed
          ? "1.5px solid #0e6dfc"
          : "1.5px solid #fc9d0eff",
        backgroundColor: !node.condensed ? "#d1e3ff" : "#ffc47cff",
        overflow: "hidden",
        fontSize: getFontSize(node.value.length),
        fontWeight: 600,
        color: "#000",
        textAlign: "center",
        wordBreak: "break-all",
        lineHeight: "1.1",
        padding: "4px",
        cursor: node.condensed ? "pointer" : "default",
      }}
      onClick={() => {
        if (node.condensed && onClick) {
          onClick(node);
        }
      }}
    >
      {node.value}
    </div>
  );
}

export default Node;
