import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import "./index.css";
import App from "./App.tsx";
import { Suspense } from "react";
import { Xwrapper } from "react-xarrows";

createRoot(document.getElementById("root")!).render(
  <StrictMode>
    <Suspense fallback={<div>Loading C++...</div>}>
      <Xwrapper>
        <App />
      </Xwrapper>
    </Suspense>
  </StrictMode>,
);
