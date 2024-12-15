> [!IMPORTANT]  
> It was decided to split distribution & building.  
> Distribution will be handled [here](https://github.com/lazy-eggplant/vs.autobuilds), while everything build-related is staying here.  
> This also includes docker images, since they are not meant for final users but developers of `vs` and its related subprojects.

## Supported platforms

At the moment, only Linux is supported for building and running `vs`.  
It is likely that all major CPU architectures are supported, and wider support for POSIX systems is also likely.

This is just temporary limitation, as all dependencies used by this project are highly portable, but my own code and the build system is probably not.

| **Platform**     | **Arch** | **Building** | **Running** |
| ---------------- | :------: | :----------: | :---------: |
| debian-trixie    |  amd64   |      ✅      |     ✅      |
| debian-trixie    |  arm64   |      ❓      |     ❓      |
| debian-trixie    |  risc64  |      ❓      |     ❓      |
| freedektop-24.08 |  amd64   |     ✅\*     |    ✅\*     |
| macos-13         |  amd64   |      ❓      |     ❓      |
| macos-14         |  arm64   |      ✅      |     ❓      |
| win64            |  amd64   |      ❌      |     ❓      |

### Available builds on release

- **deb** (not yet)
- **rpm** (not yet)
- **flatpak** (ongoing)
- **docker** container (ongoing)

## Building requirements

You will need a proper Linux environment, with a modern C++ toolchain installed.  
Specifically, I suggest `clang-19` or higher, as this repo is using modern C23 features like `#embed` to make everyone's (my) life a bit easier.
In addition to that, this repo makes use of:

- [meson](https://mesonbuild.com/) as its main build system. Any recent-ish version will do (unless you need `zig` to simplify cross-compiling; for that >= 1.60 is needed)
- [bun](https://bun.sh/) as the ts/js runtime to support all the code generation tasks and some of the more complex pipelines.  
   I hate bash, and this is what replaces it.
- [swiftc](https://www.swift.org/documentation/swift-compiler/) barely used for now, but many of the native components shipped within `vs` will be written in swift (or so I am planning). Swift 6 will be needed, but for now any version will do.

For now, you will need to install [some dependencies](https://github.com/fltk/fltk/blob/master/README.Unix.txt) to support FLTK.  
Specifically, the following are likely missing on most distributions:

- **libpng-dev**
- **libglu1-mesa-dev**
- **libpango1.0-dev**

There are also some more or less optional dependencies:

- **libcurl-dev**, unless you are trying to compile a custom version without network support, which is supported.
- [gnuplot](http://www.gnuplot.info/) if you want to generate plots from testing & benchmarking reports.
- **sqlite** is needed. If not provided by your system it will be automatically downloaded and distributed alongside `vs`.
- [flatpak-builder](https://docs.flatpak.org/en/latest/flatpak-builder.html) if you plan on packing and distributing a flatpak of `vs` (usually not needed, read more about it later in this document)

### IDE integrations

There is an ongoing effort to better support `vs` with a proper [lsp](https://github.com/lazy-eggplant/vs.lsp).  
In the same repo there is also a vscode client extension.

`vs` is expected to leverage this LSP directly in its embedded editor once integrated.

## Docker

A docker image is available that contains all the build dependencies. See
[docker/README.md](../docker/README.md)

## Building process

> [!NOTE]  
> This project uses meson, so there are no git submodules for the most part.
> One exception is made for `flatpak-builder` to be usable.  
> If you plan on using it to generate new flatpak images, please ensure submodules are also cloned with the rest of the repository.

Start by installing all the `bun` dependencies needed:

```bash
bun install
```

Then run the following NPM scripts:

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

> [!NOTE]  
> You need a modern version of `flatpak-builder` to be installed.  
> Some EOL or LTS will not ship with one. Anything > 1.4 should be ok.  
> Also, make sure your runtime is not using a cmake version in $[3.31.0,3.31.2)$.  
> A regression later fixed prevents building cmake via meson. Tracked [here](https://github.com/KaruroChori/vs-fltk/issues/46).

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
