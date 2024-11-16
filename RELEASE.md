## v0.1.1-alpha

This first alpha release is not meant to be usable in a real-world scenario nor sufficiently documented.  
The actual objective is to implement & test the full pipeline which is involved in:
- building
- installing
- generating flatpak (possibly packages as well)
It is just needed to implement a complete building & testing pipeline to support future releases.  

Still, `vs` is somewhat usable as a technical preview.  
For now the build system is not well documented and hard to replicate. Also, installing it on most distribution might cause issues with your dependencies.  Still, you should be able to try it out via flatpak.  
I am seeking feedback, so you are very welcome to test it along!

For reference, you can check what is the [intended scope](./README.md) of this project in terms of future developments and expected features.  
The [milestone](./docs/milestones.md) page page also contained some related information.  

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
- Templating engine for vs components. Integration is complete, but there is ongoing work tracked at [separate repo](https://github.com/KaruroChori/vs-templ)
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
- Very early and partial c bindings for `vs.fltk`.
- A simple in-memory caching for files, compiled scripts & parsed XML trees. The more complex sqlite cache is yet to be implemented in code.
- An early (partial) implementation of policies and coarse-grained flags as a temporary stopgap.
- Install process mostly implemented via meson.
- Functional flatpak build