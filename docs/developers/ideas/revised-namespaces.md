- Starting with `#` they cannot be reached via public APIs. If `node`,`container`,`slot` or `container-slot` they are just private namespaces with the relative restrictions. Being private, there is no way to reference those by name via public APIs.
- Any child of an element tagged as `leaf` will be placed in the nearest parent namespace available based on the XML tree structure. There is always one.

