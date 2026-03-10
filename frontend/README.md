# Michael's Additions to the Documentation

There are a few quirks with our development pipeline we are going to be working with. Our workflow is going to be a frontend of HTML and typescript. The backend will be written in C++ but compiled into 

## Node.js

You need to download and install [Node.js](https://nodejs.org/en).

After thats all done cd into `./frontend` then run these few commands. 

```bash
npm install
npm fund
npm run dev
o
```
This will install all of the dependencies. Run the development enviroment and open it in your browser.

### Development and Deployment

As of right now you have the development enviroment running and any changes you make to the React code base should be show in your browser as soon as your changes are saved. 

This works wonderfully for development, but as of right now anyone that would want to use our program would have to go through everything above to use our code. There is this handy plugin that I've installed that changes how `npm run build` works. This will compile our code into a single `index.html` file that someone could run and open in their browser to use our code.

 These files that are generated in the `./frontend/dist` folder (generated after you execute `npm run build` for the first time) is what we will be submitting.

 ## WebAssembly (wasm)

 For the backend side of this project we will be compiling our C++ code into `.wasm` files. Our special little compiler for this is [Emscripten](https://emscripten.org/index.html). You will need to download this from [here](https://emscripten.org/docs/getting_started/downloads.html#sdk-download-and-install).

To compile with this you use `emcc` very similarly to how we used `gcc` for compiling. Read the [docs](https://emscripten.org/docs/compiling/Building-Projects.html) to understand it better.

I will be trying to make a camke for this so we don't have to think about it in the future. I will also try to make some kind of class structure to use to generalize visualizations but I'm not sure how I will be going about this. Just really wanted to get to a point where you guys have all the tools to do any work.

# React + TypeScript + Vite

This template provides a minimal setup to get React working in Vite with HMR and some ESLint rules.

Currently, two official plugins are available:

- [@vitejs/plugin-react](https://github.com/vitejs/vite-plugin-react/blob/main/packages/plugin-react) uses [Babel](https://babeljs.io/) (or [oxc](https://oxc.rs) when used in [rolldown-vite](https://vite.dev/guide/rolldown)) for Fast Refresh
- [@vitejs/plugin-react-swc](https://github.com/vitejs/vite-plugin-react/blob/main/packages/plugin-react-swc) uses [SWC](https://swc.rs/) for Fast Refresh

## React Compiler

The React Compiler is not enabled on this template because of its impact on dev & build performances. To add it, see [this documentation](https://react.dev/learn/react-compiler/installation).

## Expanding the ESLint configuration

If you are developing a production application, we recommend updating the configuration to enable type-aware lint rules:

```js
export default defineConfig([
  globalIgnores(['dist']),
  {
    files: ['**/*.{ts,tsx}'],
    extends: [
      // Other configs...

      // Remove tseslint.configs.recommended and replace with this
      tseslint.configs.recommendedTypeChecked,
      // Alternatively, use this for stricter rules
      tseslint.configs.strictTypeChecked,
      // Optionally, add this for stylistic rules
      tseslint.configs.stylisticTypeChecked,

      // Other configs...
    ],
    languageOptions: {
      parserOptions: {
        project: ['./tsconfig.node.json', './tsconfig.app.json'],
        tsconfigRootDir: import.meta.dirname,
      },
      // other options...
    },
  },
])
```

You can also install [eslint-plugin-react-x](https://github.com/Rel1cx/eslint-react/tree/main/packages/plugins/eslint-plugin-react-x) and [eslint-plugin-react-dom](https://github.com/Rel1cx/eslint-react/tree/main/packages/plugins/eslint-plugin-react-dom) for React-specific lint rules:

```js
// eslint.config.js
import reactX from 'eslint-plugin-react-x'
import reactDom from 'eslint-plugin-react-dom'

export default defineConfig([
  globalIgnores(['dist']),
  {
    files: ['**/*.{ts,tsx}'],
    extends: [
      // Other configs...
      // Enable lint rules for React
      reactX.configs['recommended-typescript'],
      // Enable lint rules for React DOM
      reactDom.configs.recommended,
    ],
    languageOptions: {
      parserOptions: {
        project: ['./tsconfig.node.json', './tsconfig.app.json'],
        tsconfigRootDir: import.meta.dirname,
      },
      // other options...
    },
  },
])
```
