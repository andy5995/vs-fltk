- Starting with `#` they cannot be reached. If `node`,`container`,`slot` or `container-slot` they are just private namespaces with the relative restrictions. Using `#` in paths is usually not allowed, so there will be no way to access them legit.
- Any child of an element tagged as `leaf` will be placed in the nearest parent namespace available based on the XML tree structure. There is always one.

