## Building requirements

You will need a proper Linux environment, with a modern C++ toolchain installed.  
Specifically, I suggest `clang-19` or higher, as this repo is using modern C23 features like `#embed` to make everyone's (my) life a bit easier.
In addition to that, this repo makes use of:

- [meson](https://mesonbuild.com/) as its main build system. Any recent-ish version will do (unless you need `zig` to simplify cross-compiling; for that >= 1.60 is needed)
- [bun](https://bun.sh/) as the ts/js runtime to support all the code generation tasks and some of the more complex pipelines.  
   I hate bash, and this is what replaces it.
- **libcurl-dev**, unless you are trying to compile a custom version without network support, which is supported.

At the moment, only Linux is supported or tested, probably on any of the major CPU architectures. Wider support for POSIX systems is also likely.  
This is just temporary limitation, as all dependencies are portable, but my own code is probably not.

### Optional features

There is an ongoing effort to write and release a minimal LSP and vscode extension. Once it is ready TODO add link here.

## Building process

Start by installing all the `bun` dependencies needed:
```bash
bun install
```

Then run the following npm scripts:

```bash
bun run codegen             #Initial codegen from schemas
bun run meson-setup         #Set up the meson builddir
```

You might want to use `meson-setup.clang` to use clang-19 if installed in your system, and the default compiler might not support all the modern features needed.  


To perform tests and benchmarks:

```bash
bun run test
bun run benchmark
```

To run the dev demo where features under development are tested:

```bash
bun run vs.example
```

`meson-install` is not implemented yet. `prepare-release` works, but it is only meant for automatic pipelines at this stage.

### Patches

Notice: this issue was only observed with `zig` as the backend and cannot be easily reproduced.
If not using a precompiled version of sqlite on your system, the `meson.build` of its amalgamate might need patching:

```
  override_options: ['c_std=gnu23'],
```

at the end of the library generation. The issue is tracked [here](https://github.com/mesonbuild/wrapdb/issues/1747)

## A word about code generation

A significant portion of code in this repository is generated automatically and does not ship with your `git clone`.  
Make sure you run `bun run codegen` before you attempt any further step with meson. I will probably integrate it as part of the meson setup step.

The main source for automatic code generation is located in `/schemas` and are JSON files. *Component definitions* are compiled down into C++ classes, c bindings, typescript type definitions, XSD/RND schemas and XML data used in the embedded editor of `vs`.  
Any component shipped with `vs` (not those externally distributed in `/components`) must have a JSON schema definition, even if their class definition is not automatically generated. In that case the property `use_main_header` is set, but *props* and *computed values* are still defined via JSON schemas.

During development, you might want to use `codegen.quick` in place of `codegen`, as long as you are not altering the files involved or their naming. Some changes in schemas can also be incompatible with `codegen.quick`, so if you don't observe what you expect, just perform a full refresh.  
While imperfect, this quick variant avoids the reconfiguration of the full meson project, making your next build much faster.

## Structure of the repo

- **src** where most of the source for **vs.app** and the **vs.fltk** library are located.
- **include** like before for the header files. Directories are mostly mirrored.
- **test** test suite for **vs** & **vs.fltk** library.
- **experiments** playground where new ideas or semi-standalone prototypes are tested.
- **components** native components to ship alongside **vs** but not embedded in **vs**.
- **docs** this documentation.
- **bindings** bindings for all languages supported in embedded `script`.
- **scripts** utility scripts (mostly in TS/JS) for the building process and distribution.
- **schemas** high level specs, source of information for documentation and automatic code-gen.
- **commons** extra public files (some auto-generated) which are part of every **vs** distribution.

## Debug logging

**vs** has some features to simplify debugging, mostly to support automatic tests and benchmarks, but they might be useful in other scenarios as well.
`vs::globals::debug` is responsible for that and it is exposed in several ways:
- in embedded scripts
- via a special xml `debug` tag
- in the `vs.fltk` C interface as well
While using it you can define a key and value. The current timestamp at nanoseconds resolution is also automatically recorded.  
Records are saved to a file with name `VS_TEST_DEBUG` if set, otherwise no output will be emitted. Older content is destroyed.  
The file format is just a simple csv with horizontal tabs as separator of fields and newlines for rows. The order is *key*, *value* & *timestamp*.

`vs::globals::debug` should not be confused with the ordinary logging functions which are also exposed in similar ways, but which are generally contextual and they mostly output to `stdout`.

## Environment Variables
They are frequently used for both *benchmarks* and *tests*.  
They can also be useful for the developer while testing new functionality, so they have been all covered in [here](./env-vars.md) for reference.
