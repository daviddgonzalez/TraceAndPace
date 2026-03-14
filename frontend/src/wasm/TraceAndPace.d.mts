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

interface EmbindModule {
  helloWorld(): string;
}

export type MainModule = WasmModule & typeof RuntimeExports & EmbindModule;
export default function MainModuleFactory (options?: unknown): Promise<MainModule>;
