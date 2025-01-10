## v0.1.3-alpha

This second alpha release had not much progress compared to `v0.1.1-alpha` in terms of exposed functionality.  
However, there have been many improvements, bug fixes and architectural changes to allow future features.

The main objectives for this release are:

- To improve the build system, moving the generation of distributable formats in a separate repo.
- Revise the code-generation of widgets.
- Better serialization/de-serialization of XML props & revised getters/setters
- To offer a wider range of fltk widgets in the runtime.
- Documentation efforts, not just in the docs themselves but also in distribution. We now have docs generation from code and markdown.
- A complete implementation of the XML templating backend, which was [split in its own project](https://github.com/lazy-eggplant/vs.templ).  
  `vs.templ` is now feature complete, or at least much more so than its parent project.

### What's new?

This is what has been implemented so far (some missing feature are reported just for context):

- [ ] Improvements to the build system
  - [ ] migration of distribution-related features to [external repo]()
  - [ ] fix linking to follow https://github.com/KaruroChori/vs-fltk/issues/63
  - [ ] debian and arch packages are now supported
  - [ ] install script can be now be safely used
  - [x] specific features of `vs` can be excluded from building.
  - [x] CI is now faster.
- [ ] New widgets & improvements
  - [ ] flex
  - [ ] grid
- [ ] Updated code-gen
  - [x] Use the new schema
- [ ] Updated core features
  - [x] Removed most of the globals & reworked singletons
  - [x] Reworked `ui_tree` interface to be more generic and not only viable for xml trees.
  - [x] Added guards to decide which features to compile `vs.fltk` with.
  - [x] Support for automatic fall through of files for components.
  - [ ] Linking external libraries and exposing symbols to `vs`
- [x] Documentation infrastructure!
  - [x] Landing page for the project
  - [x] `doxygen` for C++ code
  - [x] `mkdocs` to distribute the main documentation
- [ ] New documentation:
  - [ ] settings
  - [ ] paths/virtual paths
  - [ ] some basic information to use vs
