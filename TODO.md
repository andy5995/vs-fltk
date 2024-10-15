# Current
- [ ] Enable codegen and port the current code over
- [ ] Viewport tag
- [ ] Fix on.failure and on.loading to match with `use`
- [x] Add properties on containers to set their type/mode whatever
- [x] Add some documentation plz
- [x] Extend support for more virtual paths (all but those based on sqlite)
- [ ] Add support for more widgets to make this thing usable
- [ ] Add support of loading of so/dylib/dll modules
- [x] Split align property
- [ ] Split font property
- [x] Expose some sort of inverse for apply_prop, where selected fields can be read from a widget.
- [ ] Add support for env variables to fully disable native code, internet access and to run headless.  
      Once profiles are ready this will be obsoleted, but for now I need for testing and to let people use it "safely"
- [ ] Add pipelines for compilation on github/gitea

# Supported environments

## TCC

All done for now

- [x] Add basic support for TCC
- [ ] Provide extensive bindings for TCC
  - [x] Stage 0
  - [x] Stage 1 Full subset of libc
  - [x] Stage 2 Full interface for ui tree operations
- [ ] Add an alternative backend like Cello
- [ ] Add an alternative backend like Vala (?)

## WASM/WASI

Why is WASM so frustrating to work with!?! No compiler can compile itself into a wasm/wasi binary.  
AssemblyScript is a mess that is barely able to generate itself with WASI, but still it requires binaryen distributed alongside.  
Because of a series of requirements, V8 is basically the only runtime capable of running that combo.  
An all LLVM versions for wasm/wasi I tried were failures (or half so).  

I am starting to regret java bytecode already.

- [x] Integrate an engine (WAMR)
- [ ] ~~Find a way to have llvm/clang running as wasm an build to wasm (no emscripten)~~ cannot do that
- [ ] ~~Find a way to integrate assemblyscript as a language and embeding the compiler~~ ahah. not working

## QuickJS

- [x] Add quickjs to the repo
- [x] Add basic support for quickjs environments


# Documentation

- [x] Provide actual examples.

# Unclassified

## CFLTK
~~CFLTK is likely going to be removed as a dependency.~~ removed  
~~There is no need nor desire to expose it directly as it was originally meant.~~

- [ ] ~~add the patch file to remove calls to that single x11 function `fl_x11_gc`.~~
      ~~It would be nice to understand what is going wrong there.~~
- [ ] ~~add `./scripts/get-cfltk.ts` to some kind of pipeline~~

## General
- [x] Refactor the ui_tree to better handle different classes of components
- [x] Prepare infrastructore for codegen (no impl yet)
- [x] Move all the tcc stuff in proper pipelines
- [x] Add more components to play with

- [ ] For speed of development, many operations are written recursively.
      The stack is pretty limited, so either there is tail recursion or explicit stacks must be used.
- [ ] Add caching & preloading to avoid making the process of building UI dog slow.
