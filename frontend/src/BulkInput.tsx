import { useRef } from "react";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";

// this is an interface called props, this is how you pass things in react, dont ask me about it
interface props {
  wasm: MainModule;
}

function BulkInput(props: props) {
  const wasm = props.wasm;

  // this function needs to be called for each tree
  const raceTree = (
    treeIndex: number,
    commands: string[][],
    delay: number,
  ): void => {
    let step = 0;

    // definition of a recirsive function that delays its future call by a set time
    const runSingleCommand = () => {
      if (step >= commands.length) {
        return; // base case where we are done
      }

      wasm.executeOneAction(
        commands[step][0],
        treeIndex,
        parseInt(commands[step][1]),
      );
      // TODO: include rerender function call here
      step++;
      setTimeout(runSingleCommand, delay);
    };

    runSingleCommand();
  };

  const handleABulkInput = (input: string) => {
    const timeVector = wasm.runBulkCommands(input);

    // this turns the input string into an array of arrays of strings
    let commands = input.split("\n").map((line) => {
      return line.split(" ");
    });

    for (let i = 0; i < timeVector.size(); i++) {
      raceTree(i, commands, Number(timeVector.get(i)));
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
              <p>
                Below insert commands in the format of:
                <br />
                {"<command> <integer>"}
                <br />
                <br />
                Each command on a new line with space between command and
                number.
                <br /> Acceptable commands are: "insert", "remove", and "find".
              </p>
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
