## v0.1.1-alpha

This alpha release is not meant to be usable nor well documented.  
It is just needed to implement a complete building & testing pipeline to support future releases.  

Still, `vs` is somewhat usable as a technical preview, as long as you are able to replicate its build process locally.  
I am seeking feedback, so you are very welcome to test it along.

You can check what is the bigger picture in terms of future developments and expected features on the [readme](./README.md)) and the [milestone](./docs/milestones.md) page.
This is what has been implemented so far (and some missing parts for context):

- Basic CLI features for the command `vs`. These are not expected to change for quite a while.
- Code generation infrastructure for fltk components and derived schemas.
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
        - [x] Computed
        - [x] Dispatcher
        - [ ] Setters/getters
        - [ ] FFI to external libraries
    - Support for `js` via quickjs
        - [ ] Modules
        - [x] Callbacks
        - [x] Props
        - [ ] Computed
        - [ ] Dispatcher
        - [ ] Setters/getters
        - [ ] FFI to external libraries
- Very early and partial c bindings for `vs.fltk`.
- A simple in-memory caching for files, compiled scripts & parsed XML trees. The more complex sqlite-based cache has no implementation yet.
- An early (partial) implementation of policies and coarse-grained flags as a temporary stopgap.
