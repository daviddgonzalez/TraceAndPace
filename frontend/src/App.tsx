import "bootstrap/dist/css/bootstrap.min.css";
import "bootstrap/dist/js/bootstrap.bundle.min.js";
import "./App.css";
import { use, useState } from "react";

import WasmFactory from "./wasm/TraceAndPace.mjs";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";
import Visualization from "./Visualization";
import BulkInput from "./BulkInput";

const initializeWasm = async (): Promise<MainModule> => {
  return await WasmFactory();
};

// this needs to be outside the react element so it only runs once.
const wasmPromise = initializeWasm();

function App() {
  // need to call use becuase its resolution is asyncronous
  const wasm = use(wasmPromise);

  // these are strings because react will treat the input as a string the event that we are reading it from returns a string
  // we will only try and parse it into a number once the user submits the form. This allows them to type a negative sign
  // there are restrictions on the input tags that allow only numbers, means it will always be a number
  // interestingly this also allows for scientific notation, thats some pretty nifty validation that we got for free
  const [insertInput, setInsertInput] = useState<string>("");
  const [removeInput, setRemoveInput] = useState<string>("");

  return (
    <>
      {/* bootstrap navbar element, made a dropdown to add a tree */}
      <nav className="navbar navbar-expand-lg bg-body-tertiary">
        <div className="container-fluid">
          <a className="navbar-brand" href="#">
            TraceAndPace
          </a>
          <button
            className="navbar-toggler"
            type="button"
            data-bs-toggle="collapse"
            data-bs-target="#navbarNavDropdown"
            aria-controls="navbarNavDropdown"
            aria-expanded="false"
            aria-label="Toggle navigation"
          >
            <span className="navbar-toggler-icon"></span>
          </button>
          <div className="collapse navbar-collapse" id="navbarNavDropdown">
            <ul className="navbar-nav">
              <li className="nav-item dropdown">
                <a
                  className="nav-link dropdown-toggle"
                  href="#"
                  role="button"
                  data-bs-toggle="dropdown"
                  aria-expanded="false"
                >
                  Add a Data Structure
                </a>
                <ul className="dropdown-menu">
                  <li>
                    <button
                      className="dropdown-item"
                      type="button"
                      onClick={() => wasm.addTree()}
                    >
                      B Tree
                    </button>
                  </li>
                  <li>
                    <button
                      className="dropdown-item"
                      type="button"
                      onClick={() => wasm.addTree()}
                    >
                      B+ Tree
                    </button>
                  </li>
                  <li>
                    <button
                      className="dropdown-item"
                      type="button"
                      onClick={() => wasm.addTree()}
                    >
                      AVL Tree
                    </button>
                  </li>
                  <li>
                    <button
                      className="dropdown-item"
                      type="button"
                      onClick={() => wasm.addTree()}
                    >
                      Splay
                    </button>
                  </li>
                </ul>
              </li>
              {/* This is the button for opening the bulk input popup */}
              <li>
                <button
                  className="btn btn-outline-primary"
                  type="button"
                  data-bs-toggle="modal"
                  data-bs-target="#bulkInputTextBox"
                  style={{ marginLeft: "10px" }}
                >
                  Bulk Input
                </button>
              </li>
            </ul>
          </div>
          {/* This is that little input bar for inserting numbers */}
          <form
            className="d-flex"
            role="search"
            onSubmit={(e) => {
              e.preventDefault();
              const num = parseInt(insertInput, 10);
              if (!isNaN(num)) {
                wasm.insertToTrees(num);
                setInsertInput("");
              }
            }}
          >
            <input
              className="form-control me-2"
              type="number"
              placeholder="Insert Element"
              aria-label="Search"
              value={insertInput}
              onChange={(e) => setInsertInput(e.target.value)}
            />
            <button className="btn btn-outline-primary" type="submit">
              Insert
            </button>
          </form>
          {/* This is that little input bar for removing numbers */}
          <form
            className="d-flex"
            role="search"
            onSubmit={(e) => {
              e.preventDefault();
              const num = parseInt(removeInput, 10);
              if (!isNaN(num)) {
                wasm.removeFromTrees(num);
                setRemoveInput("");
              }
            }}
            style={{ marginLeft: "10px" }}
          >
            <input
              className="form-control me-2"
              type="number"
              placeholder="Remove Element"
              aria-label="Search"
              value={removeInput}
              onChange={(e) => setRemoveInput(e.target.value)}
            />
            <button className="btn btn-outline-primary" type="submit">
              Remove
            </button>
          </form>
        </div>
      </nav>

      <div className="container">
        {/* DAVID! fix this. currently this doesnt get prompted for a rerender when one of the buttons are pressed. This means that the new visualization insnt shown until after you modify something else. */}
        {Array.from({ length: wasm.numOfTrees() }).map((_, i) => (
          <Visualization key={i} />
        ))}
        <BulkInput wasm={wasm} />
      </div>
    </>
  );
}

export default App;
