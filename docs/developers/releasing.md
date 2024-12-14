## Todo list for releases

Ideally all these steps should be made automatic when a release request is triggered.  
But for now they are not, so use this as a checklist for reference:

- [ ] Make sure tests are all passing with the current commit (locally).
- [ ] Update the version number in `package.json` to the next **odd** revision.
- [ ] Clear content in `TODO.md`.
- [ ] Make sure `RELEASE.md` is up to date. Copy it in `./docs/releases/` using the version name as filename.
- [ ] Run the full codegen step.
- [ ] Commit & push.
- [ ] Double check CI are good.
- [ ] Prepare a tagged release on github. This will trigger the construction of all artifacts.
  - [ ] Packages
  - [ ] Docker images
  - [ ] Flatpak
  - [ ] Full source archive including subprojects to fix their version

After release:

- [ ] Update the version number in `meson.build` and `package.json` to the next **even** revision.
- [ ] Clear content in `RELEASE.md`
