## Roots

The top level object is marked as `app` and is effectively the application root.  
The loader in running mode will require an xml with such an entry point (the root cannot be a native object).
The tag `component` is used as the top level for any reusable component.

## Tag conventions

UI components are not scoped in a namespace, however by convention they are all written in lower letters.  
Custom user components should have the first letter as capital, or use some namespace like `hello:world`.

## Attribute conventions

Attributes can often be composed, like multiple parts making up the full attribute, at least logically.  
For example `box` can be split in `box.x`, `box.y`, `box.w` and `box.h`.  
If intersective, the more nested variant takes priority.  
So `box="0,0,0,0" box.x="10"` will be equivalent to `box="10,0,0,0"` regardless of order.

## Reserved names

- `app`
- `component`
- `use`
- `import`
- `script`
- `script.global`
- `data`
- `res` for externally loaded resources, later usable via its name symbol
- `namespace` to scope names within a component, limit upper and lower visibility, filter events and determine

Within each, some additional tags might be reserved. All the other names are free, except for widgets based on your distribution.  
As for attributes there are only a handful which are always reserved:

- `name`
- `groups`
- `on.*`: can only be used to define callbacks. Custom events are possible, but they have a specific semantics & side effects.
- `auth` and all its sub-arguments
- `src` and all its sub-arguments
- `body` and all its sub-arguments

There are few that, while not restricted, should only be used if following the same semantics:

- `box`
- `label`
- `visible`
- `enabled`
