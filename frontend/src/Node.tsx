import type { InceptaNode } from "./TreeStructure";

interface props {
  node: InceptaNode;
}

// scales font size down as the string gets longer
function getFontSize(len: number): string {
  if (len < 3) return "1.4rem";
  if (len < 10) return String(4.4 / len) + "rem";
  return "0.44rem";
}

function Node({ node }: props) {
  return (
    <div
      className="rounded-circle d-flex justify-content-center align-items-center"
      style={{
        width: "50px",
        height: "50px",
        minWidth: "50px",
        minHeight: "50px",
        border: "1.5px solid #0e6dfc",
        backgroundColor: "#d1e3ff",
        overflow: "hidden",
        fontSize: getFontSize(node.value.length),
        fontWeight: 600,
        color: "#000",
        textAlign: "center",
        wordBreak: "break-all",
        lineHeight: "1.1",
        padding: "4px",
      }}
    >
      {node.value}
    </div>
  );
}

export default Node;
