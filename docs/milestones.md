## 0.1.1-alpha

- [ ] Implement flex, grid, scroll & pack via codegen.
- [ ] Tidy up the XML caching
- [ ] Tidy up the JS scripting interface.
- [x] Complete the build process
- [ ] Docker image
- [x] Flatpak building pipeline
- [ ] Prepare pipeline for github actions

## 0.1.5

- [x] Move all components to the new codegen format
- [x] Support for script modules
- [ ] Full support for getters and setters
- [x] Final implementation of the static xml builder (now external dependency)
- [ ] Full infrastructure for benchmarking & tests implemented
- [x] Support basic policies via env flags

## 0.2.x

`vs` must be a minimum viable product.

- Fully functional c & js support in embedded scripts.
  - DOM-like operations (constained due to its mostly immutable structure).
  - Constrained operations on files.
  - Constrained fetch requests.
- Loading of native components.
- HTTP/HTTPS ~~& gemini~~
- A good coverage of basic fltk components via codegen.

## 0.3.x

- [x] Caching engine.
- [ ] Permanent storage.
- [ ] Session storage.

## 0.4.x

- Fast parser (support multithreading in first pass).

## 0.5.x

- Support policies.
- Support themes.
- Support profiles.

## 0.6.x

- Support for unix domain sockets for the `external` mode.
- Library of native components.

## 0.7.x

- Support for wasm scripts.

## 0.8.x

- Support for wasm components.
