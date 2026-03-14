import {Suspense, use, useState} from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'


// I got this way to work around the imports from https://stackoverflow.com/questions/76477419/loading-wasm-with-react-ts-and-vitejs
import wasmUrl from './wasm/TraceAndPace.wasm?url';
import WasmFactory from './wasm/TraceAndPace.mjs';
import type {MainModule} from './wasm/TraceAndPace.d.mts';

// need to steal the import from the .mjs because react could hash the directory away in production builds
// this is the code to correctly start the c++ code.
const initializeWasm = async (): Promise<MainModule> => {
    return await WasmFactory({
        locateFile: (path: string) => {
            if (path.endsWith('.wasm')) {
                return wasmUrl; // Serve the Vite-resolved hashed URL
            }
            return path;
        }
    });
};

// this needs to be outside the react element so it only runs once.
// it isnt guaranteed to be loaded instantly so you must call use() on it whenever you use it.
const wasmPromise = initializeWasm();

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      <div>
        <a href="https://vite.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <h1>Vite + React</h1>
      <div className="card">
        <button onClick={() => setCount((count) => count + 1)}>
          count is {count}
        </button>
        <p>
          Edit <code>src/App.tsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Click on the Vite and React logos to learn more
      </p>
        <Suspense> {/* must treat a use hook as throwing an error and have to catch it with a suspense bracket */}
            {use(wasmPromise).helloWorld()}
        </Suspense>
    </>
  )
}

export default App;