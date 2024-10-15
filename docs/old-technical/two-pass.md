# Main rendering loop (to revised)

The UI must all be run in a single thread, which can slow down things.
For this reason rendering is split in two passes.

## First pass:
During the first pass all tags needing complex or long computation (loading files from the internet or compiling stuff) will preload data based on the respective caching policies. Each src or resource to handle can have its own thread, and results are collected on protected data structures. The original xml is updated with special `src-cached` attributes, so that they are used as references in place of the original source.  
Excluded are elements where a special tag is used to mark them not to be prefetched. And also `not module scripts` and `data` which are handled later.

## Rendering
UI is rendered. Resources which were marked not to be fetched earlier are handled stopping the main thread. `not module scripts` and `data` once met are added to a pool, and handled asynchronously. 

## Final join
After waiting for the second pool of threads to complete, all the missing things are added, events enabled, and the process is finished.

# Component rendering steps
- Allocate the new component
- Resolve mixin, data or script inside first, before any other children. They are not real DOM children.
- Apply all props (mixin+inline)
- Run on children (except for terminal widgets)