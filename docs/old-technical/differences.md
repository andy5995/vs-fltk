While `vs` shares many of its technologies and ideas with modern SFC frameworks, its design is significantly more opinionated and less free.

### Can we have something like `<canvas>`?

No. You are free to add or use a custom native component if you really want. And there is support for arbitrary SVG files already as part of the `fltk` library. But canvas-like components are not and will never be part of the core components shipped with `vs`. Because of that such capabilities are not meant to be universally accessible.  
The reason is that canvas-like components can be abused to replace UI primitives, and as a result break the semantics on which accessibility tools and robots are based on. Furthermore, to be useful we should also allow arbitrary execution of code in embedded scripts that is generally discouraged.

### Can we have a webgpu, opengl, vulkan context?

No, same as for `<canvas>`.


