import "bootstrap/dist/css/bootstrap.min.css";
import "bootstrap/dist/js/bootstrap.bundle.min.js";
import "./App.css";

import WasmFactory from "./wasm/TraceAndPace.mjs";
import type { MainModule } from "./wasm/TraceAndPace.d.mts";
import Visualization from "./Visualization";

const initializeWasm = async (): Promise<MainModule> => {
  return await WasmFactory();
};

// this needs to be outside the react element so it only runs once.
// it isnt guaranteed to be loaded instantly so you must call use() on it whenever you use it.
const wasmPromise = initializeWasm();

import { useState } from "react";

function App() {
  // array of numbers, the ids to trees
  const [trees, setTrees] = useState<number[]>([0]);

  const addTree = (e: React.MouseEvent) => {
    e.preventDefault();
    setTrees([...trees, trees.length]); // this is replacing the old array with a whole new array plus the new array
  };

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
                    <a className="dropdown-item" href="#" onClick={addTree}>
                      B Tree
                    </a>
                  </li>
                  <li>
                    <a className="dropdown-item" href="#" onClick={addTree}>
                      B+ Tree
                    </a>
                  </li>
                  <li>
                    <a className="dropdown-item" href="#" onClick={addTree}>
                      AVL Tree
                    </a>
                  </li>
                  <li>
                    <a className="dropdown-item" href="#" onClick={addTree}>
                      Splay
                    </a>
                  </li>
                </ul>
              </li>
            </ul>
          </div>
        </div>
      </nav>

      <div className="container">
        {trees.map((id) => (
          <Visualization key={id} />
        ))}
      </div>
    </>
  );
}

export default App;
