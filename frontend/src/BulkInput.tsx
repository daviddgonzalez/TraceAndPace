import { useRef } from "react";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";

// this is an interface called props, this is how you pass things in react, dont ask me about it
interface props {
  wasm: MainModule;
}

function BulkInput(props: props) {
  const wasm = props.wasm;

  const handleABulkInput = (input: string) => {
    // we handle this in two parts, first we have the cpp code simulate it and time it.
    // Then we go and update all the trees slowly for the visualization.
    // TODO: make the bulk input function for cpp and pass input to it
    // the below code cant work as of now... we need to think of a new way to do this
    // const lines = input.split("\n");
    // for (const line of lines) {
    //   const parts = line.split(" ");
    //   const command = parts[0];
    //   const value = parseInt(parts[1], 10);
    //   if (command === "insert") {
    //     wasm.insertToTrees(value);
    //   } else if (command === "remove") {
    //     wasm.removeFromTrees(value);
    //   } else if (command === "find") {
    //     wasm.findInTrees(value);
    //   }
    //   const json = wasm.treeToJsonString();
    // }
  };
  const textArea = useRef<HTMLTextAreaElement>(null);
  return (
    <>
      {/* bootstrap is so cool */}
      <div className="modal" tabIndex={-1} id="bulkInputTextBox">
        <div className="modal-dialog">
          <div className="modal-content">
            <div className="modal-header">
              <h5 className="modal-title">Bulk Input</h5>
              <button
                type="button"
                className="btn-close"
                data-bs-dismiss="modal"
                aria-label="Close"
              ></button>
            </div>
            <div className="modal-body">
              <div className="form-floating">
                <textarea
                  className="form-control"
                  placeholder="Insert Lines of Commands Each On a New Line"
                  id="floatingTextarea2"
                  style={{ height: "100px" }}
                  ref={textArea}
                ></textarea>
                {/* the ref thing is basically a react variable that is a refrence to this. So we can diddle this element later */}
                <label htmlFor="floatingTextarea2">Paste Commands Here</label>
              </div>
            </div>
            <div className="modal-footer">
              <button
                type="button"
                className="btn btn-secondary"
                data-bs-dismiss="modal"
              >
                Close
              </button>
              {/* run button, when pressed it reads in the content of the text area, clears it, closes the popup, and starts the tree race */}
              <button
                type="button"
                className="btn btn-primary"
                data-bs-dismiss="modal"
                onClick={() => {
                  if (textArea.current) {
                    handleABulkInput(textArea.current.value);
                    textArea.current.value = "";
                  }
                }}
              >
                Run
              </button>
            </div>
          </div>
        </div>
      </div>
    </>
  );
}

export default BulkInput;
