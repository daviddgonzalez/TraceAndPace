// TypeScript bindings for emscripten-generated code.  Automatically generated at compile time.
declare namespace RuntimeExports {
    function FS_createPath(...args: any[]): any;
    function FS_createDataFile(...args: any[]): any;
    function FS_preloadFile(parent: any, name: any, url: any, canRead: any, canWrite: any, dontCreateFile: any, canOwn: any, preFinish: any): Promise<void>;
    function FS_unlink(...args: any[]): any;
    function FS_createLazyFile(...args: any[]): any;
    function FS_createDevice(...args: any[]): any;
    function addRunDependency(id: any): void;
    function removeRunDependency(id: any): void;
}
interface WasmModule {
}

type EmbindString = ArrayBuffer|Uint8Array|Uint8ClampedArray|Int8Array|string;
interface EmbindModule {
  helloWorld(): string;
  addTree(): void;
  insertToTrees(_0: number): void;
  removeFromTrees(_0: number): boolean;
  findInTrees(_0: number): boolean;
  numOfTrees(): number;
  removeTree(_0: number): void;
  getTreeJson(_0: number): string;
  insertToTree(_0: number, _1: number): boolean;
  removeFromTree(_0: number, _1: number): boolean;
  findInTree(_0: number, _1: number): boolean;
  simulateBulkRace(_0: EmbindString): any;
}

export type MainModule = WasmModule & typeof RuntimeExports & EmbindModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
