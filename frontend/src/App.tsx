import './App.css'

import WasmFactory from './wasm/TraceAndPace.mjs';
import type {MainModule} from './wasm/TraceAndPace.d.mts';

const initializeWasm = async (): Promise<MainModule> => {
    return await WasmFactory();
};

// this needs to be outside the react element so it only runs once.
// it isnt guaranteed to be loaded instantly so you must call use() on it whenever you use it.
const wasmPromise = initializeWasm();

// contains all the tree elements
const trees = [];

function App() {

  return (
    <>
      
    </>
  )
}

export default App;