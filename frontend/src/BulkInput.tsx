import { useRef } from "react";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";

// this is an interface called props, this is how you pass things in react, dont ask me about it
interface props {
  wasm: MainModule;
}

function BulkInput(props: props) {
  const wasm = props.wasm;

  const handleABulkInput = (input: string) => {
    const lines = input.split("\n");
    for (let line of lines) {
      const command = line.split(" ");
      for (let i = 0; i < wasm.numOfTrees(); i++) {
        wasm.executeOneAction(command[0], i, parseInt(command[1]));
      }
    }
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
