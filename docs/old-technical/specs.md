## app & component
Both `app` and `component` are supporting virtually the same underlying structure, and they only differ as one can be root for an application, the other does not.
### Attributes
- `app.token` unique id for this app, used to separate pages in cache. It is a 512bit key expressed in hex notation. If set to `""` a random one will be assigned. Unlike `component`, `app` must define it as it cannot inherit and expose the secret user token directly.
### Special children
- `metadata` contains sub-field for information about the author, licence etc. It is made available as map to the entire application
- `script.global` contains sub-tags `decl` and `impl`. Each will have the same fields as normal `script`. Only one for each is allowed, and only one `script.global` can be defined per `app`/`component`
- `import` with attribute `src.*` and `as`
- `policies` TBD
### Normal children & attributes
Both `app` & `component` can contain any tag or attribute as the other container-like tags.

## container-like components
