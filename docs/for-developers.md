## Building requirements

You will need a proper Linux environment, with a modern C++ toolchain installed.  
Specifically, I suggest `clang-19` or higher, as this repo is using modern C23 features like `#embed` to make everyone's (my) life a bit easier.
In addition to that, this repo makes use of:

- [meson](https://mesonbuild.com/) as its main build system. Any recent-ish version will do (unless you need `zig` to simplify cross-compiling; for that >= 1.60 is needed)
- [bun](https://bun.sh/) as the ts/js runtime to support all the code generation tasks and some of the more complex pipelines.  
   I hate bash, and this is what replaces it.
- [swiftc](https://www.swift.org/documentation/swift-compiler/) barely used for now, but many of the native components shipped within `vs` will be written in swift (or so I am planning). Swift 6 will be needed, but for now any version will do.

For now, you will need to install [some dependencies](https://github.com/fltk/fltk/blob/master/README.Unix.txt) to support FLTK.  
Specifically `libpng-dev` & `libglu1-mesa-dev` are likely missing on most distributions.

There are also some more or less optional dependencies:

- **libcurl-dev**, unless you are trying to compile a custom version without network support, which is supported.
- **sqlite** is needed. If not provided by your system it will be automatically downloaded and distributed alongside `vs`.
- [flatpak-builder](https://docs.flatpak.org/en/latest/flatpak-builder.html) if you plan on packing and distributing a flatpak of `vs` (usually not needed, read more about it later in this document)

### IDE integrations

There is an ongoing effort to write and release a minimal LSP and vscode extension.  
Once it is ready enough TODO add link here.

## Docker

A docker image is available that contains all the build dependencies. See
[docker/README.md](https://github.com/KaruroChori/vs-fltk/docker/README.md)

## Supported platforms

At the moment, only Linux is supported or tested, probably on any of the major CPU architectures. Wider support for POSIX systems is also likely.  
This is just temporary limitation, as all dependencies are portable, but my own code is probably not.

| **Platform**     | **Arch** | **Building** | **Running** |
| ---------------- | :------: | :----------: | :---------: |
| debian-trixie    |  amd64   |      ✅      |     ✅      |
| freedektop-24.08 |  amd64   |     ✅\*     |    ✅\*     |
| macos-13         |  amd64   |      ❓      |     ❓      |
| macos-14         |  arm64   |      ❓      |     ❓      |
| win64            |  amd64   |      ❌      |     ❓      |

## Building process

> [!NOTICE]  
> This project uses meson, so there are no git submodules for the most part. 
> One exception is made for `flatpak-builder` to be usable.  
> If you plan on using it to generate new flatpak images, please ensure submodules are also cloned with the rest of the repository.

Start by installing all the `bun` dependencies needed:

```bash
bun install
```

Then run the following npm scripts:

```bash
bun run codegen                     #Initial codegen from schemas
bun run meson-setup.release         #Set up the meson build directory.
```

You might want to use `meson-setup.clang-release` to use clang-19 if found on your system, and your default compiler or choice might not support some modern functions used in this project.

To perform tests and benchmarks:

```bash
bun run test
bun run benchmark
```

To run the dev demo where features under development are usually being tested:

```bash
bun run vs.example
```

## Installation

> [!WARNING]  
> Don't install it via `meson install ...` without a custom `DESTDIR` set, as shown by its wrapper in `package.json` script.  
> Some library names will most surely clash with those already installed on your system, and will be overridden.  
> Or future system updates might break `vs`.  
> `vs.fltk` is using very recent versions which have not been rolled out yet in most distribution. Or even worse, custom forks.

`bun run meson-install` is now implemented & tested. By default, it installs everything in `./build/dist`.

### Flatpak

Flatpak is not really the ideal approach to deliver `vs` due to the intrinsic cost of having a separate runtime.  
Also, its sandboxing & permission system might raise some issues (but also help to mitigate the current lack of safety features).  
Still, it is good to support it at these early stages, since many libraries & build dependencies are bleeding-edge, and other forms of distribution are not as convenient.

```sh
flatpak --user remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
```

if you never configured `--user` on your system.  
Then

```sh
bun run codegen           #If not run already
bun run flatpak-builder
```

## A word about code generation

A significant portion of code in this repository is generated automatically and does not ship with your `git clone`.  
Make sure you run `bun run codegen` before you attempt any further step with meson.  
I will probably integrate it as part of the meson setup step, but I am still evaluating the potential drawbacks.

The main source for automatic code generation is located in `/schemas` and are JSON files. _Component definitions_ are compiled down into C++ classes, c bindings, typescript type definitions, XSD/RND schemas and XML data used in the embedded editor of `vs`.  
Any component shipped with `vs` (not those externally distributed in `/components`) must have a JSON schema definition, even if their class definition is not automatically generated. In that case the property `use_main_header` is set, but _props_ and _computed values_ are still defined via JSON schemas.

During development, you might want to use `codegen.quick` in place of `codegen`, as long as you are not altering the files involved or their naming. Some changes in schemas can also be incompatible with `codegen.quick`, so if you don't observe what you expect, just perform a full refresh.  
While imperfect, this quick variant avoids the reconfiguration of the full meson project, making your next build much faster.

### Naming conventions

- names following the pattern `**.autogen.*` are full c/cpp/h/hpp files to be included, compiled and/or linked.
- names following the pattern `**.autofrag.*` are fragments introduced in the source of other files only. They are not directly listed in `meson.build`

In either case, such files are not tracked by git.

## Structure of the repo

- **src** where most of the source for **vs.app** and the **vs.fltk** library are located.
- **include** like before for the header files. Directories are mostly mirrored.
- **test** test suite for **vs** & **vs.fltk** library.
- **experiments** playground where new ideas or semi-standalone prototypes are tested.
- **components** native components to ship alongside **vs** but not embedded in **vs**.
- **docs** this documentation.
- **bindings** bindings for all languages supported in embedded `script`.
- **scripts** utility scripts (mostly in TS/JS) for the building process and distribution.
- **schemas** high level specs, souinrce of information for documentation and automatic code-gen.
- **commons** extra public files (some auto-generated) which are part of every **vs** distribution.

## Variables of environment

They are frequently used for both _benchmarks_ and _tests_.  
They can also be useful for the developer while testing new functionality, so they have been all covered in [here](./env-vars.md) for reference.

## Programming guidelines

### About exceptions

Exceptions are fully allowed in the CLI at `/src/app`.  
However, they are strongly discouraged anywhere else in the library code, and it is possible they will be fully disabled via `-fno-exceptions` at some point. The main reason is that the UI should be fault-tolerant and provide as much functionality as possible even if parts of it are broken, like malformed XML files, some scripts failing compilation, missing resources and so on. Exceptions, in this sense, are internally handled and semantically supported by providing subtrees to be rendered in case of failure events.  
Exceptions should only be used in those cases when the application **must** stop, either because the error is not recoverable or because it would leave the rest of the application in an inconsistent state.

### Memory allocations

As for memory allocations, spawning small dynamic objects is also discouraged. If possible, stack allocations are a better alternative. Arrays with variable length on stack are totally fine to be used in place of local objects allocated on heap.  
`std::string` is also highly discouraged, make sure `std::string_view` is used instead whenever possible.

### About log levels

Internally these are the log levels supported and their relative semantics:
- **INFO** to present informational messages, often as a result of user requests
- **OK** to notify that a certain operation completed successfully.
- **WARNING** to notify that a certain operation was not able to fully succeed, something should be fixed, but everything is fine. 
- **CONTINUE** to notify that a certain operation was skipped by design.
- **PANIC** to notify that a certain operation failed in a way which cannot be recovered (but the application can still run)
- **LOG** to introduce a log without any further connotation.

For example, loading a remote resource:
- If the file was already cached, a `continue` message will be notified
- If no proper answer has been received from the remote host, a `panic` message is generated, since that content cannot be recovered
- If everything loaded fine, an `ok` message can be used
- If the component is using features which are not allowed by policies, `warning` messages can be generated.

Log levels are:
- **NORMAL** to be emitted when running the program normally
- **SILENT** to be emitted when running the program even in silent mode
- **VERBOSE** to be emitted when running the program in verbose mode only
- **DEBUG** to be emitted always when running a debug build or in debugging mode.

Each messagge must specify their semantic type and log level (normal by default). Usually:
- `panic` messages have a `silent` log level
- `ok` and `continue` messages are usually `verbose`
- `info`, `warning` and `log` are usually `normal`

## Logging

### Debug logging

**vs** has some features to simplify debugging, mostly to support automatic tests and benchmarks, but they might be useful in other scenarios as well.
`vs::globals::debug` is responsible for that, and it is exposed in several ways:

- within embedded scripts
- via a special xml `debug` tag
- in the `vs.fltk` C interface as well
  While using it you can define a key and value. The current timestamp at nanoseconds resolution is also automatically recorded.  
  Records are saved to a file with name `VS_DEBUG_FILE` if set, otherwise no output will be emitted. Older content is destroyed.  
  The file format is just a simple CSV with horizontal tabs as separator of fields and newlines for rows. The order is _key_, _value_ & _timestamp_.

`vs::globals::debug` should not be confused with the ordinary logging functions which are also exposed in similar ways, but which are generally contextual and they mostly output to `stdout`.

## Testing
