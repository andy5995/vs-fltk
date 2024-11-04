# Current

The memory footprint (RSS) went up from 4.5MB from two weeks ago or so, to about 19MB in this latest version. There is no good reason for that. for no good reason while implementing internal features.  
There is no clear reason why this happened out of nowhere, and the overall memory usage by heap/stack is still quite small. What is causing this massive increase?

- [x] Enable codegen
- [ ] and port the current code over the new codegen architecture.
- [ ] Viewport element.
- [ ] Fix on.failure and on.loading to match specs in `use` element.
- [x] Add properties on containers to set their type/mode whatever.
- [x] Extend support for more virtual paths (all but those based on sqlite which will be handled later on)
- [ ] Add namespaces for vs elements and fltk elements
- [ ] Add support for more widgets to make this thing usable
- [ ] Add support of loading of so/dylib/dll modules
  - [x] Test linking
  - [ ] On http/s & gemini
  - [ ] Without libc?
  - [ ] If nobuiltin is not usable, I need to add more of libtcc to meson targets.
- [x] Split align property
- [ ] Split font property
- [x] Expose some sort of inverse for apply_prop, where selected fields can be read from a widget.
- [x] Add support for env variables to fully disable native code, internet access and to run headless.  
       Once profiles are ready this will be obsoleted, but for now I need for testing and to let people use it "safely"
- [ ] Add pipelines for compilation & testing on github/gitea
- [ ] Policies are so complex to write manually and very easy to mess them up.
      Maybe code generation starting from a schema might be a better option.
- [x] Simplify codegen for all computed/props types which are not `raw` by writing more or the boilerplate.
- [ ] Basic LSP for the vs syntax. Implementation @ https://github.com/KaruroChori/vs-fltk-lsp
- [ ] Add special `inherit` value to props to remove the current one and let it be computed based on its parents? I have to decide if we want this one.
- [ ] Make some dependencies and their related features toggable (tcc, quickjs, lua, wamr).
- [ ] Decide what to do about swift.
- [ ] Fix building process of tcc to generate the minimal libraries needed.

- [ ] Caching
  - [ ] Source files
    - [x] From fs
    - [ ] From network
  - [ ] Pugi XML documents
  - [x] Scripts 

# Supported environments


## TCC

- [x] Add basic support for TCC
- [x] Provide extensive bindings for TCC
  - [x] Stage 0
  - [x] Stage 1 Full subset of libc
  - [x] Stage 2 Full interface for ui tree operations
  - [x] UI base support of module scripts
  - [x] UI base support of getters/setters.
- [ ] Add an alternative backend like Cello
- [ ] Add an alternative backend like Vala (?)

All done for now

## WASM/WASI

Why is WASM so frustrating to work with!?! No compiler can compile itself into a wasm/wasi binary.  
AssemblyScript is a mess that is barely able to generate itself with WASI, but still it requires binaryen distributed alongside.  
Because of a series of requirements, V8 is basically the only runtime capable of running that combo.  
An all LLVM versions for wasm/wasi I tried were failures (or half so).

I am starting to regret java bytecode already.

Notice: the wasmer registry seems to have a small collection of clang builds on it. It might be good enough to start experimenting.  
https://wasmer.io/taybenlor/runno-clang

- [x] Integrate an engine (WAMR)
- [ ] ~~Find a way to have llvm/clang running as wasm an build to wasm (no emscripten)~~ cannot do that
- [ ] ~~Find a way to integrate assemblyscript as a language and embeding the compiler~~ ahah. not working

## QuickJS

- [x] Add quickjs to the repo
- [x] Add basic support for quickjs embedded scripts.
- [x] UI base support of module scripts
- [ ] Support of getters and setters

## LUA

- [ ] Add Lua & LuaJIT to the repo
- [ ] Add baseline support for Lua embedded scripts.

# Documentation

- [x] Provide actual examples.

# Unclassified

## General

- [x] Refactor the ui_tree to better handle different classes of components
- [x] Prepare infrastructure for codegen (no impl yet)
- [x] Move all the tcc stuff in proper pipelines
- [x] Add more components to play with

- [ ] For speed of development, many operations are written recursively.
      The stack is pretty limited, so either there is tail recursion or stacks in heap must be used.
- [ ] Add caching & preloading to avoid making the process of building UI dog slow.
  - [x] Partial implementation to support modules
  - [ ] Partial implementation to load resources in memory within the session (they might be promoted once SQLITE gets integrated)

## References for FFI & Lua

- https://github.com/shajunxing/quickjs-ffi
- https://github.com/q66/cffi-lua
- https://github.com/LuaJIT/LuaJIT
- http://lua-users.org/wiki/SimpleLuaApiExample

Probably I will allow switching between Lua & LuaJIT with a compile flag.
