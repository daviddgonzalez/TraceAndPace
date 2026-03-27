import { useRef, useState } from "react";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";

// this is an interface called props, this is how you pass things in react, dont ask me about it
interface props {
  wasm: MainModule;
  rerenderTreeViews: () => void;
}

function BulkInput({ wasm, rerenderTreeViews }: props) {
  // this function needs to be called for each tree
  const raceTree = (
    treeIndex: number,
    commands: string[][],
    delay: number,
  ): void => {
    let step = 0;

    // definition of a recursive function that delays its future call by a set time
    const runSingleCommand = () => {
      if (step >= commands.length) {
        return; // base case where we are done
      }

      wasm.executeOneAction(
        commands[step][0],
        treeIndex,
        parseInt(commands[step][1]),
      );
      rerenderTreeViews();
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

    timeVector.delete();
  };

  const textArea = useRef<HTMLTextAreaElement>(null);
  const fileInput = useRef<HTMLInputElement>(null);
  const [useFile, setUseFile] = useState<boolean>(false);
  const [hasHeader, setHasHeader] = useState<boolean>(false);
  const [needsHash, setNeedsHash] = useState<boolean>(false);

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
            {/* a ternary operator that wraps the two options for input, controls which the user sees */}
            {useFile ? (
              <div className="modal-body">
                <label htmlFor="formFile" className="form-label">
                  Insert a .csv file of your choosing here
                </label>
                <input
                  className="form-control"
                  id="formFile"
                  type="file"
                  accept=".csv"
                  ref={fileInput}
                />
                <div className="form-check mt-3">
                  <input
                    className="form-check-input"
                    type="checkbox"
                    value=""
                    id="checkDefault"
                    checked={hasHeader}
                    onChange={() => {
                      setHasHeader(!hasHeader);
                    }}
                  />
                  <label className="form-check-label" htmlFor="checkDefault">
                    This .csv has a header row
                  </label>
                </div>
                <div className="form-check">
                  <input
                    className="form-check-input"
                    type="checkbox"
                    value=""
                    id="checkDefault"
                    checked={needsHash}
                    onChange={() => {
                      setNeedsHash(!needsHash);
                    }}
                  />
                  <label className="form-check-label" htmlFor="checkDefault">
                    This .csv has a first column of integers
                  </label>
                </div>
              </div>
            ) : (
              <div className="modal-body">
                <p>
                  Below insert commands in the format of:
                  <br />
                  {"<command> <integer>"}
                  <br />
                  <br />
                  Each command on a new line with space between command and
                  number.
                  <br /> Acceptable commands are: "insert", "remove", and
                  "find".
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
            )}
            <div className="modal-footer">
              <button
                type="button"
                className="btn btn-outline-secondary me-auto"
                onClick={() => {
                  setUseFile(!useFile);
                }}
              >
                {useFile ? "press to use text input" : "press to input .csv"}
              </button>
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
                  if (useFile && fileInput.current?.files?.[0]) {
                    const reader = new FileReader();

                    // we are defining what will happen when this object reads in a file
                    // it prevents the browser from freezing when loading the file
                    reader.onload = (event) => {
                      wasm.insertCSV(
                        event.target?.result as string,
                        hasHeader,
                        needsHash,
                      );
                    };
                    // this calls the above function reading in the file
                    reader.readAsText(fileInput.current.files[0]);

                    fileInput.current.value = "";
                    setHasHeader(false);
                    setNeedsHash(false);
                    setUseFile(false);
                  } else if (textArea.current) {
                    handleABulkInput(textArea.current.value);
                    textArea.current.value = "";
                  }
                }}
              >
                {useFile ? "Insert" : "Run"}
              </button>
            </div>
          </div>
        </div>
      </div>
    </>
  );
}

export default BulkInput;
