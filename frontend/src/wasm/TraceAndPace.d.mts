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
export interface ClassHandle {
  isAliasOf(other: ClassHandle): boolean;
  delete(): void;
  deleteLater(): this;
  isDeleted(): boolean;
  // @ts-ignore - If targeting lower than ESNext, this symbol might not exist.
  [Symbol.dispose](): void;
  clone(): this;
}
export interface doubleVector extends ClassHandle, Iterable<number> {
  push_back(_0: number): void;
  resize(_0: number, _1: number): void;
  size(): number;
  get(_0: number): number | undefined;
  set(_0: number, _1: number): boolean;
}

interface EmbindModule {
  helloWorld(): string;
  addAVLTree(): void;
  addBTree(): void;
  addSplayTree(): void;
  insertToTrees(_0: number): void;
  removeFromTrees(_0: number): boolean;
  findInTrees(_0: number): boolean;
  numOfTrees(): number;
  removeTree(_0: number): void;
  executeOneAction(_0: EmbindString, _1: number, _2: number): void;
  doubleVector: {
    new(): doubleVector;
  };
  runBulkCommands(_0: EmbindString): doubleVector;
  insertCSV(_0: EmbindString, _1: boolean, _2: boolean): void;
  getWholeView(_0: number, _1: number): string;
}

export type MainModule = WasmModule & typeof RuntimeExports & EmbindModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
