## v0.1.3-alpha

This second alpha release is still not usable in real-world scenarios, yet some simple and somewhat useful demos should be achievable.

The main objectives for this release are:

- To improve the build system moving it to a separate repo.
- To introduce a wider range of fltk widgets.
- Complete the revised code-generation of widgets
- Better serialization/de-serialization of XML props & revised getters/setters

### What's new?

This is what has been implemented so far (and some missing feature for context):

- [ ] Improvements to the build system

  - [ ] migration of distribution-related features to [external repo]()
  - [ ] fix linking to follow https://github.com/KaruroChori/vs-fltk/issues/63
  - [ ] debian and arch packages are now supported
  - [ ] install script can be now be safely used

- [ ] New widgets & improvements

- [ ] Updated code-gen

- [ ] Updated core features
  - [x] Removed most of the globals
  - [x] Reworked `ui_tree` interface to be more generic and not only viable for xml trees.
