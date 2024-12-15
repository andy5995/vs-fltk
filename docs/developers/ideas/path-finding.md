Order of search for component when the direct `fullname` is not found. `name` is `fullname` reduced of its last extension.

First attempt is to look for the exact name:

- `fullname`

If it fails, check for any the following in such order:

- `name.vs`
- `name.xml`
- `name.wasm`
- `name.riscv`
- `name.[arch].so` | `name.[arch].dll` | `name.[arch].dylib`
- `name.c`
- `name.md`

If it fails and the folder `name` exists, check for these ones:

- `name/main.vs`
- `name/main.xml`
- `name/main.wasm`
- `name.riscv`
- `name/main.[arch].so` | `name/main.[arch].dll` | `name/main.[arch].dylib`
- `name/main.c`
- `name/main.md`

## Loading pipelines

- `.vs` & `.xml` files are either loaded directly, or after being compiled from `vs.templ`.
- `.wasm` files are handled as wasm components by wamr
- `.so` & al. are loaded as native components
- `.c` are compiled by tcc into native components and loaded as such
- `.md` is loaded directly as a component
