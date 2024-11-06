## General questions

While `vs` shares many of its technologies and ideas with modern SFC frameworks, its design is significantly more opinionated and less free.  
This document addresses several of these issues and provides some of the implied rationale.

### Can we have something like `<canvas>`?

No. You are free to add or use a custom native component if you really want. And there is support for arbitrary SVG files already as part of the `fltk` library. But canvas-like components are not and will never be part of the core components shipped with `vs`. Because of that such capabilities are not meant to be universally accessible.  
The reason is that canvas-like components can be abused to replace UI primitives, and as a result break the semantics on which accessibility tools and robots are based on. Furthermore, to be useful we should also allow arbitrary execution of code in embedded scripts that is generally discouraged.

### Can we have a webgpu, opengl, vulkan context?

No, same as for `<canvas>`.

### Why C in embedded scrips? It is unsafe!

Yes, you are basically giving all keys to the XML provider to run arbitrary code on your machine.  
Even if `vs` is not exposing direct access to the fs or network to embedded scripts, arbitrary C code compiled in there will be able to do arbitrary things still.  
It is understandable that from the point of view of "web development" this idea is absurd. But `vs` is not about web development, it is designed to provide native UIs. You can load XML components and resources from the internet & gemini, but you should only do that for trusted sources, quite possibly some you have control over.  
And still, it would be not less safe than just compiling and running some random code from github.  
In comparison, QuickJS, Lua and especially WAMR do offer a much more sandboxed experience.  
In the future it will be possible to set up robust policies based on domains to determine which type of scripts should be allowed if any.

For now there are some coarse [environment variables]() that can be set fully disable certain features.

### Can it be used for fully standalone applications?

Ideally yes. Broadly speaking there are three "modes" in which `vs` can operate:

- As a fully standalone application. The main issue being the limited selection of functions which are made available in embedded scripts, and their sandboxed nature which only allows specific types of symbols to be exposed and shared.  
  Keep in mind that even while working in C, as unsafe as it is you are not given direct full access to the standard library, and this is a design choice to limit the scope of custom code in scripts.
- As the front end of a dynamically linked library which embeds all complex functionalities and provides hooks for the UI in `vs`. Events triggered from UI will reflect as calls to features exposed in your library. Useful for event driven applications, like providing a simple visual front end to your CLI.
- As the front end of a separate process, sharing information via UNIX Domain Sockets. Useful for multi-client applications, or those in which the bulk of the operations are not event driven. It also avoids the need to compile your codebase into a dynamic library, allowing it to interoperate with virtually any runtime.

For now, most efforts are focused in making the first two options viable. The third one is planned for later.

### How to map SFC concepts in here?

| Concept            | Web                                | VS                              |
| ------------------ | ---------------------------------- | ------------------------------- |
| Components         | XML or HTML template               | XML                             |
| Static templating  | XSLT or JSX                        | XSLT-like approach              |
| Dynamic templating | alpine, htmx, jquery, ...          | Restricted, standard components |
| Style              | XSLT and/or CSS and transpilation  | Mixins                          |
| Scripting          | JS, WASM and transpilation         | C/JS/Lua/WASM                   |
| Data handling      | Limited via forms, JSON/XML via JS | Native `data` component         |

All flow control features and templating like `if` or `for-each` blocks which are typical in frameworks like [vue](https://vuejs.org/) are performed either:

- As a build step for components if they can be evaluated statically.
- Via custom elements and are not part of basic `vs` for a dynamic behaviour.  

This comparison is just meant to provide some basic understanding of how things are matched.  
Still, the design patterns between `vs` and more traditional SFC frameworks are quite different.

### But I want a CSS-like experience!

In theory there is nothing preventing `vs` to integrate some style equivalent with mixins being rewritten as CSS-like classes.  
If you want to do that feel free to send a PR, I am more than willing to see it integrated.

### Theming and colors schemes. What can I do?

FLTK, the toolkit on which `vs` is based, has limited capabilities in this respect.  
Out of the box, it makes opinionated choices on what and how to render boxes based on its class, and of there are just a few embedded in the engine.  
Hence, both indexed colors and frame types have their semantics enforced by FLTK and are globally shared within each process.  
Given these constraints, one can specify any color or any rendering function for the different frame types (even SVG images). Yet it is not possible to introduce new classes out of thin air.

It is possible to define custom draw functions and adopt mixins to override the default style, but doing it more than occasionally would result in some quite messy code. If you need fine and frequent control over your UI, `vs` is likely not the right framework for you.

### Don't we have fluid already for FLTK?

[fluid](https://www.fltk.org/doc-1.4/fluid.html) is an excellent tool, but in my opinion it has few problems:

- The visual editor is nice, but it does not really allow the definition of custom widgets as standalone and reusable components. At least, not easily enough.
- It integrates C/C++ code only to "fill in" the missing parts of the application, in a way which I don't find particularly ergonomic.
- It does nothing to provide runtime capabilities for the UI beyond what FLTK offers, which is a bit too basic.
- It is restricted to C/C++, unless you linked your own dynamic library written with a language of choice.

### Is there any plan to support alternative backends to fltk?

Short answer, no. However, most of the infrastructure and code of this repo has only minimal reliance over fltk.  
It is possible for more and more of this code to be exported in a generic `vs` repo, and promote alternative backends from there.  
One scenario which might be interesting to consider is [lgvl](https://lvgl.io/) for example.  

## Likely pitfalls

Unlike other runtimes adopting DOM-like structures, `vs` is not using XML nor its parsed version in memory as the actual UI representation.  
The XML source is statically compiled into a UI tree, a wrapper/superset of the tree structure that FLTK internally implements, enabling all the higher level features of `vs`.  
This means that at runtime the source XML will we de-allocated from memory right after the UI tree is built, and it cannot be trivially referenced again.  
We also plan in a not so far distant future to use the XML file to generate the equivalent C++ code, and use that to speed up the component instantiation even further.

> How do I create a list which can spawn multiple copies of some component based on runtime data in XML then?

You don't. Components based on XML have several limitations on which mutations are allowed during runtime. A basic principle of this framework is to keep arbitrary code as far away as possible from the final user, and what one sees in code should perfectly match what one visually gets. Making the tree mostly immutable and restricting which operations are allowed in the embedded scripts vibes with this vision of simplicity.  
In addition to XML components, native ones are also allowed in `vs`. Native components are written in C++ (or any language which can work with our C bindings) & built dynamically against the full library.  
Native components cover the same features provided by the XML builder and much more. There is (will be) a library of base native components designed to cover all most common scenarios where dynamic data is driving UI changes, but if yours does not fit you will have to write a custom one.
Realistically, once this software gets stable enough, the standard library should be plenty for virtually any developer.

This way any dynamic behaviour requiring arbitrary code execution is kept outside the XML tree, in a native codebase which is not controlled by any single developer (unless the final user decides to enable certain native components distributed by third parties). This fits with the design philosophy that there should only one good way to implement a certain feature on the UI frontend.

> Ok, but being forced to manually write the code for 5x5 buttons on a widget is not really clever.

This is what build steps are for! You can generate your xml in [react](https://react.dev/) or [xslt](https://www.w3schools.com/xml/xsl_intro.asp) for example.  
We are also implementing our [own xml preprocessor](https://github.com/KaruroChori/vs-templ) to statically generate components within the `vs` viewer itself. Right now `vs.templ` is working in `vs`, but not every feature has been implemented yet.

> If the XML is lost, how to I know & query the state of widgets?

You generally don't. In theory FLTK provides an interface to get the current value of its properties from its own widgets, however we decided not to expose it fully.  
Still, vs widgets can provide some selected computed properties. It should be considered that props in the XML and keys used by FLTK are not a 1-to-1 match, even if we try to keep them as close as possible if reasonable; some complex processing on the input expressions can take place both at compile time and at runtime. None of that is directly reflected in the state of an FLTK widget.

Aside from these considerations, can still keep state, just not within vs widgets. There are three main ways, and different scenarions will call for different approaches:

- As variables in single user scripts (modules would require some tricks to achieve the same result).  
  They can be exported and accessed via `setters` and `getters`.  
  This approach is quite general, but also more powerful than what one would generally need.
- In a self mixin, whose content is fully populated during compilation, yet readable at runtime.
- As part of a `data`.
- As entries in cache (which are possibly persisted beyond the current session's lifetime).

I would like to close with an example showing a typical pitfall in handling state:

```xml
<component-a prop-1="value-1">
    <mixin prop-2="value-2">
</compoent-a>
```

`component-a` will be built with its own `prop-1` and `prop-2` inherited from the self mixin. However, the content of `prop-1` is applied & lost after compilation, while `prop-2` can be queried at runtime. Mixins can be mutated at runtime as well, but an explicit update request must be triggered onto a component for it to be updated & reflect such changes. (TODO: Carefully consider this)

> What is mutable in a runtime instance?

- Viewports can be fully redrawn and re-sourced.
- Props of ui widgets can be freely changed. Exclusions apply to `name` and few more which are reserved for internal use during the `ui-tree` construction.
- ~~Mixins can be added, removed or changed. Subtrees can be asked to recompute their style info after these changes.~~ (am I sure I want this?)

What is left out:

- You cannot re-render components imported and used or direct `use` as their slot designation is lost at runtime.
- Like all the other props, except for `name`, the `mixin` property of a widget is mutable, but the previous state is not stored. So you cannot just append or remove parts from it, it must be fully overwritten each time.
- When you want a widget to be refreshed, its inline styling cannot be evaluated again, as that is only available while parsing XML. If you want to avoid this pitfall, keep all of its style in external mixins, and just mutate its `mixin` list to set the desired visual state.

## Technical details

### How does the rendering work?

It is a process in two stages for each component:

- The first pass checks for all resources needed and scripts to compile. These sub-tasks are added to a pool and run in parallel. If cached, these results are retrieved. If cache-able, they are stored after computation.
- Once all resources for a component are resolved, the UI tree is constructed visiting the XML again with a depth-first pass.

At the end of this process the XML is freed.  
XML files are assumed to be fully loaded in memory during processing. However, it is possible to write a streaming parser with much lower memory footprint, as long as the XML is following the strict schema shipped with this repo (it has a rigid order for some tags).  
This strategy was not employed as it makes the loading process much slower for multithreaded systems with reasonable amounts of memory, and would only be better for low-end embedded systems where fltk is unlikely to be suitable.
