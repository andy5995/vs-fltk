## Building requirements
You will need a proper Linux environment, with a modern C++ toolchain installed. In addition to that, this repo makes use of:
- [meson](https://mesonbuild.com/) as its main build system
- [bun](https://bun.sh/) as the ts/js runtime to support some codegen and the more complex releasing pipelines. I hate bash, and this is what replaces it.

At the moment only Linux is supported, regardless or CPU architecture. Possibly some more UNIX-like systems.  
This is just temporary limitation, as all dependencies are portable, but my own code is not.

## Building process
Run the following npm scripts:
```bash
bun run codegen             #Initial codegen from schemas
bun run meson-configure     #Set up the meson builddir
bun run vs                  #Compile and run the main application with the demo xml.
```

To perform tests:
```bash
bun run meson-test
```

`meson-install` is not implemented yet. `meson-release` works, but it is only meant to be used by the automatic pipelines.

## Structure of the repo
- **src**
- **include**
- **test**
- **experiments**
- **components**
- **docs**
- **bindings**
- **scripts**
