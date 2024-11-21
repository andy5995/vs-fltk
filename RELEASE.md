## v0.1.1-alpha

This first alpha release is not meant to be usable in real-world scenarios, and documentation will not be there yet for the most part.  
The main objective is to implement & test most of the pipelines needed to:

- build `vs`
- install it
- the generation of its flatpak image

Still, `vs` is somewhat usable as a technical preview.  
For now, the building pipeline is not well documented and it can be hard to replicate. Also, installing it on most distributions might cause issues with your past or future dependencies. Still, you should be able to try it out via flatpak and the docker image provided by @andy5995.  
I am seeking feedback, so you are very welcome to test it along!

For reference, you can check what is the [intended scope](./README.md) of this project in terms of future developments and expected features.  
The [milestone](./docs/milestones.md) page also contains some related information.

This is what has been implemented so far (and some missing feature for context):

- Basic CLI features for the command `vs`. These are not expected to change for quite a while.
- Code generation infrastructure for fltk components and derived schemas.
- Infrastructure to handle the user sqlite database with proper schemas and migration.
- A small set of components from fltk being exposed, like buttons, labels, windows etc.
- Several features of `vs` exposed as XML elements:
  - [x] `mixin`
  - [x] embedded `script`
  - [ ] embedded `policies`
  - [x] `namespace`
  - [x] `import`
  - [x] `use`
  - [ ] caching directives
  - [ ] dynamic data
- Basic XML builder (no full caching, no multithreading)
- Templating engine for vs components. Integration is complete, but there is ongoing work tracked in a [separate repo](https://github.com/KaruroChori/vs-templ)
- Embedded scripts:
  - Support for `c` via tcc
    - [x] Modules
    - [x] Callbacks
    - [x] Props
    - [ ] Computed
    - [x] Dispatcher
    - [ ] Setters/getters
    - [ ] FFI to external libraries
    - [x] Debug interface
  - Support for `js` via quickjs
    - [ ] Modules
    - [x] Callbacks
    - [x] Props
    - [ ] Computed
    - [ ] Dispatcher
    - [ ] Setters/getters
    - [ ] FFI to external libraries
    - [x] Debug interface
- Very early and limited c bindings for `vs.fltk`, mostly for testing purposes.
- A simple in-memory caching for files, compiled scripts & parsed XML trees.  
  The more complex sqlite cache is yet to be implemented in code.
- An early (partial) implementation of policies and coarse-grained flags as a temporary stopgap.
- Install process mostly implemented in meson (the pre-build codegen step will stay in typescript).
- Functional flatpak build
- A docker image for development
