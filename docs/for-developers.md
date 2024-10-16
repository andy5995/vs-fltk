## Building requirements
You will need a proper Linux environment, with a modern C++ toolchain installed. In addition to that, this repo makes use of:
- [meson](https://mesonbuild.com/) as its main build system
- [bun](https://bun.sh/) as the ts/js runtime to support some codegen and the more complex releasing pipelines. I hate bash, and this is what replaces it.
- [libcurl] unless you are trying to compile a release without.

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

To run my dev demo:
```bash
bun run vs
```

`meson-install` is not implemented yet. `meson-release` works, but it is only meant for automatic pipelines.

### Patches
If not using a precompiled version of sqlite on your system, the `meson.build` of its amalgamate might need patching:
```
  override_options: ['c_std=gnu23'],
```
at the end of the library generation. The issue is tracked [here](https://github.com/mesonbuild/wrapdb/issues/1747)

## Structure of the repo
- **src** where 
- **include**
- **test**
- **experiments**
- **components**
- **docs**
- **bindings**
- **scripts**
